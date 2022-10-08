#pragma once
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
// http://www.nolimitconnect.org
//============================================================================

#include "GuiUser.h"

class FriendList
{
public:
	FriendList();
	virtual ~FriendList();

	void						setFriendViewType( EFriendViewType eWhichFriendsToShow );
	EFriendViewType				getFriendViewType( void );

	virtual void				onFriendAdded( GuiUser* poFriend ) {};
	virtual void				onFriendUpdated( GuiUser* poFriend ) {};
	virtual void				onFriendRemoved( GuiUser* poFriend ) {};

	//! find friend by id.. returns NULL if not found
	GuiUser*					findFriend( VxGUID& oId );

protected:
	//! update friend in list
	void						updateFriendList( GuiUser* guiUser, bool sessionTimeChange );
	//! remove friend from list.. return non zero if should not be removed
	RCODE						removeFriendFromList( VxGUID& oId );

	//=== vars ===//
	std::vector<GuiUser*>		m_aoFriends;
	EFriendViewType				m_eFriendViewType;
};