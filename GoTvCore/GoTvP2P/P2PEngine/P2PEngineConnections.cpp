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
// http://www.gotvptop.com
//============================================================================

#include "P2PEngine.h"
#include "P2PConnectList.h"

#include <GoTvInterface/IToGui.h>

#include <GoTvCore/GoTvP2P/BigListLib/BigListInfo.h>
#include <GoTvCore/GoTvP2P/Plugins/PluginMgr.h>

#include <PktLib/PktsRelay.h>
#include <PktLib/PktsPing.h>

#include <NetLib/VxSktBase.h>

#include <CoreLib/VxGlobals.h>
#include <CoreLib/VxParse.h>

//============================================================================
void P2PEngine::replaceConnection( VxNetIdent * netIdent, VxSktBase * poOldSkt, VxSktBase * poNewSkt )
{
#ifdef DEBUG_CONNECTIONS
	LogMsg( LOG_INFO, "P2PEngine::replaceConnection: %s old skt %d new skt %d\n", 
		knownContactNameFromId( netIdent ),
		poOldSkt->m_iSktId,
		poNewSkt->m_iSktId );
#endif // DEBUG_CONNECTIONS
	m_RcScan.replaceConnection( netIdent, poOldSkt, poNewSkt );
	m_PluginMgr.replaceConnection( netIdent, poOldSkt, poNewSkt );
}

//============================================================================
//! socket became disconnected
void P2PEngine::onConnectionLost( VxSktBase * sktBase )								
{
#ifdef DEBUG_CONNECTIONS
	LogMsg( LOG_INFO, "P2PEngine::connectionLost: skt %d\n", sktBase->m_iSktId );
#endif // DEBUG_CONNECTIONS
	m_RcScan.onConnectionLost( sktBase );
	m_ConnectionList.onConnectionLost( sktBase );
	m_PluginMgr.onConnectionLost( sktBase );
}

//============================================================================
void P2PEngine::onContactConnected( RcConnectInfo * poInfo, bool connectionListIsLocked, bool newContact )
{
	if( ( false == VxIsAppShuttingDown() ) 
		&& poInfo->getSkt()->isConnected() ) 
	{
		poInfo->m_BigListInfo->setIsOnline( true );
		poInfo->m_BigListInfo->setIsConnected( true );
		std::string strName = poInfo->m_BigListInfo->getOnlineName();
		if( shouldNotifyGui( poInfo->m_BigListInfo ) )
		{
			//LogMsg( LOG_INFO, "toGuiContactOnline id %s name %s\n", 
			//	poInfo->m_BigListInfo->getMyOnlineId().describeVxGUID().c_str(),
			//	strName.c_str() );
			IToGui::getToGui().toGuiContactOnline( poInfo->m_BigListInfo, newContact );
		}
		//else
		//{
		//	LogMsg( LOG_INFO, "NO NOTIFY Gui of new contact id %s name %s\n", 
		//		poInfo->m_BigListInfo->getMyOnlineId().describeVxGUID().c_str(),
		//		strName.c_str() );
		//}

		if(  m_PktAnn.requiresRelay()
			&& ( false == m_ConnectionList.isMyRelayAvailable() )
			&& ( false == poInfo->m_BigListInfo->requiresRelay() ) )
		{
			onPotentialRelayServiceAvailable( poInfo, connectionListIsLocked );
		}

		m_PluginMgr.onContactWentOnline( (VxNetIdent *)poInfo->m_BigListInfo, poInfo->getSkt() );
		m_RcScan.onContactWentOnline( (VxNetIdent *)poInfo->m_BigListInfo, poInfo->getSkt() );
	}
}

//============================================================================
void P2PEngine::onContactDisconnected( RcConnectInfo * poInfo, bool connectionListLocked )
{
	if( false == VxIsAppShuttingDown() )
	{
		poInfo->m_BigListInfo->setIsOnline( false );
		poInfo->m_BigListInfo->setIsConnected( false );
		if( poInfo->m_BigListInfo->getMyOnlineId() == m_PktAnn.getRelayOnlineId() )
		{
			onRelayServiceUnavailable( poInfo, connectionListLocked );
		}

		LogMsg( LOG_INFO, "onContactDisconnected %s telling plugin mgr\n", poInfo->m_BigListInfo->getOnlineName() );
		m_RcScan.onContactWentOffline( (VxNetIdent *)poInfo->m_BigListInfo, poInfo->getSkt() );
		IToGui::getToGui().toGuiContactOffline( (VxNetIdent *)poInfo->m_BigListInfo );
		m_PluginMgr.onContactWentOffline( (VxNetIdent *)poInfo->m_BigListInfo, poInfo->getSkt() );
	}
}

//============================================================================
void P2PEngine::onContactNearby( BigListInfo * poBigListInfo )
{
	if( false == VxIsAppShuttingDown() )
	{
		if( shouldNotifyGui( poBigListInfo ) )
		{
			IToGui::getToGui().toGuiContactNearby( (VxNetIdent *)poBigListInfo );
		}
	}
}

//============================================================================
void P2PEngine::onContactNotNearby( BigListInfo * poBigListInfo )
{
	if( false == VxIsAppShuttingDown() )
	{
		if( shouldNotifyGui( poBigListInfo ) )
		{
			IToGui::getToGui().toGuiContactNotNearby( (VxNetIdent *)poBigListInfo );
		}
	}
}

