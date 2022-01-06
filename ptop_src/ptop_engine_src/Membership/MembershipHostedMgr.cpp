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

#include "MembershipHostedMgr.h"
#include "MembershipHostedCallbackInterface.h"
#include <ptop_src/ptop_engine_src/P2PEngine/P2PEngine.h>

//============================================================================
MembershipHostedMgr::MembershipHostedMgr( P2PEngine& engine )
    : MembershipMgrBase( engine )
{
}

//============================================================================
void MembershipHostedMgr::updateMemberHosted( VxGUID& sktConnectId, VxGUID& onlineId, MembershipHosted& memberHosted )
{
    if( !sktConnectId.isVxGUIDValid() )
    {
        LogMsg( LOG_ERROR, "MembershipHostedMgr::updateSktIdent invalid skt id" );
        return;
    }

    if( !onlineId.isVxGUIDValid() )
    {
        LogMsg( LOG_ERROR, "MembershipHostedMgr::updateSktIdent invalid id" );
        return;
    }

    if( onlineId == m_Engine.getMyOnlineId() )
    {
        LogMsg( LOG_ERROR, "MembershipHostedMgr::updateSktIdent cannot member myself" );
        return;
    }

    bool wasUpdated = false;
    lockList();
    for( auto iter = m_MemberList.begin(); iter != m_MemberList.end(); ++iter )
    {
        if( iter->first == onlineId && iter->second.first == sktConnectId )
        {
            iter->second.second = memberHosted;
            wasUpdated = true;
            break;
        }
    }

    if( !wasUpdated )
    {
        m_MemberList.push_back( std::make_pair( onlineId, std::make_pair( sktConnectId, memberHosted ) ) );
    }

    unlockList();

    if( !wasUpdated )
    {
        onMembershipUpdated( onlineId, sktConnectId, memberHosted );
    }
    else
    {
        onMembershipAdded( onlineId, sktConnectId, memberHosted );
    }
}

//============================================================================
void MembershipHostedMgr::removeConnection( VxGUID& sktConnectId )
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
void MembershipHostedMgr::removeConnection( VxGUID& onlineId, VxGUID& sktConnectId )
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

    MembershipHosted memberHosted;
    VxGUID sktId;
    bool updateMembership = false;
    if( wasRemoved )
    {
        lockList();
        for( auto iter = m_MemberList.begin(); iter != m_MemberList.end(); ++iter )
        {
            if( iter->first == onlineId )
            {
                sktId = iter->second.first;
                memberHosted = iter->second.second;
                updateMembership = true;
                break;
            }
        }

        unlockList();
    }

    if( updateMembership )
    {
        onMembershipUpdated( onlineId, sktId, memberHosted );
    }
    else
    {
        onMembershipRemoved( onlineId );
    }
}


//============================================================================
void MembershipHostedMgr::removeIdent( VxGUID& onlineId )
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
void MembershipHostedMgr::removeAll( void )
{
    lockList();
    // make copy
    std::vector< std::pair<VxGUID, std::pair<VxGUID, MembershipHosted>>> memberList = m_MemberList;
    unlockList();

    for( auto iter = memberList.begin(); iter != memberList.end(); ++iter )
    {
        removeIdent( iter->first );
    }
}

//============================================================================
void MembershipHostedMgr::onMembershipAdded( VxGUID& onlineId, VxGUID& sktConnectId, MembershipHosted& memberHosted )
{
    announceMembershipAdded( onlineId, sktConnectId, memberHosted );
}

//============================================================================
void MembershipHostedMgr::onMembershipUpdated( VxGUID& onlineId, VxGUID& sktConnectId, MembershipHosted& memberHosted )
{
    announceMembershipUpdated( onlineId, sktConnectId, memberHosted );
}

//============================================================================
void MembershipHostedMgr::onMembershipRemoved( VxGUID& onlineId )
{
    announceMembershipRemoved( onlineId );
}

//============================================================================
void MembershipHostedMgr::addMemberClient( MembershipHostedCallbackInterface* client, bool enable )
{
    lockClientList();
    if( enable )
    {
        m_MemberClients.push_back( client );
    }
    else
    {
        std::vector<MembershipHostedCallbackInterface*>::iterator iter;
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
void MembershipHostedMgr::announceMembershipAdded( VxGUID& onlineId, VxGUID& sktConnectId, MembershipHosted& memberHosted )
{
    LogMsg( LOG_INFO, "UserOnlineMgr::announceMembershipAdded start" );
    lockClientList();
    std::vector<MembershipHostedCallbackInterface*>::iterator iter;
    for( iter = m_MemberClients.begin(); iter != m_MemberClients.end(); ++iter )
    {
        MembershipHostedCallbackInterface* client = *iter;
        client->callbackMembershipHostedAdded( onlineId, sktConnectId, memberHosted );
    }

    unlockClientList();
    LogMsg( LOG_INFO, "UserOnlineMgr::announceMembershipAdded done" );
}

//============================================================================
void MembershipHostedMgr::announceMembershipUpdated( VxGUID& onlineId, VxGUID& sktConnectId, MembershipHosted& memberHosted )
{
    LogMsg( LOG_INFO, "UserOnlineMgr::announceMembershipUpdated start" );
    lockClientList();
    std::vector<MembershipHostedCallbackInterface*>::iterator iter;
    for( iter = m_MemberClients.begin(); iter != m_MemberClients.end(); ++iter )
    {
        MembershipHostedCallbackInterface* client = *iter;
        client->callbackMembershipHostedUpdated( onlineId, sktConnectId, memberHosted );
    }

    unlockClientList();
    LogMsg( LOG_INFO, "UserOnlineMgr::announceMembershipUpdated done" );
}

//============================================================================
void MembershipHostedMgr::announceMembershipRemoved( VxGUID& onlineId )
{
    lockClientList();
    std::vector<MembershipHostedCallbackInterface*>::iterator iter;
    for( iter = m_MemberClients.begin(); iter != m_MemberClients.end(); ++iter )
    {
        MembershipHostedCallbackInterface* client = *iter;
        client->callbackMembershipHostedRemoved( onlineId );
    }

    unlockClientList();
}
