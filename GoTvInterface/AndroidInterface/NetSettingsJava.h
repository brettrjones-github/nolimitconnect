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

#include "JavaBaseClass.h"

class NetSettingsJavaClass : public JavaBaseClass
{
public:
	NetSettingsJavaClass( JavaEnvCache& envCache, jclass javaRefClass );
	virtual ~NetSettingsJavaClass();

	//=== vars ===//
	//=== java class ids ===//
	jfieldID					m_NetworkNameID;
	jfieldID					m_NetworkHostUrlID;
	jfieldID					m_HostWebsiteUrlID;

	jfieldID					m_ThisDeviceIsAnHostID;

	jfieldID					m_MyTcpInPortID;
	jfieldID					m_MyMulticastPortID;
	jfieldID					m_MulticastEnableID;
	jfieldID					m_UseUpnpPortForwardID;

};

NetSettingsJavaClass * GetNetSettingsJavaClass( void );
void NetSettingsJavaClassThreadExit( unsigned int uiThreadId );
void NetSettingsJavaClassShutdown( void );

bool Java2NetSettings( jobject& oNetSettingsClass, NetSettings& netSettings );
bool NetSettings2Java( NetSettings& netSettings, jobject& oNetSettingsClass );
bool FillNetSettingsJavaClass( NetSettings& netSettings, jobject& oNetSettingsClass );


#endif //TARGET_OS_ANDROID
