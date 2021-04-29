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
UserList::UserList( const UserList& rhs )
    : m_UserList( rhs.m_UserList)
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
void UserList::addOrUpdateUser( User& userIn )
{
    if( !userIn.getNetIdent() )
    {
        LogMsg( LOG_ERROR, "UserList::addOrUpdateUser invalid param " );
        return;
    }

    bool foundUser = false;
    VxGUID onlineId = userIn.getNetIdent()->getMyOnlineId();
    //VxGUID sessionId = userIn.getSessionId();
    for( User& user : m_UserList )
    {
        if( user.getNetIdent() && user.getNetIdent()->getMyOnlineId() == onlineId ) // && user.getSessionId() == sessionId )
        {
            user = userIn;
            foundUser = true;
            break;
        }
    }

    if( !foundUser )
    {
        m_UserList.push_back( userIn );
    }
}

//============================================================================
void UserList::removeUser( VxGUID& onlineId, VxGUID& sessionId )
{
    for( auto iter = m_UserList.begin(); iter != m_UserList.end(); ++iter )
    {
        if( iter->getNetIdent() && iter->getNetIdent()->getMyOnlineId() == onlineId ) // && iter->getSessionId() == sessionId )
        {
            m_UserList.erase( iter );
            break;
        }
    }
}
