//============================================================================
// Copyright (C) 2019 Brett R. Jones 
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

#include "HostListEntry.h"

//============================================================================
HostListEntry::HostListEntry( const HostListEntry& rhs )
: VxConnectIdent( rhs )
, m_ListEntryLen( rhs.m_ListEntryLen )
, m_ListEntryType( rhs.m_ListEntryType )
{
}

//============================================================================
HostListEntry& HostListEntry::operator=( const HostListEntry& rhs )
{
    if( this != &rhs )
    {
        *((VxConnectIdent*)this) = *((VxConnectIdent*)&rhs);
        m_ListEntryLen = rhs.m_ListEntryLen;
        m_ListEntryType = rhs.m_ListEntryType;
    }

    return *this;
}

//============================================================================
void HostListEntry::setListEntryLength( uint16_t entryLen )
{
    m_ListEntryLen = htons( entryLen );
}

//============================================================================
uint16_t HostListEntry::getListEntryLength( void ) const
{
    return ntohs( m_ListEntryLen );
}

//============================================================================
void HostListEntry::setListEntryType( uint16_t entryType )
{
    m_ListEntryType = htons( entryType );
}

//============================================================================
uint16_t HostListEntry::getListEntryType( void ) const
{
    return ntohs( m_ListEntryType );
}
