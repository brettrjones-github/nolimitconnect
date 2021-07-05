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
// http://www.nolimitconnect.com
//============================================================================

#include <CoreLib/VxMutex.h>

#include <vector>

// implements circular buffer and delays available data for anti jitter reasons
// buffers are MY_OPUS_PKT_UNCOMPRESSED_DATA_LEN long
class AudioJitterBuffer 
{
public:
	AudioJitterBuffer( int queDepth );
	~AudioJitterBuffer();

	void						lockResource( void )		{ m_QueMutex.lock(); }
	void						unlockResource( void )		{ m_QueMutex.unlock(); }

	void						clearBuffers( void );  // resets data avail and starts over

	// gets buffer to write to.. if null then overflowed.. increments index if buffer given
	char *						getBufToFill( void );
	// gets buffer to read.. if null then no data available.. increments index if buffer given
	char *						getBufToRead( void );

protected:
	//=== vars ===//
	VxMutex						m_QueMutex;
	std::vector<char *>			m_BufList;
	unsigned int				m_BeginSendDepth;
	unsigned int				m_HeadIdx;
	unsigned int				m_TailIdx;
	unsigned int				m_BufsUsed;
};
