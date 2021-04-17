/*
*  Copyright (c) 2015 The WebRTC project authors. All Rights Reserved.
*
*  Use of this source code is governed by a BSD-style license
*  that can be found in the LICENSE file in the root of the source
*  tree. An additional intellectual property rights grant can be found
*  in the file PATENTS.  All contributing project authors may
*  be found in the AUTHORS file in the root of the source tree.
*/
// Code Modified for MyP2PWeb by Brett R. Jones

// android specific code
#ifdef TARGET_OS_ANDROID

#include "AudioMgrJava.h"
#include "JavaHelpers.h"

#include <CoreLib/VxDebug.h>
#include <CoreLib/VxMutex.h>
#include <CoreLib/VxThread.h>

#include <map>

namespace
{
	std::map<unsigned int, AudioMgrJavaClass *>		g_oAudioMgrJavaMap;
	VxMutex											g_AudioMgrJavaMutex;
}

//============================================================================
AudioMgrJavaClass::AudioMgrJavaClass( JavaEnvCache& envCache, jclass javaRefClass )
: JavaBaseClass( envCache, javaRefClass, true )
{
}

//============================================================================
AudioMgrJavaClass::~AudioMgrJavaClass()
{
}

//============================================================================
bool AudioMgrJavaClass::Init( jobject audioMgrJavaObj ) 
{
	return callBooleanMethod( audioMgrJavaObj, init_ );
}

//============================================================================
void AudioMgrJavaClass::Close( jobject audioMgrJavaObj ) 
{
	callVoidMethod(audioMgrJavaObj, dispose_);
}

//============================================================================
bool AudioMgrJavaClass::IsCommunicationModeEnabled( jobject audioMgrJavaObj ) 
{
	return callBooleanMethod( audioMgrJavaObj, is_communication_mode_enabled_ );
}


bool AudioMgrJavaClass::IsDeviceBlacklistedForOpenSLESUsage( jobject audioMgrJavaObj ) 
{
	return callBooleanMethod( audioMgrJavaObj, is_device_blacklisted_for_open_sles_usage_ );
}

//============================================================================
void AudioMgrJavaClassThreadExit( unsigned int uiThreadId )
{
	g_AudioMgrJavaMutex.lock();
	std::map<unsigned int, AudioMgrJavaClass *>::iterator oIter;

	oIter = g_oAudioMgrJavaMap.find(uiThreadId);
	if( oIter != g_oAudioMgrJavaMap.end() )
	{
		delete oIter->second;
		g_oAudioMgrJavaMap.erase( oIter );
	}

	g_AudioMgrJavaMutex.unlock();
}

//============================================================================
void AudioMgrJavaClassShutdown( void )
{
	g_AudioMgrJavaMutex.lock();
	std::map<unsigned int, AudioMgrJavaClass *>::iterator oIter;
	for( oIter = g_oAudioMgrJavaMap.begin(); oIter != g_oAudioMgrJavaMap.end(); ++oIter )
	{
		delete oIter->second;
	}

	g_oAudioMgrJavaMap.clear();	
	g_AudioMgrJavaMutex.unlock();
}

//============================================================================
AudioMgrJavaClass * GetAudioMgrJavaClass( void )
{
	unsigned int uiThreadId = VxGetCurrentThreadTid();
	std::map<unsigned int, AudioMgrJavaClass *>::iterator oIter;

	g_AudioMgrJavaMutex.lock();
	oIter = g_oAudioMgrJavaMap.find(uiThreadId);
	if( oIter != g_oAudioMgrJavaMap.end() )
	{
		AudioMgrJavaClass * javaFriendClass =  oIter->second;
		g_AudioMgrJavaMutex.unlock();
		return javaFriendClass;
	}

	// does not exist for this thread
	AudioMgrJavaClass * poClass = new AudioMgrJavaClass( GetJavaEnvCache(), GetJavaEnvCache().getClassAudioManager() );
	if( false == poClass->attachToThread() )
	{
		LogMsg( LOG_ERROR, "GetAudioMgrJavaClass: FAIL Thread 0x%x\n", uiThreadId );
		delete poClass;
		g_AudioMgrJavaMutex.unlock();
		return NULL;		
	}

	// get java member id s
	//LogMsg( LOG_INFO, "GetAudioMgrJavaClass: Getting member ids\n" );

	poClass->init_ 				= poClass->getMethodId( "init", "()Z" );
	poClass->dispose_ 			= poClass->getMethodId( "dispose", "()V"  );

	poClass->is_communication_mode_enabled_ 			= poClass->getMethodId( "isCommunicationModeEnabled", "()Z" );
	poClass->is_device_blacklisted_for_open_sles_usage_ = poClass->getMethodId( "isDeviceBlacklistedForOpenSLESUsage", "()Z" );

	//LogMsg( LOG_INFO, "GetAudioMgrJavaClass: inserting pair\n" );
	g_oAudioMgrJavaMap.insert( std::make_pair(uiThreadId, poClass) );

	//LogMsg( LOG_INFO, "GetAudioMgrJavaClass: Added thread 0x%x AudioMgrJavaClass 0x%x\n", uiThreadId, poClass );
	g_AudioMgrJavaMutex.unlock();
	return poClass;
}

#endif //TARGET_OS_ANDROID
