#pragma once
//============================================================================
// Copyright (C) 2016 Brett R. Jones
// Issued to MIT style license by Brett R. Jones in 2017
//
// You may use, copy, modify, merge, publish, distribute, sub-license, and/or sell this software
// provided this Copyright is not modified or removed and is included all copies or substantial portions of the Software
//
// This code is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
//
// bjones.engineer@gmail.com
// http://www.nolimitconnect.com
//============================================================================

// android specific code
#ifdef TARGET_OS_ANDROID

#include "JavaBaseClass.h"

class AudioMgrJavaClass : public JavaBaseClass
{
public:
	AudioMgrJavaClass( JavaEnvCache& envCache, jclass javaRefClass );
	virtual ~AudioMgrJavaClass();

	bool						Init( jobject audioMgrObject );
	void						Close( jobject audioMgrObject );
	bool						IsCommunicationModeEnabled( jobject audioMgrObject );
	bool						IsDeviceBlacklistedForOpenSLESUsage( jobject audioMgrObject );

	//=== vars ===//
	jmethodID					init_;
	jmethodID					dispose_;
	jmethodID					is_communication_mode_enabled_;
	jmethodID					is_device_blacklisted_for_open_sles_usage_;
};

AudioMgrJavaClass * GetAudioMgrJavaClass( void );
void AudioMgrJavaClassThreadExit( unsigned int uiThreadId );
void AudioMgrJavaClassShutdown( void );

#endif //TARGET_OS_ANDROID
