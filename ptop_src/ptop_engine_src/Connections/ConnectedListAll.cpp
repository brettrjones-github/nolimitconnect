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

#include "ConnectedListAll.h"
#include "ConnectedInfo.h"

#include <ptop_src/ptop_engine_src/BigListLib/BigListInfo.h>

#include <NetLib/VxSktBase.h>

//============================================================================
ConnectedListAll::ConnectedListAll( P2PEngine& engine )
    : m_Engine( engine )
{
}

//============================================================================
ConnectedInfo* ConnectedListAll::getOrAddConnectedInfo( BigListInfo * bigListInfo )
{
    if( nullptr == bigListInfo )
    {
        LogMsg( LOG_ERROR, "ConnectedListAll::getOrAddConnectedInfo bigListInfo is NULL" );
        return nullptr;
    }

    if( !bigListInfo->getMyOnlineId().isVxGUIDValid() )
    {
        LogMsg( LOG_ERROR, "ConnectedListAll::getOrAddConnectedInfo id is INVALID" );
        return nullptr;
    }

    auto iter = m_ConnectList.find( bigListInfo->getMyOnlineId() );
    if( iter != m_ConnectList.end() )
    {
        return iter->second;
    }
    else
    {
        ConnectedInfo* newInfo = new ConnectedInfo( m_Engine, bigListInfo );
        if(m_ConnectList.insert(std::make_pair(bigListInfo->getMyOnlineId(), newInfo)).second == false)
        {
            LogMsg( LOG_ERROR, "ConnectedListAll::getOrAddConnectedInfo insert FAILED" );
            return nullptr;
        }

        return newInfo;
    }
}

//============================================================================
ConnectedInfo* ConnectedListAll::getConnectedInfo( const VxGUID& onlineId )
{
    ConnectedInfo* connectedInfo = nullptr;
    auto iter = m_ConnectList.find( onlineId );
    if( iter != m_ConnectList.end() )
    {
        connectedInfo = iter->second;
        if( !connectedInfo->getSktBase() || !connectedInfo->getSktBase()->isConnected() )
        {
            connectedInfo = nullptr;
            removeConnectedInfo( onlineId );
        }
    }

    return connectedInfo;
}

//============================================================================
void ConnectedListAll::removeConnectedInfo( const VxGUID& onlineId )
{
    auto iter = m_ConnectList.find( onlineId );
    if( iter != m_ConnectList.end() )
    {
        ConnectedInfo* connectedInfo = iter->second;
        if( connectedInfo )
        {
            m_ConnectList.erase( onlineId );
            connectedInfo->aboutToDelete();
            delete connectedInfo;
        }
    }
}

//============================================================================
void ConnectedListAll::onSktDisconnected( VxSktBase* sktBase )
{
    ConnectedInfo* connectInfo = getConnectedInfo( sktBase->getPeerOnlineId() );
    if( connectInfo )
    {
        connectInfo->onSktDisconnected( sktBase );
        removeConnectedInfo( sktBase->getPeerOnlineId() );
    }
    else
    {
        // if never recieved a PktAnnounce the online id is invalid
        // remove by connectId which is always valid
        for(auto iter = m_ConnectList.begin(); iter != m_ConnectList.end(); ++iter )
        {
            ConnectedInfo* connectedInfo = iter->second;
            if( connectedInfo )
            {
                VxSktBase* connectSktBase = connectedInfo->getSktBase();
                if( connectSktBase && ( connectSktBase == sktBase || connectSktBase->getConnectionId() == sktBase->getConnectionId() ) )
                {
                    connectInfo->onSktDisconnected( sktBase );
                    m_ConnectList.erase( iter );
                    connectedInfo->aboutToDelete();
                    delete connectedInfo;
                    break;
                }
            }
        }
    }
}
