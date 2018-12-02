/*
 * Copyright (c) 2016, Alliance for Open Media. All rights reserved
 *
 * This source code is subject to the terms of the BSD 2 Clause License and
 * the Alliance for Open Media Patent License 1.0. If the BSD 2 Clause License
 * was not distributed with this source code in the LICENSE file, you can
 * obtain it at www.aomedia.org/license/software. If the Alliance for Open
 * Media Patent License 1.0 was not distributed with this source code in the
 * PATENTS file, you can obtain it at www.aomedia.org/license/patent.
 */

#ifndef VIDEO_COMMON_H_
#define VIDEO_COMMON_H_

#include "./tools_common.h"

typedef struct {
  uint32_t codec_fourcc;
  int frame_width;
  int frame_height;
  struct AvxRational time_base;
} AvxVideoInfo;

#endif  // VIDEO_COMMON_H_
