#pragma once
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

#include "SndDefs.h"
#include "MyOpusHeader.h"
#include "OpusCallbackInterface.h"

#include "SndDefs.h"
#include "MyOpusHeader.h"

#include <MediaToolsLib/include/opus.h>
#include <MediaToolsLib/include/opus_multistream.h>
#include <libogg/include/ogg/ogg.h>

#include <MediaToolsLib/include/opus_defines.h>
#include <MediaToolsLib/OpusTools/speex_resampler.h>

#define OPUS_OUT_MAX_SAMPLES (MY_OPUS_FRAME_SAMPLE_CNT * 9) 
#define MIN_OPUS_FILE_LEN 1104

struct OpusDecoder;

class OpusAudioDecoder
{
public:
	OpusAudioDecoder( int sampleRate = 8000, int channels = 1  );
	virtual ~OpusAudioDecoder();

	bool						isInitialized( void )						{ return m_DecoderInitialized; }
	OpusDecoder *				getDecoder( void )							{ return m_OpusDecoder; }

	bool						decodeToPcmData(	uint8_t *	encodedOpusAudio,	
													int32_t		encodedFrame1Len,
													int32_t		encodedFrame2Len,
													uint16_t *	pcmDataBuf, 
													int32_t		pcmDataBufLen );
protected:
	bool						initDecoder( void );
	int							decodeOneFrameToPcmData(	uint8_t *	encodedOpusAudio,	
															int32_t		encodedFrameLenBytes,
															uint16_t *	pcmDataBuf, 
															int32_t		pcmDataBufLen );
	int							opusFloatOutputToPcm(	float *					opusOutput, 
														int						channels, 
														int						frameSampleCnt, 
														SpeexResamplerState *	resampler,
														int *					skip, 
														uint8_t *					pcmOutBuf,
														unsigned int			maxout );

	//=== vars ===//
	int							m_SampleRate;
	int							m_Channels;
	bool						m_DecoderInitialized;

	OpusDecoder *				m_OpusDecoder;
	int							m_OpusErr;
	int							m_BitRate;
	int							m_FrameSize;
	bool 						m_FirstFillSilence;
	float						m_OpusOutBuf[ OPUS_OUT_MAX_SAMPLES  ];
	SpeexResamplerState *		m_Resampler;
};

