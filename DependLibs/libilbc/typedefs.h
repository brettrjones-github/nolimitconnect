/*
 *  Copyright (c) 2012 The WebRTC project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */

// This file contains platform-specific typedefs and defines.
// Much of it is derived from Chromium's build/build_config.h.

#pragma once

// Processor architecture detection.  For more info on what's defined, see:
//   http://msdn.microsoft.com/en-us/library/b0084kay.aspx
//   http://www.agner.org/optimize/calling_conventions.pdf
//   or with gcc, run: "echo | gcc -E -dM -"

#include <libilbc/config_libilbc.h>


//// Annotate a function indicating the caller must examine the return value.
//// Use like:
////   int foo() WARN_UNUSED_RESULT;
//// TODO(ajm): Hack to avoid multiple definitions until the base/ of webrtc and
//// libjingle are merged.
//#if !defined(WARN_UNUSED_RESULT)
//#if defined(__GNUC__)
//#define WARN_UNUSED_RESULT __attribute__((warn_unused_result))
//#else
//#define WARN_UNUSED_RESULT
//#endif
//#endif  // WARN_UNUSED_RESULT
//
//// Put after a variable that might not be used, to prevent compiler warnings:
////   int result ATTRIBUTE_UNUSED = DoSomething();
////   assert(result == 17);
//#ifndef ATTRIBUTE_UNUSED
//#if defined(__GNUC__) || defined(__clang__)
//#define ATTRIBUTE_UNUSED __attribute__((unused))
//#else
//#define ATTRIBUTE_UNUSED
//#endif
//#endif

#define NO_RETURN		NLC_NO_RETURN



