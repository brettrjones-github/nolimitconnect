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
// http://www.nolimitconnect.com
//============================================================================

#include "P2PEngine.h"
#include "P2PConnectList.h"
#include <GuiInterface/IToGui.h>

#include <ptop_src/ptop_engine_src/Network/NetworkMgr.h>
#include <ptop_src/ptop_engine_src/BigListLib/BigListInfo.h>

#include <memory.h>

//============================================================================
//! called after m_PktAnn has changed
void P2PEngine::doPktAnnHasChanged( bool connectionListIsLocked )
{
	//LogMsg( LOG_ERROR, "P2PEngine::doPktAnnHasChanged start\n" );
    setPktAnnLastModTime( GetTimeStampMs() );
	m_ConnectionList.setIsRelayRequired( m_PktAnn.requiresRelay() );
	// announce to all our new announce
	if( false == isP2POnline() )
	{
		// don't announce to users until online
		return;
	}

	m_NetworkMgr.onPktAnnUpdated();
	PktAnnounce oPktAnn;
	BigListInfo * poBigInfo;

	if( false == connectionListIsLocked )
	{
		LogMsg( LOG_ERROR, "P2PEngine::doPktAnnHasChanged m_ConnectListMutex attempt lock" );
		m_ConnectionList.connectListLock();
		LogMsg( LOG_ERROR, "P2PEngine::doPktAnnHasChanged m_ConnectListMutex lock success" );
	}

	// make copy of pkt announce 
	lockAnnouncePktAccess();
	memcpy( &oPktAnn, &m_PktAnn, sizeof(PktAnnounce) );
	unlockAnnouncePktAccess();
	oPktAnn.setIsPktAnnReplyRequested( false );

	std::map<VxGUID, RcConnectInfo *, cmp_vxguid>::iterator iter;
	for( iter = m_ConnectionList.m_ConnectList.begin(); iter != m_ConnectionList.m_ConnectList.end(); ++iter )
	{
		RcConnectInfo * poConnectInfo = iter->second;
		if( poConnectInfo )
		{
			poBigInfo = poConnectInfo->m_BigListInfo;

			// set permissions
			oPktAnn.setMyFriendshipToHim(poBigInfo->getMyFriendshipToHim());
			oPktAnn.setHisFriendshipToMe(poBigInfo->getHisFriendshipToMe());
			txSystemPkt( poBigInfo->getMyOnlineId(), poConnectInfo->m_SktBase, &oPktAnn );
		}
	}

	if( false == connectionListIsLocked )
	{
		LogMsg( LOG_INFO, "P2PEngine::doPktAnnHasChanged: m_ConnectListMutex.unlock()" );
		m_ConnectionList.connectListUnlock();
	}

	//LogMsg( LOG_INFO, "P2PEngine::doPktAnnHasChanged done\n" );
}

//============================================================================
//! pkt ann has changed and needs to be re announced
void P2PEngine::doPktAnnConnectionInfoChanged( bool connectionListIsLocked )
{
	doPktAnnHasChanged( connectionListIsLocked );
	IToGui::getToGui().toGuiUpdateMyIdent( &m_PktAnn );
}
