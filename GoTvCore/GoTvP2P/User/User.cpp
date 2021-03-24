//============================================================================
// Copyright (C) 2021 Brett R. Jones
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

#include "UserList.h"
#include <PktLib/VxCommon.h>

//============================================================================
User::User( VxSktBase * sktBase, VxNetIdent * netIdent, VxGUID& sessionId, bool online )
    : m_NetIdent( netIdent )
    , m_SktBase( sktBase )
    , m_SessionId( sessionId )
{
}

//============================================================================
User::User( const User& rhs )
    : m_NetIdent( rhs.m_NetIdent )
    , m_SktBase( rhs.m_SktBase )
    , m_SessionId( rhs.m_SessionId )
{
}
