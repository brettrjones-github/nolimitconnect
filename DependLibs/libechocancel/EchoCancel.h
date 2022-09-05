#pragma once
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

#include <CoreLib/VxMutex.h>
#include <CoreLib/VxTimer.h>

#include <MediaToolsLib/SndDefs.h>
#include "ring_buffer.h"

namespace webrtc
{
	struct AecCore;
}

// Errors
#define AEC_UNSPECIFIED_ERROR 12000
#define AEC_UNSUPPORTED_FUNCTION_ERROR 12001
#define AEC_UNINITIALIZED_ERROR 12002
#define AEC_NULL_POINTER_ERROR 12003
#define AEC_BAD_PARAMETER_ERROR 12004

// Warnings
#define AEC_BAD_PARAMETER_WARNING 12050

enum { kAecNlpConservative = 0, kAecNlpModerate, kAecNlpAggressive };
enum { kAecFalse = 0, kAecTrue };

typedef struct {
	int16_t nlpMode;      // default kAecNlpModerate
	int16_t skewMode;     // default kAecFalse
	int16_t metricsMode;  // default kAecFalse
	int delay_logging;    // default kAecFalse
	// float realSkew;
} AecConfig;

typedef struct {
	int instant;
	int average;
	int max;
	int min;
} AecLevel;

typedef struct {
	AecLevel rerl;
	AecLevel erl;
	AecLevel erle;
	AecLevel aNlp;
	float divergent_filter_fraction;
} AecMetrics;

class EchoCancel
{
public:
	static const int			ECHO_SAMPLE_RATE_HZ = 8000;						// both input and output
	static const int			ECHO_SAMPLE_CHANNELS = 1;							// both input and output
	static const int			ECHO_SAMPLES_PER_10MS = ECHO_SAMPLE_RATE_HZ / 100;	// all processing must be done in buffers containing this exact numbe of sample

	EchoCancel() = default;
	virtual ~EchoCancel() = default;

	void						startupEchoCancel( void );
	void						shutdownEchoCancel( void );

	int							getEchoSampleRate( void )	{ return ECHO_SAMPLE_RATE_HZ; }  // require this exact sample rate
	int							getEchoChannels( void )		{ return ECHO_SAMPLE_CHANNELS; } // only allow one channel
	int							getSamplePer10Ms( void )	{ return ECHO_SAMPLE_RATE_HZ / 100; } // all processing must be done in buffers containing this exact number of samples

	void						processEchoCancel( int16_t* pcmMicData, int sampleCnt, int16_t* pcmSpeakerData, int16_t* pcmRetSamplesProcessed, int totalDelayMs, int clockDrift = 0 );

protected:

	int							setAecConfig( AecConfig config );

	void						sendSpeakerDataToEchoProcessor( const float* speakerSamples, int samplesToProcess );
	int32_t						bufferFarend( const float* farend, size_t nrOfSamples );
	int32_t						processMicrophoneChunk(	const float* const* nearend,
														size_t num_bands,
														float* const* out,
														size_t nrOfSamples,
														int16_t msInSndCardBuf,
														int32_t skew );
	int							processMicrophoneChunkNormal(	const float* const* nearend,
																size_t num_bands,
																float* const* out,
																size_t nrOfSamples,
																int16_t msInSndCardBuf,
																int32_t skew ); 
	void						processMicrophoneChunkExtended(	const float* const* near,
																size_t num_bands,
																float* const* out,
																size_t num_samples,
																int16_t reported_delay_ms,
																int32_t skew );
	void						estimateBufDelayNormal( void ); 
	void						estimateBufDelayExtended( void ); 

	//=== vars ===//
	webrtc::AecCore*			m_Aec{ nullptr }; //Acoustic Echo Cancellation
	VxMutex						m_AecMutex;

	int							m_EchoMsOffs{ 0 };
	bool						m_EchoDelayTestStart{ false };
	bool						m_EchoTestSignalSent{ false };
	int							m_EchoTestMixerReqCnt{ 0 };
	bool						m_EchoDelayTestComplete{ false };
	int							m_EchoDelaySampleCnt{ 0 };
	VxTimer						m_EchoDelayTimer;

	int							delayCtr;
	int							sampFreq;
	int							splitSampFreq;
	int							scSampFreq;
	float						sampFactor;  // scSampRate / sampFreq
	short						skewMode;
	int							bufSizeStart;
	int							knownDelay;
	int							rate_factor;

	short						initFlag;  // indicates if AEC has been initialized

	// Variables used for averaging far end buffer size
	short						counter;
	int							sum;
	short						firstVal;
	short						checkBufSizeCtr;

	// Variables used for delay shifts
	short						msInSndCardBuf;
	short						filtDelay;  // Filtered delay estimate.
	int							timeForDelayChange;
	int							startup_phase;
	int							checkBuffSize;
	short						lastDelayDiff;

	// Structures
	void*						m_resampler;

	int							m_skewFrCtr;
	int							resample;  // if the skew is small enough we don't resample
	int							highSkewCtr;
	float						m_Skew;

	RingBuffer *				far_pre_buf;  // Time domain far-end pre-buffer.

	int							farend_started;
	bool						m_FirstMixerSendToProcess{ true };
	bool						m_EchoCancelEnabled{ true };
};
