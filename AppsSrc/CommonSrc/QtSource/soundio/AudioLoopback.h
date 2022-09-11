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

#include "AudioMixerInterface.h"
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

class AudioLoopback : public QObject, public AudioMixerInterface
{
	Q_OBJECT
public:
	AudioLoopback( AudioIoMgr& audioIoMgr, QObject* parent = nullptr);

	void						audioLoopbackShutdown( void );

	void                        lockMixer( void )									{ m_MixerMutex.lock(); }
	void                        unlockMixer( void )									{ m_MixerMutex.unlock(); }

	virtual void                microphoneDeviceEnabled( bool isEnabled ) override;
	virtual void                speakerDeviceEnabled( bool isEnabled ) override;

	virtual qint64				readRequestFromSpeaker( char* data, qint64 maxlen ) override;

	virtual void				echoCancelSyncStateThreaded( bool inSync ) override;

	virtual void				frame80msElapsed( void ) override;
	// assumes 80 ms of pcm 8000hz mono audio
	virtual int					toGuiAudioFrameThreaded( EAppModule appModule, int16_t* pcmData, bool isSilenceIn ) override;

	virtual void				fromGuiEchoCanceledSamplesThreaded( int16_t* pcmData, int sampleCnts, bool isSilence ) override;

	void						processAudioLoopbackThreaded( void );

protected:
	int                         incrementMixerWriteIndex( void );
	AudioLoopbackFrame&			getAudioWriteFrame( void )							{ return m_MixerFrames[ m_MixerWriteIdx ]; };

	int                         incrementMixerReadIndex( void );
	AudioLoopbackFrame&			getAudioReadFrame( void )							{ return m_MixerFrames[ m_MixerReadIdx ]; };

	int                         getMixerFrameSize( void )							{ return MIXER_CHUNK_LEN_BYTES; }
	int                         getMixerSamplesPerFrame( void )						{ return MIXER_CHUNK_LEN_SAMPLES; }

	void						processOutSpaceAvailable( void ) {};

	//=== vars ===//
	AudioIoMgr&					m_AudioIoMgr;
	AppCommon&					m_MyApp;

	VxThread					m_ProcessAudioLoopbackThread;
	VxSemaphore					m_AudioLoopbackSemaphore;

	int16_t						m_MixerBuf[ MIXER_CHUNK_LEN_SAMPLES ];
	int16_t						m_QuietEchoBuf[ MIXER_CHUNK_LEN_SAMPLES * 8 ];

	QMutex                      m_MixerMutex;
	AudioLoopbackFrame          m_MixerFrames[ MAX_GUI_MIXER_FRAMES ];
	int                         m_MixerWriteIdx{ 0 };
	int                         m_MixerReadIdx{ 0 };

	int                         m_PrevLerpedSamplesCnt{ 0 };
	int16_t                     m_PrevLerpedSampleValue{ 0 };

	AudioSampleBuf				m_MicSampleBuf;
	AudioSpeakerBuf				m_SpeakerProcessedBuf;
	AudioSpeakerBuf				m_EchoProcessedBuf;
	QMutex                      m_ProcessedBufMutex;

	AudioBitrate                m_EchoCanceledBitrate;
	AudioBitrate                m_ProcessFrameBitrate;
	AudioBitrate                m_ProcessSpeakerBitrate;
	AudioBitrate                m_SpeakerReadBitrate;
};