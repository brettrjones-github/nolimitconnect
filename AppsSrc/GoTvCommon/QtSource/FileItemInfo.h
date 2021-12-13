#pragma once
//============================================================================
// Copyright (C) 2013 Brett R. Jones
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

#include "VxMyFileInfo.h"

#include <QWidget>

#define FILE_STATUS_DOWNLOADING			0x0001
#define FILE_STATUS_UPLOADING			0x0002

class VxNetIdent;

class FileItemInfo
{
public:
    FileItemInfo( uint8_t u8FileType, const char * pFileName, uint64_t u64FileLen, VxGUID assetId, VxNetIdent * netIdent = 0, uint8_t * fileHashId = 0 );
	FileItemInfo( VxMyFileInfo& fileInfo, VxNetIdent * netIdent = 0, bool isShared = false, bool isInLibrary = false );

	void						setIsShared( bool isShared )			{ m_IsShared = isShared; }
	bool						getIsShared( void )						{ return m_IsShared; }

	void						setIsInLibrary( bool isInLibrary )		{ m_IsInLibrary = isInLibrary; }
	bool						getIsInLibrary( void )					{ return m_IsInLibrary; }

	QString&					getFullFileName( void )					{ return m_FileInfo.getFullFileName(); }
	QString						getJustFileName( void )					{ return m_FileInfo.getJustFileName(); }

    uint8_t						getFileType( void )						{ return m_FileInfo.getFileType(); }
    uint64_t					getFileLength( void )					{ return m_FileInfo.getFileLength(); }
	VxSha1Hash&					getFileHashId( void )					{ return m_FileInfo.getFileHashId(); }

	void 						setAssetId( VxGUID& assetId )			{ m_FileInfo.setAssetId( assetId ); }
	VxGUID						getAssetId( void )						{ return m_FileInfo.getAssetId(); }

	bool						toggleIsShared( void );
	bool						toggleIsInLibrary( void );
	void						updateWidget( void );
	bool						shouldOpenFile( void );

	bool						isDirectory( void )						{ return m_FileInfo.isDirectory(); }

    QString						describeFileType();
    QString						describeFileLength();

	VxMyFileInfo&				getMyFileInfo( void )					{ return m_FileInfo; }

	//=== vars ===//
private:
    VxMyFileInfo				m_FileInfo;
    QWidget *					m_Widget{nullptr};
    bool						m_IsShared{false};
    bool						m_IsInLibrary{false};
};
