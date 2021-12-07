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

#include "PktTypes.h"
#include "PktsPushToTalk.h"

//============================================================================
PktPushToTalkReq::PktPushToTalkReq()
: m_u8CompressionType(1)
, m_u8CompressionVersion(1)
, m_s64TimeMs(0)
, m_u32Res(0)
{
	setPktType( PKT_TYPE_PUSH_TO_TALK_REQ );
}

//============================================================================
void PktPushToTalkReq::calcPktLen( void )
{
	setPktLength( ROUND_TO_16BYTE_BOUNDRY( (sizeof( PktPushToTalkReq ) - ( sizeof( m_CompressedData ) ) + (getFrame1Len() + getFrame2Len()) ) ) );
}

PktPushToTalkReply::PktPushToTalkReply()
	: m_u16Res1( 0 )
	, m_u32TimeMs( 0 )
	, m_u32Res2( 0 )
	, m_u32Res3( 0 )
	, m_u32Res4( 0 )
{
	setPktType( PKT_TYPE_PUSH_TO_TALK_REPLY );
	setPktLength( sizeof( PktPushToTalkReply ) );
}
