/*
 *  Copyright (C) 2017-2018 Team Kodi
 *  This file is part of Kodi - https://kodi.tv
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSES/README.md for more information.
 */

#pragma once

#include <stdint.h>

using color_t = uint32_t;

enum
{
  RENDER_QUIRKS_MAJORMEMLEAK_OVERLAYRENDERER = 1 << 0,
  RENDER_QUIRKS_YV12_PREFERED = 1 << 1,
  RENDER_QUIRKS_BROKEN_OCCLUSION_QUERY = 1 << 2,
};

enum RENDER_STEREO_VIEW
{
  RENDER_STEREO_VIEW_OFF,
  RENDER_STEREO_VIEW_LEFT,
  RENDER_STEREO_VIEW_RIGHT,
};

enum RENDER_STEREO_MODE
{
  RENDER_STEREO_MODE_OFF,
  RENDER_STEREO_MODE_SPLIT_HORIZONTAL,
  RENDER_STEREO_MODE_SPLIT_VERTICAL,
  RENDER_STEREO_MODE_ANAGLYPH_RED_CYAN,
  RENDER_STEREO_MODE_ANAGLYPH_GREEN_MAGENTA,
  RENDER_STEREO_MODE_ANAGLYPH_YELLOW_BLUE,
  RENDER_STEREO_MODE_INTERLACED,
  RENDER_STEREO_MODE_CHECKERBOARD,
  RENDER_STEREO_MODE_HARDWAREBASED,
  RENDER_STEREO_MODE_MONO,
  RENDER_STEREO_MODE_COUNT,

  // Pseudo modes
  RENDER_STEREO_MODE_AUTO = 100,
  RENDER_STEREO_MODE_UNDEFINED = 999,
};
