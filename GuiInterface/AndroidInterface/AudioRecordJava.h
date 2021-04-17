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

class AudioRecordJavaClass : public JavaBaseClass
{
public:
	AudioRecordJavaClass( JavaEnvCache& envCache, jclass javaRefClass );
	~AudioRecordJavaClass();

	int							InitRecording( jobject audioRecordObject, int sample_rate, size_t channels );
	bool						StartRecording( jobject audioRecordObject );
	bool						StopRecording( jobject audioRecordObject );
	bool						EnableBuiltInAEC( jobject audioRecordObject, bool enable);
	bool						EnableBuiltInAGC( jobject audioRecordObject, bool enable);
	bool						EnableBuiltInNS( jobject audioRecordObject, bool enable);

	//=== vars ===//	
	jmethodID					init_recording_;
	jmethodID					start_recording_;
	jmethodID					stop_recording_;
	jmethodID					enable_built_in_aec_;
	jmethodID					enable_built_in_agc_;
	jmethodID					enable_built_in_ns_;
};

AudioRecordJavaClass * GetAudioRecordJavaClass( void );
void AudioRecordJavaClassThreadExit( unsigned int uiThreadId );
void AudioRecordJavaClassShutdown( void );

#endif //TARGET_OS_ANDROID
