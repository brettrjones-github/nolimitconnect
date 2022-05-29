//============================================================================
// Copyright (C) 2015 Brett R. Jones
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

#include "OpusAudioEncoder.h"
#include "SndDefs.h"

#include "opus_multistream.h"

#include <CoreLib/IsBigEndianCpu.h>
#include <CoreLib/VxDebug.h>

//============================================================================
OpusAudioEncoder::OpusAudioEncoder( int sampleRate, int channels )
: m_SampleRate( sampleRate )
, m_Channels( channels )
, m_EncoderInitialized( false )
, m_Encoder( 0 )
, m_OpusErr( 0 )
, m_BitRate( MY_OPUS_TARGET_BIT_RATE )
, m_FrameSize( MY_OPUS_FRAME_SAMPLE_CNT )
{
	initEncoder();
}

//============================================================================
void OpusAudioEncoder::initEncoder( void )
{
	m_OpusHeader.m_Channels			= m_Channels;
	m_OpusHeader.m_ChannelMapping	= m_OpusHeader.m_Channels > 8 ? 255 : m_Channels > 2;
	m_OpusHeader.m_InputSampleRate	= m_SampleRate;
	m_OpusHeader.m_Gain				= 0;

	int streamCnt; 
	int coupledCnt;

	m_Encoder = opus_multistream_surround_encoder_create( 
			m_SampleRate, 
			m_Channels, 
			m_OpusHeader.m_ChannelMapping, 
			&streamCnt, 
			&coupledCnt,
			m_OpusHeader.m_StreamMap, 
			OPUS_APPLICATION_RESTRICTED_LOWDELAY, // RESTRICTED_LOWDELAY makes smaller AND takes about 1/2 the CPU time
			&m_OpusErr );
	if( OPUS_OK != m_OpusErr )
	{
		LogMsg( LOG_ERROR, "Error cannot create encoder: %s\n", opus_strerror( m_OpusErr ) );
		return;
	}

	m_OpusHeader.m_StreamCnt	= (uint8_t)streamCnt;
	m_OpusHeader.m_CoupledCnt	= (uint8_t)coupledCnt;
	m_OpusHeader.m_MaxOpusPktSize = (1275*3+7) * streamCnt;
	m_OpusErr = opus_multistream_encoder_ctl( m_Encoder, OPUS_SET_BITRATE( m_BitRate ) );
	if( OPUS_OK != m_OpusErr )
	{
		LogMsg( LOG_ERROR, "Error OPUS_SET_BITRATE returned: %s\n",opus_strerror( m_OpusErr ));
		return;
	}

	m_OpusErr = opus_multistream_encoder_ctl( m_Encoder, OPUS_SET_VBR( 1 ) );
	if( OPUS_OK != m_OpusErr )
	{
		LogMsg( LOG_ERROR, "Error OPUS_SET_VBR returned: %s\n",opus_strerror( m_OpusErr ));
		return;
	}

	m_OpusErr = opus_multistream_encoder_ctl( m_Encoder, OPUS_SET_VBR_CONSTRAINT( 0 ) );
	if( OPUS_OK != m_OpusErr )
	{
		LogMsg( LOG_ERROR, "Error OPUS_SET_VBR_CONSTRAINT returned: %s\n",opus_strerror( m_OpusErr ));
		return;
	}

	m_OpusErr = opus_multistream_encoder_ctl( m_Encoder, OPUS_SET_COMPLEXITY( 10 ) );
	if( OPUS_OK != m_OpusErr )
	{
		LogMsg( LOG_ERROR, "Error OPUS_SET_COMPLEXITY returned: %s\n",opus_strerror( m_OpusErr ));
		return;
	}

	m_OpusErr = opus_multistream_encoder_ctl( m_Encoder, OPUS_SET_PACKET_LOSS_PERC( 0 ) );
	if( OPUS_OK != m_OpusErr )
	{
		LogMsg( LOG_ERROR, "Error OPUS_SET_PACKET_LOSS_PERC returned: %s\n",opus_strerror( m_OpusErr ));
		return;
	}

	m_OpusErr = opus_multistream_encoder_ctl( m_Encoder, OPUS_SET_LSB_DEPTH( 16 ) );
	if( OPUS_OK != m_OpusErr )
	{
		LogMsg( LOG_ERROR, "Error OPUS_SET_LSB_DEPTH returned: %s\n",opus_strerror( m_OpusErr ));
		return;
	}

	// lookahead = 52 ... dont know why
	opus_int32 lookAhead;
	m_OpusErr = opus_multistream_encoder_ctl( m_Encoder, OPUS_GET_LOOKAHEAD( &lookAhead ) );
	if( OPUS_OK != m_OpusErr )
	{
		LogMsg( LOG_ERROR, "Error OPUS_GET_LOOKAHEAD returned: %s\n",opus_strerror( m_OpusErr ));
		return;
	}

	m_OpusHeader.m_LookAhead = lookAhead;
	//Regardless of the rate we're coding at the ogg timestamping/skip is always timed at 48000.
	// this comes to 312
	m_OpusHeader.m_Preskip = (uint16_t)(m_OpusHeader.m_LookAhead * ( 48000.0 / m_SampleRate ));
	// Extra samples that need to be read to compensate for the pre-skip 
	// this comes to 52 again
	m_OpusHeader.m_ExtraOut = (int32_t)(m_OpusHeader.m_Preskip*( m_SampleRate / 48000.0 ));

	m_EncoderInitialized = true;
}

//============================================================================
int OpusAudioEncoder::encodePcmData(	int16_t *	pcmData, 
										uint16_t		pcmDataLen, 
										uint8_t *	outBuf,	
										int32_t&	retEncodeFrame1Len,
										int32_t&	retEncodeFrame2Len )
{
	if( false == m_EncoderInitialized )
	{
		LogMsg( LOG_ERROR, "ERROR: OpusAudioEncoder::writePcmData not initialized\n" );
		return 0;
	}
	
	int sampleCnt = pcmDataLen >> 1;
	if(  MY_PCM_SAMPLE_CNT  != sampleCnt  )
	{
		LogMsg( LOG_ERROR, "ERROR: OpusAudioEncoder::writePcmData not expected length\n" );
		return 0;
	}

	int8_t * pcmBuf = (int8_t *)pcmData;
	if( IsBigEndianCpu() )
	{
		for( int i = 0; i < MY_PCM_SAMPLE_CNT; i++ )
		{
			m_OpusInBuf[i] = ( ( pcmBuf[ i * 2 ] << 8 ) | ( pcmBuf[ i * 2 + 1 ] & 0xff ) ) / 32768.0f;
		}
	}
	else
	{
		for( int i = 0; i < MY_PCM_SAMPLE_CNT; i++ )
		{
			m_OpusInBuf[i] = ( ( pcmBuf[ i * 2 + 1 ] << 8 ) | ( pcmBuf[ i * 2 ] & 0xff ) ) / 32768.0f;
		}
	}

	retEncodeFrame1Len = opus_multistream_encode_float( m_Encoder, m_OpusInBuf, MY_OPUS_FRAME_SAMPLE_CNT, outBuf, 3832 );
	if( 0 > retEncodeFrame1Len )
	{
		LogMsg( LOG_ERROR, "Encoding failed: %s.\n", opus_strerror( retEncodeFrame1Len ) );
		return 0;
	}

	retEncodeFrame2Len = opus_multistream_encode_float( m_Encoder, 
														&m_OpusInBuf[MY_OPUS_FRAME_SAMPLE_CNT], 
														MY_OPUS_FRAME_SAMPLE_CNT, 
														&outBuf[retEncodeFrame1Len], 3832 );
	if( 0 > retEncodeFrame2Len )
	{
		LogMsg( LOG_ERROR, "Encoding failed: %s.\n", opus_strerror( retEncodeFrame1Len ) );
		return 0;
	}

	return 1;
}


