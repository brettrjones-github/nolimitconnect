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
#ifdef TARGET_OS_ANDROID

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
#include "NativeCommon.h"
#include "FriendJava.h"
#include "NetSettingsJava.h"
#include "AssetInfoJava.h"
#include "JavaHelpers.h"

#include <CoreLib/VxThread.h>
#include <CoreLib/VxDebug.h>
#include <CoreLib/VxGlobals.h>

#define NATIVE_TO_JAVA_TAG "NativeToJava:"
//#define DEBUG_TO_JAVA_ACCESS 1
//#ifndef _DEBUG
//	#define DISABLE_DEBUG_LOGGING 1
//#endif //_DEBUG	


namespace
{
	std::map<unsigned int, NativeToJavaClass *> g_oNativeToJavaMap;
	VxMutex										g_ToJavaAccessMutex;
	VxMutex										g_NativeToJavaListMutex;
	uint8_t g_HashNullBuf[20] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

	//=====================================================================================
	void LockToJavaAccess( int instanceId = 0 )
	{
#ifdef DEBUG_TO_JAVA_ACCESS
		if( 0 != instanceId )
		{
			MyAndroidPrint( ANDROID_LOG_INFO, NATIVE_TO_JAVA_TAG, "LockToJavaAccess %d", instanceId );
		}
#endif // DEBUG_TO_JAVA_ACCESS

		g_ToJavaAccessMutex.lock();
	}

	void UnlockToJavaAccess( int instanceId = 0 )
	{
#ifdef DEBUG_TO_JAVA_ACCESS
		if( 0 != instanceId )
		{
			MyAndroidPrint( ANDROID_LOG_INFO, NATIVE_TO_JAVA_TAG, "UnlockToJavaAccess %d", instanceId );
		}
#endif // DEBUG_TO_JAVA_ACCESS

		g_ToJavaAccessMutex.unlock();
	}

	//=====================================================================================
	void LockJavaListAccess( int instanceId = 0 )
	{
#ifdef DEBUG_TO_JAVA_ACCESS
		if( 0 != instanceId )
		{
			MyAndroidPrint( ANDROID_LOG_INFO, NATIVE_TO_JAVA_TAG, "LockJavaListAccess %d", instanceId );
		}
#endif // DEBUG_TO_JAVA_ACCESS

		g_NativeToJavaListMutex.lock();
	}

	void UnlockJavaListAccess( int instanceId = 0 )
	{
#ifdef DEBUG_TO_JAVA_ACCESS
		if( 0 != instanceId )
		{
			MyAndroidPrint( ANDROID_LOG_INFO, NATIVE_TO_JAVA_TAG, "UnlockJavaListAccess %d", instanceId );
		}
#endif // DEBUG_TO_JAVA_ACCESS

		g_NativeToJavaListMutex.unlock();
	}
}

//============================================================================
NativeToJavaClass::NativeToJavaClass(  JavaEnvCache& envCache, jclass javaRefClass )
: JavaBaseClass( envCache, javaRefClass, false, true )
{
}

//============================================================================
void NativeToJavaClassThreadExit( unsigned int uiThreadId, const char * threadName )
{
	//LogMsg( LOG_DEBUG, "NativeToJavaClassThreadExit %d start\n", uiThreadId );
	std::map<unsigned int, NativeToJavaClass *>::iterator iter;

	LockJavaListAccess( 1 );
	iter = g_oNativeToJavaMap.find( uiThreadId );
	if( iter != g_oNativeToJavaMap.end() )
	{
		//NativeToJavaClass * javaClass = iter->second;
		//LogMsg( LOG_DEBUG, "NativeToJavaClassThreadExit detaching thread with id %d env 0x%x\n", uiThreadId, javaClass->m_poEnv );
		delete iter->second;
		g_oNativeToJavaMap.erase( iter );
	}
	else
	{
		// BNOTE: cant log here or will attempt to attach thread again but this is fatal error anyway
		LogMsg( LOG_DEBUG, "NativeToJavaClassThreadExit id %d %s NOT FOUND\n", uiThreadId, threadName );
	}

	// BNOTE: cant log here or will attempt to attach thread again
	//LogMsg( LOG_DEBUG, "NativeToJavaClassThreadExit %d done\n", uiThreadId );
	UnlockJavaListAccess( 1 );
}

//============================================================================
void NativeToJavaClassShutdown( void )
{
	LockJavaListAccess( 11 );
	std::map<unsigned int, NativeToJavaClass *>::iterator iter;
	for( iter = g_oNativeToJavaMap.begin(); iter != g_oNativeToJavaMap.end(); ++iter )
	{
		delete iter->second;
	}

	g_oNativeToJavaMap.clear();	
	UnlockJavaListAccess( 11 );
}

//============================================================================
NativeToJavaClass * GetNativeToJavaClass( void )
{
	unsigned int uiThreadId = VxGetCurrentThreadTid();
	std::map<unsigned int, NativeToJavaClass *>::iterator iter;

	LockJavaListAccess( 12 );
	iter = g_oNativeToJavaMap.find(uiThreadId);
	if( iter != g_oNativeToJavaMap.end() )
	{
		//LogMsg( LOG_DEBUG, "###GetNativeToJavaClass: found class for thread %d JavaVM 0x%x\n", uiThreadId, g_poJavaVM );
		NativeToJavaClass * nativeJavaClass = iter->second;
		//for( iter = g_oNativeToJavaMap.begin(); iter != g_oNativeToJavaMap.end(); ++iter )
		//{
		//	LogMsg( LOG_DEBUG, "g_oNativeToJavaMap: thread %d\n", iter->first );
		//}

		UnlockJavaListAccess( 122222 );
		return nativeJavaClass;
	}

	// does not exist for this thread
	NativeToJavaClass * poClass = new NativeToJavaClass( GetJavaEnvCache(), GetJavaEnvCache().getClassNativeRxFrom() );
	if( false == poClass->attachToThread() )
	{
		MyAndroidPrint( ANDROID_LOG_ERROR, NATIVE_TO_JAVA_TAG, "###GetNativeToJavaClass: Unable to reference class NativeTxFrom" );
		delete poClass;
		UnlockJavaListAccess( 1222223 );
		return NULL;	
	}

	// get java method id's
	poClass->m_methToGuiLog 						= poClass->m_poEnv->GetStaticMethodID( (jclass)poClass->m_JavaClass, "toGuiLog", "(ILjava/lang/String;)V");
	// we insert here so can log after this point and will find the class and use the log method
	// if log before this point the infinite loop
	g_oNativeToJavaMap.insert( std::make_pair( uiThreadId, poClass ) );

	//MyAndroidPrint( ANDROID_LOG_INFO, NATIVE_TO_JAVA_TAG, "m_methLog 0x%x\n", poClass->m_methLog );
	poClass->m_methToGuiAppErr 						= poClass->m_poEnv->GetStaticMethodID( (jclass)poClass->m_JavaClass, "toGuiAppErr", "(ILjava/lang/String;)V");
	poClass->m_methToGuiStatusMessage 				= poClass->m_poEnv->GetStaticMethodID( (jclass)poClass->m_JavaClass, "toGuiStatusMessage",  "(Ljava/lang/String;)V");
	//MyAndroidPrint( ANDROID_LOG_INFO, NATIVE_TO_JAVA_TAG, "m_methtoGuiStatusMessage 0x%x\n", poClass->m_methToGuiStatusMessage );

	poClass->m_methToGuiWantMicrophoneRecording 	= poClass->m_poEnv->GetStaticMethodID( ( jclass )poClass->m_JavaClass, "toGuiWantMicrophoneRecording", "(Z)V");
	//MyAndroidPrint( ANDROID_LOG_INFO, NATIVE_TO_JAVA_TAG, "m_methToGuiWantMicrophoneRecording 0x%x\n", poClass->m_methToGuiWantMicrophoneRecording );
	poClass->m_methToGuiWantSpeakerOutput 			= poClass->m_poEnv->GetStaticMethodID( ( jclass )poClass->m_JavaClass, "toGuiWantSpeakerOutput", "(Z)V");
	//MyAndroidPrint( ANDROID_LOG_INFO, NATIVE_TO_JAVA_TAG, "m_methToGuiWantSpeakerOutput 0x%x\n", poClass->m_methToGuiWantSpeakerOutput );

	poClass->m_methToGuiWantVideoCapture 			= poClass->m_poEnv->GetStaticMethodID( ( jclass )poClass->m_JavaClass, "toGuiWantVideoCapture", "(Z)V");
	//MyAndroidPrint( ANDROID_LOG_INFO, NATIVE_TO_JAVA_TAG, "m_methToGuiWantVideoCapture 0x%x\n", poClass->m_methToGuiWantVideoCapture );
	poClass->m_methToGuiPlayVideoFrame 				= poClass->m_poEnv->GetStaticMethodID( ( jclass )poClass->m_JavaClass, "toGuiPlayVideoFrame", "(JJ[BII)V" );
	//MyAndroidPrint( ANDROID_LOG_INFO, NATIVE_TO_JAVA_TAG, "m_methToGuiPlayVideoFrame 0x%x\n", poClass->m_methToGuiPlayVideoFrame );

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

	poClass->m_methToGuiNetworkState 			= poClass->m_poEnv->GetStaticMethodID( (jclass)poClass->m_JavaClass, "toGuiNetworkState", "(ILjava/lang/String;)V");
	//MyAndroidPrint( ANDROID_LOG_INFO, NATIVE_TO_JAVA_TAG, "m_methToGuiNetworkState 0x%x\n", poClass->m_methToGuiNetworkState );
	poClass->m_methToGuiMyRelayStatus 			= poClass->m_poEnv->GetStaticMethodID( ( jclass )poClass->m_JavaClass, "toGuiMyRelayStatus", "(ILjava/lang/String;)V" );
	//MyAndroidPrint( ANDROID_LOG_INFO, NATIVE_TO_JAVA_TAG, "m_methToGuiMyRelayStatus 0x%x\n", poClass->m_methToGuiMyRelayStatus );
	poClass->m_methToGuiHostStatus 			= poClass->m_poEnv->GetStaticMethodID( ( jclass )poClass->m_JavaClass, "toGuiHostStatus", "(ILjava/lang/String;)V" );
	//MyAndroidPrint( ANDROID_LOG_INFO, NATIVE_TO_JAVA_TAG, "m_methToGuiHostStatus 0x%x\n", poClass->m_methToGuiHostStatus );
	poClass->m_methToGuiIsPortOpenStatus 		= poClass->m_poEnv->GetStaticMethodID( ( jclass )poClass->m_JavaClass, "toGuiIsPortOpenStatus", "(ILjava/lang/String;)V" );
    //MyAndroidPrint( ANDROID_LOG_INFO, NATIVE_TO_JAVA_TAG, "m_methToGuiHostStatus 0x%x\n", poClass->m_methToGuiHostStatus );
    //poClass->m_methToGuiIsPortOpenStatus        = poClass->m_poEnv->GetStaticMethodID( ( jclass )poClass->m_JavaClass, "toGuiIsPortOpenStatus", "(Ljava/lang/String;ILjava/lang/String;)V" );
    //MyAndroidPrint( ANDROID_LOG_INFO, NATIVE_TO_JAVA_TAG, "m_methToGuiIsPortOpenStatus 0x%x\n", poClass->m_methToGuiIsPortOpenStatus );
	poClass->m_methToGuiRandomConnectStatus 		= poClass->m_poEnv->GetStaticMethodID( ( jclass )poClass->m_JavaClass, "toGuiRandomConnectStatus", "(ILjava/lang/String;)V" );
	//MyAndroidPrint( ANDROID_LOG_INFO, NATIVE_TO_JAVA_TAG, "m_methToGuiRandomConnectStatus 0x%x\n", poClass->m_methToGuiRandomConnectStatus );

	poClass->m_methToGuiContactOnline 			= poClass->m_poEnv->GetStaticMethodID( ( jclass )poClass->m_JavaClass, "toGuiContactOnline", "(Lcom/waycoolapps/myp2pweb/VxNetIdent;Z)V" );
	//MyAndroidPrint( ANDROID_LOG_INFO, NATIVE_TO_JAVA_TAG, "m_methToGuiContactOnline 0x%x\n", poClass->m_methToGuiContactOnline );
	poClass->m_methToGuiContactOffline 			= poClass->m_poEnv->GetStaticMethodID( ( jclass )poClass->m_JavaClass, "toGuiContactOffline", "(Lcom/waycoolapps/myp2pweb/VxNetIdent;)V" );
	//MyAndroidPrint( ANDROID_LOG_INFO, NATIVE_TO_JAVA_TAG, "m_methToGuiContactOffline 0x%x\n", poClass->m_methToGuiContactOffline );
	poClass->m_methToGuiContactNearby 			= poClass->m_poEnv->GetStaticMethodID( ( jclass )poClass->m_JavaClass, "toGuiContactNearby", "(Lcom/waycoolapps/myp2pweb/VxNetIdent;)V" );
	//MyAndroidPrint( ANDROID_LOG_INFO, NATIVE_TO_JAVA_TAG, "m_methToGuiContactNearby 0x%x\n", poClass->m_methToGuiContactNearby );
	poClass->m_methToGuiContactNotNearby 		= poClass->m_poEnv->GetStaticMethodID( ( jclass )poClass->m_JavaClass, "toGuiContactNotNearby", "(Lcom/waycoolapps/myp2pweb/VxNetIdent;)V" );
	//MyAndroidPrint( ANDROID_LOG_INFO, NATIVE_TO_JAVA_TAG, "m_methToGuiContactNotNearby 0x%x\n", poClass->m_methToGuiContactNotNearby );

	poClass->m_methToGuiContactNameChange 		= poClass->m_poEnv->GetStaticMethodID( ( jclass )poClass->m_JavaClass, "toGuiContactNameChange", "(Lcom/waycoolapps/myp2pweb/VxNetIdent;)V" );
	//MyAndroidPrint( ANDROID_LOG_INFO, NATIVE_TO_JAVA_TAG, "m_methToGuiContactNameChange 0x%x\n", poClass->m_methToGuiContactNameChange );
	poClass->m_methToGuiContactDescChange 		= poClass->m_poEnv->GetStaticMethodID( ( jclass )poClass->m_JavaClass, "toGuiContactDescChange", "(Lcom/waycoolapps/myp2pweb/VxNetIdent;)V" );
	//MyAndroidPrint( ANDROID_LOG_INFO, NATIVE_TO_JAVA_TAG, "m_methToGuiContactDescChange 0x%x\n", poClass->m_methToGuiContactDescChange );
	poClass->m_methToGuiContactMyFriendshipChange 	= poClass->m_poEnv->GetStaticMethodID( ( jclass )poClass->m_JavaClass, "toGuiContactMyFriendshipChange", "(Lcom/waycoolapps/myp2pweb/VxNetIdent;)V" );
	//MyAndroidPrint( ANDROID_LOG_INFO, NATIVE_TO_JAVA_TAG, "m_methToGuiContactMyFriendshipChange 0x%x\n", poClass->m_methToGuiContactMyFriendshipChange );
	poClass->m_methToGuiContactHisFriendshipChange 	= poClass->m_poEnv->GetStaticMethodID( ( jclass )poClass->m_JavaClass, "toGuiContactHisFriendshipChange", "(Lcom/waycoolapps/myp2pweb/VxNetIdent;)V" );
	//MyAndroidPrint( ANDROID_LOG_INFO, NATIVE_TO_JAVA_TAG, "m_methToGuiContactHisFriendshipChange 0x%x\n", poClass->m_methToGuiContactHisFriendshipChange );
	poClass->m_methToGuiContactSearchFlagsChange 	= poClass->m_poEnv->GetStaticMethodID( ( jclass )poClass->m_JavaClass, "toGuiContactSearchFlagsChange", "(Lcom/waycoolapps/myp2pweb/VxNetIdent;)V" );
	//MyAndroidPrint( ANDROID_LOG_INFO, NATIVE_TO_JAVA_TAG, "m_methToGuiContactSearchFlagsChange 0x%x\n", poClass->m_methToGuiContactSearchFlagsChange );
	poClass->m_methToGuiContactConnectionChange 	= poClass->m_poEnv->GetStaticMethodID( ( jclass )poClass->m_JavaClass, "toGuiContactConnectionChange", "(Lcom/waycoolapps/myp2pweb/VxNetIdent;)V" );
	//MyAndroidPrint( ANDROID_LOG_INFO, NATIVE_TO_JAVA_TAG, "m_methToGuiContactConnectionChange 0x%x\n", poClass->m_methToGuiContactConnectionChange );
	poClass->m_methToGuiContactAnythingChange 		= poClass->m_poEnv->GetStaticMethodID( ( jclass )poClass->m_JavaClass, "toGuiContactAnythingChange", "(Lcom/waycoolapps/myp2pweb/VxNetIdent;)V" );
	//MyAndroidPrint( ANDROID_LOG_INFO, NATIVE_TO_JAVA_TAG, "m_methToGuiContactAnythingChange 0x%x\n", poClass->m_methToGuiContactAnythingChange );
	poClass->m_methToGuiContactLastSessionTimeChange = poClass->m_poEnv->GetStaticMethodID( ( jclass )poClass->m_JavaClass, "toGuiContactLastSessionTimeChange", "(Lcom/waycoolapps/myp2pweb/VxNetIdent;)V" );
	//MyAndroidPrint( ANDROID_LOG_INFO, NATIVE_TO_JAVA_TAG, "m_methToGuiContactLastSessionTimeChange 0x%x\n", poClass->m_methToGuiContactLastSessionTimeChange );

	poClass->m_methToGuiPluginPermissionChange 	= poClass->m_poEnv->GetStaticMethodID( ( jclass )poClass->m_JavaClass, "toGuiPluginPermissionChange", "(Lcom/waycoolapps/myp2pweb/VxNetIdent;)V" );
	//MyAndroidPrint( ANDROID_LOG_INFO, NATIVE_TO_JAVA_TAG, "m_methToGuiPluginPermissionChange 0x%x\n", poClass->m_methToGuiPluginPermissionChange );

	poClass->m_methToGuiUpdateMyIdent 			= poClass->m_poEnv->GetStaticMethodID( ( jclass )poClass->m_JavaClass, "toGuiUpdateMyIdent", "(Lcom/waycoolapps/myp2pweb/VxNetIdent;)V" );
	//MyAndroidPrint( ANDROID_LOG_INFO, NATIVE_TO_JAVA_TAG, "m_methToGuiUpdateMyIdent 0x%x\n", poClass->m_methToGuiUpdateMyIdent );

	poClass->m_methToGuiRxedPluginOffer 		= poClass->m_poEnv->GetStaticMethodID( ( jclass )poClass->m_JavaClass, "toGuiRxedPluginOffer", "(Lcom/waycoolapps/myp2pweb/VxNetIdent;ILjava/lang/String;ILjava/lang/String;[BJJJJ)V" );
	//MyAndroidPrint( ANDROID_LOG_INFO, NATIVE_TO_JAVA_TAG, "m_methToGuiRxedPluginOffer 0x%x\n", poClass->m_methToGuiRxedPluginOffer );
	poClass->m_methToGuiRxedOfferReply 			= poClass->m_poEnv->GetStaticMethodID( ( jclass )poClass->m_JavaClass, "toGuiRxedOfferReply", "(Lcom/waycoolapps/myp2pweb/VxNetIdent;IIILjava/lang/String;[BJJJJ)V" );
	//MyAndroidPrint( ANDROID_LOG_INFO, NATIVE_TO_JAVA_TAG, "m_methToGuiRxedOfferReply 0x%x\n", poClass->m_methToGuiRxedOfferReply );
	poClass->m_methToGuiSessionEnded 			= poClass->m_poEnv->GetStaticMethodID( ( jclass )poClass->m_JavaClass, "toGuiPluginSessionEnded", "(Lcom/waycoolapps/myp2pweb/VxNetIdent;IIIJJ)V" );
	//MyAndroidPrint( ANDROID_LOG_INFO, NATIVE_TO_JAVA_TAG, "m_methToGuiSessionEnded 0x%x\n", poClass->m_methToGuiSessionEnded );
	poClass->m_methToGuiPluginStatus 			= poClass->m_poEnv->GetStaticMethodID( ( jclass )poClass->m_JavaClass, "toGuiPluginStatus", "(III)V" );
	//MyAndroidPrint( ANDROID_LOG_INFO, NATIVE_TO_JAVA_TAG, "m_methToGuiPluginStatus 0x%x\n", poClass->m_methToGuiPluginStatus );

	poClass->m_methToGuiInstMsg 				= poClass->m_poEnv->GetStaticMethodID( ( jclass )poClass->m_JavaClass, "toGuiInstMsg", "(Lcom/waycoolapps/myp2pweb/VxNetIdent;ILjava/lang/String;)V");
	//MyAndroidPrint( ANDROID_LOG_INFO, NATIVE_TO_JAVA_TAG, "m_methToGuiInstMsg 0x%x\n", poClass->m_methToGuiInstMsg );

	poClass->m_methToGuiFileList 				= poClass->m_poEnv->GetStaticMethodID( ( jclass )poClass->m_JavaClass, "toGuiFileList", "(Ljava/lang/String;JIZZ[B)V" );
	//MyAndroidPrint( ANDROID_LOG_INFO, NATIVE_TO_JAVA_TAG, "m_methToGuiFileList 0x%x\n", poClass->m_methToGuiFileList );
	poClass->m_methToGuiFileListReply 			= poClass->m_poEnv->GetStaticMethodID( ( jclass )poClass->m_JavaClass, "toGuiFileListReply", "(Lcom/waycoolapps/myp2pweb/VxNetIdent;IIJLjava/lang/String;[B)V" );
	//MyAndroidPrint( ANDROID_LOG_INFO, NATIVE_TO_JAVA_TAG, "m_methToGuiFileListReply 0x%x\n", poClass->m_methToGuiFileListReply );

	poClass->m_methToGuiStartUpload 			= poClass->m_poEnv->GetStaticMethodID( ( jclass )poClass->m_JavaClass, "toGuiStartUpload", "(Lcom/waycoolapps/myp2pweb/VxNetIdent;IJJIJLjava/lang/String;[B)V" );
	//MyAndroidPrint( ANDROID_LOG_INFO, NATIVE_TO_JAVA_TAG, "m_methToGuiStartUpload 0x%x\n", poClass->m_methToGuiStartUpload );
	poClass->m_methToGuiStartDownload 			= poClass->m_poEnv->GetStaticMethodID( ( jclass )poClass->m_JavaClass, "toGuiStartDownload", "(Lcom/waycoolapps/myp2pweb/VxNetIdent;IJJIJLjava/lang/String;[B)V" );
	//MyAndroidPrint( ANDROID_LOG_INFO, NATIVE_TO_JAVA_TAG, "m_methToGuiStartDownload 0x%x\n", poClass->m_methToGuiStartDownload );
	poClass->m_methToGuiFileXferState 			= poClass->m_poEnv->GetStaticMethodID( ( jclass )poClass->m_JavaClass, "toGuiFileXferState", "(JJIII)V" );
	//MyAndroidPrint( ANDROID_LOG_INFO, NATIVE_TO_JAVA_TAG, "m_methToGuiFileXferState 0x%x\n", poClass->m_methToGuiFileXferState );
	poClass->m_methToGuiFileDownloadComplete 	= poClass->m_poEnv->GetStaticMethodID( ( jclass )poClass->m_JavaClass, "toGuiFileDownloadComplete", "(JJLjava/lang/String;I)V" );
	//MyAndroidPrint( ANDROID_LOG_INFO, NATIVE_TO_JAVA_TAG, "m_methToGuiFileDownloadComplete 0x%x\n", poClass->m_methToGuiFileDownloadComplete );
	poClass->m_methToGuiFileUploadComplete 		= poClass->m_poEnv->GetStaticMethodID( ( jclass )poClass->m_JavaClass, "toGuiFileUploadComplete", "(JJI)V");
	//MyAndroidPrint( ANDROID_LOG_INFO, NATIVE_TO_JAVA_TAG, "m_methToGuiFileUploadComplete 0x%x\n", poClass->m_methToGuiFileUploadComplete );

	poClass->m_methToGuiScanSearchComplete 		= poClass->m_poEnv->GetStaticMethodID( ( jclass )poClass->m_JavaClass, "toGuiScanSearchComplete", "(I)V");
	//MyAndroidPrint( ANDROID_LOG_INFO, NATIVE_TO_JAVA_TAG, "m_methToGuiScanSearchComplete 0x%x\n", poClass->m_methToGuiScanSearchComplete );
	poClass->m_methToGuiSearchResultSuccess 	= poClass->m_poEnv->GetStaticMethodID( ( jclass )poClass->m_JavaClass, "toGuiScanSearchResultSuccess", "(ILcom/waycoolapps/myp2pweb/VxNetIdent;)V");
	//MyAndroidPrint( ANDROID_LOG_INFO, NATIVE_TO_JAVA_TAG, "m_methToGuiSearchResultSuccess 0x%x\n", poClass->m_methToGuiSearchResultSuccess );
	poClass->m_methToGuiSearchResultError 		= poClass->m_poEnv->GetStaticMethodID( ( jclass )poClass->m_JavaClass, "toGuiScanSearchResultError", "(ILcom/waycoolapps/myp2pweb/VxNetIdent;I)V");
	//MyAndroidPrint( ANDROID_LOG_INFO, NATIVE_TO_JAVA_TAG, "m_methToGuiSearchResultError 0x%x\n", poClass->m_methToGuiSearchResultError );
	poClass->m_methSearchResultProfilePic 		= poClass->m_poEnv->GetStaticMethodID( ( jclass )poClass->m_JavaClass, "toGuiSearchResultProfilePic", "(Lcom/waycoolapps/myp2pweb/VxNetIdent;[BI)V");
	//MyAndroidPrint( ANDROID_LOG_INFO, NATIVE_TO_JAVA_TAG, "m_methSearchResultProfilePic 0x%x\n", poClass->m_methSearchResultProfilePic );
	poClass->m_methSearchResultFileSearch 		= poClass->m_poEnv->GetStaticMethodID( ( jclass )poClass->m_JavaClass, "toGuiSearchResultFileSearch", "(Lcom/waycoolapps/myp2pweb/VxNetIdent;JJIJLjava/lang/String;)V");
	//MyAndroidPrint( ANDROID_LOG_INFO, NATIVE_TO_JAVA_TAG, "m_methSearchResultFileSearch 0x%x\n", poClass->m_methSearchResultFileSearch );

	poClass->m_methToGuiSetGameValueVar 		= poClass->m_poEnv->GetStaticMethodID( ( jclass )poClass->m_JavaClass, "toGuiSetGameValueVar", "(IJJII)V");
	//MyAndroidPrint( ANDROID_LOG_INFO, NATIVE_TO_JAVA_TAG, "m_methToGuiSetGameValueVar 0x%x\n", poClass->m_methToGuiSetGameValueVar );
	poClass->m_methToGuiSetGameActionVar 		= poClass->m_poEnv->GetStaticMethodID( ( jclass )poClass->m_JavaClass, "toGuiSetGameActionVar", "(IJJII)V");
	//MyAndroidPrint( ANDROID_LOG_INFO, NATIVE_TO_JAVA_TAG, "m_methToGuiSetGameActionVar 0x%x\n", poClass->m_methToGuiSetGameActionVar );
	poClass->m_methToGuiGenerateGuidHiPart 		= poClass->m_poEnv->GetStaticMethodID( ( jclass )poClass->m_JavaClass, "toGuiGenerateGuidHiPart", "()J");
	//MyAndroidPrint( ANDROID_LOG_INFO, NATIVE_TO_JAVA_TAG, "m_methToGuiGenerateGuidHiPart 0x%x\n", poClass->m_methToGuiGenerateGuidHiPart );
	poClass->m_methToGuiGenerateGuidLoPart 		= poClass->m_poEnv->GetStaticMethodID( ( jclass )poClass->m_JavaClass, "toGuiGenerateGuidLoPart", "()J");
	//MyAndroidPrint( ANDROID_LOG_INFO, NATIVE_TO_JAVA_TAG, "m_methToGuiGenerateGuidLoPart 0x%x\n", poClass->m_methToGuiGenerateGuidLoPart );

	poClass->m_methToGuiAssetAdded 				= poClass->m_poEnv->GetStaticMethodID( ( jclass )poClass->m_JavaClass, "toGuiAssetAdded", "(Lcom/waycoolapps/myp2pweb/AssetGuiInfo;)V" );
	//MyAndroidPrint( ANDROID_LOG_INFO, NATIVE_TO_JAVA_TAG, "m_methToGuiAssetAdded 0x%x\n", poClass->m_methToGuiGuiAssetAdded );
	poClass->m_methToGuiSessionHistory 			= poClass->m_poEnv->GetStaticMethodID( ( jclass )poClass->m_JavaClass, "toGuiAssetSessionHistory", "(Lcom/waycoolapps/myp2pweb/AssetGuiInfo;)V" );
	//MyAndroidPrint( ANDROID_LOG_INFO, NATIVE_TO_JAVA_TAG, "m_methToGuiSessionHistory 0x%x\n", poClass->m_methToGuiSessionHistory );
	poClass->m_methToGuiAssetAction 			= poClass->m_poEnv->GetStaticMethodID( ( jclass )poClass->m_JavaClass, "toGuiAssetAction", "(IJJI)V" );
	//MyAndroidPrint( ANDROID_LOG_INFO, NATIVE_TO_JAVA_TAG, "m_methToGuiAssetAction 0x%x\n", poClass->m_methToGuiAssetAction );
	poClass->m_methToGuiMultiSessionAction 			= poClass->m_poEnv->GetStaticMethodID( ( jclass )poClass->m_JavaClass, "toGuiMultiSessionAction", "(IJJI)V" );
	//MyAndroidPrint( ANDROID_LOG_INFO, NATIVE_TO_JAVA_TAG, "m_methToGuiMultiSessionAction 0x%x\n", poClass->m_methToGuiMultiSessionAction );

	UnlockJavaListAccess( 12 );
	return poClass;
}

//============================================================================
void JavaToGuiLog( int logLevel, const char * logMsg )
{
	if( logMsg )
	{
		// there is a problem in that we need lock to avoid thread issues but log is used everywhere.. so rather than making que just do android log here
		if( 0 != (logLevel & 0xffff0000 ) )
		{
			// has plugin number.. send on to gui
			// don't allow logging while trying to send log or will get infinite loop and stack overflow
			bool debugIsEnabled = VxIsDebugEnabled();
			VxSetDebugEnable( false );

			// cant use from java mutex
			LockToJavaAccess( 13 );

			NativeToJavaClass * poClass = GetNativeToJavaClass();
			if( poClass )
			{
				jint jiPluginType = (jint)logLevel;
				jstring jstrMsg;
				jstrMsg = poClass->m_poEnv->NewStringUTF(logMsg);
				if( jstrMsg )
				{
					poClass->m_poEnv->CallStaticVoidMethod( poClass->m_JavaClass, poClass->m_methToGuiLog, jiPluginType, jstrMsg );
					poClass->m_poEnv->DeleteLocalRef(jstrMsg);
				}
			}

			UnlockToJavaAccess( 13 );
			VxSetDebugEnable( debugIsEnabled );
		}
		else
		{
			switch( logLevel )
			{
			case LOG_INFO:
#ifdef DISABLE_DEBUG_LOGGING
				return; // don't do logging in release mode
#endif //	DISABLE_DEBUG_LOGGING
				__android_log_write( ANDROID_LOG_INFO, NATIVE_TO_JAVA_TAG, logMsg);
				break;
				
			case LOG_FATAL:
			case LOG_ASSERT:
			case LOG_ERROR:
				__android_log_write(ANDROID_LOG_ERROR, NATIVE_TO_JAVA_TAG, logMsg);
				break;
				
			case LOG_WARN:
				__android_log_write(ANDROID_LOG_WARN, NATIVE_TO_JAVA_TAG, logMsg);
				break;
				
			case LOG_VERBOSE:
#ifdef DISABLE_DEBUG_LOGGING
				return; // don't do logging in release mode
#endif //	DISABLE_DEBUG_LOGGING
				__android_log_write(ANDROID_LOG_DEBUG, NATIVE_TO_JAVA_TAG, logMsg);
				break;
				
			default:
#ifdef DISABLE_DEBUG_LOGGING
				return; // don't do logging in release mode
#endif //	DISABLE_DEBUG_LOGGING
				__android_log_write(ANDROID_LOG_DEBUG, NATIVE_TO_JAVA_TAG, logMsg);
			}
		}
	}
}

//============================================================================
void JavaToGuiAppErr( int eAppErr, const char * pMsg )
{
	if( pMsg )
	{
		LockToJavaAccess( 31 );
		NativeToJavaClass * poClass = GetNativeToJavaClass();
		if( poClass )
		{
			jint jiAppErr = (jint)eAppErr;
			jstring jstrMsg;
			jstrMsg = poClass->m_poEnv->NewStringUTF(pMsg);
			if( jstrMsg )
			{
				poClass->m_poEnv->CallStaticVoidMethod( poClass->m_JavaClass, poClass->m_methToGuiAppErr, jiAppErr, jstrMsg );
				poClass->m_poEnv->DeleteLocalRef(jstrMsg);
			}
		}

		UnlockToJavaAccess( 31 );
	}
}

//============================================================================
void JavaToGuiStatusMessage( const char * pMsg )
{
	if( pMsg )
	{
		LockToJavaAccess( 32 );
		NativeToJavaClass * poClass = GetNativeToJavaClass();
		if( poClass )
		{
			jstring jstrMsg;
			jstrMsg = poClass->m_poEnv->NewStringUTF(pMsg);
			if( jstrMsg )
			{
				poClass->m_poEnv->CallStaticVoidMethod( poClass->m_JavaClass, poClass->m_methToGuiStatusMessage, jstrMsg );
				poClass->m_poEnv->DeleteLocalRef(jstrMsg);
			}
		}

		UnlockToJavaAccess( 32 );
	}
}

//============================================================================
void JavaToGuiWantMicrophoneRecording( bool wantMicInput )
{
	LockToJavaAccess( 33 );
	NativeToJavaClass * poClass = GetNativeToJavaClass();
	if( poClass )
	{
		jboolean jwantInput = ( jboolean )wantMicInput;
		poClass->m_poEnv->CallStaticVoidMethod(poClass->m_JavaClass, poClass->m_methToGuiWantMicrophoneRecording, jwantInput);
	}

	UnlockToJavaAccess( 33 );
}

//============================================================================
void JavaToGuiWantSpeakerOutput( bool wantSpeakerOutput )
{
	//LogMsg( LOG_DEBUG, "JavaToGuiWantSpeakerOutput start\n" );
	LockToJavaAccess( 34 );
	NativeToJavaClass * poClass = GetNativeToJavaClass();
	if( poClass )
	{
		jboolean jwantOutput = ( jboolean )wantSpeakerOutput;
		poClass->m_poEnv->CallStaticVoidMethod(poClass->m_JavaClass, poClass->m_methToGuiWantSpeakerOutput, jwantOutput);
	}

	UnlockToJavaAccess( 34 );
	//LogMsg( LOG_DEBUG, "JavaToGuiWantSpeakerOutput done\n" );
}

//============================================================================
void JavaToGuiWantVideoCapture( bool wantVidCapture )
{
	LockToJavaAccess( 35 );
	NativeToJavaClass * poClass = GetNativeToJavaClass();
	if( poClass )
	{
		jboolean jwantCapture = (jboolean)wantVidCapture;
		poClass->m_poEnv->CallStaticVoidMethod( poClass->m_JavaClass, poClass->m_methToGuiWantVideoCapture, jwantCapture );
	}

	UnlockToJavaAccess( 35 );
}

//============================================================================
void JavaToGuiPlayVideoFrame( VxGUID& onlineId, uint8_t * pu8Jpg, uint32_t u32JpgDataLen, int motion0to100000 )
{
	LockToJavaAccess( 36 );
	NativeToJavaClass * poClass = GetNativeToJavaClass();
	if( poClass )
	{
		jint vidDataLen = (int)u32JpgDataLen;
		jbyteArray vidDataArray = poClass->m_poEnv->NewByteArray( vidDataLen );
		uint64_t hiIdPart = onlineId.getVxGUIDHiPart();
		uint64_t loIdPart = onlineId.getVxGUIDLoPart();
		//LogMsg( LOG_DEBUG, "JavaToGuiPlayVideoFrame len %d hi 0x%llx lo 0x%llx\n", u32JpgDataLen, hiIdPart, loIdPart );

		poClass->m_poEnv->SetByteArrayRegion( vidDataArray, 0, vidDataLen, (jbyte *)pu8Jpg );
		poClass->m_poEnv->CallStaticVoidMethod( poClass->m_JavaClass,
												poClass->m_methToGuiPlayVideoFrame,
												hiIdPart,
												loIdPart,
												vidDataArray,
												vidDataLen,
												motion0to100000 );
		poClass->m_poEnv->DeleteLocalRef( vidDataArray );
	}
	
	UnlockToJavaAccess( 36 );
}

//============================================================================
void JavaToGuiNetworkState( ENetworkStateType eNetworkState, const char* stateMsg )
{
	LockToJavaAccess( 37 );
	NativeToJavaClass * poClass = GetNativeToJavaClass();
	if( poClass )
	{
		jint jiState = (jint)eNetworkState;
		jstring jstrMsg;
		jstrMsg = poClass->m_poEnv->NewStringUTF(stateMsg);
		poClass->m_poEnv->CallStaticVoidMethod( poClass->m_JavaClass, poClass->m_methToGuiNetworkState, jiState, jstrMsg );
		poClass->m_poEnv->DeleteLocalRef(jstrMsg);
	}

	UnlockToJavaAccess( 37 );
}

//============================================================================
void JavaToGuiMyRelayStatus( EMyRelayStatus eRelayStatus, const char * msg )
{
	LockToJavaAccess( 38 );
	NativeToJavaClass * poClass = GetNativeToJavaClass();
	if( poClass )
	{
		jint jiStatus = (jint)eRelayStatus;
		jstring jstrMsg;
		jstrMsg = poClass->m_poEnv->NewStringUTF(msg);
		poClass->m_poEnv->CallStaticVoidMethod( poClass->m_JavaClass, poClass->m_methToGuiMyRelayStatus, jiStatus, jstrMsg );
		poClass->m_poEnv->DeleteLocalRef(jstrMsg);
	}

	UnlockToJavaAccess( 38 );
}

//============================================================================
void JavaToGuiHostStatus( EHostTestStatus eHostStatus, const char * msg )
{
	LockToJavaAccess( 39 );
	NativeToJavaClass * poClass = GetNativeToJavaClass();
	if( poClass )
	{
		jint jiStatus = (jint)eHostStatus;
		jstring jstrMsg;
		jstrMsg = poClass->m_poEnv->NewStringUTF(msg);
		poClass->m_poEnv->CallStaticVoidMethod( poClass->m_JavaClass, poClass->m_methToGuiHostStatus, jiStatus, jstrMsg );
		poClass->m_poEnv->DeleteLocalRef(jstrMsg);
	}

	UnlockToJavaAccess( 39 );
}

//============================================================================
void JavaToGuiIsPortOpenStatus( EIsPortOpenStatus eIsPortOpenStatus, const char * msg )
{
	LockToJavaAccess( 311 );
	NativeToJavaClass * poClass = GetNativeToJavaClass();
	if( poClass )
	{
		jint jiStatus = (jint)eIsPortOpenStatus;
		jstring jstrMsg;
		jstrMsg = poClass->m_poEnv->NewStringUTF(msg);
		poClass->m_poEnv->CallStaticVoidMethod( poClass->m_JavaClass, poClass->m_methToGuiIsPortOpenStatus, jiStatus, jstrMsg );
		poClass->m_poEnv->DeleteLocalRef(jstrMsg);
	}

	UnlockToJavaAccess( 311 );
}

//============================================================================
void JavaToGuiRandomConnectStatus( ERandomConnectStatus eRandomConnectStatus, const char * msg )
{
	LockToJavaAccess( 322 );
	NativeToJavaClass * poClass = GetNativeToJavaClass();
	if( poClass )
	{
		jint jiStatus = (jint)eRandomConnectStatus;
		jstring jstrMsg;
		jstrMsg = poClass->m_poEnv->NewStringUTF(msg);
		poClass->m_poEnv->CallStaticVoidMethod( poClass->m_JavaClass, poClass->m_methToGuiRandomConnectStatus, jiStatus, jstrMsg );
		poClass->m_poEnv->DeleteLocalRef(jstrMsg);
	}

	UnlockToJavaAccess( 322 );
}

//============================================================================
void JavaToGuiContactOnline( VxNetIdent * netIdent, bool newContact )
{
	LockToJavaAccess( 333 );
	//MyAndroidPrint( ANDROID_LOG_INFO, NATIVE_TO_JAVA_TAG, "JavaToGuiContactOnline\n" );

	jobject jContact = NULL;
	vxNetIdentBase2Java( netIdent, jContact );
	if( NULL != jContact )
	{
		NativeToJavaClass * poClass = GetNativeToJavaClass();
		if( poClass )
		{
			//MyAndroidPrint( ANDROID_LOG_INFO, NATIVE_TO_JAVA_TAG, "JavaToGuiContactOnline calling into java start %d\n", poClass->m_methToGuiContactOnline );
			poClass->m_poEnv->CallStaticVoidMethod( poClass->m_JavaClass, poClass->m_methToGuiContactOnline, jContact, (jboolean)newContact );
			//MyAndroidPrint( ANDROID_LOG_INFO, NATIVE_TO_JAVA_TAG, "JavaToGuiContactOnline calling into java done %d\n", poClass->m_methToGuiContactOnline );
		}
		else
		{
			MyAndroidPrint( ANDROID_LOG_INFO, NATIVE_TO_JAVA_TAG, "JavaToGuiContactOnline ERROR NULL ToJavaClass \n" );
		}
	}
	else
	{
		MyAndroidPrint( ANDROID_LOG_INFO, NATIVE_TO_JAVA_TAG, "JavaToGuiContactOnline ERROR NULL jContact \n" );
	}

	UnlockToJavaAccess( 333 );
}

//============================================================================
void JavaToGuiContactOffline( VxNetIdent * netIdent )
{
	LockToJavaAccess( 344 );
	//MyAndroidPrint( ANDROID_LOG_INFO, NATIVE_TO_JAVA_TAG, "JavaToGuiContactOffline\n" );

	jobject jContact = NULL;
	vxNetIdentBase2Java( netIdent, jContact );
	if( NULL != jContact )
	{
		NativeToJavaClass * poClass = GetNativeToJavaClass();
		if( poClass )
		{
			poClass->m_poEnv->CallStaticVoidMethod( poClass->m_JavaClass, poClass->m_methToGuiContactOffline, jContact );
		}
	}

	UnlockToJavaAccess( 344 );
}

//============================================================================
void JavaToGuiContactNearby( VxNetIdent * netIdent )
{
	LockToJavaAccess( 355 );
	MyAndroidPrint( ANDROID_LOG_INFO, NATIVE_TO_JAVA_TAG, "JavaToGuiContactNearby\n" );

	jobject jContact = NULL;
	vxNetIdentBase2Java( netIdent, jContact );
	if( NULL != jContact )
	{
		NativeToJavaClass * poClass = GetNativeToJavaClass();
		if( poClass )
		{
			poClass->m_poEnv->CallStaticVoidMethod( poClass->m_JavaClass, poClass->m_methToGuiContactNearby, jContact );
		}
	}

	UnlockToJavaAccess( 355 );
}

//============================================================================
void JavaToGuiContactNotNearby( VxNetIdent * netIdent )
{
	LockToJavaAccess( 366 );
	MyAndroidPrint( ANDROID_LOG_INFO, NATIVE_TO_JAVA_TAG, "JavaToGuiContactNotNearby\n" );

	jobject jContact = NULL;
	vxNetIdentBase2Java( netIdent, jContact );
	if( NULL != jContact )
	{
		NativeToJavaClass * poClass = GetNativeToJavaClass();
		if( poClass )
		{
			poClass->m_poEnv->CallStaticVoidMethod( poClass->m_JavaClass, poClass->m_methToGuiContactNotNearby, jContact );
		}
	}

	UnlockToJavaAccess( 366 );
}

//============================================================================
//! called when contact changes
//============================================================================
//============================================================================
void JavaToGuiContactNameChange( VxNetIdent * netIdent )
{
	LockToJavaAccess( 377 );
	MyAndroidPrint( ANDROID_LOG_INFO, NATIVE_TO_JAVA_TAG, "JavaToGuiContactNameChange\n" );

	jobject jContact = NULL;
	vxNetIdentBase2Java( netIdent, jContact );
	if( NULL != jContact )
	{
		NativeToJavaClass * poClass = GetNativeToJavaClass();
		if( poClass )
		{
			poClass->m_poEnv->CallStaticVoidMethod( poClass->m_JavaClass, poClass->m_methToGuiContactNameChange, jContact );
		}
	}

	UnlockToJavaAccess( 377 );
}

//============================================================================
void JavaToGuiContactDescChange( VxNetIdent * netIdent )
{
	LockToJavaAccess( 388 );
	MyAndroidPrint( ANDROID_LOG_INFO, NATIVE_TO_JAVA_TAG, "JavaToGuiContactDescChange\n" );

	jobject jContact = NULL;
	vxNetIdentBase2Java( netIdent, jContact );
	if( NULL != jContact )
	{
		NativeToJavaClass * poClass = GetNativeToJavaClass();
		if( poClass )
		{
			poClass->m_poEnv->CallStaticVoidMethod( poClass->m_JavaClass, poClass->m_methToGuiContactDescChange, jContact );
		}
	}

	UnlockToJavaAccess( 388 );
}

//============================================================================
void JavaToGuiContactMyFriendshipChange( VxNetIdent * netIdent )
{
	MyAndroidPrint( ANDROID_LOG_INFO, NATIVE_TO_JAVA_TAG, "JavaToGuiContactMyFriendshipChange\n" );
	LockToJavaAccess( 399 );

	jobject jContact = NULL;
	vxNetIdentBase2Java( netIdent, jContact );
	if( NULL != jContact )
	{
		NativeToJavaClass * poClass = GetNativeToJavaClass();
		if( poClass )
		{
			poClass->m_poEnv->CallStaticVoidMethod( poClass->m_JavaClass, poClass->m_methToGuiContactMyFriendshipChange, jContact );
		}
	}

	UnlockToJavaAccess( 399 );
}

//============================================================================
void JavaToGuiContactHisFriendshipChange( VxNetIdent * netIdent )
{
	MyAndroidPrint( ANDROID_LOG_INFO, NATIVE_TO_JAVA_TAG, "JavaToGuiContactHisFriendshipChange\n" );
	LockToJavaAccess( 41 );

	jobject jContact = NULL;
	vxNetIdentBase2Java( netIdent, jContact );
	if( NULL != jContact )
	{
		NativeToJavaClass * poClass = GetNativeToJavaClass();
		if( poClass )
		{
			poClass->m_poEnv->CallStaticVoidMethod( poClass->m_JavaClass, poClass->m_methToGuiContactHisFriendshipChange, jContact );
		}
	}

	UnlockToJavaAccess( 41 );
}

//============================================================================
void JavaToGuiContactSearchFlagsChange( VxNetIdent * netIdent )
{
	MyAndroidPrint( ANDROID_LOG_INFO, NATIVE_TO_JAVA_TAG, "JavaToGuiContactSearchFlagsChange\n" );
	LockToJavaAccess( 42 );

	jobject jContact = NULL;
	vxNetIdentBase2Java( netIdent, jContact );
	if( NULL != jContact )
	{
		NativeToJavaClass * poClass = GetNativeToJavaClass();
		if( poClass )
		{
			poClass->m_poEnv->CallStaticVoidMethod( poClass->m_JavaClass, poClass->m_methToGuiContactSearchFlagsChange, jContact );
		}
	}

	UnlockToJavaAccess( 42 );
}

//============================================================================
void JavaToGuiContactConnectionChange( VxNetIdent * netIdent )
{
	MyAndroidPrint( ANDROID_LOG_INFO, NATIVE_TO_JAVA_TAG, "JavaToGuiContactConnectionChange\n" );
	LockToJavaAccess( 43 );

	jobject jContact = NULL;
	vxNetIdentBase2Java( netIdent, jContact );
	if( NULL != jContact )
	{
		NativeToJavaClass * poClass = GetNativeToJavaClass();
		if( poClass )
		{
			poClass->m_poEnv->CallStaticVoidMethod( poClass->m_JavaClass, poClass->m_methToGuiContactConnectionChange, jContact );
		}
	}

	UnlockToJavaAccess( 43 );
}

//============================================================================
void JavaToGuiContactAnythingChange( VxNetIdent * netIdent )
{
	MyAndroidPrint( ANDROID_LOG_INFO, NATIVE_TO_JAVA_TAG, "JavaToGuiContactAnythingChange\n" );
	LockToJavaAccess( 53 );

	jobject jContact = NULL;
	vxNetIdentBase2Java( netIdent, jContact );
	if( NULL != jContact )
	{
		NativeToJavaClass * poClass = GetNativeToJavaClass();
		if( poClass )
		{
			poClass->m_poEnv->CallStaticVoidMethod( poClass->m_JavaClass, poClass->m_methToGuiContactAnythingChange, jContact );
		}
	}
;
	UnlockToJavaAccess( 53 );
}

//============================================================================
void JavaToGuiContactLastSessionTimeChange( VxNetIdent * netIdent )
{
	MyAndroidPrint( ANDROID_LOG_INFO, NATIVE_TO_JAVA_TAG, "JavaToGuiContactLastSessionTimeChange\n" );
	LockToJavaAccess( 63 );

	jobject jContact = NULL;
	vxNetIdentBase2Java( netIdent, jContact );
	if( NULL != jContact )
	{
		NativeToJavaClass * poClass = GetNativeToJavaClass();
		if( poClass )
		{
			poClass->m_poEnv->CallStaticVoidMethod( poClass->m_JavaClass, poClass->m_methToGuiContactLastSessionTimeChange, jContact );
		}
	}

	UnlockToJavaAccess( 63 );
}

//============================================================================
void JavaToGuiPluginPermissionChange( VxNetIdent * netIdent )
{
	MyAndroidPrint( ANDROID_LOG_INFO, NATIVE_TO_JAVA_TAG, "JavaToGuiPluginPermissionChange\n" );
	LockToJavaAccess( 73 );

	jobject jContact = NULL;
	vxNetIdentBase2Java( netIdent, jContact );
	if( NULL != jContact )
	{
		NativeToJavaClass * poClass = GetNativeToJavaClass();
		if( poClass )
		{
			poClass->m_poEnv->CallStaticVoidMethod( poClass->m_JavaClass, poClass->m_methToGuiPluginPermissionChange, jContact );
		}
	}

	UnlockToJavaAccess( 73 );
}

//============================================================================
//! called from engine when need to update identity
void JavaToGuiUpdateMyIdent( VxNetIdent * netIdent )
{
	//MyAndroidPrint( ANDROID_LOG_INFO, NATIVE_TO_JAVA_TAG, "JavaToGuiUpdateMyIdent\n" );
	LockToJavaAccess( 83 );

	jobject jContact = NULL;
	vxNetIdentBase2Java( netIdent, jContact );
	if( NULL != jContact )
	{
		NativeToJavaClass * poClass = GetNativeToJavaClass();
		if( poClass )
		{
			poClass->m_poEnv->CallStaticVoidMethod( poClass->m_JavaClass, poClass->m_methToGuiUpdateMyIdent, jContact );
		}
	}

	UnlockToJavaAccess( 83 );
}

//============================================================================
//! add offer to notify list
void JavaToGuiRxedPluginOffer(  VxNetIdent *		netIdent,
								EPluginType			ePluginType,
								const char *		pMsg,
								int					pvUserData,
								const char *		pFileName,
								uint8_t *				fileHashData,
								VxGUID&				lclSessionId,
								VxGUID&				rmtSessionId )
{
	MyAndroidPrint( ANDROID_LOG_INFO, NATIVE_TO_JAVA_TAG, "JavaToGuiRxedPluginOffer\n" );
	LockToJavaAccess( 93 );

	jobject jContact = NULL;
	vxNetIdentBase2Java( netIdent, jContact );
	if( NULL != jContact )
	{
		NativeToJavaClass * poClass = GetNativeToJavaClass();
		if( poClass )
		{
			int iPluginType = (int)ePluginType;
			jstring jstrMsg;
			jstrMsg = poClass->m_poEnv->NewStringUTF(pMsg);
			int iUserData = (int)pvUserData;
			jstring jstrFileName;
			if (pFileName)
			{
				jstrFileName = poClass->m_poEnv->NewStringUTF( pFileName );				
			}
			else
			{
				jstrFileName = poClass->m_poEnv->NewStringUTF( "" );				
			}

			jbyteArray hashDataArray = poClass->m_poEnv->NewByteArray( 20 );
			if( fileHashData )
			{
				poClass->m_poEnv->SetByteArrayRegion( hashDataArray, 0, 20, ( jbyte * )fileHashData );	
			}
			else
			{
				poClass->m_poEnv->SetByteArrayRegion( hashDataArray, 0, 20, ( jbyte * )g_HashNullBuf );	
			}


			poClass->m_poEnv->CallStaticVoidMethod( poClass->m_JavaClass, poClass->m_methToGuiRxedPluginOffer, jContact, iPluginType, jstrMsg, iUserData, 
													jstrFileName,
													hashDataArray,
													lclSessionId.getVxGUIDHiPart(),
													lclSessionId.getVxGUIDLoPart(),
													rmtSessionId.getVxGUIDHiPart(),
													rmtSessionId.getVxGUIDLoPart()
													);

			//poClass->m_poEnv->ReleaseByteArrayElements( hashDataArray, (jbyte *)g_HashNullBuf, JNI_ABORT );
			poClass->m_poEnv->DeleteLocalRef( hashDataArray );
			poClass->m_poEnv->DeleteLocalRef(jstrMsg);
			poClass->m_poEnv->DeleteLocalRef(jstrFileName);
		}
	}

	UnlockToJavaAccess( 93 );
	MyAndroidPrint( ANDROID_LOG_INFO, NATIVE_TO_JAVA_TAG, "JavaToGuiRxedPluginOffer done\n" );
}

//============================================================================
//! response to offer
void JavaToGuiRxedOfferReply(	VxNetIdent *	netIdent,
								EPluginType		ePluginType,
								int				pvUserData,
								EOfferResponse	eOfferResponse,
									const char *	pFileNameIn,
									uint8_t *			fileHashData,
									VxGUID&			lclSessionId,
									VxGUID&			rmtSessionId )
{
	//MyAndroidPrint( ANDROID_LOG_INFO, NATIVE_TO_JAVA_TAG, "javaFromPluginRxedPluginOffer\n" );
	LockToJavaAccess( 51 );
	const char * pFileName = pFileNameIn;
	if( NULL == pFileName )
	{
		pFileName = "";
	}

	//MyAndroidPrint( ANDROID_LOG_INFO, NATIVE_TO_JAVA_TAG, "JavaToGuiRxedOfferReply filename %s\n", pFileName);
	jobject jContact = NULL;
	vxNetIdentBase2Java( netIdent, jContact );
	if( NULL != jContact )
	{
		NativeToJavaClass * poClass = GetNativeToJavaClass();
		if( poClass )
		{
			jint jiPluginType = (jint)ePluginType;
			jint jiUserData = (jint)pvUserData;
			jint jeOfferResponse = (jint)eOfferResponse;
			jstring jstrFileName;
			MyAndroidPrint( ANDROID_LOG_INFO, NATIVE_TO_JAVA_TAG, "javaFromPluginRxedPluginOffer plugin type %d user data %d response %d method %d\n", 
				ePluginType, pvUserData, eOfferResponse, poClass->m_methToGuiRxedOfferReply );
			if (pFileName)
			{
				jstrFileName = poClass->m_poEnv->NewStringUTF( pFileName );				
			}
			else
			{
				jstrFileName = poClass->m_poEnv->NewStringUTF( "" );				
			}
			
			jbyteArray hashDataArray = poClass->m_poEnv->NewByteArray( 20 );
			if (fileHashData)
			{
				poClass->m_poEnv->SetByteArrayRegion( hashDataArray, 0, 20, ( jbyte * )fileHashData );	
			}
			else
			{
				poClass->m_poEnv->SetByteArrayRegion( hashDataArray, 0, 20, ( jbyte * )g_HashNullBuf );	
			}

			poClass->m_poEnv->CallStaticVoidMethod( poClass->m_JavaClass, poClass->m_methToGuiRxedOfferReply, 
														jContact, jiPluginType, jiUserData, jeOfferResponse, 
														jstrFileName, 
														hashDataArray,
														lclSessionId.getVxGUIDHiPart(),
														lclSessionId.getVxGUIDLoPart(),
														rmtSessionId.getVxGUIDHiPart(),
														rmtSessionId.getVxGUIDLoPart()  );

			poClass->m_poEnv->DeleteLocalRef( hashDataArray );
			poClass->m_poEnv->DeleteLocalRef(jstrFileName);
		}
	}

	UnlockToJavaAccess( 51 );
	MyAndroidPrint( ANDROID_LOG_INFO, NATIVE_TO_JAVA_TAG, "JavaToGuiRxedOfferReply done \n" );
}

//============================================================================
void JavaToGuiPluginSessionEnded( 	VxNetIdent *	netIdent,
									EPluginType		ePluginType,
									int				pvUserData,
									EOfferResponse	eOfferResponse,
									VxGUID&			lclSessionId )
{
	LockToJavaAccess( 52 );
	//MyAndroidPrint( ANDROID_LOG_INFO, NATIVE_TO_JAVA_TAG, "JavaToGuiPluginSessionEnded\n" );

	jobject jContact = NULL;
	vxNetIdentBase2Java( netIdent, jContact );
	if( NULL != jContact )
	{
		NativeToJavaClass * poClass = GetNativeToJavaClass();
		if( poClass )
		{
			jint jiPluginType = (jint)ePluginType;
			jint jiUserData = (jint)pvUserData;
			jint jeOfferResponse = (jint)eOfferResponse;

			poClass->m_poEnv->CallStaticVoidMethod( poClass->m_JavaClass, poClass->m_methToGuiSessionEnded, jContact, jiPluginType, jiUserData, 
													jeOfferResponse,
													lclSessionId.getVxGUIDHiPart(),
													lclSessionId.getVxGUIDLoPart() );
		}
	}

	UnlockToJavaAccess( 52 );
}

//============================================================================
void JavaToGuiPluginStatus( 	EPluginType		ePluginType,
								int				statusType,
								int				statusValue )
{
	LockToJavaAccess( 53 );
	NativeToJavaClass * poClass = GetNativeToJavaClass();
	if( poClass )
	{
		poClass->m_poEnv->CallStaticVoidMethod( poClass->m_JavaClass, poClass->m_methToGuiPluginStatus, 
					(jint)ePluginType, (jint)statusType, (jint)statusValue );
	}

	UnlockToJavaAccess( 53 );
}

//============================================================================
void JavaToGuiInstMsg( 		VxNetIdent *	netIdent,
							EPluginType		ePluginType,
							const char *	pMsg )
{
	LockToJavaAccess( 54 );
	jobject jContact = NULL;
	vxNetIdentBase2Java( netIdent, jContact );
	if( NULL != jContact )
	{
		NativeToJavaClass * poClass = GetNativeToJavaClass();
		if( poClass )
		{
			jstring jstrMsg;
			jstrMsg = poClass->m_poEnv->NewStringUTF(pMsg);
			poClass->m_poEnv->CallStaticVoidMethod( poClass->m_JavaClass, poClass->m_methToGuiInstMsg, jContact, (jint)ePluginType, jstrMsg );
			poClass->m_poEnv->DeleteLocalRef(jstrMsg);
		}
	}

	UnlockToJavaAccess( 54 );
}

//============================================================================
void JavaToGuiFileList(	const char *	fileName,
						uint64_t				fileLen,
						uint8_t				fileType,
						bool			isShared,
						bool			isInLibrary,
						uint8_t *			fileHashData )
{
	MyAndroidPrint( ANDROID_LOG_INFO, NATIVE_TO_JAVA_TAG, "JavaToGuiFileList\n" );
	LockToJavaAccess( 55 );
	NativeToJavaClass * poClass = GetNativeToJavaClass();
	if( poClass )
	{
		jint jintFileType 	= fileType; // java freaks out with 0x80 in byte value so has to be int
		jlong ju64FileLen	= ( jlong )fileLen;
		jboolean fileShared = isShared;
		jboolean fileInLib 	= isInLibrary;
		jstring jstrFileName;
		jstrFileName = poClass->m_poEnv->NewStringUTF( fileName );
		jbyteArray hashDataArray = poClass->m_poEnv->NewByteArray( 20 );
		if (fileHashData)
		{
			poClass->m_poEnv->SetByteArrayRegion( hashDataArray, 0, 20, ( jbyte * )fileHashData );
		}
		else
		{
			poClass->m_poEnv->SetByteArrayRegion( hashDataArray, 0, 20, ( jbyte * )g_HashNullBuf );
		}

		poClass->m_poEnv->CallStaticVoidMethod( poClass->m_JavaClass,
			poClass->m_methToGuiFileList,
			jstrFileName,
			ju64FileLen,
			jintFileType,
			fileShared,
			fileInLib,
			hashDataArray );

		poClass->m_poEnv->DeleteLocalRef( hashDataArray );
		poClass->m_poEnv->DeleteLocalRef(jstrFileName);
	}

	UnlockToJavaAccess( 55 );
}

//============================================================================
void JavaToGuiFileListReply(	VxNetIdent *	netIdent, 
								EPluginType		ePluginType,
								uint8_t				u8FileType,
								uint64_t				u64FileLen,
								const char *	pFileName,
								uint8_t *			fileHashData )
{
	MyAndroidPrint( ANDROID_LOG_INFO, NATIVE_TO_JAVA_TAG, "JavaToGuiFileListReply\n" );
	LockToJavaAccess( 56 );
	jobject jContact = NULL;
	vxNetIdentBase2Java( netIdent, jContact ); 
	if( NULL != jContact )
	{
		NativeToJavaClass * poClass = GetNativeToJavaClass();
		if( poClass )
		{
			jint jiPluginType		= ( jint )ePluginType;
			jint jiFileType			= ( jint )u8FileType ;
			jlong ju64FileLen		= ( jlong )(u64FileLen);
			jstring jstrFileName;
			jstrFileName = poClass->m_poEnv->NewStringUTF(pFileName);
			jbyteArray hashDataArray = poClass->m_poEnv->NewByteArray( 20 );
			if (fileHashData)
			{
				poClass->m_poEnv->SetByteArrayRegion( hashDataArray, 0, 20, ( jbyte * )fileHashData );	
			}
			else
			{
				poClass->m_poEnv->SetByteArrayRegion( hashDataArray, 0, 20, ( jbyte * )g_HashNullBuf );	
			}

			poClass->m_poEnv->CallStaticVoidMethod( poClass->m_JavaClass,
													poClass->m_methToGuiFileListReply,
													jContact,
													jiPluginType,
													jiFileType,
													ju64FileLen,
													jstrFileName,
													hashDataArray );
			poClass->m_poEnv->DeleteLocalRef( hashDataArray );
			poClass->m_poEnv->DeleteLocalRef(jstrFileName);
		}
	}

	UnlockToJavaAccess( 57 );
}

//============================================================================
void JavaToGuiStartUpload(	VxNetIdent *	netIdent,
							EPluginType		ePluginType,
							VxGUID&			fileInstanceId,
							uint8_t				u8FileType,
							uint64_t				u64FileLen,
							const char *	pFileName,
							uint8_t *			fileHashData )
{
	MyAndroidPrint( ANDROID_LOG_INFO, NATIVE_TO_JAVA_TAG, "JavaToGuiStartUpload\n" );
	LockToJavaAccess( 58 );
	jobject jContact = NULL;
	vxNetIdentBase2Java( netIdent, jContact );
	if( NULL != jContact )
	{
		NativeToJavaClass * poClass = GetNativeToJavaClass();
		if( poClass )
		{
			jint jiPluginType		= (jint)ePluginType;
			jlong fileInstanceIdHi	= (jlong)fileInstanceId.getVxGUIDHiPart();
			jlong fileInstanceIdLo	= (jlong)fileInstanceId.getVxGUIDLoPart();
			jint jiFileType			= ( jint )u8FileType;
			jlong ju64FileLen;
			ju64FileLen				= (jlong)(u64FileLen);
			jstring jstrFileName;
			jstrFileName = poClass->m_poEnv->NewStringUTF(pFileName);
			jbyteArray hashDataArray = poClass->m_poEnv->NewByteArray( 20 );
			if ( fileHashData )
			{
				poClass->m_poEnv->SetByteArrayRegion( hashDataArray, 0, 20, ( jbyte * )fileHashData );	
			}
			else
			{
				poClass->m_poEnv->SetByteArrayRegion( hashDataArray, 0, 20, ( jbyte * )g_HashNullBuf );	
			}

			poClass->m_poEnv->CallStaticVoidMethod( poClass->m_JavaClass,
													poClass->m_methToGuiStartUpload,
													jContact,
													jiPluginType,
													fileInstanceIdHi,
													fileInstanceIdLo,
													jiFileType,
													ju64FileLen,
													jstrFileName,
													hashDataArray );
			poClass->m_poEnv->DeleteLocalRef( hashDataArray );
			poClass->m_poEnv->DeleteLocalRef( jstrFileName );
		}
	}

	UnlockToJavaAccess( 58 );
}

//============================================================================
void	JavaToGuiStartDownload( VxNetIdent *	netIdent,
								EPluginType		ePluginType,
								VxGUID&			fileInstanceId,
								uint8_t				u8FileType,
								uint64_t				u64FileLen,
								const char *	pFileName,
								uint8_t *			fileHashData )
{
	MyAndroidPrint( ANDROID_LOG_INFO, NATIVE_TO_JAVA_TAG, "JavaToGuiStartDownload\n" );
	LockToJavaAccess( 59 );
	jobject jContact = NULL;
	vxNetIdentBase2Java( netIdent, jContact );
	if( NULL != jContact )
	{
		NativeToJavaClass * poClass = GetNativeToJavaClass();
		if( poClass )
		{
			jint jiPluginType		= (jint)ePluginType;
			jlong fileInstanceIdHi	= (jlong)fileInstanceId.getVxGUIDHiPart();
			jlong fileInstanceIdLo	= (jlong)fileInstanceId.getVxGUIDLoPart();
			jint jiFileType			= (int)u8FileType;
			jlong ju64FileLen		= (jlong)(u64FileLen);
			jstring jstrFileName;
			jstrFileName			= poClass->m_poEnv->NewStringUTF(pFileName);
			jbyteArray hashDataArray = poClass->m_poEnv->NewByteArray( 20 );
			if ( fileHashData )
			{
				poClass->m_poEnv->SetByteArrayRegion( hashDataArray, 0, 20, ( jbyte * )fileHashData );	
			}
			else
			{
				poClass->m_poEnv->SetByteArrayRegion( hashDataArray, 0, 20, ( jbyte * )g_HashNullBuf );	
			}

			poClass->m_poEnv->CallStaticVoidMethod(		poClass->m_JavaClass,
														poClass->m_methToGuiStartDownload,
														jContact,
														jiPluginType,
														fileInstanceIdHi,
														fileInstanceIdLo,
														jiFileType,
														ju64FileLen,
														jstrFileName,
														hashDataArray );
			poClass->m_poEnv->DeleteLocalRef( hashDataArray );
			poClass->m_poEnv->DeleteLocalRef(jstrFileName);
		}
	}
	
	UnlockToJavaAccess( 59 );
}

//============================================================================
void	JavaToGuiFileXferState( VxGUID&	lclSessionId, EXferState eXferState, int param1, int param2 )
{
	//MyAndroidPrint( ANDROID_LOG_INFO, NATIVE_TO_JAVA_TAG, "JavaToGuiUpdateFileDownload\n" );
	LockToJavaAccess( 60 );
	NativeToJavaClass * poClass = GetNativeToJavaClass();
	if( poClass )
	{
		jlong fileInstanceIdHi	= ( jlong )lclSessionId.getVxGUIDHiPart();
		jlong fileInstanceIdLo	= ( jlong )lclSessionId.getVxGUIDLoPart();
		jint jXferState			= ( jint )eXferState;
		jint jparam1			= ( jint )param1;
		jint jparam2			= ( jint )param2;
		poClass->m_poEnv->CallStaticVoidMethod( poClass->m_JavaClass, poClass->m_methToGuiFileXferState, fileInstanceIdHi, fileInstanceIdLo, jXferState, jparam1, jparam2 );
	}

	UnlockToJavaAccess( 60 );
}

//============================================================================
void	JavaToGuiFileDownloadComplete( VxGUID& lclSessionId, const char * newFileName, EXferError xferError )
{
	MyAndroidPrint( ANDROID_LOG_INFO, NATIVE_TO_JAVA_TAG, "JavaToGuiFileDownloadComplete\n" );
	LockToJavaAccess( 61 );
	NativeToJavaClass * poClass = GetNativeToJavaClass();
	if( poClass )
	{
		jlong fileInstanceIdHi	= ( jlong )lclSessionId.getVxGUIDHiPart();
		jlong fileInstanceIdLo	= ( jlong )lclSessionId.getVxGUIDLoPart();
		jint jXferError			= ( jint )xferError;
		jstring jstrFileName = poClass->m_poEnv->NewStringUTF( newFileName );
		poClass->m_poEnv->CallStaticVoidMethod( poClass->m_JavaClass, poClass->m_methToGuiFileDownloadComplete, fileInstanceIdHi, fileInstanceIdLo, jstrFileName, jXferError );
		poClass->m_poEnv->DeleteLocalRef( jstrFileName );
	}
	
	UnlockToJavaAccess( 61 );
}

//============================================================================
void	JavaToGuiFileUploadComplete( VxGUID& lclSessionId, EXferError xferError )
{
	LockToJavaAccess( 62 );
	MyAndroidPrint( ANDROID_LOG_INFO, NATIVE_TO_JAVA_TAG, "JavaToGuiFileUploadComplete\n" );
	NativeToJavaClass * poClass = GetNativeToJavaClass();
	if( poClass )
	{
		jlong fileInstanceIdHi	= ( jlong )lclSessionId.getVxGUIDHiPart();
		jlong fileInstanceIdLo	= ( jlong )lclSessionId.getVxGUIDLoPart();
		jint jXferError			= ( jint )xferError;
		poClass->m_poEnv->CallStaticVoidMethod( poClass->m_JavaClass, poClass->m_methToGuiFileUploadComplete, fileInstanceIdHi, fileInstanceIdLo, jXferError );
	}

	UnlockToJavaAccess( 62 );
}

//============================================================================
//=== scan ===//
//============================================================================
//============================================================================
void	JavaToGuiScanSearchComplete( EScanType eScanType )
{
	MyAndroidPrint( ANDROID_LOG_INFO, NATIVE_TO_JAVA_TAG, "JavaToGuiScanSearchComplete\n" );
	LockToJavaAccess( 63 );
	NativeToJavaClass * poClass = GetNativeToJavaClass();
	if( poClass )
	{
		jint jeScanType = (jint)eScanType;

		poClass->m_poEnv->CallStaticVoidMethod( poClass->m_JavaClass, poClass->m_methToGuiScanSearchComplete, jeScanType );
	}

	UnlockToJavaAccess( 63 );
}

//============================================================================
void	JavaToGuiSearchResultSuccess( EScanType eScanType, VxNetIdent *	netIdent )
{
	LockToJavaAccess( 64 );
	jobject jContact = NULL;
	vxNetIdentBase2Java( netIdent, jContact );
	if( jContact )
	{
		NativeToJavaClass * poClass = GetNativeToJavaClass();
		if( poClass )
		{
			jint jeScanType = (jint)eScanType;

			poClass->m_poEnv->CallStaticVoidMethod( poClass->m_JavaClass, poClass->m_methToGuiSearchResultSuccess, jeScanType, jContact );
		}
	}

	UnlockToJavaAccess( 64 );
}

//============================================================================
void	JavaToGuiSearchResultError( EScanType eScanType, VxNetIdent * netIdent, int errCode )
{
	LockToJavaAccess( 65 );
	jobject jContact = NULL;
	vxNetIdentBase2Java( netIdent, jContact );
	if( jContact )
	{
		NativeToJavaClass * poClass = GetNativeToJavaClass();
		if( poClass )
		{
			jint jeScanType = (jint)eScanType;
			jint jeErrCode 	= (jint)errCode;

			poClass->m_poEnv->CallStaticVoidMethod( poClass->m_JavaClass, poClass->m_methToGuiSearchResultError, jeScanType, jContact, jeErrCode );
		}
	}

	UnlockToJavaAccess( 65 );
}

//============================================================================
void	JavaToGuiSearchResultProfilePic(	VxNetIdent *	netIdent,
											uint8_t *			pu8JpgData,
											uint32_t				u32JpgDataLen )
{
	LogMsg( 0, "JavaToGuiSearchResultProfilePic: len %d\n", u32JpgDataLen );
	LockToJavaAccess( 67 );
	jobject jContact = NULL;
	vxNetIdentBase2Java( netIdent, jContact );
	if( jContact )
	{
		NativeToJavaClass * poClass = GetNativeToJavaClass();
		if( poClass )
		{
			jint picDataLen = (int)u32JpgDataLen;
			jbyteArray picDataArray = poClass->m_poEnv->NewByteArray( picDataLen );

			poClass->m_poEnv->SetByteArrayRegion( picDataArray, 0, picDataLen, (jbyte *)pu8JpgData );

			poClass->m_poEnv->CallStaticVoidMethod( poClass->m_JavaClass,
				poClass->m_methSearchResultProfilePic,
				jContact,
				picDataArray,
				picDataLen );

			poClass->m_poEnv->DeleteLocalRef( picDataArray );
			//LogMsg( 0, "javaPlayVideoFrame done\n" );
		}
	}

	UnlockToJavaAccess( 67 );
}

//============================================================================
void	JavaToGuiSearchResultFileSearch(
										VxNetIdent *		netIdent,
										VxGUID&				fileInstanceId,
										uint8_t					u8FileType,
										uint64_t					u64FileLen,
										const char *		pFileName )
{
	MyAndroidPrint( ANDROID_LOG_INFO, NATIVE_TO_JAVA_TAG, "JavaToGuiSearchResultFileSearch\n" ); 
	LockToJavaAccess( 68 );
	jobject jContact = NULL;
	vxNetIdentBase2Java( netIdent, jContact );
	if( NULL != jContact )
	{
		NativeToJavaClass * poClass = GetNativeToJavaClass();
		if( poClass )
		{
			jlong fileInstanceIdHi = (jlong)fileInstanceId.getVxGUIDHiPart();
			jlong fileInstanceIdLo = (jlong)fileInstanceId.getVxGUIDLoPart();
			jint jiFileType = u8FileType;
			jlong ju64FileLen = (jlong)u64FileLen;
			jstring jstrFileName;
			jstrFileName = poClass->m_poEnv->NewStringUTF(pFileName);

			poClass->m_poEnv->CallStaticVoidMethod( poClass->m_JavaClass, 
				poClass->m_methSearchResultFileSearch, 
				jContact,
				fileInstanceIdHi,
				fileInstanceIdLo,
				jiFileType,
				ju64FileLen,
				jstrFileName
				);

			poClass->m_poEnv->DeleteLocalRef( jstrFileName );
		}
	}
	UnlockToJavaAccess( 68 );
}

//============================================================================
bool	JavaToGuiSetGameValueVar(	EPluginType		ePluginType,
									VxGUID&		oOnlineId, 
								   int32_t				s32VarId,
								   int32_t				s32VarValue )
{
	MyAndroidPrint( ANDROID_LOG_INFO, NATIVE_TO_JAVA_TAG, "JavaToGuiSetGameValueVar\n" );
	LockToJavaAccess( 69 );
	NativeToJavaClass * poClass = GetNativeToJavaClass();
	if( poClass )
	{
		poClass->m_poEnv->CallStaticVoidMethod( poClass->m_JavaClass, 
			poClass->m_methToGuiSetGameValueVar, 
			ePluginType,
			oOnlineId.getVxGUIDHiPart(),
			oOnlineId.getVxGUIDLoPart(),
			s32VarId,
			s32VarValue );
	}

	UnlockToJavaAccess( 69 );
	return true;
}

//============================================================================
bool	JavaToGuiSetGameActionVar(	EPluginType		ePluginType,
									VxGUID&			oOnlineId, 
									int32_t				s32VarId,
									int32_t				s32VarValue )
{
	MyAndroidPrint( ANDROID_LOG_INFO, NATIVE_TO_JAVA_TAG, "JavaToGuiSetGameActionVar\n" );
	LockToJavaAccess( 70 );
	NativeToJavaClass * poClass = GetNativeToJavaClass();
	if( poClass )
	{
		poClass->m_poEnv->CallStaticVoidMethod( poClass->m_JavaClass,  
			poClass->m_methToGuiSetGameActionVar, 
			ePluginType,
			oOnlineId.getVxGUIDHiPart(),
			oOnlineId.getVxGUIDLoPart(),
			s32VarId,
			s32VarValue );
	}

	UnlockToJavaAccess( 70 );
	return true;
}

//============================================================================
void JavaToGuiGenerateGuid( VxGUID& retGuid )
{
	MyAndroidPrint( ANDROID_LOG_INFO, NATIVE_TO_JAVA_TAG, "JavaToGuiGenerateGuid\n" );
	//LockToJavaAccess( 3 ); // NOTE: don't think need to protect a non interacting static function
	NativeToJavaClass * poClass = GetNativeToJavaClass();
	if( poClass )
	{
		uint64_t hiPart = (uint64_t)poClass->m_poEnv->CallStaticLongMethod( poClass->m_JavaClass,  
					poClass->m_methToGuiGenerateGuidHiPart );
		uint64_t loPart = (uint64_t)poClass->m_poEnv->CallStaticLongMethod( poClass->m_JavaClass,  
			poClass->m_methToGuiGenerateGuidLoPart );
		retGuid.setVxGUID( hiPart, loPart );
	}

	//UnlockToJavaAccess( 3 )
}

//============================================================================
void JavaToGuiAssetAdded( AssetInfo * assetInfo )
{
	MyAndroidPrint( ANDROID_LOG_INFO, NATIVE_TO_JAVA_TAG, "JavaToGuiAssetAdded\n" );
	LockToJavaAccess( 72 );
	jobject jAssetInfo = NULL;
	AssetInfo2Java( assetInfo, jAssetInfo );
	if (NULL != jAssetInfo)
	{
		NativeToJavaClass * poClass = GetNativeToJavaClass();
		if (poClass)
		{
			poClass->m_poEnv->CallStaticVoidMethod( poClass->m_JavaClass, poClass->m_methToGuiAssetAdded, jAssetInfo );
			poClass->m_poEnv->DeleteLocalRef( jAssetInfo );
		}
	}

	UnlockToJavaAccess( 72 );
}

//============================================================================
void JavaToGuiSessionHistory( AssetInfo * assetInfo )
{
	MyAndroidPrint( ANDROID_LOG_INFO, NATIVE_TO_JAVA_TAG, "JavaToGuiSessionHistory start\n" );
	LockToJavaAccess( 73 );
	jobject jAssetInfo = NULL;
	AssetInfo2Java( assetInfo, jAssetInfo );
	if( NULL != jAssetInfo )
	{
		NativeToJavaClass * poClass = GetNativeToJavaClass();
		if (poClass)
		{
			poClass->m_poEnv->CallStaticVoidMethod( poClass->m_JavaClass, poClass->m_methToGuiAssetAdded, jAssetInfo );
			poClass->m_poEnv->DeleteLocalRef( jAssetInfo );
		}
	}

	UnlockToJavaAccess( 73 );
	MyAndroidPrint( ANDROID_LOG_INFO, NATIVE_TO_JAVA_TAG, "JavaToGuiSessionHistory done\n" );
}

//============================================================================
void JavaToGuiAssetAction(	EAssetAction	assetAction, 
							VxGUID&			assetId, 
							int				pos0to100000 )
{
	//MyAndroidPrint( ANDROID_LOG_INFO, NATIVE_TO_JAVA_TAG, "JavaToGuiAssetAction start\n" );
	LockToJavaAccess( 74 );
	NativeToJavaClass * poClass = GetNativeToJavaClass();
	if (poClass)
	{
		poClass->m_poEnv->CallStaticVoidMethod( poClass->m_JavaClass,  
			poClass->m_methToGuiAssetAction,
			assetAction,
			assetId.getVxGUIDHiPart(),
			assetId.getVxGUIDLoPart(),
			pos0to100000 );
	}

	UnlockToJavaAccess( 74 );
	//MyAndroidPrint( ANDROID_LOG_INFO, NATIVE_TO_JAVA_TAG, "JavaToGuiAssetAction done\n" );
}

//============================================================================
void JavaToGuiMultiSessionAction( EMSessionAction mSessionAction, VxGUID& onlineId, int pos0to100000 )
{
	//MyAndroidPrint( ANDROID_LOG_INFO, NATIVE_TO_JAVA_TAG, "JavaToGuiGetGuid\n" );
	LockToJavaAccess( 75 );
	NativeToJavaClass * poClass = GetNativeToJavaClass();
	if ( poClass )
	{
		poClass->m_poEnv->CallStaticVoidMethod( poClass->m_JavaClass,  
			poClass->m_methToGuiMultiSessionAction,
			mSessionAction,
			onlineId.getVxGUIDHiPart(),
			onlineId.getVxGUIDLoPart(),
			pos0to100000 );
	}

	UnlockToJavaAccess( 75 );	
}


#endif //TARGET_OS_ANDROID



