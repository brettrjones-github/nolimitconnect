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

#include "AudioBitrate.h"
#include <CoreLib/VxDebug.h>

//============================================================================
void AudioBitrate::addSamplesAndInterval( int sampleCnt, int timeIntervalMs )
{
	if( !m_BitrateLogEnable )
	{
		return;
	}

	if( !sampleCnt || !timeIntervalMs )
	{
		LogMsg( LOG_WARNING, "AudioBitrate::addSamplesAndInterval invalid param" );
		return;
	}

	if( m_SampleCntList.size() >= BITRATE_SAMPLES_PER_LOG_MSG )
	{
		m_SampleCntList.erase( m_SampleCntList.begin() );
	}

	m_SampleCntList.push_back( sampleCnt );

	if( m_IntervalMsList.size() >= BITRATE_SAMPLES_PER_LOG_MSG )
	{
		m_IntervalMsList.erase( m_IntervalMsList.begin() );
	}

	m_IntervalMsList.push_back( timeIntervalMs );

	m_RatesSampled++;
	if( m_RatesSampled >= BITRATE_SAMPLES_PER_LOG_MSG )
	{
		m_RatesSampled = 0;

		int samplesTotal = 0;
		for( auto sampleCnt : m_SampleCntList )
		{
			samplesTotal += sampleCnt;
		}

		int intervalTotalMs = 0;
		for( auto intervalMs : m_IntervalMsList )
		{
			intervalTotalMs += intervalMs;
		}

		int sampleRate = (samplesTotal * 1000) / intervalTotalMs;

		if( sampleRate > 40000 )
		{
			if( sampleRate < 47000 || sampleRate > 49000 )
			{
				LogMsg( LOG_VERBOSE, "%s samples per second %d", m_LogPrefix.c_str(), (samplesTotal * 1000) / intervalTotalMs );
			}		
		}
		else
		{
			if( sampleRate < 7000 || sampleRate > 9000 )
			{
				LogMsg( LOG_VERBOSE, "%s samples per second %d", m_LogPrefix.c_str(), (samplesTotal * 1000) / intervalTotalMs );
			}
		}		
	}
}
