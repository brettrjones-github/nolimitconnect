#pragma once
/*
*  Copyright (c) 2015 The WebRTC project authors. All Rights Reserved.
*
*  Use of this source code is governed by a BSD-style license
*  that can be found in the LICENSE file in the root of the source
*  tree. An additional intellectual property rights grant can be found
*  in the file PATENTS.  All contributing project authors may
*  be found in the AUTHORS file in the root of the source tree.
*/

// android specific code
#ifdef TARGET_OS_ANDROID

#include "JavaBaseClass.h"

class BuildInfoJavaClass : public JavaBaseClass
{
public:
	BuildInfoJavaClass( JavaEnvCache& envCache, jclass javaRefClass );
	virtual ~BuildInfoJavaClass();

	//=== vars ===//
	//=== java class ids ===//
};

BuildInfoJavaClass * GetBuildInfoJavaClass( void );
void BuildInfoJavaClassThreadExit( unsigned int uiThreadId );
void BuildInfoJavaClassShutdown( void );

#endif //TARGET_OS_ANDROID
