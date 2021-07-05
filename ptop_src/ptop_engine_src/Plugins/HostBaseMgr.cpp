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

#include <NetLib/VxSktBase.h>
#include <PktLib/PktsHostJoin.h>
#include <PktLib/PktsSearch.h>
#include <PktLib/SearchParams.h>

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
void HostBaseMgr::fromGuiAnnounceHost( EHostType hostType, VxGUID& sessionId, std::string ptopUrl )
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
void HostBaseMgr::fromGuiJoinHost( EHostType hostType, VxGUID& sessionId, std::string ptopUrl )
{
    std::string url = ptopUrl.empty() ? m_ConnectionMgr.getDefaultHostUrl( hostType ) : ptopUrl;
    if( url.empty() || hostType == eHostTypeUnknown )
    {
        LogMsg( LOG_ERROR, "HostBaseMgr::fromGuiJoinHost invalid param" );
        m_Engine.getToGui().toGuiHostJoinStatus( hostType, sessionId, eHostJoinInvalidUrl );
        return;
    }

    VxUrl hostUrl( url );
    if( hostUrl.hasValidOnlineId() )
    {
        VxSktBase* sktBase = nullptr;
        if( m_Engine.getConnectMgr().isConnectedToHost( hostType, hostUrl, sktBase ) )
        {
            // handle already connected
            onConnectToHostSuccess( hostType, sessionId, sktBase, hostUrl.getOnlineId(), HostTypeToConnectJoinReason( hostType ) );
        }
        else
        {
            // connect without having to query host id
            connectToHost( hostType, sessionId, ptopUrl, HostTypeToConnectJoinReason( hostType ) );
        }
    } 
    else
    {
        // the url may not have online id and will have to be queried from host
        connectToHost( hostType, sessionId, url, HostTypeToConnectJoinReason( hostType ) );
    }
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
        return;
    }

    if( !searchParams.getSearchSessionId().isVxGUIDValid() )
    {
        LogMsg( LOG_VERBOSE, "HostBaseMgr Search GUID invalid" );
        m_Engine.getToGui().toGuiHostSearchStatus( hostType, searchParams.getSearchSessionId(), eHostSearchInvalidParam );
        return;
    }

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
        return;
    }

    searchParams.setHostType( hostType );
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

//============================================================================
void HostBaseMgr::connectToHost( EHostType hostType, VxGUID& sessionId, std::string& url, EConnectReason connectReason )
{
    if( !url.empty() )
    {
        VxGUID hostGuid;
        LogModule( eLogHostConnect, LOG_DEBUG, "HostBaseMgr::connectToHost %s url %s reason %s", DescribeHostType( hostType ), url.c_str(), 
                  DescribeConnectReason( connectReason ));
        if( isAnnounceConnectReason( connectReason ) )
        {
            EHostAnnounceStatus annStatus = m_ConnectionMgr.lookupOrQueryAnnounceId( sessionId, url.c_str(), hostGuid, this, connectReason);
            if( eHostAnnounceQueryIdSuccess == annStatus )
            {
                // no need for id query.. just request connection
                onUrlActionQueryIdSuccess( sessionId, url, hostGuid, connectReason );
            }
            else if( eHostAnnounceQueryIdInProgress == annStatus )
            {
                // manager is attempting to query id
                m_Engine.getToGui().toGuiHostAnnounceStatus( hostType, sessionId, eHostAnnounceQueryIdInProgress );
            }
            else
            {
                m_Engine.getToGui().toGuiHostAnnounceStatus( hostType, sessionId, eHostAnnounceInvalidUrl );
                onConnectToHostFail( hostType, sessionId, connectReason, eHostAnnounceInvalidUrl );
            }
        }
        else if( isJoinConnectReason( connectReason ) )
        {
            EHostJoinStatus joinStatus = m_ConnectionMgr.lookupOrQueryJoinId( sessionId, url.c_str(), hostGuid, this, connectReason);
            if( eHostJoinQueryIdSuccess == joinStatus )
            {
                // no need for id query.. just request connection
                onUrlActionQueryIdSuccess( sessionId, url, hostGuid, connectReason );
            }
            else if( eHostJoinQueryIdInProgress == joinStatus )
            {
                // manager is attempting to query id
                m_Engine.getToGui().toGuiHostJoinStatus( hostType, sessionId, eHostJoinQueryIdInProgress );
            }
            else
            {
                m_Engine.getToGui().toGuiHostJoinStatus( hostType, sessionId, eHostJoinInvalidUrl );
                onConnectToHostFail( hostType, sessionId, connectReason, eHostJoinInvalidUrl );
            }
        }
        else if( isSearchConnectReason( connectReason ) )
        {
            EHostSearchStatus searchStatus = m_ConnectionMgr.lookupOrQuerySearchId( sessionId, url.c_str(), hostGuid, this, connectReason);
            if( eHostSearchQueryIdSuccess == searchStatus )
            {
                // no need for id query.. just request connection
                onUrlActionQueryIdSuccess( sessionId, url, hostGuid, connectReason );
            }
            else if( eHostSearchQueryIdInProgress == searchStatus )
            {
                // manager is attempting to query id
                m_Engine.getToGui().toGuiHostSearchStatus( hostType, sessionId, eHostSearchQueryIdInProgress );
            }
            else
            {
                m_Engine.getToGui().toGuiHostSearchStatus( hostType, sessionId, eHostSearchInvalidUrl );
                onConnectToHostFail( hostType, sessionId, connectReason, eHostSearchInvalidUrl );
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
        if( isAnnounceConnectReason( connectReason ) )
        {
            onConnectToHostFail( hostType, sessionId, connectReason, eHostAnnounceInvalidUrl );
        }
        else if( isSearchConnectReason( connectReason ) )
        {
            onConnectToHostFail( hostType, sessionId, connectReason, eHostSearchInvalidUrl );
        }
        else if( isJoinConnectReason( connectReason ) )
        {
            onConnectToHostFail( hostType, sessionId, connectReason, eHostJoinInvalidUrl );
        }    
        else
        {
            LogMsg( LOG_ERROR, "Unknown Connect Reason %d", connectReason );
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
    bool isJoinReason = false;
    switch( connectReason )
    {
    case eConnectReasonChatRoomJoin:
    case eConnectReasonGroupJoin:
    case eConnectReasonRandomConnectJoin:
        isJoinReason = true;
        break;
    default:
        break;
    }

    return isJoinReason;
}

//============================================================================
bool HostBaseMgr::isSearchConnectReason( EConnectReason connectReason )
{
    bool isSearchReason = false;
    switch( connectReason )
    {
    case eConnectReasonChatRoomSearch:
    case eConnectReasonGroupSearch:
    case eConnectReasonRandomConnectSearch:
        isSearchReason = true;
        break;
    default:
        break;
    }

    return isSearchReason;
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
    removeSession( sessionId, connectReason );
}

//============================================================================
void HostBaseMgr::onConnectToHostSuccess( EHostType hostType, VxGUID& sessionId, VxSktBase* sktBase, VxGUID& onlineId, EConnectReason connectReason )
{
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
    else if( isSearchConnectReason( connectReason ) )
    {
        m_Engine.getToGui().toGuiHostSearchStatus( hostType, sessionId, eHostSearchConnectSuccess );
        LogMsg( LOG_VERBOSE, "HostBaseMgr connect reason %s to host %s success Default function should be overridden", DescribeConnectReason( connectReason ), DescribeHostType( hostType ) );
    }
    else
    {
        LogMsg( LOG_ERROR, "Unknown Connect Reason %d", connectReason );
    }
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
void HostBaseMgr::onUrlActionQueryIdSuccess( VxGUID& sessionId, std::string& url, VxGUID& onlineId, EConnectReason connectReason )
{
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
    }

    VxSktBase* sktBase = nullptr;
    EConnectStatus connectStatus = m_ConnectionMgr.requestConnection( sessionId, url, onlineId, this, sktBase, connectReason );
    if( eConnectStatusHandshaking == connectStatus )
    {
        if( sktBase )
        {
            onContactHandshaking( sessionId, sktBase, onlineId, connectReason );
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
            else if( isJoinConnectReason( connectReason ) )
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
            onContactConnected( sessionId, sktBase, onlineId, connectReason );
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
            else if( isJoinConnectReason( connectReason ) )
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
        else if( isJoinConnectReason( connectReason ) )
        {
            onConnectToHostFail( hostType, sessionId, connectReason, eHostJoinConnectFailed );
        } 
        else
        {
            LogMsg( LOG_ERROR, "Unknown Connect Reason %d", connectReason );
        }
    }
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
        }
        else if( isSearchConnectReason( connectReason ) )
        {
            m_Engine.getToGui().toGuiHostSearchStatus( hostType, sessionId, eHostSearchQueryIdFailed, commErr, DescribeRunTestStatus( testStatus ) );
        }
        else if( isJoinConnectReason( connectReason ) )
        {
            m_Engine.getToGui().toGuiHostJoinStatus( hostType, sessionId, eHostJoinQueryIdFailed, DescribeRunTestStatus( testStatus ) );
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
    else if( isJoinConnectReason( connectReason ) )
    {
        onConnectToHostFail( hostType, sessionId, connectReason, eHostJoinQueryIdFailed );
    }
    else
    {
        LogMsg( LOG_ERROR, "Unknown Connect Reason %d", connectReason );
    }
}

//============================================================================
void HostBaseMgr::onContactHandshaking( VxGUID& sessionId, VxSktBase* sktBase, VxGUID& onlineId, EConnectReason connectReason )
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
        else if( isJoinConnectReason( connectReason ) )
        {
            m_Engine.getToGui().toGuiHostJoinStatus( hostType, sessionId, eHostJoinHandshaking );
        }
        else
        {
            LogMsg( LOG_ERROR, "Unknown Connect Reason %d", connectReason );
        }
    }
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
        else if( isJoinConnectReason( connectReason ) )
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
    else if( isJoinConnectReason( connectReason ) )
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
    EHostType hostType = connectReasonToHostType( connectReason );
    if( eHostTypeUnknown != hostType )
    {  
        onConnectToHostSuccess( hostType, sessionId, sktBase, onlineId, connectReason);
    }
    else
    {
        LogModule( eLogHostConnect, LOG_DEBUG, "HostBaseMgr connect reason %s unknown host type %d - %s success ", DescribeConnectReason( connectReason ), hostType, DescribeHostType( hostType ) );
    }

    return false;
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
        }
        else if( isJoinConnectReason( connectReason ) )
        {
            m_Engine.getToGui().toGuiHostJoinStatus( hostType, sessionId, eHostJoinFailConnectDropped );
        }
        else if( isSearchConnectReason( connectReason ) )
        {
            m_Engine.getToGui().toGuiHostSearchStatus( hostType, sessionId, eHostSearchFailConnectDropped );
        }
        else
        {
            LogMsg( LOG_ERROR, "Unknown Connect Reason %d", connectReason );
        }

        onConnectToHostSuccess( hostType, sessionId, sktBase, onlineId, connectReason);
    }

    if( eConnectReasonChatRoomJoin == connectReason )
    {
        sendJoinRequest( hostType, sessionId, sktBase, onlineId, connectReason );
    }
}

//============================================================================
void HostBaseMgr::onConnectRequestFail( VxGUID& sessionId, VxGUID& onlineId, EConnectStatus connectStatus, EConnectReason connectReason, ECommErr commErr )
{
    if( eConnectReasonChatRoomAnnounce == connectReason )
    {
        m_Engine.getToGui().toGuiHostAnnounceStatus( eHostTypeChatRoom, sessionId, eHostAnnounceConnectFailed, DescribeConnectStatus( connectStatus ) );
        m_ConnectionMgr.doneWithConnection( sessionId, onlineId, this, connectReason);
    }
    else if( eConnectReasonChatRoomJoin == connectReason )
    {
        m_Engine.getToGui().toGuiHostJoinStatus( eHostTypeChatRoom, sessionId, eHostJoinConnectFailed, DescribeConnectStatus( connectStatus ) );
        m_ConnectionMgr.doneWithConnection( sessionId, onlineId, this, connectReason);
    }
    else if( eConnectReasonChatRoomSearch == connectReason )
    {
        m_Engine.getToGui().toGuiHostSearchStatus( eHostTypeChatRoom, sessionId, eHostSearchConnectFailed, commErr, DescribeConnectStatus( connectStatus ) );
        m_ConnectionMgr.doneWithConnection( sessionId, onlineId, this, connectReason);
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
        m_ConnectionMgr.doneWithConnection( sessionId, onlineId, this, connectReason);
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
        m_ConnectionMgr.doneWithConnection( sessionId, onlineId, this, connectReason);
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

        //User* user = m_Engine.getUserOnlineMgr().findUser( netIdent->getMyOnlineId() );
        //if( user )
        //{
        //    m_UserListMutex.lock();
        //    m_UserList.addOrUpdateUser( user );
        //    m_UserListMutex.unlock();
        //}


        m_Engine.getToGui().toGuiUserOnlineStatus( m_Plugin.getHostType(), netIdent, sessionId, true );
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
