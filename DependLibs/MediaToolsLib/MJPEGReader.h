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

#include "AviDefs.h"

#include <GuiInterface/IFromGui.h>
#include <PktLib/VxCommon.h>

#include <CoreLib/VxTimer.h>
#include <CoreLib/VxMutex.h>
#include <CoreLib/VxThread.h>
#include <CoreLib/VxSemaphore.h>
#include <CoreLib/MediaCallbackInterface.h>

#include <string>
#include <stdio.h>
#include <memory.h>

class P2PEngine;
class MediaProcessor;
class IToGui;

enum EAviChunkType
{
	eAviChunkTypeVideo,
	eAviChunkTypeAudio,

	eMaxAviChunkType
};

class AviChunkOffs
{
public:
	AviChunkOffs( EAviChunkType chunkType, uint32_t chunkOffs, uint32_t chunkLen )
		: m_ChunkType( chunkType )
		, m_ChunkOffs( chunkOffs )
		, m_ChunkLen( chunkLen )
		, m_DataPtr( 0 )
	{
	}

	void						setDataPtr( uint8_t * data )		{ m_DataPtr = data; }
	uint8_t *					getDataPtr( void)					{ return m_DataPtr; }
	void						deleteDataPtr( void )				{ delete m_DataPtr; m_DataPtr = 0; }
	bool						isVideoChunk( void )				{ return ( eAviChunkTypeVideo == m_ChunkType ); }	
	bool						isDataEmpty( void )					{ return ( 0 == m_DataPtr ); }	
	bool						hasData( void )						{ return ( 0 != m_DataPtr ); }	

	EAviChunkType				m_ChunkType;
	uint32_t					m_ChunkOffs;
	uint32_t					m_ChunkLen;
	uint8_t *					m_DataPtr;
};

class MJPEGReader : public MediaCallbackInterface
{
public:
	MJPEGReader( P2PEngine& engine, MediaProcessor& mediaProcessor );
	virtual ~MJPEGReader();

	void						setIsPlaying( bool isPlaying )				{ m_IsPlaying = isPlaying; }
	bool						getIsPlaying( void )						{ return m_IsPlaying; }
	void						setIsPlayingPaused( bool isPaused );
	bool						getIsPlayingPaused( void )					{ return m_IsPlayingPaused; }

	bool						fromGuiIsMyP2PWebVideoFile( const char* fileName );	
	bool						fromGuiAssetAction( AssetBaseInfo& assetInfo, EAssetAction assetAction, int pos0to100000  );

	bool						startVideoRead( const char* fileName, VxGUID& assetId, int pos0to100000 );
	void						stopVideoRead( void );
	bool						playOneFrame( const char* fileName, VxGUID& assetId );						

	void						readerThread( void );
	void						lockResources( void )			{ m_ResMutex.lock(); }
	void						unlockResources( void )			{ m_ResMutex.unlock(); }

	void						setIsVidThreadRunning( bool isRunning );
	bool						getIsVidThreadRunning( void )							{ return m_IsVidThreadRunning; }
	void						setIsStartThreadCommanded( bool isStarted )				{ m_IsStartThreadCommanded = isStarted; }
	bool						getIsStartThreadCommanded( void )						{ return m_IsStartThreadCommanded; }

protected:
	virtual void				callbackAudioOutSpaceAvail( int freeSpaceLen );

	void						closeAviFile( void );
	bool						readRiffHeader( FILE * fileHandle );
	bool						readVideoHeader( void );
	bool						parseChunkOffsets( void );
	void						startVideoThread( void );
	void						stopVideoThread( void );
	bool						readChunkFromDisk( FILE * fileHandle, AviChunkOffs * chunk );
	void						calculateNewMilliSecPerFrame( unsigned int audChunkIdx );
	void						waitForAudio( void );
	void						cleanupReadChunks( void );
	bool						readFirstVidFrameFromFile( FILE * fileHandle, const char* fileName, VxGUID& assetId );
	void						resetVariables( void );


	//=== vars ===//
	P2PEngine&					m_Engine; 
	MediaProcessor&				m_MediaProcessor;

	EPluginType					m_EPluginType;
	bool						m_IsPlaying;
	bool						m_IsPlayingPaused;
	std::string					m_FileName;
	uint32_t							m_FileLen;
	VxGUID						m_AssetId;
	FILE *						m_FileHandle;

	int							m_MicroSecBetweenFrames;
	uint32_t							m_TotalJpgDataLen;
	uint32_t							m_TotalFrameCnt;

	VxTimer						m_PlayElapseTimer;
	double						m_TotalElapsedMs;
	bool						m_IsFirstFrameAfterResumePlaying;

	AviAllStreamsHdr			m_AllStreamsHdr;
	AviStreamHdr				m_VidStreamHdr;
	uint32_t							m_StreamsBeginOffs;
	uint32_t							m_StreamsEndOffs;

	VxMutex						m_ResMutex;
	VxThread					m_VidThread;
	VxSemaphore					m_VidSemaphore;
	std::vector<AviChunkOffs>	m_AviChunkList;
	AviJpgHdr					m_AviJpgHdr;

	unsigned int				m_VidWriteIdx;
	int							m_VidChunksInMemory;

	unsigned int				m_AudWriteIdx;
	int							m_AudioChunksInMemory;

	unsigned int				m_ChunkReadIdx;
	bool						m_AllChunkRead;
	float						m_MilliSecPerFrame;
	bool						m_Initialized;
	bool						m_IsStartThreadCommanded;
	bool						m_IsVidThreadRunning;
	VxTimer						m_TempTimer;
};
