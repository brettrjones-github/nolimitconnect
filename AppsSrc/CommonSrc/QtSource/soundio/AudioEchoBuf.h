#pragma once
//============================================================================
// Copyright (C) 2022 Brett R. Jones
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

#include <inttypes.h>

class AudioEchoBuf
{
public:
	AudioEchoBuf() = default;
	AudioEchoBuf( int64_t timeStamp, int16_t* pcmData, int sampleCnt, bool muted );
	AudioEchoBuf( const AudioEchoBuf& rhs );
	~AudioEchoBuf() = default;

	AudioEchoBuf& operator = ( const AudioEchoBuf& rhs );

	void						cleanupBuffer( void );

	int64_t						m_Timestamp{ 0 };
	int16_t*					m_PcmData{ nullptr };
	int							m_SampleCnt{ 0 };
	bool						m_Muted{ false };
};
