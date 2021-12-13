//============================================================================
// Copyright (C) 2009 Brett R. Jones 
// Issued to MIT style license by Brett R. Jones in 2017
//
// You may use, copy, modify, merge, publish, distribute, sub-license, and/or sell this software 
// provided this Copyright is not modified or removed and is included all copies or substantial portions of the Software
//
// This code is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
//
// bjones.engineer@gmail.com
// http://www.nolimitconnect.com
//============================================================================
#include <app_precompiled_hdr.h>
#include "AppCommon.h"
#include "MySndMgr.h"

#include "ActivityScanWebCams.h"
#include "ToGuiHardwareControlInterface.h"

#include <CoreLib/VxGlobals.h>

//============================================================================
void AppCommon::toGuiWantMicrophoneRecording( EAppModule appModule, bool wantMicInput )
{
	LogMsg( LOG_INFO, "#### AppCommon::toGuiWantMicrophoneRecording %d\n", wantMicInput );
	if( VxIsAppShuttingDown() )
	{
		return;
	}

	emit signalEnableMicrophoneRecording( wantMicInput );
}

//============================================================================
void AppCommon::toGuiMicrophonePeak( EAppModule appModule, int peekVal0to32768 )
{
    if( VxIsAppShuttingDown() )
    {
        return;
    }

    emit signalMicrophonePeak( peekVal0to32768 );
}

//============================================================================
void AppCommon::slotEnableMicrophoneRecording( bool enableMicInput )
{
	if( VxIsAppShuttingDown() )
	{
		return;
	}

	LogMsg( LOG_INFO, "#### AppCommon::slotEnableMicrophoneRecording %d start\n", enableMicInput );
	toGuiHardwareCtrlLock();
	m_MicrophoneHardwareEnabled = enableMicInput;
	std::vector<ToGuiHardwareCtrlClient>::iterator hardwareIter;
	for( hardwareIter = m_ToGuiHardwareCtrlList.begin(); hardwareIter != m_ToGuiHardwareCtrlList.end(); ++hardwareIter )
	{
		ToGuiHardwareCtrlClient& toGuiClient = *hardwareIter;
		toGuiClient.m_Callback->doGuiWantMicrophoneRecording( m_MicrophoneHardwareEnabled );
	}

	toGuiHardwareCtrlUnlock();
	LogMsg( LOG_INFO, "#### AppCommon::slotEnableMicrophoneRecording %d done\n", enableMicInput );
}

//============================================================================
//! called when microphone recoding not needed
void AppCommon::toGuiWantSpeakerOutput( EAppModule appModule, bool wantSpeakerOutput )
{
	LogMsg( LOG_INFO, "#### AppCommon::toGuiWantSpeakerOutput %d\n", wantSpeakerOutput );
	if( VxIsAppShuttingDown() )
	{
		return;
	}

	emit signalEnableSpeakerOutput( wantSpeakerOutput );
}

//============================================================================
void AppCommon::slotEnableSpeakerOutput( bool enableSpeakerOutput )
{
	// do nothing.. always on for windows. for android save power by stopping wave output
	if( VxIsAppShuttingDown() )
	{
		return;
	}

	toGuiHardwareCtrlLock();
	m_SpeakerHardwareEnabled = 	enableSpeakerOutput;
	std::vector<ToGuiHardwareCtrlClient>::iterator hardwareIter;
	for( hardwareIter = m_ToGuiHardwareCtrlList.begin(); hardwareIter != m_ToGuiHardwareCtrlList.end(); ++hardwareIter )
	{
		ToGuiHardwareCtrlClient& toGuiClient = *hardwareIter;
		toGuiClient.m_Callback->doGuiWantSpeakerOutput(	m_SpeakerHardwareEnabled );
	}

	toGuiHardwareCtrlUnlock();
}

//============================================================================
//! playback audio
int AppCommon::toGuiPlayAudio( EAppModule appModule, float * pu16PcmData, int pcmDataLenInBytes )
{
	if( VxIsAppShuttingDown() )
	{
		return 0;
	}

	//LogMsg( LOG_INFO, "slotPlayAudio %d len %d from %s\n", ePluginType, u16PcmDataLen, netIdent->getOnlineName());
	return m_MySndMgr.toGuiPlayAudio( appModule, pu16PcmData, pcmDataLenInBytes );
}

//============================================================================
//! playback audio
int AppCommon::toGuiPlayAudio( EAppModule appModule, int16_t * pu16PcmData, int pcmDataLenInBytes, bool isSilence )
{
    if( VxIsAppShuttingDown() )
    {
        return 0;
    }

    return m_MySndMgr.toGuiPlayAudio( appModule, pu16PcmData, pcmDataLenInBytes, isSilence );
}

//============================================================================
//! playback audio
double AppCommon::toGuiGetAudioDelaySeconds( EAppModule appModule )
{
    if( VxIsAppShuttingDown() )
    {
        return 0;
    }

    return m_MySndMgr.toGuiGetAudioDelaySeconds( appModule );
}

//============================================================================
double AppCommon::toGuiGetAudioCacheTotalSeconds( EAppModule appModule )
{
    if( VxIsAppShuttingDown() )
    {
        return 0;
    }

    return m_MySndMgr.toGuiGetAudioCacheTotalSeconds( appModule );
}

//============================================================================
int AppCommon::toGuiGetAudioCacheFreeSpace( EAppModule appModule )
{
    if( VxIsAppShuttingDown() )
    {
        return 0;
    }

    return m_MySndMgr.toGuiGetAudioCacheFreeSpace( appModule );
}

//============================================================================
/// Microphone sound capture with info for echo cancel ( 8000hz PCM 16 bit data, 80ms of sound )
void AppCommon::fromGuiMicrophoneDataWithInfo( int16_t * pcmData, int pcmDataLenBytes, bool isSilence, int totalDelayTimeMs, int clockDrift )
{
    getEngine().fromGuiMicrophoneDataWithInfo( pcmData, pcmDataLenBytes, isSilence, totalDelayTimeMs, clockDrift );
}

//============================================================================
/// Mute/Unmute microphone
void AppCommon::fromGuiMuteMicrophone( bool muteMic )
{
    getEngine().fromGuiMuteMicrophone( muteMic );
}

//============================================================================
/// Returns true if microphone is muted
bool AppCommon::fromGuiIsMicrophoneMuted( void )
{
    return getEngine().fromGuiIsMicrophoneMuted();
}

//============================================================================
/// Mute/Unmute speaker
void AppCommon::fromGuiMuteSpeaker( bool muteSpeaker )
{
    getEngine().fromGuiMuteSpeaker( muteSpeaker );
}

//============================================================================
/// Returns true if speaker is muted
bool AppCommon::fromGuiIsSpeakerMuted( void )
{
    return getEngine().fromGuiIsSpeakerMuted();
}

//============================================================================
/// Enable/Disable echo cancellation
void AppCommon::fromGuiEchoCancelEnable( bool enableEchoCancel )
{
    getEngine().fromGuiEchoCancelEnable( enableEchoCancel );
}

//============================================================================
/// Returns true if echo cancellation is enabled
bool AppCommon::fromGuiIsEchoCancelEnabled( void )
{
    return getEngine().fromGuiIsEchoCancelEnabled();
}

//============================================================================
/// Called when need more sound for speaker output
void AppCommon::fromGuiAudioOutSpaceAvail( int freeSpaceLen )
{
    getEngine().fromGuiAudioOutSpaceAvail( freeSpaceLen );
}
