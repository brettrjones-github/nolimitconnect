//============================================================================
// Copyright (C) 2010 Brett R. Jones
// Issued to MIT style license by Brett R. Jones in 2017
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

#include <app_precompiled_hdr.h>
#include "FriendList.h"

//============================================================================
FriendList::FriendList()
	: m_eFriendViewType( eFriendViewEverybody )
{
}

//============================================================================
FriendList::~FriendList()
{
}

//============================================================================
void FriendList::setFriendViewType( EFriendViewType eWhichFriendsToShow )
{
	m_eFriendViewType = eWhichFriendsToShow;
}

//============================================================================
EFriendViewType FriendList::getFriendViewType( void )
{
	return m_eFriendViewType;
}

//============================================================================
//! find friend by id
Friend * FriendList::findFriend( VxGUID& oId )
{
	std::vector<Friend*>::iterator iter;
	for( iter = m_aoFriends.begin(); iter != m_aoFriends.end(); ++iter )
	{
		if( oId == (*iter)->getMyOnlineId() )
		{
			return (*iter);
		}
	}
	return 0;
}

//============================================================================
//! update friend in list
void FriendList::updateFriendList( VxNetIdent * netIdent, bool sessionTimeChange )
{
	std::vector<Friend*>::iterator iter;
	for( iter = m_aoFriends.begin(); iter != m_aoFriends.end(); ++iter )
	{
		if( netIdent->getMyOnlineId() == (*iter)->getMyOnlineId() )
		{
			if( false == sessionTimeChange )
			{
				memcpy( (VxNetIdent *)(*iter), netIdent, sizeof( VxNetIdent) );
				onFriendUpdated( (*iter) );
				return;
			}
			else
			{
				VxNetIdent * removeFriend = (*iter);
				m_aoFriends.erase( iter );
				onFriendRemoved( removeFriend );
				break;
			}
		}
	}

	// not found so add
	Friend * poFriend = new Friend();
	memcpy( (VxNetIdent *) poFriend, netIdent, sizeof( VxNetIdent ) );
	if( ( 0 == m_aoFriends.size() ) || ( 0 == poFriend->getLastSessionTimeMs() ) )
	{
		m_aoFriends.push_back( poFriend );
	}
	else
	{
		bool wasInserted = false;
		for( iter = m_aoFriends.begin(); iter != m_aoFriends.end(); ++iter )
		{
			if( netIdent->getLastSessionTimeMs() < (*iter)->getLastSessionTimeMs() )
			{
				m_aoFriends.insert( iter, poFriend );
				wasInserted = true;
				break;
			}
		}

		if( false == wasInserted )
		{
			m_aoFriends.push_back( poFriend );
		}
	}

	onFriendAdded( poFriend );
}

//============================================================================
//! remove friend from list.. return non zero if should not be removed
RCODE FriendList::removeFriendFromList( VxGUID& oId )
{
	std::vector<Friend*>::iterator iter;
	for( iter = m_aoFriends.begin(); iter != m_aoFriends.end(); ++iter )
	{
		if( oId == (*iter)->getMyOnlineId() )
		{
			onFriendRemoved( (*iter) );
			delete (*iter);
			m_aoFriends.erase( iter );
			return 0;
		}
	}

	return 0;
}
