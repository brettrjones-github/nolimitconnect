#ifndef VXJAVA_H
#define VXJAVA_H
//============================================================================
// Copyright (C) 2018 Brett R. Jones
//
// You may use, copy, modify, merge, publish, distribute, sub-license, and/or sell this software
// provided this Copyright is not modified or removed and is included all copies or substantial portions of the Software
//
// This code is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
//
// bjones.engineer@gmail.com
// http://www.nolimitconnect.org
//============================================================================
#ifdef TARGET_OS_ANDROID

# include <CoreLib/VxMutex.h>
# include <jni.h>

class VxJava
{
public:
    VxJava() = default;

    static JavaVM *             getJavaVM( void )                       { return m_JavaVM; }
    static JNIEnv *             getJavaEnv( void );


    static JavaVM *             m_JavaVM;
};


VxJava& GetJavaEnvCache();

#endif // TARGET_OS_ANDROID

#endif // VXJAVA_H
