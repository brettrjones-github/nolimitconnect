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

#include <GuiInterface/IDefs.h>
#include "AudioProcessorDefs.h"
#include <vector>
#include <algorithm>

class AudioIoMgr;
class IAudioCallbacks;

class AudioInputFrame
{
public:
	AudioInputFrame() = default;
	AudioInputFrame( const AudioInputFrame& ) = delete; // don't allow copy constructor
	AudioInputFrame& operator=( const AudioInputFrame& rhs ) = delete;// don't allow copy operation

	void						resetFrame( void );
	int16_t*					getFrameBufferAtFreeIndex( void )		{ return (int16_t *)(&m_AudioBuf[ m_LenWrote ]); }

	bool						isFull( void )							{ return m_LenWrote >= MIXER_CHUNK_LEN_SAMPLES; }
	bool						isSilence( void )						{ return m_PeakAmplitude0to100 < 2; }

	int							audioSamplesInUse()						{ return m_LenWrote; }
	int							audioSamplesFreeSpace( void )			{ return MIXER_CHUNK_LEN_SAMPLES - m_LenWrote; } // how many samples are available to write into frame

	int16_t						m_AudioBuf[ MIXER_CHUNK_LEN_SAMPLES ];
	int							m_LenWrote{ 0 };
	int							m_PeakAmplitude0to100{ 0 };
	int64_t						m_WriteTimestamp{ 0 };
	int							m_WriteDelayMs{ 0 };
	int							m_ClockDrift{ 0 }; // not currently used. // TODO calculate difference between input device clock and output device clock
};
