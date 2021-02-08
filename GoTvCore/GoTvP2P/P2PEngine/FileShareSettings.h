#pragma once
//============================================================================
// Copyright (C) 2012 Brett R. Jones
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

#include <CoreLib/VxDefs.h>

#include <string>
#include <vector>

class EngineSettings;

class FileShareSettings
{
public:
	FileShareSettings();

	FileShareSettings& operator=(const FileShareSettings& rhs);

	void						loadSettings( EngineSettings& engineSettings );
	void						saveSettings( EngineSettings& engineSettings );

	void						setDownloadBandwidth( uint32_t bandwidth )			{ m_u32DownloadBandwidth = bandwidth; }
	uint32_t					getDownloadBandwidth( void )					    { return m_u32DownloadBandwidth; }
	void						setUploadBandwidth( uint32_t bandwidth )			{ m_u32UploadBandwidth = bandwidth; }
	uint32_t					getUploadBandwidth( void )						    { return m_u32UploadBandwidth; }
	void						setDownloadMaxFiles( uint32_t fileCnt )				{ m_u32MaxDownloadingFiles = fileCnt; }
	uint32_t					getDownloadMaxFiles( void )						    { return m_u32MaxDownloadingFiles; }
	void						setUploadMaxFiles( uint32_t fileCnt )				{ m_u32MaxUploadingFiles = fileCnt; }
	uint32_t					getUploadMaxFiles( void )						    { return m_u32MaxUploadingFiles; }
	void						setStartServerOnStartup( bool startEnable )		    { m_bStartServerOnStartup = startEnable; }
	bool						getStartServerOnStartup( void )					    { return m_bStartServerOnStartup; }

public:
	//=== vars ===//
	uint32_t					m_u32DownloadBandwidth;
	uint32_t					m_u32MaxDownloadingFiles;

	uint32_t					m_u32UploadBandwidth;
	uint32_t					m_u32MaxUploadingFiles;
	bool						m_bStartServerOnStartup;

};
