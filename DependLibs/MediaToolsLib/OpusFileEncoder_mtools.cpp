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
// http://www.gotvptop.com
//============================================================================

#include "OpusFileEncoder.h"
#include "OpusAudioEncoder.h"
#include "OggStream.h"

#include "opus_defines.h"
#include "opus.h"
//#include "debug.h"
#include "opus_types.h"

#include <CoreLib/IsBigEndianCpu.h>
#include <CoreLib/VxDebug.h>
#include <CoreLib/VxFileUtil.h>
 
namespace
{

	#define PCM_SAMPLE_CNT						640
	#define FLAC_WAVE_BUF_SIZE					(PCM_SAMPLE_CNT*2)		
}

//============================================================================
OpusFileEncoder::OpusFileEncoder(  )
: m_AudioEncoder( 0 )
, m_OggStream( * ( new OggStream() ) )
, m_FileHandle( 0 )
, m_TotalSndFramesInFile( 0 )
, m_EncoderInitialized( false )
{
	//uint32_t u32Len = 0;
	//void * pvData = 0;
	//VxFileUtil::readWholeFile( "F://audio_test/bluegrass.pcm", &pvData, &u32Len );
	//if( u32Len )
	//{
		//uint8_t * pcmData = ( uint8_t *) pvData;
		//beginFileEncode( "F://audio_test/opus_outTest.opus" );
		//while( u32Len > 1280 )
		//{
			//encodePcmData( (uint16_t *) pcmData, 1280 );
			//pcmData += 1280;
			//u32Len -= 1280;
		//}
		//
		//finishFileEncode();
		//delete pvData;
	//}
}

//============================================================================
OpusFileEncoder::~OpusFileEncoder()
{
	delete m_AudioEncoder;
	delete &m_OggStream;
}

//============================================================================
bool OpusFileEncoder::beginFileEncode( const char * fileName, int sampleRate, int channels )
{
	m_FileName = fileName;
	m_FileHandle = fopen( fileName, "wb+" );
	if( 0 == m_FileHandle )
	{
		LogMsg( LOG_ERROR, "OpusFileEncoder::beginWrite could not open file to write %s\n", fileName );
		return false;
	}

	m_TotalSndFramesInFile = 0;
	m_EncoderInitialized = createAudioEncoder( sampleRate, channels );
	if( m_EncoderInitialized )
	{
		//Initialize Ogg stream struct
		m_EncoderInitialized = m_OggStream.openOggStream( m_FileHandle );
		if( m_EncoderInitialized )
		{
			//Write opus header into buffer
			unsigned char opusHeaderData[ 100 ];
			// writes "OpusHead" and channel/stream info into buffer ( 19 bytes )
			int pktDataLen = m_OggStream.writeHeader( m_AudioEncoder->getOpusHeader(), opusHeaderData, 100 ); 
			if( 0 == pktDataLen )
			{
				m_EncoderInitialized = false;
			}
		}
		else
		{
			fclose( m_FileHandle );
			m_FileHandle = 0;
			VxFileUtil::deleteFile( m_FileName.c_str() );
		}
	}
	else
	{
		fclose( m_FileHandle );
		m_FileHandle = 0;
		VxFileUtil::deleteFile( m_FileName.c_str() );
	}

	return m_EncoderInitialized;
}

//============================================================================
bool OpusFileEncoder::createAudioEncoder( int sampleRate, int channels )
{
	if( m_AudioEncoder )
	{
		delete m_AudioEncoder;
	}

	m_AudioEncoder = new OpusAudioEncoder( sampleRate, channels );
	return m_AudioEncoder->isInitialized();
}


//============================================================================
int OpusFileEncoder::encodePcmData( int16_t * pcmData, uint16_t pcmDataLen )
{
	if( false == m_EncoderInitialized )
	{
		LogMsg( LOG_ERROR, "ERROR: OpusFileEncoder::writePcmData not initialized\n" );
		return false;
	}

	m_TotalSndFramesInFile++;
	int32_t encodeFrame1Len;
	int32_t encodeFrame2Len;
	uint8_t 	outBuf[ 512 ];
	int result = m_AudioEncoder->encodePcmData(	pcmData, 
													pcmDataLen, 
													outBuf,	
													encodeFrame1Len,
													encodeFrame2Len );
	if( result )
	{
		result = m_OggStream.writeEncodedFrame( outBuf, encodeFrame1Len );
		if( result )
		{
			result = m_OggStream.writeEncodedFrame( &outBuf[encodeFrame1Len], encodeFrame2Len );
		}
	}

	return result;
}

//============================================================================
int OpusFileEncoder::writeEncodedFrame( uint8_t * encodedFrameData, int32_t encodedLen )
{
	if( false == m_EncoderInitialized )
	{
		LogMsg( LOG_ERROR, "ERROR: OpusFileEncoder::writeEncodedFrame not initialized\n" );
		return 0;
	}

	m_TotalSndFramesInFile++;
	return m_OggStream.writeEncodedFrame( encodedFrameData, encodedLen );
}

//============================================================================
void OpusFileEncoder::finishFileEncode( void )
{
	m_OggStream.closeOggStream();
	if( m_FileHandle )
	{
		fclose( m_FileHandle );
		m_FileHandle = 0;
		bool writeResult = false;
		uint64_t fileLen = VxFileUtil::getFileLen( m_FileName.c_str() );
		FILE * fileHandle = fopen( m_FileName.c_str(), "rb+" );
		if( 0 != fileHandle )
		{
			writeResult = writeTotalSndFrames( fileHandle );
			VxFileUtil::fileSeek( fileHandle, fileLen );
			fclose( fileHandle );
		}

		if( false == writeResult )
		{
			LogMsg( LOG_ERROR, "OpusFileEncoder::finishFileEncode could not write frame count to file %s\n", m_FileName.c_str() );
		}
	}
}

//============================================================================
bool OpusFileEncoder::writeTotalSndFrames( FILE * fileHandle )
{
	bool writeSuccess = false;
	std::string hexTotal;
	VxFileUtil::u64ToHexAscii( htonU64( m_TotalSndFramesInFile ), hexTotal );
	uint32_t totalFramesOffs = 0xAA;
	if( ( 16 == hexTotal.length() ) && ( 0  == VxFileUtil::fileSeek( fileHandle, totalFramesOffs ) ) )
	{
		if( 16 == fwrite( hexTotal.c_str(), 1, 16, fileHandle ) )
		{
			writeSuccess = true;
		}
	}

	if( false == writeSuccess )
	{
		LogMsg( LOG_ERROR, "OpusFileEncoder::writeTotalSndFrames FAILED %s\n", m_FileName.c_str() );
	}

	return writeSuccess;
}



