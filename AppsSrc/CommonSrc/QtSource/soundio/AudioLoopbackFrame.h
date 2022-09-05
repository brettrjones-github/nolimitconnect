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
#include <ptop_src/ptop_engine_src/MediaProcessor/AudioProcessorDefs.h>

#include "AudioDefs.h"
#include <vector>
#include <algorithm>

class AppCommon;
class AudioIoMgr;
class IAudioCallbacks;
class AudioSampleBuf;

class AudioLoopbackFrame
{
public:
	AudioLoopbackFrame();

	void						setAudioIoMgr( AudioIoMgr* audioIoMgr )		{ m_AudioIoMgr = audioIoMgr; }

	void						clearFrame( bool fillSilence );
	bool						isEmpty( void )								{ return m_InputIds.empty(); }

	void						setFrameIndex( int frameIndex )				{ m_FrameIndex = frameIndex; }
	int							getFrameIndex( void )						{ return m_FrameIndex; }

	bool						hasModuleAudio( EAppModule appModule )		{ return std::find( m_InputIds.begin(), m_InputIds.end(), appModule ) != m_InputIds.end(); }
	bool						hasAnyAudio( void )							{ return !m_InputIds.empty(); }

	bool						isProcessed( void )							{ return m_IsProcessed; }

	int							speakerSamplesAvailable( void )				{ return m_SpeakerSamplesWrote - m_SpeakerSamplesRead; }
	int							echoSamplesAvailable( void )				{ return m_MixerSamplesWrote - m_MixerSamplesRead; }

	int16_t*					getSpeakerSampleBuf( void )					{ return m_SpeakerBuf; }
	int16_t*					getEchoSampleBuf( void )					{ return m_MixerBuf; }

	int							readSpeakerData( int16_t* pcmReadData, int speakerSamplesRequested, int& retSpeakerSamplesRead,
												 AudioSampleBuf& echoFarBuf, int echoSamplesRequested, int& retEchoSamplesRead, int& retPeakValue0to100 );

	// add audio data to mixer.. assumes pcm signed short mono channel 8000 Hz.. return total written to buffer
	int							toMixerPcm8000HzMonoChannel( EAppModule appModule, int16_t* pcmData, bool isSilenceIn );

	int							audioLenInUse( void ); // how many bytes is available and not read by audio out
	int							audioSamplesInUse()							{ return audioLenInUse() / 2; }
	int							audioLenFreeSpace( void ); // how many bytes is available to write into frame

	void						processFrameForSpeakerOutputThreaded( int16_t prevFrameSample );

	int16_t						getLastEchoSample( void );

	void						speakerSamplesWereRead( int samplesRead )	{ m_SpeakerSamplesRead += samplesRead;  }
	void						echoSamplesWereRead( int samplesRead )		{ m_MixerSamplesRead += samplesRead; }

	AudioIoMgr*					m_AudioIoMgr{ nullptr };

	int16_t						m_MixerBuf[ MIXER_CHUNK_LEN_SAMPLES ];
	int							m_MixerSamplesRead{ 0 };
	int							m_MixerSamplesWrote{ 0 };
	std::vector<EAppModule>		m_InputIds;

	// after processed to prepare for speakerRead
	bool						m_IsProcessed{ false };
	int16_t						m_SpeakerBuf[ SPEAKER_CHUNK_LEN_SAMPLES ];
	int							m_SpeakerSamplesRead{ 0 };
	int							m_SpeakerSamplesWrote{ 0 };
	bool						m_IsSilentSamples{ true };

	int							m_PeakValue0to100{ 0 };
	int							m_FrameIndex{ 0 };
};
