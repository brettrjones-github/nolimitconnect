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

#include "NetActionRandomConnect.h"
#include "NetServicesMgr.h"
#include "NetServiceHdr.h"

#include <ptop_src/ptop_engine_src/HostMgr/HostList.h>
#include <ptop_src/ptop_engine_src/P2PEngine/P2PEngine.h>

#include <CoreLib/VxGlobals.h>
#include <CoreLib/VxParse.h>
#include <CoreLib/VxCrypto.h>
#include <PktLib/PktAnnounce.h>

#include <string.h>
#include <stdarg.h>
#include <stdio.h>

//============================================================================
NetActionRandomConnect::NetActionRandomConnect( NetServicesMgr& netServicesMgr, std::string& anchorIp, uint16_t u16HostPort, EHostAction eHostAction )
: NetActionBase( netServicesMgr )
, m_eHostAction( eHostAction )
, m_FoundCnt( 0 )
{
}

//============================================================================
NetActionRandomConnect::~NetActionRandomConnect()
{
}

//============================================================================
void NetActionRandomConnect::doAction( void )
{
	if( m_Engine.getHasHostService( eHostServiceNetworkHost ) )
	{
		// we are the anchor
		// put ourself directly into the database
		HostList			anchorListIn;
		HostList			anchorListOut;
		anchorListIn.addEntry( &m_Engine.getMyPktAnnounce() );
		anchorListIn.m_HostAction = eHostActionRandomConnect;

		m_NetServicesMgr.getNetServiceHost().getHostDb().handleAnnounce( anchorListIn, anchorListOut );
		m_NetServicesMgr.netActionResultRandomConnect( eAppErrNone, &anchorListOut );
		return;
	}

	VxSktConnectSimple netServConn;
	if( false == m_NetServicesMgr.actionReqConnectToHost( netServConn ) )
	{
		m_NetServicesMgr.netActionResultRandomConnect( eAppErrFailedConnectHost, 0 );
		sendRandomConnectStatus( eRandomConnectStatusContactHostFail, "Could not connect to Host\n" );
		return;
	}

	m_FoundCnt = 0;
	if( doRandomConnectAction( netServConn, 1 ) )
	{
		VxSleep( 10000 );
		if( false == m_NetServicesMgr.actionReqConnectToHost( netServConn ) )
		{
			m_NetServicesMgr.netActionResultRandomConnect( eAppErrFailedConnectHost, 0 );
			sendRandomConnectStatus( eRandomConnectStatusContactHostFail, "Could not connect to Host\n" );
			return;
		}

		if( doRandomConnectAction( netServConn, 2 ) )
		{
			VxSleep( 10000 );
			if( false == m_NetServicesMgr.actionReqConnectToHost( netServConn ) )
			{
				m_NetServicesMgr.netActionResultRandomConnect( eAppErrFailedConnectHost, 0 );
				sendRandomConnectStatus( eRandomConnectStatusContactHostFail, "Could not connect to Host\n" );
				return;
			}

			doRandomConnectAction( netServConn, 3 );
		}
	}

	sendRandomConnectStatus( eRandomConnectStatusSearchComplete, "Random Connect: search complete %d found\n", m_FoundCnt );
}

//============================================================================
bool NetActionRandomConnect::doRandomConnectAction( VxSktConnectSimple& netServConn, int tryCnt )
{

	std::string strNetCmdHdr;
	HostList anchorList;
	uint16_t cryptoPort = netServConn.getRemotePort();
    buildAnnounceCmd( strNetCmdHdr, cryptoPort, anchorList, eHostActionRandomConnect );
	int anchorDataLen = anchorList.m_TotalLen;

	RCODE rc = netServConn.sendData( strNetCmdHdr.c_str(), (int)strNetCmdHdr.length() );
	if( rc )
	{
		netServConn.closeSkt();
		LogMsg( LOG_ERROR, "NetActionRandomConnect::doAction: send header error %d\n", rc );
		m_NetServicesMgr.netActionResultRandomConnect( eAppErrTxError, 0 );
		sendRandomConnectStatus( eRandomConnectStatusSendRequestFail, "Random Connect: send anchor header error %d\n", rc );
		return false;
	}

	VxKey cryptKey;
    m_NetServiceUtils.generateNetServiceCryptoKey( cryptKey, cryptoPort, m_NetServiceUtils.getNetworkKey() );
	VxSymEncrypt( &cryptKey, (char *)&anchorList, anchorDataLen );

	rc = netServConn.sendData( (char *)&anchorList, anchorDataLen );
	if( rc )
	{
		netServConn.closeSkt();
		LogMsg( LOG_ERROR, "NetActionRandomConnect::doAction: send anchor list error %d\n", rc );
		m_NetServicesMgr.netActionResultRandomConnect( eAppErrTxError, 0 );
		sendRandomConnectStatus( eRandomConnectStatusSendRequestFail, "Random Connect: send anchor list error %d\n", rc );
		return false;
	}

	NetServiceHdr netServiceHdr;
	char rxBuf[4096 * 4];
    if( false == m_NetServiceUtils.rxNetServiceCmd( &netServConn, rxBuf, sizeof( rxBuf ), netServiceHdr, RANDOM_CONNECT_RX_TIMEOUT, RANDOM_CONNECT_RX_TIMEOUT ) )
	{
		netServConn.closeSkt();
		m_NetServicesMgr.netActionResultRandomConnect( eAppErrRxError, 0 );
		sendRandomConnectStatus( eRandomConnectStatusInvalidResponse, "Random Connect: Invalid Host Response\n" );
		return false;
	}

	if( false == decryptHostList( rxBuf, netServiceHdr.m_ContentDataLen, cryptoPort ) )
	{
		netServConn.closeSkt();
		m_NetServicesMgr.netActionResultRandomConnect( eAppErrRxError, 0 );
		sendRandomConnectStatus( eRandomConnectStatusDecryptError, "Random Connect: decrypt error \n" );
		return false;
	}

	netServConn.closeSkt();
	HostList * anchorListResult;
	anchorListResult = (HostList *)rxBuf;
	if( 0 == anchorListResult->m_EntryCount )
	{
		if( tryCnt > 2 )
		{
			sendRandomConnectStatus( eRandomConnectStatusEmptyList, "Random Connect: empty search list \n" );
		}
	}
	else
	{
		m_FoundCnt = anchorListResult->m_EntryCount;
	}

	m_NetServicesMgr.netActionResultRandomConnect( eAppErrNone, anchorListResult );
	return true;
}

//============================================================================
bool NetActionRandomConnect::decryptHostList( char * content, int contentDataLen, uint16_t clientPort )
{
	if( false == m_NetServiceUtils.decryptNetServiceContent( content, contentDataLen, clientPort ) )
	{
		return 0;
	}

	HostList * anchorList = (HostList *)content;
	if( anchorList->m_TotalLen != contentDataLen )
	{
		LogMsg( LOG_ERROR, "NetActionRandomConnect::decryptHostList: invalid anchorList->m_TotalLen\n" );
		return false;
	}

    if( MAX_ANCHOR_ENTRIES <= anchorList->m_EntryCount )
	{
		LogMsg( LOG_ERROR, "NetActionRandomConnect::decryptHostList: invalid anchorList->m_EntryCount\n" );
		return false;
	}

	return true;
}

//============================================================================
int NetActionRandomConnect::buildAnnounceCmd( std::string& strNetCmdHdr, uint16_t clientPort, HostList& anchorList, EHostAction anchorAction )
{
	// build Host list
	PktAnnounce& pktAnn = m_Engine.getMyPktAnnounce();
	
	anchorList.m_HostAction = anchorAction;
	anchorList.addEntry( &pktAnn );
	int listLen = anchorList.calculateLength();

	std::string netServChallengeHash;
    m_NetServiceUtils.generateNetServiceChallengeHash( netServChallengeHash, clientPort, getNetworkKey() );

	return m_NetServiceUtils.buildNetCmdHeader( strNetCmdHdr, eNetCmdHostReq, netServChallengeHash, listLen );
}

//============================================================================
int NetActionRandomConnect::getAnnounceData( VxSktConnectSimple * netServConn, char * pRxBuf, int bufLen )
{
	bool bGotCrLfCrLf = false;
	int iMaxToReceive = bufLen - 8;
	int iTotalRxed = 0;
	for( int i = 0; i < 10; ++i )
	{
		int iRxed = 0;
		netServConn->recieveData(		&pRxBuf[ iTotalRxed ],	// data buffer to read into
										iMaxToReceive,			// length of data	
										&iRxed,					// number of bytes actually received
										ANCHOR_RX_DATA_TIMEOUT,	// timeout attempt to receive
										false, 					// if true then abort receive when \r\n\r\n is received
										&bGotCrLfCrLf );						
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

//============================================================================
void NetActionRandomConnect::sendRandomConnectStatus( ERandomConnectStatus eStatus, const char * msg, ... )
{
	char as8Buf[ 1024 ];
	va_list argList;
	va_start( argList, msg );
	vsnprintf( as8Buf, sizeof( as8Buf ), msg, argList );
	as8Buf[sizeof( as8Buf ) - 1] = 0;
	va_end( argList );
	m_Engine.getToGui().toGuiRandomConnectStatus( eStatus, as8Buf );
}
