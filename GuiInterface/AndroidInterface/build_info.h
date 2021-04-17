/*
*  Copyright (c) 2015 The WebRTC project authors. All Rights Reserved.
*
*  Use of this source code is governed by a BSD-style license
*  that can be found in the LICENSE file in the root of the source
*  tree. An additional intellectual property rights grant can be found
*  in the file PATENTS.  All contributing project authors may
*  be found in the AUTHORS file in the root of the source tree.
*/
#ifdef TARGET_OS_ANDROID
#ifndef WEBRTC_MODULES_AUDIO_DEVICE_ANDROID_BUILD_INFO_H_
#define WEBRTC_MODULES_AUDIO_DEVICE_ANDROID_BUILD_INFO_H_

#include <jni.h>
#include <memory>
#include <string>

#include "BuildInfoJava.h"

namespace webrtc {

	// Utility class used to query the Java class (org/webrtc/voiceengine/BuildInfo)
	// for device and Android build information.
	// The calling thread is attached to the JVM at construction if needed and a
	// valid Java environment object is also created.
	// All Get methods must be called on the creating thread. If not, the code will
	// hit RTC_DCHECKs when calling JNIEnvironment::JavaToStdString().
	class BuildInfo 
	{
	public:
		BuildInfo();
		~BuildInfo() {}

		// End-user-visible name for the end product (e.g. "Nexus 6").
		std::string				GetDeviceModel( void );
		// Consumer-visible brand (e.g. "google").
		std::string				GetBrand( void );
		// Manufacturer of the product/hardware (e.g. "motorola").
		std::string				GetDeviceManufacturer( void );
		// Android build ID (e.g. LMY47D).
		std::string				GetAndroidBuildId( void );
		// The type of build (e.g. "user" or "eng").
		std::string				GetBuildType( void );
		// The user-visible version string (e.g. "5.1").
		std::string				GetBuildRelease( void );
		// The user-visible SDK version of the framework (e.g. 21).
		std::string				GetSdkVersion( void );

	private:
		void					assureBuildInfoInitialized( void );
	// Helper method which calls a static getter method with |name| and returns
		// a string from Java.
		std::string				getStringFromJava( const char* name );

		// Holds the jclass object and provides access to CallStaticObjectMethod().
		// Used by GetStringFromJava() during construction only.
		BuildInfoJavaClass *	m_BuildInfoJavaClass;
		jobject					m_BuildInfoJavaObject;
		bool					m_Initialized;

		std::string				m_DeviceModel;
		std::string				m_Brand;
		std::string				m_DeviceManufacturer;
		std::string				m_AndroidBuildId;
		std::string				m_BuildType;
		std::string				m_BuildRelease;
		std::string				m_SdkVersion;

	};

}  // namespace webrtc

#endif  // WEBRTC_MODULES_AUDIO_DEVICE_ANDROID_BUILD_INFO_H_
#endif // TARGET_OS_ANDROID
