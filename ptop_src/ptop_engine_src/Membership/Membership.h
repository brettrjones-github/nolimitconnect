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
// http://www.nolimitconnect.org
//============================================================================

#include <GuiInterface/IDefs.h>
#include <CoreLib/VxGUID.h>

class MembershipAvailable
{
public:
    MembershipAvailable() = default;

	void						setTimestamp( uint64_t timestamp )		{ m_TimestampMs = timestamp; }
	uint64_t					getTimestamp( void )					{ return m_TimestampMs; }

    void                        setCanPushToTalk( bool canPtt )			{ m_CanPushToTalk = canPtt; }
	bool                        getCanPushToTalk( void )				{ return m_CanPushToTalk; }

    void                        setMembershipState( EHostType hostType, EMembershipState membership );

protected:
	uint64_t					m_TimestampMs{ 0 };
    bool                        m_CanPushToTalk{ false };
	EMembershipState			m_NetworkMembership{ eMembershipStateNone };
	EMembershipState			m_ConnectTestMembership{ eMembershipStateNone };
	EMembershipState			m_GroupMembership{ eMembershipStateNone };
	EMembershipState			m_ChatRoomMembership{ eMembershipStateNone };
	EMembershipState			m_RandomConnectMembership{ eMembershipStateNone };
};

class MembershipHosted
{
public:
	uint64_t					m_TimestampMs{ 0 };
	EMembershipState			m_MembershipHosted{ eMembershipStateNone };
	EHostType					m_HostType{ eHostTypeUnknown };
};
