//============================================================================
// Copyright (C) 2014 Brett R. Jones 
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
// http://www.nolimitconnect.org
//============================================================================

#include "PktTypes.h"
#include "PktsMembership.h"

#include <CoreLib/VxDebug.h>

//============================================================================
PktMembershipReq::PktMembershipReq()
{
	setPktType( PKT_TYPE_MEMBERSHIP_REQ );
}

//============================================================================
PktMembershipReply::PktMembershipReply()
{
	vx_assert( 0 == ( sizeof( PktMembershipReply ) & 0x0f ) );
	setPktType( PKT_TYPE_MEMBERSHIP_REPLY );
	setPktLength( sizeof( PktMembershipReply ) );
}

//============================================================================
void PktMembershipReply::setHostMembership( EHostType hostType, EMembershipState membership )
{
	switch( hostType )
	{
	case eHostTypeNetwork:
		m_NetworkMembership = ( uint8_t )membership;
		break;
	case eHostTypeConnectTest:
		m_ConnectTestMembership = ( uint8_t )membership;
		break;
	case eHostTypeGroup:
		m_GroupMembership = ( uint8_t )membership;
		break;
	case eHostTypeChatRoom:
		m_ChatRoomMembership = ( uint8_t )membership;
		break;
	case eHostTypeRandomConnect:
		m_RandomConnectMembership = ( uint8_t )membership;
		break;
	default:
		vx_assert( false );
	}
}

//============================================================================
EMembershipState PktMembershipReply::getHostMembership( EHostType hostType )
{
	switch( hostType )
	{
	case eHostTypeNetwork:
		return ( EMembershipState )m_NetworkMembership;
		break;
	case eHostTypeConnectTest:
		return ( EMembershipState )m_ConnectTestMembership;
		break;
	case eHostTypeGroup:
		return ( EMembershipState )m_GroupMembership;
		break;
	case eHostTypeChatRoom:
		return ( EMembershipState )m_ChatRoomMembership;
		break;
	case eHostTypeRandomConnect:
		return ( EMembershipState )m_RandomConnectMembership;
		break;
	default:
		vx_assert( false );
		return eMembershipStateNone;
	}
}