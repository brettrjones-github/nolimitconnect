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
#include "NativeCommon.h"
#include "AudioTrackJava.h"

#include <CoreLib/VxDebug.h>

namespace
{
	std::map<unsigned int, AudioTrackJavaClass *>	g_oAudioTrackJavaMap;
	VxMutex											g_AudioTrackJavaListMutex;
}

//============================================================================
AudioTrackJavaClass::AudioTrackJavaClass( JavaEnvCache& envCache, jclass javaRefClass )
: JavaBaseClass( envCache, javaRefClass, true )
{
}

//============================================================================
AudioTrackJavaClass::~AudioTrackJavaClass()
{
}

//============================================================================
bool AudioTrackJavaClass::InitPlayout( jobject audioTrackJavaObject, int sample_rate, int channels)
{
	return callBooleanMethod( audioTrackJavaObject, init_playout_, static_cast<jint>(sample_rate), static_cast<jint>(channels)  );
}

//============================================================================
bool AudioTrackJavaClass::StartPlayout( jobject audioTrackJavaObject )
{
	return callBooleanMethod( audioTrackJavaObject, start_playout_ );
}

//============================================================================
bool AudioTrackJavaClass::StopPlayout( jobject audioTrackJavaObject )
{
	return callBooleanMethod( audioTrackJavaObject, stop_playout_ );
}

//============================================================================
bool AudioTrackJavaClass::SetStreamVolume( jobject audioTrackJavaObject, int volume)
{
	return callBooleanMethod( audioTrackJavaObject, set_stream_volume_, static_cast<jint>(volume) );
}

//============================================================================
int AudioTrackJavaClass::GetStreamMaxVolume( jobject audioTrackJavaObject )
{
	return callIntMethod( audioTrackJavaObject, get_stream_max_volume_ );
}

//============================================================================
int AudioTrackJavaClass::GetStreamVolume( jobject audioTrackJavaObject )
{
	return callIntMethod( audioTrackJavaObject, get_stream_volume_ );
}

//============================================================================
void AudioTrackJavaClassThreadExit( unsigned int uiThreadId )
{
	std::map<unsigned int, AudioTrackJavaClass *>::iterator oIter;
	g_AudioTrackJavaListMutex.lock();
	oIter = g_oAudioTrackJavaMap.find(uiThreadId);
	if( oIter != g_oAudioTrackJavaMap.end() )
	{
		delete oIter->second;
		g_oAudioTrackJavaMap.erase( oIter );
	}

	g_AudioTrackJavaListMutex.unlock();
}

//============================================================================
void AudioTrackJavaClassShutdown( void )
{
	std::map<unsigned int, AudioTrackJavaClass *>::iterator oIter;
	g_AudioTrackJavaListMutex.lock();
	for( oIter = g_oAudioTrackJavaMap.begin(); oIter != g_oAudioTrackJavaMap.end(); ++oIter )
	{
		delete oIter->second;
	}

	g_oAudioTrackJavaMap.clear();	
	g_AudioTrackJavaListMutex.unlock();
}

//============================================================================
AudioTrackJavaClass * GetAudioTrackJavaClass( void )
{
	unsigned int uiThreadId = VxGetCurrentThreadTid();
	std::map<unsigned int, AudioTrackJavaClass *>::iterator oIter;
	g_AudioTrackJavaListMutex.lock();
	oIter = g_oAudioTrackJavaMap.find(uiThreadId);
	if( oIter != g_oAudioTrackJavaMap.end() )
	{
		AudioTrackJavaClass * javaFriendClass =  oIter->second;
		g_AudioTrackJavaListMutex.unlock();
		return javaFriendClass;
	}

	AudioTrackJavaClass * poClass = new AudioTrackJavaClass( GetJavaEnvCache(), GetJavaEnvCache().getClassAudioPlayback() );
	if( false == poClass->attachToThread() )
	{
		LogMsg( LOG_ERROR, "GetAudioMgrJavaClass: FAIL Thread 0x%x\n", uiThreadId );
		delete poClass;
		g_AudioTrackJavaListMutex.unlock();
		return NULL;		
	}

	// get java member id s
	//LogMsg( LOG_INFO, "GetAudioTrackJavaClass: Getting member ids\n" );
	poClass->init_playout_ 					= poClass->getMethodId( "initPlayout", "(II)Z" );
	poClass->start_playout_ 				= poClass->getMethodId( "startPlayout", "()Z"  );
	poClass->stop_playout_ 					= poClass->getMethodId( "stopPlayout", "()Z"  );
	poClass->set_stream_volume_ 			= poClass->getMethodId( "setStreamVolume", "(I)Z"  );
	poClass->get_stream_max_volume_ 		= poClass->getMethodId( "getStreamMaxVolume", "()I"  );
	poClass->get_stream_volume_ 			= poClass->getMethodId( "getStreamVolume", "()I"  );

	//LogMsg( LOG_INFO, "GetAudioTrackJavaClass: inserting pair\n" );
	g_oAudioTrackJavaMap.insert( std::make_pair(uiThreadId, poClass) );

	//LogMsg( LOG_INFO, "GetAudioTrackJavaClass: Added thread 0x%x AudioTrackJavaClass 0x%x\n", uiThreadId, poClass );
	g_AudioTrackJavaListMutex.unlock();
	return poClass;
}

#endif //TARGET_OS_ANDROID
