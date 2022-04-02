#pragma once

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
// http://www.nolimitconnect.com
//============================================================================

#include <GuiInterface/IDefs.h>

#include "VxPktHdr.h"
#include <CoreLib/IsBigEndianCpu.h>

#pragma pack(push)
#pragma pack(1)
class PktMembershipReq : public VxPktHdr
{
public:
	PktMembershipReq();

private:
	//=== vars ===//
	uint16_t					m_u16Res1{ 0 };
	int64_t						m_s64ResTime{ 0 };
	uint32_t					m_u32Res1{ 0 };
	uint32_t					m_u32Res2{ 0 };
	uint32_t					m_u32Res3{ 0 };
};

class PktMembershipReply : public VxPktHdr
{
public:
	PktMembershipReply();

	void						setCanPushToTalk( bool canPtt ) { m_CanPushToTalk = (bool)( canPtt ); }
	bool 						getCanPushToTalk( void )		{ return  ( bool )( m_CanPushToTalk ); }

	void						setHostMembership( EHostType hostType, EMembershipState membership );
	EMembershipState			getHostMembership( EHostType hostType );

private:
	//=== vars ===//
	uint8_t						m_CanPushToTalk{ 0 };
	uint8_t						m_u8Res1{ 0 };
	int64_t						m_s64ResTime{ 0 };

	uint8_t						m_NetworkMembership{ 0 };
	uint8_t						m_ConnectTestMembership{ 0 };
	uint8_t						m_GroupMembership{ 0 };
	uint8_t						m_ChatRoomMembership{ 0 };
	uint8_t						m_RandomConnectMembership{ 0 };

	uint8_t						m_u8Res2{ 0 };
	uint16_t					m_u16Res1{ 0 };
};

#pragma pack()
#pragma pack(pop)


