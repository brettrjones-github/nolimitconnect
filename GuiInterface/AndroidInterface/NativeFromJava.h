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

extern "C"
{
void Java_com_waycoolapps_myp2pweb_NativeTxTo_fromGuiSetIsPaid( JNIEnv*  env, jobject  thiz, jboolean isPaid );
jint Java_com_waycoolapps_myp2pweb_NativeTxTo_fromGuiGetAppVersion( JNIEnv*  env, jobject  thiz );
//=== user gui input actions ===//
//! called with orientation events
void Java_com_waycoolapps_myp2pweb_NativeTxTo_fromGuiOrientationEvent( JNIEnv*  env, jobject  thiz, jfloat f32PosX, jfloat f32PosY, jfloat f32PosZ );
//! called with touch events
void Java_com_waycoolapps_myp2pweb_NativeTxTo_fromGuiTouchEvent(JNIEnv*  env, jobject  thiz, jint iEventType, jint f32PosX, jint f32PosY);

//=== gui window actions ===//
void Java_com_waycoolapps_myp2pweb_NativeTxTo_fromGuiNativeGlInit( JNIEnv*  env, jobject  thiz );
void Java_com_waycoolapps_myp2pweb_NativeTxTo_fromGuiNativeGlResize( JNIEnv*  env, jobject  thiz, jint w, jint h );
jint Java_com_waycoolapps_myp2pweb_NativeTxTo_fromGuiNativeGlRender( JNIEnv*  env, jobject  thiz );
void Java_com_waycoolapps_myp2pweb_NativeTxTo_fromGuiNativeGlPauseRender( JNIEnv*  env, jobject  thiz );
void Java_com_waycoolapps_myp2pweb_NativeTxTo_fromGuiNativeGlResumeRender( JNIEnv*  env, jobject  thiz );
void Java_com_waycoolapps_myp2pweb_NativeTxTo_fromGuiNativeGlDestroy( JNIEnv*  env, jobject  thiz );

void Java_com_waycoolapps_myp2pweb_NativeTxTo_fromGuiAppStartup( JNIEnv* env,
																  jobject thiz, 
																  jstring strAssetsDir,
																  jstring strRootDataDir );
void Java_com_waycoolapps_myp2pweb_NativeTxTo_fromGuiAppShutdown( JNIEnv*  env, jobject  thiz );
void Java_com_waycoolapps_myp2pweb_NativeTxTo_fromGuiAppSleep( JNIEnv* env, jobject thiz );
void Java_com_waycoolapps_myp2pweb_NativeTxTo_fromGuiAppWake( JNIEnv*  env, jobject  thiz );
void Java_com_waycoolapps_myp2pweb_NativeTxTo_fromGuiAppPause( JNIEnv*  env, jobject  thiz );
void Java_com_waycoolapps_myp2pweb_NativeTxTo_fromGuiAppResume( JNIEnv*  env, jobject  thiz );
void Java_com_waycoolapps_myp2pweb_NativeTxTo_fromGuiAppIdle( JNIEnv*  env, jobject  thiz );
void Java_com_waycoolapps_myp2pweb_NativeTxTo_fromGuiAppStartActivity( JNIEnv*  env, jobject  thiz, jint iActivity );

//void Java_com_waycoolapps_myp2pweb_NativeTxTo_fromGuiMicrophoneData( JNIEnv*  env, jobject  thiz, jbyteArray pu16PcmData, jint u16PcmSampleCnt );

//void Java_com_waycoolapps_myp2pweb_NativeTxTo_fromGuiVideoData( JNIEnv*  env, jobject  thiz, jint jiVideoFormat, jobject vidData, jint iWidth, jint iHeight, jint jiRotation );


}// extern C

// android specific code
#endif // TARGET_OS_ANDROID
