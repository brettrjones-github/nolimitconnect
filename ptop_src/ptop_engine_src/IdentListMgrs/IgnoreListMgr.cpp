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

#include "IgnoreListMgr.h"
#include <ptop_src/ptop_engine_src/P2PEngine/P2PEngine.h>

//============================================================================
IgnoreListMgr::IgnoreListMgr( P2PEngine& engine )
    : IdentListMgrBase( engine )
{
    setIdentListType( eFriendListTypeIgnore );
}

//============================================================================
bool IgnoreListMgr::isIgnored( VxGUID& onlineId )
{
    bool isIgnored = false;
    lockList();
    for( auto iter = m_IgnoreIdentList.begin(); iter != m_IgnoreIdentList.end(); ++iter )
    {
        if( iter->first == onlineId )
        {
            isIgnored = true;
            break;
        }
    }

    unlockList();
    return isIgnored;
}

//============================================================================
void IgnoreListMgr::updateIdent( VxGUID& onlineId, int64_t timestamp )
{
    if( !onlineId.isVxGUIDValid() )
    {
        LogMsg( LOG_ERROR, "IgnoreListMgr::updateIgnoreIdent invalid id" );
        return;
    }

    if( onlineId == m_Engine.getMyOnlineId() )
    {
        LogMsg( LOG_ERROR, "IgnoreListMgr::updateIgnoreIdent cannot ignore myself" );
        return;
    }

    bool wasInserted = false;
    bool wasErased = false;
    bool timestampUpdated = false;
    lockList();
    for( auto iter = m_IgnoreIdentList.begin(); iter != m_IgnoreIdentList.end(); )
    {
        if( iter->first == onlineId )
        {
            iter = m_IgnoreIdentList.erase( iter );
            wasErased = true;
        }
        else if( !wasInserted )
        {
            if( timestamp > iter->second )
            {
                iter = m_IgnoreIdentList.insert( iter, std::make_pair( onlineId, timestamp ) );
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
        m_IgnoreIdentList.push_back( std::make_pair( onlineId, timestamp ) );
    }

    unlockList();

    if( timestampUpdated || ( wasInserted && !wasErased ) )
    {
        onUpdateIdent( onlineId, timestamp );
    }
}

//============================================================================
void IgnoreListMgr::removeIdent( VxGUID& onlineId )
{
    bool wasRemoved = false;
    lockList();
    for( auto iter = m_IgnoreIdentList.begin(); iter != m_IgnoreIdentList.end(); ++iter )
    {
        if( iter->first == onlineId )
        {
            m_IgnoreIdentList.erase( iter );
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
