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
// http://www.nolimitconnect.org
//============================================================================

#include "FileShareSettings.h"
#include "EngineSettings.h"
#include "EngineSettingsDefaultValues.h"

#include <CoreLib/VxFileUtil.h>
#include <CoreLib/VxGlobals.h>

namespace
{
	FileShareSettings g_oFileShareSettings; 
}

//============================================================================
FileShareSettings::FileShareSettings()
	: m_u32DownloadBandwidth(0)
	, m_u32MaxDownloadingFiles(10)
	, m_u32UploadBandwidth(0)
	, m_u32MaxUploadingFiles(5)
	, m_bStartServerOnStartup(true)
{ 
}

//============================================================================
FileShareSettings& FileShareSettings::operator=(const FileShareSettings& rhs) 
{	
	if( this != &rhs )
	{
		m_u32DownloadBandwidth		= rhs.m_u32DownloadBandwidth;
		m_u32MaxDownloadingFiles	= rhs.m_u32MaxDownloadingFiles;
		m_u32UploadBandwidth		= rhs.m_u32UploadBandwidth;
		m_u32UploadBandwidth		= rhs.m_u32UploadBandwidth;
		m_bStartServerOnStartup		= rhs.m_bStartServerOnStartup;
	}

	return *this;
}

//============================================================================
void FileShareSettings::loadSettings( EngineSettings& engineSettings )
{
	LogMsg( LOG_INFO, "FileShareSettings::loadSettings\n" );
	engineSettings.getDnldsBandwidth( m_u32DownloadBandwidth );
	engineSettings.getMaxDownloadingFiles( m_u32MaxDownloadingFiles );

	engineSettings.getUpldsBandwidth( m_u32UploadBandwidth );
	engineSettings.getMaxUploadingFiles( m_u32MaxUploadingFiles );


	if( 0 == m_u32MaxDownloadingFiles )
	{
		m_u32MaxDownloadingFiles = DEFAULT_MAX_DOWNLOADING_FILES;
		engineSettings.setMaxDownloadingFiles( m_u32MaxDownloadingFiles );
	}

	if( 0 == m_u32MaxUploadingFiles )
	{
		m_u32MaxUploadingFiles = DEFAULT_MAX_UPLOADING_FILES;
		engineSettings.setMaxUploadingFiles( m_u32MaxUploadingFiles );
	}
}

//============================================================================
void FileShareSettings::saveSettings( EngineSettings& engineSettings )
{
	engineSettings.setDnldsBandwidth( m_u32DownloadBandwidth );
	engineSettings.setMaxDownloadingFiles( m_u32MaxDownloadingFiles );

	engineSettings.setUpldsBandwidth( m_u32UploadBandwidth );
	engineSettings.setMaxUploadingFiles( m_u32MaxUploadingFiles );
}
