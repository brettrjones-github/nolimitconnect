#pragma once
#include <GuiInterface/IDefs.h>

#include <QElapsedTimer>

class AudioMixerInterface
{
public:
	virtual void                microphoneDeviceEnabled( bool isEnabled ) = 0;
	virtual void                speakerDeviceEnabled( bool isEnabled ) = 0;

	virtual qint64				readRequestFromSpeaker( char* data, qint64 maxlen ) = 0;

	virtual void				echoCancelSyncStateThreaded( bool inSync ) = 0;

	virtual void				frame80msElapsed( void ) = 0;

	// assumes 80 ms of pcm 8000hz mono audio
	virtual int					toGuiAudioFrameThreaded( EAppModule appModule, int16_t* pcmData, bool isSilenceIn ) = 0;

	virtual void				fromGuiEchoCanceledSamplesThreaded( int16_t* pcmData, int sampleCnts, bool isSilence ) = 0;
};