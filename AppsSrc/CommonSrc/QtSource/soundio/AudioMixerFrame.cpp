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

#include "AudioMixerFrame.h"
#include "AudioUtils.h"

#include <CoreLib/VxDebug.h>

//============================================================================
void AudioMixerFrame::clearFrame( void )
{
	m_LenWrote = 0;
	m_LenRead = 0;
	m_InputIds.clear();
}

//============================================================================
int AudioMixerFrame::audioLenInUse( void )
{
	return m_LenWrote - m_LenRead; // how many bytes is written and not read by audio out
}

//============================================================================
int AudioMixerFrame::audioLenFreeSpace( void )
{
	return MIXER_BUF_SIZE_8000_1_S16 - ( m_LenWrote - m_LenRead ); // how many bytes is available to write into frame
}

//============================================================================
int AudioMixerFrame::readMixerFrame( int16_t* readBuf, bool speakerMuted, int samplesDesired )
{
	if( samplesDesired > MIXER_BUF_SIZE_8000_1_S16 / 2 )
	{
		LogMsg( LOG_ERROR, "AudioMixerFrame::toMixerPcm8000HzMonoChannel bad param samplesToRead %d", samplesDesired );
		return 0;
	}

	int samplesAvailable = audioSamplesInUse();
	if( !samplesAvailable )
	{
		LogMsg( LOG_WARN, "AudioMixerFrame::toMixerPcm8000HzMonoChannel frame read underrun %d", samplesDesired );
	}

	int samplesRead = 0;
	int samplesToRead = std::min( samplesDesired, samplesAvailable );
	if( samplesToRead )
	{
		samplesRead = samplesToRead;
		if( speakerMuted )
		{
			memset( readBuf, 0, samplesToRead * 2 );
		}
		else
		{
				memcpy( readBuf, &m_MixerBuf[ m_LenRead ], samplesToRead * 2 );				
		}
	}

	m_LenRead += samplesRead * 2;
	if( m_LenRead >= MIXER_BUF_SIZE_8000_1_S16 )
	{
		// frame has been completely read
		m_LenWrote = 0;
		m_LenRead = 0;
		m_InputIds.clear();
	}

	return samplesRead;
}

//============================================================================
void AudioMixerFrame::writeMixerFrame( EAppModule appModule, char* writeBuf, int dnDivide )
{
	if( hasModuleAudio( appModule ) )
	{
		// LogMsg( LOG_WARNING, "AudioMixerFrame::toMixerPcm8000HzMonoChannel module %d overrun", appModule );
		return;
	}

	if( isSilence() )
	{
		if( 1 == dnDivide )
		{
			memcpy( m_MixerBuf, writeBuf, MIXER_BUF_SIZE_8000_1_S16 );
		}
		else
		{
			AudioUtils::dnsamplePcmAudio( (int16_t*)writeBuf, MIXER_BUF_SIZE_8000_1_S16 / 2, dnDivide, (int16_t*)m_MixerBuf );
		}
	}
	else
	{
		if( 1 == dnDivide )
		{
			AudioUtils::mixPcmAudio( (int16_t *)writeBuf, (int16_t*)m_MixerBuf, MIXER_BUF_SIZE_8000_1_S16 );
		}
		else
		{
			char tempBuf[ MIXER_BUF_SIZE_8000_1_S16 ];
			AudioUtils::dnsamplePcmAudio( (int16_t*)writeBuf, MIXER_BUF_SIZE_8000_1_S16 / 2, dnDivide, (int16_t*)tempBuf );

			AudioUtils::mixPcmAudio( (int16_t*)tempBuf, (int16_t*)m_MixerBuf, MIXER_BUF_SIZE_8000_1_S16 );
		}
	}

	if( m_InputIds.empty() )
	{
		// first write after has been read.. reset partial read counters
		m_LenWrote = MIXER_BUF_SIZE_8000_1_S16;
	}
	
	m_InputIds.push_back( appModule );
}

//============================================================================
int AudioMixerFrame::toMixerPcm8000HzMonoChannel( EAppModule appModule, int16_t* pu16PcmData, bool isSilenceIn )
{
	if( hasModuleAudio( appModule ) )
	{
		// LogMsg( LOG_WARNING, "AudioMixerFrame::toMixerPcm8000HzMonoChannel module %d overrun", appModule );
		return 0;
	}

	int16_t* pcmData = (int16_t*)pu16PcmData;
	int16_t lastVal = pcmData[ 0 ];
	for( int i = 0; i < 100; i++ )
	{
		if( std::abs(  std::abs( lastVal ) - std::abs( pcmData[ i ] ) ) > 11000 )
		{
			LogMsg( LOG_VERBOSE, "readDataFromMixer ix %d val %d lastVal %d dif %d", i, pcmData[ i ], lastVal,
				std::abs( std::abs( lastVal ) - std::abs( pcmData[ i ] ) ) );
		}

		lastVal = pcmData[ i ];
	}

	if( isSilenceIn )
	{
		if( isSilence() )
		{
			// mixer frames are not cleared between reads. clear it because we are not setting it with data but are marking it as containing audio
			memset( m_MixerBuf, 0, MIXER_BUF_SIZE_8000_1_S16 );
		}
	}
	else
	{
		writeMixerFrame( appModule, (char*)pu16PcmData, 1 );
	}


	if( m_InputIds.empty() )
	{
		// first write after has been read.. reset partial read counters
		m_LenWrote = MIXER_BUF_SIZE_8000_1_S16;
	}

	if( !hasModuleAudio( appModule ) )
	{
		m_InputIds.push_back( appModule );
	}

	return MIXER_BUF_SIZE_8000_1_S16;
}

//============================================================================
int16_t	AudioMixerFrame::peekAtNextSampleToRead( void )
{
	if( m_InputIds.empty() || m_LenWrote - m_LenRead < 2 )
	{
		return 0;
	}

	return *((int16_t*)(&m_MixerBuf[ m_LenRead ]));
}
