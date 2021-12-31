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

#include "HostedListMgr.h"
#include <GuiInterface/IDefs.h>
#include <ptop_src/ptop_engine_src/P2PEngine/P2PEngine.h>
#include <ptop_src/ptop_engine_src/BigListLib/BigListInfo.h>

#include <CoreLib/VxPtopUrl.h>

//============================================================================
HostedListMgr::HostedListMgr( P2PEngine& engine )
    : m_Engine( engine )
{
}

//============================================================================
RCODE HostedListMgr::hostedListMgrStartup( std::string& dbFileName )
{
    RCODE rc = m_HostedInfoListDb.hostedListDbStartup( HOSTED_LIST_DB_VERSION, dbFileName.c_str() );
    m_HostedInfoList.clear();
    m_HostedInfoListDb.getAllHosteds( m_HostedInfoList );
    return rc;
}

//============================================================================
RCODE HostedListMgr::hostedListMgrShutdown( void )
{
    return m_HostedInfoListDb.hostedListDbShutdown();
}

//============================================================================
void HostedListMgr::updateHosted( EHostType hostType, VxGUID& onlineId, std::string& hosted, int64_t timestampMs )
{
    if( !onlineId.isVxGUIDValid() )
    {
        LogMsg( LOG_ERROR, "HostedListMgr::updateDirectConnectIdent invalid id" );
        return;
    }

    bool wasUpdated = false;
    lockList();
    for( auto iter = m_HostedInfoList.begin(); iter != m_HostedInfoList.end(); ++iter )
    {
        if( iter->getHostType() == hostType && iter->getOnlineId() == onlineId )
        {
            iter->setHosted( hosted );
            if( timestampMs )
            {
                iter->setTimestamp( timestampMs );
                m_HostedInfoListDb.saveHosted( *iter );
            }
            
            wasUpdated = true;
            break;
        }
    }

    if( !wasUpdated )
    {
        HostedInfo hostedInfo( hostType, onlineId, hosted, timestampMs );
        m_HostedInfoList.push_back( hostedInfo );
        if( timestampMs )
        {
            m_HostedInfoListDb.saveHosted( hostedInfo );
        }
    }

    unlockList();
}

//============================================================================
bool HostedListMgr::getHosteds( EHostType hostType, std::vector<HostedInfo>& retHosteds )
{
    retHosteds.clear();
    lockList();
    for( auto iter = m_HostedInfoList.begin(); iter != m_HostedInfoList.end(); ++iter )
    {
        if( iter->getHostType() == hostType )
        {
            retHosteds.push_back( *iter );
        }
    }

    unlockList();

    return !retHosteds.empty();
}

//============================================================================
/// return false if one time use and packet has been sent. Connect Manager will disconnect if nobody else needs the connection
bool HostedListMgr::onContactConnected( VxGUID& sessionId, VxSktBase* sktBase, VxGUID& onlineId, EConnectReason connectReason ) 
{ 
    if( eConnectReasonRequestIdentity == connectReason )
    {
        BigListInfo* bigListInfo = m_Engine.getBigListMgr().findBigListInfo( onlineId );
        if( bigListInfo )
        {
            updateHostedList( bigListInfo->getVxNetIdent(), sktBase );
        }

        m_Engine.getConnectionMgr().doneWithConnection( sessionId, onlineId, this, connectReason );
    }

    return false; 
}

//============================================================================
void HostedListMgr::requestIdentity( std::string& url )
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
void HostedListMgr::updateHostedList( VxNetIdent* netIdent, VxSktBase* sktBase )
{
    if( !netIdent || !sktBase )
    {
        LogMsg( LOG_ERROR, "HostedListMgr::updateHosteds null netIdent or sktBase" );
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
                updateHosted( hostType, netIdent->getMyOnlineId(), nodeUrl, sktBase->getLastActiveTimeMs() );
            }
        }
    }
}

//============================================================================
void HostedListMgr::removeClosedPortIdent( VxGUID& onlineId )
{
    lockList();
    for( auto iter = m_HostedInfoList.begin(); iter != m_HostedInfoList.end(); )
    {
        if( iter->getOnlineId() == onlineId )
        {
            m_HostedInfoList.erase( iter );
        }
        else
        {
            ++iter;
        }
    }

    unlockList();
    m_HostedInfoListDb.removeClosedPortIdent( onlineId );
}

//============================================================================
bool HostedListMgr::fromGuiQueryMyHosted( EHostType hostType )
{
    bool result{ false };

    return result;
}

//============================================================================
bool HostedListMgr::fromGuiQueryHosts( VxPtopUrl& netHostUrl, EHostType hostType, VxGUID& hostIdIfNullThenAll )
{
    bool result{ false };

    return result;
}

//============================================================================
void HostedListMgr::clearHostedInfoList( void )
{
    /*
    for( auto iter = m_HostedInfoList.begin(); iter != m_HostedInfoList.end(); ++iter )
    {
        delete ( *iter );
    }
    */

    m_HostedInfoList.clear();
}

//============================================================================
void HostedListMgr::addHostedListMgrClient( HostedListCallbackInterface* client, bool enable )
{
    lockClientList();
    for( auto iter = m_HostedInfoListClients.begin(); iter != m_HostedInfoListClients.end(); ++iter )
    {
        if( *iter == client )
        {
            m_HostedInfoListClients.erase( iter );
            break;
        }
    }

    if( enable )
    {
        m_HostedInfoListClients.push_back( client );
    }

    unlockClientList();
}

/*
//============================================================================
void HostedListMgr::announceHostJoinUpdated( HostJoinInfo* hostInfo )
{
    HostJoinInfo* userHostInfo = dynamic_cast< HostJoinInfo* >( hostInfo );
    if( userHostInfo )
    {
        lockClientList();
        std::vector<HostJoinCallbackInterface*>::iterator iter;
        for( iter = m_HostedInfoListClients.begin(); iter != m_HostedInfoListClients.end(); ++iter )
        {
            HostJoinCallbackInterface* client = *iter;
            client->callbackHostJoinUpdated( userHostInfo );
        }

        unlockClientList();
    }
    else
    {
        LogMsg( LOG_ERROR, "HostJoinMgr::announceHostJoinUpdated dynamic_cast failed" );
    }
}

//============================================================================
void HostJoinMgr::announceHostJoinRemoved( VxGUID& hostOnlineId, EPluginType pluginType )
{
    removeFromDatabase( hostOnlineId, pluginType, false );
    lockClientList();
    std::vector<HostJoinCallbackInterface*>::iterator iter;
    for( iter = m_HostedInfoListClients.begin(); iter != m_HostedInfoListClients.end(); ++iter )
    {
        HostJoinCallbackInterface* client = *iter;
        client->callbackHostJoinRemoved( hostOnlineId, pluginType );
    }

    unlockClientList();
}
*/

