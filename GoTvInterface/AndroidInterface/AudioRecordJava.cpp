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
#include "AudioRecordJava.h"

#include <CoreLib/VxDebug.h>

#define DEBUG_JAVAAssetInfo 1 

namespace
{
	std::map<unsigned int, AudioRecordJavaClass *>		g_oAudioRecordJavaMap;
	VxMutex												g_AudioRecordJavaListMutex;
}

//============================================================================
AudioRecordJavaClass::AudioRecordJavaClass( JavaEnvCache& envCache, jclass javaRefClass )
: JavaBaseClass( envCache, javaRefClass, true )
{
}

//============================================================================
AudioRecordJavaClass::~AudioRecordJavaClass()
{
}

//============================================================================
int AudioRecordJavaClass::InitRecording( jobject audioRecordObject, int sample_rate, size_t channels )
{
	return callIntMethod( audioRecordObject, init_recording_,
		static_cast<jint>(sample_rate),
		static_cast<jint>(channels));
}

//============================================================================
bool AudioRecordJavaClass::StartRecording( jobject audioRecordObject )
{
	return callBooleanMethod( audioRecordObject, start_recording_);
}

//============================================================================
bool AudioRecordJavaClass::StopRecording( jobject audioRecordObject )
{
	return callBooleanMethod( audioRecordObject, stop_recording_);
}

//============================================================================
bool AudioRecordJavaClass::EnableBuiltInAEC( jobject audioRecordObject, bool enable)
{
	return callBooleanMethod( audioRecordObject, enable_built_in_aec_, static_cast<jboolean>(enable) );
}

//============================================================================
bool AudioRecordJavaClass::EnableBuiltInAGC( jobject audioRecordObject, bool enable)
{
	return callBooleanMethod( audioRecordObject, enable_built_in_agc_, static_cast<jboolean>(enable) );
}

//============================================================================
bool AudioRecordJavaClass::EnableBuiltInNS( jobject audioRecordObject, bool enable)
{
	return callBooleanMethod( audioRecordObject, enable_built_in_ns_, static_cast<jboolean>(enable) );
}

//============================================================================
void AudioRecordJavaClassThreadExit( unsigned int uiThreadId )
{
	std::map<unsigned int, AudioRecordJavaClass *>::iterator oIter;
	g_AudioRecordJavaListMutex.lock();
	oIter = g_oAudioRecordJavaMap.find(uiThreadId);
	if( oIter != g_oAudioRecordJavaMap.end() )
	{
		delete oIter->second;
		g_oAudioRecordJavaMap.erase( oIter );
	}

	g_AudioRecordJavaListMutex.unlock();
}

//============================================================================
void AudioRecordJavaClassShutdown( void )
{
	g_AudioRecordJavaListMutex.lock();
	std::map<unsigned int, AudioRecordJavaClass *>::iterator oIter;
	for( oIter = g_oAudioRecordJavaMap.begin(); oIter != g_oAudioRecordJavaMap.end(); ++oIter )
	{
		delete oIter->second;
	}

	g_oAudioRecordJavaMap.clear();	
	g_AudioRecordJavaListMutex.unlock();
}

//============================================================================
AudioRecordJavaClass * GetAudioRecordJavaClass( void )
{
	unsigned int uiThreadId = VxGetCurrentThreadTid();
	std::map<unsigned int, AudioRecordJavaClass *>::iterator oIter;
	g_AudioRecordJavaListMutex.lock();
	oIter = g_oAudioRecordJavaMap.find(uiThreadId);
	if( oIter != g_oAudioRecordJavaMap.end() )
	{
		AudioRecordJavaClass * javaFriendClass =  oIter->second;
		g_AudioRecordJavaListMutex.unlock();
		return javaFriendClass;
	}

	AudioRecordJavaClass * poClass = new AudioRecordJavaClass( GetJavaEnvCache(), GetJavaEnvCache().getClassAudioRecord() );
	if( false == poClass->attachToThread() )
	{
		LogMsg( LOG_ERROR, "GetAudioMgrJavaClass: FAIL Thread 0x%x\n", uiThreadId );
		delete poClass;
		g_AudioRecordJavaListMutex.unlock();
		return NULL;		
	}

	// get java member id s
	//LogMsg( LOG_INFO, "GetAudioRecordJavaClass: Getting member ids\n" );
	poClass->init_recording_ 				= poClass->getMethodId( "initRecording", "(II)I" );
	poClass->start_recording_ 				= poClass->getMethodId( "startRecording", "()Z"  );
	poClass->stop_recording_ 				= poClass->getMethodId( "stopRecording", "()Z"  );
	poClass->enable_built_in_aec_ 			= poClass->getMethodId( "enableBuiltInAEC", "(Z)Z"  );
	poClass->enable_built_in_agc_ 			= poClass->getMethodId( "enableBuiltInAGC", "(Z)Z"  );
	poClass->enable_built_in_ns_ 			= poClass->getMethodId( "enableBuiltInNS", "(Z)Z"  );
	
	//LogMsg( LOG_INFO, "GetAudioRecordJavaClass: inserting pair\n" );
	g_oAudioRecordJavaMap.insert( std::make_pair(uiThreadId, poClass) );

	//LogMsg( LOG_INFO, "GetAudioRecordJavaClass: Added thread 0x%x AudioRecordJavaClass 0x%x\n", uiThreadId, poClass );
	g_AudioRecordJavaListMutex.unlock();
	return poClass;
}

#endif //TARGET_OS_ANDROID
