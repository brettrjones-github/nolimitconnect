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

#include "HostInfoBase.h"

#include <GoTvCore/GoTvP2P/P2PEngine/P2PEngine.h>
#include <GoTvCore/GoTvP2P/NetServices/NetServicesMgr.h>
#include <GoTvCore/GoTvP2P/NetServices/NetServiceHdr.h>

#include <NetLib/VxSktConnectSimple.h>
#include <NetLib/VxSktBase.h>

#include <CoreLib/VxParse.h>
#include <CoreLib/VxDebug.h>
#include <CoreLib/VxTime.h>

#include <algorithm>

namespace
{
    const uint64_t HOST_ACTION_TIMEOUT_MS = 30000;
    const uint64_t HOST_CONNECT_ATTEMPT_TIMEOUT_MS = 60000;
}

//============================================================================
HostInfoBase::HostInfoBase( P2PEngine& engine )
    : m_Engine( engine )
{
}

//============================================================================
HostInfoBase::HostInfoBase( const HostInfoBase& rhs )
    : m_Engine( rhs.m_Engine )
{
    *this = rhs;
}

//============================================================================
HostInfoBase::HostInfoBase( P2PEngine& engine, EOtherHostType otherHostType, std::string& hostIp, uint16_t hostPort, const char * hostUrl )
    : m_Engine( engine )
{
    setOtherHostType( otherHostType );
    m_HostIpAddr = hostIp;
    m_HostPort = hostPort;
    m_HostUrl = hostUrl ? hostUrl : "";
}

//============================================================================
HostInfoBase::HostInfoBase( P2PEngine& engine, EOtherHostType otherHostType, VxGUID onlineId, std::string& hostIp, uint16_t hostPort, const char * hostUrl )
    : m_Engine( engine )
{
    setOtherHostType( otherHostType );
    m_OnlineId = onlineId;
    m_HostIpAddr = hostIp;
    m_HostPort = hostPort;
    m_HostUrl = hostUrl ? hostUrl : "";
}

//============================================================================
HostInfoBase::HostInfoBase( P2PEngine& engine, EPluginType ePluginType, VxGUID onlineId, std::string& hostIp, uint16_t hostPort, const char * hostUrl )
    : m_Engine( engine )
{
    addPluginService( ePluginType );
    m_OnlineId = onlineId;
    m_HostIpAddr = hostIp;
    m_HostPort = hostPort;
    m_HostUrl = hostUrl ? hostUrl : "";
}

//============================================================================
HostInfoBase& HostInfoBase::operator=( const HostInfoBase& rhs )
{
    if( this != &rhs )
    {
        m_HostType = rhs.m_HostType;
        m_PluginList = rhs.m_PluginList;
        m_HostUrl = rhs.m_HostUrl;
        m_HostIpAddr = rhs.m_HostIpAddr;
        m_HostPort = rhs.m_HostPort;
        m_OnlineId = rhs.m_OnlineId;
    }

    return *this;
}

//============================================================================
bool HostInfoBase::operator==( const HostInfoBase& rhs )
{
    return  m_HostType == rhs.m_HostType &&
        m_PluginList == rhs.m_PluginList &&
        m_HostUrl == rhs.m_HostUrl &&
        m_HostIpAddr == rhs.m_HostIpAddr &&
        m_HostPort == rhs.m_HostPort &&
        m_OnlineId == rhs.m_OnlineId;
}

//============================================================================
void HostInfoBase::addPluginService( EPluginType pluginType )
{
    if( !hasPluginService( pluginType ) )
    {
        m_PluginList.push_back( pluginType );
    }

    if( m_HostType == eOtherHostUnknown )
    {
        switch( pluginType )
        {
        case ePluginTypeHostChatRoom:
            m_HostType = eOtherHostChatHost;
            break;
        case ePluginTypeHostGroup:
            m_HostType = eOtherHostGroupHost;
            break;
        case ePluginTypeHostConnectTest:
            m_HostType = eOtherHostConnectTest;
            break;
        case ePluginTypeHostNetwork:
            m_HostType = eOtherHostNetworkHost;
            break;

        default:
            // do nothing
            break;
        }
    }
}

//============================================================================
void HostInfoBase::addPluginService( HostInfoBase& rhs )
{
    for( auto pluginType : rhs.getPluginList() )
    {
        addPluginService( pluginType );
    }
}

//============================================================================
void HostInfoBase::removePluginService( EPluginType pluginType )
{
    for( auto iter = m_PluginList.begin(); iter != m_PluginList.end(); ++iter )
    {
        if( *iter == pluginType )
        {
            m_PluginList.erase( iter );
            break;
        }
    }
}

//============================================================================
bool HostInfoBase::hasPluginService( EPluginType pluginType )
{
    for( auto plugin : m_PluginList )
    {
        if( plugin == pluginType )
        {
            return true;
        }
    }

    return false;
}

//============================================================================
void HostInfoBase::setOtherHostType( EOtherHostType hostType )
{
    m_HostType = hostType;

    switch( m_HostType )
    {
    case eOtherHostNetworkHost:
        // network host always provides group listing and optionally connection test
        addPluginService( ePluginTypeNetworkSearchList );
        break;
    case eOtherHostConnectTest:
        addPluginService( ePluginTypeHostConnectTest );
        break;
    case eOtherHostGroupHost:
        addPluginService( ePluginTypeHostGroup );
        break;
    case eOtherHostChatHost:
        addPluginService( ePluginTypeHostChatRoom );
        break;
  
    case eOtherPeerUserHost:
    case eOtherHostUnknown:
    default:
        // do nothing
        break;
    }
}

//============================================================================
/// return true if matches host type and either online id or ip and port
bool HostInfoBase::isMatch( HostInfoBase& rhs )
{
    bool matches = false;
    if( m_HostType == rhs.getOtherHostType() )
    {
        if( m_OnlineId.isVxGUIDValid() && m_OnlineId == rhs.getHostOnlineId() )
        {
            // matches id.. should always be unique
            // TODO check for duplicate GUID Hack
            matches = true;
        }
        else if( !m_HostIpAddr.empty() && m_HostIpAddr == rhs.getHostIpAddr() && m_HostPort == rhs.getHostPort() )
        {
            matches = true;
        }
    }

    return matches;
}

//============================================================================
/// query for host online id
bool HostInfoBase::doActionQueryHostId( VxGUID& retHostId )
{
    VxSktConnectSimple netServConn;
    if( !connectSimpleSkt( netServConn ) )
    {
        LogModule( eLogRunTest, LOG_ERROR, "HostInfoBase::doActionQueryHostId failed connect %s\n", describeHostInfo().c_str());
        return false;
    }

    std::string strNetActionUrl;
    StdStringFormat( strNetActionUrl, "ptop://%s:%d", m_HostIpAddr.c_str(), m_HostPort );

    NetServiceUtils& netUtils( getEngine().getNetServicesMgr().getNetUtils() );
    netUtils.buildQueryHostIdUrl( &netServConn, strNetActionUrl );

    RCODE rc = netServConn.sendData( strNetActionUrl.c_str(), ( int )strNetActionUrl.length() );
    if( rc )
    {
        LogModule( eLogRunTest, LOG_ERROR, "HostInfoBase::doActionQueryHostId: sendData error %d", rc );
        return false;
    }

    char rxBuf[ 513 ];
    rxBuf[ 0 ] = 0;
    NetServiceHdr netServiceHdr;
    if( false == netUtils.rxNetServiceCmd( &netServConn,
                                                    rxBuf,
                                                    sizeof( rxBuf ) - 1,
                                                    netServiceHdr,
                                                    QUERY_HOST_ID_RX_HDR_TIMEOUT,
                                                    QUERY_HOST_ID_RX_DATA_TIMEOUT ) )
    {
        LogModule( eLogRunTest, LOG_ERROR, "HostInfoBase::doActionQueryHostId: Connected to %s but failed to respond (wrong network key ?)", strNetActionUrl.c_str() );
        return false;
    }

    rxBuf[ sizeof( rxBuf ) - 1 ] = 0;
    std::string content = rxBuf;
    if( 0 == content.length() )
    {
        LogModule( eLogRunTest, LOG_ERROR, "QueryHostIdTest: no content in response");
        return false;
    }

    const char * contentBuf = content.c_str();
    if( '/' != contentBuf[ content.length() - 1 ] )
    {
        LogModule( eLogRunTest, LOG_ERROR, "QueryHostIdTest no trailing / in content");
        return false;
    }

    ( ( char * )contentBuf )[ content.length() - 1 ] = 0;

    std::string strPayload = content;
    if( content.empty() )
    {
        LogModule( eLogRunTest, LOG_ERROR, "QueryHostIdTest no host id in content");
        return false;
    }

    VxGUID hostId;
    hostId.fromVxGUIDHexString( strPayload.c_str() );
    if( !hostId.isVxGUIDValid() )
    {
        LogMsg( LOG_ERROR, "Query Host Online Id %s Invalid Content", content.c_str());
        return false;
    }
    else
    {
        retHostId = hostId;
    }

    std::string hostIdStr = hostId.toHexString();
    LogModule( eLogRunTest, LOG_VERBOSE, " success host id %s", hostIdStr.c_str());
    return true;
}

//============================================================================
bool HostInfoBase::connectSimpleSkt( VxSktConnectSimple& sktSimple )
{
    bool result = false;
    sktSimple.closeSkt();
    if( !m_HostIpAddr.empty() && m_HostPort )
    {
        std::string ptopUrl;
        StdStringFormat( ptopUrl, "ptop://%s:%d", m_HostIpAddr.c_str(), m_HostPort );

        std::string strHost;
        std::string strFile;
        uint16_t u16Port;

        if( false == sktSimple.connectToWebsite( ptopUrl.c_str(), strHost, strFile, u16Port, WEBSITE_CONNECT_TIMEOUT ) )
        {
            LogModule( eLogConnect, LOG_ERROR, "HostInfoBase::connectSimpleSkt: FAILED to Connect to %s\n", ptopUrl.c_str() );
            result = false;
        }
        else
        {
            result = true;
        }
    }

    return result;
}

//============================================================================
std::string HostInfoBase::describeHostInfo( void )
{
    std::string url = m_HostUrl.empty() ? "" : m_HostUrl;
    std::string infoDesc;
    StdStringFormat( infoDesc, " %s:%d %s", m_HostIpAddr.c_str(), m_HostPort, url.c_str() );
    return infoDesc;
}

//============================================================================
bool HostInfoBase::addConnectionRequest( EHostConnectType connectType, IHostConnectCallback* connectReq )
{
    bool callbackAdded = false;
    m_CallbackListMutex.lock();

    std::map<EHostConnectType, std::vector<IHostConnectCallback*>>::iterator mapIter = m_CallbackList.find( connectType );

    if( mapIter != m_CallbackList.end() )
    {
        // type already exists
        std::vector<IHostConnectCallback*>& callbackList = mapIter->second;
        for( auto & callback : callbackList )
        {
            bool callbackExists = false;
            if( callback == connectReq )
            {
                // already exists
                callbackExists = true;
                break;
            }

            if( !callbackExists )
            {
                callbackList.push_back( connectReq );
                callbackAdded = true;
            }
        }
    }
    else
    {
        // and new request type and list
        std::vector<IHostConnectCallback*> reqList;
        reqList.push_back( connectReq );
        m_CallbackList.emplace( connectType, reqList );
        callbackAdded = true;
    }

    if( callbackAdded )
    {
        sendConnectionToCallback( connectType, connectReq );
    }

    m_CallbackListMutex.unlock();

    return callbackAdded;
}

//============================================================================
void HostInfoBase::sendConnectionToCallback( EHostConnectType connectType, IHostConnectCallback* connectReq )
{
    if( connectReq && !m_SktList.empty() )
    {
        m_SktListMutex.lock();
        for( VxSktBase* sktBase : m_SktList )
        {
            connectReq->onContactConnected( connectType, sktBase );
        }

        m_SktListMutex.unlock();
    }
}

//============================================================================
bool HostInfoBase::removeConnectionRequest( EHostConnectType connectType, IHostConnectCallback* connectReq )
{
    bool callbackRemoved = false;

    if( connectType != eHostConnectNone )
    {
        m_CallbackListMutex.lock();

        auto mapIter = m_CallbackList.find( connectType );
        if( mapIter != m_CallbackList.end() )
        {
            std::vector<IHostConnectCallback*>& callbackList = mapIter->second;
            for( auto iter = callbackList.begin(); iter != callbackList.end(); ++iter )
            {
                if( *iter == connectReq )
                {
                    // exists so remove it
                    callbackList.erase(iter);
                    callbackRemoved = true;
                    break;
                }
            }
        }

        m_CallbackListMutex.unlock();
    }

    return callbackRemoved;
}

//============================================================================
void HostInfoBase::addToSktListIfSktNotInUse( std::vector<VxSktBase *>& sktList )
{
    bool allRequestsRemoved = true;

    m_CallbackListMutex.lock();
    m_SktListMutex.lock();

    if( !m_SktList.empty() )
    {
        for( auto iter = m_CallbackList.begin(); iter != m_CallbackList.end(); ++iter )
        {
            if( !iter->second.empty() )
            {
                allRequestsRemoved = false;
                break;
            }
        }

        if( allRequestsRemoved )
        {
            for( auto sktBase : m_SktList )
            {
                addToSktListIfMissing( sktList, sktBase );
            }
        }
    }

    m_SktListMutex.unlock();
    m_CallbackListMutex.unlock();
}

//============================================================================
void HostInfoBase::addToSktListIfMissing( std::vector<VxSktBase *>& sktList, VxSktBase* sktBase )
{
    auto iter = std::find( sktList.begin(), sktList.end(), sktBase );

    // If element was found
    if( iter == sktList.end() )
    {
        sktList.push_back( sktBase );
    }
}

//============================================================================
    /// return false if no current requests for given connection
bool HostInfoBase::isSktInUse( VxSktBase* sktBase )
{
    bool allRequestsRemoved = true;

    for( auto iter = m_CallbackList.begin(); iter != m_CallbackList.end(); ++iter )
    {
        if( !iter->second.empty() )
        {
            allRequestsRemoved = false;
            break;
        }
    }

    return allRequestsRemoved;
}


//============================================================================
void HostInfoBase::updateActionTakenTime( void )
{
    m_ActionTimeMs = GetGmtTimeMs();
}

//============================================================================
bool HostInfoBase::isActionTakenTimeExpired( void )
{
    return GetGmtTimeMs() - m_ActionTimeMs > HOST_ACTION_TIMEOUT_MS;
}

//============================================================================
void HostInfoBase::updateConnectAttemptTime( void )
{
    m_ConnectAttemptTimeMs = GetGmtTimeMs();
}

//============================================================================
bool HostInfoBase::isConnectAttemptTimeExpired( void )
{
    return GetGmtTimeMs() - m_ConnectAttemptTimeMs > HOST_CONNECT_ATTEMPT_TIMEOUT_MS;
}

//============================================================================
void HostInfoBase::onSktConnectedWithPktAnn( VxSktBase* sktBase )
{
    if( sktBase && sktBase->getIsPeerPktAnnSet() )
    {
        PktAnnounce& peerPktAnn = sktBase->getPeerPktAnn();
        if( hasHostMatch( peerPktAnn ) )
        {
            // has a service that matches our host type
            bool isSktMatch = false;
            VxGUID peerOnlineId = peerPktAnn.getConnectInfo().getMyOnlineId();
            if( m_OnlineId.isVxGUIDValid() && peerOnlineId.isVxGUIDValid() && ( peerOnlineId == m_OnlineId ) )
            {
                // is a match by online id
                isSktMatch = true;
            }
            else if( m_HostPort && !m_HostIpAddr.empty() )
            {
                // is a match by ip and port ?
                InetAddress inetIp;
                uint16_t peerPort = sktBase->getRemotePort();
                if( peerPort == m_HostPort )
                {
                    std::string rmtIp;
                    sktBase->getRemoteIp( rmtIp );
                    isSktMatch = rmtIp == m_HostIpAddr;
                }
            }
        }
    }
}

//============================================================================
EOtherHostType HostInfoBase::connectTypeToHostType( EHostConnectType connectType )
{
    EOtherHostType hostType = eOtherHostUnknown;
    switch( connectType )
    {
    case eHostConnectRelayFind:
    case eHostConnectRelayJoin:
        // relays mus be provided by group host for now
        hostType = eOtherHostGroupHost;
        break;

    case eHostConnectGroupAnnounce:
    case eHostConnectGroupFind:
        hostType = eOtherHostNetworkHost;
        break;

    case eHostConnectGroupJoin:
        hostType = eOtherHostGroupHost;
        break;

    /// chat rooms can be hosted by groups or individuals in a grou;
    case eHostConnectChatRoomAnnounce:
    case eHostConnectChatRoomFind:
        hostType = eOtherHostGroupHost;
        break;

    case eHostConnectChatRoomJoin:
        hostType = eOtherHostChatHost;
        break;

    default:
        break;
    }

    return hostType;
}

//============================================================================
/// return true if has a service that matches our host type
bool HostInfoBase::hasHostMatch( PktAnnounce& peerPktAnn )
{
    bool hasMatch = false;

    switch( m_HostType )
    {
    case eOtherHostNetworkHost:
        hasMatch = ( eFriendStateIgnore != peerPktAnn.getPluginPermission( ePluginTypeHostNetwork ) ) || ( eFriendStateIgnore != peerPktAnn.getPluginPermission( ePluginTypeNetworkSearchList ) );
        break;
    case eOtherHostConnectTest:
        hasMatch = ( eFriendStateIgnore != peerPktAnn.getPluginPermission( ePluginTypeHostConnectTest ) );
        break;
    case eOtherHostGroupHost:
        hasMatch = ( eFriendStateIgnore != peerPktAnn.getPluginPermission( ePluginTypeHostGroup ) );
        break;
    case eOtherHostChatHost:
        hasMatch = ( eFriendStateIgnore != peerPktAnn.getPluginPermission( ePluginTypeHostChatRoom ) );
        break;

    case eOtherPeerUserHost:
        hasMatch = true;
        break;

    default:
        break;
    }

    return hasMatch;
}

//============================================================================
void HostInfoBase::onSktDisconnected( VxSktBase* sktBase )
{
    if( sktBase )
    {
        m_SktListMutex.lock();

        auto iter = std::find( m_SktList.begin(), m_SktList.end(), sktBase );

        if( iter == m_SktList.end() )
        {
            m_CallbackListMutex.lock();
            for( auto iter = m_CallbackList.begin(); iter != m_CallbackList.end(); ++iter )
            {
                if( !iter->second.empty() )
                {
                    std::vector<IHostConnectCallback*>& callbackList = iter->second;
                    for( IHostConnectCallback* callback : callbackList )
                    {
                        callback->onContactDisconnected( iter->first, sktBase );
                    }
                }
            }

            m_SktList.erase( iter );
            m_CallbackListMutex.unlock();
        }

        m_SktListMutex.unlock();
    }
}
