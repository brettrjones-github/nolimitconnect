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

#include "UserList.h"
#include <PktLib/VxCommon.h>

//============================================================================
UserList::UserList()
{
}

//============================================================================
UserList::~UserList()
{
}

/*
//============================================================================
User UserList::findUser( VxGUID& userId )
{
	for( auto user : m_UserList )
	{
		if( user.getNetIdent() && userId == user.getNetIdent()->getMyOnlineId() )
		{
			return user;
		}
	}

	return nullptr;
}*/

//============================================================================
void UserList::addOrUpdateUser( User& user )
{

}

//============================================================================
void UserList::removeUser( VxGUID& userId )
{

}
