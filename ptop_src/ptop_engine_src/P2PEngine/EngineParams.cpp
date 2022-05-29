//============================================================================
// Copyright (C) 2013 Brett R. Jones
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
// http://www.nolimitconnect.org
//============================================================================
#include "EngineParams.h"
#include "EngineSettingsDefaultValues.h"
#include <NetLib/VxGetRandomPort.h>

#define ENGINE_PARAMS_DBVERSION 1
#define MY_SETTINGS_KEY "RCKEY"

//============================================================================
EngineParams::EngineParams()
: VxSettings( "EngineParamsDb" )
, m_Initialized( false )
{
}

//============================================================================
EngineParams::~EngineParams()
{
	engineParamsShutdown();
}

//============================================================================
RCODE EngineParams::engineParamsStartup( std::string& strDbFileName )
{
	if( m_Initialized )
	{
		engineParamsShutdown();
	}

	RCODE rc = dbStartup(ENGINE_PARAMS_DBVERSION, strDbFileName.c_str());
	if( 0 == rc )
	{
		m_Initialized = true;
	}

	return rc;
}

//============================================================================
void EngineParams::engineParamsShutdown( void )
{
	if( m_Initialized )
	{
		dbShutdown();
		m_Initialized = false;
	}
}


//============================================================================
void EngineParams::setLastHostWebsiteUrl( std::string& strWebsiteUrl )
{
	setIniValue( MY_SETTINGS_KEY, "HostUrl", strWebsiteUrl );
}

//============================================================================
void EngineParams::getLastHostWebsiteUrl( std::string& strWebsiteUrl )
{
	getIniValue( MY_SETTINGS_KEY, "HostUrl", strWebsiteUrl, "" );
}

//============================================================================
void EngineParams::setLastHostWebsiteResolvedIp( std::string& strWebsiteIp )
{
	setIniValue( MY_SETTINGS_KEY, "HostIp", strWebsiteIp );
}

//============================================================================
void EngineParams::getLastHostWebsiteResolvedIp( std::string& strWebsiteIp )
{
	getIniValue( MY_SETTINGS_KEY, "HostIp", strWebsiteIp, "" );
}

//============================================================================
void EngineParams::setLastConnectTestUrl( std::string& strWebsiteUrl )
{
	setIniValue( MY_SETTINGS_KEY, "NetServiceUrl", strWebsiteUrl );
}

//============================================================================
void EngineParams::getLastConnectTestUrl( std::string& strWebsiteUrl )
{
	getIniValue( MY_SETTINGS_KEY, "NetServiceUrl", strWebsiteUrl, "" );
}

//============================================================================
void EngineParams::setLastConnectTestResolvedIp( std::string& strWebsiteIp )
{
	setIniValue( MY_SETTINGS_KEY, "NetServiceIp", strWebsiteIp );
}

//============================================================================
void EngineParams::getLastConnectTestResolvedIp( std::string& strWebsiteIp )
{
	getIniValue( MY_SETTINGS_KEY, "NetServiceIp", strWebsiteIp, "" );
}
