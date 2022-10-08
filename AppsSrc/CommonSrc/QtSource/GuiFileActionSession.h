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

#include <ptop_src/ptop_engine_src/Plugins/FileInfo.h>

#include <PktLib/VxCommon.h>
#include <CoreLib/VxSha1Hash.h>
#include <CoreLib/VxGUID.h>

#include <QObject>
#include <QString>

class GuiUser;
class P2PEngine;
class QWidget;

class GuiFileActionSession : public QObject
{
	Q_OBJECT
public:
	GuiFileActionSession();
	GuiFileActionSession( FileInfo& fileInfo );
	GuiFileActionSession( EPluginType pluginType, GuiUser* guiUser, VxGUID& lclSessionId, FileInfo& fileInfo );

	QString						describeFileType( void );
	QString						describeFileLength( void );

	void						setLclSessionId( VxGUID lclSessionId ) { m_LclSessionId = lclSessionId; }
	VxGUID&						getLclSessionId( void )				{ return m_LclSessionId; }

	void						setIdent( GuiUser* ident )			{ m_Ident = ident; }
	GuiUser*					getIdent( void )					{ return m_Ident; }

	void						setWidget( QWidget* widget )		{ m_Widget = widget; }
	QWidget*					getWidget( void )					{ return m_Widget; }

	void						setFileInfo( FileInfo& fileInfo )	{ m_FileInfo = fileInfo; }
	FileInfo&					getFileInfo( void )					{ return m_FileInfo; }

	QString 					getFullFileName( void )				{ return m_FileInfo.getFullFileName().c_str(); }
	void						setFileType( uint8_t fileType )		{ m_FileInfo.setFileType( fileType ); }
	uint8_t						getFileType( void ) const			{ return m_FileInfo.getFileType(); }
	void						setFileLength( uint64_t fileLen ) { m_FileInfo.setFileLength( fileLen ); }
	uint64_t					getFileLength( void ) const			{ return m_FileInfo.getFileLength(); }
	void						setFileHashId( VxSha1Hash& id )		{ m_FileInfo.getFileHashId() = id; }
	void						setFileHashId( uint8_t* fileHashData ) { m_FileInfo.setFileHashId( fileHashData ); }
	VxSha1Hash&					getFileHashId( void )				{ return m_FileInfo.getFileHashId(); }

	VxGUID						getAssetId( void )					{ return m_FileInfo.getAssetId(); }

	QString 					getJustFileName( void )				{ return m_FileInfo.getJustFileName().c_str(); }

	void						setIsSharedFile( bool isShared )	{ m_FileInfo.setIsSharedFile( isShared ); }
	bool						getIsShared( void )					{ return m_FileInfo.getIsSharedFile(); }

	void						setIsInLibrary( bool isInLibrary )	{ m_FileInfo.setIsInLibrary( isInLibrary ); }
	bool						getIsInLibrary( void )				{ return m_FileInfo.getIsInLibrary(); }

private:
	//=== vars ===//
	EPluginType					m_ePluginType;
	GuiUser*					m_Ident{ nullptr };
	VxGUID						m_LclSessionId;
	FileInfo				    m_FileInfo;
	QWidget*					m_Widget{ nullptr };
};

