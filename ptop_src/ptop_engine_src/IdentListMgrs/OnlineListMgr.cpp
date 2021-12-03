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

#include "OnlineListMgr.h"
#include <ptop_src/ptop_engine_src/P2PEngine/P2PEngine.h>
#include <ptop_src/ptop_engine_src/BigListLib/BigListInfo.h>

//============================================================================
OnlineListMgr::OnlineListMgr( P2PEngine& engine )
    : IdentListMgrBase( engine )
{
    setIdentListType( eUserViewTypeOnline );
}

//============================================================================
bool OnlineListMgr::isOnline( VxGUID& onlineId )
{
    bool isOnlined = false;
    lockList();
    for( auto iter = m_OnlineIdentList.begin(); iter != m_OnlineIdentList.end(); ++iter )
    {
        if( iter->first == onlineId )
        {
            isOnlined = true;
            break;
        }
    }

    unlockList();
    return isOnlined;
}

//============================================================================
void OnlineListMgr::updateOnlineIdent( VxGUID& onlineId, VxGUID& sktConnectId, int64_t timestamp )
{
    if( !onlineId.isVxGUIDValid() )
    {
        LogMsg( LOG_ERROR, "OnlineListMgr::updateOnlineIdent invalid id" );
        return;
    }

    if( onlineId == m_Engine.getMyOnlineId() )
    {
        LogMsg( LOG_ERROR, "OnlineListMgr::updateOnlineIdent cannot ignore myself" );
        return;
    }

    bool wasInserted = false;
    bool wasErased = false;
    bool timestampUpdated = false;
    lockList();
    for( auto iter = m_OnlineIdentList.begin(); iter != m_OnlineIdentList.end(); )
    {
        if( iter->first == onlineId && iter->second.first == sktConnectId )
        {
            iter = m_OnlineIdentList.erase( iter );
            wasErased = true;
        }
        else if( !wasInserted )
        {
            if( timestamp > iter->second.second )
            {
                iter = m_OnlineIdentList.insert( iter, std::make_pair( onlineId, std::make_pair( sktConnectId, timestamp ) ) );
                timestampUpdated = true;
                wasInserted = true;
            }
            else
            {
                ++iter;
            }
        }
        else
        {
            ++iter;
        }

        if( wasErased && wasInserted )
        {
            break;
        }
    }

    if( !wasInserted )
    {
        m_OnlineIdentList.push_back( std::make_pair( onlineId, std::make_pair( sktConnectId, timestamp ) ) );
        wasInserted = true;
    }

    unlockList();

    if( timestampUpdated || ( wasInserted && !wasErased ) )
    {
        if( wasInserted && !wasErased )
        {
            BigListInfo* bigListInfo = m_Engine.getBigListMgr().findBigListInfo( onlineId );
            if( bigListInfo )
            {
                bigListInfo->setIsOnline( true );
            }
        }

        onUpdateIdent( onlineId, timestamp );
    }
}

//============================================================================
void OnlineListMgr::removeConnection( VxGUID& sktConnectId )
{
    std::vector<VxGUID> identList;
    lockList();
    for( auto iter = m_OnlineIdentList.begin(); iter != m_OnlineIdentList.end(); ++iter )
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
void OnlineListMgr::removeConnection( VxGUID& onlineId, VxGUID& sktConnectId )
{
    bool wasRemoved = false;
    lockList();
    for( auto iter = m_OnlineIdentList.begin(); iter != m_OnlineIdentList.end(); ++iter )
    {
        if( iter->first == onlineId && iter->second.first == sktConnectId )
        {
            m_OnlineIdentList.erase( iter );
            wasRemoved = true;
            break;
        }
    }

    unlockList();

    if( wasRemoved && !isOnline( onlineId ) )
    {
        // all connections to user is lost.. user is now offline
        BigListInfo* bigListInfo = m_Engine.getBigListMgr().findBigListInfo( onlineId );
        if( bigListInfo )
        {
            bigListInfo->setIsOnline( false );
        }

        onRemoveIdent( onlineId );
    }
}


//============================================================================
void OnlineListMgr::removeIdent( VxGUID& onlineId )
{
    bool wasErased = false;
    lockList();
    for( auto iter = m_OnlineIdentList.begin(); iter != m_OnlineIdentList.end(); )
    {
        if( iter->first == onlineId )
        {
            iter = m_OnlineIdentList.erase( iter );
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
        BigListInfo* bigListInfo = m_Engine.getBigListMgr().findBigListInfo( onlineId );
        if( bigListInfo )
        {
            bigListInfo->setIsOnline( false );
        }

        onRemoveIdent( onlineId );
    }
}


//============================================================================
void OnlineListMgr::removeAll( void )
{
    lockList();
    // make copy
    std::vector< std::pair<VxGUID, std::pair<VxGUID, int64_t>>> onlineIdentList = m_OnlineIdentList;
    unlockList();

    for( auto iter = onlineIdentList.begin(); iter != onlineIdentList.end(); ++iter )
    {
        removeIdent( iter->first );
    }
}
