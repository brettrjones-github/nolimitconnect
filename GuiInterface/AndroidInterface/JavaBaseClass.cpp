/*
 *  Copyright (c) 2015 The WebRTC project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */

//============================================================================


#ifdef TARGET_OS_ANDROID

#include "JavaBaseClass.h"
#include "JavaHelpers.h"

unsigned int			JavaBaseClass::m_ClassCnt			= 0;

//============================================================================
JavaBaseClass::JavaBaseClass( JavaEnvCache& envCache, jclass javaRefClass, bool passesNativeToContructor, bool skipGetConstructorMethod  )
: m_EnvCache( envCache )
, m_poEnv( envCache.getJavaEnv() )
, m_JavaRefClass( javaRefClass )
, m_PassesNativeToConstructor( passesNativeToContructor )
, m_SkipGetConstructorMethod( skipGetConstructorMethod )
, m_JavaClass( 0 )
, m_iThisClassId( 0 )
, m_bIsThreadAttached( false )
{
	m_ClassCnt++;
	m_iThisClassId = m_ClassCnt;
}

//============================================================================
JavaBaseClass::~JavaBaseClass()
{	
}

//============================================================================
bool JavaBaseClass::attachToThread( void )
{
	if( m_bIsThreadAttached )
	{
		LogMsg( LOG_WARN, "JavaBaseClass::attachToThread: Already Attached\n" );
		return true;
	}

	//LogMsg( LOG_DEBUG, "JavaBaseClass Attempting Attach of thread %d\n", uiThreadId );
	if( m_EnvCache.getJavaVM()->AttachCurrentThread( &m_poEnv, NULL) < 0 )
	{
		LogMsg( LOG_ERROR, "JavaBaseClass: AttachCurrentThread failed\n" );
		return false;
	}
	else
	{
		m_bIsThreadAttached = true;
		//LogMsg( LOG_INFO, "JavaBaseClass: Attached thread %d to JNI env 0x%x\n", uiThreadId, env );
	}

	if( 0 == m_JavaClass )
	{
		m_JavaClass = (jclass) m_poEnv->NewGlobalRef( m_JavaRefClass );
		if( 0 == m_JavaClass )
		{
			LogMsg( LOG_ASSERT, "JavaBaseClass: Unable to reference class" );
			jthrowable exc;
			exc = m_poEnv->ExceptionOccurred();
			if( exc ) 
			{
				//jclass newExcCls;
				m_poEnv->ExceptionDescribe();
				m_poEnv->ExceptionClear();
			}

			return false;
		}
		// The local reference is no longer useful
		//(*env)->DeleteLocalRef(env, friendClass);
	}

	// get java method id s
	if( m_SkipGetConstructorMethod )
	{
		m_methConstructor = 0;
	}
	else
	{
		//LogMsg( LOG_INFO, "JavaBaseClass: Getting Constructor\n" );
		if( m_PassesNativeToConstructor )
		{
			m_methConstructor		=  m_poEnv->GetMethodID( m_JavaClass, "<init>", "(Landroid/content/Context;J)V");
		}
		else
		{
			m_methConstructor		=  m_poEnv->GetMethodID( m_JavaClass, "<init>", "()V");
		}

		//LogMsg( LOG_INFO, "JavaBaseClass: Constructor id 0x%x\n", poClass->m_methConstructor );
		if( 0 == m_methConstructor )
		{
			LogMsg( LOG_ERROR, "JavaBaseClass: Unable to get constructor method of %s\n", m_EnvCache.getJavaClassString( m_JavaRefClass ) );
			return false;
		}
	}

	return true;
}

//============================================================================
jobject JavaBaseClass::creatNewObj( void )
{
	return m_poEnv->NewObject( m_JavaClass, m_methConstructor );
}

//============================================================================
jobject JavaBaseClass::creatNewObj( void * nativeThis )
{
	return callCreateMethod( m_JavaClass, m_methConstructor, m_EnvCache.getJavaContext(), PointerTojlong(nativeThis) );
}

//============================================================================
jobject JavaBaseClass::callCreateMethod(jclass objClass, jmethodID constructMethodID, ...) 
{
	va_list args;
	va_start( args, constructMethodID );
	jobject obj = m_poEnv->NewObjectV( objClass, constructMethodID, args );
	checkForJavaException( "Error during NewObjectV" );
	//CHECK_EXCEPTION(jni_) << "Error during NewObjectV";
	va_end(args);
	if( 0 != obj )
	{
		// need global reference for objects expected to be around longer than single call or get 
		// JNI ERROR (app bug): accessed stale local reference
		return m_poEnv->NewGlobalRef( obj );
	}

	return obj;
}

//============================================================================
bool JavaBaseClass::checkForJavaException( const char * errProgrammerMsg )
{
	if( 0 != m_poEnv )
	{
		if( m_poEnv->ExceptionCheck() )
		{
			m_poEnv->ExceptionDescribe();
			m_poEnv->ExceptionClear();
			if( errProgrammerMsg )
			{
				LogMsg( LOG_ERROR, "JavaBaseClass::checkForJavaException %s\n", errProgrammerMsg );
			}
			else
			{
				LogMsg( LOG_ERROR, "JavaBaseClass::checkForJavaException\n" );
			}

			return true;
		}

		return false;
	}
	else
	{
		LogMsg( LOG_ASSERT, "JavaBaseClass::checkForJavaException NULL m_poEnv\n" );
		return true;
	}
}


//============================================================================
jboolean JavaBaseClass::callBooleanMethod(jobject object, jmethodID methodID, ...) 
{
	va_list args;
	va_start(args, methodID);
	jboolean res = m_poEnv->CallBooleanMethodV(object, methodID, args);
	//CHECK_EXCEPTION(jni_) << "Error during CallBooleanMethod";
	va_end(args);
	return res;
}

//============================================================================
jint JavaBaseClass::callIntMethod(jobject object, jmethodID methodID, ...) 
{
	va_list args;
	va_start(args, methodID);
	jint res = m_poEnv->CallIntMethodV(object, methodID, args);
	//CHECK_EXCEPTION(jni_) << "Error during CallIntMethod";
	va_end(args);
	return res;
}

//============================================================================
void JavaBaseClass::callVoidMethod(jobject object, jmethodID methodID, ...) 
{
	va_list args;
	va_start(args, methodID);
	m_poEnv->CallVoidMethodV(object, methodID, args);
	//CHECK_EXCEPTION(jni_) << "Error during CallVoidMethod";
	va_end(args);
}

//============================================================================
jmethodID JavaBaseClass::getMethodId( const char* name, const char* signature ) 
{
	return m_poEnv->GetMethodID( m_JavaClass, name, signature);
}

//============================================================================
jmethodID JavaBaseClass::getStaticMethodId( const char* name, const char* signature) 
{
	return m_poEnv->GetStaticMethodID( m_JavaClass, name, signature );
}

//============================================================================
jobject JavaBaseClass::callStaticObjectMethod( jmethodID methodID, ... ) 
{
	va_list args;
	va_start(args, methodID);
	jobject res = m_poEnv->CallStaticObjectMethod(m_JavaClass, methodID, args);
	//CHECK_EXCEPTION(m_poEnv) << "Error during CallStaticObjectMethod";
	return res;
}

//============================================================================
// Given a (UTF-16) jstring return a new UTF-8 native string.
std::string JavaBaseClass::javaToStdString( const jstring& j_string )
{
	const char* chars = m_poEnv->GetStringUTFChars( j_string, 0 );
	//CHECK_EXCEPTION(jni) << "Error during GetStringUTFChars";
	std::string str(chars, m_poEnv->GetStringUTFLength(j_string));
	//CHECK_EXCEPTION(jni) << "Error during GetStringUTFLength";
	m_poEnv->ReleaseStringUTFChars(j_string, chars);
	//CHECK_EXCEPTION(jni) << "Error during ReleaseStringUTFChars";
	return str;
}

#endif //TARGET_OS_ANDROID

