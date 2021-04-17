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

#include "NativeCommon.h"
#include "BuildInfoJava.h"
#include <CoreLib/VxDebug.h>

namespace
{
	std::map<unsigned int, BuildInfoJavaClass *>	g_oBuildInfoJavaMap;
	VxMutex											g_BuildInfoJavaListMutex;
}

//============================================================================
BuildInfoJavaClass::BuildInfoJavaClass( JavaEnvCache& envCache, jclass javaRefClass )
: JavaBaseClass( envCache, javaRefClass )
{
}

//============================================================================
BuildInfoJavaClass::~BuildInfoJavaClass()
{
}

//============================================================================
void BuildInfoJavaClassThreadExit( unsigned int uiThreadId )
{
	g_BuildInfoJavaListMutex.lock();
	std::map<unsigned int, BuildInfoJavaClass *>::iterator oIter;

	oIter = g_oBuildInfoJavaMap.find(uiThreadId);
	if( oIter != g_oBuildInfoJavaMap.end() )
	{
		delete oIter->second;
		g_oBuildInfoJavaMap.erase( oIter );
	}

	g_BuildInfoJavaListMutex.unlock();
}

//============================================================================
void BuildInfoJavaClassShutdown( void )
{
	g_BuildInfoJavaListMutex.lock();
	std::map<unsigned int, BuildInfoJavaClass *>::iterator oIter;
	for( oIter = g_oBuildInfoJavaMap.begin(); oIter != g_oBuildInfoJavaMap.end(); ++oIter )
	{
		delete oIter->second;
	}

	g_oBuildInfoJavaMap.clear();	
	g_BuildInfoJavaListMutex.unlock();
}

//============================================================================
BuildInfoJavaClass * GetBuildInfoJavaClass( void )
{
	g_BuildInfoJavaListMutex.lock();
	unsigned int uiThreadId = VxGetCurrentThreadTid();
	std::map<unsigned int, BuildInfoJavaClass *>::iterator oIter;

	oIter = g_oBuildInfoJavaMap.find(uiThreadId);
	if( oIter != g_oBuildInfoJavaMap.end() )
	{
		BuildInfoJavaClass * javaFriendClass =  oIter->second;
		g_BuildInfoJavaListMutex.unlock();
		return javaFriendClass;
	}

	// does not exist for this thread
	BuildInfoJavaClass * poClass = new BuildInfoJavaClass( GetJavaEnvCache(), GetJavaEnvCache().getClassBuildInfo() );
	if( false == poClass->attachToThread() )
	{
		LogMsg( LOG_ERROR, "GetBuildInfoJavaClass: FAIL Thread 0x%x\n", uiThreadId );
		delete poClass;
		g_BuildInfoJavaListMutex.unlock();
		return NULL;		
	}

	LogMsg( LOG_INFO, "GetBuildInfoJavaClass: inserting pair\n" );
	g_oBuildInfoJavaMap.insert( std::make_pair(uiThreadId, poClass) );

	LogMsg( LOG_INFO, "GetBuildInfoJavaClass: Added thread 0x%x BuildInfoJavaClass 0x%x\n", uiThreadId, poClass );
	g_BuildInfoJavaListMutex.unlock();
	return poClass;
}

#endif //TARGET_OS_ANDROID
