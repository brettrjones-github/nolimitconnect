/*
 *  Copyright (C) 2005-2018 Team Kodi
 *  This file is part of Kodi - https://kodi.tv
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSES/README.md for more information.
 */

#include "utils/log.h"

#include "DVDFactoryCodec.h"
#include "Video/AddonVideoCodec.h"
#include "Video/DVDVideoCodec.h"
#include "Audio/DVDAudioCodec.h"
#include "Overlay/DVDOverlayCodec.h"
#include "cores/VideoPlayer/DVDCodecs/DVDCodecs.h"

#include "addons/AddonProvider.h"

#include "Video/DVDVideoCodecFFmpeg.h"

#include "Audio/DVDAudioCodecFFmpeg.h"
#include "Audio/DVDAudioCodecPassthrough.h"
#include "Overlay/DVDOverlayCodecSSA.h"
#include "Overlay/DVDOverlayCodecText.h"
#include "Overlay/DVDOverlayCodecTX3G.h"
#include "Overlay/DVDOverlayCodecFFmpeg.h"


#include "DVDStreamInfo.h"
#include "threads/SingleLock.h"
#include "utils/StringUtils.h"


//------------------------------------------------------------------------------
// Video
//------------------------------------------------------------------------------

std::map<std::string, CreateHWVideoCodec> CDVDFactoryCodec::m_hwVideoCodecs;
std::map<std::string, CreateHWAudioCodec> CDVDFactoryCodec::m_hwAudioCodecs;

std::map<std::string, CreateHWAccel> CDVDFactoryCodec::m_hwAccels;

CCriticalSection videoCodecSection, audioCodecSection;

CDVDVideoCodec* CDVDFactoryCodec::CreateVideoCodec(CDVDStreamInfo &hint, CProcessInfo &processInfo)
{
  CSingleLock lock(videoCodecSection);

  std::unique_ptr<CDVDVideoCodec> pCodec;
  CDVDCodecOptions options;

  // addon handler for this stream ?

  if (hint.externalInterfaces)
  {
    ADDON::BinaryAddonBasePtr addonInfo;
    kodi::addon::IAddonInstance* parentInstance;
    hint.externalInterfaces->getAddonInstance(ADDON::IAddonProvider::INSTANCE_VIDEOCODEC, addonInfo, parentInstance);
    if (addonInfo && parentInstance)
    {
      pCodec.reset(new CAddonVideoCodec(processInfo, addonInfo, parentInstance));
      if (pCodec && pCodec->Open(hint, options))
      {
        return pCodec.release();
      }
    }
    return nullptr;
  }

  // platform specifig video decoders
  if (!(hint.codecOptions & CODEC_FORCE_SOFTWARE))
  {
    for (auto &codec : m_hwVideoCodecs)
    {
      pCodec.reset(CreateVideoCodecHW(codec.first, processInfo));
      if (pCodec && pCodec->Open(hint, options))
      {
        return pCodec.release();
      }
    }
    if (!(hint.codecOptions & CODEC_ALLOW_FALLBACK))
      return nullptr;
  }

  pCodec.reset(new CDVDVideoCodecFFmpeg(processInfo));
  if (pCodec->Open(hint, options))
  {
    return pCodec.release();
  }

  return nullptr;
}

CDVDVideoCodec* CDVDFactoryCodec::CreateVideoCodecHW(std::string id, CProcessInfo &processInfo)
{
  CSingleLock lock(videoCodecSection);

  auto it = m_hwVideoCodecs.find(id);
  if (it != m_hwVideoCodecs.end())
  {
    return it->second(processInfo);
  }

  return nullptr;
}

IHardwareDecoder* CDVDFactoryCodec::CreateVideoCodecHWAccel(std::string id, CDVDStreamInfo &hint, CProcessInfo &processInfo, AVPixelFormat fmt)
{
  CSingleLock lock(videoCodecSection);

  auto it = m_hwAccels.find(id);
  if (it != m_hwAccels.end())
  {
    return it->second(hint, processInfo, fmt);
  }

  return nullptr;
}


void CDVDFactoryCodec::RegisterHWVideoCodec(std::string id, CreateHWVideoCodec createFunc)
{
  CSingleLock lock(videoCodecSection);

  m_hwVideoCodecs[id] = createFunc;
}

void CDVDFactoryCodec::ClearHWVideoCodecs()
{
  CSingleLock lock(videoCodecSection);

  m_hwVideoCodecs.clear();
}

std::vector<std::string> CDVDFactoryCodec::GetHWAccels()
{
  CSingleLock lock(videoCodecSection);

  std::vector<std::string> ret;
  for (auto &hwaccel : m_hwAccels)
  {
    ret.push_back(hwaccel.first);
  }
  return ret;
}

void CDVDFactoryCodec::RegisterHWAccel(std::string id, CreateHWAccel createFunc)
{
  CSingleLock lock(videoCodecSection);

  m_hwAccels[id] = createFunc;
}

void CDVDFactoryCodec::ClearHWAccels()
{
  CSingleLock lock(videoCodecSection);

  m_hwAccels.clear();
}

//------------------------------------------------------------------------------
// Audio
//------------------------------------------------------------------------------

CDVDAudioCodec* CDVDFactoryCodec::CreateAudioCodec(CDVDStreamInfo &hint, CProcessInfo &processInfo,
                                                   bool allowpassthrough, bool allowdtshddecode,
                                                   CAEStreamInfo::DataType ptStreamType)
{
  std::unique_ptr<CDVDAudioCodec> pCodec;
  CDVDCodecOptions options;

  if (allowpassthrough && ptStreamType != CAEStreamInfo::STREAM_TYPE_NULL)
    options.m_keys.push_back(CDVDCodecOption("ptstreamtype", StringUtils::SizeToString(ptStreamType)));

  if (!allowdtshddecode)
    options.m_keys.push_back(CDVDCodecOption("allowdtshddecode", "0"));

  // platform specifig audio decoders
  for (auto &codec : m_hwAudioCodecs)
  {
    pCodec.reset(CreateAudioCodecHW(codec.first, processInfo));
    if (pCodec && pCodec->Open(hint, options))
    {
      return pCodec.release();
    }
  }

  // we don't use passthrough if "sync playback to display" is enabled
  if (allowpassthrough && ptStreamType != CAEStreamInfo::STREAM_TYPE_NULL)
  {
    pCodec.reset(new CDVDAudioCodecPassthrough(processInfo, ptStreamType));
    if (pCodec->Open(hint, options))
    {
      return pCodec.release();
    }
  }

  pCodec.reset(new CDVDAudioCodecFFmpeg(processInfo));
  if (pCodec->Open(hint, options))
  {
    return pCodec.release();
  }

  return nullptr;
}

void CDVDFactoryCodec::RegisterHWAudioCodec(std::string id, CreateHWAudioCodec createFunc)
{
  CSingleLock lock(audioCodecSection);

  m_hwAudioCodecs[id] = createFunc;
}

void CDVDFactoryCodec::ClearHWAudioCodecs()
{
  CSingleLock lock(audioCodecSection);

  m_hwAudioCodecs.clear();
}

CDVDAudioCodec* CDVDFactoryCodec::CreateAudioCodecHW(std::string id, CProcessInfo &processInfo)
{
  CSingleLock lock(audioCodecSection);

  auto it = m_hwAudioCodecs.find(id);
  if (it != m_hwAudioCodecs.end())
  {
    return it->second(processInfo);
  }

  return nullptr;
}

//------------------------------------------------------------------------------
// Overlay
//------------------------------------------------------------------------------

CDVDOverlayCodec* CDVDFactoryCodec::CreateOverlayCodec( CDVDStreamInfo &hint )
{
  std::unique_ptr<CDVDOverlayCodec> pCodec;
  CDVDCodecOptions options;

  switch (hint.codec)
  {
    case AV_CODEC_ID_TEXT:
    case AV_CODEC_ID_SUBRIP:
      pCodec.reset(new CDVDOverlayCodecText());
      if (pCodec->Open(hint, options))
      {
        return pCodec.release();
      }
      break;

    case AV_CODEC_ID_SSA:
    case AV_CODEC_ID_ASS:
      pCodec.reset(new CDVDOverlayCodecSSA());
      if (pCodec->Open(hint, options))
      {
        return pCodec.release();
      }

      pCodec.reset(new CDVDOverlayCodecText());
      if (pCodec->Open(hint, options))
      {
        return pCodec.release();
      }
      break;

    case AV_CODEC_ID_MOV_TEXT:
      pCodec.reset(new CDVDOverlayCodecTX3G());
      if (pCodec->Open(hint, options))
      {
        return pCodec.release();
      }
      break;

    default:
      pCodec.reset(new CDVDOverlayCodecFFmpeg());
      if (pCodec->Open(hint, options))
      {
        return pCodec.release();
      }
      break;
  }

  return nullptr;
}

