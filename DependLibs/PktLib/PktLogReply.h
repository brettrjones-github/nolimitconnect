#pragma once
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
// http://www.nolimitconnect.com
//============================================================================

#include "PktTypes.h"

#pragma pack(1)

#define MAX_LOG_MSG_LEN 2040

class PktLogReply : public VxPktHdr
{
public:
	PktLogReply();

	void						setLogMsg( const char * msg );
	const char *				getLogMsg( void );
	void						setLogLevel( uint16_t u16LogLevel );
	uint16_t					getLogLevel( void );

private:
	uint16_t					m_u16StrLen;								
	uint16_t					m_u16LogLevel;								
	uint32_t					m_u32Res1;								
	uint8_t						m_au8Data[ MAX_LOG_MSG_LEN + 16 ];								
};

#pragma pack()
