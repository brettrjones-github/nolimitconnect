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
// http://www.nolimitconnect.org
//============================================================================

#include "AppCommon.h"
#include "SoundMgr.h"

#include "ActivityScanWebCams.h"
#include "ToGuiHardwareControlInterface.h"

#include <CoreLib/VxGlobals.h>

//============================================================================
void AppCommon::toGuiWantMicrophoneRecording( EAppModule appModule, bool wantMicInput )
{
	LogMsg( LOG_INFO, "#### AppCommon::toGuiWantMicrophoneRecording %d", wantMicInput );
	if( VxIsAppShuttingDown() )
	{
		return;
	}

	emit signalInternalWantMicrophoneRecording( appModule, wantMicInput );
}

//============================================================================
void AppCommon::slotInternalWantMicrophoneRecording( EAppModule appModule, bool wanteMicInput )
{
	if( VxIsAppShuttingDown() )
	{
		return;
	}

	bool wasMicAvailable = m_SoundMgr.isMicrophoneEnabled();
	m_SoundMgr.toGuiWantMicrophoneRecording( appModule, wanteMicInput );
	bool isMicAvailable = m_SoundMgr.isMicrophoneEnabled();
	if( wasMicAvailable != isMicAvailable )
	{
		for( auto hardwareIter = m_ToGuiHardwareCtrlList.begin(); hardwareIter != m_ToGuiHardwareCtrlList.end(); ++hardwareIter )
		{
			ToGuiHardwareControlInterface* toGuiClient = *hardwareIter;
			toGuiClient->callbackToGuiWantMicrophoneRecording( isMicAvailable );
		}

		LogMsg( LOG_INFO, "#### AppCommon::slotEnableMicrophoneRecording %d done", isMicAvailable );
	}
}

//============================================================================
void AppCommon::toGuiWantSpeakerOutput( EAppModule appModule, bool wantSpeakerOutput )
{
	LogMsg( LOG_INFO, "#### AppCommon::toGuiWantSpeakerOutput %d", wantSpeakerOutput );
	if( VxIsAppShuttingDown() )
	{
		return;
	}

	emit signalInternalWantSpeakerOutput( appModule, wantSpeakerOutput );
}

//============================================================================
void AppCommon::slotInternalWantSpeakerOutput( EAppModule appModule, bool enableSpeakerOutput )
{
	// do nothing.. always on for windows. for android save power by stopping wave output
	if( VxIsAppShuttingDown() )
	{
		return;
	}

	bool wasSpeakerAvailable = m_SoundMgr.isSpeakerEnabled();
	m_SoundMgr.toGuiWantSpeakerOutput( appModule, enableSpeakerOutput );
	bool isSpeakerAvailable = m_SoundMgr.isSpeakerEnabled();

	if( wasSpeakerAvailable != isSpeakerAvailable )
	{
		for( auto hardwareIter = m_ToGuiHardwareCtrlList.begin(); hardwareIter != m_ToGuiHardwareCtrlList.end(); ++hardwareIter )
		{
			ToGuiHardwareControlInterface* toGuiClient = *hardwareIter;
			toGuiClient->callbackToGuiWantSpeakerOutput( isSpeakerAvailable );
		}
	}
}

//============================================================================
//! playback audio
int AppCommon::toGuiPlayAudio( EAppModule appModule, int16_t * pu16PcmData, int pcmDataLenInBytes, bool isSilence )
{
    if( VxIsAppShuttingDown() )
    {
        return 0;
    }

    return m_SoundMgr.toGuiPlayAudio( appModule, pu16PcmData, pcmDataLenInBytes, isSilence );
}


#if ENABLE_KODI
//============================================================================
//! playback audio
int AppCommon::toGuiPlayAudio( EAppModule appModule, float* pu16PcmData, int pcmDataLenInBytes )
{
	if( VxIsAppShuttingDown() )
	{
		return 0;
	}

	//LogMsg( LOG_INFO, "slotPlayAudio %d len %d from %s\n", ePluginType, u16PcmDataLen, netIdent->getOnlineName());
	return m_SoundMgr.toGuiPlayAudio( appModule, pu16PcmData, pcmDataLenInBytes );
}
#endif // ENABLE_KODI

//============================================================================
//! playback audio
double AppCommon::toGuiGetAudioDelaySeconds( EAppModule appModule )
{
    if( VxIsAppShuttingDown() )
    {
        return 0;
    }

    return m_SoundMgr.toGuiGetAudioDelaySeconds( appModule );
}

//============================================================================
double AppCommon::toGuiGetAudioCacheTotalSeconds( EAppModule appModule )
{
    if( VxIsAppShuttingDown() )
    {
        return 0;
    }

    return m_SoundMgr.toGuiGetAudioCacheTotalSeconds( appModule );
}

//============================================================================
double AppCommon::toGuiGetAudioCacheTotalMs( void )
{
	if( VxIsAppShuttingDown() )
	{
		return 0;
	}

	return m_SoundMgr.toGuiGetAudioCacheTotalMs();
}

//============================================================================
int AppCommon::toGuiGetAudioCacheFreeSpace( EAppModule appModule )
{
    if( VxIsAppShuttingDown() )
    {
        return 0;
    }

    return m_SoundMgr.toGuiGetAudioCacheFreeSpace( appModule );
}

//============================================================================
/// Mute/Unmute microphone
void AppCommon::fromGuiMuteMicrophone( bool muteMic )
{
	m_SoundMgr.setMuteMicrophone( muteMic );
    getEngine().fromGuiMuteMicrophone( muteMic );

	for( auto hardwareIter = m_ToGuiHardwareCtrlList.begin(); hardwareIter != m_ToGuiHardwareCtrlList.end(); ++hardwareIter )
	{
		ToGuiHardwareControlInterface* toGuiClient = *hardwareIter;
		toGuiClient->callbackToGuiMicrophoneMuted( muteMic );
	}
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
	m_SoundMgr.setMuteSpeaker( muteSpeaker );
    getEngine().fromGuiMuteSpeaker( muteSpeaker );

	for( auto hardwareIter = m_ToGuiHardwareCtrlList.begin(); hardwareIter != m_ToGuiHardwareCtrlList.end(); ++hardwareIter )
	{
		ToGuiHardwareControlInterface* toGuiClient = *hardwareIter;
		toGuiClient->callbackToGuiSpeakerMuted( muteSpeaker );
	}
}

//============================================================================
/// Returns true if speaker is muted
bool AppCommon::fromGuiIsSpeakerMuted( void )
{
    return getEngine().fromGuiIsSpeakerMuted();
}

//============================================================================
/// Called when need more sound for speaker output
void AppCommon::fromGuiAudioOutSpaceAvail( int freeSpaceLen )
{
    getEngine().fromGuiAudioOutSpaceAvail( freeSpaceLen );
}
