//============================================================================
// Copyright (C) 2020 Brett R. Jones
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

#include "HostBaseMgr.h"
#include "PluginBase.h"
#include "PluginMgr.h"

#include <GoTvCore/GoTvP2P/P2PEngine/P2PEngine.h>
#include <GoTvCore/GoTvP2P/Plugins/PluginBase.h>

#include <NetLib/VxSktBase.h>
#include <PktLib/PktsHostJoin.h>

#include <memory.h>

#ifdef _MSC_VER
# pragma warning(disable: 4355) //'this' : used in base member initializer list
#endif //_MSC_VER

//============================================================================
HostBaseMgr::HostBaseMgr( P2PEngine& engine, PluginMgr& pluginMgr, VxNetIdent * myIdent, PluginBase& pluginBase )
: m_Engine( engine )
, m_PluginMgr( pluginMgr )
, m_MyIdent( myIdent )
, m_Plugin( pluginBase )
, m_ConnectionMgr(engine.getConnectionMgr())
{
}

//============================================================================
EPluginAccess HostBaseMgr::getPluginAccessState( VxNetIdent * netIdent )
{
    EPluginAccess pluginAccess = ePluginAccessNotSet;

    EFriendState pluginState = m_MyIdent->getPluginPermission( m_Plugin.getPluginType() );
    if( eFriendStateIgnore == pluginState )
    {
        // we are not enabled
        pluginAccess = ePluginAccessDisabled;
    }
    else
    {
        if( netIdent->isIgnored() )
        {
            pluginAccess = ePluginAccessIgnored;
        }
        else
        {
            EFriendState friendState = netIdent->getMyFriendshipToHim();
            // everybody gets at least guest permission
            if( friendState < eFriendStateGuest )
            {
                friendState = eFriendStateGuest;
            }

            if( friendState < pluginState )
            {
                // not enough permission
                pluginAccess = ePluginAccessLocked;
            }
            else
            {
                pluginAccess = ePluginAccessOk;
            }
        }
    }

    return pluginAccess;
}

//============================================================================
void HostBaseMgr::fromGuiJoinHost( EHostType hostType, std::string ptopUrl )
{
    std::string url = !ptopUrl.empty() ? ptopUrl : m_ConnectionMgr.getDefaultHostUrl( hostType );

    if( ptopUrl.empty() && hostType == eHostTypeUnknown )
    {
        m_Engine.getToGui().toGuiHostJoinStatus( hostType, eHostJoinInvalidUrl );
        return;
    }

    connectToHost( hostType, url, eConnectReasonChatRoomJoin );
}

//============================================================================
void HostBaseMgr::connectToHost( EHostType hostType, std::string& url, EConnectReason connectReason )
{
    if( !url.empty() )
    {
        VxGUID hostGuid;
        EHostJoinStatus joinStatus = m_ConnectionMgr.lookupOrQueryId( url.c_str(), hostGuid, this, connectReason);
        if( eHostJoinQueryIdSuccess == joinStatus )
        {
            // no need for id query.. just request connection
            onUrlActionQueryIdSuccess( url, hostGuid, connectReason );
        }
        else if( eHostJoinQueryIdInProgress == joinStatus )
        {
            // manager is attempting to query id
            m_Engine.getToGui().toGuiHostJoinStatus( hostType, eHostJoinQueryIdInProgress );
        }
        else
        {
            m_Engine.getToGui().toGuiHostJoinStatus( hostType, eHostJoinInvalidUrl );
            onConnectToHostFail( hostType, connectReason, eHostJoinInvalidUrl );
        }
    }
    else
    {
        LogMsg( LOG_VERBOSE, "HostBaseMgr host %s url is empty", DescribeHostType( hostType ) );
        onConnectToHostFail( hostType, connectReason, eHostJoinInvalidUrl );
    }
}

//============================================================================
void HostBaseMgr::onConnectToHostFail( EHostType hostType, EConnectReason connectReason, EHostJoinStatus hostJoinStatus )
{
    LogMsg( LOG_VERBOSE, "HostBaseMgr connect reason %s to host %s failed %s ", DescribeConnectReason( connectReason ), DescribeHostType( hostType ),
           DescribeHostJoinStatus(hostJoinStatus));
    m_Engine.getToGui().toGuiHostJoinStatus( hostType, hostJoinStatus );
}

//============================================================================
void HostBaseMgr::onConnectToHostSuccess( EHostType hostType, VxSktBase* sktBase, VxGUID& onlineId, EConnectReason connectReason )
{
    LogMsg( LOG_VERBOSE, "HostBaseMgr connect reason %s to host %s success ", DescribeConnectReason( connectReason ), DescribeHostType( hostType ) );
}

//============================================================================
void HostBaseMgr::onConnectionToHostDisconnect( EHostType hostType, VxSktBase* sktBase, VxGUID& onlineId, EConnectReason connectReason )
{
    LogMsg( LOG_VERBOSE, "HostBaseMgr onConnectionToHostDisconnect reason %s to host %s ", DescribeConnectReason( connectReason ), DescribeHostType( hostType ) );
}

//============================================================================
EHostType HostBaseMgr::connectReasonToHostType( EConnectReason connectReason )
{
    EHostType hostType = eHostTypeUnknown;
    switch( connectReason )
    {
    case eConnectReasonChatRoomAnnounce:
    case eConnectReasonChatRoomSearch:
    case eConnectReasonGroupAnnounce:
    case eConnectReasonGroupSearch:
    case eConnectReasonRandomConnectAnnounce:
    case eConnectReasonRandomConnectSearch:
        hostType = eHostTypeNetwork;
        break;


    case eConnectReasonChatRoomJoin:
        hostType = eHostTypeChatRoom;
        break;

    case eConnectReasonGroupJoin:
        hostType = eHostTypeGroup;
        break;

    case eConnectReasonRandomConnectJoin:
        hostType = eHostTypeRandomConnect;
        break;

    default:
        break;

    }

    return hostType;
}

//============================================================================
void HostBaseMgr::onUrlActionQueryIdSuccess( std::string& url, VxGUID& onlineId, EConnectReason connectReason )
{
    EHostType hostType = connectReasonToHostType( connectReason );
    if( eHostTypeUnknown != hostType )
    {
        m_Engine.getToGui().toGuiHostJoinStatus( hostType, eHostJoinConnecting );
    }

    VxSktBase* sktBase = nullptr;
    EConnectStatus connectStatus = m_ConnectionMgr.requestConnection( url, onlineId, this, sktBase, connectReason );
    if( eConnectStatusReady == connectStatus )
    {
        if( sktBase )
        {
            onContactConnected( sktBase, onlineId, connectReason );
        }
        else
        {
            LogMsg( LOG_ERROR, "requestConnection sucess but has null socket" );
            onConnectToHostFail( hostType, connectReason, eHostJoinConnectFailed );
        }
    }
}

//============================================================================
void HostBaseMgr::onUrlActionQueryIdFail( std::string& url, ERunTestStatus testStatus, EConnectReason connectReason )
{
    EHostType hostType = connectReasonToHostType( connectReason );
    if( eHostTypeUnknown != hostType )
    {
        m_Engine.getToGui().toGuiHostJoinStatus( hostType, eHostJoinQueryIdFailed, DescribeRunTestStatus( testStatus ) );
    }

    onConnectToHostFail( hostType, connectReason, eHostJoinQueryIdFailed );
}

//============================================================================
bool HostBaseMgr::onContactConnected( VxSktBase* sktBase, VxGUID& onlineId, EConnectReason connectReason )
{
    EHostType hostType = connectReasonToHostType( connectReason );
    if( eHostTypeUnknown != hostType )
    {
        m_Engine.getToGui().toGuiHostJoinStatus( eHostTypeChatRoom, eHostJoinConnectSuccess );
        onConnectToHostSuccess( hostType, sktBase, onlineId, connectReason);
    }

    if( eConnectReasonChatRoomJoin == connectReason )
    {
        sendJoinRequest( sktBase, onlineId, connectReason );
    }

    return false;
}

//============================================================================
void HostBaseMgr::onContactDisconnected( VxSktBase* sktBase, VxGUID& onlineId, EConnectReason connectReason )
{
    EHostType hostType = connectReasonToHostType( connectReason );
    if( eHostTypeUnknown != hostType )
    {
        m_Engine.getToGui().toGuiHostJoinStatus( eHostTypeChatRoom, eHostJoinConnectSuccess );
        onConnectToHostSuccess( hostType, sktBase, onlineId, connectReason);
    }

    if( eConnectReasonChatRoomJoin == connectReason )
    {
        sendJoinRequest( sktBase, onlineId, connectReason );
    }
}

//============================================================================
void HostBaseMgr::onConnectRequestFail( VxGUID& onlineId, EConnectStatus connectStatus, EConnectReason connectReason )
{
    if( eConnectReasonChatRoomJoin == connectReason )
    {
        m_Engine.getToGui().toGuiHostJoinStatus( eHostTypeChatRoom, eHostJoinConnectFailed, DescribeConnectStatus( connectStatus ) );
        m_ConnectionMgr.doneWithConnection( onlineId, this, connectReason);
    }
}

//============================================================================
void HostBaseMgr::sendJoinRequest( VxSktBase* sktBase, VxGUID& onlineId, EConnectReason connectReason )
{
    vx_assert( nullptr != sktBase );
    PktHostJoinReq pktJoin;
    // temp for development
    //pktJoin.setIsLoopback( true );


    pktJoin.setHostType( eHostTypeChatRoom );
    if( m_Plugin.txPacket( onlineId, sktBase, &pktJoin ) )
    {
        m_Engine.getToGui().toGuiHostJoinStatus( eHostTypeChatRoom, eHostJoinSendingJoinRequest );
    }
    else
    {
        m_Engine.getToGui().toGuiHostJoinStatus( eHostTypeChatRoom, eHostJoinSendJoinRequestFailed );
        m_ConnectionMgr.doneWithConnection( onlineId, this, connectReason);
    }
}

//============================================================================
bool HostBaseMgr::addContact( VxSktBase * sktBase, VxNetIdent * netIdent )
{
    bool wasAdded = m_ContactList.addGuidIfDoesntExist( netIdent->getMyOnlineId() );
    if( wasAdded )
    {
        // TODO implement contact added
    }

    return wasAdded;
}

//============================================================================
bool HostBaseMgr::removeContact( VxGUID& onlineId )
{
    bool wasRemoved = m_ContactList.removeGuid( onlineId );
    if( wasRemoved )
    {
        // TODO implement contact removed
    }

    return wasRemoved;
}