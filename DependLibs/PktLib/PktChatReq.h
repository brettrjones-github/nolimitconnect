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

#include "VxPktHdr.h"

#define PKT_CHAT_MAX_MSG_LEN			255
#define PKT_CHAT_MAX_IMAGES				20
#define PKT_CHAT_FLAG_IS_ASCII			0x0001
#define PKT_CHAT_FLAG_IS_CHOPPED		0x0002

#pragma pack(push)
#pragma pack(1)
class PktChatReq : public VxPktHdr
{
public:
	PktChatReq();

	char *						getDataPayload( void )				{ return m_au8Additional; }

	int							emptyLen( void );
	int							addImages( int iImageCnt, uint16_t * pu16Images );
	void						addMsg( const char * pMsg );

private:
	//=== vars ===//
	uint16_t					m_u16State;
	uint16_t					m_u16Flags;
	uint16_t					m_u16ImageCnt;
	uint16_t					m_u16Res;
	char						m_au8Additional[ PKT_CHAT_MAX_IMAGES * sizeof(uint16_t) * 2  + PKT_CHAT_MAX_MSG_LEN + 27 ];
};

#pragma pack(pop)




