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

#include "NetServiceHost.h"
#include <ptop_src/ptop_engine_src/HostMgr/HostList.h>

#include <ptop_src/ptop_engine_src/P2PEngine/P2PEngine.h>
#include <ptop_src/ptop_engine_src/NetServices/NetServiceHdr.h>
#include <ptop_src/ptop_engine_src/NetServices/NetServiceUtils.h>

#include <NetLib/VxSktBase.h>

#include <CoreLib/VxParse.h>
#include <CoreLib/VxGlobals.h>
#include <CoreLib/OsDetect.h>

namespace
{
	//const int		NET_CMD_ANCHOR_REPLY_VERSION			= 1;	 
	const char *	ANCHOR_DB_FILE_NAME						= "HostDb.db3";
	const int 		ANCHOR_DATABASE_VERSION 				= 1;
}

//============================================================================
NetServiceHost::NetServiceHost( P2PEngine& engine, NetServicesMgr& netServicesMgr, NetServiceUtils& netServiceUtils )
: m_Engine( engine )
, m_NetServicesMgr( netServicesMgr )
, m_NetServiceUtils( netServiceUtils )
{
}

//============================================================================
NetServiceHost::~NetServiceHost()
{
}

//============================================================================
void NetServiceHost::netServiceHostStartup( void )
{
	std::string anchorFileName = VxGetSettingsDirectory() + ANCHOR_DB_FILE_NAME;
	m_HostDb.dbStartup( ANCHOR_DATABASE_VERSION, anchorFileName );
}

//============================================================================
void NetServiceHost::netServiceHostShutdown( void )
{
	m_HostDb.dbShutdown();
}

//============================================================================
RCODE NetServiceHost::handleNetCmdHostReq( VxSktBase * sktBase, NetServiceHdr& netServiceHdr )
{
	RCODE rc = -1;
	if( ( 0 == netServiceHdr.m_ContentDataLen )
		|| ( 0x0f & netServiceHdr.m_ContentDataLen ) )
	{
		VxReportHack( eHackerLevelSuspicious, eHackerReasonNetCmdLength, sktBase, "handleNetCmdHostReq: Invalid content len %d", netServiceHdr.m_ContentDataLen );
		sktBase->closeSkt( eSktCloseNetCmdLengthInvalid );
		return -1;
	}
	
	char * pSktBuf = (char *)sktBase->getSktReadBuf();
	sktBase->sktBufAmountRead( 0 );

	char * content = &pSktBuf[ netServiceHdr.m_SktDataUsed ];
	int contentDataLen = netServiceHdr.m_ContentDataLen;
	uint16_t clientPort = sktBase->getCryptoKeyPort();
	HostList * anchorList = (HostList *)content;

	VxKey key;
    m_NetServiceUtils.generateNetServiceCryptoKey( key, clientPort, m_NetServiceUtils.getNetworkKey() );
	VxSymDecrypt( &key, content, contentDataLen );

	if( false == verifyHostList( anchorList, contentDataLen ) )
	{
		VxReportHack( eHackerLevelSuspicious, eHackerReasonNetCmdListInvalid, sktBase, "handleNetCmdHostReq: Invalid list" );
		sktBase->closeSkt( eSktCloseNetCmdListInvalid );
		return -1;
	}

	HostList anchorResult;
	if( m_Engine.getEngineSettings().getExcludeMeFromNetHostList() )
	{
		rc = m_HostDb.handleAnnounce( *anchorList, anchorResult, m_Engine.getMyOnlineId() );
	}
	else
	{
		rc = m_HostDb.handleAnnounce( *anchorList, anchorResult );
	}

	int sendContentLen = anchorResult.calculateLength();

	std::string cmdHdr;
	m_NetServiceUtils.buildNetCmdHeader( cmdHdr, eNetCmdHostReply, netServiceHdr.m_ChallengeHash, sendContentLen, rc, 1 );
	VxSymEncrypt( &key, (char *)&anchorResult, sendContentLen );

	if( sktBase->isConnected() )
	{
		sktBase->sendData( cmdHdr.c_str(), (int)cmdHdr.length() );
		sktBase->sendData( (char *)&anchorResult, sendContentLen );
	}
	else
	{
		rc = -1;
	}

	return rc;
}

//============================================================================
bool NetServiceHost::verifyHostList( HostList * anchorList, int contentDataLen )
{
	if( anchorList->m_TotalLen != contentDataLen )
	{
		LogMsg( LOG_ERROR, "handleNetCmdHostReq: invalid nchorList->m_TotalLen\n" );
		return false;
	}

	if( ( 0 == anchorList->m_EntryCount )
		|| ( MAX_ANCHOR_ENTRIES < anchorList->m_EntryCount ) )
	{
		LogMsg( LOG_ERROR, "handleNetCmdHostReq: invalid anchorList->m_EntryCount\n" );
		return false;
	}

	return true;
}

//============================================================================
RCODE NetServiceHost::handleNetCmdHostReply( VxSktBase * sktBase, NetServiceHdr& netServiceHdr )
{
	// not done here .. done by net action
	return 0;
}
