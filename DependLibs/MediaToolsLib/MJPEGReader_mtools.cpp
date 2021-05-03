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

#include "MJPEGReader.h"

#include <GoTvCore/GoTvP2P/P2PEngine/P2PEngine.h>
#include <GuiInterface/IToGui.h>
#include <GoTvCore/GoTvP2P/AssetMgr/AssetInfo.h>
#include <GoTvCore/GoTvP2P/MediaProcessor/MediaProcessor.h>

#include <CoreLib/VxFileUtil.h>

namespace
{
	const uint32_t AVI_FILEPOS_MYJUNK			= 0x140; 
	//const uint32_t AVI_FILEPOS_JPG_INDEXES		= sizeof( AviRiffHeader ) + sizeof( AviVideoHdr ); 

	//============================================================================
	static uint32_t MJPEGReaderThreadFunc( void * pvContext )
	{
		VxThread * poThread = (VxThread *)pvContext;
		poThread->setIsThreadRunning( true );
		MJPEGReader * reader = (MJPEGReader *)poThread->getThreadUserParam();
        if( reader )
        {
            reader->setIsVidThreadRunning( true );
            reader->setIsStartThreadCommanded( false );
            reader->readerThread();
            reader->setIsVidThreadRunning( false );
        }

		poThread->threadAboutToExit();
		return 0;
	}
}

//============================================================================
MJPEGReader::MJPEGReader( P2PEngine& engine, MediaProcessor& mediaProcessor )
: m_Engine( engine )
, m_MediaProcessor( mediaProcessor )
, m_EPluginType( ePluginTypeMJPEGReader )
, m_IsPlaying( false )
, m_IsPlayingPaused( false )
, m_FileName( "" )
, m_FileHandle( 0 )
, m_MicroSecBetweenFrames( 0 )
, m_TotalJpgDataLen( 0 )
, m_TotalFrameCnt( 0 )
, m_TotalElapsedMs( 0 )
, m_IsFirstFrameAfterResumePlaying( 0 )
, m_Initialized( false )
, m_IsStartThreadCommanded( false )
, m_IsVidThreadRunning( false )
{
}

//============================================================================
void MJPEGReader::resetVariables( void )
{
	m_IsPlaying							= false;
	m_IsPlayingPaused					= false;
	m_FileLen							= 0;
	if( 0 != m_FileHandle )
	{
		fclose( m_FileHandle );
		m_FileHandle = 0;
	}

	m_MicroSecBetweenFrames				= 0;
	m_TotalJpgDataLen					= 0;
	m_TotalFrameCnt						= 0;

	m_PlayElapseTimer.startTimer();
	m_TotalElapsedMs					= 0;
	m_IsFirstFrameAfterResumePlaying	= false;
	m_StreamsBeginOffs					= 0;
	m_StreamsEndOffs					= 0;
	m_VidWriteIdx						= 0;
	m_VidChunksInMemory					= 0;

	m_AudWriteIdx						= 0;
	m_AudioChunksInMemory				= 0;

	m_ChunkReadIdx						= 0;
	m_AllChunkRead						= false;
	m_MilliSecPerFrame					= 80;
	m_Initialized						= false;
	m_IsStartThreadCommanded			= false;
	m_IsVidThreadRunning				= false;
	cleanupReadChunks();
}

//============================================================================
void MJPEGReader::setIsVidThreadRunning( bool isRunning )
{
	LogMsg( LOG_INFO, "MJPEGReader::stopVideoRead wantMediaInput start %d\n", isRunning );
	m_Engine.getMediaProcesser().wantMediaInput( eMediaInputMixer, this, (void *)ePluginTypeMJPEGReader, isRunning );
	LogMsg( LOG_INFO, "MJPEGReader::stopVideoRead wantMediaInput end\n" );
	m_IsVidThreadRunning = isRunning; 
}

//============================================================================
bool MJPEGReader::fromGuiIsMyP2PWebVideoFile( const char * fileName )
{
	uint64_t fileLen = VxFileUtil::getFileLen( fileName );
	if( fileLen < 4000 )
	{
		return false;
	}

	FILE * fileHandle = VxFileUtil::fileOpen( fileName, "rb" );
	if( 0 == fileHandle )
	{
		LogMsg( LOG_ERROR, "MJPEGReader::fromGuiIsMyP2PWebVideoFile could not open file %s\n", fileName );
		return false;
	}

	if( 0 != VxFileUtil::fileSeek( fileHandle, AVI_FILEPOS_MYJUNK ) )
	{
		fclose( fileHandle );
		LogMsg( LOG_ERROR, "MJPEGReader::fromGuiIsMyP2PWebVideoFile could not seek file %s\n", fileName );
		return false;
	}

	char junkBuf[32];
	if( 32 != fread( junkBuf, 1, 32, fileHandle ) )
	{
		fclose( fileHandle );
		LogMsg( LOG_ERROR, "MJPEGReader::fromGuiIsMyP2PWebVideoFile could not read file %s\n", fileName );
		return false;
	}
	
	fclose( fileHandle );
	if( 0 != strncmp( junkBuf, "Created by My P2P Web", 21 ) )
	{
		LogMsg( LOG_INFO, "MJPEGReader::fromGuiIsMyP2PWebVideoFile is not My P2P Video File %s\n", fileName );
		return false;
	}

	return true;
}

//============================================================================
bool MJPEGReader::fromGuiAssetAction( AssetBaseInfo& assetInfo, EAssetAction assetAction, int pos0to100000  )
{
	bool result = false;
	switch( assetAction )
	{
	case eAssetActionPlayEnd:
		if( getIsPlaying() )
		{
			stopVideoRead();
			result = true;
		}
		break;

	case eAssetActionPlayBegin:
		if( !getIsPlaying() 
			|| (getIsPlaying() && m_AssetId == assetInfo.getAssetUniqueId() ) )
		{
			result = startVideoRead( assetInfo.getAssetName().c_str(), assetInfo.getAssetUniqueId(), pos0to100000 );
		}
		else
		{
			LogMsg( LOG_ERROR, "Already playing a movie file\n" );
			result = false;
		}

		break;

	case eAssetActionPlayOneFrame:
		result = playOneFrame( assetInfo.getAssetName().c_str(), assetInfo.getAssetUniqueId() );
		break;

	case eAssetActionPlayPause:
		setIsPlayingPaused( true );
		break;

	case eAssetActionPlayResume:
		setIsPlayingPaused( false );
		break;

	case eAssetActionPlayProgress:
	case eAssetActionPlayCancel:
	default:
		break;
	}

	return result;
}

//============================================================================
bool MJPEGReader::startVideoRead( const char * fileName, VxGUID& assetId, int pos0to100000 )
{
	stopVideoRead();
	resetVariables();
	m_FileName = fileName;
	m_AssetId = assetId;

	m_FileLen = (uint32_t)VxFileUtil::getFileLen( m_FileName.c_str() );
	if( 3000 > m_FileLen )
	{
		LogMsg( LOG_ERROR, "MJPEGReader::startAviRead to short len %lld file %s\n", m_FileLen, m_FileName.c_str() );
		return false;
	}

	m_FileHandle = VxFileUtil::fileOpen( m_FileName.c_str(), "rb" );
	if( 0 == m_FileHandle )
	{
		LogMsg( LOG_ERROR, "MJPEGReader::startAviRead could not open file %s\n", m_FileName.c_str() );
		return false;
	}

	if( false == readRiffHeader( m_FileHandle ) )
	{
		fclose( m_FileHandle );
		m_FileHandle = 0;
		return false;
	}

	if( false == readVideoHeader() )
	{
		fclose( m_FileHandle );
		m_FileHandle = 0;
		return false;
	}

	if( 0 != VxFileUtil::fileSeek( m_FileHandle, m_StreamsBeginOffs ) )
	{
		LogMsg( LOG_ERROR, "MJPEGReader::startAviRead could not seek to stream %d file %s\n", m_StreamsBeginOffs, m_FileName.c_str() );
		fclose( m_FileHandle );
		m_FileHandle = 0;
		return false;
	}

	m_Initialized = true;

	if( pos0to100000 )
	{
		double totalCnt = (double)m_AviChunkList.size();
		unsigned int posIdx = (int)( totalCnt * ( (double)pos0to100000 / 100000.0 ) );
		if( posIdx > ( m_AviChunkList.size() - 10 ) )
		{
			// seek at least 10 frames from end
			posIdx = ( m_AviChunkList.size() - 10 );
		}

		m_VidWriteIdx			= posIdx;
		m_AudWriteIdx			= posIdx;
		m_ChunkReadIdx			= posIdx;
	}

	setIsPlayingPaused( false );
	setIsPlaying( true );
	startVideoThread();
	return true;
}

//============================================================================
void MJPEGReader::stopVideoRead( void )
{
	if( getIsPlaying() )
	{
		m_VidThread.abortThreadRun( true );	
		setIsPlaying( false );
		setIsPlayingPaused( true );
		stopVideoThread();
		closeAviFile();
		m_AssetId.clearVxGUID();
	}
}

//============================================================================
bool MJPEGReader::playOneFrame( const char * fileName, VxGUID& assetId )
{
	FILE * fileHandle = VxFileUtil::fileOpen( fileName, "rb" );
	if( 0 == fileHandle )
	{
		LogMsg( LOG_ERROR, "MJPEGReader::playOneFrame could not open file %s\n", fileName );
		return false;
	}

	bool result = readFirstVidFrameFromFile( fileHandle, fileName, assetId );
	fclose( fileHandle );
	return result;
}

//============================================================================
bool MJPEGReader::readFirstVidFrameFromFile( FILE * fileHandle, const char * fileName, VxGUID& assetId )
{
	// first see if already loaded.. if so read from chunk list
	lockResources();
	if( m_Initialized
		&& ( assetId == m_AssetId )
		&& m_AviChunkList.size() )
	{
		std::vector<AviChunkOffs>::iterator iter;
		for( iter = m_AviChunkList.begin(); iter != m_AviChunkList.end(); ++iter )
		{
			AviChunkOffs& listChunk = *iter;
			if( listChunk.isVideoChunk() )
			{
				bool isEmpty = listChunk.isDataEmpty();
				if( isEmpty )
				{
					if( false == readChunkFromDisk( fileHandle, &listChunk ) )
					{
						unlockResources();
						return false;
					}
				}

				IToGui::getToGui().toGuiPlayVideoFrame( m_AssetId, &listChunk.m_DataPtr[8], listChunk.m_ChunkLen, 0 );
				if( isEmpty )
				{
					listChunk.deleteDataPtr();
				}
			
				unlockResources();
				return true;
			}
		}
	}

	unlockResources();
	// not in chunk list.. read through header then read first frame
	readRiffHeader( fileHandle );
	uint32_t totalRead = 12;
	AviDataChunk aviChunk;
	AviAllStreamsHdr allStreamsHdr;
	while( true )
	{
		if( 8 != fread( &aviChunk, 1, 8, fileHandle ) )
		{
			LogMsg( LOG_ERROR, "MJPEGReader::readFirstVidFrameFromFile error %d reading file %s\n", VxGetLastError(), fileName );
			return false;
		}

		totalRead += 8;
		if( ( 0 == aviChunk.m_PayloadDataSize )
			|| ( aviChunk.m_PayloadDataSize % 1 ) )
		{
			LogMsg( LOG_ERROR, "MJPEGReader::readFirstVidFrameFromFile error invalid chunk size %d reading file %s\n", aviChunk.m_PayloadDataSize, fileName );
			return false;
		}

		if( ! aviChunk.m_Id.isIdEqual( "LIST" ) )
		{
			// seek past JUNK and IDIT etc. nothing we want from it
			totalRead += aviChunk.m_PayloadDataSize;
			if( 0 != VxFileUtil::fileSeek( fileHandle, totalRead ) )
			{
				LogMsg( LOG_ERROR, "MJPEGReader::readFirstVidFrameFromFile could not seek to %d in file %s\n", totalRead, fileName );
				return false;
			}

			continue;
		}

		uint32_t listTotalLen		= aviChunk.m_PayloadDataSize;
		uint32_t listBeginOffset		= totalRead;

		AviFourCC listTypeId;
		if( 4 != fread( &listTypeId, 1, 4, fileHandle ) )
		{
			LogMsg( LOG_ERROR, "MJPEGReader::readFirstVidFrameFromFile error %d reading file %s\n", VxGetLastError(), fileName );
			return false;
		}

		totalRead += 4;
		if( listTypeId.isIdEqual( "HDRL" ) )
		{
			// all streams header
			if( 8 != fread( &aviChunk, 1, 8, fileHandle ) )
			{
				LogMsg( LOG_ERROR, "MJPEGReader::readFirstVidFrameFromFile error %d reading file %s\n", VxGetLastError(), fileName );
				return false;
			}

			totalRead += 8;
			if( false == aviChunk.m_Id.isIdEqual( "AVIH" ) )
			{
				LogMsg( LOG_ERROR, "MJPEGReader::readFirstVidFrameFromFile error NOT A AVI HEADER file %s\n", fileName );
				return false;
			}

			if( sizeof( AviAllStreamsHdr ) != aviChunk.m_PayloadDataSize )
			{
				LogMsg( LOG_ERROR, "MJPEGReader::readFirstVidFrameFromFile error wrong hdr size %d file %s\n", aviChunk.m_PayloadDataSize, fileName );
				return false;
			}

			if( sizeof( AviAllStreamsHdr ) != fread( &allStreamsHdr, 1, sizeof( AviAllStreamsHdr ), fileHandle ) )
			{
				LogMsg( LOG_ERROR, "MJPEGReader::readFirstVidFrameFromFile error %d reading file %s\n", VxGetLastError(), fileName );
				return false;
			}

			totalRead += sizeof( AviAllStreamsHdr );
		}
		else if( listTypeId.isIdEqual( "STRL" ) )
		{
			// possibly contains auds or vids we are interested in
			if( 8 != fread( &aviChunk, 1, 8, fileHandle ) )
			{
				LogMsg( LOG_ERROR, "MJPEGReader::readFirstVidFrameFromFile error %d reading file %s\n", VxGetLastError(), fileName );
				return false;
			}

			AviFourCC audsOrVidsId;
			if( 4 != fread( &audsOrVidsId, 1, 4, fileHandle ) )
			{
				LogMsg( LOG_ERROR, "MJPEGReader::readFirstVidFrameFromFile error %d reading file %s\n", VxGetLastError(), fileName );
				return false;
			}

			totalRead += 12;
			if( ( false == aviChunk.m_Id.isIdEqual( "STRH" ) )
				|| ( ( false == audsOrVidsId.isIdEqual( "AUDS" ) ) && ( false == audsOrVidsId.isIdEqual( "VIDS" ) ) ) )
			{
				// not one we are interested in so seek past it
				totalRead = listBeginOffset + listTotalLen;
				if( 0 != VxFileUtil::fileSeek( fileHandle, totalRead ) )
				{
					LogMsg( LOG_ERROR, "MJPEGReader::readFirstVidFrameFromFile could not seek to %d in file %s\n", totalRead, fileName );
					return false;
				}

				continue;
			}

			if( audsOrVidsId.isIdEqual( "VIDS" ) )
			{
				AviStreamHdr vidStreamHdr;
				char * vidHdrPlus4 = (char *)&vidStreamHdr;
				vidHdrPlus4 += 4; // we already read the vids id of stream header so skip over it

				if( sizeof( AviStreamHdr ) - 4 != fread( vidHdrPlus4, 1, sizeof( AviStreamHdr ) - 4, fileHandle ) )
				{
					LogMsg( LOG_ERROR, "MJPEGReader::readFirstVidFrameFromFile error %d reading file %s\n", VxGetLastError(), fileName );
					return false;
				}

				if( false == vidStreamHdr.m_CodecId.isIdEqual( "MJPG" ) )
				{
					LogMsg( LOG_ERROR, "MJPEGReader::readFirstVidFrameFromFile unknown video codec file %s\n", fileName );
					return false;
				}

				// don't need actual video format so just fall through and seek to next list
			}
			else
			{
				AviStreamHdr audStreamHdr;
				char * audHdrPlus4 = (char *)&audStreamHdr;
				audHdrPlus4 += 4; // we already read the vids id of stream header so skip over it

				if( sizeof( AviStreamHdr ) - 4 != fread( audHdrPlus4, 1, sizeof( AviStreamHdr ) - 4, fileHandle ) )
				{
					LogMsg( LOG_ERROR, "MJPEGReader::readFirstVidFrameFromFile error %d reading file %s\n", VxGetLastError(), fileName );
					return false;
				}

				if( 8000 != audStreamHdr.m_Rate )
				{
					LogMsg( LOG_ERROR, "MJPEGReader::readFirstVidFrameFromFile unknown audio codec file %s\n", fileName );
					return false;
				}
			}

			// seek to next list
			totalRead = listBeginOffset + listTotalLen;
			if( 0 != VxFileUtil::fileSeek( fileHandle, totalRead ) )
			{
				LogMsg( LOG_ERROR, "MJPEGReader::readFirstVidFrameFromFile could not seek to %d in file %s\n", totalRead, fileName );
				return false;
			}
		}
		else if( listTypeId.isIdEqual( "MOVI" ) )
		{
			// we are at the beginning of movie streams.. read first video fame
			while( true )
			{
				if( 8 != fread( &aviChunk, 1, 8, fileHandle ) )
				{
					LogMsg( LOG_ERROR, "MJPEGReader::readFirstVidFrameFromFile error %d reading file %s\n", VxGetLastError(), fileName );
					return false;
				}
				
				if( aviChunk.m_PayloadDataSize > 10000000 )
				{
					LogMsg( LOG_ERROR, "MJPEGReader::readFirstVidFrameFromFile chunk size too large reading file %s\n", fileName );
					return false;
				}

				uint8_t * payloadData = new uint8_t[ aviChunk.m_PayloadDataSize ];
				if( aviChunk.m_PayloadDataSize != fread( payloadData, 1, aviChunk.m_PayloadDataSize, fileHandle ) )
				{
					LogMsg( LOG_ERROR, "MJPEGReader::readFirstVidFrameFromFile error %d reading file %s\n", VxGetLastError(), fileName );
					return false;
				}

				if( aviChunk.m_Id.isIdEqual( "00DC" ) )
				{
					VxGUID onlineId;
					onlineId.setVxGUID( assetId );
					IToGui::getToGui().toGuiPlayVideoFrame( onlineId, payloadData, aviChunk.m_PayloadDataSize, 0 );
					delete[] payloadData;
					return true;
				}
				else if(  aviChunk.m_Id.isIdEqual( "01WB" ) )
				{
					// audio chunk.. keep going
					delete[] payloadData;
				}
				else
				{
					// unknown type
					delete[] payloadData;
					LogMsg( LOG_ERROR, "MJPEGReader::readFirstVidFrameFromFile UNKNOWN CHUNK TYPE reading file %s\n", fileName );
					return false;
				}
			}
			
			return true; // all done
		}
		else
		{
			// unknown.. just seek past it
			totalRead += listTotalLen - 4;
			if( 0 != VxFileUtil::fileSeek( fileHandle, totalRead ) )
			{
				LogMsg( LOG_ERROR, "MJPEGReader::readFirstVidFrameFromFile could not seek to %d in file %s\n", totalRead, fileName );
				return false;
			}
		}
	}

	return false;
}

//============================================================================
void MJPEGReader::setIsPlayingPaused( bool pause )
{
	if( pause )
	{
		if( getIsPlaying() )
		{
			if( !m_IsFirstFrameAfterResumePlaying )
			{
				m_TotalElapsedMs += m_PlayElapseTimer.elapsedMs();
			}
		}
		
		m_IsPlayingPaused = true;
	}
	else
	{
		m_IsFirstFrameAfterResumePlaying = true;
		m_IsPlayingPaused = false;
	}
}

//============================================================================
bool MJPEGReader::readRiffHeader( FILE * fileHandle )
{
	AviRiffHeader riffHdr;
	if( sizeof( AviRiffHeader ) != fread( &riffHdr, 1, sizeof( AviRiffHeader ), fileHandle ) )
	{
		LogMsg( LOG_ERROR, "MJPEGReader::readRiffHeader error %d writing file %s\n", VxGetLastError(), m_FileName.c_str() );
		return false;
	}

	return true;
}

//============================================================================
bool MJPEGReader::readVideoHeader( void )
{
	AviDataChunk aviChunk;
	uint32_t totalRead = 12; // already read the riff header
	while( true )
	{
		if( 8 != fread( &aviChunk, 1, 8, m_FileHandle ) )
		{
			LogMsg( LOG_ERROR, "MJPEGReader::readVideoHeader error %d reading file %s\n", VxGetLastError(), m_FileName.c_str() );
			return false;
		}

		totalRead += 8;
		if( ( 0 == aviChunk.m_PayloadDataSize )
			|| ( aviChunk.m_PayloadDataSize % 1 ) )
		{
			LogMsg( LOG_ERROR, "MJPEGReader::readVideoHeader error invalid chunk size %d reading file %s\n", aviChunk.m_PayloadDataSize, m_FileName.c_str() );
			return false;
		}

		if( ! aviChunk.m_Id.isIdEqual( "LIST" ) )
		{
			// seek past JUNK and IDIT etc. nothing we want from it
			totalRead += aviChunk.m_PayloadDataSize;
			if( 0 != VxFileUtil::fileSeek( m_FileHandle, totalRead ) )
			{
				LogMsg( LOG_ERROR, "MJPEGReader::readVideoHeader could not seek to %d in file %s\n", totalRead, m_FileName.c_str() );
				return false;
			}

			continue;
		}

		uint32_t listTotalLen		= aviChunk.m_PayloadDataSize;
		uint32_t listBeginOffset		= totalRead;

		AviFourCC listTypeId;
		if( 4 != fread( &listTypeId, 1, 4, m_FileHandle ) )
		{
			LogMsg( LOG_ERROR, "MJPEGReader::readVideoHeader error %d reading file %s\n", VxGetLastError(), m_FileName.c_str() );
			return false;
		}

		totalRead += 4;
		if( listTypeId.isIdEqual( "HDRL" ) )
		{
			// all streams header
			if( 8 != fread( &aviChunk, 1, 8, m_FileHandle ) )
			{
				LogMsg( LOG_ERROR, "MJPEGReader::readVideoHeader error %d reading file %s\n", VxGetLastError(), m_FileName.c_str() );
				return false;
			}

			totalRead += 8;
			if( false == aviChunk.m_Id.isIdEqual( "AVIH" ) )
			{
				LogMsg( LOG_ERROR, "MJPEGReader::readVideoHeader error NOT A AVI HEADER file %s\n", m_FileName.c_str() );
				return false;
			}

			if( sizeof( AviAllStreamsHdr ) != aviChunk.m_PayloadDataSize )
			{
				LogMsg( LOG_ERROR, "MJPEGReader::readVideoHeader error wrong hdr size %d file %s\n", aviChunk.m_PayloadDataSize, m_FileName.c_str() );
				return false;
			}

			if( sizeof( AviAllStreamsHdr ) != fread( &m_AllStreamsHdr, 1, sizeof( AviAllStreamsHdr ), m_FileHandle ) )
			{
				LogMsg( LOG_ERROR, "MJPEGReader::readVideoHeader error %d reading file %s\n", VxGetLastError(), m_FileName.c_str() );
				return false;
			}

			totalRead += sizeof( AviAllStreamsHdr );
		}
		else if( listTypeId.isIdEqual( "STRL" ) )
		{
			// possibly contains auds or vids we are interested in
			if( 8 != fread( &aviChunk, 1, 8, m_FileHandle ) )
			{
				LogMsg( LOG_ERROR, "MJPEGReader::readVideoHeader error %d reading file %s\n", VxGetLastError(), m_FileName.c_str() );
				return false;
			}

			AviFourCC audsOrVidsId;
			if( 4 != fread( &audsOrVidsId, 1, 4, m_FileHandle ) )
			{
				LogMsg( LOG_ERROR, "MJPEGReader::readVideoHeader error %d reading file %s\n", VxGetLastError(), m_FileName.c_str() );
				return false;
			}

			totalRead += 12;
			if( ( false == aviChunk.m_Id.isIdEqual( "STRH" ) )
				|| ( ( false == audsOrVidsId.isIdEqual( "AUDS" ) ) && ( false == audsOrVidsId.isIdEqual( "VIDS" ) ) ) )
			{
				// not one we are interested in so seek past it
				totalRead = listBeginOffset + listTotalLen;
				if( 0 != VxFileUtil::fileSeek( m_FileHandle, totalRead ) )
				{
					LogMsg( LOG_ERROR, "MJPEGReader::readVideoHeader could not seek to %d in file %s\n", totalRead, m_FileName.c_str() );
					return false;
				}

				continue;
			}

			if( audsOrVidsId.isIdEqual( "VIDS" ) )
			{
				char * vidHdrPlus4 = (char *)&m_VidStreamHdr;
				vidHdrPlus4 += 4; // we already read the vids id of stream header so skip over it

				if( sizeof( AviStreamHdr ) - 4 != fread( vidHdrPlus4, 1, sizeof( AviStreamHdr ) - 4, m_FileHandle ) )
				{
					LogMsg( LOG_ERROR, "MJPEGReader::readVideoHeader error %d reading file %s\n", VxGetLastError(), m_FileName.c_str() );
					return false;
				}

				if( false == m_VidStreamHdr.m_CodecId.isIdEqual( "MJPG" ) )
				{
					LogMsg( LOG_ERROR, "MJPEGReader::readVideoHeader unknown video codec file %s\n", m_FileName.c_str() );
					return false;
				}

				// don't need actual video format so just fall through and seek to next list
			}
			else
			{
				AviStreamHdr audStreamHdr;
				char * audHdrPlus4 = (char *)&audStreamHdr;
				audHdrPlus4 += 4; // we already read the vids id of stream header so skip over it

				if( sizeof( AviStreamHdr ) - 4 != fread( audHdrPlus4, 1, sizeof( AviStreamHdr ) - 4, m_FileHandle ) )
				{
					LogMsg( LOG_ERROR, "MJPEGReader::readAudioHeader error %d reading file %s\n", VxGetLastError(), m_FileName.c_str() );
					return false;
				}

				if( 8000 != audStreamHdr.m_Rate )
				{
					LogMsg( LOG_ERROR, "MJPEGReader::readVideoHeader unknown audio codec file %s\n", m_FileName.c_str() );
					return false;
				}
			}
		
			// seek to next list
			totalRead = listBeginOffset + listTotalLen;
			if( 0 != VxFileUtil::fileSeek( m_FileHandle, totalRead ) )
			{
				LogMsg( LOG_ERROR, "MJPEGReader::readVideoHeader could not seek to %d in file %s\n", totalRead, m_FileName.c_str() );
				return false;
			}
		
			continue;
		}
		else if( listTypeId.isIdEqual( "MOVI" ) )
		{
			// we are at the beginning of movie streams
			m_StreamsBeginOffs = totalRead;
			// seek to end and read the chunk offsets
			m_StreamsEndOffs = m_StreamsBeginOffs + listTotalLen - 4;
			if( 0 != VxFileUtil::fileSeek( m_FileHandle, m_StreamsEndOffs ) )
			{
				LogMsg( LOG_ERROR, "MJPEGReader::readVideoHeader could not seek to %d in file %s\n", m_StreamsEndOffs, m_FileName.c_str() );
				return false;
			}

			if( false == parseChunkOffsets() )
			{
				return false;
			}

			if( 0 != VxFileUtil::fileSeek( m_FileHandle, m_StreamsBeginOffs ) )
			{
				LogMsg( LOG_ERROR, "MJPEGReader::readVideoHeader could not seek to %d in file %s\n", m_StreamsBeginOffs, m_FileName.c_str() );
				return false;
			}

			return true; // all done
		}
		else
		{
			// unknown.. just seek past it
			totalRead += listTotalLen - 4;
			if( 0 != VxFileUtil::fileSeek( m_FileHandle, totalRead ) )
			{
				LogMsg( LOG_ERROR, "MJPEGReader::readVideoHeader could not seek to %d in file %s\n", totalRead, m_FileName.c_str() );
				return false;
			}

			continue;
		}
	}

	return true;
}

//============================================================================
bool MJPEGReader::parseChunkOffsets( void )
{
	m_AviChunkList.clear();
	AviDataChunk aviChunk;
	if( 8 != fread( &aviChunk, 1, 8, m_FileHandle ) )
	{
		LogMsg( LOG_ERROR, "MJPEGReader::parseChunkOffsets error %d reading file %s\n", VxGetLastError(), m_FileName.c_str() );
		return false;
	}

	if( false == aviChunk.m_Id.isIdEqual( "IDX1" ) )
	{
		LogMsg( LOG_ERROR, "MJPEGReader::parseChunkOffsets invalid index id file %s\n", m_FileName.c_str() );
		return false;
	}

	uint32_t idxDataLen = aviChunk.m_PayloadDataSize;
	if( idxDataLen % sizeof( AviFrameIndex ) )
	{
		LogMsg( LOG_ERROR, "MJPEGReader::parseChunkOffsets invalid index size %d file %s\n", idxDataLen, m_FileName.c_str() );
		return false;
	}

	char buf[ sizeof( AviFrameIndex ) * 4096 ];
	AviFrameIndex * aviFrames;
	int audioChunkCnt = 0;
	while( idxDataLen )
	{
		size_t amtToRead = idxDataLen < sizeof( buf ) ? idxDataLen : sizeof( buf );
		if( amtToRead != fread( buf, 1, amtToRead, m_FileHandle ) )
		{
			LogMsg( LOG_ERROR, "MJPEGReader::parseChunkOffsets error %d reading file %s\n", VxGetLastError(), m_FileName.c_str() );
			return false;
		}

		aviFrames = (AviFrameIndex *)buf;
		int frameCnt = amtToRead / sizeof( AviFrameIndex );
		for( int i = 0; i < frameCnt; i++ )
		{
			if ( !aviFrames[i].m_00db.isIdEqual( "00DC" ) )
				audioChunkCnt++;
			m_AviChunkList.push_back( AviChunkOffs( aviFrames[i].m_00db.isIdEqual("00DC") ? eAviChunkTypeVideo : eAviChunkTypeAudio,
													aviFrames[i].m_DataOffset + m_StreamsBeginOffs - 4, // we want offset in file instead of offset from beginning of list
													aviFrames[i].m_DataLen ) );
		}

		idxDataLen -= amtToRead;
	}

	LogMsg( LOG_INFO, "MJPEGReader audio frame count %d\n", audioChunkCnt );
	return true;
}

//============================================================================
void MJPEGReader::closeAviFile( void )
{
	if( 0 != m_FileHandle )
	{
		fclose( m_FileHandle );
		m_FileHandle = 0;
	}
}

//============================================================================
void MJPEGReader::startVideoThread( void )
{
	if( ( false == m_IsStartThreadCommanded )
		&& ( false == m_IsVidThreadRunning ) )
	{
		m_IsStartThreadCommanded = true;
		m_VidThread.startThread( (VX_THREAD_FUNCTION_T)MJPEGReaderThreadFunc, this, "MJPEGReaderThread" );
	}
}

//============================================================================
void MJPEGReader::stopVideoThread( void )
{
	LogMsg( LOG_INFO, "MJPEGReader::stopVideoThread start\n" );
	int killThreadTryCnt = 0;
	while( m_IsStartThreadCommanded || m_IsVidThreadRunning )
	{
		m_VidThread.abortThreadRun( true );
		m_VidSemaphore.signal();
		VxSleep( 500 );
		killThreadTryCnt++;
		if( m_IsStartThreadCommanded || m_IsVidThreadRunning )
		{
			LogMsg( LOG_INFO, "MJPEGReader::stopVideoThread wait thread die cnt %d\n", killThreadTryCnt );
		}

		if( killThreadTryCnt > 5 )
		{
			LogMsg( LOG_ERROR, "MJPEGReader::stopVideoThread wait thread die TIMEOUT cnt %d\n", killThreadTryCnt );
			break;
		}
	}

	if( 0 != m_FileHandle )
	{
		fclose( m_FileHandle );
		m_FileHandle = 0;
	}

	cleanupReadChunks();
	LogMsg( LOG_INFO, "MJPEGReader::stopVideoThread end\n" );
}

//============================================================================
void MJPEGReader::cleanupReadChunks( void )
{
	std::vector<AviChunkOffs>::iterator iter;
	for( iter = m_AviChunkList.begin(); iter != m_AviChunkList.end(); ++iter )
	{
		(*iter).deleteDataPtr();
	}

	m_AviChunkList.clear();
}

//============================================================================
void MJPEGReader::readerThread( void )
{
	VxTimer	vidTimer;
	m_MilliSecPerFrame = 80.0;
	bool allChunksSent = false;
	if( 0 != m_ChunkReadIdx )
	{
		AviChunkOffs * chunk1Offs = &m_AviChunkList[m_ChunkReadIdx];
		VxFileUtil::fileSeek( m_FileHandle, chunk1Offs->m_ChunkOffs );
	}

	while( ( false == m_VidThread.isAborted() ) 
			&& ( false == allChunksSent ))
	{
		// keep at least 3 audio chunks and 3 video chunks in memory
		while( ( false == m_VidThread.isAborted() )
				&& m_IsPlaying
				&& ( ( 3 > m_VidChunksInMemory ) || ( 3 > m_AudioChunksInMemory ) )
				&& ( false == m_AllChunkRead ) )
		{
			if( m_VidThread.isAborted() )
				return;

			AviChunkOffs * chunk = &m_AviChunkList[m_ChunkReadIdx];
			//LogMsg( LOG_INFO, "readerThread::lockResources start\n" );
			lockResources();
			//LogMsg( LOG_INFO, "readerThread::lockResources end\n" );
			if( false == readChunkFromDisk( m_FileHandle, chunk ) )
			{
				LogMsg( LOG_INFO, "readerThread::unlockResources\n" );
				unlockResources();
				return;
			}
			
			if( m_VidThread.isAborted() )
			{
				LogMsg( LOG_INFO, "readerThread::unlockResources\n" );
				unlockResources();
				return;
			}

			if( chunk->isVideoChunk() )
			{
				m_VidChunksInMemory++;
			}
			else
			{
				m_AudioChunksInMemory++;
			}

			m_ChunkReadIdx++;
			if( m_ChunkReadIdx >= m_AviChunkList.size() )
			{
				m_AllChunkRead = true;
			}

			//LogMsg( LOG_INFO, "readerThread::unlockResources\n" );
			unlockResources();
		}
		
		// wait the correct time
		float	timeToWait = m_MilliSecPerFrame - (float)vidTimer.elapsedMs();
		if( timeToWait > 1.0f )
		{
#ifdef TARGET_OS_ANDROID			
			LogMsg( LOG_INFO, "MJPEGReader sleeping %d ms\n", ( int )timeToWait );
			VxSleep( (int)timeToWait/2 );
#else
			VxSleep( ( int )timeToWait );
#endif 			
		}

		if( m_VidThread.isAborted() )
			return;
		while(	m_IsPlayingPaused 
				&& ( false == m_VidThread.isAborted() ) )
		{
			VxSleep( 5 );
		}

		if( m_VidThread.isAborted() )
			return;

		vidTimer.startTimer();
		//LogMsg( LOG_INFO, "readerThread::lockResources start\n" );
		lockResources();
		//LogMsg( LOG_INFO, "readerThread::lockResources end\n" );
		AviChunkOffs * aviChunk = 0;
		while( m_VidWriteIdx < m_AviChunkList.size() )
		{
			aviChunk  = &m_AviChunkList[m_VidWriteIdx];
			if( false == aviChunk->isVideoChunk() )
			{
				calculateNewMilliSecPerFrame( m_VidWriteIdx );
				m_VidWriteIdx++;
				if( aviChunk->isDataEmpty() )
					continue;
				//LogMsg( LOG_INFO, "readerThread::unlockResources\n" );
				unlockResources();
				if( m_VidThread.isAborted() )
					return;

				if( m_AudioChunksInMemory > 8 )
				{
					//LogMsg( LOG_INFO, "readerThread::waitForAudio1 audio chunks %d vid chunks %d\n", m_AudioChunksInMemory, m_VidChunksInMemory );
					waitForAudio();					
				}
				
				// maximum time between frames 200 ms
				while( ( false == m_VidThread.isAborted() ) 
						&& ( m_AudioChunksInMemory > ( m_VidChunksInMemory * 2 ) ) )
				{
					//LogMsg( LOG_INFO, "readerThread::waitForAudio2 audio chunks %d vid chunks %d\n", m_AudioChunksInMemory, m_VidChunksInMemory );
					waitForAudio();					
				}
			
				if( m_VidThread.isAborted() )
					return;

				//LogMsg( LOG_INFO, "readerThread::lockResources start\n" );
				lockResources();
				continue;
			}
			else
			{
				//LogMsg( LOG_INFO, "MJPEGReader::readerThread end of chuck list %d ms per frame %d\n", m_AviChunkList.size(), m_MilliSecPerFrame );
				m_VidWriteIdx++;
				break;
			}
		}

		if( aviChunk 
			&& aviChunk->isVideoChunk()
			&& aviChunk->hasData() )
		{
			LogMsg( LOG_INFO, "MJPEGReader vid chunk %d start\n", m_VidWriteIdx );
			IToGui::getToGui().toGuiPlayVideoFrame( m_AssetId, &aviChunk->m_DataPtr[8], aviChunk->m_ChunkLen, 0 );
			LogMsg( LOG_INFO, "MJPEGReader vid chunk %d end\n", m_VidWriteIdx );
			m_VidChunksInMemory--;
			aviChunk->deleteDataPtr();
		}
		
		int progress = ( m_VidWriteIdx * 100000 ) / m_AviChunkList.size();
		//LogMsg( LOG_INFO, "readerThread::unlockResources\n" );
		unlockResources();
		LogMsg( LOG_INFO, "MJPEGReader progress %d start\n", progress );
		IToGui::getToGui().toGuiAssetAction(  eAssetActionPlayProgress, m_AssetId, progress );
		LogMsg( LOG_INFO, "MJPEGReader progress %d done\n", progress );
		if( ( 0 == m_VidChunksInMemory )
			&& ( m_VidWriteIdx >= m_AviChunkList.size() ) )
		{
			allChunksSent = true;
			break;
		}
	}

	if( m_VidThread.isAborted() )
		return;
	
	while ( ( false == m_VidThread.isAborted() )
	      && ( m_AudioChunksInMemory > 0 ) )
	{
		// wait for audio to finish
		VxSleep( 20 );
	}

	setIsPlaying( false );
	if( m_VidThread.isAborted() )
		return;

	//LogMsg( LOG_INFO, "MJPEGReader eAssetActionPlayEnd start\n" );
	IToGui::getToGui().toGuiAssetAction( eAssetActionPlayEnd, m_AssetId, 0 );
	//LogMsg( LOG_INFO, "MJPEGReader eAssetActionPlayEnd end\n" );
}


//============================================================================
void MJPEGReader::callbackAudioOutSpaceAvail( int freeSpaceLen )
{
	if( !getIsPlaying() || getIsPlayingPaused() )
	{
		return;
	}

	LogMsg( LOG_INFO, "MJPEGReader callbackAudioOutSpaceAvail start\n" );
	//LogMsg( LOG_INFO, "callbackAudioOutSpaceAvail::lockResources start\n" );
	lockResources();
	//LogMsg( LOG_INFO, "callbackAudioOutSpaceAvail::lockResources end\n" );
	AviChunkOffs * aviChunk = 0;
	bool foundAudioChunk = false;
	while( m_AudWriteIdx < m_AviChunkList.size() )
	{
		aviChunk  = &m_AviChunkList[m_AudWriteIdx];
		if( aviChunk->isVideoChunk() )
		{
			// vid chunk..
			m_AudWriteIdx++;
			continue;
		}

		if( aviChunk->isDataEmpty() )
		{
			m_VidSemaphore.signal();
			static int readIdx = 0;	
			if ( readIdx != m_AudWriteIdx )
			{
				LogMsg( LOG_INFO, "Waiting for MJPG audio read at idx %d in mem %d\n", m_AudWriteIdx, m_AudioChunksInMemory );					
			}

			//LogMsg( LOG_INFO, "callbackAudioOutSpaceAvail::unlockResources\n" );
			unlockResources();
			return;
		}
		
		m_AudWriteIdx++;
		foundAudioChunk = true;
		break;
	}

	if( foundAudioChunk )
	{
		//LogMsg( LOG_INFO, "MJPEGReader callbackAudioOutSpaceAvail start\n" );
		// in many android devices seem low volume. do a boost in volume
		m_Engine.getMediaProcesser().increasePcmSampleVolume( (int16_t *)(&aviChunk->m_DataPtr[8]), (uint16_t)aviChunk->m_ChunkLen, 20 );
		m_MediaProcessor.playAudio( (int16_t *)(&aviChunk->m_DataPtr[8]), aviChunk->m_ChunkLen );
		//LogMsg( LOG_INFO, "MJPEGReader callbackAudioOutSpaceAvail done\n" );
		m_AudioChunksInMemory--;
		aviChunk->deleteDataPtr();
	}

	//LogMsg( LOG_INFO, "callbackAudioOutSpaceAvail::unlockResources\n" );
	unlockResources();
	LogMsg( LOG_INFO, "MJPEGReader callbackAudioOutSpaceAvail done %3.3f\n", m_TempTimer.elapsedMs() );
	m_TempTimer.startTimer();
	m_VidSemaphore.signal();
}

//============================================================================
void MJPEGReader::calculateNewMilliSecPerFrame( unsigned int audChunkIdx )
{
	int vidFrameCnt = 0;
	audChunkIdx++;
	AviChunkOffs * aviChunk = 0;
	while( audChunkIdx < m_AviChunkList.size() )
	{
		aviChunk  = &m_AviChunkList[audChunkIdx];
		audChunkIdx++;
		if( false == aviChunk->isVideoChunk() )
		{
			break;
		}

		vidFrameCnt++;
	}

	if( 0 == vidFrameCnt )
	{
		m_MilliSecPerFrame = 20;
	}
	else
	{
		// we write 1280 bytes or 640 samples at 8000 hz audio
		// 1 sec / 8000 samples per sec = 0.0000125 per sample * 640 samples = 0.08 sec or 80 millisec
		m_MilliSecPerFrame = ( 80.0f / ( (float)vidFrameCnt + 1.0f ) );
	}
}

//============================================================================
bool MJPEGReader::readChunkFromDisk( FILE * fileHandle, AviChunkOffs * chunk )
{
	chunk->deleteDataPtr();
	chunk->m_DataPtr = new uint8_t[ chunk->m_ChunkLen + 8 ];
	if( chunk->m_ChunkLen + 8 != fread( chunk->m_DataPtr, 1, chunk->m_ChunkLen + 8, fileHandle ) )
	{
		LogMsg( LOG_ERROR, "MJPEGReader::readChunkFromDisk error %d reading file %s\n", VxGetLastError(), m_FileName.c_str() );
		chunk->deleteDataPtr();
		return false;
	}

	return true;
}

//============================================================================
void MJPEGReader::waitForAudio( void )
{
	//LogMsg( LOG_INFO, "MJPEGReader::waitForAudio start in mem %d\n", m_AudioChunksInMemory  );
	m_VidSemaphore.wait();
	//LogMsg( LOG_INFO, "MJPEGReader::waitForAudio done in mem %d\n", m_AudioChunksInMemory  );
}
