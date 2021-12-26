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

#include "HostUrlListMgr.h"
#include <GuiInterface/IDefs.h>
#include <ptop_src/ptop_engine_src/P2PEngine/P2PEngine.h>
#include <ptop_src/ptop_engine_src/BigListLib/BigListInfo.h>

#include <CoreLib/VxPtopUrl.h>

//============================================================================
HostUrlListMgr::HostUrlListMgr( P2PEngine& engine )
    : m_Engine( engine )
{
}

//============================================================================
RCODE HostUrlListMgr::hostUrlListMgrStartup( std::string& dbFileName )
{
    RCODE rc = m_HostUrlListDb.hostUrlListDbStartup( HOST_URL_LIST_DB_VERSION, dbFileName.c_str() );
    m_HostUrlsList.clear();
    m_HostUrlListDb.getAllHostUrls( m_HostUrlsList );
    return rc;
}

//============================================================================
RCODE HostUrlListMgr::hostUrlListMgrShutdown( void )
{
    return m_HostUrlListDb.hostUrlListDbShutdown();
}

//============================================================================
void HostUrlListMgr::updateHostUrl( EHostType hostType, VxGUID& onlineId, std::string& hostUrl, int64_t timestampMs )
{
    if( !onlineId.isVxGUIDValid() )
    {
        LogMsg( LOG_ERROR, "HostUrlListMgr::updateDirectConnectIdent invalid id" );
        return;
    }

    bool wasUpdated = false;
    lockList();
    for( auto iter = m_HostUrlsList.begin(); iter != m_HostUrlsList.end(); ++iter )
    {
        if( iter->getHostType() == hostType && iter->getOnlineId() == onlineId )
        {
            iter->setHostUrl( hostUrl );
            if( timestampMs )
            {
                iter->setTimestamp( timestampMs );
                m_HostUrlListDb.saveHostUrl( *iter );
            }
            
            wasUpdated = true;
            break;
        }
    }

    if( !wasUpdated )
    {
        HostUrlInfo hostUrlInfo( hostType, onlineId, hostUrl, timestampMs );
        m_HostUrlsList.push_back( hostUrlInfo );
        if( timestampMs )
        {
            m_HostUrlListDb.saveHostUrl( hostUrlInfo );
        }
    }

    unlockList();
}

//============================================================================
bool HostUrlListMgr::getHostUrls( EHostType hostType, std::vector<HostUrlInfo>& retHostUrls )
{
    retHostUrls.clear();
    lockList();
    for( auto iter = m_HostUrlsList.begin(); iter != m_HostUrlsList.end(); ++iter )
    {
        if( iter->getHostType() == hostType )
        {
            retHostUrls.push_back( *iter );
        }
    }

    unlockList();

    return !retHostUrls.empty();
}

//============================================================================
/// return false if one time use and packet has been sent. Connect Manager will disconnect if nobody else needs the connection
bool HostUrlListMgr::onContactConnected( VxGUID& sessionId, VxSktBase* sktBase, VxGUID& onlineId, EConnectReason connectReason ) 
{ 
    if( eConnectReasonRequestIdentity == connectReason )
    {
        BigListInfo* bigListInfo = m_Engine.getBigListMgr().findBigListInfo( onlineId );
        if( bigListInfo )
        {
            updateHostUrls( bigListInfo->getVxNetIdent() );
            m_Engine.getToGui().toGuiContactAdded( bigListInfo->getVxNetIdent() );
        }

        m_Engine.getConnectionMgr().doneWithConnection( sessionId, onlineId, this, connectReason );
    }

    return false; 
}

//============================================================================
void HostUrlListMgr::requestIdentity( std::string& url )
{
    VxPtopUrl ptopUrl( url );
    if( ptopUrl.isValid() )
    {
        // just make up any session.. we only care about the identity then will disconnect
        VxGUID sessionId;
        sessionId.initializeWithNewVxGUID();
        VxSktBase* sktBase{ nullptr };
        m_Engine.getConnectionMgr().requestConnection( sessionId, ptopUrl.getUrl(), ptopUrl.getOnlineId(), this, sktBase, eConnectReasonRequestIdentity );
    }
}

//============================================================================
void HostUrlListMgr::updateHostUrls( VxNetIdent* netIdent )
{
    if( !netIdent )
    {
        LogMsg( LOG_ERROR, "HostUrlListMgr::updateHostUrls null netIdent" );
        return;
    }

    if( netIdent->requiresRelay() )
    {
        removeClosedPortIdent( netIdent->getMyOnlineId() );
    }
    else
    {     
        std::string nodeUrl = netIdent->getMyOnlineUrl();
        for( int i = eHostTypeUnknown + 1; i < eMaxHostType; ++i )
        {
            EHostType hostType = ( EHostType )i;
            if( netIdent->canRequestJoin( hostType ) )
            {
                updateHostUrl( hostType, netIdent->getMyOnlineId(), nodeUrl );
            }
        }
    }
}

//============================================================================
void HostUrlListMgr::removeClosedPortIdent( VxGUID& onlineId )
{
    lockList();
    for( auto iter = m_HostUrlsList.begin(); iter != m_HostUrlsList.end(); )
    {
        if( iter->getOnlineId() == onlineId )
        {
            m_HostUrlsList.erase( iter );
        }
        else
        {
            ++iter;
        }
    }

    unlockList();
    m_HostUrlListDb.removeClosedPortIdent( onlineId );
}
