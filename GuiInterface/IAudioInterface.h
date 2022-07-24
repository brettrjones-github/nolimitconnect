#pragma once
//============================================================================
// Copyright (C) 2019 Brett R. Jones
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

#include <typeinfo>
#include <inttypes.h>

class IAudioCallbacks
{
public:
    /// Microphone sound capture ( 8000hz PCM 16 bit data, 80ms of sound )
    //virtual void				fromGuiMicrophoneData( int16_t* pcmData, uint16_t pcmDataLenBytes, bool isSilence ) = 0;
    /// Microphone sound capture with info for echo cancel ( 8000hz PCM 16 bit data, 80ms of sound )
    virtual void				fromGuiMicrophoneDataWithInfo( int16_t * pcmData, int pcmDataLenBytes, bool isSilence, int totalDelayTimeMs, int clockDrift ) = 0;
    /// Mute/Unmute microphone
    virtual void				fromGuiMuteMicrophone( bool muteMic ) = 0;
    /// Returns true if microphone is muted
    virtual bool				fromGuiIsMicrophoneMuted( void ) = 0;
    /// Mute/Unmute speaker
    virtual void				fromGuiMuteSpeaker( bool muteSpeaker ) = 0;
    /// Returns true if speaker is muted
    virtual bool				fromGuiIsSpeakerMuted( void ) = 0;
    /// Called when need more sound for speaker output
    virtual void				fromGuiAudioOutSpaceAvail( int freeSpaceLen ) = 0;
};

class IAudioRequests
{
public:
    // enable disable fromGuiMicrophoneData callback
    virtual void				toGuiWantMicrophoneRecording( EAppModule appModule, bool wantMicInput ) = 0;
    // enable disable speaker sound out
    virtual void				toGuiWantSpeakerOutput( EAppModule appModule, bool wantSpeakerOutput ) = 0;

#if ENABLE_KODI
    // add audio data to play.. assumes float 2 channel 48000 Hz
    //virtual int				    toGuiPlayAudio( EAppModule appModule, float * audioSamples48000, int dataLenInBytes ) = 0;
#endif // ENABLE_KODI

    // add audio data to play.. assumes pcm mono 8000 Hz
    virtual int				    toGuiPlayAudio( EAppModule appModule, int16_t * pu16PcmData, int pcmDataLenInBytes, bool isSilence ) = 0;
    // delay of audio calculated from amount of data in queue
    virtual double				toGuiGetAudioDelaySeconds( EAppModule appModule ) = 0;
    // maximum queue cache size in seconds
    virtual double				toGuiGetAudioCacheTotalSeconds( EAppModule appModule ) = 0;
    // maximum queue cache size in seconds
    virtual double				toGuiGetAudioCacheTotalMs( void ) = 0;
    // amount of free queue space in bytes
    virtual int				    toGuiGetAudioCacheFreeSpace( EAppModule appModule ) = 0;

};
