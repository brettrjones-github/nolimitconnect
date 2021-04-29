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
User::User( VxNetIdent * netIdent )
    : m_NetIdent( netIdent )
{
    if( m_NetIdent )
    {
        m_MyOnlineId = m_NetIdent->getMyOnlineId();
    }
}

//============================================================================
User::User( VxNetIdent * netIdent, BaseSessionInfo& sessionInfo )
    : m_NetIdent( netIdent )
{
    if( m_NetIdent )
    {
        m_MyOnlineId = m_NetIdent->getMyOnlineId();
    }

    m_SessionList.push_back( sessionInfo );
}

//============================================================================
User::User( const User& rhs )
    : m_NetIdent( rhs.m_NetIdent )
    , m_MyOnlineId( rhs.m_MyOnlineId )
    , m_IsOnline( rhs.m_IsOnline )
    , m_SessionList( rhs.m_SessionList )
{
}

//============================================================================
User& User::operator=( const User& rhs ) 
{	
    if( this != &rhs )
    {
        m_NetIdent = rhs.m_NetIdent;
        m_MyOnlineId = rhs.m_MyOnlineId;
        m_IsOnline = rhs.m_IsOnline;
        m_SessionList = rhs.m_SessionList;   
    }

    return *this;
}

//============================================================================
void User::setNetIdent( VxNetIdent * netIdent )   
{ 
    m_NetIdent = netIdent; 
    if( m_NetIdent )
    {
        m_MyOnlineId = netIdent->getMyOnlineId(); 
    }
    else
    {
        m_MyOnlineId.clearVxGUID();
    }
}

//============================================================================
bool User::addSession( BaseSessionInfo& sessionInfo )
{
    lockUser();
    for( BaseSessionInfo& session : m_SessionList )
    {
        if( session == sessionInfo )
        {
            unlockUser();
            return false;
        }
    }

    m_SessionList.push_back( sessionInfo );
    unlockUser();
    return true;
}