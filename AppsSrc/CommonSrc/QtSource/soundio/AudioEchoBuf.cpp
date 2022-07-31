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

#include "AudioEchoBuf.h"

#include <CoreLib/VxTime.h>

//============================================================================
AudioEchoBuf::AudioEchoBuf( int64_t timeStamp, int16_t* pcmData, int sampleCnt, bool muted )
	: m_Timestamp( timeStamp )
	, m_PcmData( pcmData )
	, m_SampleCnt( sampleCnt )
	, m_Muted( muted )
{
}

//============================================================================
AudioEchoBuf::AudioEchoBuf( const AudioEchoBuf& rhs )
	: m_Timestamp( rhs.m_Timestamp )
	, m_PcmData( rhs.m_PcmData )
	, m_SampleCnt( rhs.m_SampleCnt )
	, m_Muted( rhs.m_Muted )
{
}

//============================================================================
AudioEchoBuf& AudioEchoBuf::operator = ( const AudioEchoBuf& rhs )
{
	if( this != &rhs )
	{
		m_Timestamp = rhs.m_Timestamp;
		m_PcmData = rhs.m_PcmData;
		m_SampleCnt = rhs.m_SampleCnt;
		m_Muted = rhs.m_Muted;
	}

	return *this;
}

//============================================================================
void AudioEchoBuf::cleanupBuffer( void )
{
	delete m_PcmData;
	m_PcmData = nullptr;
}