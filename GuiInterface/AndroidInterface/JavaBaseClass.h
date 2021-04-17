#pragma once
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

#include <jni.h>
#include <string>

class JavaEnvCache;

class JavaBaseClass
{
public:
	JavaBaseClass( JavaEnvCache& envCache, jclass javaRefClass, bool passesNativeToConstructor = false,  bool skipGetConstructorMethod = false );
	virtual ~JavaBaseClass();

	JNIEnv *					getEnv( void )						{ return m_poEnv; }
	jclass& 					getJavaClass( void )				{ return m_JavaClass; }

	bool						checkForJavaException( const char * errProgrammerMsg = 0 );

	jobject						creatNewObj( void );
	jobject						creatNewObj( void * nativeThis );
	jobject						callCreateMethod(jclass objClass, jmethodID constructMethodID, ...);

	// if returns false then invalid object
	bool 						attachToThread( void );
	jboolean					callBooleanMethod( jobject object, jmethodID methodID, ... );
	jint						callIntMethod( jobject object, jmethodID methodID, ... );
	void						callVoidMethod( jobject object, jmethodID methodID, ... );
	jmethodID					getMethodId( const char* name, const char* signature );
	jmethodID					getStaticMethodId( const char* name, const char* signature);
	jobject						callStaticObjectMethod( jmethodID methodID, ... );
	std::string					javaToStdString( const jstring& j_string );


	//=== vars ===//
	JavaEnvCache&				m_EnvCache;
	JNIEnv *					m_poEnv;
	jclass 						m_JavaRefClass;
	bool						m_PassesNativeToConstructor;
	bool						m_SkipGetConstructorMethod;
	jclass						m_JavaClass;
	static unsigned int			m_ClassCnt;			
	unsigned int				m_iThisClassId;
	bool						m_bIsThreadAttached; // if true thread attached and required detachment

	jmethodID 					m_methConstructor;

};



#endif //TARGET_OS_ANDROID
