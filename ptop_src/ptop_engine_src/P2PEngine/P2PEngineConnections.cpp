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
// http://www.nolimitconnect.org
//============================================================================

#include "P2PEngine.h"
#include "P2PConnectList.h"

#include <GuiInterface/IToGui.h>

#include <ptop_src/ptop_engine_src/BigListLib/BigListInfo.h>
#include <ptop_src/ptop_engine_src/Plugins/PluginMgr.h>
#include <ptop_src/ptop_engine_src/HostJoinMgr/HostJoinMgr.h>
#include <ptop_src/ptop_engine_src/UserJoinMgr/UserJoinMgr.h>
#include <ptop_src/ptop_engine_src/UserOnlineMgr/UserOnlineMgr.h>
#include <ptop_src/ptop_engine_src/Network/NetworkMgr.h>

#include <PktLib/PktsRelay.h>
#include <PktLib/PktsPing.h>

#include <NetLib/VxSktBase.h>

#include <CoreLib/VxGlobals.h>
#include <CoreLib/VxParse.h>

//============================================================================
void P2PEngine::replaceConnection( VxNetIdent * netIdent, VxSktBase * poOldSkt, VxSktBase * poNewSkt )
{
	LogModule( eLogConnect, LOG_VERBOSE, "P2PEngine::replaceConnection: old skt %d new skt %d handle %d",
				poOldSkt->m_SktNumber,
				poNewSkt->m_SktNumber,
				poNewSkt->m_Socket );

	m_RcScan.replaceConnection( netIdent, poOldSkt, poNewSkt );
	m_PluginMgr.replaceConnection( netIdent, poOldSkt, poNewSkt );
}

//============================================================================
//! socket became disconnected
void P2PEngine::onConnectionLost( VxSktBase * sktBase )								
{
	LogModule( eLogConnect, LOG_VERBOSE, "P2PEngine::connectionLost: skt %d", sktBase->m_SktNumber );
	getConnectIdListMgr().onConnectionLost( sktBase->getSocketId() );

	getHostJoinMgr().onConnectionLost( sktBase, sktBase->getSocketId(), sktBase->getPeerOnlineId() );
	getUserOnlineMgr().onConnectionLost( sktBase, sktBase->getSocketId(), sktBase->getPeerOnlineId() );
	getUserJoinMgr().onConnectionLost( sktBase, sktBase->getSocketId(), sktBase->getPeerOnlineId() );
	getNetworkMgr().getNearbyMgr().onConnectionLost( sktBase, sktBase->getSocketId(), sktBase->getPeerOnlineId() );
	
	m_RcScan.onConnectionLost( sktBase );
	m_ConnectionList.onConnectionLost( sktBase );
    if( sktBase->getIsPeerPktAnnSet() )
    {
        getConnectionMgr().onSktDisconnected( sktBase );
    }

	m_PluginMgr.onConnectionLost( sktBase );
}

//============================================================================
void P2PEngine::onContactConnected( RcConnectInfo * poInfo, bool connectionListIsLocked, bool newContact )
{
	if( ( false == VxIsAppShuttingDown() ) 
		&& poInfo->getSkt()->isConnected() ) 
	{
		poInfo->m_BigListInfo->setIsConnected( true );
		std::string strName = poInfo->m_BigListInfo->getOnlineName();
		if( !poInfo->getSkt()->isTempConnection() && shouldNotifyGui( poInfo->m_BigListInfo ) )
		{
			//LogMsg( LOG_INFO, "toGuiContactOnline id %s name %s\n", 
			//	poInfo->m_BigListInfo->getMyOnlineId().describeVxGUID().c_str(),
			//	strName.c_str() );
			IToGui::getToGui().toGuiContactOnline( poInfo->m_BigListInfo->getVxNetIdent() );
		}
		//else
		//{
		//	LogMsg( LOG_INFO, "NO NOTIFY Gui of new contact id %s name %s\n", 
		//		poInfo->m_BigListInfo->getMyOnlineId().describeVxGUID().c_str(),
		//		strName.c_str() );
		//}


		m_PluginMgr.onContactWentOnline( (VxNetIdent *)poInfo->m_BigListInfo, poInfo->getSkt() );
		m_RcScan.onContactWentOnline( (VxNetIdent *)poInfo->m_BigListInfo, poInfo->getSkt() );
	}
}

//============================================================================
void P2PEngine::onContactDisconnected( RcConnectInfo * poInfo, bool connectionListLocked )
{
	if( false == VxIsAppShuttingDown() )
	{
		getNetStatusAccum().onConnectionLost( poInfo->getSkt()->getSocketId() );
		poInfo->m_BigListInfo->setIsConnected( false );

		LogMsg( LOG_INFO, "onContactDisconnected %s telling plugin mgr", poInfo->m_BigListInfo->getOnlineName() );
		m_RcScan.onContactWentOffline( poInfo->m_BigListInfo->getVxNetIdent(), poInfo->getSkt() );
		IToGui::getToGui().toGuiContactOffline( poInfo->m_BigListInfo->getVxNetIdent() );
		m_PluginMgr.onContactWentOffline( poInfo->m_BigListInfo->getVxNetIdent(), poInfo->getSkt() );
	}
}
