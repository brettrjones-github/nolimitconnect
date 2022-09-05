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

// in an ideal world the sound out request for data would be 80ms and everthing would clock off of that
// because all the sound packets assume everbodys sound in in sync
// this class will attempt to call space available every 80 ms in sync with sound consumption by speaker output

#include <GuiInterface/IDefs.h>
#include <ptop_src/ptop_engine_src/MediaProcessor/AudioProcessorDefs.h>

#include "AudioBitrate.h"
#include "AudioDefs.h"
#include "AudioLoopbackFrame.h"
#include "AudioSampleBuf.h"
#include "AudioSpeakerBuf.h"

#include <QTimer>
#include <QElapsedTimer>
#include <QMutex>

#include <CoreLib/VxMutex.h>
#include <CoreLib/VxThread.h>
#include <CoreLib/VxSemaphore.h>

class AppCommon;
class AudioIoMgr;
class RawAudio;

class AudioLoopback : public QObject
{
	Q_OBJECT
public:
	AudioLoopback( AudioIoMgr& audioIoMgr, QObject* parent = nullptr);

	void						audioLoopbackShutdown( void );

	void                        microphoneDeviceEnabled( bool isEnabled );
	void                        speakerDeviceEnabled( bool isEnabled );

	qint64						readRequestFromSpeaker( char* data, qint64 maxlen );

	void						fromGuiEchoCanceledSamplesThreaded( int16_t* pcmData, int sampleCnts, bool isSilence );
	
	void						frame80msElapsed( void );

	void                        lockMixer( void )									{ m_MixerMutex.lock(); }
	void                        unlockMixer( void )									{ m_MixerMutex.unlock(); }

	void						processAudioOutThreaded( void );

	void						echoCancelSyncStateThreaded( bool inSync );

protected:
	int                         getMixerFrameSize( void )							{ return MIXER_CHUNK_LEN_BYTES; }
	int                         getMixerSamplesPerFrame( void )						{ return MIXER_CHUNK_LEN_SAMPLES; }

	int                         getModuleFrameIndex( EAppModule appModule )			{ return m_ModuleBufIndex[ appModule ]; }
	void                        incrementModuleFrameIndex( EAppModule appModule )	{ m_ModuleBufIndex[ appModule ]++; if( m_ModuleBufIndex[ appModule ] >= MAX_MIXER_FRAMES ) m_ModuleBufIndex[ appModule ] = 0; }

	int							audioQueUsedSpace( EAppModule appModule, bool mixerIsLocked );
	int							getDataReadyForSpeakersLen( bool mixerIsLocked );

	int                         incrementMixerWriteIndex( void );
	AudioLoopbackFrame&			getAudioWriteFrame( void )							{ return m_MixerFrames[ m_MixerWriteIdx ];  };
	
	int                         incrementMixerReadIndex( void );
	AudioLoopbackFrame&			getAudioReadFrame( void )							{ return m_MixerFrames[ m_MixerReadIdx ]; };

	void						processOutSpaceAvailable( void ) {};

	//=== vars ===//
	AudioIoMgr&					m_AudioIoMgr;
	AppCommon&					m_MyApp;

	AudioSampleBuf				m_MicSampleBuf;
	int64_t						m_MicInputLastSampleTime{ 0 };

	VxThread					m_ProcessAudioOutThread;
	VxSemaphore					m_AudioOutSemaphore;

	int16_t						m_QuietAudioBuf[ MIXER_CHUNK_LEN_SAMPLES ];
	int16_t						m_MixerBuf[ MIXER_CHUNK_LEN_SAMPLES ];
	int16_t						m_QuietEchoBuf[ MIXER_CHUNK_LEN_SAMPLES * 4 ];
	bool						m_MixerBufUsed;

	QMutex                      m_MixerMutex;
	AudioLoopbackFrame          m_MixerFrames[ MAX_MIXER_FRAMES ];
	int                         m_MixerWriteIdx{ 0 };
	int                         m_MixerReadIdx{ 0 };
	int                         m_ModuleBufIndex[ eMaxAppModule ];

	int                         m_PeakAmplitude{ 0 };

	int                         m_PrevLerpedSamplesCnt{ 0 };
	int16_t                     m_PrevLerpedSampleValue{ 0 };

	bool                        m_WasReset{ true };
	int64_t                     m_LastReadTimeStamp{ 0 };
	int                         m_LastReadSamplesMs{ 0 };

	int64_t                     m_MicQueueSystemTime{ 0 };
	int64_t                     m_MicWriteTime{ 0 };

	AudioSpeakerBuf				m_SpeakerProcessedBuf;
	AudioSampleBuf				m_EchoProcessedBuf;
	QMutex                      m_ProcessedBufMutex;

	AudioBitrate                m_EchoCanceledBitrate;
	AudioBitrate                m_ProcessFrameBitrate;
	AudioBitrate                m_ProcessSpeakerBitrate;
	AudioBitrate                m_SpeakerReadBitrate;
};