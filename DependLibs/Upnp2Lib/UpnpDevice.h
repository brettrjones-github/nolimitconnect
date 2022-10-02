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
#include "UpnpPortForwardService.h"

class SSPDClient;

class UpnpDevice
{
public:
	UpnpParser					m_oParser;
	SSPDClient&					m_oClient;
	UpnpPortForwardService		m_oIpConnService;


	std::string					m_strBaseURL;
	std::string					m_strFriendlyName;
	std::string					m_strUDN;

	std::string					m_strDeviceType;

public:
	UpnpDevice(SSPDClient& client);

	UpnpParser&			getParser();
	UpnpService&		getIpConnService();

#ifdef USE_TINYXML
	bool				parseDeviceFromXml( const char* pXml, int iXmlLen, std::string& strDescLocationUrl );
	bool				alternateParseDeviceFromXml( const char* pXml );

	bool				parseServiceXmlIntoService( const char* pXml );
	bool				createAddPortXml(	std::string& strAddPortXml, 
											const char* pIpAddr,  
											int iPort, 
											const char* pService  );
#endif // USE_TINYXML
	bool				requestDeviceInfo();
	bool				sendMapPortRequest( UpnpDevice * poUpnpDevice );
};
