#pragma once
//============================================================================
// Copyright (C) 2014 Brett R. Jones
// All Rights Reserved ( After My Death I Brett R. Jones Issue this source code to Public Domain )
//
// You may not modify or redistribute this code for any reason
// without written consent of Brett R. Jones
//
// This code is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
//
// brettjones1900@gmail.com
// http://www.p2panarchy.com
//============================================================================

#include <AppDependLibrariesConfig.h>

#include <CoreLib/AppErr.h>
#include <CoreLib/VxThread.h>
#include <CoreLib/VxMutex.h>
#include <NetLib/InetAddress.h>
#include <NetLib/VxSktUdp.h>

#include <string>

typedef void (*PORT_FORWARD_CALLBACK_FUNCTION )( void *, EAppErr );

class UpnpParser;
class UpnpRootDevice;

class UpnpPortForward
{
public:
	UpnpPortForward();
	virtual ~UpnpPortForward();

	void						upnpPortForwardShutdown( void );

	int							getPortToMap( void )				{ return m_iPortToForward; }
	InetAddress&				getLocalIpAddress( void )			{ return m_LocalIpAddr; }
	std::string					getNameOfServiceToForward( void )	{ return m_strServiceToForward; }

	void						setPortForwardResultCallback( PORT_FORWARD_CALLBACK_FUNCTION pfuncPortForwardCallbackHandler, void * userData );
	virtual void				tcpPortForward( int port, const char* pServiceName, const char* pLocalIPv4 = "" );

	bool						shouldAbort( void );
	void						runPortForward( void );

	void						doSearchResponseCallback( VxSktBase* sktBase );
	void						deviceAddPortMapSuccessFailCallback( bool bResult );
protected:
	void						onSearchResponseReceived( UpnpParser * upnpParser );
	void						sendUpnpDeviceSearchBroadcast( void );
	UpnpParser *				createUpnpParser( VxSktBase* sktBase, const char* pData );
	bool						haveAlreadyRequestedDeviceInfo( std::string& deviceUrl );
	void						removeAllRootDevices( void );


	int							m_iPortToForward;
	std::string					m_strServiceToForward;
	std::string					m_strLocalIPv4;
	InetAddress					m_LocalIpAddr;

	VxThread					m_PortForwardThread;
	VxMutex						m_PortForwardMutex;

	PORT_FORWARD_CALLBACK_FUNCTION	m_pfuncPortForwardCallbackHandler;
	void *						m_PortForwardCallbackUserData;

	VxSktUdp					m_UpnpSearchResponseSocket;

	std::vector<UpnpRootDevice *> m_RootDeviceList;
	std::vector<std::string>	m_DeviceInfoUrls;
	std::vector<std::string>	m_SearchResponseUrls;
};



