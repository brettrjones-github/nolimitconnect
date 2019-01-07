/*
 *  Copyright (C) 2007-2018 Team Kodi
 *  This file is part of Kodi - https://kodi.tv
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSES/README.md for more information.
 */

#pragma once

#include "cores/VideoPlayer/VideoRenderers/LinuxRendererGLES.h"
#include "DRMPRIMEEGL.h"

class CRendererDRMPRIMEGLES : public CLinuxRendererGLES
{
public:
  CRendererDRMPRIMEGLES() = default;
  ~CRendererDRMPRIMEGLES();

  // Registration
  static CBaseRenderer* Create(CVideoBuffer* buffer);
  static void Register();

  // CLinuxRendererGLES overrides
  bool Configure(const VideoPicture &picture, float fps, unsigned int orientation) override;
  void ReleaseBuffer(int index) override;

protected:
  // CLinuxRendererGLES overrides
  bool LoadShadersHook() override;
  bool RenderHook(int index) override;
  bool UploadTexture(int index) override;
  void DeleteTexture(int index) override;
  bool CreateTexture(int index) override;

  CDRMPRIMETexture m_DRMPRIMETextures[NUM_BUFFERS];
};
