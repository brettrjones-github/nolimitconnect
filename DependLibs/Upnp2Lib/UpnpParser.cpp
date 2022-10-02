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
#include "UpnpAction.h"

#include <CoreLib/VxParse.h>
#include <CoreLib/VxDebug.h>
#include <NetLib/VxSktBase.h>

#include <string.h>
#include <stdlib.h>
#include <time.h>

namespace
{
	const char* UPNP_ST = "ST";
	const char* UPNP_NT = "NT";
	const char* UPNP_USN = "USN";

	const char* UPNP_NTS = "NTS";
//	const char* UPNP_MX = "MX";
	const char* UPNP_MAN = "MAN";
    /*
//	const char* UPNP_EXT = "EXT";
//	const char* UPNP_SID = "SID";
//	const char* UPNP_SEQ = "SEQ";
//	const char* UPNP_CALLBACK = "CALLBACK";
//	const char* UPNP_TIMEOUT = "TIMEOUT";

//	const char* SOAP_HEADER = "<?xml version=\"1.0\"?>\r\n\
//<SOAP-ENV:Envelope xmlns:SOAP-ENV=\"http://schemas.xmlsoap.org/soap/envelope/\" SOAP-\
//ENV:encodingStyle=\"http://schemas.xmlsoap.org/soap/encoding/\"><SOAP-ENV:Body>";
//	const char* SOAP_TRAILER = "</SOAP-ENV:Body></SOAP-ENV:Envelope>\r\n";
//	const char* SOAP_DATA_SCHEMA = " xmlns:dt=\"urn:schemas-microsoft-com:datatypes\" dt:dt=\"";
//	const char* SOAP_MEMBER_START =  " xmlns:m=\"";
//	const char* SOAP_QUOTE_END =  "\">";
//	const char* QUOTE =  "\"";
*/
}

namespace UpnpNT 
{
	const char* UPNP_ROOTDEVICE = "upnp:rootdevice";
	const char* UPNP_EVENT = "upnp:event";
	bool isRootDevice( const char*usnValue )
	{
		return (0 == strcmp( usnValue,  UPNP_ROOTDEVICE ) );
	}
}

namespace UpnpST 
{
	const char UPNP_ROOT_DEVICE[] = "upnp:rootdevice";

    // const char UPNP_ALL_DEVICE[] = "ssdp:all";
    // const char UPNP_UUID_DEVICE[] = "uuid";
    // const char UPNP_URN_DEVICE[] = "urn:schemas-upnp-org:device:";
    // const char UPNP_URN_SERVICE[] = "urn:schemas-upnp-org:service:";

	bool isRootDevice( const char*value )
	{
		if( NULL == value )
		{
			return false;
		}
		if( 0 == strcmp( value, UPNP_ROOT_DEVICE ) )
		{
			return true;
		}
		std::string strWithQuotes = "\"";
		strWithQuotes.append( UPNP_ROOT_DEVICE );
		strWithQuotes.append("\"");
		return ( 0 == strcmp( value, strWithQuotes.c_str() ) );
	}
	bool isAllDevice( const char*value );
	bool isRootDevice( const char*value );
	bool isUUIDDevice( const char*value );
	bool isURNDevice( const char*value );
	bool isURNService( const char*value );
}

namespace UpnpUSN 
{
	const char UPNP_ROOTDEVICE[] = "upnp:rootdevice";

	bool isRootDevice( const char*usnValue )
	{
		if( NULL == usnValue )
		{
			return false;
		}
		std::string usnStr = usnValue;
		return StdStringEndsWith( usnStr, UPNP_ROOTDEVICE );
	}
	const char* getUDN( const char* pUsnValue, std::string& strRetUDN )
	{
		if( NULL == pUsnValue )
		{
			return "";
		}
		std::string strUSN = pUsnValue;
		std::string::size_type loc = strUSN.find( "::", 0 ); 
		if( loc != std::string::npos ) 
		{ 
			strRetUDN = strUSN.substr( 0, loc ); 
		} 
		else 
		{
			strRetUDN = strUSN;
		} 

		StdStringTrim( strRetUDN );
		return strRetUDN.c_str();
	}
}

namespace UpnpNTS 
{
	const char* UPNP_ALIVE = "ssdp:alive";
	const char* UPNP_BYEBYE = "ssdp:byebye";
	const char* UPNP_PROPCHANGE = "upnp:propchange";

	bool isAlive( const char*value )
	{
		if( NULL == value )
		{
			return false;
		}
		std::string strValue = value;
		return StdStringBeginsWith( strValue, UPNP_ALIVE );
	}

	bool isByeBye( const char*value )
	{
		if( NULL == value )
		{
			return false;
		}
		std::string strValue = value;
		return StdStringBeginsWith( strValue, UPNP_BYEBYE );
	}
}

namespace UpnpMAN 
{
	const char* UPNP_DISCOVER = "ssdp:discover";

	bool isDiscover( const char* value )
	{
		if( NULL == value )
		{
			return false;
		}
		if( 0 == strcmp( value, UPNP_DISCOVER ) )
		{
			return true;
		}
		std::string strWithQuotes = "\"";
		strWithQuotes.append( UPNP_DISCOVER );
		strWithQuotes.append("\"");
		return ( 0 == strcmp( value, strWithQuotes.c_str() ) );
	}
}

namespace UpnpSSPD
{
	int getLeaseTime( const char* cacheCnt )
	{
		const char* pEqualPos = strchr( cacheCnt, '=' );
		if( NULL == pEqualPos )
		{
			return 0;
		}
		return atoi((pEqualPos + 1));
	}
}


//============================================================================
UpnpParser::UpnpParser()
: m_strLclIp( "" )
, m_strRmtIp( "" )
, m_u16RmtPort( 0 )
, m_u32TimeStamp( 0 )
{
}

//============================================================================
void UpnpParser::setParserData(  VxSktBase* sktBase, const char* pDataBuf )
{
	if( pDataBuf )
	{
		setStreamData( pDataBuf );
	}
	else
	{
		setStreamData( (const char*)sktBase->getSktReadBuf() );
	}

	setTimeStamp( (uint32_t) time( NULL ) );
	setLclIp( sktBase->m_strLclIp );
	setRmtIpAndPort( sktBase->m_strRmtIp, sktBase->m_RmtIp.m_u16Port );
}

//============================================================================
void UpnpParser::setLclIp( std::string& strIp )
{
	m_strLclIp = strIp;
}

//============================================================================
void UpnpParser::setRmtIpAndPort( std::string& strIp, unsigned short u16Port )
{
	m_strRmtIp = strIp;
	m_u16RmtPort = u16Port;
}

//============================================================================
void UpnpParser::setTimeStamp( uint32_t time )
{
	m_u32TimeStamp = time;
}

//============================================================================
uint32_t UpnpParser::getTimeStamp()
{
	return m_u32TimeStamp;
}

//============================================================================
bool UpnpParser::isRootDevice()
{
	std::string buf;
	if( true == UpnpNT::isRootDevice( getNT( buf ) ) )
	{
		return true;
	}
	if( true == UpnpST::isRootDevice( getST( buf ) ) )
	{
		return true;
	}
	return UpnpUSN::isRootDevice( getUSN( buf ) );
}

//============================================================================
bool UpnpParser::isDiscover( void )
{
	std::string buf;
	return UpnpMAN::isDiscover( getMAN( buf ) );
}

//============================================================================
bool UpnpParser::isAlive( void )
{
	std::string buf;
	return UpnpNTS::isAlive(getNTS(buf));
}

//============================================================================
bool UpnpParser::isByeBye( void )
{
	std::string buf;
	return UpnpNTS::isByeBye(getNTS(buf));
}

//============================================================================
const char*  UpnpParser::getST( std::string& value )
{
	return getHttpValue( UPNP_ST, value );
}

//============================================================================
const char*  UpnpParser::getNT( std::string& value )
{
	return getHttpValue( UPNP_NT, value );
}

//============================================================================
const char*  UpnpParser::getNTS( std::string& value )
{
	return getHttpValue( UPNP_NTS, value );
}

//============================================================================
const char* UpnpParser::getUSN( std::string& value )
{
	return getHttpValue( UPNP_USN, value );
}

//============================================================================
const char* UpnpParser::getUDN( std::string& strRetValue )
{
	std::string strUSN;
	return UpnpUSN::getUDN( getUSN( strUSN ), strRetValue );
}

//============================================================================
const char*  UpnpParser::getMAN( std::string& value )
{
	return getHttpValue( UPNP_MAN, value );
}

//============================================================================
int UpnpParser::getLeaseTime( void )
{
	std::string buf;
	return UpnpSSPD::getLeaseTime( getCacheControl( buf ) );
}

#ifdef USE_TINYXML
//============================================================================
void UpnpParser::findAllElements( tinyxml2::XMLElement* pRootElem, std::vector< tinyxml2::XMLElement* >& elementList, const char* elemName )
{
	for( tinyxml2::XMLElement* pElem = pRootElem->FirstChildElement(); pElem; pElem = pElem->NextSiblingElement() )
	{
		if( 0 == strcmp( elemName,  pElem->Value() ) )
		{
			elementList.push_back( pElem );
		}
		findAllElements( pElem, elementList, elemName );
	}
}

//============================================================================
bool UpnpParser::parseRootXmlIntoDevice( const char* pXmlDoc, UpnpDevice& device, std::string& strDescLocationUrl )
{
	//dumpXml( pXmlDoc );
	if( 0 != m_XmlDoc.Parse( pXmlDoc ) )
	{
		LogMsg( LOG_DEBUG, "Could not Parse xml using tinyxml\n" );
		return false;
	}
	tinyxml2::XMLElement* poRootElem = m_XmlDoc.FirstChildElement("root");
	if( NULL == poRootElem )
	{
		LogMsg( LOG_DEBUG, "Could not find root element\n" );
		return false;
	}

	if( false == parseBaseUrl( device, poRootElem ) )
	{
		// use the location as the base url
		std::string::size_type index = strDescLocationUrl.find( "/", 7 );
		if( index == std::string::npos )
		{
			LogMsg( LOG_DEBUG, "Could not find base url from location\n" );
			return false;
		}
		device.m_strBaseURL.assign( strDescLocationUrl, 0, index );
	}

	tinyxml2::XMLElement* poRootDeviceElem = poRootElem->FirstChildElement("device");
	if( NULL == poRootDeviceElem )
	{
		LogMsg( LOG_DEBUG, "Could not find root device\n" );
		return false;
	}

	if( false == parseRootDevice( device, poRootDeviceElem ) )
	{
		return false;
	}

	tinyxml2::XMLElement* poIPConnServiceElem;
	if( false == UpnpParser::findElementWithChildElementText(	poRootDeviceElem,			// element to start search from
																& poIPConnServiceElem,		// return element found
																"service",					// element to search for
																"serviceType",				// element found must have this child element
																"WANIPConnection"  ) )		// child element must have text containing this text string
	{
		return false;
	}

	if( false == parseIPConnectService( device, poIPConnServiceElem ) )
	{
		return false;
	}

	return true;
}

//============================================================================
bool UpnpParser::parseIPConnectService( UpnpDevice& device, tinyxml2::XMLElement* poServiceElem )
{
	UpnpService& ipService = device.getIpConnService();

	if( false == getChildElementText( poServiceElem, "serviceType", ipService.m_strServiceType ) )
	{
		return false;
	}
	if( false == getChildElementText( poServiceElem, "serviceId", ipService.m_strServiceId ) )
	{
		return false;
	}
	if( false == getChildElementText( poServiceElem, "SCPDURL", ipService.m_strSCPDURL ) )
	{
		return false;
	}
	if( false == getChildElementText( poServiceElem, "controlURL", ipService.m_strControlURL ) )
	{
		return false;
	}	
	getChildElementText( poServiceElem, "eventSubURL", ipService.m_strEventSubURL );
	return true;
}

//============================================================================
bool UpnpParser::findElementWithChildElementText(	tinyxml2::XMLElement* poRootElem,		// element to start search from
													tinyxml2::XMLElement** ppoRetFoundElem, // return element found
													const char* pElemName,					// element to search for
													const char* pSubElemName,				// element found must have this child element
													const char* pSubElemTextSubStr  )		// child element must have text containing this text string
{
	std::vector< tinyxml2::XMLElement* > elementList;
	findAllElements( poRootElem, elementList, pElemName );

	std::vector< tinyxml2::XMLElement* >::iterator iter;
	for( iter = elementList.begin(); iter != elementList.end(); ++iter )
	{
		tinyxml2::XMLElement* poFoundElem = *iter;
		tinyxml2::XMLElement* poChildElem = poFoundElem->FirstChildElement(pSubElemName);
		if( poChildElem->GetText() )
		{
			if( strstr( poChildElem->GetText(), pSubElemTextSubStr ) )
			{
				*ppoRetFoundElem = poFoundElem;
				return true;
			}
		}
	}
	return false;
}


//============================================================================
bool UpnpParser::parseBaseUrl( UpnpDevice& device, tinyxml2::XMLElement* poRootElem )
{
	if( false == getChildElementText( poRootElem, "URLBase", device.m_strBaseURL ) )
	{		
		return false;
	}
	return true;
}

//============================================================================
bool UpnpParser::parseRootDevice( UpnpDevice& device, tinyxml2::XMLElement* poDeviceElem )
{
	if( false == getChildElementText( poDeviceElem, "friendlyName", device.m_strFriendlyName ) )
	{		
		return false;
	}
	if( false == getChildElementText( poDeviceElem, "deviceType", device.m_strDeviceType ) )
	{		
		return false;
	}
	if( false == getChildElementText( poDeviceElem, "UDN", device.m_strUDN ) )
	{		
		return false;
	}
	return true;
}

//============================================================================
bool UpnpParser::getChildElementText( tinyxml2::XMLElement* poElem, const char* pElemName, std::string& retElemValue )
{
	retElemValue = "";
	tinyxml2::XMLElement* pElem = poElem->FirstChildElement(pElemName);
	if( pElem )
	{
		if( pElem->GetText() )
		{
			retElemValue = pElem->GetText();
			if( retElemValue.length() )
			{
				return true;
			}
			else
			{
				LogMsg( LOG_DEBUG, "XML Child element %s has no string in text\n", pElemName );
			}		
		}
		else
		{
			LogMsg( LOG_DEBUG, "XML Child element %s has no text\n", pElemName );
		}
	}
	else
	{
		LogMsg( LOG_DEBUG, "XML Could not find child element %s\n", pElemName );
	}
	return false;
}


//============================================================================
bool UpnpParser::dumpXml( const char* pXmlDoc )
{
	bool bResult = false;
	if( 0 != m_XmlDoc.Parse( pXmlDoc ) )
	{
		return false;
	}
	int indentLevel = 0;
	for ( tinyxml2::XMLElement* pElem = m_XmlDoc.FirstChildElement(); pElem; pElem = m_XmlDoc.NextSiblingElement() )
	{
		dumpXmlElement( indentLevel, pElem );
	}
	return bResult;
}

//============================================================================
bool UpnpParser::dumpXmlElement( int indentLevel, tinyxml2::XMLElement* pElem )
{
	std::string strIndent;
	for( int i = 0; i < indentLevel; ++i )
	{
		strIndent += " ";
	}
	std::string strAttrName;
	const char* pAttr = pElem->GetText();
	if( pAttr )
	{
		strAttrName = pAttr;
	}
	std::string strAttrValue;
	const char* pAttrValue = pElem->Attribute("value");
	if( pAttrValue )
	{
		strAttrValue = pAttrValue;
	}
	//LogMsg( LOG_DEBUG, "%d%s %s att %s : %s\n", indentLevel, strIndent.c_str(), pElem->Value(), strAttrName.c_str(), strAttrValue.c_str() );
	//for ( tinyxml2::XMLElement* pElemNext = pElem->FirstChildElement(); pElemNext; pElemNext = pElemNext->NextSiblingElement() )
	//{
	//	dumpXmlElement( indentLevel + 1, pElemNext );
	//}

	return true;
}

//============================================================================
bool UpnpParser::parseServiceXmlIntoService( const char* pXmlDoc, UpnpService& service )
{
	//dumpXml( pXmlDoc );

	if( 0 != m_XmlDoc.Parse( pXmlDoc ) )
	{
		LogMsg( LOG_DEBUG, "Could not Parse xml %s\n", pXmlDoc );
		return false;
	}
	tinyxml2::XMLElement* poRootElem = m_XmlDoc.FirstChildElement("scpd");
	if( NULL == poRootElem )
	{
		LogMsg( LOG_DEBUG, "Could not find scpd element\n" );
		return false;
	}

	tinyxml2::XMLElement* poActionListElem = poRootElem->FirstChildElement("actionList");
	if( NULL == poActionListElem )
	{
		LogMsg( LOG_DEBUG, "Could not find actionList element\n" );
		return false;
	}
	if( false == parseActionList( service, poActionListElem ) )
	{
		return false;
	}
	tinyxml2::XMLElement* poStateTableElem = poRootElem->FirstChildElement("serviceStateTable");
	if( NULL == poStateTableElem )
	{
		LogMsg( LOG_DEBUG, "Could not find serviceStateTable element\n" );
		return false;
	}
	if( false == parseStateVars( service, poStateTableElem ) )
	{
		return false;
	}

	return true;
}

//============================================================================
bool UpnpParser::parseActionList( UpnpService& service, tinyxml2::XMLElement* poActionListElem )
{
	bool foundActions = false;
	for( tinyxml2::XMLElement* pActionElem = poActionListElem->FirstChildElement("action"); pActionElem; pActionElem = pActionElem->NextSiblingElement() )
	{
		UpnpAction action;
		if( false == getChildElementText( pActionElem, "name", action.m_strActionName ) )
		{		
			continue;
		}
		tinyxml2::XMLElement* pArgListElem = pActionElem->FirstChildElement("argumentList");
		if( NULL == pArgListElem )
		{
			continue;
		}
		for( tinyxml2::XMLElement* pArgElem = pArgListElem->FirstChildElement("argument"); pArgElem; pArgElem = pArgElem->NextSiblingElement("argument") )
		{
			UpnpArg arg;
			if( false == getChildElementText( pArgElem, "name", arg.m_strArgName ) )
			{		
				continue;
			}
			getChildElementText( pArgElem, "direction", arg.m_strArgDirection );
			getChildElementText( pArgElem, "relatedStateVariable", arg.m_strRelatedStateVar );
			action.m_aoArgList.push_back( arg );
		}
		service.m_aoActions.push_back( action );
		foundActions = true;
	}
	return foundActions;
}

//============================================================================
bool UpnpParser::parseStateVars( UpnpService& service, tinyxml2::XMLElement* poStateTableElem )
{
	for( tinyxml2::XMLElement* pStateElem = poStateTableElem->FirstChildElement("stateVariable"); pStateElem; pStateElem = pStateElem->NextSiblingElement("stateVariable") )
	{
		UpnpStateVariable stateVar;
		const char* pElemText = pStateElem->GetText();
		if( pElemText )
		{
			std::string strSendEvents = pElemText;
			if( strSendEvents.length() )
			{
				StdStringTrim( strSendEvents );
				if( strcmp( strSendEvents.c_str(), "sendEvents=\"yes\""))
				{
					stateVar.m_bSendEvents = true;
				}
			}
		}
		if( false == getChildElementText( pStateElem, "name", stateVar.m_strVarName ) )
		{		
			continue;
		}
		if( false == getChildElementText( pStateElem, "dataType", stateVar.m_strDataType ) )
		{		
			continue;
		}
		parseAllowedVars( stateVar, pStateElem );
		service.m_aoVariables.push_back( stateVar );
	}
	return service.m_aoVariables.size() ? true : false;
}

//============================================================================
void UpnpParser::parseAllowedVars( UpnpStateVariable& stateVar, tinyxml2::XMLElement* poStateElem )
{
	tinyxml2::XMLElement* poListElem = poStateElem->FirstChildElement("allowedValueList");
	if( NULL == poListElem )
	{
		return;
	}
	for( tinyxml2::XMLElement* poAllowedVarElem = poListElem->FirstChildElement("allowedValue"); poAllowedVarElem; poAllowedVarElem = poAllowedVarElem->NextSiblingElement("allowedValue") )
	{
		const char* pValueText = poAllowedVarElem->GetText();
		if( pValueText )
		{
			std::string strAllowedValue = pValueText;
			if( strAllowedValue.length() )
			{
				StdStringTrim( strAllowedValue );
				stateVar.m_aoAllowedVars.push_back( strAllowedValue );
			}
		}
	}
}

//============================================================================
bool UpnpParser::createAddPortXml(	UpnpService& service, 
									std::string& retXml,
									const char* pLclIp, 
									uint16_t u16Port, 
									const char* portServiceName )
{
	retXml = SOAP_HEADER;
	UpnpAction addPortAction;
	if( false == findAction( service, "AddPort", addPortAction ) )
	{
		return false;
	}
	if( 4 > addPortAction.m_aoArgList.size() )
	{
		//not enough arguments
		return false;
	}
	retXml += "<m:";
	retXml += addPortAction.m_strActionName;
	retXml += SOAP_MEMBER_START;
	retXml += service.m_strServiceType;
	retXml += "\">";

	UpnpStateVariable var;
	std::vector<UpnpArg>::iterator argIter;

	for( argIter = addPortAction.m_aoArgList.begin(); argIter !=  addPortAction.m_aoArgList.end(); ++argIter )
	{
		UpnpArg& upnpArg = (*argIter);
		if( false == findStateVar( service.m_aoVariables, upnpArg.m_strRelatedStateVar.c_str(), var ) )
		{
			return false;
		}
		if( false == addPortArgParam( retXml, pLclIp, u16Port, portServiceName, upnpArg, var ) )
		{
			return false;
		}
	}

	retXml += "</m:";
	retXml += addPortAction.m_strActionName;
	retXml += ">";
	retXml += SOAP_TRAILER;

	return  true;
}

//============================================================================
bool UpnpParser::addPortArgParam(	std::string&		retXml, 
									const char*		pLclIp, 
									uint16_t					u16Port, 
									const char*		portServiceName,
									UpnpArg&			upnpArg, 
									UpnpStateVariable&	var )
{
	std::string strParamValue;

	if( strstr( var.m_strVarName.c_str(), "RemoteHost") )
	{
		addSoapParam( retXml, upnpArg.m_strArgName, var, "" );
	}
	else if( strstr( var.m_strVarName.c_str(), "ExternalPort" ) ||
			 strstr( var.m_strVarName.c_str(), "InternalPort" )  )
	{
		StdStringFromInteger( strParamValue, u16Port );
		addSoapParam( retXml, upnpArg.m_strArgName, var, strParamValue.c_str() );
	}
	else if( strstr( var.m_strVarName.c_str(), "Protocol" ) )
	{
		addSoapParam( retXml, upnpArg.m_strArgName, var, "TCP" );
	}
	else if( strstr( var.m_strVarName.c_str(), "InternalClient" ) )
	{
		addSoapParam( retXml, upnpArg.m_strArgName, var, pLclIp );
	}
	else if( strstr( var.m_strVarName.c_str(), "Enabled" ) )
	{
		addSoapParam( retXml, upnpArg.m_strArgName, var, "1" );
	}
	else if( strstr( var.m_strVarName.c_str(), "Description" ) )
	{
		addSoapParam( retXml, upnpArg.m_strArgName, var, portServiceName );
	}
	else if( strstr( var.m_strVarName.c_str(), "LeaseDuration" ) )
	{
		addSoapParam( retXml, upnpArg.m_strArgName, var, "63072000" );
	}
	else
	{
		LogMsg( LOG_ERROR, "createAddPortXml unrecognized var %s", var.m_strVarName.c_str() );
		return false;
	}
	
	return true;
}

//============================================================================
void UpnpParser::addSoapParam( std::string& xmlDoc, std::string& paramName, UpnpStateVariable& var, const char* pValue )
{
	std::string strParam = "<";
	strParam += paramName;
	strParam += SOAP_DATA_SCHEMA;
	strParam += var.m_strDataType;
	strParam += QUOTE;
	strParam += ">";
	strParam += pValue;
	strParam += "</";
	strParam += paramName;
	strParam += ">";
	
	xmlDoc += strParam;
}

/*
//const char PortMapXML[] =
//"<?xml version="1.0 "?>\r\n\
//					<SOAP-ENV:Envelope xmlns:SOAP-ENV=\"http://schemas.xml soap.org/soap/envelope/\" SOAP-\
//					ENV:encodingStyle=\"http://schemas.xmlsoap.org/soap/encoding/\"><SOAP-ENV:Body>\
//					<m:AddPortMapping xmlns:m=\"urn:schemas-upnp-org:service:WANIPConnection:1\">\
//					<NewRemoteHost xmlns:dt=\"urn:schemas-microsoft-com:datatypes\" dt:dt=\"string\"></NewRemoteHost>\
//					<NewExternalPort xmlns:dt=\"urn:schemas-microsoft-com:datatypes\" dt:dt=\"ui2\">2003</NewExternalPort>\
//					<NewProtocol xmlns:dt=\"urn:schemas-microsoft-com:datatypes\" dt:dt=\"string\">TCP</NewProtocol>\
//					<NewInternalPort xmlns:dt=\"urn:schemas-microsoft-com:datatypes\" dt:dt=\"ui2\">2003</NewInternalPort>\
//					<NewInternalClient xmlns:dt=\"urn:schemas-microsoft-com:datatypes\" dt:dt=\"string\">192.168.0.2</NewInternalClient>\
//					<NewEnabled xmlns:dt=\"urn:schemas-microsoft-com:datatypes\" dt:dt=\"boolean\">1</NewEnabled>\
//					<NewPortMappingDescription xmlns:dt=\"urn:schemas-microsoft-com:datatypes\" dt:dt=\"string\">TestPF1</NewPortMappingDescription>\
//					<NewLeaseDuration xmlns:dt=\"urn:schemas-microsoft-com:datatypes\" dt:dt=\"ui4\">0</NewLeaseDuration>\
//					</m:AddPortMapping>\
//					</SOAP-ENV:Body></SOAP-ENV:Envelope>\r\n";  
*/

//============================================================================
bool UpnpParser::findAction( UpnpService& service, const char* pActionName, UpnpAction& action )
{
	std::vector<UpnpAction>::iterator iter;
	for( iter = service.m_aoActions.begin(); iter != service.m_aoActions.end(); ++iter )
	{
		if( strstr( (*iter).m_strActionName.c_str(), pActionName ) )
		{
			action = (*iter);
			return true;
		}
	}
	return false;
}

//============================================================================
bool UpnpParser::findStateVar( std::vector<UpnpStateVariable>& aoVariables, const char* pVarName, UpnpStateVariable& retVar )
{
	std::vector<UpnpStateVariable>::iterator iter;
	for( iter = aoVariables.begin(); iter != aoVariables.end(); ++iter )
	{
		if( strstr( (*iter).m_strVarName.c_str(), pVarName ) )
		{
			retVar = (*iter);
			return true;
		}
	}
	return false;
}
#endif // USE_TINYXML
