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

#include "ConnectedInfo.h"

#include <GoTvCore/GoTvP2P/BigListLib/BigListInfo.h>
#include <GoTvCore/GoTvP2P/NetServices/NetServicesMgr.h>
#include <GoTvCore/GoTvP2P/NetServices/NetServiceHdr.h>
#include <GoTvCore/GoTvP2P/P2PEngine/P2PEngine.h>

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
ConnectedInfo::ConnectedInfo( P2PEngine& engine, BigListInfo* bigListInfo )
    : m_Engine( engine )
    , m_BigListInfo( bigListInfo )
{
    if( bigListInfo )
    {
        m_PeerOnlineId = bigListInfo->getMyOnlineId();
    }
}

//============================================================================
ConnectedInfo::ConnectedInfo( const ConnectedInfo& rhs )
    : m_Engine( rhs.m_Engine )
    , m_BigListInfo( rhs.m_BigListInfo )
    , m_PeerOnlineId( rhs.m_PeerOnlineId )
{
    m_RmtPlugins = rhs.m_RmtPlugins;
    m_LclList = rhs.m_LclList;
    m_RmtList = rhs.m_RmtList;
    m_SktList = rhs.m_SktList; 
}

//============================================================================
ConnectedInfo& ConnectedInfo::operator=( const ConnectedInfo& rhs )
{
    if( this != &rhs )
    {
        m_BigListInfo = rhs.m_BigListInfo;
        m_PeerOnlineId = rhs.m_PeerOnlineId;
        m_RmtPlugins = rhs.m_RmtPlugins;
        m_LclList = rhs.m_LclList;
        m_RmtList = rhs.m_RmtList;
        m_SktList = rhs.m_SktList; 
    }

    return *this;
}

//============================================================================
bool ConnectedInfo::operator==( const ConnectedInfo& rhs )
{
    return  m_PeerOnlineId == rhs.m_PeerOnlineId;
}

//============================================================================
VxSktBase * ConnectedInfo::getSktBase( void )
{
    if( m_SktList.size() )
    {
        return m_SktList.front();
    }

    return nullptr;
}

//============================================================================
void ConnectedInfo::addConnectReason( IConnectRequestCallback* callback, EConnectReason connectReason )
{
    for( auto &pair : m_LclList )
    {
        if( pair.first == callback && pair.second == connectReason )
        {
            // already in list;
            return;
        }
    }

    m_LclList.push_back( std::make_pair( callback, connectReason ) );
}

//============================================================================
void ConnectedInfo::removeConnectReason( IConnectRequestCallback* callback, EConnectReason connectReason, bool disconnectIfNotInUse )
{
    for( auto iter = m_LclList.begin(); iter != m_LclList.end(); ++iter )
    {
        if( iter->first == callback && iter->second == connectReason )
        {
            m_LclList.erase(iter);
            break;
        }
    }

    if( m_LclList.empty() && m_RmtList.empty() )
    {
        // let the normal socket disconnected code do the work of removing the connection
        for( auto skt : m_SktList )
        {
            if( skt->isConnectSocket() && skt->isConnected() )
            {
                skt->closeSkt(876, true);
            }
        }
    }
}

//============================================================================
void ConnectedInfo::onSktConnected( VxSktBase * sktBase )
{
    bool exists = false;
    for( auto skt : m_SktList )
    {
        if( skt == sktBase )
        {
            exists = true;
            break;
        }
    }

    if( !exists )
    {
        m_SktList.push_back( sktBase );
    }
}

//============================================================================
void ConnectedInfo::onSktDisconnected( VxSktBase * sktBase )
{
    for( auto iter = m_SktList.begin(); iter != m_SktList.end(); ++iter )
    {
        if( *iter == sktBase )
        {
            m_SktList.erase( iter );
            break;
        }
    }
}