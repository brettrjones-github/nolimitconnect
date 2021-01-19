//============================================================================
// Copyright (C) 2010 Brett R. Jones
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

#include "PluginBaseRelay.h"
#include "PluginMgr.h"
#include "Relay.h"
#include "RelayServerSession.h"
#include "RelayClientSession.h"

#include <PktLib/PktsRelay.h>

#include <CoreLib/VxParse.h>
#include <CoreLib/VxGlobals.h>

//============================================================================
void PluginBaseRelay::onPktRelayServiceReply( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
	bool bSessionShouldBeClosed = false;
	PktRelayServiceReply * poPkt = (PktRelayServiceReply *)pktHdr;
	RelaySession * poSession = (RelaySession *)m_PluginSessionMgr.findPluginSessionBySessionId( poPkt->getRmtSessionId(), false );
	if( poSession )
	{
		if( eRelayStatusOk == poPkt->getRelayStatus() )
		{
			if( m_bTest )
			{
//				m_PluginMgr.pluginApiLog( m_ePluginType, "%3.3f Relay Session SUCCESS", m_TestTimer.elapsedSec() );
			}
		}
		else if( eRelayStatusPermissionErr == poPkt->getRelayStatus() )
		{
			if( m_bTest )
			{
//				m_PluginMgr.pluginApiLog( m_ePluginType, "%3.3f Relay Session Permission ERROR", m_TestTimer.elapsedSec() );
			}
			bSessionShouldBeClosed = true;
		}
		else if( eRelayStatusBusy == poPkt->getRelayStatus() )
		{
			if( m_bTest )
			{
//				m_PluginMgr.pluginApiLog( m_ePluginType, "%3.3f Relay Is Busy ERROR", m_TestTimer.elapsedSec() );
			}
			bSessionShouldBeClosed = true;
		}
		if( m_bTest && m_bVerbose )
		{
			m_bTest = false;
			m_bVerbose = false;
			bSessionShouldBeClosed = true;
		}
	}
	else
	{
		LogMsg( LOG_ERROR, "PluginBaseRelay::onEnginePktRelayServiceReply: Session NOT FOUND" );
	}
	if( bSessionShouldBeClosed )
	{
		m_PluginSessionMgr.endPluginSession( poSession, false );
	}
}
//============================================================================
void PluginBaseRelay::onPktRelayConnectReq( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
    //PktRelayConnectReq * poPkt = (PktRelayConnectReq *)pktHdr;
}

//============================================================================
void PluginBaseRelay::onPktRelayConnectReply( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
    //PktRelayConnectReply * poPkt = (PktRelayConnectReply *)pktHdr;
}

//============================================================================
void PluginBaseRelay::onPktRelaySessionReq( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
	PktRelaySessionReq * poPkt = (PktRelaySessionReq *)pktHdr;
	PktRelaySessionReply oPkt;
	oPkt.setTestOnly( poPkt->getTestOnly() );
	oPkt.setRmtSessionId( poPkt->getLclSessionId() );

	EPluginAccess eAccess = m_PluginMgr.pluginApiGetPluginAccessState( m_ePluginType, netIdent );
	oPkt.setPluginAccess(eAccess);

	if( (ePluginAccessOk == eAccess) 
		&& ( 0 == oPkt.getTestOnly() ) )
	{
		// create session
		RelayServerSession * poSession = new RelayServerSession( sktBase, netIdent );
		poSession->setOfferResponse( eOfferResponseAccept );
		poSession->setRmtSessionId( poPkt->getLclSessionId() );
		m_PluginSessionMgr.addSession( poSession->getLclSessionId(), poSession, false );
		oPkt.setLclSessionId( poSession->getLclSessionId() );
	}

	txPacket( netIdent, sktBase, &oPkt, oPkt.getTestOnly() ? true : false );
}

//============================================================================
void PluginBaseRelay::onPktRelaySessionReply( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
	PktRelaySessionReply * poPkt = (PktRelaySessionReply *)pktHdr;
	RelaySession * poSession = (RelaySession *)m_PluginSessionMgr.findPluginSessionBySessionId( poPkt->getRmtSessionId(), false );
	if( NULL == poSession )
	{
		LogMsg( LOG_ERROR, "PluginBaseRelay::onPktRelaySessionReply: Session NOT FOUND" );
		if( m_bTest && m_bVerbose )
		{
//			m_PluginMgr.pluginApiLog( m_ePluginType, "%3.3f Session NOT FOUND", m_TestTimer.elapsedSec() );
			m_bTest = false;
			m_bVerbose = false;
		}
		m_PluginMgr.pluginApiSktClose( sktBase );
	}
	else if( m_bTest && m_bVerbose )
	{
//		m_PluginMgr.pluginApiLog( m_ePluginType, "%3.3f Session SUCCESS", m_TestTimer.elapsedSec() );
		m_bTest = false;
		m_bVerbose = false;
		poSession->setRmtSessionId( poPkt->getLclSessionId() );
		poSession->signalTestSemaphore();
		m_PluginSessionMgr.endPluginSession( poSession, false );
	}
	else
	{
		poSession->setRmtSessionId( poPkt->getLclSessionId() );
		poSession->signalTestSemaphore();
	}
}

//============================================================================
void PluginBaseRelay::onPktRelayConnect( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
}

//============================================================================
void PluginBaseRelay::onPktRelayDisconnect( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
}

//============================================================================
void PluginBaseRelay::onPktRelayTestReq( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
	PktRelayTestReq * poPkt = (PktRelayTestReq *)pktHdr;
	if( netIdent->isHisAccessAllowedFromMe( m_ePluginType ) )
	{
		PktRelayTestReply pktReply;
		pktReply.setLclSessionId( poPkt->getRmtSessionId() );
		pktReply.setRmtSessionId( poPkt->getLclSessionId() );
		pktReply.setTimeStampMs( poPkt->getTimeStampMs() );
		pktReply.setMaxRelayUsers( m_MaxUserRelayAllowCnt );
		pktReply.setCurRelayUsers( m_FriendGuidList.size() );
		if( false == txPacket( netIdent, sktBase, &pktReply ) )
		{
			LogMsg( LOG_DEBUG, "PluginBaseRelay::onPktRelayTestReq FAILED sending reply to %s\n", netIdent->getOnlineName() );
		}
	}
	else
	{
		LogMsg( LOG_ERROR, "PluginBaseRelay::onPktRelayTestReq insufficient permission %s\n", netIdent->getOnlineName() );
	}

	//RelaySession * poSession = (RelaySession *)m_PluginSessionMgr.findPluginSessionBySessionId( poPkt->getRmtSessionId(), false );
	//if( poSession )
	//{
	//	poSession->signalTestSemaphore();
	//	PktRelayTestReply oPkt;
	//	oPkt.setLclSessionId( poSession->getRmtSessionId() );
	//	oPkt.setRmtSessionId( poPkt->getLclSessionId() );
	//	txPacket( poSession->getIdent(), sktBase, &oPkt );
	//}
	//else
	//{
	//	LogMsg( LOG_ERROR, "PluginBaseRelay::onPktRelayTestReq: Session NOT FOUND" );
	//}
}

//============================================================================
void PluginBaseRelay::onPktRelayTestReply( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
	PktRelayTestReply * pktTestReply = (PktRelayTestReply *)pktHdr;
	uint64_t timeNowMs = GetGmtTimeMs();
	uint32_t timeDifMs = (uint32_t)(timeNowMs - pktTestReply->getTimeStampMs() );

	std::string testMsg;
	StdStringFormat( testMsg, "Ping(%d)ms Cur Users(%d) Max Users(%d) Relay %s\n", 
						timeDifMs, 
						pktTestReply->getCurRelayUsers(),
						pktTestReply->getMaxRelayUsers(),
						netIdent->getOnlineName() );

	m_PluginMgr.pluginApiLog( m_ePluginType, testMsg.c_str() );


	//RelaySession * poSession = (RelaySession *)m_PluginSessionMgr.findPluginSessionBySessionId( poPkt->getRmtSessionId(), false );
	//if( poSession )
	//{
	//	poSession->signalTestSemaphore();
	//}
}


