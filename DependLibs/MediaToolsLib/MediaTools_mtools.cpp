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

#include "MediaTools.h"
#include "OpusAudioEncoder.h"
#include "SndWriter.h"
#include "SndReader.h"
#include "MJPEGWriter.h"
#include "MJPEGReader.h"
#include <GuiInterface/IToGui.h>

#include <GoTvCore/GoTvP2P/AssetMgr/AssetInfo.h>

//============================================================================
MediaTools::MediaTools( P2PEngine& engine, MediaProcessor& mediaProcessor )
: m_Engine( engine )
, m_MediaProcessor( mediaProcessor )
, m_AudioEncoder(  * ( new OpusAudioEncoder( 8000, 1 ) ) )
, m_AudioWriter(  * ( new SndWriter( engine, mediaProcessor ) ) )
, m_AudioReader(  * ( new SndReader( engine, mediaProcessor ) ) )
, m_VideoWriter(  * ( new MJPEGWriter( engine, mediaProcessor ) ) )
, m_VideoReader(  * ( new MJPEGReader( engine, mediaProcessor ) ) )
{
}

//============================================================================
MediaTools::~MediaTools()
{
	delete &m_AudioEncoder;
	delete &m_AudioWriter;
	delete &m_VideoWriter;
}

//============================================================================
IToGui&	MediaTools::getToGui()
{
    return IToGui::getToGui();
}

//============================================================================
bool MediaTools::fromGuiIsMyP2PWebVideoFile( const char * fileName )
{
	return m_VideoReader.fromGuiIsMyP2PWebVideoFile( fileName );
}

//============================================================================
bool MediaTools::fromGuiIsMyP2PWebAudioFile( const char * fileName )
{
	return m_AudioReader.fromGuiIsMyP2PWebAudioFile( fileName );
}

//============================================================================
bool MediaTools::fromGuiSndRecord( ESndRecordState eRecState, VxGUID& feedId, const char * fileName )
{
	return m_AudioWriter.fromGuiSndRecord( eRecState, feedId, fileName );
}

//============================================================================
bool MediaTools::fromGuiVideoRecord( EVideoRecordState eRecState, VxGUID& feedId, const char * fileName  )
{
	return m_VideoWriter.fromGuiVideoRecord( eRecState, feedId, fileName );
}

//============================================================================
bool MediaTools::fromGuiAssetAction( AssetInfo& assetInfo, EAssetAction assetAction, int pos0to100000 )
{
	bool result = false;
	switch( assetAction )
	{
	case eAssetActionRecordBegin:
	case eAssetActionRecordPause:
	case eAssetActionRecordResume:
	case eAssetActionRecordEnd:
	case eAssetActionRecordCancel:
		if( eAssetTypeAudio == assetInfo.getAssetType() )
		{
			result = m_AudioWriter.fromGuiAssetAction( assetInfo,  assetAction, pos0to100000 );
		}
		break;
	case eAssetActionPlayBegin:
	case eAssetActionPlayOneFrame:
	case eAssetActionPlayPause:
	case eAssetActionPlayResume:
	case eAssetActionPlayProgress:
	case eAssetActionPlayEnd:
	case eAssetActionPlayCancel:
		if( eAssetTypeAudio == assetInfo.getAssetType() )
		{
			result = m_AudioReader.fromGuiAssetAction( assetInfo, assetAction, pos0to100000 );
		}
		else if( eAssetTypeVideo == assetInfo.getAssetType() )
		{
			result = m_VideoReader.fromGuiAssetAction( assetInfo, assetAction, pos0to100000 );
		}
		break;
	default:
		break;
	}

	return result;
}
