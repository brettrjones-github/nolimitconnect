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
// http://www.nolimitconnect.org
//============================================================================

#include <ptop_src/ptop_engine_src/Plugins/FileInfo.h>

#include <QWidget>

#define FILE_STATUS_DOWNLOADING			0x0001
#define FILE_STATUS_UPLOADING			0x0002

class VxNetIdent;
class AppCommon;
class AssetBaseInfo;

class FileItemInfo
{
public:
	FileItemInfo( FileInfo& fileInfo );

	void						setIsSharedFile( bool isShared )		{ m_FileInfo.setIsSharedFile( isShared ); }
	bool						getIsSharedFile( void )					{ return m_FileInfo.getIsSharedFile(); }

	void						setIsInLibrary( bool isInLibrary )		{ m_FileInfo.setIsInLibrary( isInLibrary ); }
	bool						getIsInLibrary( void )					{ return m_FileInfo.getIsInLibrary(); }

	QString					    getFullFileName( void )					{ return m_FileInfo.getFullFileName().c_str(); }
	QString						getJustFileName( void )					{ return m_FileInfo.getShortFileName().c_str(); }

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

	FileInfo&					getFileInfo( void )						{ return m_FileInfo; }
	bool						toAsssetInfo( AppCommon& myApp, AssetBaseInfo& assetInfo, bool* retIsNewAsset = nullptr );

protected:
	//=== vars ===//
    FileInfo					m_FileInfo;
    QWidget*					m_Widget{nullptr};
};
