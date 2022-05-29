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

#include "OpusCallbackInterface.h"
#include "SndDefs.h"

#include <string>

class OpusAudioEncoder;
class OggStream;

class OpusFileEncoder
{
public:
	OpusFileEncoder( );
	virtual ~OpusFileEncoder();
 
	bool						beginFileEncode( const char * fileName, int sampleRate = 8000, int channels = 1 );
	int							writeEncodedFrame( uint8_t * encodedFrameData, int32_t encodedLen );
	void						finishFileEncode( void );

protected:
	bool						createAudioEncoder( int sampleRate, int channels );
	int							encodePcmData( int16_t * pcmData, uint16_t pcmDataLen );
	bool						writeTotalSndFrames( FILE * fileHandle );

	//=== vars ===//
	OpusAudioEncoder *			m_AudioEncoder;
	OggStream&					m_OggStream;
	std::string					m_FileName;
	FILE *						m_FileHandle;
	uint64_t							m_TotalSndFramesInFile;
	bool						m_EncoderInitialized;
};

