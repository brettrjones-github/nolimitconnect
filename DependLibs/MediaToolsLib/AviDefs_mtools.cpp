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

#include "AviDefs.h"

#include <CoreLib/IsBigEndianCpu.h>

#include <memory.h>
#include <ctype.h>
#include <string.h>

namespace
{

	// definitions of AviStreamHdr flags
	//const uint32_t		AVIF_STREAMHDR_DISABLED					= 0x00010000;
	//const uint32_t		AVIF_STREAMHDR_VIDEOPALCHANGES			= 0x00020000;

	// definitions of AviVideoStream flags
	const uint32_t		AVIF_STREAM_HASINDEX					= 0x00000010;	// has index at end of file 
	//const uint32_t		AVIF_STREAM_MUSTUSEINDEX				= 0x00000020;
	const uint32_t		AVIF_STREAM_ISINTERLEAVED				= 0x00000100;
	const uint32_t		AVIF_STREAM_TRUSTCKTYPE					= 0x00000800;
	//const uint32_t		AVIF_STREAM_WASCAPTUREFILE				= 0x00010000;
	//const uint32_t		AVIF_STREAM_COPYRIGHTED					= 0x00020000;
}

//============================================================================
AviFourCC::AviFourCC()
: m_FourCC(0)
{
}

//============================================================================
AviFourCC::AviFourCC( uint8_t char1, uint8_t char2, uint8_t char3, uint8_t char4 )
{
	m_FourCC = ((uint32_t)char1 << 24) | ((uint32_t)char2 << 16) | ((uint32_t)char3 << 8) | char4;
	if( ! IsBigEndianCpu() )
	{
		m_FourCC = swapU32( m_FourCC );
	}
}

//============================================================================
void AviFourCC::setFourCC( uint8_t char1, uint8_t char2, uint8_t char3, uint8_t char4 )
{
	m_FourCC = ((uint32_t)char1 << 24) | ((uint32_t)char2 << 16) | ((uint32_t)char3 << 8) | char4;
	if( ! IsBigEndianCpu() )
	{
		m_FourCC = swapU32( m_FourCC );
	}
}

//============================================================================
bool AviFourCC::isIdEqual( const char * idStr ) // idStr example "LIST" or MJPG
{
	if( idStr && ( 4 <= strlen( idStr ) ) )
	{
		const char * tmp = (const char *)&m_FourCC;
		for( int i = 0; i < 4; i++ )
		{
			if( toupper( tmp[i] ) != idStr[i] )
			{
				return false;
			}
		}

		return true;
	}

	return false;
}

//============================================================================
AviAudioFormat::AviAudioFormat()
//: m_Id( 'f', 'm', 't', ' ' )
//, m_ThisStructSize( 16 )
: m_FormatTag( 1 )
, m_Channels( 1 )
, m_SamplesPerSec( 8000 )
, m_AvgBytesPerSec( 16000 )
, m_BlockAlign( 2 ) // 16 bit samples
, m_BitsPerSample( 16 )
{
}

//============================================================================
AviDataChunk::AviDataChunk()
: m_Id( 'd', 'a', 't', 'a' )
, m_PayloadDataSize( 0 )
{
}

//============================================================================
AviDataChunk::AviDataChunk( uint8_t typeChar1, uint8_t typeChar2, uint8_t typeChar3, uint8_t typeChar4 )
: m_Id( typeChar1, typeChar2, typeChar3, typeChar4 )
, m_PayloadDataSize( 0 )
{
}

//============================================================================
AviAllStreamsHdr::AviAllStreamsHdr()
: m_MicroSecBetweenFrames( 0 )	
, m_MaxBytesPerSec( 0 )			
, m_Padding( 0 )				
, m_AviHdrFlags( AVIF_STREAM_HASINDEX | AVIF_STREAM_ISINTERLEAVED | AVIF_STREAM_TRUSTCKTYPE )
, m_TotalFramesInFile( 0 )	
, m_InitialFrames( 0 )			
, m_StreamCount( 2 )		
, m_SuggestedBuffSize( 0x100000 )	
, m_ImageWidth( 320 )
, m_ImageHeight( 240 )
{
	memset( m_Reserved, 0, sizeof( m_Reserved ) );
}

//============================================================================
void AviAllStreamsHdr::setParams( uint32_t totalJpgDataLen, uint32_t frameCnt, uint32_t microSecPerFrame )
{
	m_MicroSecBetweenFrames = microSecPerFrame;
	m_MaxBytesPerSec = 1000000 * ( totalJpgDataLen / ( frameCnt ? frameCnt : 1 )) / ( microSecPerFrame ? microSecPerFrame : 1 );
	m_TotalFramesInFile = frameCnt;
}

//============================================================================
AviStreamHdr::AviStreamHdr( bool isAudio )
: m_StreamTypeId( isAudio ? 'a' : 'v', isAudio ? 'u' : 'i', 'd', 's' )
, m_CodecId( 'M', 'J', 'P', 'G' )
, m_StreamFlags( 0 )
, m_Priority( 0 )
, m_InitialFrames( 0 )
, m_Scale( 0 ) // us
, m_Rate( 1000000 )
, m_StartTime( 0 )
, m_Length( 0 ) // frame
, m_SuggestedBuffSize( isAudio ? 0 : 0x1075 )
, m_Quality( 0xffffffff )
, m_SampleSize( 0 )
, m_SomeZeroNumber( 0 )
, m_ImageWidth( isAudio ? 0 : 320 )
, m_ImageHeight( isAudio ? 0 : 240 )
{
}

//============================================================================
void AviStreamHdr::setParams( uint32_t frameCnt, uint32_t microSecPerFrame )
{
	m_Scale = microSecPerFrame;
	m_Length = frameCnt;
}

//============================================================================
AviVideoFormat::AviVideoFormat()
: m_SizeOfThisStructure( sizeof( AviVideoFormat ) )
, m_VidWidth( 320 )
, m_VidHeight( 240 )
, m_PlanesBitCnt( 1 + 24 * 256 * 256 )
, m_CompressionId( 'M', 'J', 'P', 'G' )
, m_ImageSize( 320 * 240 * 3 )
, m_XPelsMeter( 0 )
, m_YPelsMeter( 0 )
, m_NumColors( 0 )
, m_ImportantColors( 0 )
{
}

//============================================================================
void AviVideoFormat::setImageDimensions( uint32_t width, uint32_t height )
{
	m_VidWidth = width;
	m_VidHeight = height;
	m_ImageSize = m_VidWidth * m_VidHeight * 3;
}

//============================================================================
AviListBeginHdr::AviListBeginHdr()
: m_ListStructId( 'L', 'I', 'S', 'T' )
, m_SizeOfOwnerStructMinus8( 0 )
, m_ListTypeId()
{
}

//============================================================================
AviListBeginHdr::AviListBeginHdr( uint32_t sizeOfStructMinus8, uint8_t typeChar1, uint8_t typeChar2, uint8_t typeChar3, uint8_t typeChar4 )
: m_ListStructId( 'L', 'I', 'S', 'T' )
, m_SizeOfOwnerStructMinus8( sizeOfStructMinus8 )
, m_ListTypeId( typeChar1, typeChar2, typeChar3, typeChar4 )
{
}

//============================================================================
AviVidStream::AviVidStream()
: m_ListBeginHdr( sizeof(AviVidStream) - 8, 's', 't', 'r', 'l' )
, m_StreamHdrChunkId( 's', 't', 'r', 'h' )
, m_StreamHdrChunkLen( sizeof( AviStreamHdr ) )
, m_StreamHdr( false )
, m_StreamFormatChunkId( 's', 't', 'r', 'f' )
, m_StreamFormatChunkLen( sizeof( AviVideoFormat ) )
, m_StreamFormat(  )
{
}

//============================================================================
void AviVidStream::setParams( uint32_t imageWidth, uint32_t imageHeight, uint32_t frameCnt, uint32_t microSecPerFrame )
{
	m_StreamFormat.setImageDimensions( imageWidth, imageHeight );
	m_StreamHdr.setParams( frameCnt, microSecPerFrame );
}

//============================================================================
AviAudioStream::AviAudioStream()
: m_ListBeginHdr( sizeof(AviAudioStream) - 8, 's', 't', 'r', 'l' )
, m_StreamHdrChunkId( 's', 't', 'r', 'h' )
, m_StreamHdrChunkLen( sizeof( AviStreamHdr ) )
, m_StreamHdr( true )
, m_StreamFormatChunkId( 's', 't', 'r', 'f' )
, m_StreamFormatChunkLen( sizeof( AviAudioFormat ) )
, m_StreamFormat(  )
{
	m_StreamHdr.m_CodecId.setFourCC( 1, 0, 0, 0 );
	m_StreamHdr.setRate( 8000 );
	m_StreamHdr.setScale( 1 );
	m_StreamHdr.m_Length = 1280;
	m_StreamHdr.m_Quality = 0xffffffff;
	m_StreamHdr.m_SampleSize = 2;
}

//============================================================================
void AviAudioStream::setPcmChunkTotals( uint32_t frameCnt, uint32_t frameLenInBytes )
{
	m_StreamHdr.m_SuggestedBuffSize = frameLenInBytes;
	m_StreamHdr.m_Length = frameCnt * (frameLenInBytes/2);
}

//============================================================================
AviVidJunk::AviVidJunk()
: m_JunkId( 'J', 'U', 'N', 'K' )
, m_JunkLen( sizeof( AviVidJunk )  - 8 )
{
	memset( m_JunkData, 0, sizeof( m_JunkData ) );
	strcpy( (char *)m_JunkData, "Created by My P2P Web at http://www.nolimitconnect.org" );
}

//============================================================================
AviAudioJunk::AviAudioJunk()
: m_JunkId( 'J', 'U', 'N', 'K' )
, m_JunkLen( sizeof( AviAudioJunk )  - 8 )
{
	memset( m_JunkData, 0, sizeof( m_JunkData ) );
	strcpy( (char *)m_JunkData, "Created by My P2P Web at http://www.nolimitconnect.org" );
}

//============================================================================
AviJunkOdml::AviJunkOdml()
: m_JunkId( 'J', 'U', 'N', 'K' )
, m_JunkLen( sizeof( AviJunkOdml )  - 8 )
{
	memset( m_JunkData, 0, sizeof( m_JunkData ) );
	strcpy( (char *)m_JunkData, "odmldmlh" );
}

//============================================================================
AviJunkInfo::AviJunkInfo()
: m_JunkId( 'J', 'U', 'N', 'K' )
, m_JunkLen( sizeof( AviJunkInfo )  - 8 )
{
	memset( m_JunkData, 0, sizeof( m_JunkData ) );
}

//============================================================================
AviIdit::AviIdit()
: m_IditId( 'I', 'D', 'I', 'T' )
, m_IditLen( sizeof( AviIdit )  - 8 )
{
	memset( m_DateData, 0, sizeof( m_DateData ) );
	strcpy( (char *)m_DateData, "Thu Mar 4 21:56:53 1962" );
}

//============================================================================
AviSoftwareInfo::AviSoftwareInfo()
: m_AviListBeginHdr( sizeof(AviSoftwareInfo) - 8, 'I', 'N', 'F', 'O')
, m_IsftChunk( 'I', 'S', 'F', 'T' )
{
	m_IsftChunk.m_PayloadDataSize = sizeof( m_InfoData );
	memset( m_InfoData, 0, sizeof( m_InfoData ) );
	strcpy( (char *)m_InfoData, "MyP2PWeb" );
}

//============================================================================
AviVideoHdr::AviVideoHdr()
: m_AviListBeginHdr( ( sizeof( AviVideoHdr ) - ( sizeof( m_SoftwareInfo ) + sizeof( m_FramesBeginHdr ) ) ) - 8, 'h', 'd', 'r', 'l')
, m_AviAllStreamsId( 'a', 'v', 'i', 'h' )
, m_SizeOfAllStreamsHdr( sizeof( AviAllStreamsHdr ) )
, m_AviAllStreamsHdr()
, m_VidStream()
, m_FramesBeginHdr( 0, 'm', 'o', 'v', 'i' )
{
}

//============================================================================
bool AviVideoHdr::setAviParameters(	uint32_t	imageWidth, 
									uint32_t	imageHeight, 
									uint32_t	microSecBetweenFrames, 
									uint32_t	frameCnt, 
									uint32_t	totalJpgDataLen )
{
	m_AviAllStreamsHdr.setImageDimensions( imageWidth, imageHeight );
	m_AviAllStreamsHdr.setParams( totalJpgDataLen, frameCnt, microSecBetweenFrames );
	m_VidStream.setParams( imageWidth, imageHeight, frameCnt, microSecBetweenFrames );
	m_FramesBeginHdr.setSizeVariable( totalJpgDataLen + 8 * frameCnt + 4 );  
	//m_OdmlList.setFrames( frameCnt );
	return true;
}

//============================================================================
void AviVideoHdr::setPcmChunkTotals( uint32_t totalAudioChunks, uint32_t microSecPerChunk )
{
	m_AudioStream.setPcmChunkTotals( totalAudioChunks, microSecPerChunk );
};

//============================================================================
AviJpgHdr::AviJpgHdr()
: m_00db( '0', '0', 'd', 'c' )
, m_DataLen( 0 )
//, m_Avi1CC( 'A', 'V', 'I', '1' )
, m_Avi1CC( 'J', 'F', 'I', 'F' )
{
}

//============================================================================
AviAudioHdr::AviAudioHdr()
: m_01wb( '0', '1', 'w', 'b' )
, m_DataLen( 0 )
{
}

//============================================================================
AviFrameIndex::AviFrameIndex()
: m_00db( '0', '0', 'd', 'b' )
, m_Magic16( 16 )
{
}
