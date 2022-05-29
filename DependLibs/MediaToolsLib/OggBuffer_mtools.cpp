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

#include "OggBuffer.h"
#include "MyOpusHeader.h"

#include <CoreLib/VxDebug.h>

#include <string.h>

//============================================================================
OggBuffer::OggBuffer()
: m_DataBuf( 0 )
, m_MaxLen( 0 )
, m_Pos( 0 )
{
}

//============================================================================
int OggBuffer::writeU32( uint32_t val )
{
	if( m_Pos > m_MaxLen - 4 )
		return 0;
	m_DataBuf[ m_Pos     ] = ( val      ) & 0xFF;
	m_DataBuf[ m_Pos + 1 ] = ( val >> 8 ) & 0xFF;
	m_DataBuf[ m_Pos + 2 ] = ( val >> 16 ) & 0xFF;
	m_DataBuf[ m_Pos + 3 ] = ( val >> 24 ) & 0xFF;
	m_Pos += 4;
	return 1;
}

//============================================================================
int OggBuffer::writeU16( uint16_t val )
{
	if( m_Pos > m_MaxLen - 2 )
		return 0;
	m_DataBuf[ m_Pos     ] = ( val      ) & 0xFF;
	m_DataBuf[ m_Pos + 1 ] = ( val >> 8 ) & 0xFF;
	m_Pos += 2;
	return 1;
}

//============================================================================
int OggBuffer::writeChars( const uint8_t * str, int charCount )
{
	if( m_Pos > m_MaxLen - charCount )
		return 0;
	for( int i = 0; i < charCount; i++ )
		m_DataBuf[ m_Pos++ ] = str[i];
	return 1;
}

//============================================================================
int OggBuffer::writeChars( const char * str, int charCount )
{
	if( m_Pos > m_MaxLen - charCount )
		return 0;
	for( int i = 0; i < charCount; i++ )
		m_DataBuf[ m_Pos++ ] = str[i];
	return 1;
}

//============================================================================
int OggBuffer::writeStringLengthThenString( const char * str )
{
	int32_t strLen = (int32_t)strlen( str );
	if( strLen )
	{
		if( m_Pos > m_MaxLen - strLen )
			return 0;
		writeU32( strLen );
		writeChars( (uint8_t *)str, strLen );
		return 1;
	}

	return 0;
}

//============================================================================
int OggBuffer::writeStringLengthThenString( std::string& str )
{
	int32_t strLen = (int32_t)str.length();
	if( str.length() )
	{
		if( m_Pos > m_MaxLen - strLen )
			return 0;
		writeU32( strLen );
		writeChars( (uint8_t *)str.c_str(), strLen );
		return 1;
	}

return 0;
}

//============================================================================
int OggBuffer::readU32( uint32_t * retVal )
{
	if( m_Pos > m_MaxLen - 4 )
		return 0;
	*retVal =  (uint32_t)m_DataBuf[ m_Pos     ];
	*retVal |= (uint32_t)m_DataBuf[ m_Pos + 1 ] <<  8;
	*retVal |= (uint32_t)m_DataBuf[ m_Pos + 2 ] << 16;
	*retVal |= (uint32_t)m_DataBuf [m_Pos + 3 ] << 24;
	m_Pos += 4;
	return 1;
}

//============================================================================
int OggBuffer::readU16( uint16_t * retVal )
{
	if( m_Pos > m_MaxLen - 2 )
		return 0;
	*retVal =  (uint16_t)m_DataBuf[ m_Pos     ];
	*retVal |= (uint16_t)m_DataBuf[ m_Pos + 1 ]<<8;
	m_Pos += 2;
	return 1;
}

//============================================================================
int OggBuffer::readChars( uint8_t * str, int charCount )
{
	if( m_Pos > m_MaxLen - charCount )
		return 0;
	for( int i = 0; i < charCount; i++ )
		str[ i ] = m_DataBuf[ m_Pos++ ];
	return 1;
}

//============================================================================
int OggBuffer::stuffOpusHeaderIntoPacket( MyOpusHeader& opusHeader, uint8_t * packetBuf, int pktBufLen )
{
	uint8_t ch;
	setBuffer( packetBuf, pktBufLen );
	m_DataBuf	= packetBuf;
	m_MaxLen	= pktBufLen;
	m_Pos		= 0;
	if( pktBufLen < 19 )
		return 0;
	if( !writeChars( (const uint8_t*)"OpusHead", 8 ) )
		return 0;
	// version is 1 
	ch = 1;
	if( !writeChars( &ch, 1 ) )
		return 0;

	if( !writeChars( &opusHeader.m_Channels, 1 ) )
		return 0;

	if( !writeU16( opusHeader.m_Preskip ) ) // preskip 312
		return 0;

	if( !writeU32( opusHeader.m_InputSampleRate ))
		return 0;

	if( !writeU16( opusHeader.m_Gain ))
		return 0;

	ch = opusHeader.m_ChannelMapping;
	if( !writeChars( &ch, 1) )
		return 0;

	if( 0 != opusHeader.m_ChannelMapping )
	{
		ch = opusHeader.m_StreamCnt;
		if( !writeChars( &ch, 1 ) )
			return 0;

		ch = opusHeader.m_CoupledCnt;
		if( !writeChars( &ch, 1 ) )
			return 0;

		/* Multi-stream support */
		for( int i = 0; i < opusHeader.m_Channels; i++ )
		{
			if( !writeChars( &opusHeader.m_StreamMap[i], 1 ) )
				return 0;
		}
	}

	return m_Pos;
}

//============================================================================
int OggBuffer::parsePktIntoOpusHeader( uint8_t * packetBuf, int pktBufLen, MyOpusHeader& opusHeader )
{
	char	charBuf[9];
	OggBuffer	oggPkt;
	uint8_t		ch;

	oggPkt.m_DataBuf	= (uint8_t*)packetBuf;
	oggPkt.m_MaxLen		= pktBufLen;
	oggPkt.m_Pos		= 0;
	charBuf[8] = 0;
	if( pktBufLen < 19 )
		return 0;
	oggPkt.readChars( (uint8_t*)charBuf, 8);
	if( 0 != memcmp( charBuf, "OpusHead", 8) )
	{
		LogMsg( LOG_ERROR, "parsePktIntoOpusHeader did not contain OpusHead\n" );
		return 0;
	}

	if( !oggPkt.readChars( &ch, opusHeader.m_Version ) )
		return 0;
	if( 0 != ( opusHeader.m_Version & 240 ) ) 
	{
		LogMsg( LOG_ERROR, "parsePktIntoOpusHeader only major version 0 supported\n" );
		return 0;
	}

	if( !oggPkt.readChars( &ch, opusHeader.m_Channels ) )
		return 0;
	if( 0 == opusHeader.m_Channels )
	{
		LogMsg( LOG_ERROR, "parsePktIntoOpusHeader 0 channels specified\n" );
		return 0;
	}

	if( !oggPkt.readU16( &opusHeader.m_Preskip ) )
		return 0;

	if( !oggPkt.readU32( &opusHeader.m_InputSampleRate ) )
		return 0;

	if( !oggPkt.readU16( &opusHeader.m_Gain ) )
		return 0;

	if( !oggPkt.readChars( &ch, opusHeader.m_ChannelMapping ) )
		return 0;
	if( 0 != opusHeader.m_ChannelMapping )
	{
		if( !oggPkt.readChars( &ch, 1 ) )
			return 0;

		if( ch < 1 )
			return 0;
		opusHeader.m_StreamCnt = ch;

		if( !oggPkt.readChars( &ch, 1))
			return 0;

		if( ch > opusHeader.m_StreamCnt || (ch+opusHeader.m_StreamCnt) > 255 )
			return 0;
		opusHeader.m_CoupledCnt = ch;

		/* Multi-stream support */
		for( int i = 0; i < opusHeader.m_Channels; i++ )
		{
			if( !oggPkt.readChars( &opusHeader.m_StreamMap[i], 1 ) )
				return 0;
			if (opusHeader.m_StreamMap[i] > (opusHeader.m_StreamCnt + opusHeader.m_CoupledCnt) && opusHeader.m_StreamMap[i] != 255 )
				return 0;
		}
	} 
	else 
	{
		if( opusHeader.m_Channels > 2 )
			return 0;
		opusHeader.m_StreamCnt = 1;
		opusHeader.m_CoupledCnt = opusHeader.m_Channels > 1;
		opusHeader.m_StreamMap[0]=0;
		opusHeader.m_StreamMap[1]=1;
	}

	/*For version 0/1 we know there won't be any more data
	so reject any that have data past the end.*/
	if( ( opusHeader.m_Version == 0 || opusHeader.m_Version == 1 ) && oggPkt.getPos() != pktBufLen )
		return 0;
	return 1;
}
