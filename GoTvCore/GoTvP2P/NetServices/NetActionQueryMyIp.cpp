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

#include "NetActionQueryMyIp.h"
#include "NetServicesMgr.h"

#include <GoTvCore/GoTvP2P/P2PEngine/P2PEngine.h>
#include <GoTvCore/GoTvP2P/NetServices/NetServiceHdr.h>

#include <CoreLib/VxParse.h>

#include <string.h>

//============================================================================
NetActionQueryMyIp::NetActionQueryMyIp( NetServicesMgr& netServicesMgr )
: NetActionBase( netServicesMgr )
{
}

//============================================================================
NetActionQueryMyIp::~NetActionQueryMyIp()
{
}

//============================================================================
void NetActionQueryMyIp::doAction( void )
{
	VxSktConnectSimple * netServConn = m_NetServicesMgr.actionReqConnectToNetService();
	if( NULL == netServConn )
	{
		m_NetServicesMgr.netActionResultQueryWhatsMyIp( NULL );
		return;
	}

	std::string strHttpUrl;
	buildQueryMyIpUrl( netServConn, strHttpUrl );

	RCODE rc = netServConn->sendData( strHttpUrl.c_str(), (int)strHttpUrl.length() );
	if( rc )
	{
		LogMsg( LOG_ERROR, "NetActionQueryMyIp::doAction: sendData error %d\n", rc );
		m_NetServicesMgr.netActionResultQueryWhatsMyIp( NULL );
		return;
	}

	int iRxed = 0;
	char rxBuf[ 512 ];
	bool bGotCrLfCrLf = false;
	netServConn->recieveData(		rxBuf,					// data buffer to read into
									511,					// length of buffer	
									&iRxed,					// number of bytes actually received
									5000,					// timeout attempt to receive
									true, 					// if true then abort receive when \r\n\r\n is received
									&bGotCrLfCrLf );

	netServConn->closeSkt();

	if( ( iRxed <= 0 )
		|| ( false == bGotCrLfCrLf ) )
	{
		LogMsg( LOG_ERROR, "NetActionQueryMyIp::doAction: failed to recieve response\n" );
		m_NetServicesMgr.netActionResultQueryWhatsMyIp( NULL );
		return;
	}

	const std::string strNetServiceUrl = rxBuf;
	std::vector<std::string> astrParts;

	StdStringSplit( strNetServiceUrl, '/', astrParts );

	if( 6 != astrParts.size() )
	{
		LogMsg( LOG_ERROR, "NetActionQueryMyIp::doAction: failed to recieve valid ip\n" );
		m_NetServicesMgr.netActionResultQueryWhatsMyIp( NULL );
		return;
	}

	std::string strPing = astrParts[4];
	const char * pIp = strchr( strPing.c_str(), '-' );
	if( 0 == pIp )
	{
		LogMsg( LOG_ERROR, "NetActionQueryMyIp::doAction: failed to parse ip\n" );
		m_NetServicesMgr.netActionResultQueryWhatsMyIp( NULL );
		return;
	}
	
	pIp++;
	std::string strIp = pIp;

	LogMsg( LOG_INFO, "NetActionQueryMyIp::doAction: my ip is %s\n", strIp.c_str() );
	m_NetServicesMgr.netActionResultQueryWhatsMyIp( strIp.c_str() );
}

//============================================================================
void NetActionQueryMyIp::buildQueryMyIpUrl( VxSktConnectSimple * netServConn, std::string& strHttpUrl )
{
	std::string netServChallengeHash;
    m_NetServiceUtils.generateNetServiceChallengeHash( netServChallengeHash, netServConn, m_NetServiceUtils.getNetworkKey() );

	std::string strContent;
	m_NetServiceUtils.buildNetCmd( strHttpUrl, eNetCmdPing, netServChallengeHash, strContent );
}
