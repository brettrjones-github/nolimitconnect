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

#include "AudioJitterBuffer.h"

#include <MediaToolsLib/SndDefs.h>

// implements circular buffer and delays available data for anti jitter reasons
// buffers are MY_OPUS_PKT_UNCOMPRESSED_DATA_LEN long

//============================================================================
AudioJitterBuffer::AudioJitterBuffer( int queDepth )
: m_BeginSendDepth( 0 )
, m_HeadIdx( 0 )
, m_TailIdx( 0 )
, m_BufsUsed( 0 )
{
	for( int i = 0; i < queDepth; i++ )
	{
		m_BufList.push_back( new char[ MY_OPUS_PKT_UNCOMPRESSED_DATA_LEN] );
	}

	m_BeginSendDepth = ( queDepth * 70 ) / 100;
}

//============================================================================
AudioJitterBuffer::~AudioJitterBuffer()
{
	std::vector<char *>::iterator iter;
	for( iter = m_BufList.begin(); iter != m_BufList.end(); ++iter )
	{
		delete *iter;
	}
}

//============================================================================
void AudioJitterBuffer::clearBuffers( void )  // resets data avail and starts over
{
	m_HeadIdx	= 0;
	m_TailIdx	= 0;
	m_BufsUsed	= 0;
}

//============================================================================
// gets buffer to write to.. if null then overflowed.. increments index if buffer given
char * AudioJitterBuffer::getBufToFill( void )
{
static bool lastWriteSuccess = true;
	char * retBuf = 0;
	if( m_BufsUsed < m_BufList.size() )
	{
		retBuf = m_BufList[ m_HeadIdx ];
		m_HeadIdx++;
		if( m_HeadIdx >= m_BufList.size() )
		{
			m_HeadIdx = 0;
		}

		m_BufsUsed++;
		lastWriteSuccess = true;
	}
	else
	{
		// throw away a buffer
		getBufToRead();
		if( lastWriteSuccess )
		{
			LogMsg( LOG_INFO, "AudioJitterBuffer::getBufToFill overflow\n" );
		}

		lastWriteSuccess = false;
	}

	return retBuf;
}

//============================================================================
// gets buffer to read.. if null then no data available.. increments index if buffer given
char * AudioJitterBuffer::getBufToRead( void )
{
static bool lastReadSuccess = true;
	char * retBuf = 0;
	if( m_BufsUsed >= m_BeginSendDepth )
	{
		retBuf = m_BufList[ m_TailIdx ];
		m_TailIdx++;
		if( m_TailIdx >= m_BufList.size() )
		{
			m_TailIdx = 0;
		}

		m_BufsUsed--;
		lastReadSuccess = true;
	}
	else if( lastReadSuccess )
	{
		lastReadSuccess = false;
		// LogMsg( LOG_INFO, "AudioJitterBuffer::getBufToRead underflow" );
	}

	return retBuf;
}

