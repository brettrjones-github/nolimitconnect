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

#include "NearbyListMgr.h"
#include <ptop_src/ptop_engine_src/P2PEngine/P2PEngine.h>
#include <ptop_src/ptop_engine_src/BigListLib/BigListInfo.h>

//============================================================================
NearbyListMgr::NearbyListMgr( P2PEngine& engine )
    : IdentListMgrBase( engine )
{
    setIdentListType( eUserViewTypeNearby );
}

//============================================================================
bool NearbyListMgr::isNearby( VxGUID& onlineId )
{
    bool isNearbyd = false;
    lockList();
    for( auto iter = m_NearbyIdentList.begin(); iter != m_NearbyIdentList.end(); ++iter )
    {
        if( iter->first == onlineId )
        {
            isNearbyd = true;
            break;
        }
    }

    unlockList();
    return isNearbyd;
}

//============================================================================
void NearbyListMgr::updateIdent( VxGUID& onlineId, int64_t timestamp )
{
    if( !onlineId.isVxGUIDValid() )
    {
        LogMsg( LOG_ERROR, "NearbyListMgr::updateNearbyIdent invalid id" );
        return;
    }

    if( onlineId == m_Engine.getMyOnlineId() )
    {
        LogMsg( LOG_ERROR, "NearbyListMgr::updateNearbyIdent cannot ignore myself" );
        return;
    }

    bool wasInserted = false;
    bool wasErased = false;
    bool timestampUpdated = false;
    lockList();
    for( auto iter = m_NearbyIdentList.begin(); iter != m_NearbyIdentList.end(); )
    {
        if( iter->first == onlineId )
        {
            iter = m_NearbyIdentList.erase( iter );
            wasErased = true;
        }
        else if( !wasInserted )
        {
            if( timestamp > iter->second )
            {
                iter = m_NearbyIdentList.insert( iter, std::make_pair( onlineId, timestamp ) );
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
        m_NearbyIdentList.push_back( std::make_pair( onlineId, timestamp ) );
        wasInserted = true;
    }

    unlockList();

    if( timestampUpdated || ( wasInserted && !wasErased ) )
    {
        onUpdateIdent( onlineId, timestamp );
    }
}

//============================================================================
void NearbyListMgr::removeIdent( VxGUID& onlineId )
{
    bool wasRemoved = false;
    lockList();
    for( auto iter = m_NearbyIdentList.begin(); iter != m_NearbyIdentList.end(); ++iter )
    {
        if( iter->first == onlineId )
        {
            m_NearbyIdentList.erase( iter );
            wasRemoved = true;
            break;
        }
    }

    unlockList();

    if( wasRemoved )
    {
        onRemoveIdent( onlineId );
    }
}

//============================================================================
void NearbyListMgr::removeAll( void )
{
    lockList();
    // make copy
    std::vector<std::pair<VxGUID, int64_t>> nearbyIdentList = m_NearbyIdentList;
    unlockList();

    for( auto iter = nearbyIdentList.begin(); iter != nearbyIdentList.end(); ++iter )
    {
        VxGUID onlineId = iter->first;
        BigListInfo* bigListInfo = m_Engine.getBigListMgr().findBigListInfo( onlineId );
        if( bigListInfo )
        {
            bigListInfo->setIsNearby( false );
        }

        removeIdent( onlineId );
    }
}
