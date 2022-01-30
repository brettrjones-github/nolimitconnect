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

#include "OnlineListMgr.h"
#include "OnlineListCallbackInterface.h"

#include <ptop_src/ptop_engine_src/P2PEngine/P2PEngine.h>
#include <ptop_src/ptop_engine_src/BigListLib/BigListInfo.h>

//============================================================================
OnlineListMgr::OnlineListMgr( P2PEngine& engine )
    : OnlineSktList( engine )
{
    setIdentListType( eUserViewTypeOnline );
}

//============================================================================
void OnlineListMgr::onUserOnlineStatusChange( VxGUID& onlineId, bool isOnline )
{
    announceOnlineStatus( onlineId, isOnline );
}

//============================================================================
void OnlineListMgr::onUserOnlineStatusChange( GroupieId& groupieId, bool isOnline )
{
    announceOnlineStatus( groupieId.getGroupieOnlineId(), isOnline );
}

//============================================================================
void OnlineListMgr::wantOnlineCallback( OnlineListCallbackInterface* client, bool enable )
{
    lockClientList();
    for( auto iter = m_CallbackClients.begin(); iter != m_CallbackClients.end(); ++iter )
    {
        if( *iter == client )
        {
            m_CallbackClients.erase( iter );
            break;
        }
    }

    if( enable )
    {
        m_CallbackClients.push_back( client );
    }

    unlockClientList();
}

//============================================================================
void OnlineListMgr::announceOnlineStatus( VxGUID& onlineId, bool isOnline )
{
    lockClientList();

    for( auto iter = m_CallbackClients.begin(); iter != m_CallbackClients.end(); ++iter )
    {
        OnlineListCallbackInterface* client = *iter;
        client->callbackOnlineStatusChange( onlineId, isOnline );
    }

    unlockClientList();
}
