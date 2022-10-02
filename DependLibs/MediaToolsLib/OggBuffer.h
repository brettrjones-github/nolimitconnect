#pragma once
//============================================================================
// Copyright (C) 2015 Brett R. Jones
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

#include <CoreLib/VxDefs.h>

#include <string>

class MyOpusHeader;

class OggBuffer
{
public:
	OggBuffer();
	~OggBuffer() = default;

	void						setDataBuf( uint8_t * dataBuf )				{ m_DataBuf = dataBuf; }
	uint8_t *					getDataBuf( void )							{ return m_DataBuf; }
	void						setMaxLen( int maxLen )						{ m_MaxLen = maxLen; }
	int							getMaxLen( void )							{ return m_MaxLen; }
	void						setBuffer( uint8_t * dataBuf, int len )		{ m_DataBuf = dataBuf; m_MaxLen = len; }

	void						setPos( int pos )							{ m_Pos = pos; }
	int							getPos( void )								{ return m_Pos; }

	int							writeU32( uint32_t val );
	int							writeU16( uint16_t val );
	int							writeChars( const uint8_t * str, int charCount );
	int							writeChars( const char* str, int charCount );
	int							writeStringLengthThenString( const char* str );
	int							writeStringLengthThenString( std::string& str );


	int							readU32( uint32_t * retVal );
	int							readU16( uint16_t * retVal );
	int							readChars( uint8_t * str, int charCount );

	int							stuffOpusHeaderIntoPacket(	MyOpusHeader&			opusHeader, 
															uint8_t *					packetBuf, 
															int						pktBufLen );

	int							parsePktIntoOpusHeader(		uint8_t *					packetBuf, 
															int						pktBufLen, 
															MyOpusHeader&			opusHeader );


	uint8_t *					m_DataBuf;
	int							m_MaxLen;
	int							m_Pos;
};
