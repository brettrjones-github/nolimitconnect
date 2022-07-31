
/*
*  Copyright (c) 2012 The WebRTC project authors. All Rights Reserved.
*
*  Use of this source code is governed by a BSD-style license
*  that can be found in the LICENSE file in the root of the source
*  tree. An additional intellectual property rights grant can be found
*  in the file PATENTS.  All contributing project authors may
*  be found in the AUTHORS file in the root of the source tree.
*/
//============================================================================
// Copyright (C) 2016 Brett R. Jones
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
#include "EchoCancel.h"
#include "EchoUtil.h"

#include "aec_core.h"
#include "aec_resampler.h"
#include "signal_processing_library.h"

#if defined(WEBRTC_CHROMIUM_BUILD) && defined(WEBRTC_MAC)
#define WEBRTC_UNTRUSTED_DELAY
#endif

#if defined(WEBRTC_UNTRUSTED_DELAY) && defined(WEBRTC_MAC)
static const int kDelayDiffOffsetSamples = -160;
#else
// Not enabled for now.
static const int kDelayDiffOffsetSamples = 0;
#endif

#if defined(WEBRTC_MAC)
static const int kFixedDelayMs = 20;
#else
static const int kFixedDelayMs = 50;
#endif
#if !defined(WEBRTC_UNTRUSTED_DELAY)
static const int kMinTrustedDelayMs = 20;
#endif
static const int kMaxTrustedDelayMs = 500;

// Maximum length of resampled signal. Must be an integer multiple of frames
// (ceil(1/(1 + MIN_SKEW)*2) + 1)*FRAME_LEN
// The factor of 2 handles wb, and the + 1 is as a safety margin
// TODO(bjornv): Replace with kResamplerBufferSize
#define MAX_RESAMP_LEN (5 * FRAME_LEN)

static const int kMaxBufSizeStart = 62;  // In partitions
static const int sampMsNb = 8;           // samples per ms in nb
static const int initCheck = 42;
// Limit resampling to doubling/halving of signal
static const float minSkewEst = -0.5f;
static const float maxSkewEst = 1.0f;

//============================================================================
void EchoCancel::startupEchoCancel( void )
{
	if( 0 != m_Aec )
	{
		LogMsg( LOG_ERROR, "ERROR EchoCancel::initEchoCancel already created" );
		return;
	}

	m_Aec = webrtc::WebRtcAec_CreateAec();
	if( 0 == m_Aec )
	{
		LogMsg( LOG_ERROR, "ERROR EchoCancel::initEchoCancel null Aec created" );
		return;
	}

	m_resampler = webrtc::WebRtcAec_CreateResampler();
	if( !m_resampler) 
	{
		LogMsg( LOG_ERROR, "ERROR EchoCancel::initEchoCancel null resampler" );
		webrtc::WebRtcAec_FreeAec( m_Aec );
		return;
	}

	// Create far-end pre-buffer. The buffer size has to be large enough for
	// largest possible drift compensation (kResamplerBufferSize) + "almost" an
	// FFT buffer (PART_LEN2 - 1).
	far_pre_buf = WebRtc_CreateBuffer(PART_LEN2 + webrtc::kResamplerBufferSize, sizeof(float));
	if( !far_pre_buf ) 
	{
		webrtc::WebRtcAec_FreeAec( m_Aec );
		return;
	}

	initFlag = 0;
	sampFreq = ECHO_SAMPLE_RATE_HZ;
	scSampFreq = sampFreq;
	// extended filter is available for all platforms
	// kEchoCanceller3, kAecRefinedAdaptiveFilter, kDelayAgnostic are not available on mobile platform
	webrtc::WebRtcAec_enable_extended_filter( m_Aec, true );

	int status = webrtc::WebRtcAec_InitAec( m_Aec, sampFreq /* sample freq hz */);
	if( 0 != status )
	{
		LogMsg( LOG_ERROR, "ERROR EchoCancel::initEchoCancel init returned error %d", status );
		webrtc::WebRtcAec_FreeAec( m_Aec );
		m_Aec = 0;
		return;
	}

	// extended filter is available for all platforms
	// kEchoCanceller3, kAecRefinedAdaptiveFilter, kDelayAgnostic are not available on mobile platform
	webrtc::WebRtcAec_enable_extended_filter( m_Aec, true );

	if( webrtc::WebRtcAec_InitResampler( m_resampler, scSampFreq ) == -1 ) 
	{
		LogMsg( LOG_ERROR, "ERROR EchoCancel::initEchoCancel WebRtcAec_InitResampler returned error %d", -1 );
		return;
	}

	WebRtc_InitBuffer( far_pre_buf );
	WebRtc_MoveReadPtr( far_pre_buf, -PART_LEN );  // Start overlap.

	initFlag			= initCheck;  // indicates that initialization has been done
	splitSampFreq		= sampFreq;
	delayCtr			= 0;
	sampFactor			= (scSampFreq * 1.0f) / splitSampFreq;
	// Sampling frequency multiplier (SWB is processed as 160 frame size).
	rate_factor			= splitSampFreq / 8000;

	sum					= 0;
	counter				= 0;
	checkBuffSize		= 1;
	firstVal			= 0;

	// We skip the startup_phase completely (setting to 0) if DA-AEC is enabled,
	// but not extended_filter mode.
	startup_phase		= WebRtcAec_extended_filter_enabled( m_Aec ) || !WebRtcAec_delay_agnostic_enabled( m_Aec );
	bufSizeStart		= 0;
	checkBufSizeCtr		= 0;
	msInSndCardBuf		= 0;
	filtDelay			= -1;  // -1 indicates an initialized state.
	timeForDelayChange	= 0;
	knownDelay			= 0;
	lastDelayDiff		= 0;

	m_skewFrCtr			= 0;
	resample			= kAecFalse;
	highSkewCtr			= 0;
	m_Skew				= 0;

	farend_started		= 0;
	m_FirstMixerSendToProcess = true;

	// Default settings.
	AecConfig aecConfig;
	//aecConfig.nlpMode		= kAecNlpModerate;
	aecConfig.nlpMode		= kAecNlpAggressive;
	aecConfig.metricsMode	= kAecFalse;
	aecConfig.delay_logging = kAecFalse;
	// skewMode = drift compensation.. probably should use on windows because can happen if different capture and playback devices..
	// example web cam may have its own microphone capture driver. 
	// for now leave false to avoid complications
	aecConfig.skewMode		= kAecFalse; 

	int rc = setAecConfig( aecConfig );
	if( 0 != rc )
	{
		LogMsg( LOG_ERROR, "ERROR EchoCancel::initEchoCancel WebRtcAec_set_config returned error %d", rc );
		return;
	}
}

//============================================================================
void EchoCancel::shutdownEchoCancel( void )
{
	WebRtc_FreeBuffer( far_pre_buf );
	if( 0 != m_Aec )
	{
		webrtc::WebRtcAec_FreeAec( m_Aec );
		m_Aec = 0;
	}

	webrtc::WebRtcAec_FreeResampler( m_resampler );
}

//============================================================================
void EchoCancel::sendSpeakerDataToEchoProcessor( const float* speakerSamples, int samplesToProcess )
{
	this->bufferFarend( speakerSamples, samplesToProcess );
}

//============================================================================
void EchoCancel::processEchoCancel( int16_t* pcmMicData, int sampleCnt, int16_t* pcmSpeakerData, int16_t* pcmRetSamplesProcessed, int totalDelayMs, int clockDrift )
{
    // Cancel echo
    if( 0 == m_Aec )
    {
        memcpy( pcmRetSamplesProcessed, pcmMicData, sampleCnt * 2 );
		LogMsg( LOG_ERROR, "EchoCancel::processMicrophoneWrite aec is not set" );
        return;
    }

	if( sampleCnt != ECHO_SAMPLES_PER_10MS )
	{
		memcpy( pcmRetSamplesProcessed, pcmMicData, sampleCnt * 2 );
		LogMsg( LOG_ERROR, "EchoCancel::processMicrophoneWrite invalid sample count %d", sampleCnt );
		return;
	}

	static float micInFloats[ ECHO_SAMPLES_PER_10MS ];
	static float speakerFloats[ ECHO_SAMPLES_PER_10MS ];
	static float echoCanceledFloats[ ECHO_SAMPLES_PER_10MS ];
    PcmS16ToFloats( pcmMicData, sampleCnt, micInFloats );
	PcmS16ToFloats( pcmSpeakerData, sampleCnt, speakerFloats );

	sendSpeakerDataToEchoProcessor( speakerFloats, sampleCnt );

	int processResult = processMicrophoneChunk( (const float* const*)(&micInFloats),
		1,
		(float* const*)(&echoCanceledFloats),
		sampleCnt,
		(uint16_t)totalDelayMs,
		clockDrift );

	if( 0 == processResult )
	{
		FloatsToPcmS16( echoCanceledFloats, pcmRetSamplesProcessed, sampleCnt );
	}
	else
	{
		memcpy( pcmRetSamplesProcessed, pcmMicData, sampleCnt * 2 );
		// do not flood log with messages
		static int errCnt = 0;
		static int errCode = 0;
		if( 0 == processResult )
		{
			errCnt = 0;
		}
		else
		{
			errCnt++;
			if( processResult != errCode || errCnt == 1 )
			{
				LogMsg( LOG_DEBUG, "Error:  EchoCancel::processFromMicrophone delay %d result %d", totalDelayMs, processResult );
			}

			errCode = processResult;
			if( errCnt >= 100 )
			{
				errCnt = 0;
			}
		}
	}

    /*
    if( m_EchoDelayTestStart  && m_EchoTestSignalSent )
    {
        int samplesThisChunk = dataLenBytes >> 1;
        float * floatData = &m_MicInBuf[ 0 ][ 0 ];
        int consecutiveHighVolume = 0;
        for( int i = 0; i < samplesThisChunk; i++ )
        {
            if( ( *floatData > 0.07 ) || ( *floatData < -0.07 ) )
            {
                consecutiveHighVolume++;
                if( consecutiveHighVolume > 3 )
                {
                    double elapsedMs = m_EchoDelayTimer.elapsedMs();
                    m_EchoDelayTestStart = false;
                    LogMsg( LOG_INFO, "Audio Delay measured %3.3f ms, estimated %d ms samples%d  \n", elapsedMs, totalDelayMs, m_EchoDelaySampleCnt );
                    break;
                }
            }
            else
            {
                consecutiveHighVolume = 0;
            }

            m_EchoDelaySampleCnt++;
        }
    }
    */
}

//============================================================================
int EchoCancel::setAecConfig( AecConfig config ) 
{
	if( initFlag != initCheck) 
	{
		return AEC_UNINITIALIZED_ERROR;
	}

	if( config.skewMode != kAecFalse && config.skewMode != kAecTrue ) 
	{
		return AEC_BAD_PARAMETER_ERROR;
	}

	skewMode = config.skewMode;

	if (config.nlpMode != kAecNlpConservative &&
		config.nlpMode != kAecNlpModerate &&
		config.nlpMode != kAecNlpAggressive) 
	{
		return AEC_BAD_PARAMETER_ERROR;
	}

	if (config.metricsMode != kAecFalse && config.metricsMode != kAecTrue) 
	{
		return AEC_BAD_PARAMETER_ERROR;
	}

	if (config.delay_logging != kAecFalse && config.delay_logging != kAecTrue) 
	{
		return AEC_BAD_PARAMETER_ERROR;
	}

	WebRtcAec_SetConfigCore( m_Aec, config.nlpMode, config.metricsMode, config.delay_logging );
	return 0;
}

//============================================================================
int32_t EchoCancel::processMicrophoneChunk(	const float* const* nearend,
											size_t num_bands,
											float* const* out,
											size_t nrOfSamples,
											int16_t msInSndCardBuf,
											int32_t skew ) 
{
	int32_t retVal = 0;

	if( out == NULL ) 
	{
		return AEC_NULL_POINTER_ERROR;
	}

	if( initFlag != initCheck ) 
	{
		return AEC_UNINITIALIZED_ERROR;
	}

	// number of samples == 160 for SWB input
	if( nrOfSamples != 80 && nrOfSamples != 160 ) 
	{
		return AEC_BAD_PARAMETER_ERROR;
	}

	if (msInSndCardBuf < 0) 
	{
		msInSndCardBuf = 0;
		retVal = AEC_BAD_PARAMETER_WARNING;
	}
	else if( msInSndCardBuf > kMaxTrustedDelayMs ) 
	{
		// The clamping is now done in ProcessExtended/Normal().
		retVal = AEC_BAD_PARAMETER_WARNING;
	}

	// This returns the value of aec->extended_filter_enabled.
	if( WebRtcAec_extended_filter_enabled( m_Aec ) ) 
	{
		processMicrophoneChunkExtended( nearend, num_bands, out, nrOfSamples, msInSndCardBuf, skew );
	} 
	else 
	{
		retVal = processMicrophoneChunkNormal( nearend, num_bands, out, nrOfSamples, msInSndCardBuf, skew );
	}

#ifdef WEBRTC_AEC_DEBUG_DUMP
	{
		int16_t far_buf_size_ms = (int16_t)(WebRtcAec_system_delay(aec) /
			(sampMsNb * rate_factor));
		(void)fwrite(&far_buf_size_ms, 2, 1, bufFile);
		(void)fwrite(&knownDelay, sizeof(knownDelay), 1,
			delayFile);
	}
#endif

	return retVal;
}

//============================================================================
// only buffer L band for farend
int32_t EchoCancel::bufferFarend( const float* farend, size_t nrOfSamples ) 
{
	size_t newNrOfSamples = nrOfSamples;
	float new_farend[MAX_RESAMP_LEN];
	const float* farend_ptr = farend;

	if( skewMode == kAecTrue && resample == kAecTrue) 
	{
		// Resample and get a new number of samples
		webrtc::WebRtcAec_ResampleLinear( m_resampler, farend, nrOfSamples, m_Skew, new_farend, &newNrOfSamples);
		farend_ptr = new_farend;
	}

	farend_started = 1;
	WebRtcAec_SetSystemDelay( m_Aec, WebRtcAec_system_delay( m_Aec ) + static_cast<int>(newNrOfSamples));

	// Write the time-domain data to |far_pre_buf|.
	WebRtc_WriteBuffer( far_pre_buf, farend_ptr, newNrOfSamples );

	// TODO(minyue): reduce to |PART_LEN| samples for each buffering, when
	// WebRtcAec_BufferFarendPartition() is changed to take |PART_LEN| samples.
	while (WebRtc_available_read( far_pre_buf) >= PART_LEN2 ) 
	{
		// We have enough data to pass to the FFT, hence read PART_LEN2 samples.
		{
			float* ptmp = NULL;
			float tmp[PART_LEN2];
			WebRtc_ReadBuffer( far_pre_buf, reinterpret_cast<void**>(&ptmp), tmp, PART_LEN2 );
			WebRtcAec_BufferFarendPartition( m_Aec, ptmp );
		}

		// Rewind |far_pre_buf| PART_LEN samples for overlap before continuing.
		WebRtc_MoveReadPtr( far_pre_buf, -PART_LEN );
	}

	return 0;
}

//============================================================================
int  EchoCancel::processMicrophoneChunkNormal(	const float* const* nearend,
												size_t num_bands,
												float* const* out,
												size_t nrOfSamples,
												int16_t msInSndCardBuf,
												int32_t skew ) 
{
	int retVal = 0;
	size_t i;
	size_t nBlocks10ms;

	msInSndCardBuf = msInSndCardBuf > kMaxTrustedDelayMs ? kMaxTrustedDelayMs : msInSndCardBuf;
	// TODO(andrew): we need to investigate if this +10 is really wanted.
	msInSndCardBuf += 10;

	if (skewMode == kAecTrue) {
		if (m_skewFrCtr < 25) {
			m_skewFrCtr++;
		} else {
			retVal = webrtc::WebRtcAec_GetSkew( m_resampler, skew, &m_Skew);
			if (retVal == -1) {
				m_Skew = 0;
				retVal = AEC_BAD_PARAMETER_WARNING;
			}

			m_Skew /= sampFactor * nrOfSamples;

			if (m_Skew < 1.0e-3 && m_Skew > -1.0e-3) {
				resample = kAecFalse;
			} else {
				resample = kAecTrue;
			}

			if (m_Skew < minSkewEst) 
			{
				m_Skew = minSkewEst;
			} 
			else if (m_Skew > maxSkewEst) 
			{
				m_Skew = maxSkewEst;
			}

			//data_dumper->DumpRaw("aec_skew", 1, &m_Skew);
		}
	}

	nBlocks10ms = nrOfSamples / (FRAME_LEN * rate_factor);

	if (startup_phase) {
		for (i = 0; i < num_bands; ++i) {
			// Only needed if they don't already point to the same place.
			if (nearend[i] != out[i]) {
				memcpy(out[i], nearend[i], sizeof(nearend[i][0]) * nrOfSamples);
			}
		}

		// The AEC is in the start up mode
		// AEC is disabled until the system delay is OK

		// Mechanism to ensure that the system delay is reasonably stable.
		if (checkBuffSize) {
			checkBufSizeCtr++;
			// Before we fill up the far-end buffer we require the system delay
			// to be stable (+/-8 ms) compared to the first value. This
			// comparison is made during the following 6 consecutive 10 ms
			// blocks. If it seems to be stable then we start to fill up the
			// far-end buffer.
			if (counter == 0) {
				firstVal = msInSndCardBuf;
				sum = 0;
			}

			if (abs(firstVal - msInSndCardBuf) <
				WEBRTC_SPL_MAX(0.2 * msInSndCardBuf, sampMsNb)) {
					sum += msInSndCardBuf;
					counter++;
			} else {
				counter = 0;
			}

			if (counter * nBlocks10ms >= 6) {
				// The far-end buffer size is determined in partitions of
				// PART_LEN samples. Use 75% of the average value of the system
				// delay as buffer size to start with.
				bufSizeStart =
					WEBRTC_SPL_MIN((3 * sum * rate_factor * 8) /
					(4 * counter * PART_LEN),
					kMaxBufSizeStart);
				// Buffer size has now been determined.
				checkBuffSize = 0;
			}

			if (checkBufSizeCtr * nBlocks10ms > 50) {
				// For really bad systems, don't disable the echo canceller for
				// more than 0.5 sec.
				bufSizeStart = WEBRTC_SPL_MIN(
					(msInSndCardBuf * rate_factor * 3) / 40,
					kMaxBufSizeStart);
				checkBuffSize = 0;
			}
		}

		// If |checkBuffSize| changed in the if-statement above.
		if (!checkBuffSize) {
			// The system delay is now reasonably stable (or has been unstable
			// for too long). When the far-end buffer is filled with
			// approximately the same amount of data as reported by the system
			// we end the startup phase.
			int overhead_elements =
				webrtc::WebRtcAec_system_delay(m_Aec) / PART_LEN - bufSizeStart;
			if (overhead_elements == 0) {
				// Enable the AEC
				startup_phase = 0;
			} else if (overhead_elements > 0) {
				// TODO(bjornv): Do we need a check on how much we actually
				// moved the read pointer? It should always be possible to move
				// the pointer |overhead_elements| since we have only added data
				// to the buffer and no delay compensation nor AEC processing
				// has been done.
				webrtc::WebRtcAec_MoveFarReadPtr(m_Aec, overhead_elements);

				// Enable the AEC
				startup_phase = 0;
			}
		}
	} else {
		// AEC is enabled.
		estimateBufDelayNormal();

		// Call the AEC.
		// TODO(bjornv): Re-structure such that we don't have to pass
		// |knownDelay| as input. Change name to something like
		// |system_buffer_diff|.
		WebRtcAec_ProcessFrames( m_Aec, nearend, num_bands, nrOfSamples, knownDelay, out );
	}

	return retVal;
}

//============================================================================
void  EchoCancel::processMicrophoneChunkExtended(	const float* const* nearBuf,
													 size_t num_bands,
													 float* const* outBuf,
													 size_t num_samples,
													 int16_t reported_delay_ms,
													 int32_t skew ) 
{
	const int delay_diff_offset = kDelayDiffOffsetSamples;
#if defined(WEBRTC_UNTRUSTED_DELAY)
	reported_delay_ms = kFixedDelayMs;
#else
	// This is the usual mode where we trust the reported system delay values.
	// Due to the longer filter, we no longer add 10 ms to the reported delay
	// to reduce chance of non-causality. Instead we apply a minimum here to avoid
	// issues with the read pointer jumping around needlessly.
	reported_delay_ms = reported_delay_ms < kMinTrustedDelayMs
		? kMinTrustedDelayMs
		: reported_delay_ms;
	// If the reported delay appears to be bogus, we attempt to recover by using
	// the measured fixed delay values. We use >= here because higher layers
	// may already clamp to this maximum value, and we would otherwise not
	// detect it here.
	reported_delay_ms = reported_delay_ms >= kMaxTrustedDelayMs
		? kFixedDelayMs
		: reported_delay_ms;
#endif
	msInSndCardBuf = reported_delay_ms;

	if( !farend_started ) 
	{
		for( size_t  i = 0; i < num_bands; ++i ) 
		{
			// Only needed if they don't already point to the same place.
			if( nearBuf[i] != outBuf[i] ) 
			{
				memcpy(outBuf[i], nearBuf[i], sizeof(nearBuf[i][0]) * num_samples);
			}
		}

		return;
	}
	if( startup_phase ) 
	{
		// In the extended mode, there isn't a startup "phase", just a special
		// action on the first frame. In the trusted delay case, we'll take the
		// current reported delay, unless it's less then our conservative
		// measurement.
		int startup_size_ms =
			reported_delay_ms < kFixedDelayMs ? kFixedDelayMs : reported_delay_ms;
#if defined(WEBRTC_ANDROID)
		int target_delay = startup_size_ms * rate_factor * 8;
#else
		// To avoid putting the AEC in a non-causal state we're being slightly
		// conservative and scale by 2. On Android we use a fixed delay and
		// therefore there is no need to scale the target_delay.
		int target_delay = startup_size_ms * rate_factor * 8 / 2;
#endif
		int overhead_elements =
			(WebRtcAec_system_delay(m_Aec) - target_delay) / PART_LEN;
		WebRtcAec_MoveFarReadPtr(m_Aec, overhead_elements);
		startup_phase = 0;
	}

	estimateBufDelayExtended();

	{
		// |delay_diff_offset| gives us the option to manually rewind the delay on
		// very low delay platforms which can't be expressed purely through
		// |reported_delay_ms|.
		const int adjusted_known_delay =
			WEBRTC_SPL_MAX(0, knownDelay + delay_diff_offset);

		WebRtcAec_ProcessFrames( m_Aec, nearBuf, num_bands, num_samples, adjusted_known_delay, outBuf);
	}
}

//============================================================================
void  EchoCancel::estimateBufDelayNormal( void ) 
{
	int nSampSndCard = msInSndCardBuf * sampMsNb * rate_factor;
	int current_delay = nSampSndCard - WebRtcAec_system_delay( m_Aec );
	int delay_difference = 0;

	// Before we proceed with the delay estimate filtering we:
	// 1) Compensate for the frame that will be read.
	// 2) Compensate for drift resampling.
	// 3) Compensate for non-causality if needed, since the estimated delay can't
	//    be negative.

	// 1) Compensating for the frame(s) that will be read/processed.
	current_delay += FRAME_LEN * rate_factor;

	// 2) Account for resampling frame delay.
	if (skewMode == kAecTrue && resample == kAecTrue) {
		current_delay -= webrtc::kResamplingDelay;
	}

	// 3) Compensate for non-causality, if needed, by flushing one block.
	if (current_delay < PART_LEN) {
		current_delay += WebRtcAec_MoveFarReadPtr( m_Aec, 1) * PART_LEN;
	}

	// We use -1 to signal an initialized state in the "extended" implementation;
	// compensate for that.
	filtDelay = filtDelay < 0 ? 0 : filtDelay;
	filtDelay =
		WEBRTC_SPL_MAX(0, static_cast<int16_t>(0.8 *
		filtDelay +
		0.2 * current_delay));

	delay_difference = filtDelay - knownDelay;
	if (delay_difference > 224) {
		if (lastDelayDiff < 96) {
			timeForDelayChange = 0;
		} else {
			timeForDelayChange++;
		}
	} else if (delay_difference < 96 && knownDelay > 0) {
		if (lastDelayDiff > 224) {
			timeForDelayChange = 0;
		} else {
			timeForDelayChange++;
		}
	} else {
		timeForDelayChange = 0;
	}
	lastDelayDiff = delay_difference;

	if (timeForDelayChange > 25) {
		knownDelay = WEBRTC_SPL_MAX((int)filtDelay - 160, 0);
	}
}

//============================================================================
void  EchoCancel::estimateBufDelayExtended( void ) 
{
	int reported_delay = msInSndCardBuf * sampMsNb * rate_factor;
	int current_delay = reported_delay - WebRtcAec_system_delay( m_Aec );
	int delay_difference = 0;

	// Before we proceed with the delay estimate filtering we:
	// 1) Compensate for the frame that will be read.
	// 2) Compensate for drift resampling.
	// 3) Compensate for non-causality if needed, since the estimated delay can't
	//    be negative.

	// 1) Compensating for the frame(s) that will be read/processed.
	current_delay += FRAME_LEN * rate_factor;

	// 2) Account for resampling frame delay.
	if (skewMode == kAecTrue && resample == kAecTrue) 
	{
		current_delay -= webrtc::kResamplingDelay;
	}

	// 3) Compensate for non-causality, if needed, by flushing two blocks.
	if (current_delay < PART_LEN) 
	{
		current_delay += webrtc::WebRtcAec_MoveFarReadPtr( m_Aec, 2) * PART_LEN;
	}

	if (filtDelay == -1) {
		filtDelay = (int16_t)WEBRTC_SPL_MAX(0, 0.5 * current_delay);
	} else {
		filtDelay = WEBRTC_SPL_MAX(
			0, static_cast<int16_t>(0.95 * filtDelay + 0.05 * current_delay));
	}

	delay_difference = filtDelay - knownDelay;
	if (delay_difference > 384) {
		if (lastDelayDiff < 128) {
			timeForDelayChange = 0;
		} else {
			timeForDelayChange++;
		}
	} else if (delay_difference < 128 && knownDelay > 0) {
		if (lastDelayDiff > 384) {
			timeForDelayChange = 0;
		} else {
			timeForDelayChange++;
		}
	} else {
		timeForDelayChange = 0;
	}
	lastDelayDiff = delay_difference;

	if (timeForDelayChange > 25) {
		knownDelay = WEBRTC_SPL_MAX((int)filtDelay - 256, 0);
	}
}
