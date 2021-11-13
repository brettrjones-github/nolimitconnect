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

#include "FriendListMgr.h"
#include <ptop_src/ptop_engine_src/P2PEngine/P2PEngine.h>

//============================================================================
FriendListMgr::FriendListMgr( P2PEngine& engine )
    : IdentListMgrBase( engine )
{
}

//============================================================================
bool FriendListMgr::isFriend( VxGUID& onlineId )
{
    bool isFriend = false;
    lockList();
    for( auto iter = m_FriendIdentList.begin(); iter != m_FriendIdentList.end(); ++iter )
    {
        if( iter->first == onlineId )
        {
            isFriend = true;
            break;
        }
    }

    unlockList();
    return isFriend;
}

//============================================================================
void FriendListMgr::updateIdent( VxGUID& onlineId, int64_t timestamp )
{
    if( !onlineId.isVxGUIDValid() )
    {
        LogMsg( LOG_ERROR, "IgnoreListMgr::updateIgnoreIdent invalid id" );
        return;
    }

    if( onlineId == m_Engine.getMyOnlineId() )
    {
        LogMsg( LOG_ERROR, "IgnoreListMgr::updateIgnoreIdent cannot change friendship to myself" );
        return;
    }

    bool wasInserted = false;
    bool wasErased = false;
    lockList();
    for( auto iter = m_FriendIdentList.begin(); iter != m_FriendIdentList.end(); )
    {
        if( iter->first == onlineId )
        {
            iter = m_FriendIdentList.erase( iter );
            wasErased = true;
        }
        else if( !wasInserted )
        {
            if( timestamp > iter->second )
            {
                iter = m_FriendIdentList.insert( iter, std::make_pair( onlineId, timestamp ) );
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
        m_FriendIdentList.push_back( std::make_pair( onlineId, timestamp ) );
    }

    unlockList();
}

//============================================================================
void FriendListMgr::removeIdent( VxGUID& onlineId )
{
    lockList();
    for( auto iter = m_FriendIdentList.begin(); iter != m_FriendIdentList.end(); ++iter )
    {
        if( iter->first == onlineId )
        {
            m_FriendIdentList.erase( iter );
            break;
        }
    }

    unlockList();
}
