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

#include "NetActionQueryHostOnlineId.h"
#include "NetServicesMgr.h"

#include <GoTvCore/GoTvP2P/P2PEngine/P2PEngine.h>
#include <GoTvCore/GoTvP2P/NetServices/NetServiceHdr.h>

#include <CoreLib/VxParse.h>

#include <string.h>

//============================================================================
NetActionQueryHostOnlineId::NetActionQueryHostOnlineId( NetServicesMgr& netServicesMgr )
: NetActionBase( netServicesMgr )
{
}

//============================================================================
void NetActionQueryHostOnlineId::doAction( void )
{
    VxGUID hostOnlineId;
	VxSktConnectSimple * netServConn = m_NetServicesMgr.actionReqConnectToNetService();
	if( NULL == netServConn )
	{
		m_NetServicesMgr.netActionResultQueryHostId( eAppErrFailedToConnect, hostOnlineId );
		return;
	}

	std::string strHttpUrl;
    buildQueryHostOnlineIdUrl( netServConn, strHttpUrl );

	RCODE rc = netServConn->sendData( strHttpUrl.c_str(), (int)strHttpUrl.length() );
	if( rc )
	{
		LogMsg( LOG_ERROR, "NetActionQueryHostOnlineId::doAction: sendData error %d\n", rc );
		m_NetServicesMgr.netActionResultQueryHostId( eAppErrFailedToConnect, hostOnlineId );
		return;
	}

	int iRxed = 0;
	char rxBuf[ 512 ];
	bool bGotCrLfCrLf = false;
	netServConn->recieveData(		rxBuf,					// data buffer to read into
									511,					// length of buffer	
									&iRxed,					// number of bytes actually received
									6000,					// timeout attempt to receive
									true, 					// if true then abort receive when \r\n\r\n is received
									&bGotCrLfCrLf );

	netServConn->closeSkt();

	if( ( iRxed <= 0 )
		|| ( false == bGotCrLfCrLf ) )
	{
		LogMsg( LOG_ERROR, "NetActionQueryHostOnlineId::doAction: failed to recieve response\n" );
		m_NetServicesMgr.netActionResultQueryHostId( eAppErrRxError, hostOnlineId );
		return;
	}

	const std::string strNetServiceUrl = rxBuf;
	std::vector<std::string> astrParts;

	StdStringSplit( strNetServiceUrl, '/', astrParts );

	if( 6 != astrParts.size() )
	{
		LogMsg( LOG_ERROR, "NetActionQueryHostOnlineId::doAction: failed to recieve valid id\n" );
		m_NetServicesMgr.netActionResultQueryHostId( eAppErrParseError, hostOnlineId );
		return;
	}

	std::string strContent = astrParts[4];
    if( strContent.empty() )
    {
        LogMsg( LOG_ERROR, "NetActionQueryHostOnlineId::doAction: failed to parse online id\n" );
        m_NetServicesMgr.netActionResultQueryHostId( eAppErrParseError, hostOnlineId );
        return;
    }

    hostOnlineId.fromVxGUIDHexString( strContent.c_str() );
    if( !hostOnlineId.isVxGUIDValid() )
    {
        LogMsg( LOG_ERROR, "NetActionQueryHostOnlineId::doAction: failed to parse online id\n" );
        m_NetServicesMgr.netActionResultQueryHostId( eAppErrParseError, hostOnlineId );
        return;
    }

    LogMsg( LOG_INFO, "NetActionQueryHostOnlineId::doAction: host online id is %s\n", strContent.c_str() );
	m_NetServicesMgr.netActionResultQueryHostId( eAppErrNone, hostOnlineId );
}

//============================================================================
void NetActionQueryHostOnlineId::buildQueryHostOnlineIdUrl( VxSktConnectSimple * netServConn, std::string& strHttpUrl )
{
	std::string netServChallengeHash;
    m_NetServiceUtils.generateNetServiceChallengeHash( netServChallengeHash, netServConn, getNetworkKey() );

	std::string strContent;
	m_NetServiceUtils.buildNetCmd( strHttpUrl, eNetCmdQueryHostOnlineIdReq, netServChallengeHash, strContent );
}
