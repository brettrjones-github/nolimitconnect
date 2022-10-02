#pragma once
//============================================================================
// Copyright (C) 2012 Brett R. Jones
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

#include "UpnpParser.h"

#include <CoreLib/VxThread.h>
#include <CoreLib/VxMutex.h>
#include <CoreLib/VxXml.h>

#include <string>

class UpnpPortForward;
class UpnpTcpSkt;
class InetAddress;

class UpnpRootDevice
{
public:
	UpnpRootDevice( UpnpPortForward& client, std::string& strUrl );

	void						startup();
	void						preShutdown();
	void						shutdown();
	bool						doRootDeviceQueryThreadFunction();

	UpnpParser&					getParser();
	void						sendResult( bool bResult );

	void						xmlCallback( EXmlParseType eXmlType, const char* pName, const char* pValue );
	bool						currentXmlTags( const char* str1, const char* str2 );

private:
	bool						addPortMapToDevice( void );
	bool						parseDeviceInfo( void );
	bool						requestAddPortMap( InetAddress& lclAddr, int iPortToMap, const char* pProtocol );

public:
	UpnpPortForward&			m_Client;
	UpnpParser					m_Parser;
	VxThread					m_DeviceThread;
	VxMutex						m_Mutex;

	std::string					m_strUrl;
	std::string					m_strHostName;
	int							m_iPort;
	std::string					m_strPath;
	bool						m_bDisabled;

	std::vector<std::string>	m_aoXmlTags;
	bool						m_bAmParsingService;
	std::string					m_strControlUrl;
	std::string					m_strModel;
	std::string					m_strBaseUrl;
	std::string					m_strServiceType;
	UpnpTcpSkt *				m_stUpnpSkt;

};
