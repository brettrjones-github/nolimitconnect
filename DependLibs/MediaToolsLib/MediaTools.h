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

#include <CoreLib/MediaCallbackInterface.h>
#include <GuiInterface/IFromGui.h>

#include <string>

class P2PEngine;
class MediaProcessor;
class IToGui;
class OpusAudioEncoder;
class SndWriter;
class SndReader;
class MJPEGWriter;
class MJPEGReader;

class MediaTools : public MediaCallbackInterface
{
public:
	MediaTools( P2PEngine& engine, MediaProcessor& mediaProcessor );
	virtual ~MediaTools();

	OpusAudioEncoder&			getAudioEncoder( void )					{ return m_AudioEncoder; }
	SndWriter&					getAudioWriter( void )					{ return m_AudioWriter; }
	SndReader&					getAudioReader( void )					{ return m_AudioReader; }
	MJPEGWriter&				getVideoWriter( void )					{ return m_VideoWriter; }
	MJPEGReader&				getVideoReader( void )					{ return m_VideoReader; }

	bool						fromGuiIsMyP2PWebVideoFile( const char* fileName );	
	bool						fromGuiIsMyP2PWebAudioFile( const char* fileName );	
	bool						fromGuiSndRecord( ESndRecordState eRecState, VxGUID& feedId, const char* fileName );
	bool						fromGuiVideoRecord( EVideoRecordState eRecState, VxGUID& feedId, const char* fileName );
	bool						fromGuiAssetAction( AssetBaseInfo& assetInfo, EAssetAction assetAction, int pos0to100000 );

protected:

	//=== vars ===//
	P2PEngine&					m_Engine;
	MediaProcessor&				m_MediaProcessor;
	IToGui&						getToGui();
	OpusAudioEncoder&			m_AudioEncoder;
	SndWriter&					m_AudioWriter;
	SndReader&					m_AudioReader;
	MJPEGWriter&				m_VideoWriter;
	MJPEGReader&				m_VideoReader;
};
