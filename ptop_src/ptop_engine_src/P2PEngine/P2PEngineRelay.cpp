//============================================================================
// Copyright (C) 2009 Brett R. Jones 
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

#include "P2PEngine.h"
#include "P2PConnectList.h"

#include <GuiInterface/IToGui.h>

#include <ptop_src/ptop_engine_src/Plugins/PluginServiceRelay.h>
#include <ptop_src/ptop_engine_src/Plugins/PluginMgr.h>
#include <ptop_src/ptop_engine_src/Network/NetConnector.h>
#include <ptop_src/ptop_engine_src/Network/NetworkStateMachine.h>

#include <ptop_src/ptop_engine_src/BigListLib/BigListInfo.h>
#include <NetLib/VxSktBase.h>
#include <PktLib/PktsRelay.h>
#include <PktLib/PktTcpPunch.h>

#include <CoreLib/VxParse.h>

//============================================================================
void P2PEngine::onPotentialRelayServiceAvailable( RcConnectInfo * poConnection, bool connectionListIsLocked )
{
	getConnectList().onPotentialRelayServiceAvailable( poConnection, connectionListIsLocked );
}

//============================================================================
void P2PEngine::onRelayServiceAvailable( RcConnectInfo * poConnection, bool connectionListIsLocked )
{
	if( m_PktAnn.requiresRelay() )
	{
		LogMsg( LOG_STATUS, "Using Relay Service Available from %s", poConnection->m_BigListInfo->getOnlineName() );
		// our proxy service is now connected
		// if proxy info in announcement packet is not this server then
		// update PktAnn
		BigListInfo * poBigInfo = poConnection->m_BigListInfo;

		LogMsg( LOG_STATUS, "eMyRelayStatusConnected %s", poConnection->m_BigListInfo->getOnlineName() );
		getToGui().toGuiMyRelayStatus( eMyRelayStatusConnected, poConnection->m_BigListInfo->getOnlineName() );
		m_NetworkStateMachine.setIsRelayServiceConnected( true );
	}
#ifdef DEBUG_RELAY
	else
	{
		LogMsg( LOG_INFO, "WE DONT REQUIRE Relay from %s\n", poConnection->m_BigListInfo->getOnlineName() );
	}
#endif // DEBUG_RELAY
}

//============================================================================
void P2PEngine::onRelayServiceUnavailable( RcConnectInfo * poConnection, bool connectionListIsLocked )
{
#ifdef DEBUG_RELAY
	LogMsg( LOG_INFO, "Relay Service from %s Unavailable\n", poConnection->m_BigListInfo->getOnlineName() );
#endif // DEBUG_RELAY

	if( false == connectionListIsLocked )
	{
		#ifdef DEBUG_MUTEXES
			LogMsg( LOG_INFO, "onRelayServiceUnavailable: m_ConnectListMutex.lock()\n" );
		#endif // DEBUG_MUTEXES
		m_ConnectionList.connectListLock();
	}

	std::vector<RcConnectInfo *>::iterator iter = m_ConnectionList.m_RelayServerConnectedList.begin();
	while( iter != m_ConnectionList.m_RelayServerConnectedList.end() )
	{
		if( (*iter) == poConnection )
		{
			iter = m_ConnectionList.m_RelayServerConnectedList.erase(iter);
		}
		else
		{
			++iter;
		}
	}

	if( false == connectionListIsLocked )
	{
#ifdef DEBUG_MUTEXES
		LogMsg( LOG_INFO, "onRelayServiceUnavailable: m_ConnectListMutex.unlock()\n" );
#endif // DEBUG_MUTEXES
		m_ConnectionList.connectListUnlock();
	}

	if( poConnection == m_ConnectionList.m_RelayServiceConnection )
	{
#ifdef DEBUG_RELAY
		LogMsg( LOG_INFO, "Our Relay Service from %s Disconnected\n", poConnection->m_BigListInfo->getOnlineName() );
#endif // DEBUG_RELAY
		m_ConnectionList.m_RelayServiceConnection = NULL;
		IToGui::getToGui().toGuiMyRelayStatus( eMyRelayStatusDisconnected );
		m_NetworkStateMachine.setIsRelayServiceConnected( false );
	}
#ifdef DEBUG_RELAY
	else
	{
		LogMsg( LOG_INFO, "Removed Unused Relay Service from %s \n", poConnection->m_BigListInfo->getOnlineName() );
	}
#endif // DEBUG_RELAY
}

//============================================================================
void P2PEngine::onPktRelayServiceReq( VxSktBase * sktBase, VxPktHdr * pktHdr )
{
	PktRelayServiceReq * pktReq = ( PktRelayServiceReq * )pktHdr;
	PktRelayServiceReply pktReply;
	pktReply.setAccessState( ePluginAccessLocked );
	VxGUID srcOnlineId = pktHdr->getSrcOnlineId();

#ifdef DEBUG_MUTEXES
	LogMsg( LOG_INFO, "P2PEngine::onPktRelayServiceReq: m_ConnectListMutex.lock()\n" );
#endif // DEBUG_MUTEXES
	BigListInfo * bigListInfo = getBigListMgr().findBigListInfo( srcOnlineId );
	if( 0 != bigListInfo )
	{
		PluginBase * pluginBase = m_PluginMgr.findPlugin( ePluginTypeRelay );
		if( 0 != pluginBase )
		{
			EPluginAccess eAccess = ( ( PluginServiceRelay * )pluginBase)->handlePktRelayServiceReq( bigListInfo, sktBase, pktReq, pktReply );
			pktReply.setAccessState( eAccess );
		}

#ifdef DEBUG_RELAY
		else
		{
			LogMsg( LOG_ERROR, "onPktRelayServiceReq %s Connection NOT FOUND\n", srcOnlineId.toHexString().c_str() );
		}
#endif // DEBUG_RELAY
	}
	else
	{
		LogMsg( LOG_ERROR, "onPktRelayServiceReq NO BIG LIST INFO\n", srcOnlineId.toHexString().c_str() );
	}

	txSystemPkt( pktHdr->getSrcOnlineId(), sktBase, &pktReply );
}

//============================================================================
void P2PEngine::onPktRelayServiceReply( VxSktBase * sktBase, VxPktHdr * pktHdr )
{
	m_NetworkStateMachine.onPktRelayServiceReply( sktBase, (PktRelayServiceReply *)pktHdr );
}

//============================================================================
void P2PEngine::onPktRelayConnectToUserReq( VxSktBase * sktBase, VxPktHdr * pktHdr )
{
	PktRelayConnectToUserReply oPkt;
	PktRelayConnectToUserReq * poPkt = (PktRelayConnectToUserReq *)pktHdr;
#ifdef DEBUG_CONNECT_MUTEXES
	LogMsg( LOG_INFO, "P2PEngine::onPktRelayConnectToUserReq: m_ConnectListMutex.lock()\n" );
#endif // DEBUG_CONNECT_MUTEXES
	m_ConnectionList.connectListLock();
	RcConnectInfo * poDestConnectInfo = m_ConnectionList.findConnection( poPkt->m_ConnectId.getOnlineId(), true );
	if( poDestConnectInfo )
	{
		oPkt.setConnectFailed( false );
#ifdef DEBUG_RELAY
		//LogMsg( LOG_INFO, "onPktRelayConnectToUserReq: SUCCESS from %s id %s to %s id %s\n",
		//	knownContactNameFromId( poPkt->getSrcOnlineId() ),
		//	poPkt->getSrcOnlineId().describeVxGUID().c_str(),
		//	knownContactNameFromId( poPkt->m_ConnectId.getOnlineId() ),
		//	poPkt->m_ConnectId.getOnlineId().describeVxGUID().c_str() );
#endif // DEBUG_RELAY
	}
	else
	{
		oPkt.setConnectFailed( true );
		//LogMsg( LOG_INFO, "onPktRelayConnectToUserReq: FAIL from %s id %s to %s id %s\n",
		//	knownContactNameFromId( poPkt->getSrcOnlineId() ),
		//	poPkt->getSrcOnlineId().describeVxGUID().c_str(),
		//	knownContactNameFromId( poPkt->m_ConnectId.getOnlineId() ),
		//	poPkt->m_ConnectId.getOnlineId().describeVxGUID().c_str() );
	}

	oPkt.m_ConnectId = poPkt->m_ConnectId;
	RcConnectInfo * poCallerConnectInfo = m_ConnectionList.findConnection(poPkt->getSrcOnlineId(), true );
	if( poCallerConnectInfo )
	{
		poCallerConnectInfo->setIsRelayClient( true );
		// before we send back the response we want to send tcp punch so the caller's thread is not busy
		if( ( 0 == oPkt.getConnectFailed() )
			&& poDestConnectInfo->getBigListInfo()->requiresRelay()
			&& poCallerConnectInfo->getBigListInfo()->requiresRelay() )
		{
			// try tcp style stun
			PktTcpPunch pktCallerPunch;
			pktCallerPunch.m_ConnectInfo = poDestConnectInfo->getBigListInfo()->getConnectInfo();
			PktTcpPunch pktDestPunch;
			pktDestPunch.m_ConnectInfo = poCallerConnectInfo->getBigListInfo()->getConnectInfo();
			if( poDestConnectInfo->getSkt() )
			{
				pktCallerPunch.setSrcOnlineId( m_PktAnn.getMyOnlineId() );
				pktDestPunch.setSrcOnlineId( m_PktAnn.getMyOnlineId() );
				poDestConnectInfo->getSkt()->txPacket( pktDestPunch.m_ConnectInfo.getMyOnlineId(), &pktDestPunch );
				sktBase->txPacket( pktCallerPunch.m_ConnectInfo.getMyOnlineId(), &pktCallerPunch );
			}
		}

		txSystemPkt( poCallerConnectInfo->m_BigListInfo, sktBase, &oPkt);
	}

#ifdef DEBUG_CONNECT_MUTEXES
	LogMsg( LOG_INFO, "P2PEngine::onPktRelayConnectToUserReq: m_ConnectListMutex.unlock()\n" );
#endif // DEBUG_CONNECT_MUTEXES

	m_ConnectionList.connectListUnlock();
}

//============================================================================
void P2PEngine::onPktRelayConnectToUserReply( VxSktBase * sktBase, VxPktHdr * pktHdr )
{
	PktRelayConnectToUserReply * poPkt = (PktRelayConnectToUserReply *)pktHdr;
	BigListInfo* bigListInfo = m_BigListMgr.findBigListInfo( poPkt->m_ConnectId.getOnlineId() );
	if( bigListInfo )
	{
		getRelayMgr().onPktRelayConnectToUserReply( sktBase, pktHdr, bigListInfo->getVxNetIdent() );
	}

	/*
	if( 0 == poPkt->getConnectFailed() )
	{
//#ifdef DEBUG_RELAY
//		LogMsg( LOG_INFO, "onPktRelayConnectToUserReply: SUCCESS proxy %s id %s to %s id %s\n",
//								knownContactNameFromId( poPkt->getSrcOnlineId() ),
//								poPkt->getSrcOnlineId().describeVxGUID().c_str(),
//								knownContactNameFromId( poPkt->m_ConnectId.getOnlineId() ),
//								poPkt->m_ConnectId.getOnlineId().describeVxGUID().c_str() );
//#endif // DEBUG_RELAY
		m_NetConnector.sendMyPktAnnounce(  poPkt->m_ConnectId.getOnlineId(), sktBase, true );
		BigListInfo * bigListInfo = m_BigListMgr.findBigListInfo(poPkt->m_ConnectId.getOnlineId());
		if( 0 != bigListInfo )
		{
			LogMsg( LOG_INFO, "sendMyPktAnnounce 8\n" ); 
			// send our announce to rmt user through proxy
			m_ConnectionList.addConnection( sktBase, bigListInfo, false );
		}
	}
	else
	{
		// failed to connect.. remove if have connection
		//removeConnectionIfNoLongerUsed( poPkt->m_ConnectId.getOnlineId() );

		std::string strIp;
		poPkt->m_ConnectId.getIpAddress(strIp);

		std::string strFromId;
		poPkt->getSrcOnlineId().toHexString( strFromId );

		std::string connectToId;
		poPkt->m_ConnectId.getOnlineId().toHexString( connectToId );

//#ifdef DEBUG_RELAY
//		LogMsg( LOG_INFO, "onPktRelayConnectToUserReply: FAIL proxy %s id %s to %s id %s ip %s port %d\n",
//			knownContactNameFromId( poPkt->getSrcOnlineId() ),
//			poPkt->getSrcOnlineId().describeVxGUID().c_str(),
//			knownContactNameFromId( poPkt->m_ConnectId.getOnlineId() ),
//			poPkt->m_ConnectId.getOnlineId().describeVxGUID().c_str(),
//			strIp.c_str(), 
//			poPkt->m_ConnectId.getPort() );
//#endif // DEBUG_RELAY
	}
	//LogMsg( LOG_INFO, "onPktRelayConnectToUserReply: signaling\n" );
	//sktBase->m_EventSemaphore.signal();
	*/
}

//============================================================================
void P2PEngine::onPktRelayUserDisconnect( VxSktBase * sktBase, VxPktHdr * pktHdr )
{
	PktRelayUserDisconnect * poPkt = (PktRelayUserDisconnect *)pktHdr;
	BigListInfo * poBigInfo = m_BigListMgr.findBigListInfo(poPkt->m_UserId);
	if( poBigInfo )
	{
//#ifdef DEBUG_RELAY
//		LogMsg( LOG_INFO, "onPktRelayUserDisconnect: USER %s disconnected from relay %s\n",
//			poBigInfo->getOnlineName(),
//			knownContactNameFromId( poPkt->getSrcOnlineId() ) );
//#endif // DEBUG_RELAY

		m_ConnectionList.removeConnection( poPkt->m_UserId );
	}
}
