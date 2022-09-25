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

#include <AppDependLibrariesConfig.h>
#include <NetLib/VxHttpParse.h>

#include <vector>

class UpnpDevice;
class UpnpService;
class UpnpStateVariable;
class UpnpAction;
class UpnpArg;
class VxSktBase;

class UpnpParser : public VxHttpParse
{
public:
	UpnpParser();
	virtual ~UpnpParser(){};

	void						setParserData(  VxSktBase* sktBase, const char * pDataBuf );
	void						setLclIp( std::string& strIp );
	void						setRmtIpAndPort( std::string& strIp, uint16_t u16Port );
	void						setTimeStamp( uint32_t time );
	uint32_t					getTimeStamp( void );

	bool						isRootDevice( void );
	bool						isDiscover( void );
	bool						isAlive( void );
	bool						isByeBye( void );

	const char *				getST( std::string& value );
	const char *				getNT( std::string& value );
	const char *				getNTS( std::string& value );
	const char *				getMAN( std::string& value );
	const char *				getUSN( std::string& value );
	const char *				getUDN( std::string& value );

	int							getLeaseTime( void );

	//=== vars ===//
	std::string					m_strLclIp;
	std::string					m_strRmtIp;
	uint16_t							m_u16RmtPort;
	uint32_t							m_u32TimeStamp;	
};

