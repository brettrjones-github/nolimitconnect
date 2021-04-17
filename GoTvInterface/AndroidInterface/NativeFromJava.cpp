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

#include "NativeCommon.h"
#include "EngineImp.h"

#include "JavaHelpers.h"

#include "FriendJava.h"
#include "NetSettingsJava.h"
#include "FileShareSettingsJava.h"
#include "AssetInfoJava.h"
#include "GuidIdJava.h"

#include <GoTvCore/GoTvP2P/P2PEngine/FileShareSettings.h>
#include <P2PEngineLib/MediaProcessor/MediaProcessor.h>
#include <GoTvCore/GoTvP2P/AssetMgr/AssetInfo.h>

#include <NetLib/VxGetRandomPort.h>
#include <VxVideoLib/VxVideoLib.h>
#include <CoreLib/VxGlobals.h>
#include <CoreLib/VxThread.h>

#define FROM_NATIVE_LOG_TAG "NativeFromJava:"

namespace
{
	int							g_JniLoadCalledCnt	= 0;
	VxMutex						g_oJavaMutex;
}

//int importGLInit();
//void importGLDeinit();

//============================================================================
void ThreadStartCallbackFunction( unsigned int uiThreadId, const char * threadName )
{
	//MyAndroidPrint( ANDROID_LOG_INFO, FROM_NATIVE_LOG_TAG, "ThreadStartCallbackFunction Thread %d %s\n", uiThreadId, threadName );
}

//============================================================================
void ThreadExitCallbackFunction( unsigned int uiThreadId, bool isExitCallbackLocked, const char * threadName )
{
	if( false == isExitCallbackLocked )
	{
		g_FromJavaAccessMutex.lock();
	}

	//LogMsg( LOG_ERROR, "ThreadExitCallbackFunction Thread %d %s detaching\n", uiThreadId, threadName );
	//NativeToJavaClassThreadExit( uiThreadId, threadName );
	//FriendJavaClassThreadExit( uiThreadId );
	//JavaAssetInfoClassThreadExit( uiThreadId );
	//JavaGuidIdClassThreadExit( uiThreadId );
	//FileShareSettingsJavaClassThreadExit( uiThreadId );
	//NetSettingsJavaClassThreadExit( uiThreadId );

    GetJavaEnvCache().getJavaVM()->DetachCurrentThread();
	if ( false == isExitCallbackLocked )
	{
		g_FromJavaAccessMutex.unlock();
	}
}

extern "C"
{

//============================================================================
jclass GetJavaClassReference( JNIEnv * env, const char * classPath )
{
    jclass javaRefClass = env->FindClass( classPath );
    if( 0 == javaRefClass )
    {
        //PDEBUG_OUT ( "Can't find java class..com/waycoolapps/myp2pweb/NativeRxFrom\n" );
        jthrowable exc;
        exc = env->ExceptionOccurred();
        if( exc )
        {
            //jclass newExcCls;
            env->ExceptionDescribe();
            env->ExceptionClear();
        }

        return 0;
    }

    return (jclass)(env)->NewGlobalRef( javaRefClass );
}

//============================================================================
jint JNI_OnLoad( JavaVM * vm, void * reserved )
{
	__android_log_write( ANDROID_LOG_INFO, FROM_NATIVE_LOG_TAG, "JNI_OnLoad start" );
	g_JniLoadCalledCnt++;
	JavaEnvCache::m_JavaVM = vm;
	JNIEnv * env;
	if( vm->GetEnv( ( void** ) &env, JNI_VERSION_1_6 ) )
	{
		__android_log_write( ANDROID_LOG_INFO, FROM_NATIVE_LOG_TAG, "JNI_OnLoad FAIL Get Env" );
		return -1;
	}

    JavaEnvCache::m_JavaEnv = env;
	if( 0 == env )
	{
		__android_log_write( ANDROID_LOG_INFO, FROM_NATIVE_LOG_TAG, "JNI_OnLoad NULL Get Env" );
	}

    JavaEnvCache::m_NativeRxFrom = GetJavaClassReference( env, "com/waycoolapps/myp2pweb/NativeRxFrom" );
    if( 0 == JavaEnvCache::m_NativeRxFrom )
    {
		__android_log_write( ANDROID_LOG_INFO, FROM_NATIVE_LOG_TAG, "Can't find java class..com/waycoolapps/myp2pweb/NativeRxFrom" );
       //PDEBUG_OUT ( "Can't find java class..com/waycoolapps/myp2pweb/NativeRxFrom\n" );
        return -2;
    }

    JavaEnvCache::m_AssetGuiInfo            = GetJavaClassReference( env, "com/waycoolapps/myp2pweb/AssetGuiInfo" );
    JavaEnvCache::m_GuidId                  = GetJavaClassReference( env, "com/waycoolapps/myp2pweb/VxGUID" );
    JavaEnvCache::m_FriendBase              = GetJavaClassReference( env, "com/waycoolapps/myp2pweb/VxNetIdent" );
    JavaEnvCache::m_FileShareSettings       = GetJavaClassReference( env, "com/waycoolapps/myp2pweb/FileShareSettings" );
    JavaEnvCache::m_NetSettings             = GetJavaClassReference( env, "com/waycoolapps/myp2pweb/NetSettings" );
	//webrtc audio
	JavaEnvCache::m_BuildInfo				= GetJavaClassReference( env, "com/waycoolapps/myp2pweb/BuildInfo" );
	JavaEnvCache::m_WebRtcAudioManager      = GetJavaClassReference( env, "com/waycoolapps/myp2pweb/WebRtcAudioManager" );
	JavaEnvCache::m_WebRtcAudioRecord       = GetJavaClassReference( env, "com/waycoolapps/myp2pweb/WebRtcAudioRecord" );
	JavaEnvCache::m_WebRtcAudioPlayback     = GetJavaClassReference( env, "com/waycoolapps/myp2pweb/WebRtcAudioTrack" );

	//__android_log_write( ANDROID_LOG_INFO, FROM_NATIVE_LOG_TAG, "GetNativeToJavaClass g_FromJavaAccessMutex.lock" );
	g_FromJavaAccessMutex.lock();
	// set up thread exit callback so we can attach thread to java
	VxThread::setThreadStartCallback( ThreadStartCallbackFunction );

	// set up thread exit callback so we can detach thread from java
	VxThread::setThreadExitCallback( ThreadExitCallbackFunction );

	GetNativeToJavaClass(); // force register for this thread
	//LogMsg( LOG_WARN, "JJ JNI_OnLoad called JAVA VM 0x%x Load Count %d\n", g_poJavaVM, g_JniLoadCalledCnt );
	
	g_FromJavaAccessMutex.unlock();
	//__android_log_write( ANDROID_LOG_INFO, FROM_NATIVE_LOG_TAG, "GetNativeToJavaClass g_FromJavaAccessMutex.unlock" );
	__android_log_write( ANDROID_LOG_INFO, FROM_NATIVE_LOG_TAG, "JNI_OnLoad done" );
	return JNI_VERSION_1_4;
}

//============================================================================
void JNI_OnUnload(JavaVM *vm, void *reserved)
{
	VxSetAppIsShuttingDown( true );
	GetEngineImp().getEngine().getPeerMgr().stopListening();
	__android_log_write( ANDROID_LOG_INFO, FROM_NATIVE_LOG_TAG, "JNI_OnUnload" );
}

//============================================================================
void Java_com_waycoolapps_myp2pweb_NativeTxTo_fromGuiSetupContext( JNIEnv* env, jobject thiz, jobject appContext )
{
	__android_log_write( ANDROID_LOG_INFO, FROM_NATIVE_LOG_TAG, "NativeTxTo_fromGuiSetupContext start" );
	JavaEnvCache::m_JavaContext = appContext;
	GetEngineImp().fromGuiSetupContext();
	__android_log_write( ANDROID_LOG_INFO, FROM_NATIVE_LOG_TAG, "NativeTxTo_fromGuiSetupContext done" );
}

//============================================================================
void Java_com_waycoolapps_myp2pweb_NativeTxTo_fromGuiKickWatchdog( JNIEnv* env, jobject thiz )
{
	//__android_log_write( ANDROID_LOG_INFO, FROM_NATIVE_LOG_TAG, "NativeTxTo_fromGuiKickWatchdog" );
	GetEngineImp().fromGuiKickWatchdog();
}

//============================================================================
void Java_com_waycoolapps_myp2pweb_NativeTxTo_fromGuiCacheAudioParameters(	JNIEnv* env,
																			jobject thiz,
																			jint sample_rate,
																			jint channels,
																			jboolean hardware_aec,
																			jboolean hardware_agc,
																			jboolean hardware_ns,
																			jboolean low_latency_output,
																			jint output_buffer_size,
																			jint input_buffer_size,
																			jlong native_audio_manager) 
{
	__android_log_write( ANDROID_LOG_INFO, FROM_NATIVE_LOG_TAG, "NativeTxTo_fromGuiCacheAudioParameters" );
	webrtc::AudioManager* this_object = reinterpret_cast<webrtc::AudioManager*>(native_audio_manager);
	this_object->fromGuiOnCacheAudioParameters(
		env, sample_rate, channels, hardware_aec, hardware_agc, hardware_ns,
		low_latency_output, output_buffer_size, input_buffer_size);
}

//============================================================================
void Java_com_waycoolapps_myp2pweb_NativeTxTo_fromGuiCacheRecordDirectBufferAddress( JNIEnv* env, jobject thiz, jobject byte_buffer, 
																			  jlong nativeAudioRecord ) 
{
	__android_log_write( ANDROID_LOG_INFO, FROM_NATIVE_LOG_TAG, "NativeTxTo_fromGuiCacheRecordDirectBufferAddress" );
	webrtc::AudioRecordJni * this_object = reinterpret_cast<webrtc::AudioRecordJni*> (nativeAudioRecord);
	this_object->fromGuiOnCacheRecordDirectBufferAddress(env, byte_buffer);
}

//============================================================================
void Java_com_waycoolapps_myp2pweb_NativeTxTo_fromGuiCachePlayoutDirectBufferAddress( JNIEnv* env, jobject thiz, jobject byte_buffer, 
																					jlong nativeAudioRecord ) 
{
	__android_log_write( ANDROID_LOG_INFO, FROM_NATIVE_LOG_TAG, "NativeTxTo_fromGuiCachePlayoutDirectBufferAddress" );
	webrtc::AudioTrackJni * this_object = reinterpret_cast<webrtc::AudioTrackJni*> (nativeAudioRecord);
	this_object->fromGuiOnCachePlayoutDirectBufferAddress(env, byte_buffer);
}

//============================================================================
void Java_com_waycoolapps_myp2pweb_NativeTxTo_fromGuiGetPlayoutData( JNIEnv* env, jobject thiz, jint length, jlong nativeAudioTrack ) 
{
	//__android_log_write( ANDROID_LOG_INFO, FROM_NATIVE_LOG_TAG, "NativeTxTo_fromGuiGetPlayoutData" );
	webrtc::AudioTrackJni * this_object = reinterpret_cast<webrtc::AudioTrackJni*> (nativeAudioTrack);
	this_object->fromGuiOnGetPlayoutData(static_cast<size_t>(length));
}

//============================================================================
void Java_com_waycoolapps_myp2pweb_NativeTxTo_fromGuiDataIsRecorded( JNIEnv* env, jobject thiz, jint length, jlong nativeAudioRecord ) 
{
	//__android_log_write( ANDROID_LOG_INFO, FROM_NATIVE_LOG_TAG, "NativeTxTo_fromGuiDataIsRecorded" );
	webrtc::AudioRecordJni * this_object = reinterpret_cast<webrtc::AudioRecordJni*> (nativeAudioRecord);
	this_object->fromGuiOnDataIsRecorded(length);
}

//============================================================================
void Java_com_waycoolapps_myp2pweb_NativeTxTo_fromGuiSetIsAppCommercial( JNIEnv*  env, jobject  thiz, jboolean isCommercial )
{
	return VxSetIsApplicationCommercial( isCommercial );
}

//============================================================================
jboolean Java_com_waycoolapps_myp2pweb_NativeTxTo_fromGuiGetIsAppCommercial( JNIEnv*  env, jobject  thiz )
{
	return VxGetIsApplicationCommercial();
}

//============================================================================
jint Java_com_waycoolapps_myp2pweb_NativeTxTo_fromGuiGetAppVersionBinary( JNIEnv*  env, jobject  thiz )
{
	return VxGetAppVersion();
}

//============================================================================
jstring Java_com_waycoolapps_myp2pweb_NativeTxTo_fromGuiGetAppVersionString( JNIEnv*  env, jobject  thiz )
{
	std::string strAppVersion = VxGetAppVersionString();
	jstring javaAppVersion;
	javaAppVersion = env->NewStringUTF( strAppVersion.c_str() ); 
	return javaAppVersion;
}

//============================================================================
jstring Java_com_waycoolapps_myp2pweb_NativeTxTo_fromGuiGetAppName(	JNIEnv* env,
																	jobject thiz )
{
	std::string strAppName = VxGetApplicationName();
	jstring javaAppName;
	javaAppName = env->NewStringUTF( strAppName.c_str() ); 
	return javaAppName;
}

//============================================================================
jstring Java_com_waycoolapps_myp2pweb_NativeTxTo_fromGuiGetAppNameNoSpaces(	JNIEnv* env,
																			jobject thiz )
{
	std::string strAppNameNoSpaces = VxGetApplicationNameNoSpaces();
	jstring javaAppName;
	javaAppName = env->NewStringUTF( strAppNameNoSpaces.c_str() ); 
	return javaAppName;
}

//============================================================================
void Java_com_waycoolapps_myp2pweb_NativeTxTo_fromGuiAppStartup(	JNIEnv* env,
																	jobject thiz, 
																	jstring jstrAssetsDir,
																	jstring jstrRootDataDir )
{
	__android_log_write( ANDROID_LOG_INFO, FROM_NATIVE_LOG_TAG, "NativeTxTo_fromGuiAppStartup start" );
	//importGLInit();
	g_FromJavaAccessMutex.lock();

	//Get the characters from the jstring
	const char * assetsDir			= env->GetStringUTFChars( jstrAssetsDir, 0 );
	const char * rootDataDir		= env->GetStringUTFChars( jstrRootDataDir, 0 );
	std::string strAssetsDir		= assetsDir;
	std::string strRootDataDir		= rootDataDir;
	env->ReleaseStringUTFChars( jstrAssetsDir, assetsDir );
	env->ReleaseStringUTFChars( jstrRootDataDir, rootDataDir );
	
	// we need to release lock.. there will be other calls before AppStartup returns
	g_FromJavaAccessMutex.unlock();

	GetEngineImp().fromGuiAppStartup( strAssetsDir.c_str(), strRootDataDir.c_str() );
	__android_log_write( ANDROID_LOG_INFO, FROM_NATIVE_LOG_TAG, "NativeTxTo_fromGuiAppStartup done" );
}

//============================================================================
void Java_com_waycoolapps_myp2pweb_NativeTxTo_fromGuiSetUserXferDir(	JNIEnv* env,
																		jobject thiz,
																		jstring strUserXferDir )
{
	g_FromJavaAccessMutex.lock();
	const char * userDir = env->GetStringUTFChars( strUserXferDir, 0 );
	MyAndroidPrint( ANDROID_LOG_INFO, FROM_NATIVE_LOG_TAG, "NativeTxTo_fromGuiSetUserXferDir %s\n", userDir );
	GetEngineImp().fromGuiSetUserXferDir( userDir );
	env->ReleaseStringUTFChars( strUserXferDir, userDir );
	g_FromJavaAccessMutex.unlock();
	__android_log_write( ANDROID_LOG_INFO, FROM_NATIVE_LOG_TAG, "NativeTxTo_fromGuiSetUserXferDir done" );
}

//============================================================================
jlong Java_com_waycoolapps_myp2pweb_NativeTxTo_fromGuiGetDiskFreeSpace(	JNIEnv* env, jobject thiz )
{
	return (jlong)GetEngineImp().fromGuiGetDiskFreeSpace();
}

//============================================================================
void Java_com_waycoolapps_myp2pweb_NativeTxTo_fromGuiSetUserSpecificDir(	JNIEnv* env,
																			jobject thiz, 
																			jstring strUserSpecificDir )
{
	__android_log_write( ANDROID_LOG_INFO, FROM_NATIVE_LOG_TAG, "NativeTxTo_fromGuiSetUserSpecificDir start" );
	g_FromJavaAccessMutex.lock();
	const char * userDir = env->GetStringUTFChars( strUserSpecificDir, 0 );
	GetEngineImp().fromGuiSetUserSpecificDir( userDir );
	env->ReleaseStringUTFChars( strUserSpecificDir, userDir );
	g_FromJavaAccessMutex.unlock();
	__android_log_write( ANDROID_LOG_INFO, FROM_NATIVE_LOG_TAG, "NativeTxTo_fromGuiSetUserSpecificDir done" );
}

//============================================================================
void Java_com_waycoolapps_myp2pweb_NativeTxTo_fromGuiAppShutdown( JNIEnv*  env, jobject  thiz )
{
	//g_FromJavaAccessMutex.lock();
	__android_log_write( ANDROID_LOG_INFO, FROM_NATIVE_LOG_TAG, "NativeTxTo_fromGuiAppShutdown\n" );
	GetEngineImp().fromGuiAppShutdown();
	//importGLDeinit();
	//g_FromJavaAccessMutex.unlock();
}

//============================================================================
void Java_com_waycoolapps_myp2pweb_NativeTxTo_fromGuiAppIdle( JNIEnv*  env, jobject  thiz )
{
	//__android_log_write( ANDROID_LOG_INFO, FROM_NATIVE_LOG_TAG, "NativeTxTo_fromGuiAppIdle\n" );
	//g_FromJavaAccessMutex.lock();
	GetEngineImp().fromGuiAppIdle();
	//g_FromJavaAccessMutex.unlock();
}

//============================================================================
//! called when application is put to sleep
void Java_com_waycoolapps_myp2pweb_NativeTxTo_fromGuiAppPauseOrResume( JNIEnv* env, jobject thiz, jboolean jAppIsPaused )
{
	bool isPaused = (bool)jAppIsPaused;
	__android_log_write( ANDROID_LOG_INFO, FROM_NATIVE_LOG_TAG, "NativeTxTo_fromGuiAppPauseOrResume start" );
	g_FromJavaAccessMutex.lock();
	GetEngineImp().fromGuiAppPauseOrResume( isPaused );
	g_FromJavaAccessMutex.unlock();
	__android_log_write( ANDROID_LOG_INFO, FROM_NATIVE_LOG_TAG, "NativeTxTo_fromGuiAppPauseOrResume done" );
}

//============================================================================
//! called with orientation events
void Java_com_waycoolapps_myp2pweb_NativeTxTo_fromGuiOrientationEvent( JNIEnv*  env, jobject  thiz,  jfloat f32PosX, jfloat f32PosY, jfloat f32PosZ  )
{
	g_FromJavaAccessMutex.lock();
	MyAndroidPrint( ANDROID_LOG_INFO, FROM_NATIVE_LOG_TAG, "OrientationEvent: %3.2f %3.2f, %3.2f", f32PosX, f32PosY, f32PosZ );
	GetEngineImp().fromGuiOrientationEvent( f32PosX, f32PosY, f32PosZ  );
	g_FromJavaAccessMutex.unlock();
}

//============================================================================
//! called with touch events
void Java_com_waycoolapps_myp2pweb_NativeTxTo_fromGuiTouchEvent(JNIEnv*  env, jobject  thiz, jint iEventType, jint f32PosX, jint f32PosY)
{
	g_FromJavaAccessMutex.lock();
	MyAndroidPrint( ANDROID_LOG_INFO, FROM_NATIVE_LOG_TAG, "Touch Event %d X=%d, Y=%d\n", iEventType, f32PosX, f32PosY );

	// fake like it was mouse input
	switch( iEventType )
	{
	case E_TOUCH_ACTION_DOWN:
		GetEngineImp().fromGuiMouseEvent( eMouseButtonLeft, eMouseEventDown, f32PosX, f32PosY  );
		break;
	case E_TOUCH_ACTION_UP:
		GetEngineImp().fromGuiMouseEvent( eMouseButtonLeft, eMouseEventUp, f32PosX, f32PosY  );
		break;
	case E_TOUCH_ACTION_MOVE:
		GetEngineImp().fromGuiMouseEvent( eMouseButtonNone, eMouseEventMove, f32PosX, f32PosY  );
		break;
	}

	//MyAndroidPrint( ANDROID_LOG_INFO, FROM_NATIVE_LOG_TAG, "Touch Event done\n" );
	g_FromJavaAccessMutex.unlock();
}

//============================================================================
void Java_com_waycoolapps_myp2pweb_NativeTxTo_fromGuiKeyEvent( JNIEnv*  env, jobject  thiz, jint eKeyEventType, jint eKey  )
{
	g_FromJavaAccessMutex.lock();
	MyAndroidPrint( ANDROID_LOG_INFO, FROM_NATIVE_LOG_TAG, "NativeTxTo_fromGuiKeyEvent: event %d key %d", eKeyEventType, eKey );
	GetEngineImp().fromGuiKeyEvent( (EKeyEventType)eKeyEventType, (EKeyCode)eKey  );
	g_FromJavaAccessMutex.unlock();
}

//============================================================================
//! called when microphone audio is available
//! return false to stop recording
void Java_com_waycoolapps_myp2pweb_NativeTxTo_fromGuiMicrophoneData( JNIEnv*  env, jobject  thiz, jobject jAudioDataBuf, jint jintPcmDataLen )
{
	// BNOTE: this is no longer used.. instead pcm data is sent directly to audio processing via fromGuiDataIsRecorded
	// get array of bytes
	//jbyte* audioData = env->GetByteArrayElements(pu16PcmData, NULL);
	//// process the audio
	//GetEngineImp().fromGuiMicrophoneData( (int16_t *)audioData, (int)u16PcmDataLen );
	//env->ReleaseByteArrayElements(pu16PcmData, audioData, JNI_ABORT);

	int16_t * audioData = (int16_t *)env->GetDirectBufferAddress( jAudioDataBuf );
	if( 0 != audioData )
	{
		GetEngineImp().fromGuiMicrophoneData( audioData, (int)jintPcmDataLen );
	}
	else
	{
		MyAndroidPrint( ANDROID_LOG_INFO, FROM_NATIVE_LOG_TAG, "ERROR NativeTxTo_fromGuiMicrophoneData null buffer\n" );
	}
}

//============================================================================
void Java_com_waycoolapps_myp2pweb_NativeTxTo_fromGuiMuteMicrophone( JNIEnv* env, jobject thiz, jboolean bMute )
{
	MyAndroidPrint( ANDROID_LOG_INFO, FROM_NATIVE_LOG_TAG, "NativeTxTo_fromGuiMuteMicrophone\n" );
	GetEngineImp().fromGuiMuteMicrophone( bMute );
}

//============================================================================
void Java_com_waycoolapps_myp2pweb_NativeTxTo_fromGuiMuteSpeaker( JNIEnv* env, jobject thiz, jboolean bMute )
{
	MyAndroidPrint( ANDROID_LOG_INFO, FROM_NATIVE_LOG_TAG, "NativeTxTo_fromGuiMuteSpeaker\n" );
	GetEngineImp().fromGuiMuteSpeaker( bMute );
}

//============================================================================
void Java_com_waycoolapps_myp2pweb_NativeTxTo_fromGuiEchoCancelEnable( JNIEnv* env, jobject thiz, jboolean enableEchoCancel )
{
	MyAndroidPrint( ANDROID_LOG_INFO, FROM_NATIVE_LOG_TAG, "NativeTxTo_fromGuiEchoCancelEnable\n" );
	GetEngineImp().fromGuiEchoCancelEnable( enableEchoCancel );
}

//============================================================================
jboolean Java_com_waycoolapps_myp2pweb_NativeTxTo_fromGuiIsMicrophoneMuted( JNIEnv* env, jobject thiz )
{
	MyAndroidPrint( ANDROID_LOG_INFO, FROM_NATIVE_LOG_TAG, "NativeTxTo_fromGuiIsMicrophoneMuted\n" );
	return GetEngineImp().fromGuiIsMicrophoneMuted();
}

//============================================================================
jboolean Java_com_waycoolapps_myp2pweb_NativeTxTo_fromGuiIsSpeakerMuted( JNIEnv* env, jobject thiz )
{
	MyAndroidPrint( ANDROID_LOG_INFO, FROM_NATIVE_LOG_TAG, "NativeTxTo_fromGuiIsSpeakerMuted\n" );
	return GetEngineImp().fromGuiIsSpeakerMuted();
}

//============================================================================
jboolean Java_com_waycoolapps_myp2pweb_NativeTxTo_fromGuiIsEchoCancelEnabled( JNIEnv * env, jobject thiz )
{
	MyAndroidPrint( ANDROID_LOG_INFO, FROM_NATIVE_LOG_TAG, "NativeTxTo_fromGuiIsEchoCancelEnabled\n" );
	return GetEngineImp().fromGuiIsEchoCancelEnabled();
}

//============================================================================
void Java_com_waycoolapps_myp2pweb_NativeTxTo_fromGuiWantMediaInput( JNIEnv * env, jobject thiz, jlong u64IdHiPart, jlong u64IdLoPart, jint iMediaType, jboolean bWantInput )
{
	VxGUID oOnlineId(u64IdHiPart, u64IdLoPart);
	GetEngineImp().fromGuiWantMediaInput(oOnlineId, (EMediaInputType)iMediaType, (bool)bWantInput );
}

#define DROID_NV16  	0x00000010 		//16 (YCbCr_422_SP) Since: API Level 8 YCbCr format, used for video. Whether this format is supported by the camera hardware can be determined by getSupportedPreviewFormats().
#define DROID_NV21  	0x00000011 		//17 (YCbCr_420_SP) Since: API Level 8 YCrCb format used for images, which uses the NV21 encoding format. This is the default format for camera preview images, when not otherwise set with setPreviewFormat(int).
#define DROID_RGB_565   0x00000004   	//4 Since: API Level 8 RGB format used for pictures encoded as RGB_565 see setPictureFormat(int).
#define DROID_YUY2 		0x00000014		//20 (YCbCr_422_I) Since: API Level 8 YCbCr format used for images, which uses YUYV (YUY2) encoding format. This is an alternative format for camera preview images. Whether this format is supported by the camera hardware can be determined by getSupportedPreviewFormats().
#define DROID_YV12 		0x32315659		//842094169 Since: API Level 9 Planar 4:2:0 YCrCb format. This format assumes an horizontal stride of 16 pixels for all planes and an implicit vertical stride of the image height's next multiple of two. y_size = stride * ALIGN(height, 2) c_size = ALIGN(stride/2, 16) * height size = y_size + c_size * 2 cr_offset = y_size cb_offset = y_size + c_size Whether this format is supported by the camera hardware can be determined by getSupportedPreviewFormats().

//============================================================================
//! called when video data is available
void Java_com_waycoolapps_myp2pweb_NativeTxTo_fromGuiVideoData( JNIEnv * env, jobject thiz,
																jlong jiVideoFormat,
																jbyteArray pu8VidData, jint iWidth, jint iHeight, jint jiRotation )
{
	uint32_t u32FourCc = 0;
	//MyAndroidPrint( ANDROID_LOG_INFO, FROM_NATIVE_LOG_TAG, "6NativeFromJava::fromGuiVideoData format 0x%x\n", iVideoFormat );
	switch( (long)jiVideoFormat )
	{
	case DROID_NV21: // default image format for android camera
		u32FourCc = FOURCC_YUVS; // special for android
		break;

	case DROID_NV16:
		u32FourCc = FOURCC_IYUV;
		break;

	case DROID_YUY2:
		u32FourCc = FOURCC_YUY2;
		break;

	case DROID_YV12:
		u32FourCc = FOURCC_YV12;
		break;

	case FOURCC_RGB:
		u32FourCc = FOURCC_RGB;
		break;

		//case DROID_RGB_565; // not supported
	default:
		MyAndroidPrint( ANDROID_LOG_INFO, FROM_NATIVE_LOG_TAG, "fromGuiVideoData unsupported format %d\n", (int)jiVideoFormat );
		break;
	}

	if( 0 == u32FourCc )
	{
		return; // cant handle format
	}

	// get length of data we need to copy
	uint32_t u32ImageInLen = VxCalcImageDataLen( u32FourCc, iWidth, iHeight );
	if( 0 == u32ImageInLen )
	{
		MyAndroidPrint( ANDROID_LOG_INFO, FROM_NATIVE_LOG_TAG, "fromGuiVideoData 0 data length\n" );
		return; // cant handle format
	}
	
	jbyte* videoBytes = env->GetByteArrayElements(pu8VidData, NULL);
	// process the video
	GetEngineImp().getEngine().getMediaProcesser().fromGuiVideoData( u32FourCc, (uint8_t *)videoBytes, (int)iWidth, (int)iHeight, u32ImageInLen, (int)jiRotation );
	env->ReleaseByteArrayElements(pu8VidData, videoBytes, JNI_ABORT);
}

//============================================================================
void Java_com_waycoolapps_myp2pweb_NativeTxTo_fromGuiYUV420CaptureImage(  	JNIEnv * env, jobject thiz,
																			jbyteArray jyBytes, jbyteArray juBytes, jbyteArray jvBytes,
																			jint jyRowStride, jint juRowStride, jint jvRowStride,
																			jint jyPixStride, jint juPixStride, jint jvPixStride,
																			jint jwidth, jint jheight, jint jiRotate )
{
	jbyte * yBytes = env->GetByteArrayElements(jyBytes, NULL);
	jbyte * uBytes = env->GetByteArrayElements(juBytes, NULL);
	jbyte * vBytes = env->GetByteArrayElements(jvBytes, NULL);

	GetEngineImp().getEngine().getMediaProcesser().fromGuiYUV420CaptureImage( (uint8_t *)yBytes, (uint8_t *)uBytes, (uint8_t *)vBytes,
																			(int)jyRowStride, (int)juRowStride, (int)jvRowStride, 
																			(int)jyPixStride, (int)juPixStride, (int)jvPixStride, 
																			(int)jwidth, (int)jheight, (int)jiRotate );

	env->ReleaseByteArrayElements( jyBytes, yBytes, JNI_ABORT );
	env->ReleaseByteArrayElements( juBytes, uBytes, JNI_ABORT );
	env->ReleaseByteArrayElements( jvBytes, vBytes, JNI_ABORT );
}

//============================================================================
void Java_com_waycoolapps_myp2pweb_NativeTxTo_fromGuiMovieDone( JNIEnv*  env, jobject  thiz )
{
	GetEngineImp().fromGuiMovieDone();
}

//============================================================================
void Java_com_waycoolapps_myp2pweb_NativeTxTo_fromGuiUserLoggedOn( JNIEnv* env, jobject thiz, jobject netIdent )
{
	MyAndroidPrint( ANDROID_LOG_INFO, FROM_NATIVE_LOG_TAG, "fromGuiUserLoggedOn\n" );	
	VxNetIdent oIdent;
	java2VxNetIdentBase( netIdent, oIdent );
	//MyAndroidPrint( ANDROID_LOG_INFO, FROM_NATIVE_LOG_TAG, "fromGuiUserLoggedOn name %s id %s\n", oIdent.getOnlineName(), oIdent.getMyOnlineId().describeVxGUID().c_str() );

	//oIdent.debugDumpIdent();

	//MyAndroidPrint( ANDROID_LOG_INFO, FROM_NATIVE_LOG_TAG, "fromGuiUserLoggedOn called engine\n" );
	GetEngineImp().fromGuiUserLoggedOn( &oIdent );

	MyAndroidPrint( ANDROID_LOG_INFO, FROM_NATIVE_LOG_TAG, "fromGuiUserLoggedOn done\n" );
}

//============================================================================
void Java_com_waycoolapps_myp2pweb_NativeTxTo_fromGuiOnlineNameChanged( JNIEnv* env, jobject thiz, jstring jnewOnlineName )
{
	MyAndroidPrint( ANDROID_LOG_INFO, FROM_NATIVE_LOG_TAG, "NativeTxTo_fromGuiOnlineNameChanged\n" );
	const char * newOnlineName = env->GetStringUTFChars( jnewOnlineName, 0 );
	GetEngineImp().fromGuiOnlineNameChanged( newOnlineName );
	env->ReleaseStringUTFChars( jnewOnlineName, newOnlineName );
	MyAndroidPrint( ANDROID_LOG_INFO, FROM_NATIVE_LOG_TAG, "fromGuiOnlineNameChanged done\n" );
}

//============================================================================
void Java_com_waycoolapps_myp2pweb_NativeTxTo_fromGuiMoodMessageChanged( JNIEnv* env, jobject thiz, jstring jnewMoodMessage )
{
	MyAndroidPrint( ANDROID_LOG_INFO, FROM_NATIVE_LOG_TAG, "NativeTxTo_fromGuiMoodMessageChanged\n" );
	const char * newMoodMessage = env->GetStringUTFChars( jnewMoodMessage, 0 );
	GetEngineImp().fromGuiMoodMessageChanged( newMoodMessage );
	env->ReleaseStringUTFChars( jnewMoodMessage, newMoodMessage );
	MyAndroidPrint( ANDROID_LOG_INFO, FROM_NATIVE_LOG_TAG, "fromGuiMoodMessageChanged done\n" );
}

//============================================================================
void Java_com_waycoolapps_myp2pweb_NativeTxTo_fromGuiSetUserHasProfilePicture( JNIEnv* env, jobject thiz, jboolean jhasProfilePicture )
{
	GetEngineImp().fromGuiSetUserHasProfilePicture( (bool)jhasProfilePicture );
}

//============================================================================
	void Java_com_waycoolapps_myp2pweb_NativeTxTo_fromGuiUpdateMyIdent( JNIEnv* env, jobject thiz, jobject netIdent, jboolean permissionAndStatsOnly )
{
	g_FromJavaAccessMutex.lock();
	MyAndroidPrint( ANDROID_LOG_INFO, FROM_NATIVE_LOG_TAG, "javafromGuiUpdateMyIdent\n" );
	VxNetIdent oIdent;
	java2VxNetIdentBase( netIdent, oIdent );
	g_FromJavaAccessMutex.unlock();
	
	GetEngineImp().getEngine().fromGuiUpdateMyIdent( &oIdent, permissionAndStatsOnly );
	MyAndroidPrint( ANDROID_LOG_INFO, FROM_NATIVE_LOG_TAG, "javafromGuiUpdateMyIdent done\n" );
}

//============================================================================
void Java_com_waycoolapps_myp2pweb_NativeTxTo_fromGuiQueryMyIdent( JNIEnv* env, jobject thiz, jobject netIdent )
{
	MyAndroidPrint( ANDROID_LOG_INFO, FROM_NATIVE_LOG_TAG, "javafromGuiQueryMyIdent\n" );
	VxNetIdent myIdent;
	GetEngineImp().getEngine().fromGuiQueryMyIdent( &myIdent );
	g_FromJavaAccessMutex.lock();
	vxNetIdentBaseFillFriendJavaClass( &myIdent, netIdent );
	g_FromJavaAccessMutex.unlock();
	MyAndroidPrint( ANDROID_LOG_INFO, FROM_NATIVE_LOG_TAG, "javafromGuiQueryMyIdent done\n" );
}

//============================================================================
void Java_com_waycoolapps_myp2pweb_NativeTxTo_fromGuiSetIdentHasTextOffers( JNIEnv* env, jobject thiz, jlong u64IdHiPart, jlong u64IdLoPart, jboolean hasTextOffers )
{
	MyAndroidPrint( ANDROID_LOG_INFO, FROM_NATIVE_LOG_TAG, "NativeTxTo_fromGuiSetMyFriendshipToHim raw id 0x%llx 0x%llx\n", (uint64_t)u64IdHiPart, (uint64_t)u64IdLoPart );
	VxGUID oOnlineId(u64IdHiPart, u64IdLoPart);
	GetEngineImp().fromGuiSetIdentHasTextOffers( oOnlineId, hasTextOffers );
}

//============================================================================
void Java_com_waycoolapps_myp2pweb_NativeTxTo_fromGuiChangeMyFriendshipToHim( JNIEnv* env, jobject thiz, jlong u64IdHiPart, jlong u64IdLoPart, jint eMyFriendshipToHim, jint eHisFriendshipToMe )
{
	MyAndroidPrint( ANDROID_LOG_INFO, FROM_NATIVE_LOG_TAG, "NativeTxTo_fromGuiSetMyFriendshipToHim raw id 0x%llx 0x%llx\n", (uint64_t)u64IdHiPart, (uint64_t)u64IdLoPart );
	VxGUID oOnlineId(u64IdHiPart, u64IdLoPart);
	GetEngineImp().fromGuiChangeMyFriendshipToHim(	oOnlineId, (EFriendState)eMyFriendshipToHim, (EFriendState)eHisFriendshipToMe );
}

//============================================================================
void Java_com_waycoolapps_myp2pweb_NativeTxTo_fromGuiSetNetSettings( JNIEnv* env, jobject thiz, jobject jNetSettings )
{
	//MyAndroidPrint( ANDROID_LOG_INFO, FROM_NATIVE_LOG_TAG, "NativeTxTo_fromGuiSetNetSettings start\n" );
	g_FromJavaAccessMutex.lock();
	NetSettings netSettings;
	Java2NetSettings( jNetSettings, netSettings );
	g_FromJavaAccessMutex.unlock();
	GetEngineImp().getEngine().getEngineSettings().setNetSettings( netSettings );
	//MyAndroidPrint( ANDROID_LOG_INFO, FROM_NATIVE_LOG_TAG, "NativeTxTo_fromGuiSetNetSettings done\n" );
}

//============================================================================
void Java_com_waycoolapps_myp2pweb_NativeTxTo_fromGuiGetNetSettings( JNIEnv* env, jobject thiz, jobject jNetSettings )
{
	//MyAndroidPrint( ANDROID_LOG_INFO, FROM_NATIVE_LOG_TAG, "NativeTxTo_fromGuiSetNetSettings start\n" );
	NetSettings netSettings;
	GetEngineImp().getEngine().getEngineSettings().getNetSettings( netSettings );
	g_FromJavaAccessMutex.lock();
	FillNetSettingsJavaClass( netSettings, jNetSettings );
	g_FromJavaAccessMutex.unlock();
	//MyAndroidPrint( ANDROID_LOG_INFO, FROM_NATIVE_LOG_TAG, "NativeTxTo_fromGuiSetNetSettings done\n" );
}

//============================================================================
void Java_com_waycoolapps_myp2pweb_NativeTxTo_fromGuiNetworkSettingsChanged( JNIEnv* env, jobject thiz )
{
	//MyAndroidPrint( ANDROID_LOG_INFO, FROM_NATIVE_LOG_TAG, "NativeTxTo_fromGuiNetworkSettingsChanged\n" );
	GetEngineImp().fromGuiNetworkSettingsChanged();
}

//============================================================================
void Java_com_waycoolapps_myp2pweb_NativeTxTo_fromGuiSetRelaySettings( JNIEnv* env, jobject thiz, jint userRelayMaxCnt, jint systemRelayMaxCnt  )
{
	MyAndroidPrint( ANDROID_LOG_INFO, FROM_NATIVE_LOG_TAG, "NativeTxTo_fromGuiSetRelaySettings start\n" );
	GetEngineImp().getEngine().fromGuiSetRelaySettings( (int) userRelayMaxCnt, (int) systemRelayMaxCnt  );
	MyAndroidPrint( ANDROID_LOG_INFO, FROM_NATIVE_LOG_TAG, "NativeTxTo_fromGuiSetRelaySettings done\n" );
}

//============================================================================
void Java_com_waycoolapps_myp2pweb_NativeTxTo_fromGuiNetworkAvailable( JNIEnv* env, jobject thiz, jstring jstrIpAddr, jboolean isCellNetwork )
{
	MyAndroidPrint( ANDROID_LOG_INFO, FROM_NATIVE_LOG_TAG, "NativeTxTo_fromGuiNetworkAvailable\n" );
	const char * ipAddr = env->GetStringUTFChars( jstrIpAddr, 0 );
	GetEngineImp().fromGuiNetworkAvailable( ipAddr, isCellNetwork );
	env->ReleaseStringUTFChars( jstrIpAddr, ipAddr );
}

//============================================================================
void Java_com_waycoolapps_myp2pweb_NativeTxTo_fromGuiNetworkLost( JNIEnv* env, jobject thiz )
{
	MyAndroidPrint( ANDROID_LOG_INFO, FROM_NATIVE_LOG_TAG, "NativeTxTo_fromGuiNetworkLost\n" );
	GetEngineImp().fromGuiNetworkLost();
}

//============================================================================
int Java_com_waycoolapps_myp2pweb_NativeTxTo_fromGuiVerifyHostSettings( JNIEnv* env, jobject thiz )
{
	MyAndroidPrint( ANDROID_LOG_INFO, FROM_NATIVE_LOG_TAG, "NativeTxTo_fromGuiVerifyHostSettings\n" );
	GetEngineImp().fromGuiVerifyHostSettings();
	return 0;
}

//============================================================================
void Java_com_waycoolapps_myp2pweb_NativeTxTo_fromGuiRunIsPortOpenTest( JNIEnv* env, jobject thiz, jint iPort )
{
	MyAndroidPrint( ANDROID_LOG_INFO, FROM_NATIVE_LOG_TAG, "NativeTxTo_fromGuiRunIsPortOpenTest\n" );
	GetEngineImp().fromGuiRunIsPortOpenTest( (uint16_t)iPort );
}

//============================================================================
void Java_com_waycoolapps_myp2pweb_NativeTxTo_fromGuiSetFileShareSettings( JNIEnv* env, jobject thiz, jobject jFileShareSettings )
{
	MyAndroidPrint( ANDROID_LOG_INFO, FROM_NATIVE_LOG_TAG, "NativeTxTo_fromGuiSetFileShareSettings start\n" );
	g_FromJavaAccessMutex.lock();

	FileShareSettings fileShareSettings;
	Java2FileShareSettings( jFileShareSettings, fileShareSettings );
	g_FromJavaAccessMutex.unlock();
	GetEngineImp().getEngine().fromGuiSetFileShareSettings( fileShareSettings );

	MyAndroidPrint( ANDROID_LOG_INFO, FROM_NATIVE_LOG_TAG, "NativeTxTo_fromGuiSetFileShareSettings done\n" );
}

//============================================================================
void Java_com_waycoolapps_myp2pweb_NativeTxTo_fromGuiGetFileShareSettings( JNIEnv* env, jobject thiz, jobject jFileShareSettings )
{
	MyAndroidPrint( ANDROID_LOG_INFO, FROM_NATIVE_LOG_TAG, "NativeTxTo_fromGuiGetFileShareSettings start\n" );

	FileShareSettings fileShareSettings;
	GetEngineImp().getEngine().fromGuiGetFileShareSettings( fileShareSettings );
	g_FromJavaAccessMutex.lock();
	FillFileShareSettingsJavaClass( fileShareSettings, jFileShareSettings );
	g_FromJavaAccessMutex.unlock();
	MyAndroidPrint( ANDROID_LOG_INFO, FROM_NATIVE_LOG_TAG, "NativeTxTo_fromGuiGetFileShareSettings done\n" );
}

//============================================================================
void Java_com_waycoolapps_myp2pweb_NativeTxTo_fromGuiUpdateWebPageProfile( JNIEnv* env, jobject thiz,
																		 jstring	jstrProfileDir,	// directory containing user profile
																		 jstring	jstrGreeting,	// greeting text
																		 jstring	jaboutMe,		// about me text
																		 jstring	jurl1,			// favorite url 1
																		 jstring	jurl2,			// favorite url 2
																		 jstring	jurl3 )		// favorite url 3
{
	MyAndroidPrint( ANDROID_LOG_INFO, FROM_NATIVE_LOG_TAG, "NativeTxTo_fromGuiUpdateWebPageProfile\n" );
	g_FromJavaAccessMutex.lock();
	const char * pProfileDir = env->GetStringUTFChars( jstrProfileDir, 0 );
	const char * pGreeting = env->GetStringUTFChars( jstrGreeting, 0 );
	const char * pAboutMe = env->GetStringUTFChars( jaboutMe, 0 );
	const char * pUrl1 = env->GetStringUTFChars( jurl1, 0 );
	const char * pUrl2 = env->GetStringUTFChars( jurl2, 0 );
	const char * pUrl3 = env->GetStringUTFChars( jurl3, 0 );
	GetEngineImp().fromGuiUpdateWebPageProfile(		pProfileDir, 
													pGreeting,	//
													pAboutMe,		
													pUrl1,		
													pUrl2,
													pUrl3 );
	env->ReleaseStringUTFChars( jstrProfileDir, pProfileDir );
	env->ReleaseStringUTFChars( jstrGreeting, pGreeting );
	env->ReleaseStringUTFChars( jaboutMe, pAboutMe );
	env->ReleaseStringUTFChars( jurl1, pUrl1 );
	env->ReleaseStringUTFChars( jurl2, pUrl2 );
	env->ReleaseStringUTFChars( jurl3, pUrl3 );

	g_FromJavaAccessMutex.unlock();
}

//============================================================================
void Java_com_waycoolapps_myp2pweb_NativeTxTo_fromGuiSetPluginPermission( JNIEnv* env, jobject thiz, jint iPlugin, jint iPermission )
{
	MyAndroidPrint( ANDROID_LOG_INFO, FROM_NATIVE_LOG_TAG, "fromGuiSetPluginPermission plugin %d permission %d \n", iPlugin, iPermission );
	GetEngineImp().fromGuiSetPluginPermission( (EPluginType)iPlugin, iPermission );
}

//============================================================================
//! return non zero if plugin already in session
int Java_com_waycoolapps_myp2pweb_NativeTxTo_fromGuiGetPluginPermission( JNIEnv* env, jobject thiz, jint iPlugin )
{
	MyAndroidPrint( ANDROID_LOG_INFO, FROM_NATIVE_LOG_TAG, "NativeTxTo_fromGuiGetPluginPermission\n" );
	g_FromJavaAccessMutex.lock();
	int result = (int)GetEngineImp().fromGuiGetPluginPermission( (EPluginType)iPlugin );
	g_FromJavaAccessMutex.unlock();
	return result;
}

//============================================================================
//! return non zero if plugin already in session
int Java_com_waycoolapps_myp2pweb_NativeTxTo_fromGuiGetPluginServerState( JNIEnv* env, jobject thiz, jint iPlugin )
{
	MyAndroidPrint( ANDROID_LOG_INFO, FROM_NATIVE_LOG_TAG, "NativeTxTo_fromGuiGetPluginServerState\n" );
	g_FromJavaAccessMutex.lock();
	int result = (int)GetEngineImp().fromGuiGetPluginServerState( (EPluginType)iPlugin );
	g_FromJavaAccessMutex.unlock();
	return result;
}

//============================================================================
//! return non zero if plugin already in session
void Java_com_waycoolapps_myp2pweb_NativeTxTo_fromGuiStartPluginSession( JNIEnv* env, jobject thiz, jint iPlugin, jlong u64IdHiPart, jlong u64IdLoPart, jint iUserData, jlong jsessionIdHiPart, jlong jsessionIdLoPart )
{
	//MyAndroidPrint( ANDROID_LOG_INFO, FROM_NATIVE_LOG_TAG, "fromGuiStartPluginSession %d id 0x%llx, 0x%llx\n", iPlugin, u64IdHiPart, u64IdLoPart );
	VxGUID oOnlineId(u64IdHiPart, u64IdLoPart);
	VxGUID sessionId(jsessionIdHiPart, jsessionIdLoPart);
	GetEngineImp().fromGuiStartPluginSession( (EPluginType)iPlugin, oOnlineId, iUserData, sessionId );
}

//============================================================================
//! return non zero if plugin already in session
void Java_com_waycoolapps_myp2pweb_NativeTxTo_fromGuiStopPluginSession( JNIEnv* env, jobject thiz, jint iPlugin, jlong u64IdHiPart, jlong u64IdLoPart, jint iUserData, jlong jsessionIdHiPart, jlong jsessionIdLoPart )
{
	//MyAndroidPrint( ANDROID_LOG_INFO, FROM_NATIVE_LOG_TAG, "NativeTxTo_fromGuiStopPluginSession\n" );
	VxGUID oOnlineId(u64IdHiPart, u64IdLoPart);
	VxGUID sessionId(jsessionIdHiPart, jsessionIdLoPart);
	GetEngineImp().fromGuiStopPluginSession( (EPluginType)iPlugin, oOnlineId, iUserData, sessionId );
}

//============================================================================
jboolean Java_com_waycoolapps_myp2pweb_NativeTxTo_fromGuiIsPluginInSession( JNIEnv* env, jobject thiz, jint iPlugin, jlong u64IdHiPart, jlong u64IdLoPart, jint iUserData, jlong jsessionIdHiPart, jlong jsessionIdLoPart )
{
	//MyAndroidPrint( ANDROID_LOG_INFO, FROM_NATIVE_LOG_TAG, "NativeTxTo_fromGuiIsPluginInSession start\n" );
	VxGUID oOnlineId(u64IdHiPart, u64IdLoPart);
	VxGUID sessionId(jsessionIdHiPart, jsessionIdLoPart);
	bool isInSession = GetEngineImp().fromGuiIsPluginInSession( (EPluginType)iPlugin, oOnlineId, iUserData, sessionId );
	//MyAndroidPrint( ANDROID_LOG_INFO, FROM_NATIVE_LOG_TAG, "NativeTxTo_fromGuiIsPluginInSession done\n" );
	return (jboolean)isInSession;
}

//============================================================================
jboolean Java_com_waycoolapps_myp2pweb_NativeTxTo_fromGuiMakePluginOffer( JNIEnv* env, jobject thiz, jint iPlugin, jlong u64IdHiPart, jlong u64IdLoPart, jint u32ActionData, jstring strMsg, jstring strFileName, jlong sessionHiPart, jlong sessionLoPart )
{
	MyAndroidPrint( ANDROID_LOG_INFO, FROM_NATIVE_LOG_TAG, "NativeTxTo_fromGuiMakePluginOffer start\n" );
	VxGUID oOnlineId(u64IdHiPart, u64IdLoPart);
	VxGUID sessionId( sessionHiPart, sessionLoPart );
	g_FromJavaAccessMutex.lock();
	const char * pMsg = env->GetStringUTFChars(strMsg, 0);
	const char * pFileName = env->GetStringUTFChars( strFileName, 0 );
	g_FromJavaAccessMutex.unlock();

	//MyAndroidPrint( ANDROID_LOG_INFO, FROM_NATIVE_LOG_TAG, "NativeTxTo_fromGuiMakePluginOffer plugin %d action %d msg %s id 0x%llx 0x%llx\n",
	//	iPlugin,
	//	u32ActionData,
	//	pMsg,
	//	(uint64_t)u64IdHiPart, 
	//	(uint64_t)u64IdLoPart );

	bool bResult =  GetEngineImp().fromGuiMakePluginOffer( ( EPluginType )iPlugin, oOnlineId, ( int )u32ActionData, pMsg, pFileName, sessionId );

	//MyAndroidPrint( ANDROID_LOG_INFO, FROM_NATIVE_LOG_TAG, "NativeTxTo_fromGuiMakePluginOffer releasing msg start\n" );

	//Release memory (required or else memory leaks)
	g_FromJavaAccessMutex.lock();
	env->ReleaseStringUTFChars(strMsg, pMsg);
	env->ReleaseStringUTFChars( strFileName, pFileName );
	g_FromJavaAccessMutex.unlock();

	MyAndroidPrint( ANDROID_LOG_INFO, FROM_NATIVE_LOG_TAG, "NativeTxTo_fromGuiMakePluginOffer releasing msg done\n" );

	return (jboolean)bResult;
}

//============================================================================
jboolean Java_com_waycoolapps_myp2pweb_NativeTxTo_fromGuiToPluginOfferReply( JNIEnv* env, jobject thiz, jint iPlugin, jlong u64IdHiPart, jlong u64IdLoPart, jint iUserData, jint iResponse, jlong lclSessionIdHiPart, jlong lclSessionIdLoPart )
{
	MyAndroidPrint( ANDROID_LOG_INFO, FROM_NATIVE_LOG_TAG, "NativeTxTo_fromGuiToPluginOfferReply start\n" );
	g_FromJavaAccessMutex.lock();
	VxGUID oOnlineId(u64IdHiPart, u64IdLoPart);
	VxGUID lclSessionId( lclSessionIdHiPart, lclSessionIdLoPart );

	MyAndroidPrint( ANDROID_LOG_INFO, FROM_NATIVE_LOG_TAG, "NativeTxTo_fromGuiToPluginOfferReply plugin %d action %d response %d id 0x%llX 0x%llX\n",
															iPlugin,
															iUserData,
															iResponse,
															(uint64_t)u64IdHiPart, 
															(uint64_t)u64IdLoPart );

	bool bResult =  GetEngineImp().fromGuiToPluginOfferReply( ( EPluginType )iPlugin, oOnlineId, ( int )iUserData, iResponse, lclSessionId );

	MyAndroidPrint( ANDROID_LOG_INFO, FROM_NATIVE_LOG_TAG, "NativeTxTo_fromGuiToPluginOfferReply releasing msg done\n" );
	g_FromJavaAccessMutex.unlock();

	return (jboolean)bResult;
}

//============================================================================
int Java_com_waycoolapps_myp2pweb_NativeTxTo_fromGuiPluginControl( JNIEnv* env,
																	jobject thiz, jint iPlugin, 
																	jlong u64IdHiPartIn, jlong u64IdLoPartIn,
																	jstring strControl, jstring strAction, jint u32ActionDataIn,
																	jlong sessionHiPartIn, jlong sessionLoPartIn, 
																	jbyteArray fileHashId )
{
	g_FromJavaAccessMutex.lock();
	MyAndroidPrint( ANDROID_LOG_INFO, FROM_NATIVE_LOG_TAG, "NativeTxTo_fromGuiPluginControl raw id 0x%llx 0x%llx\n", (uint64_t)u64IdHiPartIn, (uint64_t)u64IdLoPartIn );
	bool haveHashId = false;
	VxSha1Hash sha1Hash;
	if ( 0 != fileHashId )
	{
		jbyte * hashBytes = env->GetByteArrayElements( fileHashId, NULL );
		sha1Hash.setHashData( ( uint8_t * )hashBytes );
		env->ReleaseByteArrayElements( fileHashId, hashBytes, JNI_ABORT );
		haveHashId = sha1Hash.isHashValid();
	}
	
	uint8_t * hashData = haveHashId ? sha1Hash.getHashData() : 0;	

	//MyAndroidPrint( ANDROID_LOG_INFO, FROM_NATIVE_LOG_TAG, "NativeTxTo_fromGuiPluginControl plugin %d control %s action %s id 0x%llX 0x%llX\n", 
		//iPlugin, 
		//pControl, 
		//pAction,
		//(uint64_t)u64IdHiPart, (uint64_t)u64IdLoPart );

	uint64_t u64IdHiPart = u64IdHiPartIn;
	uint64_t u64IdLoPart = u64IdLoPartIn;
	VxGUID oOnlineId( u64IdHiPart, u64IdLoPart );

	//Get the characters from the jstring
	const char * pControl	= "";
	const char * pAction	= "";
	
	if( 0 != strControl )
	{
		pControl	= env->GetStringUTFChars( strControl, 0 );		
	}
	
	if( 0 != strAction )
	{
		pAction	= env->GetStringUTFChars( strAction, 0 );		
	}
	
	uint64_t sessionHiPart = sessionHiPartIn;
	uint64_t sessionLoPart = sessionLoPartIn;
	VxGUID lclSessionId( sessionHiPart, sessionLoPart );
	uint32_t u32ActionData = ( uint32_t )u32ActionDataIn;
	int callResult = GetEngineImp().fromGuiPluginControl( ( EPluginType )iPlugin, oOnlineId, pControl, pAction, u32ActionData, lclSessionId, hashData );

	//Release memory (required or else memory leaks)
	if( 0 != strControl )
	{	
		env->ReleaseStringUTFChars( strControl, pControl );
	}
	
	if( 0 != strAction )
	{	
		env->ReleaseStringUTFChars( strAction, pAction );
	}
	
	g_FromJavaAccessMutex.unlock();

	return callResult;
}

//============================================================================
jboolean Java_com_waycoolapps_myp2pweb_NativeTxTo_fromGuiInstMsg( JNIEnv* env, jobject thiz, jlong u64IdHiPart, jlong u64IdLoPart, jint iPlugin, jstring strMsg )
{
	g_FromJavaAccessMutex.lock();
	VxGUID oOnlineId(u64IdHiPart, u64IdLoPart);

	//Get the characters from the jstring
	const char * pMsg = env->GetStringUTFChars(strMsg, 0);
	MyAndroidPrint( ANDROID_LOG_INFO, FROM_NATIVE_LOG_TAG, "NativeTxTo_fromGuiInstMsg plugin %d msg %s id 0x%llX 0x%llX\n", iPlugin, pMsg, (uint64_t)u64IdHiPart, (uint64_t)u64IdLoPart );

	bool bResult = GetEngineImp().fromGuiInstMsg( (EPluginType)iPlugin, oOnlineId, pMsg );

	//Release memory (required or else memory leaks)
	env->ReleaseStringUTFChars(strMsg, pMsg);
	g_FromJavaAccessMutex.unlock();

	return (jboolean)bResult;
}

//============================================================================
void Java_com_waycoolapps_myp2pweb_NativeTxTo_fromGuiUseRelay( JNIEnv* env, jobject thiz, jlong u64IdHiPart, jlong u64IdLoPart, jboolean bUseAsRelay )
{
	MyAndroidPrint( ANDROID_LOG_INFO, FROM_NATIVE_LOG_TAG, "NativeTxTo_fromGuiUseRelay start\n" );
	g_FromJavaAccessMutex.lock();
	VxGUID oOnlineId(u64IdHiPart, u64IdLoPart);

	GetEngineImp().fromGuiUseRelay( oOnlineId, bUseAsRelay );
	MyAndroidPrint( ANDROID_LOG_INFO, FROM_NATIVE_LOG_TAG, "NativeTxTo_fromGuiUseRelay done\n" );
	g_FromJavaAccessMutex.unlock();
}

//============================================================================
//! request list of contacts
void Java_com_waycoolapps_myp2pweb_NativeTxTo_fromGuiSendContactList( JNIEnv* env, jobject thiz, jint iFriendViewType )
{
	g_FromJavaAccessMutex.lock();
	int iMaxContactsToSend = 500;
	MyAndroidPrint( ANDROID_LOG_INFO, FROM_NATIVE_LOG_TAG, "fromGuiSendContactList view type %d max contacts %d\n", iFriendViewType, iMaxContactsToSend );
	GetEngineImp().fromGuiSendContactList( (EFriendViewType)iFriendViewType, iMaxContactsToSend );
	MyAndroidPrint( ANDROID_LOG_INFO, FROM_NATIVE_LOG_TAG, "fromGuiSendContactList done\n" );
	g_FromJavaAccessMutex.unlock();
}

//============================================================================
//! request list of contacts
void Java_com_waycoolapps_myp2pweb_NativeTxTo_fromGuiRefreshContactList( JNIEnv* env, jobject thiz )
{
	g_FromJavaAccessMutex.lock();
	int iMaxContactsToSend = 500;
	MyAndroidPrint( ANDROID_LOG_INFO, FROM_NATIVE_LOG_TAG, "fromGuiRefreshContactList max contacts %d\n", iMaxContactsToSend );
	GetEngineImp().fromGuiRefreshContactList( iMaxContactsToSend );
	MyAndroidPrint( ANDROID_LOG_INFO, FROM_NATIVE_LOG_TAG, "fromGuiRefreshContactList done\n" );
	g_FromJavaAccessMutex.unlock();
}

//============================================================================
void Java_com_waycoolapps_myp2pweb_NativeTxTo_fromGuiStartScan( 	JNIEnv* env,
																	jobject thiz,
																	jint eScanType,
																	jint searchFlags,
																	jint fileTypeFlags,
																	jstring strSearchPattern )
{
	MyAndroidPrint( ANDROID_LOG_INFO, FROM_NATIVE_LOG_TAG, "##fromGuiStartScan lock\n" );
	g_FromJavaAccessMutex.lock();
	MyAndroidPrint( ANDROID_LOG_INFO, FROM_NATIVE_LOG_TAG, "##fromGuiStartScan lock done\n" );
	const char * pSearchPattern = env->GetStringUTFChars( strSearchPattern, 0 );
	g_FromJavaAccessMutex.unlock();
	GetEngineImp().fromGuiStartScan( (EScanType)eScanType, (uint8_t)searchFlags, (uint8_t)fileTypeFlags, pSearchPattern );
	g_FromJavaAccessMutex.lock();
	env->ReleaseStringUTFChars( strSearchPattern, pSearchPattern );
	MyAndroidPrint( ANDROID_LOG_INFO, FROM_NATIVE_LOG_TAG, "##fromGuiStartScan start done\n" );
	g_FromJavaAccessMutex.unlock();
	MyAndroidPrint( ANDROID_LOG_INFO, FROM_NATIVE_LOG_TAG, "##fromGuiStartScan unlock done\n" );
}

//============================================================================
void Java_com_waycoolapps_myp2pweb_NativeTxTo_fromGuiNextScan( 	JNIEnv* env,
															  jobject thiz,
															  jint eScanType )
{
	MyAndroidPrint( ANDROID_LOG_INFO, FROM_NATIVE_LOG_TAG, "NativeTxTo_fromGuiNextScan\n" );
	g_FromJavaAccessMutex.lock();
	GetEngineImp().fromGuiNextScan( (EScanType)eScanType );
	g_FromJavaAccessMutex.unlock();
}

//============================================================================
void Java_com_waycoolapps_myp2pweb_NativeTxTo_fromGuiStopScan( 	JNIEnv* env,
															  jobject thiz,
															  jint eScanType )
{
	g_FromJavaAccessMutex.lock();
	MyAndroidPrint( ANDROID_LOG_INFO, FROM_NATIVE_LOG_TAG, "##fromGuiStopScan start\n" );
	GetEngineImp().fromGuiStopScan( (EScanType)eScanType );
	MyAndroidPrint( ANDROID_LOG_INFO, FROM_NATIVE_LOG_TAG, "##fromGuiStopScan start done\n" );
	g_FromJavaAccessMutex.unlock();
}

//============================================================================
void Java_com_waycoolapps_myp2pweb_NativeTxTo_fromGuiUserModifiedStoryboard( 	JNIEnv* env,
																				jobject thiz )
{
	g_FromJavaAccessMutex.lock();
	MyAndroidPrint( ANDROID_LOG_INFO, FROM_NATIVE_LOG_TAG, "NativeTxTo_fromGuiUserModifiedStoryboard\n" );
	GetEngineImp().fromGuiUserModifiedStoryboard();
	g_FromJavaAccessMutex.unlock();
}

//============================================================================
void Java_com_waycoolapps_myp2pweb_NativeTxTo_fromGuiCancelDownload( 	JNIEnv* env,
																		jobject thiz,
																		jlong fileInstanceHi,
																		jlong fileInstanceLo )
{
	g_FromJavaAccessMutex.lock();
	MyAndroidPrint( ANDROID_LOG_INFO, FROM_NATIVE_LOG_TAG, "NativeTxTo_fromGuiCancelDownload\n" );
	VxGUID fileInstance( fileInstanceHi, fileInstanceLo );
	GetEngineImp().fromGuiCancelDownload( fileInstance );
	g_FromJavaAccessMutex.unlock();
}

//============================================================================
void Java_com_waycoolapps_myp2pweb_NativeTxTo_fromGuiCancelUpload( 	JNIEnv* env,
																	jobject thiz,
																	jlong fileInstanceHi,
																	jlong fileInstanceLo )
{
	g_FromJavaAccessMutex.lock();
	MyAndroidPrint( ANDROID_LOG_INFO, FROM_NATIVE_LOG_TAG, "NativeTxTo_fromGuiCancelUpload\n" );
	VxGUID fileInstance( fileInstanceHi, fileInstanceLo );
	GetEngineImp().fromGuiCancelUpload( fileInstance );
	g_FromJavaAccessMutex.unlock();
}

//============================================================================
jboolean Java_com_waycoolapps_myp2pweb_NativeTxTo_fromGuiTestCmd( 	JNIEnv*		env,
																  jobject	thiz,
																  jint		eTestParam1,
																  jint		eTestParam2,
																  jstring	strTestParam3 )
{
	g_FromJavaAccessMutex.lock();
	MyAndroidPrint( ANDROID_LOG_INFO, FROM_NATIVE_LOG_TAG, "NativeTxTo_fromGuiTestCmd\n" );
	const char * pTestParam3 = env->GetStringUTFChars(strTestParam3, 0);
	bool bResult = GetEngineImp().fromGuiTestCmd( (IFromGui::ETestParam1)eTestParam1, eTestParam2, pTestParam3 );
	env->ReleaseStringUTFChars( strTestParam3, pTestParam3 );
	g_FromJavaAccessMutex.unlock();
	return (jboolean)bResult;
}

//============================================================================
jboolean Java_com_waycoolapps_myp2pweb_NativeTxTo_fromGuiSetGameValueVar(	JNIEnv* env, 
																		jobject thiz, 
																		jint iPlugin, 
																		jlong u64IdHiPart, 
																		jlong u64IdLoPart, 
																		jint iVarId,
																		jint iVarValue )
{
	MyAndroidPrint( ANDROID_LOG_INFO, FROM_NATIVE_LOG_TAG, "##fromGuiSetGameValueVar\n" );
	g_FromJavaAccessMutex.lock();
	VxGUID oOnlineId(u64IdHiPart, u64IdLoPart);

	bool bResult = GetEngineImp().fromGuiSetGameValueVar( (EPluginType)iPlugin, oOnlineId, (int32_t)iVarId, (int32_t)iVarValue );
	g_FromJavaAccessMutex.unlock();
	return (jboolean)bResult;
}

//============================================================================
jboolean Java_com_waycoolapps_myp2pweb_NativeTxTo_fromGuiSetGameActionVar(	JNIEnv* env, 
																					 jobject thiz, 
																					 jint iPlugin, 
																					 jlong u64IdHiPart, 
																					 jlong u64IdLoPart, 
																					 jint iVarId,
																					 jint iVarValue )
{
	MyAndroidPrint( ANDROID_LOG_INFO, FROM_NATIVE_LOG_TAG, "##fromGuiSetGameActionVar\n" );
	g_FromJavaAccessMutex.lock();
	VxGUID oOnlineId(u64IdHiPart, u64IdLoPart);

	bool bResult = GetEngineImp().fromGuiSetGameActionVar( (EPluginType)iPlugin, oOnlineId, (int32_t)iVarId, (int32_t)iVarValue );
	g_FromJavaAccessMutex.unlock();
	return (jboolean)bResult;
}

//============================================================================
int Java_com_waycoolapps_myp2pweb_NativeTxTo_fromGuiGetRandomTcpPort()
{
	g_FromJavaAccessMutex.lock();
	uint16_t u16Port = VxGetRandomTcpPort();
	MyAndroidPrint( ANDROID_LOG_INFO, FROM_NATIVE_LOG_TAG, "fromGuiGetRandomTcpPort got port %d\n", u16Port );
	g_FromJavaAccessMutex.unlock();
	return u16Port;
}

//============================================================================
void Java_com_waycoolapps_myp2pweb_NativeTxTo_fromGuiMulitcastPkt( JNIEnv*  env, jobject  thiz, jbyteArray jpktData, jint dataLen )
{
	MyAndroidPrint( ANDROID_LOG_INFO, FROM_NATIVE_LOG_TAG, "NativeTxTo_fromGuiMulitcastPkt\n" );
	g_FromJavaAccessMutex.lock();
	jbyte* pktData = env->GetByteArrayElements(jpktData, NULL);
	GetEngineImp().fromGuiMulitcastPkt( (uint8_t *)pktData, (int)dataLen );
	env->ReleaseByteArrayElements( jpktData, pktData, JNI_ABORT);
	g_FromJavaAccessMutex.unlock();
}

//============================================================================
jboolean Java_com_waycoolapps_myp2pweb_NativeTxTo_fromGuiBrowseFiles( JNIEnv*  env, jobject  thiz, jstring jstrDir, jboolean lookupShareStatus, jint fileFilterMask )
{
	g_FromJavaAccessMutex.lock();
	const char * pDirToList = env->GetStringUTFChars( jstrDir, 0 );
	g_FromJavaAccessMutex.unlock();
	jboolean result = (jboolean) GetEngineImp().fromGuiBrowseFiles( pDirToList, (bool)lookupShareStatus, (uint8_t)fileFilterMask );
	g_FromJavaAccessMutex.lock();
	env->ReleaseStringUTFChars( jstrDir, pDirToList );
	g_FromJavaAccessMutex.unlock();
	return result;
}

//============================================================================
jboolean Java_com_waycoolapps_myp2pweb_NativeTxTo_fromGuiGetSharedFiles( JNIEnv*  env, jobject  thiz, jint jintFileFilter )
{
	return (jboolean) GetEngineImp().fromGuiGetSharedFiles( (uint8_t)jintFileFilter );
}

//============================================================================
jboolean Java_com_waycoolapps_myp2pweb_NativeTxTo_fromGuiSetFileIsShared( JNIEnv*  env, jobject  thiz, jstring fileName, jboolean isShared )
{
	g_FromJavaAccessMutex.lock();
	const char * pFileName = env->GetStringUTFChars( fileName, 0 );
	g_FromJavaAccessMutex.unlock();
	jboolean result = (jboolean) GetEngineImp().fromGuiSetFileIsShared( pFileName, (bool)isShared );
	g_FromJavaAccessMutex.lock();
	env->ReleaseStringUTFChars( fileName, pFileName );
	g_FromJavaAccessMutex.unlock();
	return (jboolean)result;
}
	
//============================================================================
jboolean Java_com_waycoolapps_myp2pweb_NativeTxTo_fromGuiGetIsFileShared( JNIEnv*  env, jobject  thiz, jstring fileName )
{
	g_FromJavaAccessMutex.lock();
	const char * pFileName = env->GetStringUTFChars( fileName, 0 );
	g_FromJavaAccessMutex.unlock();
	jboolean result = (jboolean) GetEngineImp().fromGuiGetIsFileShared( pFileName );
	g_FromJavaAccessMutex.lock();
	env->ReleaseStringUTFChars( fileName, pFileName );
	g_FromJavaAccessMutex.unlock();
	return (jboolean)result;
}

//============================================================================
jint Java_com_waycoolapps_myp2pweb_NativeTxTo_fromGuiGetFileDownloadState( JNIEnv*  env, jobject  thiz, jbyteArray fileHashId )
{
	g_FromJavaAccessMutex.lock();
	bool haveHashId = false;
	VxSha1Hash sha1Hash;
	if ( 0 != fileHashId )
	{
		jbyte * hashBytes = env->GetByteArrayElements( fileHashId, NULL );
		sha1Hash.setHashData( ( uint8_t * )hashBytes );
		env->ReleaseByteArrayElements( fileHashId, hashBytes, JNI_ABORT );
		haveHashId = sha1Hash.isHashValid();
	}
	
	g_FromJavaAccessMutex.unlock();
	int result = -1;
	if ( haveHashId )
	{
		result = GetEngineImp().fromGuiGetFileDownloadState( sha1Hash.getHashData() );
	}
	
	return result;
}
	
//============================================================================
jboolean Java_com_waycoolapps_myp2pweb_NativeTxTo_fromGuiAddFileToLibrary( JNIEnv*  env, jobject  thiz, jstring fileName, jboolean addFile, jbyteArray fileHashId )
{
	g_FromJavaAccessMutex.lock();
	const char * pFileName = env->GetStringUTFChars( fileName, 0 );
	bool haveHashId = false;
	VxSha1Hash sha1Hash;
	if ( 0 != fileHashId )
	{
		jbyte * hashBytes = env->GetByteArrayElements( fileHashId, NULL );
		sha1Hash.setHashData( (uint8_t *)hashBytes );
		env->ReleaseByteArrayElements( fileHashId, hashBytes, JNI_ABORT );
		haveHashId = sha1Hash.isHashValid();
	}
	
	g_FromJavaAccessMutex.unlock();
	
	jboolean result = ( jboolean ) GetEngineImp().fromGuiAddFileToLibrary( pFileName, ( bool )addFile, haveHashId ? sha1Hash.getHashData() : 0 );
	g_FromJavaAccessMutex.lock();
	env->ReleaseStringUTFChars( fileName, pFileName );
	g_FromJavaAccessMutex.unlock();
	return (jboolean)result;
}
	
//============================================================================
void Java_com_waycoolapps_myp2pweb_NativeTxTo_fromGuiGetFileLibraryList( JNIEnv*  env, jobject  thiz, jint fileTypeFilter )
{
	GetEngineImp().fromGuiGetFileLibraryList( ( uint8_t )fileTypeFilter );
}	

//============================================================================
jboolean Java_com_waycoolapps_myp2pweb_NativeTxTo_fromGuiGetIsFileInLibrary( JNIEnv*  env, jobject  thiz, jstring fileName )
{
	g_FromJavaAccessMutex.lock();
	const char * pFileName = env->GetStringUTFChars( fileName, 0 );
	g_FromJavaAccessMutex.unlock();
	jboolean result = (jboolean) GetEngineImp().fromGuiGetIsFileInLibrary( pFileName );
	g_FromJavaAccessMutex.lock();
	env->ReleaseStringUTFChars( fileName, pFileName );
	g_FromJavaAccessMutex.unlock();
	return (jboolean)result;
}

//============================================================================
jboolean Java_com_waycoolapps_myp2pweb_NativeTxTo_fromGuiIsMyP2PWebVideoFile( JNIEnv*  env, jobject  thiz, jstring fileName )
{
	g_FromJavaAccessMutex.lock();
	const char * pFileName = env->GetStringUTFChars( fileName, 0 );
	g_FromJavaAccessMutex.unlock();
	jboolean result = (jboolean) GetEngineImp().fromGuiIsMyP2PWebVideoFile( pFileName );
	g_FromJavaAccessMutex.lock();
	env->ReleaseStringUTFChars( fileName, pFileName );
	g_FromJavaAccessMutex.unlock();
	return result;
}

//============================================================================
jboolean Java_com_waycoolapps_myp2pweb_NativeTxTo_fromGuiIsMyP2PWebAudioFile( JNIEnv*  env, jobject  thiz, jstring fileName )
{
	g_FromJavaAccessMutex.lock();
	const char * pFileName = env->GetStringUTFChars( fileName, 0 );
	g_FromJavaAccessMutex.unlock();
	jboolean result = (jboolean) GetEngineImp().fromGuiIsMyP2PWebAudioFile( pFileName );
	g_FromJavaAccessMutex.lock();
	env->ReleaseStringUTFChars( fileName, pFileName );
	g_FromJavaAccessMutex.unlock();
	return result;
}
	
//============================================================================
jint Java_com_waycoolapps_myp2pweb_NativeTxTo_fromGuiDeleteFile( JNIEnv*  env, jobject  thiz, jstring fileName, jboolean shredFile )
{
	g_FromJavaAccessMutex.lock();
	const char * pFileName = env->GetStringUTFChars( fileName, 0 );
	g_FromJavaAccessMutex.unlock();
	jint result = ( jint ) GetEngineImp().fromGuiDeleteFile( pFileName, ( bool )shredFile );
	g_FromJavaAccessMutex.lock();
	env->ReleaseStringUTFChars( fileName, pFileName );
	g_FromJavaAccessMutex.unlock();
	return result;
}

//============================================================================
jboolean Java_com_waycoolapps_myp2pweb_NativeTxTo_fromGuiAssetAction( JNIEnv* env, jobject thiz, jint assetAction, jobject assetInfoIn, jint pos0to100000 )
{
	jboolean result = ( jboolean )false;
	g_FromJavaAccessMutex.lock();
	AssetInfo assetInfo;
	bool convertOk = Java2AssetInfo( assetInfoIn, &assetInfo );
	g_FromJavaAccessMutex.unlock();
	if ( convertOk )
	{
		result = ( jboolean ) GetEngineImp().fromGuiAssetAction( ( EAssetAction )assetAction, assetInfo, pos0to100000 );
	}
	else
	{
		LogMsg( LOG_ERROR, "NativeTxTo_fromGuiAssetAction failed to convert AssetInfo\n " );
	}
	
	return result;
}

//============================================================================
jboolean Java_com_waycoolapps_myp2pweb_NativeTxTo_fromGuiAssetActionById( JNIEnv* env, jobject thiz, jint assetAction, jlong assetIdHiPart, jlong assetIdLoPart, jint pos0to100000 )
{
	//fromGuiAssetAction( EAssetAction assetAction, VxGUID& assetId, int pos0to100000 = 0 )
	VxGUID assetId( assetIdHiPart, assetIdLoPart );
	jboolean result = ( jboolean ) GetEngineImp().fromGuiAssetAction( ( EAssetAction )assetAction, assetId, pos0to100000 );
	return result;
}

//============================================================================
jboolean Java_com_waycoolapps_myp2pweb_NativeTxTo_fromGuiVideoRecord( JNIEnv*  env, jobject  thiz, jint eRecState, jlong feedIdHiPart, jlong feedIdLoPart, jstring fileName )
{
	const char * pFileName = 0;
	if( 0 != fileName )
	{
		pFileName = env->GetStringUTFChars( fileName, 0 );
	}

	VxGUID feedId( ( uint64_t )feedIdHiPart, ( uint64_t )feedIdLoPart );
	bool actionResult = GetEngineImp().fromGuiVideoRecord( ( EVideoRecordState )eRecState, feedId, pFileName );
	if( 0 != fileName )
	{
		env->ReleaseStringUTFChars( fileName, pFileName );
	}

	return (jboolean)actionResult;
}
	
//============================================================================
jboolean Java_com_waycoolapps_myp2pweb_NativeTxTo_fromGuiSndRecord( JNIEnv*  env, jobject  thiz, jint eRecState, jlong feedIdHiPart, jlong feedIdLoPart, jstring fileName )
{
	const char * pFileName = 0;
	if( 0 != fileName )
	{
		pFileName = env->GetStringUTFChars( fileName, 0 );
	}

	VxGUID feedId( ( uint64_t )feedIdHiPart, ( uint64_t )feedIdLoPart );
	bool actionResult = GetEngineImp().fromGuiSndRecord( ( ESndRecordState )eRecState, feedId, pFileName );
	if( 0 != fileName )
	{
		env->ReleaseStringUTFChars( fileName, pFileName );
	}

	return (jboolean)actionResult;
}	
	
//============================================================================
void Java_com_waycoolapps_myp2pweb_NativeTxTo_fromGuiQuerySessionHistory( JNIEnv*  env, jobject  thiz, jlong historyIdHiPart, jlong historyIdLoPart )
{
	VxGUID historyId( historyIdHiPart, historyIdLoPart );
	GetEngineImp().fromGuiQuerySessionHistory( historyId );
}

//============================================================================
jboolean Java_com_waycoolapps_myp2pweb_NativeTxTo_fromGuiMultiSessionAction( JNIEnv* env, jobject thiz, jint mSessionAction, jlong onlineIdHiPart, jlong onlineIdLoPart, jint pos0to100000 )
{
	VxGUID onlineId( onlineIdHiPart, onlineIdLoPart );
	jboolean result = ( jboolean ) GetEngineImp().fromGuiMultiSessionAction( ( EMSessionAction )mSessionAction, onlineId, pos0to100000 ) ;
	return result;
}

}// extern C

// android specific code
#endif // TARGET_OS_ANDROID



