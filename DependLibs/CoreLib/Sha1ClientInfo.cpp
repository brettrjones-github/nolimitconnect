//============================================================================
// Copyright (C) 2022 Brett R. Jones 
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

#include "Sha1ClientInfo.h"

#include <CoreLib/VxFileUtil.h>
#include <CoreLib/VxParse.h>
#include <CoreLib/VxGUID.h>
#include <CoreLib/Invite.h>
#include <CoreLib/VxDebug.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//============================================================================
Sha1ClientInfo::Sha1ClientInfo( VxGUID& fileId, std::string& fileName,  Sha1GeneratorCallback* client )
    : m_Sha1Info( fileId, fileName )
    , m_Client( client )
{
}

//============================================================================
Sha1ClientInfo::Sha1ClientInfo( const Sha1Info& sha1Info, Sha1GeneratorCallback* client )
    : m_Sha1Info( sha1Info )
    , m_Client( client )
{
}

//============================================================================
Sha1ClientInfo::Sha1ClientInfo( const Sha1ClientInfo& rhs )
    : m_Sha1Info( rhs.m_Sha1Info )
    , m_Client( rhs.m_Client )
{
}

//============================================================================
Sha1ClientInfo& Sha1ClientInfo::operator = ( const Sha1ClientInfo& rhs )
{
    if( this != &rhs )
    {
        m_Sha1Info = rhs.m_Sha1Info;
        m_Client = rhs.m_Client;
    }

    return *this;
}

//============================================================================
bool Sha1ClientInfo::operator == ( const Sha1ClientInfo& rhs ) const
{
    return m_Sha1Info == rhs.m_Sha1Info && m_Client == rhs.m_Client;
}

//============================================================================
bool Sha1ClientInfo::isValid( void )
{
    return m_Client != nullptr && m_Sha1Info.isValid();
}