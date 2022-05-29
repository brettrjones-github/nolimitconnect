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

#include "PktsStoryBoard.h"

//============================================================================
PktStoryDataReq::PktStoryDataReq()
: m_u32TotalDataLen(0)
{
	setPktType( PKT_TYPE_STORY_DATA_REQ );
}

//============================================================================
void PktStoryDataReq::calcPktLen( void )
{
	//calculate length of packet from length of text string
	setPktLength( (unsigned short)ROUND_TO_16BYTE_BOUNDRY( sizeof( PktStoryDataReq ) + getTotalDataLen() ) );
}

//============================================================================
void PktStoryDataReq::setTotalDataLen( uint32_t dataLen )
{
	m_u32TotalDataLen = htonl( dataLen );
}

//============================================================================
uint32_t PktStoryDataReq::getTotalDataLen( void )
{
	return ntohl( m_u32TotalDataLen );
}
