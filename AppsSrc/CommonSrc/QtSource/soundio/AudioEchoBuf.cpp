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
#include "AudioEchoCancelImpl.h"

#include <CoreLib/VxTime.h>

int	AudioEchoBuf::m_TotalSpeakerSamples = 0;

//============================================================================
AudioEchoBuf::AudioEchoBuf( int64_t timeStamp, int16_t* pcmData, int sampleCnt, bool isSpeakerSamples )
	: m_Timestamp( timeStamp )
	, m_PcmData( pcmData )
	, m_SampleCnt( sampleCnt )
	, m_IsSpeakerSamples( isSpeakerSamples )
{
	if( isSpeakerSamples )
	{
		m_TotalSpeakerSamples += sampleCnt;
	}
}

//============================================================================
AudioEchoBuf::AudioEchoBuf( const AudioEchoBuf& rhs )
	: m_Timestamp( rhs.m_Timestamp )
	, m_PcmData( rhs.m_PcmData )
	, m_SampleCnt( rhs.m_SampleCnt )
	, m_SamplesRead( rhs.m_SamplesRead )
	, m_IsSpeakerSamples( rhs.m_IsSpeakerSamples )
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
		m_SamplesRead = rhs.m_SamplesRead;
		m_IsSpeakerSamples = rhs.m_IsSpeakerSamples;
	}

	return *this;
}

//============================================================================
void AudioEchoBuf::cleanupBuffer( void )
{
	if( m_IsSpeakerSamples )
	{
		m_TotalSpeakerSamples -= getSamplesAvailable();
	}

	delete m_PcmData;
	m_PcmData = nullptr;
}

//============================================================================
int AudioEchoBuf::readSamples( int16_t* retSamplesBuf, int maxSampleCnt, int64_t timeEst )
{

	int samplesRead{ 0 };
	if( isInTimeRange( timeEst ) )
	{
		samplesRead = std::min( maxSampleCnt, getSamplesAvailable() );
		if( samplesRead )
		{
			memcpy( retSamplesBuf, &m_PcmData[ m_SamplesRead ], samplesRead * 2 );
			m_SamplesRead += samplesRead;
			m_TotalSpeakerSamples -= samplesRead;
		}
	}

	return samplesRead;
}


//============================================================================
bool AudioEchoBuf::isInTimeRange( int64_t& timeEst )
{
	if( timeEst < m_Timestamp - TIME_MS_RANGE_EPSILON )
	{
		// more that epsilon ms before this time stamp
		return false;
	}

	if( timeEst > m_Timestamp + AudioEchoCancelImpl::calculateEchoSamplesMs( m_SampleCnt ) + TIME_MS_RANGE_EPSILON )
	{
		// past the end of buffer time
		return false;
	}

    return true;
}
