#pragma once
//============================================================================
// Copyright (C) 2014 Brett R. Jones 
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

#include <CoreLib/VxThread.h>
#include <CoreLib/VxMutex.h>
#include <CoreLib/AppErr.h>

class NetworkStateMachine;
class NetworkMgr;
class NetServicesMgr;
class P2PEngine;

class DirectConnectTestResults
{
public:
	DirectConnectTestResults()
		: m_eAppErr( eAppErrUnknown )
	{
	}

	DirectConnectTestResults( const DirectConnectTestResults& rhs )
	{
		*this = rhs;
	}

	DirectConnectTestResults& operator =( const DirectConnectTestResults& rhs )
	{
		if( this != &rhs )
		{
			m_MyIpAddr			= rhs.m_MyIpAddr;
			m_eAppErr			= rhs.m_eAppErr;
		}
		return *this;
	}

	bool						getCanDirectConnect( void )							{ return ( eAppErrNone == m_eAppErr) ? true : false ; }

	EAppErr						m_eAppErr;
	std::string					m_MyIpAddr;
};

class DirectConnectTester
{
public:
	DirectConnectTester( NetworkStateMachine& stateMachine );
	virtual ~DirectConnectTester();

	DirectConnectTestResults&	getDirectConnectTestResults( void )		{ return m_TestResults; }

	void						testCanDirectConnect( void );

	bool						isDirectConnectTestComplete( void );
	bool						isTestResultCanDirectConnect( void );

	void						myPortOpenCallback( EAppErr eAppErr, std::string& myExternalIp );
protected:

	//=== vars ===//
	NetworkStateMachine&		m_NetworkStateMachine;
	P2PEngine&					m_Engine;
	NetworkMgr&					m_NetworkMgr;
	NetServicesMgr&				m_NetServicesMgr;

	bool						m_bTestIsRunning;
	DirectConnectTestResults	m_TestResults;

private:
	DirectConnectTester(); // don't allow default constructor
	DirectConnectTester(const DirectConnectTester&); // don't allow copy constructor
};


