#pragma once
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

// android specific code
#ifdef TARGET_OS_ANDROID
//Field Descriptor Java Language Type
//Z	boolean
//B	byte
//C	char
//S	short
//I	int
//J	long
//F	float
//D	double
//Java type	C/C++ typename	Description
//byte		jbyte		8-bit signed integer
//short		jshort		16-bit signed integer
//int		jint		32-bit signed integer
//long		jlong		64-bit signed integer
//float		jfloat		32-bit IEEE floating-point number
//double	jdouble		64-bit IEEE floating-point number
//char		jchar		16-bit Unicode character
//boolean	jboolean	logical (Boolean) values
//void		void		no value



#include <jni.h>
#include <android/log.h>
//#include <JNIHelp.h>
//#include <android_runtime/AndroidRuntime.h>
#include <sys/time.h>
#include <time.h>
#include <stdint.h>
#include <unistd.h>
#include <libpthread/pthread.h>
#include <GoTvCore/GoTvP2P/P2PEngine/P2PEngine.h>
#include <GuiInterface/IToGui.h>

#include "JavaHelpers.h"

#include "NativeToJava.h"
#include "NativeFromJava.h"

#include <CoreLib/VxMutex.h>

extern VxMutex	g_oJavaMutex;

#endif // TARGET_OS_ANDROID
