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

#include "HostList.h"

#include <PktLib/PktAnnounce.h>
#include <CoreLib/VxMacros.h>

#include <memory.h>
#include <string.h>

//============================================================================
HostList::HostList()
{
}

//============================================================================
bool HostList::addEntry( PktAnnounce * pktAnn )
{
	if( MAX_ANCHOR_ENTRIES <= m_EntryCount )
	{
		LogMsg( LOG_ERROR, "ERROR HostList::addEntry: list is full %d\n", m_EntryCount );
		return false;
	}

	VxConnectIdent& baseInfo = *pktAnn;
	HostListEntry * entry = &m_List[m_EntryCount];
	memcpy( &entry->getConnectIdent(), &baseInfo, sizeof( VxConnectIdent ) );	
	m_EntryCount++;
	return true;
}

//============================================================================
uint16_t HostList::calculateLength( void )
{
	m_TotalLen = ROUND_TO_16BYTE_BOUNDRY((sizeof(HostList) - (sizeof(m_List) + sizeof(m_Padding))) + (m_EntryCount * sizeof(HostListEntry)));
    vx_assert( m_TotalLen <= sizeof( HostList ) );
	return m_TotalLen;
}
