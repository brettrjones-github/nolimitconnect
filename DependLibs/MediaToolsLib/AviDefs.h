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

#define RIFF_2GB_FILESIZE_LIMIT			2147483648ULL 
#define AVI_FILEPOS_RIFF				0 

#pragma pack(push)
#pragma pack(1)
class AviFourCC
{
public:
	AviFourCC();
	AviFourCC( uint8_t char1, uint8_t char2, uint8_t char3, uint8_t char4 );

	void						setFourCC( uint8_t char1, uint8_t char2, uint8_t char3, uint8_t char4 );

	bool						isIdEqual( const char* idStr ); // idStr example "LIST" or MJPG

	//=== vars ===//
	uint32_t							m_FourCC;
};

class AviRiffHeader
{   
public:
	AviRiffHeader()
	: m_RiffFourCC( 'R', 'I', 'F', 'F' )
	, m_TotalRiffLen( 0 )
	, m_AviFourCC( 'A', 'V', 'I', ' ' )
	{
	}

	void						setTotalRiffLen( uint32_t len )		{ m_TotalRiffLen = len; }
	uint32_t							getTotalRiffLen( void )			{ return m_TotalRiffLen; }

private:
	//=== vars ===//
	AviFourCC					m_RiffFourCC;
	uint32_t							m_TotalRiffLen;
	AviFourCC					m_AviFourCC;
};

class AviAudioFormat
{   
public:
	AviAudioFormat();

	//=== vars ===//
	//AviFourCC					m_Id;					// "fmt "
	//uint32_t							m_ThisStructSize;		// 16 if no extra data
	uint16_t							m_FormatTag;			// format type WAVE_FORMAT_PCM=1
	uint16_t							m_Channels;				// number of channels ( 1 = mono, 2 = stereo...) 
	uint32_t							m_SamplesPerSec;		// sample rate = 8000 or 11025 or 22050 or 44100
	uint32_t							m_AvgBytesPerSec;		// = BlockAlign * dwSamplesPerSec 
	uint16_t							m_BlockAlign;			// = Channels * (wBitsPerSample==8?1:2)
	uint16_t							m_BitsPerSample;		// number of bits per sample of mono data = 8 or 16, for bits per sample
	//uint16_t							m_ExtraSize;            // the count in bytes of the size of
	// extra information 
};

class AviDataChunk
{
public:
	AviDataChunk();
	AviDataChunk( uint8_t typeChar1, uint8_t typeChar2, uint8_t typeChar3, uint8_t typeChar4 );

	//=== vars ===//
	AviFourCC					m_Id;					// = "data" type id
	uint32_t							m_PayloadDataSize;		// = data size, size of the following array
	//uint8_t							m_PayloadData[1];		// = the raw data goes here
};

class AviIdit
{
public:
	AviIdit();

	//=== vars ===//
	AviFourCC					m_IditId;
	uint32_t							m_IditLen;
	uint8_t							m_DateData[26];
};

class AviVidJunk
{
public:
	AviVidJunk();

	//=== vars ===//
	AviFourCC					m_JunkId;
	uint32_t							m_JunkLen;
	uint8_t							m_JunkData[0x1018 + 0x4c];
};

class AviAudioJunk
{
public:
	AviAudioJunk();

	//=== vars ===//
	AviFourCC					m_JunkId;
	uint32_t							m_JunkLen;
	uint8_t							m_JunkData[0x1018];
};

class AviJunkOdml
{
public:
	AviJunkOdml();

	//=== vars ===//
	AviFourCC					m_JunkId;
	uint32_t							m_JunkLen;
	uint8_t							m_JunkData[ 0x104 ];
};

class AviJunkInfo
{
public:
	AviJunkInfo();

	//=== vars ===//
	AviFourCC					m_JunkId;
	uint32_t							m_JunkLen;
	uint8_t							m_JunkData[0x3f8];
};

class AviAllStreamsHdr 
{
public:
	AviAllStreamsHdr();

	void						setImageDimensions( uint32_t imageWidth, uint32_t imageHeight ) { m_ImageWidth = imageWidth; m_ImageHeight = imageHeight; }
	void						setParams( uint32_t totalJpgDataLen, uint32_t frameCnt, uint32_t microSecPerFrame );

	//=== vars ===//
	uint32_t							m_MicroSecBetweenFrames;	// can be zero
	uint32_t							m_MaxBytesPerSec;			// max xfer rate
	uint32_t							m_Padding;					// pad to multiples of this size (normally 2048)
	uint32_t							m_AviHdrFlags;
	uint32_t							m_TotalFramesInFile;	
	uint32_t							m_InitialFrames;			//
	uint32_t							m_StreamCount;				// number of streams 1 video + 1 audio = 2 if have audio
	uint32_t							m_SuggestedBuffSize;		//   
	uint32_t							m_ImageWidth;
	uint32_t							m_ImageHeight;
	uint32_t							m_Reserved[4];
};


class AviStreamHdr
{
public:
	AviStreamHdr( bool isAudio = false );

	void						setParams( uint32_t frameCnt, uint32_t microSecPerFrame );
	void						setRate( uint32_t rate )				{ m_Rate = rate; }
	void						setScale( uint32_t scale )			{ m_Scale = scale; }

	//=== vars ===//
	AviFourCC					m_StreamTypeId;			// vids = video, auds = audio, txts = subtitle
	AviFourCC					m_CodecId;				// MJPG 
	uint32_t							m_StreamFlags;
	uint32_t							m_Priority;
	uint32_t							m_InitialFrames;		// ??
	uint32_t							m_Scale;
	uint32_t							m_Rate;					// rate / scale = samples/second
	uint32_t							m_StartTime;			// number of frames to play before starting
	uint32_t							m_Length;				// in units above
	uint32_t							m_SuggestedBuffSize;
	uint32_t							m_Quality;
	uint32_t							m_SampleSize;
	uint32_t							m_SomeZeroNumber;
	uint16_t							m_ImageWidth;
	uint16_t							m_ImageHeight;
};

class AviVideoFormat
{   
public:
	AviVideoFormat();

	void						setImageDimensions( uint32_t width, uint32_t height );

	//=== vars ===//
	uint32_t							m_SizeOfThisStructure;
	uint32_t							m_VidWidth;
	uint32_t							m_VidHeight;
	uint32_t							m_PlanesBitCnt;
	AviFourCC					m_CompressionId;
	uint32_t							m_ImageSize;
	uint32_t							m_XPelsMeter;
	uint32_t							m_YPelsMeter;
	uint32_t							m_NumColors;
	uint32_t							m_ImportantColors;
};

class AviListBeginHdr
{
public:
	AviListBeginHdr();
	AviListBeginHdr( uint32_t sizeOfStructMinus8, uint8_t typeChar1, uint8_t typeChar2, uint8_t typeChar3, uint8_t typeChar4 );

	void						setSizeVariable( uint32_t sizeVar )			{ m_SizeOfOwnerStructMinus8 = sizeVar; }

	//=== vars ===//
	AviFourCC					m_ListStructId;		// always "LIST"
	uint32_t							m_SizeOfOwnerStructMinus8;
	AviFourCC					m_ListTypeId;		// fourcc of type of list
};

class AviVidStream
{
public:
	AviVidStream();

	void						setParams( uint32_t imageWidth, uint32_t imageHeight, uint32_t frameCnt, uint32_t microSecPerFrame );

	//=== vars ===//
	AviListBeginHdr				m_ListBeginHdr;

	// stream header chunk
	AviFourCC					m_StreamHdrChunkId;
	uint32_t							m_StreamHdrChunkLen;
	AviStreamHdr				m_StreamHdr;

	// video stream format chunk
	AviFourCC					m_StreamFormatChunkId;
	uint32_t							m_StreamFormatChunkLen;
	AviVideoFormat				m_StreamFormat;
	//AviVidJunk					m_VidJunk;
};

class AviAudioStream
{
public:
	AviAudioStream();

	void						setPcmChunkTotals( uint32_t totalAudioChunks,  uint32_t frameLenInBytes );
	void						setPcmAudioRate( uint32_t rate )				{ m_StreamHdr.setRate( rate ); }

	//=== vars ===//
	AviListBeginHdr				m_ListBeginHdr;

	// stream header chunk
	AviFourCC					m_StreamHdrChunkId;
	uint32_t							m_StreamHdrChunkLen;
	AviStreamHdr				m_StreamHdr;

	// audio stream format chunk
	AviFourCC					m_StreamFormatChunkId;
	uint32_t							m_StreamFormatChunkLen;
	AviAudioFormat				m_StreamFormat;
	AviAudioJunk				m_AudioJunk;
};

class AviSoftwareInfo
{
public:
	AviSoftwareInfo();

	AviListBeginHdr				m_AviListBeginHdr;
	AviDataChunk				m_IsftChunk;
	uint8_t							m_InfoData[0x0E];
	//AviJunkInfo					m_InfoJunk;
};

class AviVideoHdr
{
public:
	AviVideoHdr();

	bool						setAviParameters(	uint32_t	imageWidth, 
													uint32_t	imageHeight, 
													uint32_t	microSecBetweenFrames, 
													uint32_t	frameCnt, 
													uint32_t	totalJpgDataLen );
	void						setPcmChunkTotals( uint32_t totalAudioChunks, uint32_t microSecPerChunk );

	//=== vars ===//
	AviListBeginHdr				m_AviListBeginHdr;

	AviFourCC					m_AviAllStreamsId;
	uint32_t							m_SizeOfAllStreamsHdr;
	AviAllStreamsHdr			m_AviAllStreamsHdr;

	AviVidStream				m_VidStream;
	AviAudioStream				m_AudioStream;
	//AviIdit						m_Idit;
	AviSoftwareInfo				m_SoftwareInfo;		

	AviListBeginHdr				m_FramesBeginHdr;
};

class AviJpgHdr
{
public:
	AviJpgHdr();

	AviFourCC					m_00db;
	uint32_t							m_DataLen;
	char						m_First6BytesOfJpg[6];
	AviFourCC					m_Avi1CC;
};

class AviAudioHdr
{
public:
	AviAudioHdr();

	AviFourCC					m_01wb;
	uint32_t							m_DataLen;
};


class AviFrameIndex
{
public:
	AviFrameIndex();

	void						setOffset( uint32_t offset )				{ m_DataOffset = offset; }
	void						setDataLen( uint32_t dataLen )			{ m_DataLen = dataLen; }

	AviFourCC					m_00db;
	uint32_t							m_Magic16;
	uint32_t							m_DataOffset;
	uint32_t							m_DataLen;
};

#pragma pack(pop)

