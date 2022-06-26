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

#include "IGoTvDefs.h"
#include "AudioDefs.h"
#include <vector>
#include <algorithm>

class AudioIoMgr;
class IAudioCallbacks;

class AudioMixerFrame
{
public:
	AudioMixerFrame() = default;

	void						clearFrame( void );

	bool						isSilence( void )						{ return m_InputIds.empty(); }
	bool						hasModuleAudio( EAppModule appModule )	{ return std::find( m_InputIds.begin(), m_InputIds.end(), appModule ) != m_InputIds.end(); }
	bool						hasAnyAudio( void )						{ return !m_InputIds.empty(); }

	// returns number of mixer samples read from frame
	int							readMixerFrame( char* readBuf, bool speakerMuted, int samplesToRead, int upMultiply, int16_t prevSamp, int16_t& retPrevSamp );
	void						writeMixerFrame( EAppModule appModule, char* writeBuf, int dnDivide = 1 );

	// add audio data to mixer.. assumes pcm signed short mono channel 8000 Hz.. return total written to buffer
	int							toMixerPcm8000HzMonoChannel( EAppModule appModule, int16_t* pu16PcmData, bool isSilenceIn );

	int							audioLenInUse( void ); // how many bytes is available and not read by audio out
	int							audioSamplesInUse()							{ return audioLenInUse() / 2; }
	int							audioLenFreeSpace( void ); // how many bytes is available to write into frame

	char						m_MixerBuf[ MIXER_BUF_SIZE_8000_1_S16 ];
	int							m_LenRead{ 0 };
	int							m_LenWrote{ 0 };
	std::vector<EAppModule>		m_InputIds;
};