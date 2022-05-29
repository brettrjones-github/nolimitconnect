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

struct OpusMSEncoder;

class OpusAudioEncoder
{
public:
	OpusAudioEncoder( int sampleRate = 8000, int channels = 1  );
	virtual ~OpusAudioEncoder() = default;

	bool						isInitialized( void )						{ return m_EncoderInitialized; }
	MyOpusHeader&				getOpusHeader( void )						{ return m_OpusHeader; }

	int							encodePcmData(		int16_t *	pcmData, 
													uint16_t	pcmDataLen, 
													uint8_t *	outBuf,	
													int32_t&	retEncodeFrame1Len,
													int32_t&	retEncodeFrame2Len );
protected:
	void						initEncoder( void );

	//=== vars ===//
	int							m_SampleRate;
	int							m_Channels;
	bool						m_EncoderInitialized;

	OpusMSEncoder *				m_Encoder;
	int							m_OpusErr;
	int							m_BitRate;
	int							m_FrameSize;
	MyOpusHeader				m_OpusHeader;
	float						m_OpusInBuf[ MY_PCM_SAMPLE_CNT  ];
};

