//============================================================================
// Copyright (C) 2003 Brett R. Jones 
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

#include "PktLogReply.h"

#include <CoreLib/VxDebug.h>

#include <string.h>

//============================================================================
PktLogReply::PktLogReply()
: m_u16StrLen(0)
, m_u16LogLevel(0)
, m_u32Res1(0)
{
	setPktType( PKT_TYPE_LOG_REPLY );
	m_au8Data[0] = 0;
}

//============================================================================
void PktLogReply::setLogMsg( const char * msg )
{
	vx_assert( msg );
	uint16_t msgLen = ( uint16_t)strlen( msg );
	vx_assert( MAX_LOG_MSG_LEN > msgLen );
	strcpy( (char *)m_au8Data, msg );
	setPktLength( ROUND_TO_16BYTE_BOUNDRY( sizeof( PktLogReply ) - sizeof(m_au8Data) + msgLen ) ); 
	m_u16StrLen = htons( msgLen );
}

//============================================================================
const char * PktLogReply::getLogMsg( void )
{
	return (const char *)m_au8Data;
}

//============================================================================
void PktLogReply::setLogLevel( uint16_t u16LogLevel )
{
	m_u16LogLevel = htons( u16LogLevel );
}

//============================================================================
uint16_t PktLogReply::getLogLevel( void )
{
	return ntohs( m_u16LogLevel );
}
