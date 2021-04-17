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

class AudioTrackJavaClass : public JavaBaseClass
{
public:
	AudioTrackJavaClass( JavaEnvCache& envCache, jclass javaRefClass );
	virtual ~AudioTrackJavaClass();

	//=== vars ===//
	bool						InitPlayout( jobject audioTrackJavaObject, int sample_rate, int channels );
	bool						StartPlayout( jobject audioTrackJavaObject );
	bool						StopPlayout( jobject audioTrackJavaObject );
	bool						SetStreamVolume( jobject audioTrackJavaObject, int volume );
	int							GetStreamMaxVolume( jobject audioTrackJavaObject );
	int							GetStreamVolume( jobject audioTrackJavaObject );

	jmethodID					init_playout_;
	jmethodID					start_playout_;
	jmethodID					stop_playout_;
	jmethodID					set_stream_volume_;
	jmethodID					get_stream_max_volume_;
	jmethodID					get_stream_volume_;
};

AudioTrackJavaClass * GetAudioTrackJavaClass( void );
void AudioTrackJavaClassThreadExit( unsigned int uiThreadId );
void AudioTrackJavaClassShutdown( void );

#endif //TARGET_OS_ANDROID
