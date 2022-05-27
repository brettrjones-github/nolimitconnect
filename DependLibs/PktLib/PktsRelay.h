#pragma once
//============================================================================
// Copyright (C) 2010 Brett R. Jones 
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

#include "PktTypes.h"
#include "VxCommon.h"

#include <GuiInterface/IDefs.h>

#pragma pack(push)
#pragma pack(1)

class PktRelayUserDisconnect : public VxPktHdr
{
public:
	PktRelayUserDisconnect();

	void						setUserOnlineId( VxGUID& onlineId )		{ m_UserOnlineId = onlineId;  }
	void						setHostOnlineId( VxGUID& onlineId )		{ m_HostOnlineId = onlineId; }

	//=== vars ===//
	VxGUID						m_UserOnlineId;
	VxGUID						m_HostOnlineId;
private:
	uint32_t					m_u32Res1{ 0 };
	uint32_t					m_u32Res2{ 0 };
};

#pragma pack(pop)

