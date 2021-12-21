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

#include "UpnpRootDevice.h"
#include "UpnpPortForward.h"
#include "UpnpTcpSkt.h"

#include <CoreLib/VxUrl.h>
#include <NetLib/VxParseUrlComponents.h>
#include <CoreLib/VxFileUtil.h>

#include <stdio.h>
#include <string.h>

#ifdef TARGET_OS_WINDOWS
	#define sprintf sprintf_s
	#define snprintf _snprintf_s
#endif // TARGET_OS_WINDOWS

namespace
{
	const char * SERVICE_IP_CONNECT		= "urn:schemas-upnp-org:service:WANIPConnection:1";
	const char * SERVICE_PPP_CONNECT	= "urn:schemas-upnp-org:service:WANPPPConnection:1";

	uint32_t UpnpRootThreadHandler( void * pvContext )
	{
		bool bResult = false;
		VxThread * poThread = (VxThread *)pvContext;
		poThread->setIsThreadRunning( true );
		UpnpRootDevice * poDevice = (UpnpRootDevice *)poThread->getThreadUserParam();
		if( poDevice )
		{
			bResult = poDevice->doRootDeviceQueryThreadFunction();
            poDevice->m_bDisabled = bResult;
        }

		poThread->threadAboutToExit();
		return 0;
	}

	void XmlCallback( void * pvUserData, EXmlParseType eXmlType, const char * pName, const char * pValue )
	{
		((UpnpRootDevice *)pvUserData)->xmlCallback( eXmlType, pName, pValue );
	}
}

//============================================================================
UpnpRootDevice::UpnpRootDevice( UpnpPortForward& client, std::string& strUrl )
: m_Client( client )
, m_strUrl( strUrl )
, m_bDisabled( false )
, m_stUpnpSkt( NULL )
{
}

//============================================================================
UpnpParser& UpnpRootDevice::getParser()
{
	return m_Parser;
}

//============================================================================
void UpnpRootDevice::startup()
{
	if( false == m_DeviceThread.isThreadRunning() )
	{
		m_DeviceThread.startThread( (VX_THREAD_FUNCTION_T)UpnpRootThreadHandler, this, "UpnpRootDevice" );
	}
}

//============================================================================
void UpnpRootDevice::preShutdown()
{
	m_DeviceThread.abortThreadRun( true );
	m_Mutex.lock( 2246 );
	if( m_stUpnpSkt )
	{
		m_stUpnpSkt->closeSkt( 898 );
	}
	
	m_Mutex.unlock();
}

//============================================================================
void UpnpRootDevice::shutdown()
{
	preShutdown();
	m_DeviceThread.killThread();
}

//============================================================================
void UpnpRootDevice::sendResult( bool bResult )
{
	m_Client.deviceAddPortMapSuccessFailCallback( bResult );
}

//============================================================================
bool UpnpRootDevice::doRootDeviceQueryThreadFunction( void )
{
	bool result = addPortMapToDevice();
	if( false == result )
	{
		LogMsg( LOG_ERROR, "UpnpRootDevice failed addPortMapToDevice %s port %d\n", m_strUrl.c_str(), m_Client.getPortToMap() );
	}
	else
	{
		LogMsg( LOG_DEBUG, "UpnpRootDevice success addPortMapToDevice %s port %d\n", m_strUrl.c_str(), m_Client.getPortToMap() );
	}
	
	if( m_DeviceThread.isAborted() )
	{
		return false;
	}

	sendResult( result );
	return result;
}

//============================================================================
bool UpnpRootDevice::addPortMapToDevice()
{
	VxUrl oUrl;
	oUrl.setUrl( m_strUrl.c_str() );

	UpnpTcpSkt oUpnpSkt;
	// get the root device xml
	LogMsg( LOG_INFO, "UpnpRootDevice requesting Device Info from %s\n", m_strUrl.c_str() );
	//m_Client.statusMsg( "UPNP", "requesting Device Info from %s\n", m_strUrl.c_str() );

	if( m_DeviceThread.isAborted() )
	{
		return false;
	}

	m_Mutex.lock();
	m_stUpnpSkt = &oUpnpSkt;
	m_Mutex.unlock();

	if( false == oUpnpSkt.requestFile( oUrl, m_Parser ) )
	{
		m_Mutex.lock();
		m_stUpnpSkt = NULL;
		oUpnpSkt.closeSkt( 86544 );
		m_Mutex.unlock();
		if( m_DeviceThread.isAborted() )
		{
			return false;
		}
		//m_Client.statusMsg( "UPNP", "FAILED get device Info\n" );
		return false;
	}

	m_Mutex.lock();
	oUpnpSkt.closeSkt( 86543 );
	m_stUpnpSkt = NULL;
	m_Mutex.unlock();

	if( m_DeviceThread.isAborted() )
	{
		return false;
	}

	VxBuffer& oContentsBuffer = m_Parser.getContentsBuffer();				
	if( 0 == oContentsBuffer.length() )
	{
		return false;
	}

#if 0 // print contents to file for inspection
	const char * pPath = oUrl.getPath();
	const char * pFileName = strrchr( pPath, '/' );
	if( pFileName )
	{
		std::string strFileName = "F:/";
		strFileName += pFileName;
		VxFileUtil::writeWholeFile( strFileName.c_str(), (void *)oContentsBuffer.getDataPtr(), oContentsBuffer.length() );
	}
#endif 

	if( strstr( oContentsBuffer.getDataPtr(), SERVICE_IP_CONNECT ) )
	{
		m_strServiceType = SERVICE_IP_CONNECT;
	}
	else if( strstr( oContentsBuffer.getDataPtr(), SERVICE_PPP_CONNECT ) )
	{
		m_strServiceType = SERVICE_PPP_CONNECT;
	}
	else
	{
		//m_Client.statusMsg( "UPNP", "%s does not contain WANIPConnection\n", m_strUrl.c_str() );
		return false;
	}

	if( m_DeviceThread.isAborted() )
	{
		return false;
	}

	m_aoXmlTags.clear();
	m_bAmParsingService = false;
	char * pXmlData = oContentsBuffer.getDataPtr();

	VxXmlParse( this, pXmlData, &pXmlData[ oContentsBuffer.length() - 1 ], XmlCallback );

	if( m_DeviceThread.isAborted() )
	{
		return false;
	}

	if( m_strControlUrl.empty() )
	{
		return false;
	}

	if( ( false == m_strBaseUrl.empty() ) && 
		m_strControlUrl.substr(0, 7) != "http://" )
	{
		// combine url base and control url into complete path 
		if( ( m_strBaseUrl[ m_strBaseUrl.size() - 1 ] == '/' ) && 
			( false == m_strControlUrl.empty() ) &&
			( m_strControlUrl[0] == '/' ) )
		{
			m_strBaseUrl.erase( m_strBaseUrl.end() - 1 );
		}
		m_strControlUrl = m_strBaseUrl + m_strControlUrl;
	}
	else if( m_strControlUrl.substr(0, 7) != "http://" )
	{
		// generate full url from known host
		std::string strFullUrl = "http://";
		strFullUrl += oUrl.getHost();
		strFullUrl += ":";
		char as8Buf[20];
		sprintf( as8Buf, "%d", oUrl.getPort() );
		strFullUrl += as8Buf;
		m_strControlUrl = strFullUrl + m_strControlUrl;
	}

	if( m_DeviceThread.isAborted() )
	{
		return false;
	}

	return requestAddPortMap( m_Client.getLocalIpAddress(), m_Client.getPortToMap(), "TCP" );
}

//============================================================================
bool UpnpRootDevice::requestAddPortMap( InetAddress& lclAddr, int iPortToMap, const char * pProtocol )
{
	if( m_DeviceThread.isAborted() )
	{
		return false;
	}

	LogMsg( LOG_INFO, "UPNP: Requesting Add Port Map %d", iPortToMap );
	std::string strLclIp = lclAddr.toStdString();
	// in newer Century link routers anything besides 0 lease duration causes http error 500
	int leaseDuration = 0; // was 0 in utorrent //1440;
	//int leaseDuration = 3700; // was 0 in utorrent //1440;
	std::string strSoapAction = "AddPortMapping";
	std::string strPortMapDescription = m_Client.getNameOfServiceToForward();
	strPortMapDescription +=" (";
	strPortMapDescription += pProtocol;
	strPortMapDescription += ")";

	char soapContent[2048];
	snprintf(soapContent, sizeof(soapContent), "<?xml version=\"1.0\"?>\r\n"
		"<SOAP-ENV:Envelope xmlns:SOAP-ENV=\"http://schemas.xmlsoap.org/soap/envelope/\" "
		"SOAP-ENV:encodingStyle=\"http://schemas.xmlsoap.org/soap/encoding/\">"
		"<SOAP-ENV:Body><m:%s xmlns:m=\"%s\">"
		"<NewRemoteHost xmlns:dt=\"urn:schemas-microsoft-com:datatypes\" dt:dt=\"string\"></NewRemoteHost>"
		"<NewExternalPort xmlns:dt=\"urn:schemas-microsoft-com:datatypes\" dt:dt=\"ui2\">%u</NewExternalPort>"
		"<NewProtocol xmlns:dt=\"urn:schemas-microsoft-com:datatypes\" dt:dt=\"string\">%s</NewProtocol>"
		"<NewInternalPort xmlns:dt=\"urn:schemas-microsoft-com:datatypes\" dt:dt=\"ui2\">%u</NewInternalPort>"
		"<NewInternalClient xmlns:dt=\"urn:schemas-microsoft-com:datatypes\" dt:dt=\"string\">%s</NewInternalClient>"
		"<NewEnabled xmlns:dt=\"urn:schemas-microsoft-com:datatypes\" dt:dt=\"boolean\">1</NewEnabled>"
		"<NewPortMappingDescription xmlns:dt=\"urn:schemas-microsoft-com:datatypes\" dt:dt=\"string\">%s</NewPortMappingDescription>"
		"<NewLeaseDuration xmlns:dt=\"urn:schemas-microsoft-com:datatypes\" dt:dt=\"ui4\">%u</NewLeaseDuration>"
		"</m:%s></SOAP-ENV:Body></SOAP-ENV:Envelope>\r\n"
		, strSoapAction.c_str()
		, m_strServiceType.c_str()
		, iPortToMap // external port
		, pProtocol
		, iPortToMap // internal port
		, strLclIp.c_str()
		, strPortMapDescription.c_str() // PortMappingDescription
		, leaseDuration
		, strSoapAction.c_str() );

	int iSoapContentLen = strlen( soapContent );

#if 0 // print contents to file for inspection
#ifdef TARGET_OS_WINDOWS
	std::string strFileNameSoapContent = "F:/AddPortSoapContent.txt";
	VxFileUtil::writeWholeFile( strFileNameSoapContent.c_str(), (void *)soapContent, strlen(soapContent) );
#endif // TARGET_OS_WINDOWS
#endif 

	if( m_DeviceThread.isAborted() )
	{
		return false;
	}

	std::string strHostName;	// host name
	std::string strAuth;		// user:pass
	std::string strProtocol;	// http or https
	int			iHostPort;		// port 
	std::string strPath;

	bool bResult = VxParseUrlComponents(	m_strControlUrl,	// url to parse 
											strHostName,		// host name
											strAuth,			// user:pass
											strProtocol,		// http or https
											iHostPort,			// port 
											strPath
											);
	if( false == bResult )
	{
		LogMsg( LOG_ERROR, "UpnpRootDevice::requestAddPortMap could not parse url ^s\n", m_strControlUrl.c_str() );
		return false;
	}
	
	char header[2048];
	snprintf(header, sizeof(header), "POST %s HTTP/1.1\r\n"
		"Cache-Control: no-cache\r\n"
		"Connection: Close\r\n"
		"Pragma: no-cache\r\n"
		"Content-Type: text/xml; charset=\"utf-8\"\r\n"
		"User-Agent: Microsoft-Windows/6.2 UPnP/1.0\r\n"
		"SOAPAction: \"%s#%s\"\r\n"
		"Content-Length: %d\r\n"
		"Host: %s:%u\r\n\r\n"
		"%s"
		, strPath.c_str()
		, m_strServiceType.c_str()
		, strSoapAction.c_str()
		, iSoapContentLen
		, strHostName.c_str()
		, iHostPort
		, soapContent );

	if( m_DeviceThread.isAborted() )
	{
		return false;
	}

#if 1 // print contents to file for inspection
#ifdef TARGET_OS_WINDOWS
	std::string strFileName = "F:/AddPortPost.txt";
	VxFileUtil::writeWholeFile( strFileName.c_str(), (void *)header, strlen(header) );
#endif // TARGET_OS_WINDOWS
#endif 

	UpnpTcpSkt oSkt;
	m_Mutex.lock();
	m_stUpnpSkt = &oSkt;
	m_Mutex.unlock();
	LogMsg( LOG_INFO, "Requesting UPNP from %s \n", strHostName.c_str() );

	if( oSkt.postRequest( strHostName, iHostPort, header, strlen(header) ) )
	{
		if( m_DeviceThread.isAborted() )
		{
			m_Mutex.lock();
			m_stUpnpSkt = NULL;
			m_Mutex.unlock();
			return false;
		}

		VxHttpParse oParser;
		int httpErrCode = -1;
		if( oSkt.getHttpHeader( oParser ) )
		{
			httpErrCode = oParser.getHttpCodeNumber();
			if( 200 == httpErrCode )
			{
				bResult = true;
				LogMsg( LOG_DEBUG, "UpnpRootDevice::requestAddPortMap success protocol %s port %d to \n", pProtocol, iPortToMap );
			}
			else
			{
				LogMsg( LOG_ERROR, "UpnpRootDevice::requestAddPortMap returned error %d\n", httpErrCode );
				//			.p<?xml version="1.0"?>..<SOAP-ENV:Envelope xmlns:SOAP-ENV="http://schemas.xmlsoap.org/soap/envelope/" SOAP-ENV:encodingStyle="h
				//ttp://schemas.xmlsoap.org/soap/encoding/"><SOAP-ENV:Body><m:AddPortMapping xmlns:m="urn:schemas-upnp-org:service
				//				:WANIPConnection:1">
				//<NewRemoteHost xmlns:dt="urn:schemas-microsoft-com:datatypes" dt:dt="string"></NewRemoteHost>
				//<NewExternalPort xmlns:dt="urn:schemas-microsoft-com:datatypes" dt:dt="ui2">11201</NewExternalPort><NewProtocol
				//xmlns:dt="urn:schemas-microsoft-com:datatypes" dt:dt="string">TCP</NewProtocol><NewInternalPort xmlns:dt="urn:schemas-microsoft
				//	  -com:datatypes" dt:dt="ui2">11201</NewInternalPort>
				//<NewInternalClient xmlns:dt="urn:schemas-microsoft-com:datatypes" dt:dt="string">192.168.0.5</NewInternalClient>
				//<NewEnabled xmlns:dt="urn:schemas-microsoft-com:datatypes" dt:dt="boolean">1</NewEnabled>
				//<NewPortMappingDescription xmlns:dt="urn:schemas-microsoft-com:datatypes" dt:dt="string">uTorrent (TCP)</NewPortMappingDescription>
				//<NewLeaseDuration xmlns:dt="urn:schemas-microsoft-com:datatypes" dt:dt="ui4">0</NewLeaseDuration>
				//</m:AddPortMapping></SOAP-ENV:Body></SOAP-ENV:Envelope>..    

//				.pPOST /uuid:04bf6de5-b3e2-e2b3-e56d-bf04bfe5e202/WANIPConn1 HTTP/1.1..Cache-Control: no-cache..
//Connection: Close..Pragma: no-cache..Content-Type: text/xml; charset="utf-8"..
//			User-Agent: Microsoft-Windows/10.0 UPnP/1.0..
//SOAPAction: "urn:schemas-upnp-org:service:WANIPConnection:1#AddPortMapping"..Content-Length: 1127..Host: 192.168.0.1:5431....
//
			}
		}
		else
		{
			LogMsg( LOG_ERROR, "UpnpRootDevice::requestAddPortMap failed get response header\n" );
		}
	}
	else
	{
		LogMsg( LOG_ERROR, "UpnpRootDevice::requestAddPortMap failed post request\n" );
	}

	m_Mutex.lock();
	m_stUpnpSkt = NULL;
	oSkt.closeSkt( 343 );
	m_Mutex.unlock();

	if( m_DeviceThread.isAborted() )
	{
		return false;
	}

	return bResult;
}

//============================================================================
bool UpnpRootDevice::currentXmlTags( const char* str1, const char* str2 )
{
	std::vector<std::string>::reverse_iterator i = m_aoXmlTags.rbegin();
	if (i == m_aoXmlTags.rend()) return false;
	if (!XmlStringNoCaseEqual(i->c_str(), str2)) return false;
	++i;
	if (i == m_aoXmlTags.rend()) return false;
	if (!XmlStringNoCaseEqual(i->c_str(), str1)) return false;
	return true;
}

//============================================================================
void UpnpRootDevice::xmlCallback( EXmlParseType eXmlType, const char * pName, const char * pValue )
{
	if( eXmlStartTag == eXmlType )
	{
		std::string tag;
		XmlCopyToLower( tag, pName );
		m_aoXmlTags.push_back(tag);
	}
	else if( eXmlEndTag == eXmlType )
	{
		if(false == m_aoXmlTags.empty() )
		{
			if( m_bAmParsingService 
				&& ( m_aoXmlTags.back() == "service" ) )
			{
				m_bAmParsingService = false;
			}

			m_aoXmlTags.pop_back();
		}
	}
	else if( eXmlStringTag == eXmlType )
	{
		if( m_aoXmlTags.empty() )
		{
			return;
		}

		if( !m_bAmParsingService 
			&& currentXmlTags( "service", "servicetype" ) )
		{
			if( XmlStringNoCaseEqual( pName, m_strServiceType.c_str() ) )
			{
				m_bAmParsingService = true;
			}
		}
		else if( m_strControlUrl.empty() 
			&& m_bAmParsingService 
			&& currentXmlTags( "service", "controlurl" ))
		{
			m_strControlUrl = pName;
		}
		else if( m_strModel.empty() 
			&& currentXmlTags("device", "modelname"))
		{
			m_strModel = pName;
		}
		else if (m_aoXmlTags.back() == "urlbase")
		{
			m_strBaseUrl = pName;
		}
	}
}


/*
+		pXmlData	0x0488a4d8 "<?xml version="1.0"?>
<root xmlns="urn:schemas-upnp-org:device-1-0">
	<specVersion>
	<major>1</major>
	<minor>0</minor>
	</specVersion>
	<URLBase>http://192.168.0.1:5431/</URLBase>
	<device>
	<deviceType>urn:schemas-upnp-org:device:InternetGatewayDevice:1</deviceType>
	<presentationURL>http://192.168.0.1:80/</presentationURL>
	<friendlyName>CenturyLink UPnP v1.0</friendlyName>
	<manufacturer>ZyXEL</manufacturer>..<manufacturerURL>http://www.zyxel.com/</manufacturerURL>
	<modelDescription>DSL Modem</modelDescription>..<modelName>C1100Z</modelName>
	<modelNumber>C1100Z</modelNumber>
	<modelURL>http://www.centurylink.com/</modelURL>..<serialNumber>C1100ZS150Y52062374</serialNumber>
	<UDN>uuid:04bf6de5-b3e2-e2b3-e56d-bf04bfe5e200</UDN>
	<serviceList>
		<service>
			<serviceType>urn:schemas-upnp-org:service:Layer3Forwarding:1</serviceType>
			<serviceId>urn:upnp-org:serviceId:L3Forwarding1</serviceId>
			<controlURL>/uuid:04bf6de5-b3e2-e2b3-e56d-bf04bfe5e200/L3Forwarding1</controlURL>..
			<eventSubURL>/uuid:04bf6de5-b3e2-e2b3-e56d-bf04bfe5e200/L3Forwarding1</eventSubURL>
			<SCPDURL>/dynsvc/L3Forwarding1.xml</SCPDURL>
		</service>
	</serviceList>
	<deviceList>
		<device>
			<deviceType>urn:schemas-upnp-org:device:WANDevice:1</deviceType>
			<friendlyName>WanDevice.1</friendlyName>
			<manufacturer>ZyXEL</manufacturer>..<manufacturerURL>http://www.zyxel.com/</manufacturerURL>
			<modelDescription>DSL Modem</modelDescription>
			<modelName>C1100Z</modelName>
			<modelNumber>C1100Z</modelNumber>
			<modelURL>http://www.centurylink.com/</modelURL>
			<serialNumber>C1100ZS150Y52062374</serialNumber>
			<UDN>uuid:04bf6de5-b3e2-e2b3-e56d-bf04bfe5e201</UDN>
		<serviceList>
			<service>
				<serviceType>urn:schemas-upnp-org:service:WANCommonInterfaceConfig:1</serviceType>
				<serviceId>urn:upnp-org:serviceId:WANCommonIFC1</serviceId>
				<controlURL>/uuid:04bf6de5-b3e2-e2b3-e56d-bf04bfe5e201/WANCommonIFC1</controlURL>
				<eventSubURL>/uuid:04bf6de5-b3e2-e2b3-e56d-bf04bfe5e201/WANCommonIFC1</eventSubURL>
				<SCPDURL>/dynsvc/WANCommonIFC1.xml</SCPDURL>
			</service>
		</serviceList>
	<deviceList>..
	<device>
		<deviceType>urn:schemas-upnp-org:device:WANConnectionDevice:1</deviceType>
		<friendlyName>WanConnectionDevice-1</friendlyName>
		<manufacturer>ZyXEL</manufacturer>
		<manufacturerURL>http://www.zyxel.com/</manufacturerURL>
		<modelDescription>DSL Modem</modelDescription>
		<modelName>C1100Z</modelName>
		<modelNumber>C1100Z</modelNumber>
		<modelURL>http://www.centurylink.com/</modelURL>
		<serialNumber>C1100ZS150Y52062374</serialNumber>
		<UDN>uuid:04bf6de5-b3e2-e2b3-e56d-bf04bfe5e202</UDN>
		<serviceList>
			<service>
				<serviceType>urn:schemas-upnp-org:service:WANIPConnection:1
				</serviceType>
				<serviceId>urn:upnp-org:serviceId:WANIPConn1</serviceId>
				<controlURL>/uuid:04bf6de5-b3e2-e2b3-e56d-bf04bfe5e202/WANIPConn1</controlURL>
				<eventSubURL>/uuid:04bf6de5-b3e2-e2b3-e56d-bf04bfe5e202/WANIPConn1</eventSubURL>
				<SCPDURL>/dynsvc/WANIPConn1.xml</SCPDURL>
			</service>
			<service>
				<serviceType>urn:schemas-upnp-org:service:WANDSLLinkConfig:1</serviceType>
				<serviceId>urn:upnp-org:serviceId:WANDSLLinkC1</serviceId>
				<controlURL>/uuid:04bf6de5-b3e2-e2b3-e56d-bf04bfe5e202/WANDSLLinkC1</controlURL>
				<eventSubURL>/uuid:04bf6de5-b3e2-e2b3-e56d-bf04bfe5e202/WANDSLLinkC1</eventSubURL>
				<SCPDURL>/dynsvc/WANDSLLinkC1.xml</SCPDURL>
			</service>
		</serviceList>
		</device>
	</deviceList>
	</device>..</deviceList>
	</device>
</root>
	   */
