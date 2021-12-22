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

#include "NetServiceDefs.h"

#include <CoreLib/VxGUID.h>
class VxNetIdent;
class NetServiceHdr
{
public:
    NetServiceHdr() = default;

	ENetCmdError                getError() { return m_CmdError; }

	std::string					m_ChallengeHash;
    ENetCmdType					m_NetCmdType{ eNetCmdUnknown };
    int							m_CmdVersion{ 0 };
	ENetCmdError				m_CmdError{ eNetCmdErrorUnknown };
	int							m_TotalDataLen{ 0 };
	int							m_ContentDataLen{ 0 };
	VxGUID					    m_OnlineId;
	VxNetIdent *				m_Ident{ nullptr };
	int							m_SktDataUsed{ 0 };
};

