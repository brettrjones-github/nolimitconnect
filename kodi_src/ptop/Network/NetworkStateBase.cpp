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

#include "NetworkStateBase.h"
#include "NetworkStateMachine.h"

#include <GoTvCore/GoTvP2P/P2PEngine/P2PEngine.h>

//============================================================================
NetworkStateBase::NetworkStateBase( NetworkStateMachine& stateMachine )
: m_Engine( stateMachine.getEngine() )
, m_PktAnn( stateMachine.getEngine().getMyPktAnnounce() )
, m_NetServicesMgr( stateMachine.getEngine().getNetServicesMgr() )
, m_NetworkStateMachine( stateMachine )
, m_ENetworkStateType( eNetworkStateTypeUnknown )
{
}

//============================================================================
bool NetworkStateBase::checkForAbortOrShutdown( void )
{
	return m_NetworkStateMachine.checkForAbortOrShutdown();
}

//============================================================================
void NetworkStateBase::checkAndHandleNetworkEvents( void )
{
	m_NetworkStateMachine.checkAndHandleNetworkEvents();
}
