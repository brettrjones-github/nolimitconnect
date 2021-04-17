//============================================================================
// Copyright (C) 2010 Brett R. Jones
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

//Java Type JNI Type machine dependent
//C/C++ typedef Signature Call...Method
//Get...Field 
//boolean jboolean unsigned char Z Boolean 
//byte jbyte signed char B Byte 
//char jchar unsigned short C Char 
//short jshort short S Short 
//int jint int I Int 
//long jlong long J Long 
//float jfloat float F Float 
//double jdouble double D Double 
//void void V Void 
//nonprimitive jobject *... L...; Object  

// android specific code
#ifdef TARGET_OS_ANDROID
#include "NativeCommon.h"
#include <GoTvCore/GoTvP2P/P2PEngine/FileShareSettings.h>
#include "FileShareSettingsJava.h"

#define DEBUG_JAVAFRIEND 1 

namespace
{
	std::map<unsigned int, FileShareSettingsJavaClass *> g_oFileShareSettingsJavaMap;
	VxMutex												g_FileShareJavaListMutex;
}

//============================================================================
FileShareSettingsJavaClass::FileShareSettingsJavaClass( JavaEnvCache& envCache, jclass javaRefClass )
: JavaBaseClass( envCache, javaRefClass )
{
}

//============================================================================
FileShareSettingsJavaClass::~FileShareSettingsJavaClass()
{
}

//============================================================================
void FileShareSettingsJavaClassThreadExit( unsigned int uiThreadId )
{
	std::map<unsigned int, FileShareSettingsJavaClass *>::iterator oIter;
	g_FileShareJavaListMutex.lock();
	oIter = g_oFileShareSettingsJavaMap.find(uiThreadId);
	if( oIter != g_oFileShareSettingsJavaMap.end() )
	{
		delete oIter->second;
		g_oFileShareSettingsJavaMap.erase( oIter );
	}

	g_FileShareJavaListMutex.unlock();
}

//============================================================================
void FileShareSettingsJavaClassShutdown( void )
{
	std::map<unsigned int, FileShareSettingsJavaClass *>::iterator oIter;
	g_FileShareJavaListMutex.lock();
	for( oIter = g_oFileShareSettingsJavaMap.begin(); oIter != g_oFileShareSettingsJavaMap.end(); ++oIter )
	{
		delete oIter->second;
	}

	g_oFileShareSettingsJavaMap.clear();	
	g_FileShareJavaListMutex.unlock();
}

//============================================================================
FileShareSettingsJavaClass * GetFileShareSettingsJavaClass( void )
{
	unsigned int uiThreadId = VxGetCurrentThreadTid();
	std::map<unsigned int, FileShareSettingsJavaClass *>::iterator oIter;
	g_FileShareJavaListMutex.lock();
	oIter = g_oFileShareSettingsJavaMap.find(uiThreadId);
	if( oIter != g_oFileShareSettingsJavaMap.end() )
	{
		FileShareSettingsJavaClass * javaFileShareClass = oIter->second;
		g_FileShareJavaListMutex.unlock();
		return javaFileShareClass;
	}

	// does not exist for this thread
	FileShareSettingsJavaClass * poClass = new FileShareSettingsJavaClass( GetJavaEnvCache(), GetJavaEnvCache().getClassFileShareSettings() );
	if( false == poClass->attachToThread() )
	{
		LogMsg( LOG_ERROR, "GetFileShareSettingsJavaClass: FAIL Thread 0x%x\n", uiThreadId );
		delete poClass;
		g_FileShareJavaListMutex.unlock();
		return NULL;		
	}

	// get java member id s
	LogMsg( LOG_INFO, "GetFileShareSettingsJavaClass: Getting ID s\n" );

	poClass->m_u32DownloadBandwidthID	= poClass->m_poEnv->GetFieldID(poClass->m_JavaClass, "m_u32DownloadBandwidth", "I");
	poClass->m_u32MaxDownloadingFilesID = poClass->m_poEnv->GetFieldID(poClass->m_JavaClass, "m_u32MaxDownloadingFiles", "I");

	poClass->m_u32UploadBandwidthID		= poClass->m_poEnv->GetFieldID(poClass->m_JavaClass, "m_u32UploadBandwidth", "I");
	poClass->m_u32MaxUploadingFilesID	= poClass->m_poEnv->GetFieldID(poClass->m_JavaClass, "m_u32MaxUploadingFiles", "I");
	poClass->m_bStartServerOnStartupID 	= poClass->m_poEnv->GetFieldID(poClass->m_JavaClass, "m_bStartServerOnStartup", "Z");

		LogMsg( LOG_INFO, "GetFileShareSettingsJavaClass: inserting pair\n" );
	g_oFileShareSettingsJavaMap.insert( std::make_pair(uiThreadId, poClass) );

	LogMsg( LOG_INFO, "GetFileShareSettingsJavaClass: Added thread 0x%x FileShareSettingsJavaClass 0x%x\n", uiThreadId, poClass );
	g_FileShareJavaListMutex.unlock();
	return poClass;
}

//============================================================================
bool Java2FileShareSettings( jobject& o, FileShareSettings& shareSettings )
{
	LogMsg( LOG_INFO, "Java2FileShareSettings start\n" );
	FileShareSettingsJavaClass * poClass = GetFileShareSettingsJavaClass();
	if( poClass )
	{
		jint jintDownloadBandwidth = (jint) poClass->m_poEnv->GetIntField( o, poClass->m_u32DownloadBandwidthID );
		shareSettings.setDownloadBandwidth( jintDownloadBandwidth );
		jint jintUploadBandwidth = (jint) poClass->m_poEnv->GetIntField( o, poClass->m_u32UploadBandwidthID );
		shareSettings.setUploadBandwidth( jintUploadBandwidth );

		jint jintDownloadMaxFile = (jint) poClass->m_poEnv->GetIntField( o, poClass->m_u32MaxDownloadingFilesID );
		shareSettings.setDownloadMaxFiles( jintDownloadMaxFile );
		jint jintUploadMaxFile = (jint) poClass->m_poEnv->GetIntField( o, poClass->m_u32MaxUploadingFilesID );
		shareSettings.setUploadMaxFiles( jintUploadMaxFile );

		jboolean bValue = poClass->m_poEnv->GetBooleanField( o, poClass->m_bStartServerOnStartupID );
		shareSettings.setStartServerOnStartup( bValue );
		
		return true;
	}
	else
	{
		LogMsg( LOG_INFO, "java2VxNetIdentBase: FAIL GetFileShareSettingsJavaClass\n" );
	}

    return false;
}

//============================================================================
static void javaFillFileShareSettingsClass( FileShareSettingsJavaClass * poClass, FileShareSettings& shareSettings, jobject& oFileShareSettingsClass )
{
	poClass->m_poEnv->SetIntField(oFileShareSettingsClass, poClass->m_u32DownloadBandwidthID, shareSettings.m_u32DownloadBandwidth );
	poClass->m_poEnv->SetIntField(oFileShareSettingsClass, poClass->m_u32MaxDownloadingFilesID, shareSettings.m_u32MaxDownloadingFiles );
	poClass->m_poEnv->SetIntField(oFileShareSettingsClass, poClass->m_u32UploadBandwidthID, shareSettings.m_u32UploadBandwidth );
	poClass->m_poEnv->SetIntField(oFileShareSettingsClass, poClass->m_u32MaxUploadingFilesID, shareSettings.m_u32MaxUploadingFiles );
	poClass->m_poEnv->SetBooleanField(oFileShareSettingsClass, poClass->m_bStartServerOnStartupID, shareSettings.m_bStartServerOnStartup );
}

//============================================================================
bool FileShareSettings2Java( FileShareSettings& netSettings, jobject& oFileShareSettingsClass )
{
	FileShareSettingsJavaClass * poClass = GetFileShareSettingsJavaClass();
	if( NULL == poClass )
	{
		LogMsg( LOG_INFO, "java2VxNetIdentBase: FAIL GetFileShareSettingsJavaClass\n" );
		return false;
	}

	oFileShareSettingsClass = poClass->creatNewObj();

	if( NULL == oFileShareSettingsClass )
	{
		LogMsg( LOG_ERROR, "FileShareSettings2Java: FAIL create a FileShareSettings object" );
		return false;
	}

	javaFillFileShareSettingsClass( poClass, netSettings, oFileShareSettingsClass );
	return true;
}


//============================================================================
bool FillFileShareSettingsJavaClass( FileShareSettings& netSettings, jobject& oFileShareSettingsClass )
{
	FileShareSettingsJavaClass * poClass = GetFileShareSettingsJavaClass();
	if( NULL == poClass )
	{
		LogMsg( LOG_INFO, "FillFileShareSettingsJavaClass: FAIL GetFileShareSettingsJavaClass\n" );
		return false;
	}

	if( NULL == oFileShareSettingsClass )
	{
		LogMsg( LOG_ERROR, "FillFileShareSettingsJavaClass: FAIL because NULL object" );
		return false;
	}

	javaFillFileShareSettingsClass( poClass, netSettings, oFileShareSettingsClass );
	return true;
}

#endif //TARGET_OS_ANDROID
