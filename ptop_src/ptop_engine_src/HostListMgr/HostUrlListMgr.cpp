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
#include <ptop_src/ptop_engine_src/P2PEngine/P2PEngine.h>

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
    for( auto iter = m_HostUrlsList.begin(); iter != m_HostUrlsList.end(); ++iter )
    {
        if( iter->getHostType() == hostType )
        {
            retHostUrls.push_back( *iter );
        }
    }

    return !retHostUrls.empty();
}