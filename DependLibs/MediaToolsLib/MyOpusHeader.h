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

#include <NlcDependLibrariesConfig.h>

#define	    NO_LIMIT_OPUS_SIGNITURE "nolimitconnect.com v"
#define	    NO_LIMIT_OPUS_SIGNITURE_LEN 20
#define	    NO_LIMIT_OPUS_SIGNITURE_OFFS 0x99

class MyOpusHeader 
{
public:
	MyOpusHeader();

	uint8_t							m_Version;
	uint8_t							m_Channels;			// Number of channels: 1..255 
	uint16_t						m_Preskip;			// calculated from look ahead and 
	uint32_t						m_InputSampleRate;	// rate in frequency
	uint16_t						m_Gain;				// in dB S7.8 should be zero whenever possible 
	uint8_t							m_ChannelMapping;
	// These used only used if channel_mapping != 0 
	uint8_t							m_StreamCnt;
	uint8_t							m_CoupledCnt;
	uint8_t							m_StreamMap[255];
	// Other required for ogg stream etc
	int32_t							m_MaxOpusPktSize;
	int32_t							m_LookAhead;		// from OPUS_GET_LOOKAHEAD
	int32_t							m_ExtraOut;
	int32_t							m_OpusFrameSize;


};
