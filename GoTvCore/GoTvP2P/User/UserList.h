#pragma once
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

#include "User.h"

#include <CoreLib/VxMutex.h>

class UserList
{
public:
    UserList();
	virtual ~UserList();

    /*
    User					    findUser( VxGUID& userId );
    */
    void 				        addOrUpdateUser( User& user );
    void 				        removeUser( VxGUID& userId );

protected:

	//=== vars ===//
	std::vector<User>		    m_UserList;
    VxMutex                     m_UserMutex;

};