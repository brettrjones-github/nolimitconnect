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
#include "PktChatReq.h"

#include <CoreLib/VxParse.h>

#include <memory.h>

PktChatReq::PktChatReq()
: m_u16State(1)
, m_u16Flags(PKT_CHAT_FLAG_IS_ASCII)
, m_u16ImageCnt(0)
, m_u16Res(0)
{
	setPktType(  PKT_TYPE_CHAT_REQ ); 
}

//============================================================================
int PktChatReq::emptyLen( void )
{ 
	return sizeof( PktChatReq ) - sizeof( m_au8Additional ); 
} 

//============================================================================
int PktChatReq::addImages( int iImageCnt, uint16_t * pu16Images )
{
	setPktLength( emptyLen() );
	m_u16ImageCnt = (uint16_t)iImageCnt;
	if( iImageCnt )
	{
		memcpy( m_au8Additional, pu16Images, iImageCnt * 2 * sizeof( uint16_t ) );
		return 0;
	}
	return -1;
}

//============================================================================
void PktChatReq::addMsg( const char* pMsg )
{
	int iLen = (int)strlen( pMsg );
	if( iLen < PKT_CHAT_MAX_MSG_LEN )
	{
		strcpy( (char *)m_au8Additional, pMsg );
	}
	else
	{
		iLen = PKT_CHAT_MAX_MSG_LEN - 1;
		strncpy( (char *)m_au8Additional, pMsg,  iLen );
		m_au8Additional[ PKT_CHAT_MAX_MSG_LEN ] = 0;
	}

	setPktLength( ROUND_TO_16BYTE_BOUNDRY( emptyLen() + iLen + 1 ) );
}
