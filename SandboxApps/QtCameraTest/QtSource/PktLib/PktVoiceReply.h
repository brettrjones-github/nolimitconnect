#ifndef PKT_VOICE_REPLY_H
#define PKT_VOICE_REPLY_H

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

#pragma pack(push)
#pragma pack(1)
class PktVoiceReply : public VxPktHdr
{
public:
	PktVoiceReply();

private:
	//=== vars ===//
	uint16_t							m_u16Res1;
	uint32_t							m_u32TimeMs;
	uint32_t							m_u32Res2;
	uint32_t							m_u32Res3;
	uint32_t							m_u32Res4;
};

#pragma pack(pop)

#endif // PKT_VOICE_REPLY_H