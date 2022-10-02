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

#include "SndDefs.h"

#include <GuiInterface/IFromGui.h>
#include <PktLib/VxCommon.h>

#include <CoreLib/VxTimer.h>
#include <CoreLib/VxMutex.h>
#include <CoreLib/MediaCallbackInterface.h>

#include <string>
#include <stdio.h>
#include <memory.h>

class P2PEngine;
class MediaProcessor;
class IToGui;
class AssetInfo;
class OpusAudioEncoder;
class OpusFileEncoder;

class SndWriter : public MediaCallbackInterface
{
public:
	SndWriter( P2PEngine& engine, MediaProcessor& mediaProcessor );
	virtual ~SndWriter();

	void						setIsRecording( bool isRecording )				{ m_IsRecording = isRecording; }
	bool						getIsRecording( void )							{ return m_IsRecording; }
	void						setIsRecordingPaused( bool isPaused );
	bool						getIsRecordingPaused( void )					{ return m_IsRecordingPaused; }


	bool						fromGuiSndRecord( ESndRecordState eRecState, VxGUID& feedId, const char* fileName  );
	bool						fromGuiAssetAction( AssetBaseInfo& assetInfo, EAssetAction assetAction, int pos0to100000 );
	virtual void				callbackOpusEncoded( void * userData, uint8_t * encodedAudio, int32_t frame1Len, int32_t	frame2Len );

	bool						startSndWrite( const char* fileName, bool beginInPausedState );
	void						stopSndWrite( void );

protected:
	bool						setSndParameters(	uint32_t imageWidth, 
													uint32_t imageHeight, 
													uint32_t microSecBetweenFrames, 
													uint32_t	frameCnt, 
													uint32_t totalJpgDataLen );
	uint64_t					calculateRiffSize( uint32_t frameCnt, uint32_t totalJpgDataLen );
	bool						writeRiffHeader( void );
	bool						writeVideoHeader( void );
	void						closeSndFile( void );


	//=== vars ===//
	P2PEngine&					m_Engine; 
	MediaProcessor&				m_MediaProcessor;

	EPluginType					m_EPluginType;
	bool						m_IsRecording;
	bool						m_IsRecordingPaused;
	std::string					m_FileName;
	FILE *						m_FileHandle;
	int							m_MicroSecBetweenFrames;
	VxTimer						m_RecordElapseTimer;
	double						m_TotalElapsedMs;
	bool						m_IsFirstFrameAfterResumeRecording;

	std::vector<uint32_t>		m_FrameOffsetList;
	VxMutex						m_RecMutex;
	OpusAudioEncoder&			m_OpusEncoder;
	OpusFileEncoder&			m_OpusFileEncoder;
};
