/*
 *  Copyright (c) 2012 The WebRTC project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */

#include <config_appcorelibs.h>
#ifdef TARGET_OS_ANDROID

//#include <cpu-features.h>
#include <stdint.h>

uint64_t WebRtc_GetCPUFeaturesARM(void) {
  //return android_getCpuFeatures();
	return 0;
}

#endif // TARGET_OS_ANDROID
