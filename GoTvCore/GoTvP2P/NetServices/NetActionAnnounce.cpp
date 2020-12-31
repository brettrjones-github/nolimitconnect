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

#include "NetActionAnnounce.h"
#include "NetServicesMgr.h"
#include "NetServiceHdr.h"

#include <GoTvCore/GoTvP2P/HostMgr/HostList.h>
#include <GoTvCore/GoTvP2P/P2PEngine/P2PEngine.h>

#include <CoreLib/VxGlobals.h>
#include <CoreLib/VxParse.h>
#include <CoreLib/VxCrypto.h>
#include <PktLib/PktAnnounce.h>

#include <string.h>

//============================================================================
NetActionAnnounce::NetActionAnnounce( NetServicesMgr& netServicesMgr, std::string& anchorIp, uint16_t u16HostPort, EHostAction eHostAction )
: NetActionBase( netServicesMgr )
, m_eHostAction( eHostAction )
{
}

//============================================================================
void NetActionAnnounce::doAction( void )
{
	if( m_Engine.getHasHostService( eHostServiceNetworkHost ) )
	{
		if( m_Engine.getEngineSettings().getExcludeMeFromNetHostList() )
		{
			LogMsg( LOG_INFO, "NetActionAnnounce::doAction ignoring because getExcludeMeFromNetHostList true\n" );
			return;
		}

		LogMsg( LOG_INFO, "NetActionAnnounce::doAction anouncing as anchor\n" );
		// we are the anchor
		HostList			anchorListOut;
		HostList			anchorListIn;
		// put ourself directly into the database
		anchorListIn.addEntry( &m_Engine.getMyPktAnnounce() );

		m_NetServicesMgr.getNetServiceHost().getHostDb().handleAnnounce( anchorListIn, anchorListOut );
		m_NetServicesMgr.netActionResultAnnounce( eAppErrNone, &anchorListOut );
		return;
	}

	VxSktConnectSimple netServConn;
	if( false == m_NetServicesMgr.actionReqConnectToHost( netServConn ) )
	{
		m_NetServicesMgr.netActionResultAnnounce( eAppErrFailedConnectHost, 0 );
		AppErr( eAppErrFailedConnectHost, "Host Announce: Could not connect to Host\n" );
		return;
	}

	std::string strNetCmdHdr;
	HostList anchorList;
	uint16_t acceptedPort = netServConn.getRemotePort();
    buildAnnounceCmd( strNetCmdHdr, acceptedPort, anchorList, eHostActionAnnounce );
	int anchorDataLen = anchorList.m_TotalLen;

	RCODE rc = netServConn.sendData( strNetCmdHdr.c_str(), (int)strNetCmdHdr.length() );
	if( rc )
	{
		LogMsg( LOG_ERROR, "### ERROR NetActionAnnounce::doAction: send header error %d %s\n", rc, VxDescribeSktError( rc ) );
		m_NetServicesMgr.netActionResultAnnounce( eAppErrTxError, 0 );
		AppErr( eAppErrTxError, "Host Announce: send anchor header error %d %s\n", rc, VxDescribeSktError( rc ) );
		netServConn.closeSkt();
		return;
	}

	VxKey cryptKey;
    m_NetServiceUtils.generateNetServiceCryptoKey( cryptKey, acceptedPort, m_NetServicesMgr.getNetworkKey() );
	VxSymEncrypt( &cryptKey, (char *)&anchorList, anchorDataLen );

	rc = netServConn.sendData( (char *)&anchorList, anchorDataLen );
	if( rc )
	{
		netServConn.closeSkt();
		LogMsg( LOG_ERROR, "### ERROR NetActionAnnounce::doAction: send anchor list error %d %s\n", rc, VxDescribeSktError( rc ) );
		m_NetServicesMgr.netActionResultAnnounce( eAppErrTxError, 0 );
		AppErr( eAppErrTxError, "Host Announce: send anchor list error %d %s\n", rc, VxDescribeSktError( rc ) );
		return;
	}

	NetServiceHdr netServiceHdr;
	char rxBuf[(4096 * 4) + 1];
	if( false == m_NetServiceUtils.rxNetServiceCmd( &netServConn, 
													rxBuf, 
													sizeof( rxBuf ) - 1, 
													netServiceHdr,
													ANCHOR_RX_HDR_TIMEOUT,
													ANCHOR_RX_DATA_TIMEOUT ) )
	{
		LogMsg( LOG_ERROR, "### ERROR NetActionAnnounce::rxNetServiceCmd: hdr timout %d data timeout %d\n", ANCHOR_RX_HDR_TIMEOUT, ANCHOR_RX_DATA_TIMEOUT );
		netServConn.closeSkt();
		m_NetServicesMgr.netActionResultAnnounce( eAppErrRxError, 0 );
		AppErr( eAppErrRxError, "Host Announce: receive cmd error \n" );
		return;
	}

    rxBuf[ sizeof( rxBuf ) - 1 ] = 0;
	if( false == decryptHostList( rxBuf, netServiceHdr.m_ContentDataLen, acceptedPort ) )
	{
		netServConn.closeSkt();
		m_NetServicesMgr.netActionResultAnnounce( eAppErrRxError, 0 );
		AppErr( eAppErrRxError, "Host Announce: decrypt error \n" );
		return;
	}

	netServConn.closeSkt();
	m_NetServicesMgr.netActionResultAnnounce( eAppErrNone, (HostList *)rxBuf );
}

//============================================================================
bool NetActionAnnounce::decryptHostList( char * content, int contentDataLen, uint16_t clientPort )
{
	if( false == m_NetServiceUtils.decryptNetServiceContent( content, contentDataLen, clientPort ) )
	{
		return 0;
	}

	HostList * anchorList = (HostList *)content;
	if( anchorList->m_TotalLen != contentDataLen )
	{
		LogMsg( LOG_ERROR, "NetActionAnnounce::decryptHostList: invalid anchorList->m_TotalLen\n" );
		return false;
	}

    if( MAX_ANCHOR_ENTRIES < anchorList->m_EntryCount )
	{
		LogMsg( LOG_ERROR, "NetActionAnnounce::decryptHostList: invalid anchorList->m_EntryCount\n" );
		return false;
	}

	return true;
}

//============================================================================
int NetActionAnnounce::buildAnnounceCmd( std::string& strNetCmdHdr, uint16_t clientPort, HostList& anchorList, EHostAction anchorAction )
{
	// build Host list
	PktAnnounce& pktAnn = m_Engine.getMyPktAnnounce();
	
	anchorList.m_HostAction = anchorAction;
	anchorList.addEntry( &pktAnn );
	int listLen = anchorList.calculateLength();

	std::string netServChallengeHash;
    m_NetServiceUtils.generateNetServiceChallengeHash( netServChallengeHash, clientPort, m_NetServiceUtils.getNetworkKey() );

	int totalLen = m_NetServiceUtils.buildNetCmdHeader( strNetCmdHdr, eNetCmdHostReq, netServChallengeHash, listLen );
	return totalLen;
}

//============================================================================
int NetActionAnnounce::getAnnounceData( VxSktConnectSimple * netServConn, char * pRxBuf, int bufLen )
{
	int iMaxToReceive = bufLen - 8;
	int iTotalRxed = 0;
	for( int i = 0; i < 10; ++i )
	{
		int iRxed = 0;
		netServConn->recieveData(	&pRxBuf[ iTotalRxed ],		// data buffer to read into
									iMaxToReceive,				// length of data	
									&iRxed,						// number of bytes actually received
									ANCHOR_RX_DATA_TIMEOUT );	// timeout attempt to receive
		if( iRxed <= 0 )
		{
			break;
		}

		pRxBuf[ iTotalRxed + iRxed ] = 0; 
		if( strstr( &pRxBuf[ iTotalRxed ], "Footer" ) )
		{
			iTotalRxed += iRxed;
			break;
		}

		iTotalRxed += iRxed;
		iMaxToReceive -= iRxed;
	}

	pRxBuf[ iTotalRxed ] = 0; 

	return iTotalRxed;
}


