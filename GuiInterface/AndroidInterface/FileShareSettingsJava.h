#pragma once
//============================================================================
// Copyright (C) 2010 Brett R. Jones
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

// android specific code
#ifdef TARGET_OS_ANDROID

#include "JavaBaseClass.h"

class FileShareSettingsJavaClass : public JavaBaseClass
{
public:
	FileShareSettingsJavaClass( JavaEnvCache& envCache, jclass javaRefClass );
	virtual ~FileShareSettingsJavaClass( );

	//=== vars ===//
	//=== java class ids ===//
	jfieldID					m_u32DownloadBandwidthID;
	jfieldID					m_u32MaxDownloadingFilesID;

	jfieldID					m_u32UploadBandwidthID;
	jfieldID					m_u32MaxUploadingFilesID;
	jfieldID					m_bStartServerOnStartupID;
};

FileShareSettingsJavaClass * GetFileShareSettingsJavaClass( void );
void FileShareSettingsJavaClassThreadExit( unsigned int uiThreadId );
void FileShareSettingsJavaClassShutdown( void );

bool Java2FileShareSettings( jobject& oFileShareSettingsClass, FileShareSettings& netSettings );
bool FileShareSettings2Java( FileShareSettings& netSettings, jobject& oFileShareSettingsClass );
bool FillFileShareSettingsJavaClass( FileShareSettings& netSettings, jobject& oFileShareSettingsClass );


#endif //TARGET_OS_ANDROID
