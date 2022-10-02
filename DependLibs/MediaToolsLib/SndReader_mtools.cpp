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

#include "SndReader.h"
#include "OpusAudioDecoder.h"
#include "OpusFileDecoder.h"

#include <ptop_src/ptop_engine_src/P2PEngine/P2PEngine.h>
#include <ptop_src/ptop_engine_src/AssetMgr/AssetInfo.h>
#include <ptop_src/ptop_engine_src/MediaProcessor/MediaProcessor.h>

#include <CoreLib/VxFileUtil.h>
#include <CoreLib/VxFileShredder.h>
#include <CoreLib/VxMacros.h>

namespace
{
	uint32_t			OPUS_MYP2PWEB_SIGNITURE_OFFSET		= 0x9c;
}

//============================================================================
SndReader::SndReader( P2PEngine& engine, MediaProcessor& mediaProcessor )
: m_Engine( engine )
, m_MediaProcessor( mediaProcessor )
, m_EPluginType( ePluginTypeSndReader )
, m_IsPlaying( false )
, m_IsPlayingPaused( false )
, m_FileName( "" )
, m_FileHandle( 0 )
//, m_MicroSecBetweenFrames( 0 )
//, m_TotalJpgDataLen( 0 )
//, m_TotalFrameCnt( 0 )
//, m_TotalElapsedMs( 0 )
, m_IsFirstFrameAfterResumePlaying( 0 )
, m_OpusDecoder( * ( new OpusAudioDecoder( 8000, 1 ) ) )
, m_OpusFileDecoder( * ( new OpusFileDecoder( engine, mediaProcessor ) ) )
{
}

//============================================================================
SndReader::~SndReader()
{
	delete &m_OpusDecoder;
}

//============================================================================
bool SndReader::fromGuiIsMyP2PWebAudioFile( const char* fileName )
{
	uint64_t fileLen = VxFileUtil::getFileLen( fileName );
	if( fileLen < 1000 )
	{
		return false;
	}


	FILE * fileHandle = VxFileUtil::fileOpen( fileName, "rb" );
	if( 0 == fileHandle )
	{
		LogMsg( LOG_ERROR, "SndReader::fromGuiIsMyP2PWebAudioFile could not open file %s\n", fileName );
		return false;
	}

	if( 0 != VxFileUtil::fileSeek( fileHandle,  OPUS_MYP2PWEB_SIGNITURE_OFFSET ) )
	{
		fclose( fileHandle );
		LogMsg( LOG_ERROR, "SndReader::fromGuiIsMyP2PWebAudioFile could not seek file %s\n", fileName );
		return false;
	}

	// at 0x9c ( should be signature myp2pweb.com v0000000000000000-XXv where the zeros are hex ascii of total snd frames and XX is version number

	char junkBuf[16];
	if( 16 != fread( junkBuf, 1, 16, fileHandle ) )
	{
		fclose( fileHandle );
		LogMsg( LOG_ERROR, "SndReader::fromGuiIsMyP2PWebAudioFile could not read file %s\n", fileName );
		return false;
	}

	fclose( fileHandle );
	if( 0 != strncmp( junkBuf, "myp2pweb.com", 12 ) )
	{
		LogMsg( LOG_INFO, "SndReader::fromGuiIsMyP2PWebAudioFile is not My P2P Audio File %s\n", fileName );
		return false;
	}

	return true;
}

//============================================================================
bool SndReader::fromGuiAssetAction( AssetBaseInfo& assetInfo, EAssetAction assetAction, int pos0to100000 )
{
	bool result = false;
	switch( assetAction )
	{
	case eAssetActionPlayBegin:
		if( m_AssetId.isVxGUIDValid() )
		{
			fromGuiSndPlay( eSndPlayStateStopPlaying, m_AssetId, 0 );
		}

		m_FileName	= assetInfo.getAssetName();
		m_AssetId	= assetInfo.getAssetUniqueId();
		return fromGuiSndPlay( eSndPlayStateStartPlaying, assetInfo.getAssetUniqueId(), pos0to100000 );

	case eAssetActionPlayEnd:
		if( m_AssetId == assetInfo.getAssetUniqueId() )
		{
			return fromGuiSndPlay( eSndPlayStateStopPlaying, assetInfo.getAssetUniqueId(), pos0to100000 );
		}

		break;
	case eAssetActionPlayCancel:
		if( m_AssetId == assetInfo.getAssetUniqueId() )
		{
			fromGuiSndPlay( eSndPlayStateStopPlaying, assetInfo.getAssetUniqueId(), pos0to100000 );
		}

		break;

	default:
		break;
	}

	return result;

}

//============================================================================
bool SndReader::fromGuiSndPlay( ESndPlayState eRecState, VxGUID& assetId, int pos0to100000 )
{
	bool result = false;
	switch( eRecState )
	{
	case eSndPlayStateStopPlaying:
		if( getIsPlaying() )
		{
			stopSndRead();
			result = true;
		}

		break;

	case eSndPlayStateStartPlaying:
		if( getIsPlaying() )
		{
			stopSndRead();
		}
			
		result = startSndRead( m_FileName.c_str(), assetId, pos0to100000 );
		break;

	case eSndPlayStateStartPlayInSeekPos:
		if( getIsPlaying() )
		{
			stopSndRead();
		}

		result = startSndRead( m_FileName.c_str(), assetId, pos0to100000 );
		break;

	case eSndPlayStatePausePlaying:
		setIsPlayingPaused( true );
		break;

	case eSndPlayStateResumePlaying:
		setIsPlayingPaused( false );
		break;

	case eSndPlayStateCancelPlaying:
		result = true;
		if( getIsPlaying() )
		{
			stopSndRead();
		}

		break;

	case eSndPlayStateDisabled:
	case eSndPlayStateError:
	default:
		break;
	}

	return result;
}

//============================================================================
bool SndReader::startSndRead( const char* fileName, VxGUID& assetId, int pos0to100000 )
{
	stopSndRead();
	bool result = m_OpusFileDecoder.beginFileDecode( fileName, assetId, pos0to100000 );
	if( result )
	{
		setIsPlayingPaused( false );
		setIsPlaying( true );
	}

	return result;
}

//============================================================================
void SndReader::setIsPlayingPaused( bool pause )
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
void SndReader::stopSndRead( void )
{
	if( getIsPlaying() )
	{
		setIsPlayingPaused( true );
		m_OpusFileDecoder.finishFileDecode( true );
		setIsPlaying( false );
	}
}

//============================================================================
void SndReader::closeSndFile( void )
{
	if( 0 != m_FileHandle )
	{
		fclose( m_FileHandle );
		m_FileHandle = 0;
	}
}
