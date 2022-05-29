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

#include "NetPortForward.h"
#include "NetworkStateMachine.h"

#include <CoreLib/VxGlobals.h>

#include <time.h>

#ifdef _MSC_VER
# pragma warning(disable: 4355) //'this' : used in base member initializer list
#endif //_MSC_VER

namespace
{
	typedef void (*PORT_FORWARD_CALLBACK_FUNCTION )( void *, EAppErr );

	//============================================================================
	void NetPortForwardCallback( void * userData, EAppErr eAppErr )
	{
		NetPortForward * poUpdater = (NetPortForward *)userData;
		if( poUpdater )
		{
			poUpdater->portForwardCallback( eAppErr );
		}
	}
}

//============================================================================
NetPortForwardSettings::NetPortForwardSettings() 
{
};

//============================================================================
NetPortForwardSettings::NetPortForwardSettings(const NetPortForwardSettings& rhs) 
{
	*this = rhs;
};

//============================================================================
NetPortForwardSettings::~NetPortForwardSettings() 
{
};

//============================================================================
NetPortForwardSettings& NetPortForwardSettings::operator= (const NetPortForwardSettings& rhs)
{
	if( &rhs != this )
	{
		m_u16Port			= rhs.m_u16Port;
		m_strAppName		= rhs.m_strAppName;
		m_strIPv4			= rhs.m_strIPv4;
		m_strIPv6			= rhs.m_strIPv6;
	}
	return *this;
}

//============================================================================
NetPortForward::NetPortForward( NetworkStateMachine& networkStateMachine )
: m_NetworkStateMachine( networkStateMachine )
, m_UpnpMapper()
, m_LastUsedSettings()
, m_bIsCompleted( false )
, m_DidUpnpPortOpen( false )
, m_TimeLastAttempt( 0 )
{
	m_UpnpMapper.setPortForwardResultCallback( NetPortForwardCallback, this );
}

//============================================================================
NetPortForward::~NetPortForward()
{
	netPortForwardShutdown();
}

//============================================================================
void NetPortForward::netPortForwardShutdown( void )
{
	m_UpnpMapper.upnpPortForwardShutdown();
}

//============================================================================
time_t	 NetPortForward::elapsedSecondsLastAttempt( void )
{
	return GetTimeStampMs() - m_TimeLastAttempt;
}

//============================================================================
void NetPortForward::beginPortForward( uint16_t u16Port, const char * ipToForwardTo )
{
	NetPortForwardSettings		settings;
	settings.m_u16Port			= u16Port;
	settings.m_strAppName		= VxGetApplicationNameNoSpaces();
	settings.m_strIPv4			= ipToForwardTo;
	m_bIsCompleted				= false;
	m_TimeLastAttempt			= GetTimeStampMs();

	if( ( settings.m_u16Port == m_LastUsedSettings.m_u16Port )
		&& ( settings.m_strIPv4 == m_LastUsedSettings.m_strIPv4 ) )
	{
		LogMsg( LOG_INFO, "NetPortForward::beginPortForward: Already attempted port %d to %s\n", u16Port, ipToForwardTo );
		return;
	}

#ifdef DEBUG_UPNP
	LogMsg( LOG_INFO, "NetPortForward::beginPortForward: port %d to %s\n", u16Port, ipToForwardTo );
#endif // DEBUG_UPNP
	m_DidUpnpPortOpen = false;
	m_UpnpMapper.tcpPortForward( settings.m_u16Port, settings.m_strAppName.c_str(), settings.m_strIPv4.c_str()  );
}

//============================================================================
bool NetPortForward::hasPortForwardCompleted( void )
{
	return m_bIsCompleted;
}

//============================================================================
bool NetPortForward::didUpnpPortOpen( void )
{
	return m_DidUpnpPortOpen;
}

//============================================================================
void NetPortForward::portForwardCallback( EAppErr eAppErr )
{
	// there may be more than one.. if any one succeeds then say port is open
	if( eAppErrNone == eAppErr )
	{
		m_DidUpnpPortOpen = true;	
	}

	m_bIsCompleted = true;
}

//============================================================================
bool NetPortForward::waitTime( int iTimeSec )
{
	for( int i = 0; i < iTimeSec; i++ )
	{
		if( m_Thread.isAborted() 
			|| VxIsAppShuttingDown() )
		{
			return false;
		}
		VxSleep( 1000 );
	}

	return true;
}
