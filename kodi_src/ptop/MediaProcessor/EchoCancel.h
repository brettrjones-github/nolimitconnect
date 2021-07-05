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
// http://www.nolimitconnect.com
//============================================================================

#include <CoreLib/VxMutex.h>
#include <CoreLib/VxTimer.h>

#include <MediaToolsLib/SndDefs.h>
#include <GoTvCore/GoTvP2P/EchoCancel/ring_buffer.h>

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


const uint32_t					ECHO_MIXER_BUF_CNT					= 4;
const uint32_t					ECHO_MIXER_IN_BUF_SAMPLE_CNT		= 640;

class EchoCancel
{
public:
	EchoCancel();
	virtual ~EchoCancel();

	void						startupEchoCancel( void );
	void						shutdownEchoCancel( void );

	void						fromGuiEchoCancelEnable( bool enableEchoCancel )			{ m_EchoCancelEnabled = enableEchoCancel; }		 
	bool						fromGuiIsEchoCancelEnabled( void )							{ return m_EchoCancelEnabled; } 
	void						fromGuiSoundDelayTest( void );

	int							setAecConfig( AecConfig config ); 

	void						processFromMicrophone( int16_t * pcmDataFromMic, uint32_t dataLenBytes, int16_t * pcmRetDataProcessed, uint32_t totalDelayMs, int clockDrift );
	void						processFromMixer( int16_t * pcmDataFromMixer, uint32_t dataLenBytes );

protected:
	void						sendMixerDataToEchoProcessor( int samplesToProcess, uint32_t totalDelayMs );
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
	webrtc::AecCore *			m_Aec; //Acoustic Echo Cancellation
	VxMutex						m_AecMutex;
	float						m_MicInBuf[8][80];
	float						m_MicOutBuf[8][80];
	float						m_MixerInBuf[ECHO_MIXER_BUF_CNT][ECHO_MIXER_IN_BUF_SAMPLE_CNT];
	int							m_MixerWriteIdx;
	int							m_MixerReadIdx;
	int							m_MixerReadOffset;

	int							m_EchoMsOffs;
	bool						m_EchoDelayTestStart;
	bool						m_EchoTestSignalSent;
	int							m_EchoTestMixerReqCnt;
	bool						m_EchoDelayTestComplete;
	int							m_EchoDelaySampleCnt;
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

//#ifdef WEBRTC_AEC_DEBUG_DUMP
//	FILE*						bufFile;
//	FILE*						delayFile;
//	FILE*						skewFile;
//#endif

	// Structures
	void*						m_resampler;

	int							m_skewFrCtr;
	int							resample;  // if the skew is small enough we don't resample
	int							highSkewCtr;
	float						m_Skew;

	RingBuffer *				far_pre_buf;  // Time domain far-end pre-buffer.

	int							farend_started;
	bool						m_FirstMixerSendToProcess;
	bool						m_EchoCancelEnabled;
};
