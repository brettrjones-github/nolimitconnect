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
// http://www.nolimitconnect.com
//============================================================================

#include "PktTypes.h"
#include "VxConnectInfo.h"

#pragma pack(1)

class PktNearbyReq : public VxPktHdr
{
public:
	PktNearbyReq();

	void						setTimestamp( uint64_t timeStamp );
	uint64_t					getTimestamp( void );

private:
	uint64_t					m_Timestamp;	
	InetAddrIPv4				m_LanIPv4;
	VxConnectId					m_MyConnectId;
	VxConnectId					m_GroupConnectId;
	char						m_MyOnlineName[MAX_ONLINE_NAME_LEN];	// users online name
	char						m_GroupOnlineName[MAX_ONLINE_NAME_LEN];	// users online name
};

#pragma pack()
