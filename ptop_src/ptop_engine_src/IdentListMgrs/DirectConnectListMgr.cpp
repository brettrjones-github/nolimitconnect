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

#include "DirectConnectListMgr.h"
#include <ptop_src/ptop_engine_src/P2PEngine/P2PEngine.h>
#include <ptop_src/ptop_engine_src/BigListLib/BigListInfo.h>

//============================================================================
DirectConnectListMgr::DirectConnectListMgr( P2PEngine& engine )
    : IdentListMgrBase( engine )
{
    setIdentListType( eUserViewTypeDirectConnect );
}

//============================================================================
bool DirectConnectListMgr::isDirectConnect( VxGUID& onlineId )
{
    bool isDirectConnectd = false;
    lockList();
    for( auto iter = m_DirectConnectIdentList.begin(); iter != m_DirectConnectIdentList.end(); ++iter )
    {
        if( iter->first == onlineId )
        {
            isDirectConnectd = true;
            break;
        }
    }

    unlockList();
    return isDirectConnectd;
}

//============================================================================
void DirectConnectListMgr::updateDirectConnectIdent( VxGUID& onlineId, VxGUID& sktConnectId, int64_t timestamp )
{
    if( !onlineId.isVxGUIDValid() )
    {
        LogMsg( LOG_ERROR, "DirectConnectListMgr::updateDirectConnectIdent invalid id" );
        return;
    }

    if( onlineId == m_Engine.getMyOnlineId() )
    {
        LogMsg( LOG_ERROR, "DirectConnectListMgr::updateDirectConnectIdent cannot ignore myself" );
        return;
    }

    bool wasInserted = false;
    bool wasErased = false;
    bool timestampUpdated = false;
    lockList();
    for( auto iter = m_DirectConnectIdentList.begin(); iter != m_DirectConnectIdentList.end(); )
    {
        if( iter->first == onlineId && iter->second.first == sktConnectId )
        {
            iter = m_DirectConnectIdentList.erase( iter );
            wasErased = true;
        }
        else if( !wasInserted )
        {
            if( timestamp > iter->second.second )
            {
                iter = m_DirectConnectIdentList.insert( iter, std::make_pair( onlineId, std::make_pair( sktConnectId, timestamp ) ) );
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
        m_DirectConnectIdentList.push_back( std::make_pair( onlineId, std::make_pair( sktConnectId, timestamp ) ) );
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
                bigListInfo->setIsDirectConnect( true );
            }
        }

        onUpdateIdent( onlineId, timestamp );
    }
}

//============================================================================
void DirectConnectListMgr::onConnectionLost( VxGUID& sktConnectId )
{
    std::vector<VxGUID> identList;
    lockList();
    for( auto iter = m_DirectConnectIdentList.begin(); iter != m_DirectConnectIdentList.end(); ++iter )
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
void DirectConnectListMgr::removeConnection( VxGUID& onlineId, VxGUID& sktConnectId )
{
    bool wasRemoved = false;
    lockList();
    for( auto iter = m_DirectConnectIdentList.begin(); iter != m_DirectConnectIdentList.end(); ++iter )
    {
        if( iter->first == onlineId && iter->second.first == sktConnectId )
        {
            m_DirectConnectIdentList.erase( iter );
            wasRemoved = true;
            break;
        }
    }

    unlockList();

    if( wasRemoved && !isDirectConnect( onlineId ) )
    {
        // all connections to user is lost.. user is now offline
        BigListInfo* bigListInfo = m_Engine.getBigListMgr().findBigListInfo( onlineId );
        if( bigListInfo )
        {
            bigListInfo->setIsDirectConnect( false );
        }

        onRemoveIdent( onlineId );
    }
}


//============================================================================
void DirectConnectListMgr::removeIdent( VxGUID& onlineId )
{
    bool wasErased = false;
    lockList();
    for( auto iter = m_DirectConnectIdentList.begin(); iter != m_DirectConnectIdentList.end(); )
    {
        if( iter->first == onlineId )
        {
            iter = m_DirectConnectIdentList.erase( iter );
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
            bigListInfo->setIsDirectConnect( false );
        }

        onRemoveIdent( onlineId );
    }
}


//============================================================================
void DirectConnectListMgr::removeAll( void )
{
    lockList();
    // make copy
    std::vector< std::pair<VxGUID, std::pair<VxGUID, int64_t>>> directConnectIdentList = m_DirectConnectIdentList;
    unlockList();

    for( auto iter = directConnectIdentList.begin(); iter != directConnectIdentList.end(); ++iter )
    {
        removeIdent( iter->first );
    }
}
