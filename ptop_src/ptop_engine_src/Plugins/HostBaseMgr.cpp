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

#include <ptop_src/ptop_engine_src/P2PEngine/P2PEngine.h>
#include <ptop_src/ptop_engine_src/Plugins/PluginBase.h>
#include <ptop_src/ptop_engine_src/UserJoinMgr/UserJoinMgr.h>
#include <ptop_src/ptop_engine_src/UserOnlineMgr/UserOnlineMgr.h>

#include <ptop_src/ptop_engine_src/UrlMgr/UrlMgr.h>

#include <NetLib/VxSktBase.h>
#include <PktLib/PktsHostJoin.h>
#include <PktLib/PktsSearch.h>
#include <PktLib/SearchParams.h>

#include <CoreLib/VxPtopUrl.h>

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
EPluginType HostBaseMgr::getPluginType( void )
{
    return m_Plugin.getPluginType();
}

//============================================================================
EHostType HostBaseMgr::getHostType( void )
{
    return PluginTypeToHostType( getPluginType() );
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
EConnectReason HostBaseMgr::getSearchConnectReason( EHostType hostType )
{
    EConnectReason connectReason = eConnectReasonUnknown;
    switch( hostType )
    {
    case eHostTypeChatRoom:
        connectReason = eConnectReasonChatRoomSearch;
        break;
    case eHostTypeGroup:
        connectReason = eConnectReasonGroupSearch;
        break;
    case eHostTypeRandomConnect:
        connectReason = eConnectReasonRandomConnectSearch;
        break;
    default:
        break;
    }

    return connectReason;
}

//============================================================================
void HostBaseMgr::fromGuiAnnounceHost( EHostType hostType, VxGUID& sessionId, std::string& ptopUrl )
{
    std::string url = !ptopUrl.empty() ? ptopUrl : m_ConnectionMgr.getDefaultHostUrl( hostType );

    if( ptopUrl.empty() || hostType == eHostTypeUnknown )
    {
        m_Engine.getToGui().toGuiHostAnnounceStatus( hostType, sessionId, eHostAnnounceInvalidUrl );
        return;
    }

    connectToHost( hostType, sessionId, url, HostTypeToConnectAnnounceReason( hostType ) );
}

//============================================================================
void HostBaseMgr::fromGuiJoinHost( EHostType hostType, VxGUID& sessionId, std::string& ptopUrl )
{
    connectToHostByPtopUrlAndReason( hostType, sessionId, ptopUrl, HostTypeToConnectJoinReason( hostType ) );
}

//============================================================================
void HostBaseMgr::fromGuiLeaveHost( EHostType hostType, VxGUID& sessionId, std::string& ptopUrl )
{
    connectToHostByPtopUrlAndReason( hostType, sessionId, ptopUrl, HostTypeToConnectLeaveReason( hostType ) );
}

//============================================================================
void HostBaseMgr::fromGuiUnJoinHost( EHostType hostType, VxGUID& sessionId, std::string& ptopUrl )
{
    connectToHostByPtopUrlAndReason( hostType, sessionId, ptopUrl, HostTypeToConnectUnJoinReason( hostType ) );
}

//============================================================================
void HostBaseMgr::fromGuiSearchHost( EHostType hostType, SearchParams& searchParams, bool enable )
{
    std::string url = searchParams.getSearchUrl();
    if( url.empty() )
    {
        url = m_ConnectionMgr.getDefaultHostUrl( hostType );
    }

    if( url.empty() || hostType == eHostTypeUnknown )
    {
        LogMsg( LOG_VERBOSE, "HostBaseMgr Invalid url or host type %d", hostType );
        m_Engine.getToGui().toGuiHostSearchStatus( hostType, searchParams.getSearchSessionId(), eHostSearchInvalidUrl );
        m_Engine.getToGui().toGuiHostSearchComplete( hostType, searchParams.getSearchSessionId() );
        return;
    }

    if( !searchParams.getSearchSessionId().isVxGUIDValid() )
    {
        LogMsg( LOG_VERBOSE, "HostBaseMgr Search GUID invalid" );
        m_Engine.getToGui().toGuiHostSearchStatus( hostType, searchParams.getSearchSessionId(), eHostSearchInvalidParam );
        m_Engine.getToGui().toGuiHostSearchComplete( hostType, searchParams.getSearchSessionId() );
        return;
    }

    searchParams.setHostType( hostType );
    if( m_Engine.isNetworkHostEnabled() )
    {
        // I am network host so search myself
        m_Engine.fromGuiSendAnnouncedList( hostType, m_Engine.getMyOnlineId() );
        m_Engine.getToGui().toGuiHostSearchStatus( hostType, m_Engine.getMyOnlineId(), eHostSearchCompleted );
        m_Engine.getToGui().toGuiHostSearchComplete( hostType, searchParams.getSearchSessionId() );
    }
    else
    {
        EConnectReason connectReason = eConnectReasonUnknown;
        bool isJoinSearch = false;
        switch( searchParams.getSearchType() )
        {
        case eSearchChatRoomHost:	        //!< Search for Chat Room to Join
            isJoinSearch = true;
            connectReason = eConnectReasonChatRoomSearch;
            break;
        case eSearchGroupHost:	            //!< Search for Group to Join
            isJoinSearch = true;
            connectReason = eConnectReasonGroupSearch;
            break;
        case eSearchRandomConnectHost:	    //!< Search for Random Connect Server to Join
            isJoinSearch = true;
            connectReason = eConnectReasonRandomConnectSearch;
            break;

        default:
            LogMsg( LOG_VERBOSE, "HostBaseMgr Unknown search type" );
            m_Engine.getToGui().toGuiHostSearchStatus( hostType, searchParams.getSearchSessionId(), eHostSearchInvalidParam );
            m_Engine.getToGui().toGuiHostSearchComplete( hostType, searchParams.getSearchSessionId() );
            return;
        }

        if( !enable )
        {
            stopHostSearch( hostType, searchParams );
        }
        else
        {
            if( addSearchSession( searchParams.getSearchSessionId(), searchParams ) )
            {
                connectToHost( hostType, searchParams.getSearchSessionId(), url, connectReason );
            }
        }
    }
}

//============================================================================
bool HostBaseMgr::isAnnounceConnectReason( EConnectReason connectReason )
{
    bool isAnnounceReason = false;
    switch( connectReason )
    {
    case eConnectReasonChatRoomAnnounce:
    case eConnectReasonGroupAnnounce:
    case eConnectReasonRandomConnectAnnounce:
        isAnnounceReason = true;
        break;
    default:
        break;
    }

    return isAnnounceReason;
}

//============================================================================
bool HostBaseMgr::isJoinConnectReason( EConnectReason connectReason )
{
    return IsConnectReasonJoin( connectReason );
}

//============================================================================
bool HostBaseMgr::isLeaveConnectReason( EConnectReason connectReason )
{
    return IsConnectReasonLeave( connectReason );
}

//============================================================================
bool HostBaseMgr::isUnJoinConnectReason( EConnectReason connectReason )
{
    return IsConnectReasonUnJoin( connectReason );
}

//============================================================================
bool HostBaseMgr::isSearchConnectReason( EConnectReason connectReason )
{
    return IsConnectReasonSearch( connectReason );
}

//============================================================================
void HostBaseMgr::onConnectToHostFail( EHostType hostType, VxGUID& sessionId, EConnectReason connectReason, EHostAnnounceStatus hostAnnStatus )
{
    LogMsg( LOG_VERBOSE, "HostBaseMgr connect reason %s to host %s failed %s ", DescribeConnectReason( connectReason ), DescribeHostType( hostType ),
        DescribeHostAnnounceStatus(hostAnnStatus));
    m_Engine.getToGui().toGuiHostAnnounceStatus( hostType, sessionId, hostAnnStatus );
    removeSession( sessionId, connectReason );
}

//============================================================================
void HostBaseMgr::onConnectToHostFail( EHostType hostType, VxGUID& sessionId, EConnectReason connectReason, EHostJoinStatus hostJoinStatus )
{
    LogMsg( LOG_VERBOSE, "HostBaseMgr connect reason %s to host %s failed %s ", DescribeConnectReason( connectReason ), DescribeHostType( hostType ),
           DescribeHostJoinStatus(hostJoinStatus));
    m_Engine.getToGui().toGuiHostJoinStatus( hostType, sessionId, hostJoinStatus );
    removeSession( sessionId, connectReason );
}

//============================================================================
void HostBaseMgr::onConnectToHostFail( EHostType hostType, VxGUID& sessionId, EConnectReason connectReason, EHostSearchStatus hostSearchStatus )
{
    LogMsg( LOG_VERBOSE, "HostBaseMgr connect reason %s to host %s failed %s ", DescribeConnectReason( connectReason ), DescribeHostType( hostType ),
        DescribeHostSearchStatus(hostSearchStatus));
    m_Engine.getToGui().toGuiHostSearchStatus( hostType, sessionId, hostSearchStatus );
    m_Engine.getToGui().toGuiHostSearchComplete( hostType, sessionId );
    removeSession( sessionId, connectReason );
}

//============================================================================
bool HostBaseMgr::onConnectToHostSuccess( EHostType hostType, VxGUID& sessionId, VxSktBase* sktBase, VxGUID& onlineId, EConnectReason connectReason )
{
    bool result{ true };
    LogMsg( LOG_VERBOSE, "HostBaseMgr connect reason %s to host %s success ", DescribeConnectReason( connectReason ), DescribeHostType( hostType ) );
    if( isAnnounceConnectReason( connectReason ) )
    {
        m_Engine.getToGui().toGuiHostAnnounceStatus( hostType, sessionId, eHostAnnounceConnectSuccess );
        sendAnnounceRequest( hostType, sessionId, sktBase, onlineId, connectReason );
    }
    else if( isJoinConnectReason( connectReason ) )
    {
        m_Engine.getToGui().toGuiHostJoinStatus( hostType, sessionId, eHostJoinConnectSuccess );
        sendJoinRequest( hostType, sessionId, sktBase, onlineId, connectReason );
    }
    else if( isLeaveConnectReason( connectReason ) )
    {
        m_Engine.getToGui().toGuiHostJoinStatus( hostType, sessionId, eHostJoinConnectSuccess );
        sendLeaveRequest( hostType, sessionId, sktBase, onlineId, connectReason );
    }
    else if( isUnJoinConnectReason( connectReason ) )
    {
        m_Engine.getToGui().toGuiHostJoinStatus( hostType, sessionId, eHostJoinConnectSuccess );
        sendUnJoinRequest( hostType, sessionId, sktBase, onlineId, connectReason );
    }
    else if( isSearchConnectReason( connectReason ) )
    {
        m_Engine.getToGui().toGuiHostSearchStatus( hostType, sessionId, eHostSearchConnectSuccess );
        LogMsg( LOG_VERBOSE, "HostBaseMgr connect reason %s to host %s success Default function should be overridden", DescribeConnectReason( connectReason ), DescribeHostType( hostType ) );
    }
    else
    {
        LogMsg( LOG_ERROR, "Unknown Connect Reason %d", connectReason );
        result = false;
    }

    return result;
}

//============================================================================
void HostBaseMgr::onConnectionToHostDisconnect( EHostType hostType, VxGUID& sessionId, VxSktBase* sktBase, VxGUID& onlineId, EConnectReason connectReason )
{
    LogMsg( LOG_VERBOSE, "HostBaseMgr onConnectionToHostDisconnect reason %s to host %s ", DescribeConnectReason( connectReason ), DescribeHostType( hostType ) );
    removeSession( sessionId, connectReason );
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

    case eConnectReasonGroupJoin:
    case eConnectReasonGroupLeave:
    case eConnectReasonGroupUnJoin:
        hostType = eHostTypeGroup;
        break;

    case eConnectReasonChatRoomJoin:
    case eConnectReasonChatRoomLeave:
    case eConnectReasonChatRoomUnJoin:
        hostType = eHostTypeChatRoom;
        break;

    case eConnectReasonRandomConnectJoin:
    case eConnectReasonRandomConnectLeave:
    case eConnectReasonRandomConnectUnJoin:
        hostType = eHostTypeRandomConnect;
        break;

    default:
        break;
    }

    return hostType;
}

//============================================================================
bool HostBaseMgr::onUrlActionQueryIdSuccess( VxGUID& sessionId, std::string& url, VxGUID& onlineId, EConnectReason connectReason )
{
    bool result{ false };
    EHostType hostType = connectReasonToHostType( connectReason );
    LogModule( eLogHostConnect, LOG_DEBUG, "HostBaseMgr::onUrlActionQueryIdSuccess %s url %s reason %s", DescribeHostType( hostType ), url.c_str(), 
               DescribeConnectReason( connectReason ));
    if( eHostTypeUnknown != hostType )
    {
        if( isAnnounceConnectReason( connectReason ) )
        {
            m_Engine.getToGui().toGuiHostAnnounceStatus( hostType, sessionId, eHostAnnounceConnecting );
        }
        else if( isSearchConnectReason( connectReason ) )
        {
            m_Engine.getToGui().toGuiHostSearchStatus( hostType, sessionId, eHostSearchConnecting );
        }
        else if( isJoinConnectReason( connectReason ) )
        {
            m_Engine.getToGui().toGuiHostJoinStatus( hostType, sessionId, eHostJoinConnecting );
        }  
        else if( isLeaveConnectReason( connectReason ) )
        {
            m_Engine.getToGui().toGuiHostJoinStatus( hostType, sessionId, eHostJoinConnecting );
        }
        else if( isUnJoinConnectReason( connectReason ) )
        {
            m_Engine.getToGui().toGuiHostJoinStatus( hostType, sessionId, eHostJoinConnecting );
        }
        else
        {
            LogMsg( LOG_ERROR, "onUrlActionQueryIdSuccess Unknown connect reason %s", DescribeConnectReason( connectReason ) );
        }
    }

    VxSktBase* sktBase = nullptr;
    EConnectStatus connectStatus = m_ConnectionMgr.requestConnection( sessionId, url, onlineId, this, sktBase, connectReason );
    if( eConnectStatusHandshaking == connectStatus )
    {
        if( sktBase )
        {
            result = onContactHandshaking( sessionId, sktBase, onlineId, connectReason );
        }
        else
        {
            LogModule( eLogHostConnect, LOG_DEBUG, "requestConnection success but has null socket" );
            if( isAnnounceConnectReason( connectReason ) )
            {
                onConnectToHostFail( hostType, sessionId, connectReason, eHostAnnounceConnectFailed );
            }
            else if( isSearchConnectReason( connectReason ) )
            {
                onConnectToHostFail( hostType, sessionId, connectReason, eHostSearchConnectFailed );
            }
            else if( isJoinConnectReason( connectReason ) || isLeaveConnectReason( connectReason ) || isUnJoinConnectReason( connectReason ) )
            {
                onConnectToHostFail( hostType, sessionId, connectReason, eHostJoinConnectFailed );
            } 
            else
            {
                LogMsg( LOG_ERROR, "Unknown Connect Reason %d", connectReason );
            }
        }
    }
    else if( eConnectStatusReady == connectStatus )
    {
        if( sktBase )
        {
            result = onContactConnected( sessionId, sktBase, onlineId, connectReason );
        }
        else
        {
            LogModule( eLogHostConnect, LOG_DEBUG, "requestConnection success but has null socket" );
            if( isAnnounceConnectReason( connectReason ) )
            {
                onConnectToHostFail( hostType, sessionId, connectReason, eHostAnnounceConnectFailed );
            }
            else if( isSearchConnectReason( connectReason ) )
            {
                onConnectToHostFail( hostType, sessionId, connectReason, eHostSearchConnectFailed );
            }
            else if( isJoinConnectReason( connectReason ) || isLeaveConnectReason( connectReason ) || isUnJoinConnectReason( connectReason ) )
            {
                onConnectToHostFail( hostType, sessionId, connectReason, eHostJoinConnectFailed );
            } 
            else
            {
                LogMsg( LOG_ERROR, "Unknown Connect Reason %d", connectReason );
            }
        }
    }
    else
    {
        LogModule( eLogHostConnect, LOG_DEBUG, "requestConnection failed" );
        if( isAnnounceConnectReason( connectReason ) )
        {
            onConnectToHostFail( hostType, sessionId, connectReason, eHostAnnounceConnectFailed );
        }
        else if( isSearchConnectReason( connectReason ) )
        {
            onConnectToHostFail( hostType, sessionId, connectReason, eHostSearchConnectFailed );
        }
        else if( isJoinConnectReason( connectReason ) || isLeaveConnectReason( connectReason ) || isUnJoinConnectReason( connectReason ) )
        {
            onConnectToHostFail( hostType, sessionId, connectReason, eHostJoinConnectFailed );
        } 
        else
        {
            LogMsg( LOG_ERROR, "Unknown Connect Reason %d", connectReason );
        }
    }

    return result;
}

//============================================================================
void HostBaseMgr::onUrlActionQueryIdFail( VxGUID& sessionId, std::string& url, ERunTestStatus testStatus, EConnectReason connectReason, ECommErr commErr )
{
    EHostType hostType = connectReasonToHostType( connectReason );
    if( eHostTypeUnknown != hostType )
    {
        if( isAnnounceConnectReason( connectReason ) )
        {
            m_Engine.getToGui().toGuiHostAnnounceStatus( hostType, sessionId, eHostAnnounceQueryIdFailed, DescribeRunTestStatus( testStatus ) );
            m_Engine.getToGui().toGuiHostSearchComplete( hostType, sessionId );
        }
        else if( isSearchConnectReason( connectReason ) )
        {
            m_Engine.getToGui().toGuiHostSearchStatus( hostType, sessionId, eHostSearchQueryIdFailed, commErr, DescribeRunTestStatus( testStatus ) );
            m_Engine.getToGui().toGuiHostSearchComplete( hostType, sessionId );
        }
        else if( isJoinConnectReason( connectReason ) )
        {
            m_Engine.getToGui().toGuiHostJoinStatus( hostType, sessionId, eHostJoinQueryIdFailed, DescribeRunTestStatus( testStatus ) );
            m_Engine.getToGui().toGuiHostSearchComplete( hostType, sessionId );
        }
        else
        {
            LogMsg( LOG_ERROR, "Unknown Connect Reason %d", connectReason );
        }
    }
    else if( isAnnounceConnectReason( connectReason ) )
    {
        onConnectToHostFail( hostType, sessionId, connectReason, eHostAnnounceQueryIdFailed );
    }
    else if( isSearchConnectReason( connectReason ) )
    {
        onConnectToHostFail( hostType, sessionId, connectReason, eHostSearchQueryIdFailed );
    }
    else if( isJoinConnectReason( connectReason ) || isLeaveConnectReason( connectReason ) || isUnJoinConnectReason( connectReason ) )
    {
        onConnectToHostFail( hostType, sessionId, connectReason, eHostJoinQueryIdFailed );
    }
    else
    {
        LogMsg( LOG_ERROR, "Unknown Connect Reason %d", connectReason );
    }
}

//============================================================================
bool HostBaseMgr::onContactHandshaking( VxGUID& sessionId, VxSktBase* sktBase, VxGUID& onlineId, EConnectReason connectReason )
{
    EHostType hostType = connectReasonToHostType( connectReason );
    if( eHostTypeUnknown != hostType )
    {
        if( isAnnounceConnectReason( connectReason ) )
        {
            m_Engine.getToGui().toGuiHostAnnounceStatus( hostType, sessionId, eHostAnnounceHandshaking );
        }
        else if( isSearchConnectReason( connectReason ) )
        {
            m_Engine.getToGui().toGuiHostSearchStatus( hostType, sessionId, eHostSearchHandshaking );
        }
        else if( isJoinConnectReason( connectReason ) || isLeaveConnectReason( connectReason ) || isUnJoinConnectReason( connectReason ) )
        {
            m_Engine.getToGui().toGuiHostJoinStatus( hostType, sessionId, eHostJoinHandshaking );
        }
        else
        {
            LogMsg( LOG_ERROR, "Unknown Connect Reason %d", connectReason );
        }
    }

    return true;
}

//============================================================================
void HostBaseMgr::onHandshakeTimeout( VxGUID& sessionId, VxSktBase* sktBase, VxGUID& onlineId, EConnectReason connectReason )
{
    EHostType hostType = connectReasonToHostType( connectReason );
    if( eHostTypeUnknown != hostType )
    {
        if( isAnnounceConnectReason( connectReason ) )
        {
            m_Engine.getToGui().toGuiHostAnnounceStatus( hostType, sessionId, eHostAnnounceHandshakeTimeout );
        }
        else if( isSearchConnectReason( connectReason ) )
        {
            m_Engine.getToGui().toGuiHostSearchStatus( hostType, sessionId, eHostSearchHandshakeTimeout );
        }
        else if( isJoinConnectReason( connectReason ) || isLeaveConnectReason( connectReason ) || isUnJoinConnectReason( connectReason ) )
        {
            m_Engine.getToGui().toGuiHostJoinStatus( hostType, sessionId, eHostJoinHandshakeTimeout );
        }
        else
        {
            LogMsg( LOG_ERROR, "Unknown Connect Reason %d", connectReason );
        }
    }
    else if( isAnnounceConnectReason( connectReason ) )
    {
        onConnectToHostFail( hostType, sessionId, connectReason, eHostAnnounceHandshakeTimeout );
    }
    else if( isSearchConnectReason( connectReason ) )
    {
        onConnectToHostFail( hostType, sessionId, connectReason, eHostSearchHandshakeTimeout );
    }
    else if( isJoinConnectReason( connectReason ) || isLeaveConnectReason( connectReason ) || isUnJoinConnectReason( connectReason ) )
    {
        onConnectToHostFail( hostType, sessionId, connectReason, eHostJoinHandshakeTimeout );
    }
    else
    {
        LogMsg( LOG_ERROR, "Unknown Connect Reason %d", connectReason );
    }
}

//============================================================================
void HostBaseMgr::onContactSessionDone( VxGUID& sessionId, VxSktBase* sktBase, VxGUID& onlineId, EConnectReason connectReason )
{
    LogMsg( LOG_INFO, "onContactSessionDone  Reason %s", DescribeConnectReason( connectReason ) );
}

//============================================================================
bool HostBaseMgr::onContactConnected( VxGUID& sessionId, VxSktBase* sktBase, VxGUID& onlineId, EConnectReason connectReason )
{
    bool result{ false };
    EHostType hostType = connectReasonToHostType( connectReason );
    if( eHostTypeUnknown != hostType )
    {  
        result = onConnectToHostSuccess( hostType, sessionId, sktBase, onlineId, connectReason);
    }
    else
    {
        LogModule( eLogHostConnect, LOG_DEBUG, "HostBaseMgr connect reason %s unknown host type %d - %s success ", DescribeConnectReason( connectReason ), hostType, DescribeHostType( hostType ) );
    }

    return result;
}

//============================================================================
void HostBaseMgr::onContactDisconnected( VxGUID& sessionId, VxSktBase* sktBase, VxGUID& onlineId, EConnectReason connectReason )
{
    EHostType hostType = connectReasonToHostType( connectReason );
    if( eHostTypeUnknown != hostType )
    {
        if( isAnnounceConnectReason( connectReason ) )
        {
            m_Engine.getToGui().toGuiHostAnnounceStatus( hostType, sessionId, eHostAnnounceFailConnectDropped );
            onConnectToHostFail( hostType, sessionId, connectReason, eHostAnnounceConnectFailed );
        }
        else if( isJoinConnectReason( connectReason ) || isLeaveConnectReason( connectReason ) || isUnJoinConnectReason( connectReason ) )
        {
            m_Engine.getToGui().toGuiHostJoinStatus( hostType, sessionId, eHostJoinFailConnectDropped );
            onConnectToHostFail( hostType, sessionId, connectReason, eHostJoinFailConnectDropped );
        }
        else if( isSearchConnectReason( connectReason ) )
        {
            m_Engine.getToGui().toGuiHostSearchStatus( hostType, sessionId, eHostSearchFailConnectDropped );
        }
        else
        {
            LogMsg( LOG_ERROR, "Unknown Connect Reason %d", connectReason );
        }
    }
}

//============================================================================
void HostBaseMgr::onConnectRequestFail( VxGUID& sessionId, VxGUID& onlineId, EConnectStatus connectStatus, EConnectReason connectReason, ECommErr commErr )
{
    EHostType hostType = connectReasonToHostType( connectReason );
    if( hostType != eHostTypeUnknown )
    {
        if( isAnnounceConnectReason( connectReason ) )
        {
            m_Engine.getToGui().toGuiHostAnnounceStatus( hostType, sessionId, eHostAnnounceConnectFailed, DescribeConnectStatus( connectStatus ) );
            m_ConnectionMgr.doneWithConnection( sessionId, onlineId, this, connectReason );
        }
        else if( isJoinConnectReason( connectReason ) || isLeaveConnectReason( connectReason ) || isUnJoinConnectReason( connectReason ) )
        {
            m_Engine.getToGui().toGuiHostJoinStatus( hostType, sessionId, eHostJoinConnectFailed, DescribeConnectStatus( connectStatus ) );
            m_ConnectionMgr.doneWithConnection( sessionId, onlineId, this, connectReason );
        }
        else if( isSearchConnectReason( connectReason ) )
        {
            m_Engine.getToGui().toGuiHostSearchStatus( hostType, sessionId, eHostSearchConnectFailed, commErr, DescribeConnectStatus( connectStatus ) );
            m_Engine.getToGui().toGuiHostSearchComplete( hostType, sessionId );
            m_ConnectionMgr.doneWithConnection( sessionId, onlineId, this, connectReason );
        }
    }
    else
    {
        LogMsg( LOG_ERROR, "HostBaseMgr::onConnectRequestFail unknown connect reason %s", DescribeConnectReason( connectReason ) );
    }
}

//============================================================================
void HostBaseMgr::sendAnnounceRequest( EHostType hostType, VxGUID& sessionId, VxSktBase* sktBase, VxGUID& onlineId, EConnectReason connectReason )
{
    vx_assert( nullptr != sktBase );
    LogModule( eLogHostConnect, LOG_DEBUG, "HostBaseMgr:: sendAnnounceRequest not done %s", DescribeConnectReason( connectReason ) );
    PktHostJoinReq pktJoin;
    pktJoin.setPluginType( m_Plugin.getPluginType() );
    pktJoin.setHostType( hostType );
    pktJoin.setSessionId( sessionId );
    if( m_Plugin.txPacket( onlineId, sktBase, &pktJoin ) )
    {
        m_Engine.getToGui().toGuiHostAnnounceStatus( hostType, sessionId, eHostAnnounceSendingJoinRequest );
    }
    else
    {
        m_Engine.getToGui().toGuiHostAnnounceStatus( hostType, sessionId, eHostAnnounceSendJoinRequestFailed );
    }
}

//============================================================================
void HostBaseMgr::sendJoinRequest( EHostType hostType, VxGUID& sessionId, VxSktBase* sktBase, VxGUID& onlineId, EConnectReason connectReason )
{
    vx_assert( nullptr != sktBase );
    PktHostJoinReq pktJoin;

    m_Engine.getToGui().toGuiHostJoinStatus( hostType, sessionId, eHostJoinSendingJoinRequest );
    pktJoin.setPluginType( m_Plugin.getPluginType() );
    pktJoin.setHostType( hostType );
    pktJoin.setSessionId( sessionId );
    if( !m_Plugin.txPacket( onlineId, sktBase, &pktJoin ) )
    {
        m_Engine.getToGui().toGuiHostJoinStatus( hostType, sessionId, eHostJoinSendJoinRequestFailed );
    }
}

//============================================================================
void HostBaseMgr::sendLeaveRequest( EHostType hostType, VxGUID& sessionId, VxSktBase* sktBase, VxGUID& onlineId, EConnectReason connectReason )
{
    vx_assert( nullptr != sktBase );
    PktHostLeaveReq pktReq;

    m_Engine.getToGui().toGuiHostJoinStatus( hostType, sessionId, eHostJoinSendingLeaveRequest );
    pktReq.setPluginType( m_Plugin.getPluginType() );
    pktReq.setHostType( hostType );
    pktReq.setSessionId( sessionId );
    if( !m_Plugin.txPacket( onlineId, sktBase, &pktReq ) )
    {
        m_Engine.getToGui().toGuiHostJoinStatus( hostType, sessionId, eHostJoinSendLeaveRequestFailed );
    }
}

//============================================================================
void HostBaseMgr::sendUnJoinRequest( EHostType hostType, VxGUID& sessionId, VxSktBase* sktBase, VxGUID& onlineId, EConnectReason connectReason )
{
    vx_assert( nullptr != sktBase );
    PktHostUnJoinReq pktJoin;

    m_Engine.getToGui().toGuiHostJoinStatus( hostType, sessionId, eHostJoinSendingUnJoinRequest );
    pktJoin.setPluginType( m_Plugin.getPluginType() );
    pktJoin.setHostType( hostType );
    pktJoin.setSessionId( sessionId );
    if( !m_Plugin.txPacket( onlineId, sktBase, &pktJoin ) )
    {
        m_Engine.getToGui().toGuiHostJoinStatus( hostType, sessionId, eHostJoinSendUnJoinRequestFailed );
        m_ConnectionMgr.doneWithConnection( sessionId, onlineId, this, connectReason );
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

//============================================================================
bool HostBaseMgr::stopHostSearch( EHostType hostType, SearchParams& searchParams )
{
    return true;
}

//============================================================================
bool HostBaseMgr::addSearchSession( VxGUID& sessionId, SearchParams& searchParams )
{
    removeSearchSession( sessionId );
    m_SearchParamsMutex.lock();
    m_SearchParamsList[sessionId] = searchParams;
    m_SearchParamsMutex.unlock();
    return true;
}

//============================================================================
void HostBaseMgr::removeSearchSession( VxGUID& sessionId )
{
    m_SearchParamsMutex.lock();
    auto iter = m_SearchParamsList.find( sessionId );
    if( iter != m_SearchParamsList.end() )
    {
        m_SearchParamsList.erase( sessionId );
    }
    else
    {
        LogMsg( LOG_INFO, "HostBaseMgr::removeSearchSession session not found " );
    }

    m_SearchParamsMutex.unlock();
}

//============================================================================
void HostBaseMgr::onInvalidRxedPacket( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent, const char * msg )
{
    // TODO proper invalid packet handling
    LogMsg( LOG_INFO, "HostBaseMgr::onInvalidRxedPacket " );
}


//============================================================================
void HostBaseMgr::onUserOnline( VxSktBase* sktBase, VxNetIdent* netIdent, VxGUID& sessionId )
{
    if( sktBase && netIdent )
    {
        netIdent->upgradeToGuestFriendship();
        
        BaseSessionInfo sessionInfo( m_Plugin.getPluginType(), netIdent->getMyOnlineId(), sessionId, sktBase->getConnectionId() );
        m_Engine.getUserOnlineMgr().onUserOnline( sktBase, netIdent, sessionInfo );

        m_Engine.getToGui().toGuiUserOnlineStatus( netIdent, true );
    }
    else
    {
        LogMsg( LOG_ERROR, "HostBaseMgr::onUserOnline invalid param " );
    }
}

//============================================================================
void HostBaseMgr::onUserOffline( VxGUID& onlineId, VxGUID& sessionId )
{
    //m_UserListMutex.lock();
    //m_UserList.removeUser( onlineId, sessionId );
    //m_UserListMutex.unlock();
}

//============================================================================
EJoinState HostBaseMgr::getJoinState( VxNetIdent* netIdent, EHostType hostType )
{
    // BRJ TODO if has been accepted to host then should return eJoinStateJoinGranted
    return getPluginAccessState( netIdent ) == ePluginAccessOk ? eJoinStateJoinWasGranted : eJoinStateJoinRequested;
}

//============================================================================
EMembershipState HostBaseMgr::getMembershipState( VxNetIdent* netIdent, EHostType hostType )
{
    // BRJ TODO if has been accepted to host then should return eJoinStateJoinGranted
    return getPluginAccessState( netIdent ) == ePluginAccessOk ? eMembershipStateJoined : eMembershipStateJoinDenied;
}

//============================================================================
bool HostBaseMgr::connectToHostByPtopUrlAndReason( EHostType hostType, VxGUID& sessionId, std::string& ptopUrl, EConnectReason connectReason )
{
    std::string url = ptopUrl.empty() ? m_ConnectionMgr.getDefaultHostUrl( hostType ) : ptopUrl;
    if( url.empty() || hostType == eHostTypeUnknown )
    {
        LogMsg( LOG_ERROR, "HostBaseMgr::connectToHostByPtopUrlAndReason invalid param" );
        m_Engine.getToGui().toGuiHostJoinStatus( hostType, sessionId, eHostJoinInvalidUrl );
        return false;
    }

    bool result{ false };
    VxPtopUrl hostUrl( url );
    if( hostUrl.isValid() )
    {
        VxSktBase* sktBase = nullptr;
        if( m_Engine.getConnectMgr().isConnectedToHost( hostType, hostUrl, sktBase ) )
        {
            // handle already connected
            result = true;
            onConnectToHostSuccess( hostType, sessionId, sktBase, hostUrl.getOnlineId(), connectReason );
        }
        else
        {
            // try lookup by groupie id
            GroupieId groupieId( m_Engine.getMyOnlineId(), hostUrl.getOnlineId(), hostType );
            sktBase = m_Engine.getConnectIdListMgr().findHostConnection( groupieId );
            if( sktBase )
            {
                result = true;
                onConnectToHostSuccess( hostType, sessionId, sktBase, hostUrl.getOnlineId(), connectReason );
            }
            else
            {
                // connect without having to query host id
                result = connectToHost( hostType, sessionId, ptopUrl, connectReason );
            }
        }
    }
    else
    {
        // the url may not have online id and will have to be queried from host
        result = connectToHost( hostType, sessionId, url, connectReason );
    }

    return result;
}

//============================================================================
bool HostBaseMgr::connectToHost( EHostType hostType, VxGUID& sessionId, std::string& urlIn, EConnectReason connectReason )
{
    bool result{ false };
    std::string url;
    VxPtopUrl ptopUrl( urlIn );
    if( ptopUrl.isValid() )
    {
        // no need to resolve. is already valid
        url = urlIn;
    }
    else
    {
        url = m_Engine.getUrlMgr().resolveUrl( urlIn );
    }

    if( !url.empty() )
    {
        VxGUID hostGuid = ptopUrl.getOnlineId();

        LogModule( eLogHostConnect, LOG_DEBUG, "HostBaseMgr::connectToHost %s url %s reason %s", DescribeHostType( hostType ), url.c_str(),
            DescribeConnectReason( connectReason ) );
        if( isAnnounceConnectReason( connectReason ) )
        {
            if( ptopUrl.isValid() )
            {
                onUrlActionQueryIdSuccess( sessionId, url, hostGuid, connectReason );
                result = true;
            }
            else
            {
                EHostAnnounceStatus annStatus = m_ConnectionMgr.lookupOrQueryAnnounceId( hostType, sessionId, url.c_str(), hostGuid, this, connectReason );
                if( eHostAnnounceQueryIdSuccess == annStatus )
                {
                    // no need for id query.. just request connection
                    onUrlActionQueryIdSuccess( sessionId, url, hostGuid, connectReason );
                    result = true;
                }
                else if( eHostAnnounceQueryIdInProgress == annStatus )
                {
                    // manager is attempting to query id
                    m_Engine.getToGui().toGuiHostAnnounceStatus( hostType, sessionId, eHostAnnounceQueryIdInProgress );
                    result = true;
                }
                else
                {
                    m_Engine.getToGui().toGuiHostAnnounceStatus( hostType, sessionId, eHostAnnounceInvalidUrl );
                    onConnectToHostFail( hostType, sessionId, connectReason, eHostAnnounceInvalidUrl );
                }
            }
        }
        else if( isJoinConnectReason( connectReason ) || isLeaveConnectReason( connectReason ) || isUnJoinConnectReason( connectReason ) )
        {
            if( ptopUrl.isValid() )
            {
                result = onUrlActionQueryIdSuccess( sessionId, url, hostGuid, connectReason );
            }
            else
            {
                EHostJoinStatus joinStatus = m_ConnectionMgr.lookupOrQueryJoinId( sessionId, url.c_str(), hostGuid, this, connectReason );
                if( eHostJoinQueryIdSuccess == joinStatus )
                {
                    // no need for id query.. just request connection
                    result = onUrlActionQueryIdSuccess( sessionId, url, hostGuid, connectReason );
                }
                else if( eHostJoinQueryIdInProgress == joinStatus )
                {
                    // manager is attempting to query id
                    m_Engine.getToGui().toGuiHostJoinStatus( hostType, sessionId, eHostJoinQueryIdInProgress );
                    result = true;
                }
                else
                {
                    m_Engine.getToGui().toGuiHostJoinStatus( hostType, sessionId, eHostJoinInvalidUrl );
                    onConnectToHostFail( hostType, sessionId, connectReason, eHostJoinInvalidUrl );
                }
            }
        }
        else if( isSearchConnectReason( connectReason ) )
        {
            if( ptopUrl.isValid() )
            {
                result = onUrlActionQueryIdSuccess( sessionId, url, hostGuid, connectReason );
            }
            else
            {
                EHostSearchStatus searchStatus = m_ConnectionMgr.lookupOrQuerySearchId( sessionId, url.c_str(), hostGuid, this, connectReason );
                if( eHostSearchQueryIdSuccess == searchStatus )
                {
                    // no need for id query.. just request connection
                    result = onUrlActionQueryIdSuccess( sessionId, url, hostGuid, connectReason );
                }
                else if( eHostSearchQueryIdInProgress == searchStatus )
                {
                    // manager is attempting to query id
                    m_Engine.getToGui().toGuiHostSearchStatus( hostType, sessionId, eHostSearchQueryIdInProgress );
                    result = true;
                }
                else
                {
                    m_Engine.getToGui().toGuiHostSearchStatus( hostType, sessionId, eHostSearchInvalidUrl );
                    m_Engine.getToGui().toGuiHostSearchComplete( hostType, sessionId );

                    onConnectToHostFail( hostType, sessionId, connectReason, eHostSearchInvalidUrl );
                }
            }
        }
        else
        {
            LogMsg( LOG_VERBOSE, "HostBaseMgr unknown connect reason %d-%s", connectReason, DescribeConnectReason( connectReason ) );
        }
    }
    else
    {
        LogMsg( LOG_VERBOSE, "HostBaseMgr host %s url is empty", DescribeHostType( hostType ) );
        if( isAnnounceConnectReason( connectReason ) || isSearchConnectReason( connectReason ) || isJoinConnectReason( connectReason ) || 
            isLeaveConnectReason( connectReason ) || isUnJoinConnectReason( connectReason ) )
        {
            onConnectToHostFail( hostType, sessionId, connectReason, eHostAnnounceInvalidUrl );
        }
        else
        {
            LogMsg( LOG_ERROR, "Unknown Connect Reason %d", connectReason );
        }
    }

    return result;
}
