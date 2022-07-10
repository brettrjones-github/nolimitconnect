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
// http://www.nolimitconnect.org
//============================================================================

#include "VxPktHdr.h"
#include <CoreLib/IsBigEndianCpu.h>

#define PUSH_TO_TALK_PACKET_MAX_COMPRESSED_LEN 1280

#pragma pack(push)
#pragma pack(1)
class PktPushToTalkReq : public VxPktHdr
{
public:
	PktPushToTalkReq();

private:
	//=== vars ===//
	uint32_t					m_u32Res1{ 0 };
	uint32_t					m_u32Res2{ 0 };
};

class PktPushToTalkReply : public VxPktHdr
{
public:
	PktPushToTalkReply();

private:
	//=== vars ===//
	uint32_t					m_u32Res1{ 0 };
	uint32_t					m_u32Res2{ 0 };
};

class PktPushToTalkStart : public VxPktHdr
{
public:
	PktPushToTalkStart();

private:
	//=== vars ===//
	uint32_t					m_u32Res1{ 0 };
	uint32_t					m_u32Res2{ 0 };
};

class PktPushToTalkStop : public VxPktHdr
{
public:
	PktPushToTalkStop();

private:
	//=== vars ===//
	uint32_t					m_u32Res1{ 0 };
	uint32_t					m_u32Res2{ 0 };
};

#pragma pack(pop)



