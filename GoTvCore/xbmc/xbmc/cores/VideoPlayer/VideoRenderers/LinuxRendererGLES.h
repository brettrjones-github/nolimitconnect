/*
 *  Copyright (C) 2010-2018 Team Kodi
 *  This file is part of Kodi - https://kodi.tv
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSES/README.md for more information.
 */

#pragma once

#include <vector>

#include "system_gl.h"

#include "FrameBufferObject.h"
#include "xbmc/guilib/Shader.h"
#include "cores/VideoSettings.h"
#include "RenderFlags.h"
#include "RenderInfo.h"
#include "windowing/GraphicContext.h"
#include "BaseRenderer.h"
#include "xbmc/cores/VideoPlayer/DVDCodecs/Video/DVDVideoCodec.h"

extern "C" {
#include "libavutil/mastering_display_metadata.h"
}

class CRenderCapture;
class CRenderSystemGLES;

class CBaseTexture;
namespace Shaders { class BaseYUV2RGBGLSLShader; }
namespace Shaders { class BaseVideoFilterShader; }

struct DRAWRECT
{
  float left;
  float top;
  float right;
  float bottom;
};

enum RenderMethod
{
  RENDER_GLSL = 0x01,
  RENDER_CUSTOM = 0x02,
};

enum RenderQuality
{
  RQ_LOW=1,
  RQ_SINGLEPASS,
  RQ_MULTIPASS,
  RQ_SOFTWARE
};

#define PLANE_Y 0
#define PLANE_U 1
#define PLANE_V 2

#define FIELD_FULL 0
#define FIELD_TOP 1
#define FIELD_BOT 2

class CEvent;

class CLinuxRendererGLES : public CBaseRenderer
{
public:
  CLinuxRendererGLES();
  virtual ~CLinuxRendererGLES();

  // Registration
  static CBaseRenderer* Create(CVideoBuffer *buffer);
  static bool Register();

  // Player functions
  virtual bool Configure(const VideoPicture &picture, float fps, unsigned int orientation) override;
  virtual bool IsConfigured() override { return m_bConfigured; }
  virtual void AddVideoPicture(const VideoPicture &picture, int index) override;
  virtual void UnInit() override;
  virtual bool Flush(bool saveBuffers) override;
  virtual void SetBufferSize(int numBuffers) override { m_NumYV12Buffers = numBuffers; }
  virtual bool IsGuiLayer() override;
  virtual void ReleaseBuffer(int idx) override;
  virtual void RenderUpdate(int index, int index2, bool clear, unsigned int flags, unsigned int alpha) override;
  virtual void Update() override;
  virtual bool RenderCapture(CRenderCapture* capture) override;
  virtual CRenderInfo GetRenderInfo() override;
  virtual bool ConfigChanged(const VideoPicture &picture) override;

  // Feature support
  virtual bool SupportsMultiPassRendering() override;
  virtual bool Supports(ERENDERFEATURE feature) override;
  virtual bool Supports(ESCALINGMETHOD method) override;

protected:
  virtual void Render(unsigned int flags, int index);
  virtual void RenderUpdateVideo(bool clear, unsigned int flags = 0, unsigned int alpha = 255);

  int  NextYV12Texture();
  virtual bool ValidateRenderTarget();
  virtual void LoadShaders(int field=FIELD_FULL);
  virtual void ReleaseShaders();
  void SetTextureFilter(GLenum method);
  void UpdateVideoFilter();
  AVColorPrimaries GetSrcPrimaries(AVColorPrimaries srcPrimaries, unsigned int width, unsigned int height);

  // textures
  virtual bool UploadTexture(int index);
  virtual void DeleteTexture(int index);
  virtual bool CreateTexture(int index);

  bool UploadYV12Texture(int index);
  void DeleteYV12Texture(int index);
  bool CreateYV12Texture(int index);
  virtual bool SkipUploadYV12(int index) { return false; }

  bool UploadNV12Texture(int index);
  void DeleteNV12Texture(int index);
  bool CreateNV12Texture(int index);

  void CalculateTextureSourceRects(int source, int num_planes);

  // renderers
  void RenderToFBO(int index, int field);
  void RenderFromFBO();
  void RenderSinglePass(int index, int field);    // single pass glsl renderer

  GLint GetInternalFormat(GLint format, int bpp);

  // hooks for HwDec Renderered
  virtual bool LoadShadersHook() { return false; };
  virtual bool RenderHook(int idx) { return false; };
  virtual void AfterRenderHook(int idx) {};

  struct
  {
    CFrameBufferObject fbo;
    float width{0.0};
    float height{0.0};
  } m_fbo;

  int m_iYV12RenderBuffer{0};
  int m_NumYV12Buffers{0};

  bool m_bConfigured{false};
  bool m_bValidated{false};
  GLenum m_textureTarget;
  int m_renderMethod{RENDER_GLSL};
  RenderQuality m_renderQuality{RQ_SINGLEPASS};

  // Raw data used by renderer
  int m_currentField{FIELD_FULL};
  int m_reloadShaders{0};
  CRenderSystemGLES *m_renderSystem{nullptr};

  struct CYuvPlane
  {
    GLuint id;
    CRect  rect;

    float  width;
    float  height;

    unsigned texwidth;
    unsigned texheight;

    //pixels per texel
    unsigned pixpertex_x;
    unsigned pixpertex_y;
  };

  struct CPictureBuffer
  {
    CPictureBuffer();
    ~CPictureBuffer() = default;

    CYuvPlane fields[MAX_FIELDS][YuvImage::MAX_PLANES];
    YuvImage image;

    CVideoBuffer *videoBuffer{nullptr};
    bool loaded{false};

    AVColorPrimaries m_srcPrimaries;
    AVColorSpace m_srcColSpace;
    int m_srcBits{8};
    int m_srcTextureBits{8};
    bool m_srcFullRange;

    bool hasDisplayMetadata = false;
    AVMasteringDisplayMetadata displayMetadata;
    bool hasLightMetadata = false;
    AVContentLightMetadata lightMetadata;
  };

  // YV12 decoder textures
  // field index 0 is full image, 1 is odd scanlines, 2 is even scanlines
  CPictureBuffer m_buffers[NUM_BUFFERS];

  void LoadPlane(CYuvPlane& plane, int type,
                 unsigned width,  unsigned height,
                 int stride, int bpp, void* data);

  Shaders::BaseYUV2RGBGLSLShader *m_pYUVProgShader{nullptr};
  Shaders::BaseYUV2RGBGLSLShader *m_pYUVBobShader{nullptr};
  Shaders::BaseVideoFilterShader *m_pVideoFilterShader{nullptr};
  ESCALINGMETHOD m_scalingMethod{VS_SCALINGMETHOD_LINEAR};
  ESCALINGMETHOD m_scalingMethodGui{VS_SCALINGMETHOD_MAX};
  bool m_fullRange;
  AVColorPrimaries m_srcPrimaries;
  bool m_toneMap = false;

  // clear colour for "black" bars
  float m_clearColour{0.0f};
  CRect m_viewRect;
};
