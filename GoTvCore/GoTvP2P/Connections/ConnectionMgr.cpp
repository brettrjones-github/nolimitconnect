//============================================================================
// Copyright (C) 2021 Brett R. Jones 
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

#include "ConnectionMgr.h"
#include "ConnectedInfo.h"

#include <GoTvCore/GoTvP2P/P2PEngine/P2PEngine.h>

//============================================================================
ConnectionMgr::ConnectionMgr( P2PEngine& engine )
    : m_Engine( engine )
    , m_BigListMgr( engine.getBigListMgr() )
    , m_AllList( engine )
{
}

//============================================================================
bool ConnectionMgr::onSktConnectedWithPktAnn( VxSktBase* sktBase, BigListInfo * bigListInfo )
{
    bool result = true;
    m_ConnectionMutex.lock();
    ConnectedInfo* connectInfo = m_AllList.getOrAddConnectedInfo( bigListInfo );
    if( nullptr == connectInfo )
    {
        result = false;
        LogMsg( LOG_ERROR, "ConnectionMgr get connection info FAILED" );
    }
    else
    {
        connectInfo->onSktConnected( sktBase );
    }

    m_ConnectionMutex.unlock();
    return result;
}

//============================================================================
void ConnectionMgr::onSktDisconnected( VxSktBase* sktBase )
{
    m_ConnectionMutex.lock();
    m_AllList.onSktDisconnected( sktBase );
    m_ConnectionMutex.unlock();
}
