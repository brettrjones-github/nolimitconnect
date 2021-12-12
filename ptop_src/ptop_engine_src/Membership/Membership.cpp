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

#include "Membership.h"

#include <CoreLib/VxDebug.h>

//============================================================================

//============================================================================
void MembershipAvailable::setMembershipState( EHostType hostType, EMembershipState membership )
{
	switch( hostType )
	{
	case eHostTypeNetwork:
		m_NetworkMembership = membership;
		break;
	case eHostTypeConnectTest:
		m_ConnectTestMembership = membership;
		break;
	case eHostTypeGroup:
		m_GroupMembership = membership;
		break;
	case eHostTypeChatRoom:
		m_ChatRoomMembership = membership;
		break;
	case eHostTypeRandomConnect:
		m_RandomConnectMembership = membership;
		break;
	default:
		vx_assert( false );
	}
}


