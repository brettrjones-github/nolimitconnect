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
#include "PktsPushToTalk.h"

#include <CoreLib/VxDebug.h>

//============================================================================
PktPushToTalkReq::PktPushToTalkReq()
{
	vx_assert( 0 == (sizeof( PktPushToTalkReq ) & 0x0f) );
	setPktType( PKT_TYPE_PUSH_TO_TALK_REQ );
	setPktLength( sizeof( PktPushToTalkReq ) );
}

//============================================================================
PktPushToTalkReply::PktPushToTalkReply()
{
    vx_assert( 0 == (sizeof( PktPushToTalkReply ) & 0x0f) );
	setPktType( PKT_TYPE_PUSH_TO_TALK_REPLY );
	setPktLength( sizeof( PktPushToTalkReply ) );
}

//============================================================================
PktPushToTalkStart::PktPushToTalkStart()
{
	vx_assert( 0 == (sizeof( PktPushToTalkStart ) & 0x0f) );
	setPktType( PKT_TYPE_PUSH_TO_TALK_START );
	setPktLength( sizeof( PktPushToTalkStart ) );
}

//============================================================================
PktPushToTalkStop::PktPushToTalkStop()
{
	vx_assert( 0 == (sizeof( PktPushToTalkStop ) & 0x0f) );
	setPktType( PKT_TYPE_PUSH_TO_TALK_STOP );
	setPktLength( sizeof( PktPushToTalkStop ) );
}