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
#pragma once

#include <PktLib/VxCommon.h>

#pragma pack(push) 
#pragma pack(1)

class HostListEntry : public VxConnectIdent
{
public:
    HostListEntry() = default;
    HostListEntry( const HostListEntry& rhs );

    HostListEntry& operator=( const HostListEntry& rhs );

    void						setListEntryLength( uint16_t entryLen );
    uint16_t					getListEntryLength( void ) const;

    void						setListEntryType( uint16_t entryType );
    uint16_t					getListEntryType( void ) const;

    //=== vars ===//
    uint16_t                    m_ListEntryLen{ 0 };
    uint16_t					m_ListEntryType{ 0 };			
};

#pragma pack(pop)
