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

#include "P2PEngine.h"

//============================================================================
VxSktLoopback::VxSktLoopback( P2PEngine& engine )
    : VxSktBase()
    , m_Engine( engine )
{
	m_eSktType = eSktTypeLoopback;
    m_ConnectionId = m_LoopbackConnectId;
}

//============================================================================
RCODE VxSktLoopback::txPacketWithDestId( VxPktHdr * pktHdr, bool bDisconnect )
{
    vx_assert( pktHdr->getDestOnlineId() == m_Engine.getMyOnlineId() );
    m_Engine.handlePkt( this, pktHdr );
    return 0;
}
