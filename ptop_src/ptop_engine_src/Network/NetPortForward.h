#pragma once
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
// http://www.nolimitconnect.com
//============================================================================

//#include <UpnpLib/UpnpClientInterface.h>
#include <Upnp2Lib/UpnpPortForward.h>

#include <CoreLib/VxSemaphore.h>
#include <CoreLib/VxDefs.h>

#include <string>
#include <vector>

class EngineSettings;

class NetPortForwardSettings
{
public:
	NetPortForwardSettings();
	NetPortForwardSettings( const NetPortForwardSettings& rhs );
	virtual ~NetPortForwardSettings();

	NetPortForwardSettings& operator=( const NetPortForwardSettings& rhs );

	//=== vars ===//
	uint16_t							m_u16Port;
	std::string					m_strAppName;
	std::string					m_strIPv4;
	std::string					m_strIPv6;
};

class NetworkStateMachine;

class NetPortForward //: public UpnpClientInterface
{
public:
	NetPortForward( NetworkStateMachine& networkStateMachine ); 
	virtual ~NetPortForward();

	void						netPortForwardShutdown( void );

	virtual void				beginPortForward( uint16_t u16Port, const char * ipToForwardTo );
	time_t						elapsedSecondsLastAttempt( void );
	bool						hasPortForwardCompleted( void );
	bool						didUpnpPortOpen( void );

	void						portForwardCallback( EAppErr eAppErr );
//// UpnpClientInterface
//	virtual void				upnpStatusMsg( const char * pDeviceId, const char * pMsg );
//	virtual void				upnpPortForwardSent(  const char * pDeviceId );
//	virtual void				upnpPortForwardTimedOut( void );
//	virtual void				upnpComplete( void );

	void						runUpdates();

protected:
	bool						waitTime( int iTimeSec );

	//=== vars ===//
	NetworkStateMachine&		m_NetworkStateMachine;
    VxThread					m_Thread;
	VxMutex						m_PortForwardMutex;
	std::vector<NetPortForwardSettings>	m_SettingsList; 
    UpnpPortForward				m_UpnpMapper;
    NetPortForwardSettings		m_LastUsedSettings;
    bool						m_bIsCompleted;
	bool						m_DidUpnpPortOpen;
	time_t						m_TimeLastAttempt;
};

