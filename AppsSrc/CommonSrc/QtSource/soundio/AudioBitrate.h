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

#include <stdint.h>
#include <vector>
#include <string>

class AudioBitrate
{
public:
	static const int			BITRATE_SAMPLES_PER_LOG_MSG = 10; // if enabled log the average bitrate every 10 sample/interval count

	AudioBitrate( void ) = default;
	~AudioBitrate( void ) = default;

	void						setIsBitrateLogEnabled( bool enable )			{ m_BitrateLogEnable = enable; reset(); }
	bool						isBitrateLogEnabled( void )						{ return m_BitrateLogEnable; }

	void						setLogMessagePrefix( std::string logPrefix )	{ m_LogPrefix = logPrefix; }

	void						reset( void )									{ m_SampleCntList.clear(); m_IntervalMsList.clear(); m_RatesSampled = 0; }

	void						addSamplesAndInterval( int sampleCnt, int timeIntervalMs );	

	bool						m_BitrateLogEnable{ false };
	int							m_RatesSampled{ 0 };
	std::string					m_LogPrefix{ "" };
	std::vector<int>			m_SampleCntList;
	std::vector<int>			m_IntervalMsList;
};

