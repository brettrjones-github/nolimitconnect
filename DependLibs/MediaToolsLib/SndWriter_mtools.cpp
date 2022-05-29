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

#include "SndWriter.h"
#include "OpusAudioEncoder.h"
#include "OpusFileEncoder.h"

#include <ptop_src/ptop_engine_src/P2PEngine/P2PEngine.h>
#include <ptop_src/ptop_engine_src/AssetMgr/AssetInfo.h>
#include <ptop_src/ptop_engine_src/MediaProcessor/MediaProcessor.h>

#include <PktLib/PktVoiceReq.h>
#include <PktLib/PktVoiceReply.h>
#include <PktLib/PktChatReq.h>

#include <CoreLib/VxFileUtil.h>
#include <CoreLib/VxFileShredder.h>
#include <CoreLib/VxMacros.h>


//============================================================================
SndWriter::SndWriter( P2PEngine& engine, MediaProcessor& mediaProcessor )
: m_Engine( engine )
, m_MediaProcessor( mediaProcessor )
, m_EPluginType( ePluginTypeSndWriter )
, m_IsRecording( false )
, m_IsRecordingPaused( false )
, m_FileName( "" )
, m_FileHandle( 0 )
, m_MicroSecBetweenFrames( 0 )
, m_TotalElapsedMs( 0 )
, m_IsFirstFrameAfterResumeRecording( 0 )
, m_OpusEncoder( * ( new OpusAudioEncoder( 8000, 1 ) ) )
, m_OpusFileEncoder( * ( new OpusFileEncoder() ) )
{
}

//============================================================================
SndWriter::~SndWriter()
{
	delete &m_OpusEncoder;
}

//============================================================================
bool SndWriter::fromGuiAssetAction( AssetBaseInfo& assetInfo, EAssetAction assetAction, int pos0to100000 )
{
	bool result = false;
	switch( assetAction )
	{
	case eAssetActionRecordBegin:
		return fromGuiSndRecord( eSndRecordStateStartRecording, assetInfo.getHistoryId(), assetInfo.getAssetName().c_str() );

	case eAssetActionRecordEnd:
		return fromGuiSndRecord( eSndRecordStateStopRecording, assetInfo.getHistoryId(), assetInfo.getAssetName().c_str() );

	case eAssetActionRecordCancel:
		fromGuiSndRecord( eSndRecordStateStopRecording, assetInfo.getHistoryId(), assetInfo.getAssetName().c_str() );
		GetVxFileShredder().shredFile( assetInfo.getAssetName() );
		break;

	default:
		break;
	}

	return result;

}

//============================================================================
bool SndWriter::fromGuiSndRecord( ESndRecordState eRecState, VxGUID& feedId, const char * fileName )
{
	bool result = false;
	switch( eRecState )
	{
	case eSndRecordStateStopRecording:
		if( getIsRecording() )
		{
			stopSndWrite();
			result = true;
		}
		break;

	case eSndRecordStateStartRecording:
		if( !getIsRecording() )
		{
			result = startSndWrite( fileName, false );
		}

		break;

	case eSndRecordStateStartRecordingInPausedState:
		if( !getIsRecording() )
		{
			result = startSndWrite( fileName, true );
		}

		break;

	case eSndRecordStatePauseRecording:
		setIsRecordingPaused( true );
		break;

	case eSndRecordStateResumeRecording:
		setIsRecordingPaused( false );
		break;

	case eSndRecordStateCancelRecording:
		result = true;
		if( getIsRecording() )
		{
			stopSndWrite();
			std::string strFileName( fileName );
			GetVxFileShredder().shredFile( strFileName );
		}

		break;

	case eSndRecordStateDisabled:
	case eSndRecordStateError:
	default:
		break;
	}

	return result;
}

//============================================================================
bool SndWriter::startSndWrite( const char * fileName, bool beginInPausedState )
{
	stopSndWrite();
#ifdef MAKE_PCM_INSTEAD_OF_OPUS
	bool result = true;
	m_FileHandle = fopen( fileName, "wb+" );
	if( 0 == m_FileHandle )
	{
		LogMsg( LOG_ERROR, "startSndWrite:: pcm Write could not open file to write %s\n", fileName );
		result = false;
	}
#else
	m_FileName = fileName;
	bool result = m_OpusFileEncoder.beginFileEncode( fileName );
#endif //MAKE_PCM_INSTEAD_OF_OPUS
	if( result )
	{
		setIsRecordingPaused( beginInPausedState );
		setIsRecording( true );
		m_Engine.getMediaProcesser().wantMediaInput( eMediaInputAudioOpus, this,  (void *)ePluginTypeSndWriter, true );
	}

	return result;
}

//============================================================================
void SndWriter::setIsRecordingPaused( bool pause )
{
	if( pause )
	{
		if( getIsRecording() )
		{
			if( !m_IsFirstFrameAfterResumeRecording )
			{
				m_TotalElapsedMs += m_RecordElapseTimer.elapsedMs();
			}
		}
		
		m_IsRecordingPaused = true;
	}
	else
	{
		m_IsFirstFrameAfterResumeRecording = true;
		m_IsRecordingPaused = false;
	}
}

//============================================================================
void SndWriter::stopSndWrite( void )
{
	if( getIsRecording() )
	{
		setIsRecordingPaused( true );
		m_Engine.getMediaProcesser().wantMediaInput( eMediaInputAudioOpus, this,  (void *)ePluginTypeSndWriter, false );

#ifdef MAKE_PCM_INSTEAD_OF_OPUS
		if( m_FileHandle )
		{
			fclose( m_FileHandle );
			m_FileHandle = 0;
		}
#else
		m_OpusFileEncoder.finishFileEncode();
#endif // MAKE_PCM_INSTEAD_OF_OPUS		

		setIsRecording( false );
	}
}

//============================================================================
void SndWriter::closeSndFile( void )
{
	if( 0 != m_FileHandle )
	{
		fclose( m_FileHandle );
		m_FileHandle = 0;
	}
}

//============================================================================
void SndWriter::callbackOpusEncoded( void * /*userData*/, uint8_t * encodedAudio, int32_t frame1Len, int32_t	frame2Len )
{
	if( getIsRecording() )
	{
#ifdef MAKE_PCM_INSTEAD_OF_OPUS
		if( m_FileHandle )
		{
			fwrite( pu16PcmData, 1, u16PcmDataLen, m_FileHandle );
		}
#else
		m_OpusFileEncoder.writeEncodedFrame( encodedAudio, frame1Len );
		m_OpusFileEncoder.writeEncodedFrame( &encodedAudio[frame1Len], frame2Len );
#endif // MAKE_PCM_INSTEAD_OF_OPUS
	}
}
