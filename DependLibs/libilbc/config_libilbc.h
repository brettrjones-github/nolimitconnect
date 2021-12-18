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

#include <AppDependLibrariesConfig.h>

// Processor architecture detection.  For more info on what's defined, see:
//   http://msdn.microsoft.com/en-us/library/b0084kay.aspx
//   http://www.agner.org/optimize/calling_conventions.pdf
//   or with gcc, run: "echo | gcc -E -dM -"
#if defined(_M_X64) || defined(__x86_64__)
#define WEBRTC_ARCH_X86_FAMILY
#define WEBRTC_ARCH_X86_64
#define WEBRTC_ARCH_64_BITS
#define WEBRTC_ARCH_LITTLE_ENDIAN
#elif defined(__aarch64__)
#define WEBRTC_ARCH_64_BITS
#define WEBRTC_ARCH_LITTLE_ENDIAN
#elif defined(_M_IX86) || defined(__i386__)
#define WEBRTC_ARCH_X86_FAMILY
#define WEBRTC_ARCH_X86
#define WEBRTC_ARCH_32_BITS
#define WEBRTC_ARCH_LITTLE_ENDIAN
#define WEBRTC_ARCH_LITTLE_ENDIAN
#define WEBRTC_ARCH_LITTLE_ENDIAN
#define WEBRTC_ARCH_LITTLE_ENDIAN

#elif defined(__ARMEL__)
// TODO(ajm): We'd prefer to control platform defines here, but this is
// currently provided by the Android makefiles. Commented to avoid duplicate
// definition warnings.
#define WEBRTC_ARCH_ARM
// TODO(ajm): Chromium uses the following two defines. Should we switch?
#define WEBRTC_ARCH_ARM_FAMILY
#define WEBRTC_ARCH_ARMEL
#define WEBRTC_ARCH_LITTLE_ENDIAN
#define WEBRTC_ARCH_32_BITS
#elif defined(__MIPSEL__)
#define WEBRTC_ARCH_32_BITS

#elif defined(__pnacl__)
#define WEBRTC_ARCH_32_BITS
#else
#error Please add support for your architecture in typedefs.h
#endif

#if !(defined(WEBRTC_ARCH_LITTLE_ENDIAN) ^ defined(WEBRTC_ARCH_BIG_ENDIAN))
#error Define either WEBRTC_ARCH_LITTLE_ENDIAN or WEBRTC_ARCH_BIG_ENDIAN
#endif

#if (defined(WEBRTC_ARCH_X86_FAMILY) && !defined(__SSE2__)) ||  \
    (defined(WEBRTC_ARCH_ARM_V7) && !defined(WEBRTC_ARCH_ARM_NEON))
#define WEBRTC_CPU_DETECTION
#endif


#if !defined(OVERRIDE)
#define WARN_UNUSED_RESULT		GOTV_OVERRIDE
#endif  // WARN_UNUSED_RESULT

#if !defined(WARN_UNUSED_RESULT)
#define WARN_UNUSED_RESULT		GOTV_WARN_UNUSED_RESULT
#endif  // WARN_UNUSED_RESULT

//#define BUILD_ILBC_UNIT_TESTS

