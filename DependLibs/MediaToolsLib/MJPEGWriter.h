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
#include <CoreLib/MediaCallbackInterface.h>

#include <string>
#include <stdio.h>
#include <memory.h>

class P2PEngine;
class MediaProcessor;
class IToGui;

class MJPEGWriter : public MediaCallbackInterface
{
public:
	MJPEGWriter( P2PEngine& engine, MediaProcessor& mediaProcessor );
	virtual ~MJPEGWriter();

	void						setIsRecording( bool isRecording )				{ m_IsRecording = isRecording; }
	bool						getIsRecording( void )							{ return m_IsRecording; }
	void						setIsRecordingPaused( bool isPaused );
	bool						getIsRecordingPaused( void )					{ return m_IsRecordingPaused; }


	bool						fromGuiVideoRecord( EVideoRecordState eRecState, VxGUID& feedId, const char* fileName  );
	virtual void				callbackVideoJpgSmall( void * userData, VxGUID& feedId, uint8_t * jpgData, uint32_t jpgDataLen, int motion0to100000 );
	virtual void				callbackPcm( void * userData, VxGUID& feedId, int16_t * pcmData, uint16_t pcmDataLen );

	bool						startAviWrite( const char* fileName, uint32_t MicroSecBetweenFrames, bool beginInPausedState );
	void						stopAviWrite( bool deleteFile = false );

protected:
	bool						setAviParameters(	uint32_t imageWidth, 
													uint32_t imageHeight, 
													uint32_t microSecBetweenFrames, 
													uint32_t	frameCnt, 
													uint32_t totalJpgDataLen );
	bool						writeRiffHeader( void );
	bool						writeVideoHeader( void );
	void						closeAviFile( void );


	//=== vars ===//
	P2PEngine&					m_Engine; 
	MediaProcessor&				m_MediaProcessor;
	
	VxGUID						m_FeedId;

	EPluginType					m_EPluginType;
	bool						m_IsRecording;
	bool						m_IsRecordingPaused;
	std::string					m_FileName;
	FILE *						m_FileHandle;
	int							m_MicroSecBetweenFrames;
	uint32_t							m_TotalJpgDataLen;
	uint32_t							m_TotalFrameCnt;
	VxTimer						m_RecordElapseTimer;
	double						m_TotalElapsedMs;
	bool						m_IsFirstFrameAfterResumeRecording;

	AviRiffHeader				m_RiffHdr;
	AviVideoHdr					m_AviHdr;
	std::vector<uint32_t>			m_FrameOffsetList;
	uint32_t							m_PrevFrameJpgLen;
	VxMutex						m_RecMutex;
	int							m_ImageWidth;
	int							m_ImageHeight;
	AviJpgHdr					m_AviJpgHdr;
	std::vector<uint32_t>			m_PcmOffsetList;
	AviAudioHdr					m_AviAudioHdr;
	VxMutex						m_AviFileAccessMutex;
};
