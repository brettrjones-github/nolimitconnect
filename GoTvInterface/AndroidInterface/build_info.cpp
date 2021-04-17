/*
*  Copyright (c) 2015 The WebRTC project authors. All Rights Reserved.
*
*  Use of this source code is governed by a BSD-style license
*  that can be found in the LICENSE file in the root of the source
*  tree. An additional intellectual property rights grant can be found
*  in the file PATENTS.  All contributing project authors may
*  be found in the AUTHORS file in the root of the source tree.
*/
// Code Modified for MyP2PWeb by Brett R. Jones

#ifdef TARGET_OS_ANDROID
#include "build_info.h"
#include "BuildInfoJava.h"

namespace webrtc 
{
	//============================================================================
	BuildInfo::BuildInfo()
	: m_BuildInfoJavaClass( 0 )
	, m_BuildInfoJavaObject( 0 )
	, m_Initialized( false )
	{
	}

	//============================================================================
	void BuildInfo::assureBuildInfoInitialized( void )
	{
		if( !m_Initialized )
		{
			m_DeviceModel			= getStringFromJava( "getDeviceModel" );
			m_Brand					= getStringFromJava( "getBrand" );
			m_DeviceManufacturer	= getStringFromJava( "getDeviceManufacturer" );
			m_AndroidBuildId		= getStringFromJava( "getAndroidBuildId" );
			m_BuildType				= getStringFromJava( "getBuildType" );
			m_BuildRelease			= getStringFromJava( "getBuildRelease" );
			m_SdkVersion			= getStringFromJava( "getSdkVersion" );

			m_Initialized = true;
		}
	}

	//============================================================================
	std::string BuildInfo::getStringFromJava( const char* name ) 
	{
		if( 0 == m_BuildInfoJavaClass )
		{
			m_BuildInfoJavaClass = GetBuildInfoJavaClass();
			m_BuildInfoJavaObject = m_BuildInfoJavaClass->creatNewObj();
		}

		jmethodID id = m_BuildInfoJavaClass->getStaticMethodId( name, "()Ljava/lang/String;" );
		jstring j_string = static_cast<jstring>( m_BuildInfoJavaClass->callStaticObjectMethod( id ) );
		return m_BuildInfoJavaClass->javaToStdString(j_string);
	}

	//============================================================================
	std::string BuildInfo::GetDeviceModel() 
	{
		assureBuildInfoInitialized();
		return m_DeviceModel;
	}

	//============================================================================
	std::string BuildInfo::GetBrand() 
	{
		assureBuildInfoInitialized();
		return m_Brand;
	}

	//============================================================================
	std::string BuildInfo::GetDeviceManufacturer() 
	{
		assureBuildInfoInitialized();
		return m_DeviceManufacturer;
	}

	//============================================================================
	std::string BuildInfo::GetAndroidBuildId() 
	{
		assureBuildInfoInitialized();
		return m_AndroidBuildId;
	}

	//============================================================================
	std::string BuildInfo::GetBuildType() 
	{
		assureBuildInfoInitialized();
		return m_BuildType;
	}

	//============================================================================
	std::string BuildInfo::GetBuildRelease() 
	{
		assureBuildInfoInitialized();
		return m_BuildRelease;
	}

	//============================================================================
	std::string BuildInfo::GetSdkVersion() 
	{
		assureBuildInfoInitialized();
		return m_SdkVersion;
	}

}  // namespace webrtc

#endif // TARGET_OS_ANDROID
