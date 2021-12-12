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

#include "GuiMembershipAvailableMgr.h"
#include "GuiMembershipAvailableCallback.h"
#include "GuiHelpers.h"
#include "AppCommon.h"

#include <ptop_src/ptop_engine_src/ThumbMgr/ThumbMgr.h>

#include <CoreLib/VxFileUtil.h>

//============================================================================
GuiMembershipAvailableMgr::GuiMembershipAvailableMgr( AppCommon& app )
    : QObject( &app )
    , m_MyApp( app )
{
}

//============================================================================
void GuiMembershipAvailableMgr::onAppCommonCreated( void )
{
    connect( this, SIGNAL( signalInternalMembershipAvailableAdded( VxGUID, VxGUID, MembershipAvailable ) ), this, SLOT( slotInternalMembershipAvailableAdded( VxGUID, VxGUID, MembershipAvailable ) ), Qt::QueuedConnection );
    connect( this, SIGNAL( signalInternalMembershipAvailableUpdated( VxGUID, VxGUID, MembershipAvailable ) ), this, SLOT( slotInternalMembershipAvailableUpdated( VxGUID, VxGUID, MembershipAvailable ) ), Qt::QueuedConnection );
    connect( this, SIGNAL( signalInternalMembershipAvailableRemoved(VxGUID) ), this, SLOT( slotInternalMembershipAvailableRemoved(VxGUID) ), Qt::QueuedConnection );

    m_MyApp.getEngine().getMembershipAvailableMgr().addMemberClient( this, true );
}

//============================================================================
bool GuiMembershipAvailableMgr::isMessengerReady( void )
{
    return m_MyApp.isMessengerReady();
}

//============================================================================
bool GuiMembershipAvailableMgr::canPushToTalk( VxGUID& onlineId )
{
    bool canPtt = false;
    for( auto iter = m_MemberList.begin(); iter != m_MemberList.end(); ++iter )
    {
        if( iter->first == onlineId )
        {
            if( iter->second.second.getCanPushToTalk() )
            {
                canPtt = true;
                break;
            }
        }
    }

    return canPtt;
}

//============================================================================
void GuiMembershipAvailableMgr::callbackMembershipAvailableAdded( VxGUID& onlineId, VxGUID& connectId, MembershipAvailable& memberAvail )
{
    emit signalInternalMembershipAvailableAdded( onlineId, connectId, memberAvail );
}

//============================================================================
void GuiMembershipAvailableMgr::callbackMembershipAvailableUpdated( VxGUID& onlineId, VxGUID& connectId, MembershipAvailable& memberAvail )
{
    emit signalInternalMembershipAvailableUpdated( onlineId, connectId, memberAvail );
}

//============================================================================
void GuiMembershipAvailableMgr::callbackMembershipAvailableRemoved( VxGUID& onlineId )
{
    emit signalInternalMembershipAvailableRemoved( onlineId );
}

//============================================================================
void GuiMembershipAvailableMgr::slotInternalMembershipAvailableAdded( VxGUID onlineId, VxGUID connectId, MembershipAvailable memberAvail )
{
    updateMemberAvail( onlineId, connectId, memberAvail );
}

//============================================================================
void GuiMembershipAvailableMgr::slotInternalMembershipAvailableUpdated( VxGUID onlineId, VxGUID connectId, MembershipAvailable memberAvail )
{
    updateMemberAvail( onlineId, connectId, memberAvail );
}

//============================================================================
void GuiMembershipAvailableMgr::slotInternalMembershipAvailableRemoved( VxGUID onlineId )
{
    removeMemberAvail( onlineId );
}

//============================================================================
void GuiMembershipAvailableMgr::updateMemberAvail( VxGUID& sktConnectId, VxGUID& onlineId, MembershipAvailable& memberAvail )
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

    bool wasUpdated = false;
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
void GuiMembershipAvailableMgr::removeMemberAvail( VxGUID& onlineId )
{

}

//============================================================================
void GuiMembershipAvailableMgr::onMembershipAdded( VxGUID& onlineId, VxGUID& sktConnectId, MembershipAvailable& memberAvail )
{
    announceMembershipAdded( onlineId, sktConnectId, memberAvail );
}

//============================================================================
void GuiMembershipAvailableMgr::onMembershipUpdated( VxGUID& onlineId, VxGUID& sktConnectId, MembershipAvailable& memberAvail )
{
    announceMembershipUpdated( onlineId, sktConnectId, memberAvail );
}

//============================================================================
void GuiMembershipAvailableMgr::onMembershipRemoved( VxGUID& onlineId )
{
    announceMembershipRemoved( onlineId );
}

//============================================================================
void GuiMembershipAvailableMgr::addMemberClient( GuiMembershipAvailableCallback* client, bool enable )
{
    if( enable )
    {
        m_MemberClients.push_back( client );
    }
    else
    {
        for( auto iter = m_MemberClients.begin(); iter != m_MemberClients.end(); ++iter )
        {
            if( *iter == client )
            {
                m_MemberClients.erase( iter );
                break;
            }
        }
    }
}

//============================================================================
void GuiMembershipAvailableMgr::announceMembershipAdded( VxGUID& onlineId, VxGUID& sktConnectId, MembershipAvailable& memberAvail )
{
    LogMsg( LOG_INFO, "GuiMembershipAvailableMgr::announceMembershipAdded start" );
    for( auto iter = m_MemberClients.begin(); iter != m_MemberClients.end(); ++iter )
    {
        GuiMembershipAvailableCallback* client = *iter;
        client->callbackGuiMembershipAvailableAdded( onlineId, sktConnectId, memberAvail );
    }

    LogMsg( LOG_INFO, "GuiMembershipAvailableMgr::announceMembershipAdded done" );
}

//============================================================================
void GuiMembershipAvailableMgr::announceMembershipUpdated( VxGUID& onlineId, VxGUID& sktConnectId, MembershipAvailable& memberAvail )
{
    LogMsg( LOG_INFO, "GuiMembershipAvailableMgr::announceMembershipUpdated start" );
    for( auto iter = m_MemberClients.begin(); iter != m_MemberClients.end(); ++iter )
    {
        GuiMembershipAvailableCallback* client = *iter;
        client->callbackGuiMembershipAvailableUpdated( onlineId, sktConnectId, memberAvail );
    }

    LogMsg( LOG_INFO, "GuiMembershipAvailableMgr::announceMembershipUpdated done" );
}

//============================================================================
void GuiMembershipAvailableMgr::announceMembershipRemoved( VxGUID& onlineId )
{
    for( auto iter = m_MemberClients.begin(); iter != m_MemberClients.end(); ++iter )
    {
        GuiMembershipAvailableCallback* client = *iter;
        client->callbackGuiMembershipAvailableRemoved( onlineId );
    }
}
