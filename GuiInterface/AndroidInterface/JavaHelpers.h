#pragma once
//============================================================================
// Copyright (C) 2013 Brett R. Jones
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

#include <CoreLib/VxMutex.h>
// android specific code
#ifdef TARGET_OS_ANDROID
#include "NativeCommon.h"

#define  NATIVE_LOG_TAG    "MyP2PNative"
void MyAndroidPrint( int prio, const char *tag, const char *fmt, ... );


bool JavaClassCreate( JNIEnv *env, const char * pObjPath, jobject * retJobj ) ;

class JavaEnvCache
{
public:
    JavaVM *                    getJavaVM( void )                       { return m_JavaVM; }
    JNIEnv *                    getJavaEnv( void )                      { return m_JavaEnv; }
	jobject						getJavaContext( void )                  { return m_JavaContext; }

	const char *				getJavaClassString( jclass refClass );

    jclass                      getClassNativeRxFrom( void )            { return m_NativeRxFrom; }
    jclass                      getClassAssetGuiInfo( void )            { return m_AssetGuiInfo; }
    jclass                      getClassGuidId( void )                  { return m_GuidId; }
    jclass                      getClassFriendBase( void )              { return m_FriendBase; }
    jclass                      getClassFileShareSettings( void )       { return m_FileShareSettings; }
    jclass                      getClassNetSettings( void )             { return m_NetSettings; }
	jclass                      getClassBuildInfo( void )				{ return m_BuildInfo; }
	jclass                      getClassAudioManager( void )            { return m_WebRtcAudioManager; }
	jclass                      getClassAudioRecord( void )             { return m_WebRtcAudioRecord; }
	jclass                      getClassAudioPlayback( void )           { return m_WebRtcAudioPlayback; }


    static JavaVM *             m_JavaVM;
    static JNIEnv *             m_JavaEnv;
	static jobject				m_JavaContext;

    static jclass               m_NativeRxFrom;
    static jclass               m_AssetGuiInfo;
    static jclass               m_GuidId;
    static jclass               m_FriendBase;
    static jclass               m_FileShareSettings;
    static jclass               m_NetSettings;
	static jclass               m_BuildInfo;
	static jclass               m_WebRtcAudioManager;
	static jclass               m_WebRtcAudioRecord;
	static jclass               m_WebRtcAudioPlayback;
};

JavaEnvCache& 		GetJavaEnvCache( void );

extern VxMutex	    g_FromJavaAccessMutex;

jlong PointerTojlong(void* ptr);

#endif // TARGET_OS_ANDROID
