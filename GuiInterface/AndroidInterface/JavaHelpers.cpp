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

// android specific code
#ifdef TARGET_OS_ANDROID
#include "NativeCommon.h"

#include <stdarg.h>
#include <stdio.h>

VxMutex			g_FromJavaAccessMutex;
JavaEnvCache	g_JavaEnvCache;

JavaVM * 		JavaEnvCache::m_JavaVM					= 0;
JNIEnv * 		JavaEnvCache:: m_JavaEnv				= 0;
jobject 		JavaEnvCache:: m_JavaContext			= 0;

jclass 			JavaEnvCache::m_NativeRxFrom			= 0;
jclass 			JavaEnvCache::m_AssetGuiInfo			= 0;
jclass 			JavaEnvCache::m_GuidId					= 0;
jclass 			JavaEnvCache::m_FriendBase				= 0;
jclass 			JavaEnvCache::m_FileShareSettings		= 0;
jclass 			JavaEnvCache::m_NetSettings				= 0;

jclass 			JavaEnvCache::m_BuildInfo				= 0;
jclass 			JavaEnvCache::m_WebRtcAudioManager		= 0;
jclass 			JavaEnvCache::m_WebRtcAudioRecord		= 0;
jclass 			JavaEnvCache::m_WebRtcAudioPlayback		= 0;

//============================================================================
void MyAndroidPrint( int prio, const char *tag, const char *fmt, ... )
{
#ifndef _DEBUG
	if( prio <= ANDROID_LOG_INFO )
	{
		return;
	}
#endif // _DEBUG

	char as8Buf[ 1024 ];
	va_list argList;
	va_start( argList, fmt );
	vsnprintf( as8Buf, sizeof( as8Buf ), fmt, argList );
	as8Buf[sizeof( as8Buf ) - 1] = 0;
	va_end(argList);
	__android_log_write( prio, tag, as8Buf );
}

//============================================================================
JavaEnvCache& 		GetJavaEnvCache( void )
{
	return g_JavaEnvCache;
}

const char * JavaEnvCache::getJavaClassString( jclass refClass )
{
	if( 0 == refClass )
	{
		return "NULL jclass refClass";
	}
	else if( refClass == m_AssetGuiInfo )
	{
		return "AssetGuiInfo";
	}
	else if( refClass == m_GuidId )
	{
		return "VxGUID";
	}
	else if( refClass == m_FriendBase )
	{
		return "VxNetIdent";
	}
	else if( refClass == m_FileShareSettings )
	{
		return "FileShareSettings";
	}
	else if( refClass == m_NetSettings )
	{
		return "NetSettings";
	}
	else if( refClass == m_BuildInfo )
	{
		return "BuildInfo";
	}
	else if( refClass == m_WebRtcAudioManager )
	{
		return "WebRtcAudioManager";
	}
	else if( refClass == m_WebRtcAudioRecord )
	{
		return "WebRtcAudioRecord";
	}
	else if( refClass == m_WebRtcAudioPlayback )
	{
		return "WebRtcAudioTrack";
	}
	else
	{
		return "UNKOWN jclass refClass";
	}
}

//============================================================================
bool JavaClassCreate( JNIEnv *env, const char * pObjPath, jobject * retJobj )
{
	jclass cls = env->FindClass( pObjPath );
	if(!cls)
	{
		LogMsg( LOG_ERROR, "JavaClassCreate: failed to get %s class reference", pObjPath );
		return false;
	}
	
	jmethodID constr = env->GetMethodID(cls, "<init>", "()V");
	if( !constr )
	{
		LogMsg( LOG_ERROR, "JavaClassCreate: initClassHelper: failed to get %s constructor", pObjPath );
		return false;
	}
	
	jobject obj = env->NewObject(cls, constr);
	if( !obj )
	{
		LogMsg( LOG_ERROR, "JavaClassCreate: initClassHelper: failed to create a %s object", pObjPath );
		return false;
	}
	
	(*retJobj) = env->NewGlobalRef(obj);
	return true;
}

// Return a |jlong| that will correctly convert back to |ptr|.  This is needed
// because the alternative (of silently passing a 32-bit pointer to a vararg
// function expecting a 64-bit param) picks up garbage in the high 32 bits.
jlong PointerTojlong(void* ptr) 
{
	//vx_assert( sizeof(intptr_t) <= sizeof(jlong) );
	// Going through intptr_t to be obvious about the definedness of the
	// conversion from pointer to integral type.  intptr_t to jlong is a standard
	// widening by the static_assert above.
	jlong ret = reinterpret_cast<intptr_t>(ptr);
	//vx_assert(reinterpret_cast<void*>(ret) == ptr);
	return ret;
}

#endif // TARGET_OS_ANDROID
