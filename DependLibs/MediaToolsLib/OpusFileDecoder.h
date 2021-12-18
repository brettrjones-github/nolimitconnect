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
// http://www.nolimitconnect.com
//============================================================================

#include <config_appcorelibs.h>

#include <ptop_src/ptop_engine_src/MediaProcessor/MediaProcessor.h>
#include "OpusCallbackInterface.h"
#include "SndDefs.h"
#include "MyOpusHeader.h"

#include <opus.h>
#include <opus_multistream.h>
#include <libogg/include/ogg/ogg.h>

#include "opus_defines.h"
#include "OpusTools/speex_resampler.h"

#include <string>

typedef struct shapestate shapestate;
struct shapestate 
{
	float * b_buf;
	float * a_buf;
	int fs;
	int mute;
};

class OpusAudioDecoder;
class OggStream;
class P2PEngine;
class MediaProcessor;

class OpusFileDecoder : public MediaCallbackInterface
{
public:
	OpusFileDecoder( P2PEngine& engine, MediaProcessor& mediaProcessor );
	virtual ~OpusFileDecoder();
 
	bool						beginFileDecode( const char * fileName, VxGUID& assetId, int pos0to100000 );
	int							decodedNextFrame( uint8_t * frameBuffer, int frameBufferLen );
	void						finishFileDecode( bool abortedByUser = false );

	virtual void				callbackAudioOutSpaceAvail( int freeSpaceLen );

protected:
	void						moveOpusFramesToOutput( uint8_t * outBuffer );
	bool						processOggFileHeader( MyOpusHeader& header, ogg_packet *op, float manualGain );
	int							opusFloatOutputToPcm(	float *			opusOutput, 
														int				channels, 
														int				frame_size, 
														SpeexResamplerState *resampler,
														int *			skip, 
														opus_int64		maxout );
	bool						seekOpusFile( FILE * fileHandle, int pos0to100000 );
	bool						readTotalSndFrames( FILE * fileHandle );
	bool						seekFile( FILE * fileHandle, uint64_t filePosition );

	int							calculateFileProgress( void );
	void						clearDecodedFrames( void );
	void						enableSpaceAvailCallback( bool enableCallback, bool lockResources );

	//=== vars ===//
	P2PEngine&					m_Engine;
	MediaProcessor&				m_MediaProcessor;
	OpusDecoder *				m_OpusDecoder;
	std::string					m_FileName;
	VxGUID						m_AssetId; 
	uint64_t							m_FileLen;
	uint64_t							m_FilePos;
	FILE *						m_FileHandle;
	uint64_t							m_TotalSndFramesInFile;
	uint64_t							m_ConsumedSndFrames;
	bool						m_DecoderInitialized;
	bool						m_InputInitialized;

	MyOpusHeader				m_MyOpusHeader;
	ogg_sync_state				m_OggSyncState;		// oy;
	ogg_stream_state			m_StreamState;		// os
	ogg_packet					m_OggPkt;			// op;
	ogg_page					m_OggPage;			// og
	bool						m_StreamInit;
	int							m_HasOpusStream;
	int							m_HasTagsPacket;
	ogg_int32_t					m_OpusSerialNum;
	int							m_Eos;
	int							m_PacketCount;
	opus_int64					m_LinkOut;
	float						m_ManualGain;
	int							m_Channels;
	int							m_Rate;
	int							m_Preskip;
	int 						m_GranOffset;
	SpeexResamplerState *		m_Resampler;
	float *						m_OpusOutput;
	int							m_DecodedSampleCnt;
	std::vector<char *>			m_DecodedFrames;
	bool						m_FirstDecodedFrame;
	bool						m_HeaderHasBeenRead;
	VxMutex						m_ResourceMutex;
	bool						m_SpaceAvailCallbackEnabled;
};

