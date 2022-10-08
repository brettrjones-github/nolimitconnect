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

#include "RcScan.h"

#include <ptop_src/ptop_engine_src/P2PEngine/P2PEngine.h>
#include <GuiInterface/IToGui.h>
#include <ptop_src/ptop_engine_src/NetServices/NetServicesMgr.h>
#include <ptop_src/ptop_engine_src/Network/NetConnector.h>

#include <ptop_src/ptop_engine_src/Plugins/PluginCamServer.h>
#include <ptop_src/ptop_engine_src/BigListLib/BigListInfo.h>

#include <CoreLib/VxParse.h>
#include <CoreLib/VxGlobals.h>

#include <string.h>
#include <memory.h>

#ifdef _MSC_VER
# pragma warning(disable: 4355) //'this' : used in base member initializer list
#endif

//============================================================================
RcScan::RcScan( P2PEngine& engine, P2PConnectList& connectList )
: m_Engine(engine)
, m_ConnectList( connectList )
, m_bIsScanning(false)
, m_bActionThreadRunning(false)
, m_eScanType( eScanTypeNone )
, m_ScanAction( engine, *this )
, m_u32ToGuiSendPicTime(0)
, m_u32SearchActionComplete(0)
, m_s64LastActionTimeMs( 0 )
{
}

//============================================================================
void RcScan::onOncePer30Seconds( void )
{
	if( m_bIsScanning )
	{
		int64_t timeNow = GetGmtTimeMs();
		if( ( timeNow - m_s64LastActionTimeMs ) >= 30000 )
		{
			m_ScanAction.searchConnectionsTimedOut();
		}

		m_ScanAction.onOncePer30Seconds();	
	}
}

//============================================================================
void RcScan::onOncePerMinute( void )
{
	if( m_bIsScanning )
	{
		m_ScanAction.onOncePerMinute();	
	}
}

//============================================================================
void RcScan::actionThreadRunning( bool isRunning )
{
	m_bActionThreadRunning = isRunning;
}

//============================================================================
// handle case where BigListInfo is about to be deleted
void RcScan::onIdentDelete( VxNetIdent* netIdent )
{
	m_ScanAction.onIdentDelete( netIdent );
}

//============================================================================
void RcScan::onContactWentOnline( VxNetIdent* netIdent, VxSktBase* sktBase )
{
	if( m_bIsScanning 
		&& m_IdentsSentSearchPktList.addGuidIfDoesntExist( netIdent->getMyOnlineId() ) )
	{
		m_Engine.txImAlivePkt( netIdent->getMyOnlineId(), sktBase );		
		m_Engine.txSystemPkt( netIdent->getMyOnlineId(), sktBase, &m_SearchPkt );
	}
}

//============================================================================
void RcScan::onContactWentOffline( VxNetIdent* netIdent, VxSktBase* sktBase )
{
	m_ScanAction.onContactWentOffline( netIdent, sktBase );
}

//============================================================================
void RcScan::onConnectionLost( VxSktBase* sktBase )
{
	m_ScanAction.onConnectionLost( sktBase );
}

//============================================================================
//! called when new better connection from user
void RcScan::replaceConnection( VxNetIdent* netIdent, VxSktBase * poOldSkt, VxSktBase * poNewSkt )
{
	m_ScanAction.replaceConnection( netIdent, poOldSkt, poNewSkt );
}

//============================================================================
void RcScan::scanShutdown( void )
{
	fromGuiStopScan( m_eScanType );
}

//============================================================================
void RcScan::fromGuiStartScan( EScanType eScanType, uint8_t searchFlags, uint8_t fileTypeFlags, const char* pSearchPattern )
{
	if( VxIsAppShuttingDown() )
	{
		return;
	}

	if( eScanTypeRandomConnect == eScanType )
	{
		//m_Engine.getNetServicesMgr().performRandomConnect();
		return;
	}

	// stop any previous scans
	LogMsg( LOG_INFO, "** fromGuiStartScan stop prev scan");
	fromGuiStopScan( m_eScanType );

	LogMsg( LOG_INFO, "** fromGuiStartScan begin");
	m_eScanType = eScanType;
	m_s64LastActionTimeMs = GetGmtTimeMs();

	VxGUID myOnlineId = m_Engine.getMyOnlineId();

	m_SearchPkt.setSearchType( eScanType );
	m_SearchPkt.setSearchFlags( searchFlags );
	m_SearchPkt.setSharedFileTypes( fileTypeFlags );
	m_SearchPkt.setSearchExpression( pSearchPattern );
	m_SearchPkt.setSrcOnlineId( myOnlineId );
	m_SearchPkt.calcPktLen();

	m_ScanMutex.lock();
	m_IdentsReqConnectList.clearList();
	m_IdentsReqConnectList.addGuid( myOnlineId );
	m_IdentsSentSearchPktList.clearList();
	m_IdentsSentSearchPktList.addGuid( myOnlineId );
	m_IdentsWithSearchMatchList.clearList();
	m_IdentsWithSearchMatchList.addGuid( myOnlineId );
	m_Engine.broadcastSystemPkt( &m_SearchPkt, m_IdentsSentSearchPktList );
	m_IdentsSentSearchPktList.copyTo( m_IdentsReqConnectList );
	m_bIsScanning = true;
	m_ScanMutex.unlock();


	m_ScanAction.fromGuiStartScan( m_eScanType );
	LogMsg( LOG_INFO, "** fromGuiStartScan done");
}

//============================================================================
void RcScan::fromGuiNextScan( EScanType eScanType )
{
	m_ScanAction.fromGuiNextScan( eScanType );
}

//============================================================================
void RcScan::fromGuiStopScan( EScanType eScanType )
{
	LogMsg( LOG_INFO, "** fromGuiStopScan begin\n");
	m_bIsScanning = false;
	m_ScanAction.fromGuiStopScan( eScanType );
	LogMsg( LOG_INFO, "** fromGuiStopScan done");
	m_ScanMutex.lock();
	m_IdentsReqConnectList.clearList();
	m_IdentsSentSearchPktList.clearList();
	m_IdentsWithSearchMatchList.clearList();
	m_ScanMutex.unlock();
}

//============================================================================
void RcScan::onPktScanReq( VxNetIdent* netIdent, VxSktBase* sktBase, PktScanReq* poPkt )
{
	unsigned int iMatchesFound = 0;
	unsigned int iNotMatchesFound = 0;

	P2PConnectList::ConnectListIter iter;
	BigListInfo * poBigListInfo;
	RcConnectInfo * poConnectInfo;
	PktScanReply oPktReply;
	uint8_t * pktPayload = oPktReply.getSearchMatch();
	oPktReply.setSizeOfConnectInfo( (uint16_t)sizeof( VxConnectInfo ) );

#ifdef DEBUG_MUTEXES
	LogMsg( LOG_INFO, "RcScan::handlePktScanReq: m_ConnectListMutex.lock()\n" );
#endif // DEBUG_MUTEXES
	std::vector<VxNetIdent*> noMatchList;
	m_ConnectList.connectListLock();

	for( iter = m_ConnectList.m_ConnectList.begin(); iter != m_ConnectList.m_ConnectList.end(); ++iter )
	{
		poConnectInfo = iter->second;
		poBigListInfo = poConnectInfo->m_BigListInfo;
		VxNetIdent* ident = (VxNetIdent*)poBigListInfo;
		if( ident )
		{
			if( isRemoteSearchMatch( poBigListInfo, poPkt ) )
			{
				memcpy( &pktPayload[ iMatchesFound * sizeof( VxConnectInfo ) ], 
					&ident->getConnectInfo(), 
					sizeof( VxConnectInfo ) );
				iMatchesFound++;
				if( PKT_ANN_MAX_IDENT_MATCH <= iMatchesFound )
				{
					break;
				}
			}
			else
			{
				noMatchList.push_back( ident );
			}
		}
	}

#ifdef DEBUG_MUTEXES
	LogMsg( LOG_INFO, "RcScan::handlePktScanReq: m_ConnectListMutex.unlock()\n" );
#endif // DEBUG_MUTEXES
	m_ConnectList.connectListUnlock();

	std::vector<VxNetIdent*>::iterator noMatchIter;
	for( noMatchIter = noMatchList.begin(); noMatchIter != noMatchList.end(); ++noMatchIter )
	{
		if( iMatchesFound + iNotMatchesFound < PKT_ANN_MAX_IDENT_MATCH )
		{
			VxNetIdent* ident = (*noMatchIter);
			memcpy( &pktPayload[ (iMatchesFound + iNotMatchesFound) * sizeof( VxConnectInfo ) ], 
				&ident->getConnectInfo(), 
				sizeof( VxConnectInfo ) );
			iNotMatchesFound++;
		}
		else
		{
			break;
		}
	}

	// fill the packet 
	oPktReply.setIdentMatchCount( (uint16_t)iMatchesFound );
	int totalIdents = iMatchesFound + iNotMatchesFound;
	if( totalIdents > PKT_ANN_MAX_IDENT_MATCH )
	{
		totalIdents = PKT_ANN_MAX_IDENT_MATCH;
	}

	oPktReply.setSizeOfConnectInfo( (uint16_t) sizeof( VxConnectInfo ) );
	oPktReply.setTotalIdentCount( (uint16_t)totalIdents );
	oPktReply.calcPktLen();
	oPktReply.setSearchType( poPkt->getSearchType() );
	oPktReply.setSearchFlags( poPkt->getSearchFlags() );

	m_Engine.txSystemPkt( netIdent, sktBase, &oPktReply );
}

//============================================================================
void RcScan::onPktScanReply( VxNetIdent* netIdent, VxSktBase* sktBase, PktScanReply * poPkt )
{
	if( m_bIsScanning )
	{
		m_ScanMutex.lock();
		if( ( m_IdentsWithSearchMatchList.addGuidIfDoesntExist( netIdent->getMyOnlineId() ) )
			&& ( true == isLocalSearchMatch( netIdent ) ) )
		{
			if( ( eScanTypePeopleSearch == m_eScanType )
				|| ( eScanTypeMoodMsgSearch == m_eScanType )
				|| ( eScanTypeCamServer == m_eScanType ) )
			{
				m_Engine.getToGui().toGuiScanResultSuccess( m_eScanType, netIdent );
			}
			else
			{
				m_ScanAction.addMatchedConnection( netIdent, sktBase );
			}
		}

		VxNetIdent* fromPktIdent;
		int totalIdentCnt	= poPkt->getTotalIdentCount();
		uint8_t * pktPayload		= poPkt->getSearchMatch();
		for( int i = 0; i < totalIdentCnt; ++i )
		{
			fromPktIdent = (VxNetIdent*)&pktPayload[ i * sizeof(VxNetIdent) ];
			if( m_IdentsReqConnectList.addGuidIfDoesntExist( fromPktIdent->getMyOnlineId() ) )
			{
				m_Engine.getNetConnector().addConnectRequestToQue( fromPktIdent->getConnectInfo(), eConnectReasonOtherSearch );
			}
		}

		m_ScanMutex.unlock();
	}
}

//============================================================================
void RcScan::onPktFindFileReq( VxNetIdent* netIdent, VxSktBase* sktBase, PktFindFileReq* poPkt )
{

}

//============================================================================
void RcScan::onPktFindFileReply( VxNetIdent* netIdent, VxSktBase* sktBase, PktFindFileReply * poPkt )
{

}

//============================================================================
void RcScan::onScanResultError( EScanType eScanType, VxNetIdent* netIdent, VxSktBase* sktBase,  uint32_t errCode )
{
	m_ScanAction.onScanResultError( eScanType, netIdent, sktBase, errCode );
}

//============================================================================
void RcScan::onScanResultProfilePic( VxNetIdent* netIdent, VxSktBase* sktBase, uint8_t* pu8JpgData, uint32_t u32JpgDataLen )
{
	m_ScanAction.onScanResultProfilePic( netIdent, sktBase, pu8JpgData, u32JpgDataLen );
}

//============================================================================
void RcScan::scanComplete( void )
{
	fromGuiStopScan( m_eScanType );
	m_Engine.getToGui().toGuiScanSearchComplete( m_eScanType );
}

//============================================================================
bool RcScan::isLocalSearchMatch( VxNetIdent* netIdent )
{
	bool bIsMatch = false;
	switch( m_eScanType )
	{
	case eScanTypeProfilePic:
		{
			EFriendState ePermissionLevel = netIdent->getPluginPermission( ePluginTypeAboutMePageServer );
			if( eFriendStateIgnore == ePermissionLevel )
			{
				return false;
			}

			if( ePermissionLevel > netIdent->getHisFriendshipToMe() )
			{
				return false;
			}

			if( netIdent->hasProfilePicture() )
			{
				bIsMatch = true;
			}

			break;
		}

	case eScanTypePeopleSearch:
		{
			if( stristr( netIdent->getOnlineName(), m_SearchPkt.getSearchExpression() ) )
			{
				bIsMatch = true;
			}

			break;
		}

	case eScanTypeMoodMsgSearch:
		{
			if( stristr( netIdent->getOnlineDescription(), m_SearchPkt.getSearchExpression() ) )
			{
				bIsMatch = true;
			}

			break;
		}

	case eScanTypeCamServer:
		{
			if( netIdent->hasSharedWebCam() &&
				( ePluginAccessOk == netIdent->getMyAccessPermissionFromHim( ePluginTypeCamServer )) )
			{
				bIsMatch = true;
			}

			break;
		}

	case eScanTypeFileSearch:
		{
			if( ( netIdent->getSharedFileTypes() & m_SearchPkt.getSharedFileTypes() ) &&
				( ePluginAccessOk == netIdent->getMyAccessPermissionFromHim( ePluginTypeFileShareServer ) ) )
			{
				bIsMatch = true;
			}

			break;
		}

	default:
		break;
	}

	return bIsMatch;
}

//============================================================================
bool RcScan::isRemoteSearchMatch( VxNetIdent* netIdent, PktScanReq* poPkt )
{
	bool bIsMatch = false;
	switch( poPkt->getSearchType() )
	{
	case eScanTypeProfilePic:
		{
			EFriendState eAccessState = netIdent->getPluginPermission( ePluginTypeAboutMePageServer );
			if( netIdent->hasProfilePicture() 
				&& ( eFriendStateIgnore != eAccessState ) )
			{
				LogMsg( LOG_INFO, "MATCH isRemoteSearchMatch eScanTypeProfilePic %s\n", netIdent->getOnlineName() );
				bIsMatch = true;
			}
			else
			{
				LogMsg( LOG_INFO, "NO MATCH isRemoteSearchMatch eScanTypeProfilePic %s hasProfilePic %d\n", netIdent->getOnlineName(), netIdent->hasProfilePicture() );
			}

			break;
		}

	case eScanTypePeopleSearch:
		{
			if( stristr( netIdent->getOnlineName(), poPkt->getSearchExpression() ) )
			{
				bIsMatch = true;
			}

			break;
		}

	case eScanTypeMoodMsgSearch:
		{
			if( stristr( netIdent->getOnlineDescription(), poPkt->getSearchExpression() ) )
			{
				bIsMatch = true;
			}

			break;
		}

	case eScanTypeCamServer:
		{
			if( netIdent->hasSharedWebCam() )
			{
				bIsMatch = true;
				LogMsg( LOG_INFO, "MATCH isRemoteSearchMatch eScanTypeCamServer %s\n", netIdent->getOnlineName() );
			}
			else
			{
				LogMsg( LOG_INFO, "NO MATCH isRemoteSearchMatch eScanTypeCamServer %s hasSharedWebCam %d\n", netIdent->getOnlineName(), netIdent->hasSharedWebCam() );
			}

			break;
		}

	case eScanTypeFileSearch:
		{
			if( 0 != ( netIdent->getSharedFileTypes() & poPkt->getSharedFileTypes() ) )
			{
				bIsMatch = true;
			}
			else
			{
				LogMsg( LOG_INFO, "NO MATCH isRemoteSearchMatch eScanTypeFileSearch %s getSharedFileTypes 0x%x\n", netIdent->getOnlineName(), netIdent->getSharedFileTypes() );
			}

			break;
		}

	default:
		break;
	}

	return bIsMatch;
}

//============================================================================
void RcScan::searchMsgToUser( const char* msgToUser, ... )
{

}
