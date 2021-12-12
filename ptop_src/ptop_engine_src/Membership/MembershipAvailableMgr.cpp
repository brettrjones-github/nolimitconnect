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

#include "MembershipAvailableMgr.h"
#include "MembershipAvailableCallbackInterface.h"
#include <ptop_src/ptop_engine_src/P2PEngine/P2PEngine.h>

//============================================================================
MembershipAvailableMgr::MembershipAvailableMgr( P2PEngine& engine )
    : MembershipMgrBase( engine )
{
}

//============================================================================
void MembershipAvailableMgr::updateMemberAvail( VxGUID& sktConnectId, VxGUID& onlineId, MembershipAvailable& memberAvail )
{
    if( !sktConnectId.isVxGUIDValid() )
    {
        LogMsg( LOG_ERROR, "MembershipAvailableMgr::updateSktIdent invalid skt id" );
        return;
    }

    if( !onlineId.isVxGUIDValid() )
    {
        LogMsg( LOG_ERROR, "MembershipAvailableMgr::updateSktIdent invalid id" );
        return;
    }

    if( onlineId == m_Engine.getMyOnlineId() )
    {
        LogMsg( LOG_ERROR, "MembershipAvailableMgr::updateSktIdent cannot member myself" );
        return;
    }

    bool wasUpdated = false;
    lockList();
    for( auto iter = m_MemberList.begin(); iter != m_MemberList.end(); ++iter )
    {
        if( iter->first == onlineId && iter->second.first == sktConnectId )
        {
            iter->second.second = memberAvail;
            wasUpdated = true;
            break;
        }
    }

    if( !wasUpdated )
    {
        m_MemberList.push_back( std::make_pair( onlineId, std::make_pair( sktConnectId, memberAvail ) ) );
    }

    unlockList();

    if( !wasUpdated )
    {
        onMembershipUpdated( onlineId, sktConnectId, memberAvail );
    }
    else
    {
        onMembershipAdded( onlineId, sktConnectId, memberAvail );
    }
}

//============================================================================
void MembershipAvailableMgr::removeConnection( VxGUID& sktConnectId )
{
    std::vector<VxGUID> identList;
    lockList();
    for( auto iter = m_MemberList.begin(); iter != m_MemberList.end(); ++iter )
    {
        if( iter->second.first == sktConnectId )
        {
            identList.push_back( iter->first );
        }
    }

    unlockList();

    for( auto onlineId : identList )
    {
        removeConnection( onlineId, sktConnectId );
    }
}

//============================================================================
void MembershipAvailableMgr::removeConnection( VxGUID& onlineId, VxGUID& sktConnectId )
{
    bool wasRemoved = false;
    lockList();
    for( auto iter = m_MemberList.begin(); iter != m_MemberList.end(); ++iter )
    {
        if( iter->first == onlineId && iter->second.first == sktConnectId )
        {
            m_MemberList.erase( iter );
            wasRemoved = true;
            break;
        }
    }

    unlockList();

    MembershipAvailable memberAvail;
    VxGUID sktId;
    bool updateAvailable = false;
    if( wasRemoved )
    {
        lockList();
        for( auto iter = m_MemberList.begin(); iter != m_MemberList.end(); ++iter )
        {
            if( iter->first == onlineId )
            {
                sktId = iter->second.first;
                memberAvail = iter->second.second;
                updateAvailable = true;
                break;
            }
        }

        unlockList();
    }

    if( updateAvailable )
    {
        onMembershipUpdated( onlineId, sktId, memberAvail );
    }
    else
    {
        onMembershipRemoved( onlineId );
    } 
}


//============================================================================
void MembershipAvailableMgr::removeIdent( VxGUID& onlineId )
{
    bool wasErased = false;
    lockList();
    for( auto iter = m_MemberList.begin(); iter != m_MemberList.end(); )
    {
        if( iter->first == onlineId )
        {
            iter = m_MemberList.erase( iter );
            wasErased = true;
        }
        else
        {
            ++iter;
        }
    }

    unlockList();
    if( wasErased )
    {
        onMembershipRemoved( onlineId );
    }
}

//============================================================================
void MembershipAvailableMgr::removeAll( void )
{
    lockList();
    // make copy
    std::vector< std::pair<VxGUID, std::pair<VxGUID, MembershipAvailable>>> memberList = m_MemberList;
    unlockList();

    for( auto iter = memberList.begin(); iter != memberList.end(); ++iter )
    {
        removeIdent( iter->first );
    }
}

//============================================================================
void MembershipAvailableMgr::onMembershipAdded( VxGUID& onlineId, VxGUID& sktConnectId, MembershipAvailable& memberAvail )
{
    announceMembershipAdded( onlineId, sktConnectId, memberAvail );
}

//============================================================================
void MembershipAvailableMgr::onMembershipUpdated( VxGUID& onlineId, VxGUID& sktConnectId, MembershipAvailable& memberAvail )
{
    announceMembershipUpdated( onlineId, sktConnectId, memberAvail );
}

//============================================================================
void MembershipAvailableMgr::onMembershipRemoved( VxGUID& onlineId )
{
    announceMembershipRemoved( onlineId );
}

//============================================================================
void MembershipAvailableMgr::addMemberClient( MembershipAvailableCallbackInterface* client, bool enable )
{
    lockClientList();
    if( enable )
    {
        m_MemberClients.push_back( client );
    }
    else
    {
        std::vector<MembershipAvailableCallbackInterface*>::iterator iter;
        for( iter = m_MemberClients.begin(); iter != m_MemberClients.end(); ++iter )
        {
            if( *iter == client )
            {
                m_MemberClients.erase( iter );
                break;
            }
        }
    }

    unlockClientList();
}

//============================================================================
void MembershipAvailableMgr::announceMembershipAdded( VxGUID& onlineId, VxGUID& sktConnectId, MembershipAvailable& memberAvail )
{
    LogMsg( LOG_INFO, "MembershipAvailableMgr::announceMembershipAdded start" );
    lockClientList();
    std::vector<MembershipAvailableCallbackInterface*>::iterator iter;
    for( iter = m_MemberClients.begin(); iter != m_MemberClients.end(); ++iter )
    {
        MembershipAvailableCallbackInterface* client = *iter;
        client->callbackMembershipAvailableAdded( onlineId, sktConnectId, memberAvail );
    }

    unlockClientList();
    LogMsg( LOG_INFO, "MembershipAvailableMgr::announceMembershipAdded done" );
}

//============================================================================
void MembershipAvailableMgr::announceMembershipUpdated( VxGUID& onlineId, VxGUID& sktConnectId, MembershipAvailable& memberAvail )
{
    LogMsg( LOG_INFO, "MembershipAvailableMgr::announceMembershipUpdated start" );
    lockClientList();
    std::vector<MembershipAvailableCallbackInterface*>::iterator iter;
    for( iter = m_MemberClients.begin(); iter != m_MemberClients.end(); ++iter )
    {
        MembershipAvailableCallbackInterface* client = *iter;
        client->callbackMembershipAvailableUpdated( onlineId, sktConnectId, memberAvail );
    }

    unlockClientList();
    LogMsg( LOG_INFO, "MembershipAvailableMgr::announceMembershipUpdated done" );
}

//============================================================================
void MembershipAvailableMgr::announceMembershipRemoved( VxGUID& onlineId )
{
    lockClientList();
    std::vector<MembershipAvailableCallbackInterface*>::iterator iter;
    for( iter = m_MemberClients.begin(); iter != m_MemberClients.end(); ++iter )
    {
        MembershipAvailableCallbackInterface* client = *iter;
        client->callbackMembershipAvailableRemoved( onlineId );
    }

    unlockClientList();
}
