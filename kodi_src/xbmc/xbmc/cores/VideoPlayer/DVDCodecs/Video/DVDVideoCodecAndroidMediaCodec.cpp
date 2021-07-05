/*
 *  Copyright (C) 2013-2018 Team Kodi
 *  This file is part of Kodi - https://kodi.tv
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSES/README.md for more information.
 */

// http://developer.android.com/reference/android/media/MediaCodec.html
//
// Android MediaCodec class can be used to access low-level media codec,
// i.e. encoder/decoder components. (android.media.MediaCodec). Requires SDK21+
//
#include "config_kodi.h"

#include "DVDVideoCodecAndroidMediaCodec.h"

#include <android/jni/ByteBuffer.h>
#include <android/jni/MediaCodecList.h>
#include <android/jni/MediaCodecInfo.h>
#include <android/jni/Surface.h>
#include <android/jni/SurfaceTexture.h>
#include <media/NdkMediaCrypto.h>

#include "Application.h"
#include "ServiceBroker.h"
#include "messaging/ApplicationMessenger.h"
#include "cores/VideoPlayer/Interface/Addon/TimingConstants.h"

#include "utils/BitstreamConverter.h"
#include "utils/BitstreamWriter.h"
#include "utils/CPUInfo.h"
#include "utils/log.h"

#include "DVDCodecs/DVDFactoryCodec.h"
#if defined(TARGET_ANDROID) && !defined(HAVE_QT_GUI)
# include "platform/android/activity/XBMCApp.h"
#elif defined(TARGET_ANDROID) && defined(HAVE_QT_GUI)
# include "platform/qt/KodiQtApp.h"
#endif // defined(TARGET_ANDROID) && !defined(HAVE_QT_GUI)

#include "cores/VideoPlayer/VideoRenderers/RenderManager.h"
#include "cores/VideoPlayer/VideoRenderers/RenderFlags.h"
#include "cores/VideoPlayer/Interface/Addon/DemuxCrypto.h"
#include "cores/VideoPlayer/Process/VideoBuffer.h"
#include "platform/android/activity/AndroidFeatures.h"
#include "platform/android/activity/JNIXBMCSurfaceTextureOnFrameAvailableListener.h"
#include "settings/AdvancedSettings.h"
#include "settings/Settings.h"
#include "settings/SettingsComponent.h"

#include "utils/TimeUtils.h"

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

#include <cassert>
#include <memory>

#define XMEDIAFORMAT_KEY_ROTATION "rotation-degrees"
#define XMEDIAFORMAT_KEY_SLICE "slice-height"
#define XMEDIAFORMAT_KEY_CROP_LEFT "crop-left"
#define XMEDIAFORMAT_KEY_CROP_RIGHT "crop-right"
#define XMEDIAFORMAT_KEY_CROP_TOP "crop-top"
#define XMEDIAFORMAT_KEY_CROP_BOTTOM "crop-bottom"
#define XMEDIAFORMAT_KEY_TUNNELED_PLAYBACK "feature-tunneled-playback"

using namespace KODI::MESSAGING;

enum MEDIACODEC_STATES
{
  MEDIACODEC_STATE_UNINITIALIZED,
  MEDIACODEC_STATE_CONFIGURED,
  MEDIACODEC_STATE_FLUSHED,
  MEDIACODEC_STATE_RUNNING,
  MEDIACODEC_STATE_ENDOFSTREAM,
  MEDIACODEC_STATE_ERROR,
  MEDIACODEC_STATE_STOPPED
};

static bool IsBlacklisted(const std::string &name)
{
  static const char *blacklisted_decoders[] = {
    // No software decoders
    "OMX.google",
    // For Rockchip non-standard components
    "AVCDecoder",
    "AVCDecoder_FLASH",
    "FLVDecoder",
    "M2VDecoder",
    "M4vH263Decoder",
    "RVDecoder",
    "VC1Decoder",
    "VPXDecoder",
    // End of Rockchip
    NULL
  };
  for (const char **ptr = blacklisted_decoders; *ptr; ptr++)
  {
    if (!strnicmp(*ptr, name.c_str(), strlen(*ptr)))
      return true;
  }
  return false;
}

static bool IsSupportedColorFormat(int color_format)
{
  static const int supported_colorformats[] = {
    CJNIMediaCodecInfoCodecCapabilities::COLOR_FormatYUV420Planar,
    CJNIMediaCodecInfoCodecCapabilities::COLOR_TI_FormatYUV420PackedSemiPlanar,
    CJNIMediaCodecInfoCodecCapabilities::COLOR_FormatYUV420SemiPlanar,
    CJNIMediaCodecInfoCodecCapabilities::COLOR_QCOM_FormatYUV420SemiPlanar,
    CJNIMediaCodecInfoCodecCapabilities::OMX_QCOM_COLOR_FormatYVU420SemiPlanarInterlace,
    -1
  };
  for (const int *ptr = supported_colorformats; *ptr != -1; ptr++)
  {
    if (color_format == *ptr)
      return true;
  }
  return false;
}

/*****************************************************************************/
/*****************************************************************************/
class CDVDMediaCodecOnFrameAvailable : public CEvent, public CJNIXBMCSurfaceTextureOnFrameAvailableListener
{
public:
  CDVDMediaCodecOnFrameAvailable(std::shared_ptr<CJNISurfaceTexture> &surfaceTexture)
    : CJNIXBMCSurfaceTextureOnFrameAvailableListener()
    , m_surfaceTexture(surfaceTexture)
  {
    m_surfaceTexture->setOnFrameAvailableListener(*this);
  }

  virtual ~CDVDMediaCodecOnFrameAvailable()
  {
    // unhook the callback
    CJNIXBMCSurfaceTextureOnFrameAvailableListener nullListener(jni::jhobject(NULL));
    m_surfaceTexture->setOnFrameAvailableListener(nullListener);
  }

protected:
  void onFrameAvailable(CJNISurfaceTexture)
  {
    Set();
  }

private:
  std::shared_ptr<CJNISurfaceTexture> m_surfaceTexture;
};

/*****************************************************************************/
/*****************************************************************************/
CMediaCodec::CMediaCodec(const char *name)
 : m_codec(AMediaCodec_createCodecByName(name))
{
}

CMediaCodec::~CMediaCodec()
{
  AMediaCodec_delete(m_codec);
};


/*****************************************************************************/
/*****************************************************************************/
void CMediaCodecVideoBuffer::Set(int bufferId, int textureId,
  std::shared_ptr<CJNISurfaceTexture> surfacetexture,
  std::shared_ptr<CDVDMediaCodecOnFrameAvailable> frameready,
  std::shared_ptr<CJNIXBMCVideoView> videoview)
{
  m_bufferId = bufferId;
  m_textureId = textureId;
  m_surfacetexture = surfacetexture;
  m_frameready = frameready;
  m_videoview = videoview;
}

bool CMediaCodecVideoBuffer::WaitForFrame(int millis)
{
  return m_frameready->WaitMSec(millis);
}

void CMediaCodecVideoBuffer::ReleaseOutputBuffer(bool render, int64_t displayTime, CMediaCodecVideoBufferPool* pool)
{
  std::shared_ptr<CMediaCodec> codec(static_cast<CMediaCodecVideoBufferPool*>(pool ? pool : m_pool.get())->GetMediaCodec());

  if (m_bufferId < 0 || !codec)
    return;

  // release OutputBuffer and render if indicated
  // then wait for rendered frame to become available.

  if (render)
    if (m_frameready)
      m_frameready->Reset();

  if (CServiceBroker::GetSettingsComponent()->GetAdvancedSettings()->CanLogComponent(LOGVIDEO))
  {
    int64_t diff = displayTime ? displayTime - CurrentHostCounter() : 0;
    CLog::Log(LOGDEBUG, "CMediaCodecVideoBuffer::ReleaseOutputBuffer index(%d), render(%d), time:%lld, offset:%lld", m_bufferId, render, displayTime, diff);
  }

  media_status_t mstat;
  if (!render || displayTime == 0)
    mstat = AMediaCodec_releaseOutputBuffer(codec->codec(), m_bufferId, render);
  else
    mstat = AMediaCodec_releaseOutputBufferAtTime(codec->codec(), m_bufferId, displayTime);
  m_bufferId = -1; //mark released

  if (mstat != AMEDIA_OK)
    CLog::Log(LOGERROR, "CMediaCodecVideoBuffer::ReleaseOutputBuffer error %d in render(%d)", mstat, render);
}

int CMediaCodecVideoBuffer::GetBufferId() const
{
  // since m_texture never changes,
  // we do not need a m_section lock here.
  return m_bufferId;
}

int CMediaCodecVideoBuffer::GetTextureId() const
{
  // since m_texture never changes,
  // we do not need a m_section lock here.
  return m_textureId;
}

void CMediaCodecVideoBuffer::GetTransformMatrix(float *textureMatrix)
{
  m_surfacetexture->getTransformMatrix(textureMatrix);
}

void CMediaCodecVideoBuffer::UpdateTexImage()
{
  // updateTexImage will check and spew any prior gl errors,
  // clear them before we call updateTexImage.
  glGetError();

  // this is key, after calling releaseOutputBuffer, we must
  // wait a little for MediaCodec to render to the surface.
  // Then we can updateTexImage without delay. If we do not
  // wait, then video playback gets jerky. To optimize this,
  // we hook the SurfaceTexture OnFrameAvailable callback
  // using CJNISurfaceTextureOnFrameAvailableListener and wait
  // on a CEvent to fire. 50ms seems to be a good max fallback.
  WaitForFrame(50);

  m_surfacetexture->updateTexImage();
  if (xbmc_jnienv()->ExceptionCheck())
  {
    CLog::Log(LOGERROR, "CMediaCodecVideoBuffer::UpdateTexImage updateTexImage:ExceptionCheck");
    xbmc_jnienv()->ExceptionDescribe();
    xbmc_jnienv()->ExceptionClear();
  }

  if (xbmc_jnienv()->ExceptionCheck())
  {
    CLog::Log(LOGERROR, "CMediaCodecVideoBuffer::UpdateTexImage getTimestamp:ExceptionCheck");
    xbmc_jnienv()->ExceptionDescribe();
    xbmc_jnienv()->ExceptionClear();
  }
}

void CMediaCodecVideoBuffer::RenderUpdate(const CRect &DestRect, int64_t displayTime)
{
  CRect surfRect = m_videoview->getSurfaceRect();
  if (DestRect != surfRect)
  {
    CRect adjRect = CXBMCApp::MapRenderToDroid(DestRect);
    if (adjRect != surfRect)
    {
      m_videoview->setSurfaceRect(adjRect);
      CLog::Log(LOGDEBUG, LOGVIDEO, "CMediaCodecVideoBuffer::RenderUpdate: Dest - %f+%f-%fx%f", DestRect.x1, DestRect.y1, DestRect.Width(), DestRect.Height());
      CLog::Log(LOGDEBUG, LOGVIDEO, "CMediaCodecVideoBuffer::RenderUpdate: Adj  - %f+%f-%fx%f", adjRect.x1, adjRect.y1, adjRect.Width(), adjRect.Height());

      // setVideoViewSurfaceRect is async, so skip rendering this frame
      ReleaseOutputBuffer(false, 0);
    }
    else
      ReleaseOutputBuffer(true, displayTime);
  }
  else
    ReleaseOutputBuffer(true, displayTime);
}

/*****************************************************************************/
/*****************************************************************************/
CMediaCodecVideoBufferPool::~CMediaCodecVideoBufferPool()
{
  CLog::Log(LOGDEBUG, "CMediaCodecVideoBufferPool::~CMediaCodecVideoBufferPool Releasing %u buffers", static_cast<unsigned int>(m_videoBuffers.size()));
  for (auto buffer : m_videoBuffers)
    delete buffer;
}

CVideoBuffer* CMediaCodecVideoBufferPool::Get()
{
  CSingleLock lock(m_criticalSection);

  if (m_freeBuffers.empty())
  {
    m_freeBuffers.push_back(m_videoBuffers.size());
    m_videoBuffers.push_back(new CMediaCodecVideoBuffer(static_cast<int>(m_videoBuffers.size())));
  }
  int bufferIdx(m_freeBuffers.back());
  m_freeBuffers.pop_back();

  m_videoBuffers[bufferIdx]->Acquire(shared_from_this());

  return m_videoBuffers[bufferIdx];
}

void CMediaCodecVideoBufferPool::Return(int id)
{
  CSingleLock lock(m_criticalSection);
  m_videoBuffers[id]->ReleaseOutputBuffer(false, 0, this);
  m_freeBuffers.push_back(id);
}

std::shared_ptr<CMediaCodec> CMediaCodecVideoBufferPool::GetMediaCodec()
{
  CSingleLock lock(m_criticalSection);
  return m_codec;
}

void CMediaCodecVideoBufferPool::ResetMediaCodec()
{
  CSingleLock lock(m_criticalSection);
  m_codec = nullptr;
}


/*****************************************************************************/
/*****************************************************************************/
CDVDVideoCodecAndroidMediaCodec::CDVDVideoCodecAndroidMediaCodec(CProcessInfo &processInfo, bool surface_render)
: CDVDVideoCodec(processInfo)
, m_formatname("mediacodec")
, m_opened(false)
, m_jnivideoview(nullptr)
, m_jnisurface(nullptr)
, m_crypto(nullptr)
, m_textureId(0)
, m_surface(nullptr)
, m_OutputDuration(0)
, m_fpsDuration(0)
, m_lastPTS(-1)
, m_dtsShift(DVD_NOPTS_VALUE)
, m_bitstream(nullptr)
, m_render_surface(surface_render)
, m_mpeg2_sequence(nullptr)
, m_useDTSforPTS(false)
{
  m_videobuffer.Reset();
}

CDVDVideoCodecAndroidMediaCodec::~CDVDVideoCodecAndroidMediaCodec()
{
  Dispose();

  if (m_crypto)
  {
    AMediaCrypto_delete(m_crypto);
    m_crypto = nullptr;
  }
  if (m_mpeg2_sequence)
  {
    delete (m_mpeg2_sequence);
    m_mpeg2_sequence = nullptr;
  }
}

CDVDVideoCodec* CDVDVideoCodecAndroidMediaCodec::Create(CProcessInfo &processInfo)
{
  return new CDVDVideoCodecAndroidMediaCodec(processInfo);
}

bool CDVDVideoCodecAndroidMediaCodec::Register()
{
  CDVDFactoryCodec::RegisterHWVideoCodec("mediacodec_dec", &CDVDVideoCodecAndroidMediaCodec::Create);
  return true;
}

std::atomic<bool> CDVDVideoCodecAndroidMediaCodec::m_InstanceGuard(false);

bool CDVDVideoCodecAndroidMediaCodec::Open(CDVDStreamInfo &hints, CDVDCodecOptions &options)
{
  int num_codecs;
  bool needSecureDecoder(false);
  int profile(0);

  m_opened = false;
  // allow only 1 instance here
  if (m_InstanceGuard.exchange(true))
  {
    CLog::Log(LOGERROR, "CDVDVideoCodecAndroidMediaCodec::Open - InstanceGuard locked\n");
    return false;
  }

  // mediacodec crashes with null size. Trap this...
  if (!hints.width || !hints.height)
  {
    CLog::Log(LOGERROR, "CDVDVideoCodecAndroidMediaCodec::Open - %s\n", "null size, cannot handle");
    goto FAIL;
  }
  else if (hints.stills)
  {
    // Won't work reliably
    goto FAIL;
  }
  else if (hints.orientation && m_render_surface && CJNIBase::GetSDKVersion() < 23)
  {
    CLog::Log(LOGERROR, "CDVDVideoCodecAndroidMediaCodec::Open - %s\n", "Surface does not support orientation before API 23");
    goto FAIL;
  }
  else if (!CServiceBroker::GetSettingsComponent()->GetSettings()->GetBool(CSettings::SETTING_VIDEOPLAYER_USEMEDIACODEC) &&
           !CServiceBroker::GetSettingsComponent()->GetSettings()->GetBool(CSettings::SETTING_VIDEOPLAYER_USEMEDIACODECSURFACE))
    goto FAIL;

  m_render_surface = CServiceBroker::GetSettingsComponent()->GetSettings()->GetBool(CSettings::SETTING_VIDEOPLAYER_USEMEDIACODECSURFACE);
  m_state = MEDIACODEC_STATE_UNINITIALIZED;
  m_noPictureLoop = 0;
  m_codecControlFlags = 0;
  m_hints = hints;
  m_indexInputBuffer = -1;
  m_dtsShift = DVD_NOPTS_VALUE;
  m_useDTSforPTS = false;

  CLog::Log(LOGDEBUG, LOGVIDEO, "CDVDVideoCodecAndroidMediaCodec::Open hints: fpsrate %d / fpsscale %d\n", m_hints.fpsrate, m_hints.fpsscale);
  CLog::Log(LOGDEBUG, LOGVIDEO, "CDVDVideoCodecAndroidMediaCodec::Open hints: CodecID %d \n", m_hints.codec);
  CLog::Log(LOGDEBUG, LOGVIDEO, "CDVDVideoCodecAndroidMediaCodec::Open hints: StreamType %d \n", m_hints.type);
  CLog::Log(LOGDEBUG, LOGVIDEO, "CDVDVideoCodecAndroidMediaCodec::Open hints: Level %d \n", m_hints.level);
  CLog::Log(LOGDEBUG, LOGVIDEO, "CDVDVideoCodecAndroidMediaCodec::Open hints: Profile %d \n", m_hints.profile);
  CLog::Log(LOGDEBUG, LOGVIDEO, "CDVDVideoCodecAndroidMediaCodec::Open hints: PTS_invalid %d \n", m_hints.ptsinvalid);
  CLog::Log(LOGDEBUG, LOGVIDEO, "CDVDVideoCodecAndroidMediaCodec::Open hints: Tag %d \n", m_hints.codec_tag);
  CLog::Log(LOGDEBUG, LOGVIDEO, "CDVDVideoCodecAndroidMediaCodec::Open hints: %dx%d \n", m_hints.width,  m_hints.height);

  switch(m_hints.codec)
  {
    case AV_CODEC_ID_MPEG2VIDEO:
      m_mime = "video/mpeg2";
      m_mpeg2_sequence = new mpeg2_sequence;
      m_mpeg2_sequence->width  = m_hints.width;
      m_mpeg2_sequence->height = m_hints.height;
      m_mpeg2_sequence->ratio  = m_hints.aspect;
      m_mpeg2_sequence->fps_scale = m_hints.fpsscale;
      m_mpeg2_sequence->fps_rate = m_hints.fpsrate;
      m_useDTSforPTS = true;
      m_formatname = "amc-mpeg2";
      break;
    case AV_CODEC_ID_MPEG4:
      m_mime = "video/mp4v-es";
      m_formatname = "amc-mpeg4";
      m_useDTSforPTS = true;
      break;
    case AV_CODEC_ID_H263:
      m_mime = "video/3gpp";
      m_formatname = "amc-h263";
      break;
    case AV_CODEC_ID_VP6:
    case AV_CODEC_ID_VP6F:
      m_mime = "video/x-vnd.on2.vp6";
      m_formatname = "amc-vp6";
      break;
    case AV_CODEC_ID_VP8:
      m_mime = "video/x-vnd.on2.vp8";
      m_formatname = "amc-vp8";
      break;
    case AV_CODEC_ID_VP9:
      switch(hints.profile)
      {
        case FF_PROFILE_VP9_0:
          profile = CJNIMediaCodecInfoCodecProfileLevel::VP9Profile0;
          break;
        case FF_PROFILE_VP9_1:
          profile = CJNIMediaCodecInfoCodecProfileLevel::VP9Profile1;
          break;
        case FF_PROFILE_VP9_2:
          profile = CJNIMediaCodecInfoCodecProfileLevel::VP9Profile2;
          break;
        case FF_PROFILE_VP9_3:
          profile = CJNIMediaCodecInfoCodecProfileLevel::VP9Profile3;
          break;
        default:;
      }
      m_mime = "video/x-vnd.on2.vp9";
      m_formatname = "amc-vp9";
      free(m_hints.extradata);
      m_hints.extradata = nullptr;
      m_hints.extrasize = 0;
      break;
    case AV_CODEC_ID_AVS:
    case AV_CODEC_ID_CAVS:
    case AV_CODEC_ID_H264:
      switch(hints.profile)
      {
        case FF_PROFILE_H264_HIGH_10:
          profile = CJNIMediaCodecInfoCodecProfileLevel::AVCProfileHigh10;
          break;
        case FF_PROFILE_H264_HIGH_10_INTRA:
          // No known h/w decoder supporting Hi10P
          goto FAIL;
      }
      m_mime = "video/avc";
      m_formatname = "amc-h264";
      // check for h264-avcC and convert to h264-annex-b
      if (m_hints.extradata && !m_hints.cryptoSession)
      {
        m_bitstream = new CBitstreamConverter;
        if (!m_bitstream->Open(m_hints.codec, (uint8_t*)m_hints.extradata, m_hints.extrasize, true))
        {
          SAFE_DELETE(m_bitstream);
        }
      }
      break;
    case AV_CODEC_ID_HEVC:
      m_mime = "video/hevc";
      m_formatname = "amc-h265";
      // check for hevc-hvcC and convert to h265-annex-b
      if (m_hints.extradata && !m_hints.cryptoSession)
      {
        m_bitstream = new CBitstreamConverter;
        if (!m_bitstream->Open(m_hints.codec, (uint8_t*)m_hints.extradata, m_hints.extrasize, true))
        {
          SAFE_DELETE(m_bitstream);
        }
      }
      break;
    case AV_CODEC_ID_WMV3:
      if (m_hints.extrasize == 4 || m_hints.extrasize == 5)
      {
        // Convert to SMPTE 421M-2006 Annex-L
        static uint8_t annexL_hdr1[] = {0x8e, 0x01, 0x00, 0xc5, 0x04, 0x00, 0x00, 0x00};
        static uint8_t annexL_hdr2[] = {0x0c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
        free(m_hints.extradata);
        m_hints.extrasize = 36;
        m_hints.extradata = malloc(m_hints.extrasize);

        unsigned int offset = 0;
        char buf[4];
        memcpy(m_hints.extradata, annexL_hdr1, sizeof(annexL_hdr1));
        offset += sizeof(annexL_hdr1);
        memcpy(&((char *)(m_hints.extradata))[offset], hints.extradata, 4);
        offset += 4;
        BS_WL32(buf, hints.height);
        memcpy(&((char *)(m_hints.extradata))[offset], buf, 4);
        offset += 4;
        BS_WL32(buf, hints.width);
        memcpy(&((char *)(m_hints.extradata))[offset], buf, 4);
        offset += 4;
        memcpy(&((char *)(m_hints.extradata))[offset], annexL_hdr2, sizeof(annexL_hdr2));
      }

      m_mime = "video/x-ms-wmv";
      m_formatname = "amc-wmv";
      break;
    case AV_CODEC_ID_VC1:
    {
      if (m_hints.extrasize < 16)
        goto FAIL;

      // Reduce extradata to first SEQ header
      unsigned int seq_offset = 0;
      for (; seq_offset <= m_hints.extrasize-4; ++seq_offset)
      {
        char *ptr = &((char*)m_hints.extradata)[seq_offset];
        if (ptr[0] == 0x00 && ptr[1] == 0x00 && ptr[2] == 0x01 && ptr[3] == 0x0f)
          break;
      }
      if (seq_offset > m_hints.extrasize-4)
        goto FAIL;

      if (seq_offset)
      {
        free(m_hints.extradata);
        m_hints.extrasize -= seq_offset;
        m_hints.extradata = malloc(m_hints.extrasize);
        memcpy(m_hints.extradata, &((char *)(hints.extradata))[seq_offset], m_hints.extrasize);
      }

      m_mime = "video/wvc1";
      m_formatname = "amc-vc1";
      break;
    }
    default:
      CLog::Log(LOGDEBUG, "CDVDVideoCodecAndroidMediaCodec::Open Unknown hints.codec(%d)", hints.codec);
      goto FAIL;
      break;
  }

  if (m_crypto)
  {
    AMediaCrypto_delete(m_crypto);
    m_crypto = nullptr;
  }

  if (m_hints.cryptoSession)
  {
    CLog::Log(LOGDEBUG, "CDVDVideoCodecAndroidMediaCodec::Open Initializing MediaCrypto");

    const AMediaUUID *uuid(nullptr);
    const AMediaUUID wvuuid = {0xED,0xEF,0x8B,0xA9,0x79,0xD6,0x4A,0xCE,0xA3,0xC8,0x27,0xDC,0xD5,0x1D,0x21,0xED};
    const AMediaUUID pruuid = {0x9A,0x04,0xF0,0x79,0x98,0x40,0x42,0x86,0xAB,0x92,0xE6,0x5B,0xE0,0x88,0x5F,0x95};

    if (m_hints.cryptoSession->keySystem == CRYPTO_SESSION_SYSTEM_WIDEVINE)
      uuid = &wvuuid;
    else if (m_hints.cryptoSession->keySystem == CRYPTO_SESSION_SYSTEM_PLAYREADY)
      uuid = &pruuid;
    else
    {
      CLog::Log(LOGERROR, "CDVDVideoCodecAndroidMediaCodec::Open Unsupported crypto-keysystem %u", m_hints.cryptoSession->keySystem);
      goto FAIL;
    }

    m_crypto = AMediaCrypto_new(*uuid, m_hints.cryptoSession->sessionId, m_hints.cryptoSession->sessionIdSize);

    if (!m_crypto)
    {
      CLog::Log(LOGERROR, "CDVDVideoCodecAndroidMediaCodec::Open: MediaCrypto creation failed");
      goto FAIL;
    }
    needSecureDecoder = AMediaCrypto_requiresSecureDecoderComponent(m_mime.c_str()) && (m_hints.cryptoSession->flags & DemuxCryptoSession::FLAG_SECURE_DECODER) != 0;
  }

  if (m_render_surface)
  {
    m_jnivideoview.reset(CJNIXBMCVideoView::createVideoView(this));
    if (!m_jnivideoview || !m_jnivideoview->waitForSurface(2000))
    {
      CLog::Log(LOGERROR, "CDVDVideoCodecAndroidMediaCodec: VideoView creation failed!!");
      goto FAIL;
    }
  }

  m_codec = nullptr;
  m_colorFormat = -1;
  num_codecs = CJNIMediaCodecList::getCodecCount();

  for (int i = 0; i < num_codecs; i++)
  {
    CJNIMediaCodecInfo codec_info = CJNIMediaCodecList::getCodecInfoAt(i);
    if (codec_info.isEncoder())
      continue;

    m_codecname = codec_info.getName();
    if (IsBlacklisted(m_codecname))
      continue;

    CLog::Log(LOGNOTICE, "CDVDVideoCodecAndroidMediaCodec::Open Testing codec:%s", m_codecname.c_str());

    bool codecIsSecure(m_codecname.find(".secure") != std::string::npos);
    if (needSecureDecoder)
    {
      if (!codecIsSecure)
        m_codecname += ".secure";
    }
    else if (codecIsSecure)
      continue;

    CJNIMediaCodecInfoCodecCapabilities codec_caps = codec_info.getCapabilitiesForType(m_mime);
    if (xbmc_jnienv()->ExceptionCheck())
    {
      // Unsupported type?
      xbmc_jnienv()->ExceptionClear();
      continue;
    }

    std::vector<int> color_formats = codec_caps.colorFormats();

    if (profile)
    {
      std::vector<CJNIMediaCodecInfoCodecProfileLevel> profileLevels = codec_caps.profileLevels();
      if (std::find_if(profileLevels.cbegin(), profileLevels.cend(),
        [&](const CJNIMediaCodecInfoCodecProfileLevel profileLevel){ return profileLevel.profile() == profile; }) == profileLevels.cend())
      {
        CLog::Log(LOGERROR, "CDVDVideoCodecAndroidMediaCodec::Open: profile not supported: %d", profile);
        continue;
      }
    }

    std::vector<std::string> types = codec_info.getSupportedTypes();
    // return the 1st one we find, that one is typically 'the best'
    for (size_t j = 0; j < types.size(); ++j)
    {
      if (types[j] == m_mime)
      {
        m_codec = std::shared_ptr<CMediaCodec>(new CMediaCodec(m_codecname.c_str()));
        if (!m_codec->codec())
        {
          CLog::Log(LOGERROR, "CDVDVideoCodecAndroidMediaCodec::Open cannot create codec");
          continue;
        }

        for (size_t k = 0; k < color_formats.size(); ++k)
        {
          CLog::Log(LOGDEBUG, "CDVDVideoCodecAndroidMediaCodec::Open "
            "m_codecname(%s), colorFormat(%d)", m_codecname.c_str(), color_formats[k]);
          if (IsSupportedColorFormat(color_formats[k]))
            m_colorFormat = color_formats[k]; // Save color format for initial output configuration
        }
        break;
      }
    }
    if (m_codec->codec())
      break;
  }
  if (!m_codec)
  {
    CLog::Log(LOGERROR, "CDVDVideoCodecAndroidMediaCodec:: Failed to create Android MediaCodec");
    goto FAIL;
  }

  // setup a YUV420P VideoPicture buffer.
  // first make sure all properties are reset.
  m_videobuffer.Reset();

  m_videobuffer.iWidth  = m_hints.width;
  m_videobuffer.iHeight = m_hints.height;
  // these will get reset to crop values later
  m_videobuffer.iDisplayWidth  = m_hints.width;
  m_videobuffer.iDisplayHeight = m_hints.height;

  if (!ConfigureMediaCodec())
    goto FAIL;

  if (m_codecname.find("OMX.Nvidia", 0, 10) == 0)
    m_invalidPTSValue = AV_NOPTS_VALUE;
  else
    m_invalidPTSValue = 0;

  CLog::Log(LOGINFO, "CDVDVideoCodecAndroidMediaCodec:: "
    "Open Android MediaCodec %s", m_codecname.c_str());

  m_opened = true;

  m_processInfo.SetVideoDecoderName(m_formatname, true );
  m_processInfo.SetVideoPixelFormat(m_render_surface ? "Surface" : "EGL");
  m_processInfo.SetVideoDimensions(m_hints.width, m_hints.height);
  m_processInfo.SetVideoDeintMethod("hardware");
  m_processInfo.SetVideoDAR(m_hints.aspect);

  m_videoBufferPool = std::shared_ptr<CMediaCodecVideoBufferPool>(new CMediaCodecVideoBufferPool(m_codec));

  UpdateFpsDuration();

  return true;

FAIL:
  m_InstanceGuard.exchange(false);
  if (m_crypto)
  {
    AMediaCrypto_delete(m_crypto);
    m_crypto = nullptr;
  }

  if (m_jnivideoview)
  {
    m_jnivideoview->release();
    m_jnivideoview.reset();
  }

  m_codec = nullptr;

  SAFE_DELETE(m_bitstream);

  return false;
}

void CDVDVideoCodecAndroidMediaCodec::Dispose()
{
  if (!m_opened)
    return;

  // invalidate any inflight outputbuffers
  FlushInternal();

  if (m_videoBufferPool)
  {
    m_videoBufferPool->ResetMediaCodec();
    m_videoBufferPool = nullptr;
  }

  m_videobuffer.iFlags = 0;

  if (m_codec->codec())
  {
    AMediaCodec_stop(m_codec->codec());
    m_codec = nullptr;
    m_state = MEDIACODEC_STATE_STOPPED;
  }
  ReleaseSurfaceTexture();

  if(m_surface)
    ANativeWindow_release(m_surface);
  m_surface = nullptr;

  m_InstanceGuard.exchange(false);
  if (m_render_surface)
  {
    m_jnivideoview->release();
    m_jnivideoview.reset();
  }

  SAFE_DELETE(m_bitstream);

  m_opened = false;
}

bool CDVDVideoCodecAndroidMediaCodec::AddData(const DemuxPacket &packet)
{
  if (!m_opened || m_state == MEDIACODEC_STATE_STOPPED)
    return false;

  double pts(packet.pts), dts(packet.dts);

  if (CServiceBroker::GetSettingsComponent()->GetAdvancedSettings()->CanLogComponent(LOGVIDEO))
    CLog::Log(LOGDEBUG, "CDVDVideoCodecAndroidMediaCodec::AddData dts:%0.2lf pts:%0.2lf sz:%d indexBuffer:%d current state (%d)", dts, pts, packet.iSize, m_indexInputBuffer, m_state);
  else if (m_state != MEDIACODEC_STATE_RUNNING)
    CLog::Log(LOGDEBUG, "CDVDVideoCodecAndroidMediaCodec::AddData current state (%d)", m_state);

  if (m_hints.ptsinvalid)
    pts = DVD_NOPTS_VALUE;

  uint8_t *pData(packet.pData);
  size_t iSize(packet.iSize);

  if (m_state == MEDIACODEC_STATE_ENDOFSTREAM)
  {
    // We received a packet but already reached EOS. Flush...
    FlushInternal();
    AMediaCodec_flush(m_codec->codec());
    m_state = MEDIACODEC_STATE_FLUSHED;
  }

  if (pData && iSize)
  {
    if (m_indexInputBuffer >= 0)
    {
      if (m_state == MEDIACODEC_STATE_FLUSHED)
        m_state = MEDIACODEC_STATE_RUNNING;
      if (!(m_state == MEDIACODEC_STATE_FLUSHED || m_state == MEDIACODEC_STATE_RUNNING))
        CLog::Log(LOGERROR, "CDVDVideoCodecAndroidMediaCodec::AddData: Wrong state (%d)", m_state);

      if (m_mpeg2_sequence && CBitstreamConverter::mpeg2_sequence_header(pData, iSize, m_mpeg2_sequence))
      {
        m_hints.fpsrate = m_mpeg2_sequence->fps_rate;
        m_hints.fpsscale = m_mpeg2_sequence->fps_scale;
        m_hints.width    = m_mpeg2_sequence->width;
        m_hints.height   = m_mpeg2_sequence->height;
        m_hints.aspect   = m_mpeg2_sequence->ratio;

        m_processInfo.SetVideoDAR(m_hints.aspect);
        UpdateFpsDuration();
      }

      // we have an input buffer, fill it.
      if (pData && m_bitstream)
      {
        m_bitstream->Convert(pData, iSize);
        iSize = m_bitstream->GetConvertSize();
        pData = m_bitstream->GetConvertBuffer();
      }
      size_t out_size;
      uint8_t* dst_ptr = AMediaCodec_getInputBuffer(m_codec->codec(), m_indexInputBuffer, &out_size);
      if ((size_t)iSize > out_size)
      {
        CLog::Log(LOGERROR, "CDVDVideoCodecAndroidMediaCodec::Decode, iSize(%d) > size(%d)", iSize, out_size);
        iSize = out_size;
      }

      AMediaCodecCryptoInfo *cryptoInfo(0);
      if (m_crypto && packet.cryptoInfo)
      {
        std::vector<size_t> clearBytes(packet.cryptoInfo->clearBytes, packet.cryptoInfo->clearBytes + packet.cryptoInfo->numSubSamples);
        std::vector<size_t> cipherBytes(packet.cryptoInfo->cipherBytes, packet.cryptoInfo->cipherBytes + packet.cryptoInfo->numSubSamples);

        cryptoInfo = AMediaCodecCryptoInfo_new(
          packet.cryptoInfo->numSubSamples,
          packet.cryptoInfo->kid,
          packet.cryptoInfo->iv,
          AMEDIACODECRYPTOINFO_MODE_AES_CTR,
          &clearBytes[0], &cipherBytes[0]);
      }
      if (dst_ptr)
      {
        // Codec specifics
        switch(m_hints.codec)
        {
          case AV_CODEC_ID_VC1:
          {
            if (iSize >= 4 && pData[0] == 0x00 && pData[1] == 0x00 && pData[2] == 0x01 && (pData[3] == 0x0d || pData[3] == 0x0f))
              memcpy(dst_ptr, pData, iSize);
            else
            {
              dst_ptr[0] = 0x00;
              dst_ptr[1] = 0x00;
              dst_ptr[2] = 0x01;
              dst_ptr[3] = 0x0d;
              memcpy(dst_ptr+4, pData, iSize);
              iSize += 4;
            }

            break;
          }

          default:
            memcpy(dst_ptr, pData, iSize);
            break;
        }
      }


      // Translate from VideoPlayer dts/pts to MediaCodec pts,
      // pts WILL get re-ordered by MediaCodec if needed.
      // Do not try to pass pts as a unioned double/int64_t,
      // some android devices will diddle with presentationTimeUs
      // and you will get NaN back and VideoPlayerVideo will barf.
      if (m_dtsShift == DVD_NOPTS_VALUE)
        m_dtsShift = (dts == DVD_NOPTS_VALUE) ? 0 : dts;

      int64_t presentationTimeUs = m_invalidPTSValue;
      if (pts != DVD_NOPTS_VALUE)
      {
        presentationTimeUs = (pts - m_dtsShift);
        m_useDTSforPTS = false;
      }
      else if (m_useDTSforPTS && dts != DVD_NOPTS_VALUE)
        presentationTimeUs = (dts - m_dtsShift);

      int flags = 0;
      int offset = 0;

      media_status_t mstat;
      if (!cryptoInfo)
        mstat = AMediaCodec_queueInputBuffer(m_codec->codec(), m_indexInputBuffer, offset, iSize, presentationTimeUs, flags);
      else
      {
        mstat = AMediaCodec_queueSecureInputBuffer(m_codec->codec(), m_indexInputBuffer, offset, cryptoInfo, presentationTimeUs, flags);
        AMediaCodecCryptoInfo_delete(cryptoInfo);
      }
      if (mstat != AMEDIA_OK)
      {
        CLog::Log(LOGERROR, "CDVDVideoCodecAndroidMediaCodec::AddData error(%d)", mstat);
        return false;
      }
      m_indexInputBuffer = -1;
    }
    else
      return false;
  }
  return true;
}

void CDVDVideoCodecAndroidMediaCodec::Reset()
{
  if (!m_opened)
    return;

  if (m_codec->codec())
  {
    // flush all outputbuffers inflight, they will
    // become invalid on m_codec->flush and generate
    // a spew of java exceptions if used
    FlushInternal();

    // now we can flush the actual MediaCodec object
    CLog::Log(LOGDEBUG, "CDVDVideoCodecAndroidMediaCodec::Reset Current state (%d)", m_state);
    m_state = MEDIACODEC_STATE_FLUSHED;
    AMediaCodec_flush(m_codec->codec());

    // Invalidate our local VideoPicture bits
    m_videobuffer.pts = DVD_NOPTS_VALUE;

    m_indexInputBuffer = -1;
  }
}

bool CDVDVideoCodecAndroidMediaCodec::Reconfigure(CDVDStreamInfo &hints)
{
  CLog::Log(LOGDEBUG, "CDVDVideoCodecAndroidMediaCodec::Reconfigure called");
  return false;
}

CDVDVideoCodec::VCReturn CDVDVideoCodecAndroidMediaCodec::GetPicture(VideoPicture* pVideoPicture)
{
  if (!m_opened)
    return VC_NONE;

  if (m_OutputDuration < m_fpsDuration || (m_codecControlFlags & DVD_CODEC_CTRL_DRAIN)!=0)
  {
    m_videobuffer.videoBuffer = pVideoPicture->videoBuffer;

    int retgp = GetOutputPicture();

    if (retgp > 0)
    {
      m_noPictureLoop = 0;

      pVideoPicture->videoBuffer = nullptr;
      pVideoPicture->SetParams(m_videobuffer);
      pVideoPicture->videoBuffer = m_videobuffer.videoBuffer;

      CLog::Log(LOGDEBUG, LOGVIDEO, "CDVDVideoCodecAndroidMediaCodec::GetPicture index: %d, pts:%0.4lf",
          static_cast<CMediaCodecVideoBuffer*>(m_videobuffer.videoBuffer)->GetBufferId(), pVideoPicture->pts);

      m_videobuffer.videoBuffer = nullptr;

      return VC_PICTURE;
    }
    else
    {
      m_videobuffer.videoBuffer = nullptr;
      if (retgp == -1 || ((m_codecControlFlags & DVD_CODEC_CTRL_DRAIN)!=0 && ++m_noPictureLoop == 10))  // EOS
      {
        m_state = MEDIACODEC_STATE_ENDOFSTREAM;
        m_noPictureLoop = 0;
        return VC_EOF;
      }
    }
  }
  else
    m_OutputDuration = 0;

  if ((m_codecControlFlags & DVD_CODEC_CTRL_DRAIN) == 0)
  {
    // try to fetch an input buffer
    if (m_indexInputBuffer < 0)
      m_indexInputBuffer = AMediaCodec_dequeueInputBuffer(m_codec->codec(), 5000 /*timout*/);

    if (m_indexInputBuffer >= 0)
    {
      CLog::Log(LOGDEBUG, LOGVIDEO, "CDVDVideoCodecAndroidMediaCodec::GetPicture VC_BUFFER");
      return VC_BUFFER;
    }
  }
  return VC_NONE;
}

void CDVDVideoCodecAndroidMediaCodec::SetCodecControl(int flags)
{
  if (m_codecControlFlags != flags)
  {
    CLog::Log(LOGDEBUG, LOGVIDEO, "%s %x->%x",  __func__, m_codecControlFlags, flags);
    m_codecControlFlags = flags;

    if (m_codecControlFlags & DVD_CODEC_CTRL_DROP)
      m_videobuffer.iFlags |= DVP_FLAG_DROPPED;
    else
      m_videobuffer.iFlags &= ~DVP_FLAG_DROPPED;
  }
}

unsigned CDVDVideoCodecAndroidMediaCodec::GetAllowedReferences()
{
  return 4;
}

void CDVDVideoCodecAndroidMediaCodec::FlushInternal()
{
  // invalidate any existing inflight buffers and create
  // new ones to match the number of output buffers
  if (m_indexInputBuffer >=0 && CJNIBase::GetSDKVersion() >= 26)
    AMediaCodec_queueInputBuffer(m_codec->codec(), m_indexInputBuffer, 0, 0, 0, AMEDIACODEC_BUFFER_FLAG_END_OF_STREAM);

  m_OutputDuration = 0;
  m_lastPTS = -1;
  m_dtsShift = DVD_NOPTS_VALUE;
}

bool CDVDVideoCodecAndroidMediaCodec::ConfigureMediaCodec(void)
{
  // setup a MediaFormat to match the video content,
  // used by codec during configure
  AMediaFormat* mediaformat = AMediaFormat_new();
  AMediaFormat_setString(mediaformat, AMEDIAFORMAT_KEY_MIME, m_mime.c_str());
  AMediaFormat_setInt32(mediaformat, AMEDIAFORMAT_KEY_WIDTH, m_hints.width);
  AMediaFormat_setInt32(mediaformat, AMEDIAFORMAT_KEY_HEIGHT, m_hints.height);
  AMediaFormat_setInt32(mediaformat, AMEDIAFORMAT_KEY_MAX_INPUT_SIZE, 0);

  if (CJNIBase::GetSDKVersion() >= 23 && m_render_surface)
  {
    // Handle rotation
    AMediaFormat_setInt32(mediaformat, XMEDIAFORMAT_KEY_ROTATION, m_hints.orientation);
    AMediaFormat_setInt32(mediaformat, XMEDIAFORMAT_KEY_TUNNELED_PLAYBACK, 0);
  }


  // handle codec extradata
  if (m_hints.extrasize)
  {
    size_t size = m_hints.extrasize;
    void  *src_ptr = m_hints.extradata;
    if (m_bitstream)
    {
      size = m_bitstream->GetExtraSize();
      src_ptr = m_bitstream->GetExtraData();
    }

    AMediaFormat_setBuffer(mediaformat, "csd-0", src_ptr, size);
  }

  if (m_render_surface)
  {
    m_jnivideosurface = m_jnivideoview->getSurface();
    if (!m_jnivideosurface)
    {
      CLog::Log(LOGERROR, "CDVDVideoCodecAndroidMediaCodec: VideoView getSurface failed!!");
      m_jnivideoview->release();
      m_jnivideoview.reset();
      return false;
    }
    m_surface = ANativeWindow_fromSurface(xbmc_jnienv(), m_jnivideosurface.get_raw());

    m_formatname += "(S)";
  }
  else
      InitSurfaceTexture();

  // configure and start the codec.
  // use the MediaFormat that we have setup.
  // use a null MediaCrypto, our content is not encrypted.

  int flags = 0;
  media_status_t mstat = AMediaCodec_configure(m_codec->codec(), mediaformat, m_surface, m_crypto, flags);

  if (mstat != AMEDIA_OK)
  {
    CLog::Log(LOGERROR, "CDVDVideoCodecAndroidMediaCodec configure error: %d", mstat);
    return false;
  }
  m_state = MEDIACODEC_STATE_CONFIGURED;

  mstat = AMediaCodec_start(m_codec->codec());
  if (mstat != AMEDIA_OK)
  {
    CLog::Log(LOGERROR, "CDVDVideoCodecAndroidMediaCodec start error: %d", mstat);
    return false;
  }
  m_state = MEDIACODEC_STATE_FLUSHED;

  // There is no guarantee we'll get an INFO_OUTPUT_FORMAT_CHANGED (up to Android 4.3)
  // Configure the output with defaults
  ConfigureOutputFormat(mediaformat);

  return true;
}

int CDVDVideoCodecAndroidMediaCodec::GetOutputPicture(void)
{
  int rtn = 0;

  int64_t timeout_us = 10000;
  AMediaCodecBufferInfo bufferInfo;
  ssize_t index = AMediaCodec_dequeueOutputBuffer(m_codec->codec(), &bufferInfo, timeout_us);
  if (index >= 0)
  {
    int64_t pts = bufferInfo.presentationTimeUs;
    m_videobuffer.dts = DVD_NOPTS_VALUE;
    m_videobuffer.pts = DVD_NOPTS_VALUE;
    if (pts != AV_NOPTS_VALUE)
    {
      m_videobuffer.pts = pts;
      m_videobuffer.pts += m_dtsShift;
      if (m_lastPTS >= 0 && pts > m_lastPTS)
        m_OutputDuration += pts - m_lastPTS;
      m_lastPTS = pts;
    }

    if (m_codecControlFlags & DVD_CODEC_CTRL_DROP)
    {
      AMediaCodec_releaseOutputBuffer(m_codec->codec(), index, false);
      return -1;
    }

    if (bufferInfo.flags & AMEDIACODEC_BUFFER_FLAG_END_OF_STREAM)
    {
      CLog::Log(LOGDEBUG, "CDVDVideoCodecAndroidMediaCodec:: BUFFER_FLAG_END_OF_STREAM");
      AMediaCodec_releaseOutputBuffer(m_codec->codec(), index, false);
      return -1;
    }

    if (m_videobuffer.videoBuffer)
      m_videobuffer.videoBuffer->Release();

    m_videobuffer.videoBuffer = m_videoBufferPool->Get();
    static_cast<CMediaCodecVideoBuffer*>(m_videobuffer.videoBuffer)->Set(index, m_textureId,  m_surfaceTexture, m_frameAvailable, m_jnivideoview);

    rtn = 1;
  }
  else if (index == AMEDIACODEC_INFO_OUTPUT_FORMAT_CHANGED)
  {
    AMediaFormat* mediaformat = AMediaCodec_getOutputFormat(m_codec->codec());
    if (!mediaformat)
      CLog::Log(LOGERROR, "CDVDVideoCodecAndroidMediaCodec::GetOutputPicture(INFO_OUTPUT_FORMAT_CHANGED) ExceptionCheck: getOutputBuffers");
    else
      ConfigureOutputFormat(mediaformat);
  }
  else if (index == AMEDIACODEC_INFO_TRY_AGAIN_LATER || index == AMEDIACODEC_INFO_OUTPUT_BUFFERS_CHANGED)
  {
    // ignore
    rtn = 0;
  }
  else
  {
    // we should never get here
    CLog::Log(LOGERROR, "CDVDVideoCodecAndroidMediaCodec::GetOutputPicture unknown index(%d)", index);
    rtn = -2;
  }

  return rtn;
}

void CDVDVideoCodecAndroidMediaCodec::ConfigureOutputFormat(AMediaFormat* mediaformat)
{
  int width       = 0;
  int height      = 0;
  int stride      = 0;
  int slice_height= 0;
  int color_format= 0;
  int crop_left   = 0;
  int crop_top    = 0;
  int crop_right  = 0;
  int crop_bottom = 0;

  int tmpVal;
  if (AMediaFormat_getInt32(mediaformat, AMEDIAFORMAT_KEY_WIDTH, &tmpVal))
    width = tmpVal;
  if (AMediaFormat_getInt32(mediaformat, AMEDIAFORMAT_KEY_HEIGHT, &tmpVal))
    height = tmpVal;
  if (AMediaFormat_getInt32(mediaformat, AMEDIAFORMAT_KEY_STRIDE, &tmpVal))
    stride = tmpVal;
  if (AMediaFormat_getInt32(mediaformat, XMEDIAFORMAT_KEY_SLICE, &tmpVal))
    slice_height = tmpVal;
  if (AMediaFormat_getInt32(mediaformat, AMEDIAFORMAT_KEY_COLOR_FORMAT, &tmpVal))
    color_format = tmpVal;
  if (AMediaFormat_getInt32(mediaformat, XMEDIAFORMAT_KEY_CROP_LEFT, &tmpVal))
    crop_left = tmpVal;
  if (AMediaFormat_getInt32(mediaformat, XMEDIAFORMAT_KEY_CROP_RIGHT, &tmpVal))
    crop_right = tmpVal;
  if (AMediaFormat_getInt32(mediaformat, XMEDIAFORMAT_KEY_CROP_TOP, &tmpVal))
    crop_top = tmpVal;
  if (AMediaFormat_getInt32(mediaformat, XMEDIAFORMAT_KEY_CROP_BOTTOM, &tmpVal))
    crop_bottom = tmpVal;

  if (!crop_right)
    crop_right = width-1;
  if (!crop_bottom)
    crop_bottom = height-1;

  // clear any jni exceptions
  if (xbmc_jnienv()->ExceptionCheck())
    xbmc_jnienv()->ExceptionClear();

  CLog::Log(LOGDEBUG, "CDVDVideoCodecAndroidMediaCodec:: "
    "width(%d), height(%d), stride(%d), slice-height(%d), color-format(%d)",
    width, height, stride, slice_height, color_format);
  CLog::Log(LOGDEBUG, "CDVDVideoCodecAndroidMediaCodec:: "
    "crop-left(%d), crop-top(%d), crop-right(%d), crop-bottom(%d)",
    crop_left, crop_top, crop_right, crop_bottom);

  if (m_render_surface)
    CLog::Log(LOGDEBUG, "CDVDVideoCodecAndroidMediaCodec:: Multi-Surface Rendering");
  else
    CLog::Log(LOGDEBUG, "CDVDVideoCodecAndroidMediaCodec:: Direct Surface Rendering");

  if (crop_right)
    width = crop_right  + 1 - crop_left;
  if (crop_bottom)
    height = crop_bottom + 1 - crop_top;

  m_videobuffer.iDisplayWidth  = m_videobuffer.iWidth  = width;
  m_videobuffer.iDisplayHeight = m_videobuffer.iHeight = height;

  if (m_hints.aspect > 1.0 && !m_hints.forced_aspect)
  {
    m_videobuffer.iDisplayWidth  = ((int)lrint(m_videobuffer.iHeight * m_hints.aspect)) & ~3;
    if (m_videobuffer.iDisplayWidth > m_videobuffer.iWidth)
    {
      m_videobuffer.iDisplayWidth  = m_videobuffer.iWidth;
      m_videobuffer.iDisplayHeight = ((int)lrint(m_videobuffer.iWidth / m_hints.aspect)) & ~3;
    }
  }
}

void CDVDVideoCodecAndroidMediaCodec::CallbackInitSurfaceTexture(void *userdata)
{
  CDVDVideoCodecAndroidMediaCodec *ctx = static_cast<CDVDVideoCodecAndroidMediaCodec*>(userdata);
  ctx->InitSurfaceTexture();
}

void CDVDVideoCodecAndroidMediaCodec::InitSurfaceTexture(void)
{
  if (m_render_surface)
    return;

  // We MUST create the GLES texture on the main thread
  // to match where the valid GLES context is located.
  // It would be nice to move this out of here, we would need
  // to create/fetch/create from g_RenderManager. But g_RenderManager
  // does not know we are using MediaCodec until Configure and we
  // we need m_surfaceTexture valid before then. Chicken, meet Egg.
  if (g_application.IsCurrentThread())
  {
    // localize GLuint so we do not spew gles includes in our header
    GLuint texture_id;

    glGenTextures(1, &texture_id);
    glBindTexture(  GL_TEXTURE_EXTERNAL_OES, texture_id);
    glTexParameterf(GL_TEXTURE_EXTERNAL_OES, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_EXTERNAL_OES, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_EXTERNAL_OES, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_EXTERNAL_OES, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glBindTexture(  GL_TEXTURE_EXTERNAL_OES, 0);
    m_textureId = texture_id;

    m_surfaceTexture = std::shared_ptr<CJNISurfaceTexture>(new CJNISurfaceTexture(m_textureId));
    // hook the surfaceTexture OnFrameAvailable callback
    m_frameAvailable = std::shared_ptr<CDVDMediaCodecOnFrameAvailable>(new CDVDMediaCodecOnFrameAvailable(m_surfaceTexture));
    m_jnisurface = new CJNISurface(*m_surfaceTexture);
    m_surface = ANativeWindow_fromSurface(xbmc_jnienv(), m_jnisurface->get_raw());
  }
  else
  {
    ThreadMessageCallback callbackData;
    callbackData.callback = &CallbackInitSurfaceTexture;
    callbackData.userptr  = (void*)this;

    // wait for it.
    CApplicationMessenger::GetInstance().SendMsg(TMSG_CALLBACK, -1, -1, static_cast<void*>(&callbackData));
  }

  return;
}

void CDVDVideoCodecAndroidMediaCodec::ReleaseSurfaceTexture(void)
{
  if (m_render_surface)
    return;

  // it is safe to delete here even though these items
  // were created in the main thread instance
  SAFE_DELETE(m_jnisurface);
  m_frameAvailable.reset();
  m_surfaceTexture.reset();

  if (m_textureId > 0)
  {
    GLuint texture_id = m_textureId;
    glDeleteTextures(1, &texture_id);
    m_textureId = 0;
  }
}

void CDVDVideoCodecAndroidMediaCodec::UpdateFpsDuration()
{
  if (m_hints.fpsrate > 0 && m_hints.fpsscale > 0)
    m_fpsDuration = static_cast<uint32_t>(static_cast<uint64_t>(DVD_TIME_BASE) * m_hints.fpsscale /  m_hints.fpsrate);
  else
    m_fpsDuration = 1;

  m_processInfo.SetVideoFps(static_cast<float>(m_hints.fpsrate) / m_hints.fpsscale);

  CLog::Log(LOGDEBUG, "CDVDVideoCodecAndroidMediaCodec::UpdateFpsDuration fpsRate:%u fpsscale:%u, fpsDur:%u", m_hints.fpsrate, m_hints.fpsscale, m_fpsDuration);
}

void CDVDVideoCodecAndroidMediaCodec::surfaceChanged(CJNISurfaceHolder holder, int format, int width, int height)
{
}

void CDVDVideoCodecAndroidMediaCodec::surfaceCreated(CJNISurfaceHolder holder)
{
  if (m_state == MEDIACODEC_STATE_STOPPED)
  {
    ConfigureMediaCodec();
  }
}

void CDVDVideoCodecAndroidMediaCodec::surfaceDestroyed(CJNISurfaceHolder holder)
{
  if (m_state != MEDIACODEC_STATE_STOPPED && m_state != MEDIACODEC_STATE_UNINITIALIZED)
  {
    m_state = MEDIACODEC_STATE_STOPPED;
    if(m_surface)
      ANativeWindow_release(m_surface);
    AMediaCodec_stop(m_codec->codec());
  }
}
