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

#include "VxMyFileInfo.h"

#include <PktLib/VxCommon.h>
#include <CoreLib/VxSha1Hash.h>
#include <CoreLib/VxGUID.h>

#include <QString>

class P2PEngine;
class QWidget;

class GuiFileActionSession : public QObject
{
	Q_OBJECT
public:
	GuiFileActionSession();

	GuiFileActionSession(	EPluginType		ePluginType, 
							VxNetIdent *	netIdent, 
							VxGUID&			lclSessionId, 
							uint8_t			u8FileType, 
							uint64_t		u64FileLen, 
							const char *	pFileName,
							VxGUID			assetId,
							VxSha1Hash&		fileHashId );

	GuiFileActionSession(	EPluginType		ePluginType, 
							VxNetIdent *	netIdent, 
							VxGUID&			lclSessionId, 
							uint8_t			u8FileType, 
							uint64_t		u64FileLen, 
							const char *	pFileName,
							VxGUID			assetId,
							uint8_t *		fileHashId );

	GuiFileActionSession(	EPluginType		ePluginType, 
							VxNetIdent *	netIdent, 
							VxGUID&			lclSessionId, 
							VxMyFileInfo&	fileInfo );

	GuiFileActionSession(	VxMyFileInfo&	fileInfo, bool isShared = false, bool isInLibrary = false );


	QString						describeFileType( void )					{ return m_FileInfo.describeFileType(); }
	QString						describeFileLength( void )					{ return m_FileInfo.describeFileLength(); }

	void						setLclSessionId( VxGUID lclSessionId )		{ m_LclSessionId = lclSessionId; }
	VxGUID&						getLclSessionId( void )						{ return m_LclSessionId; }

	void						setIdent( VxNetIdent * ident )				{ m_Ident = ident; }
	VxNetIdent *				getIdent( void )							{ return m_Ident; }

	void						setWidget( QWidget * widget )				{ m_Widget = widget; }
	QWidget *					getWidget( void )							{ return m_Widget; }

	void						setFileInfo( VxMyFileInfo& fileInfo )		{ m_FileInfo = fileInfo; }
	VxMyFileInfo&				getFileInfo( void )							{ return m_FileInfo; }

	QString&					getFullFileName( void )						{ return m_FileInfo.getFullFileName(); }
	void						setFileType( uint8_t	fileType )			{ m_FileInfo.setFileType( fileType ); }
	uint8_t						getFileType( void ) const					{ return m_FileInfo.getFileType(); }
	void						setFileLength( uint64_t fileLen )			{ m_FileInfo.setFileLength( fileLen ); }
	uint64_t					getFileLength( void ) const					{ return m_FileInfo.getFileLength(); }
	void						setFileHashId( VxSha1Hash& id )				{ m_FileInfo.getFileHashId() = id; }
	void						setFileHashId( uint8_t * fileHashData )		{ m_FileInfo.setFileHashId( fileHashData ); }
	VxSha1Hash&					getFileHashId( void )						{ return m_FileInfo.getFileHashId(); }

	VxGUID						getAssetId( void )							{ return m_FileInfo.getAssetId(); }

	void						setJustFileName( QString fileName )			{ m_FileInfo.setJustFileName( fileName ); }
	QString&					getJustFileName( void )						{ return m_FileInfo.getJustFileName(); }

	void						setIsShared( bool isShared )				{ m_IsShared = isShared; }
	bool						getIsShared( void )							{ return m_IsShared; }

	void						setIsInLibrary( bool isInLibrary )			{ m_IsInLibrary = isInLibrary; }
	bool						getIsInLibrary( void )						{ return m_IsInLibrary; }

private:
	//=== vars ===//
	EPluginType					m_ePluginType;
	VxNetIdent *				m_Ident;
	VxGUID						m_LclSessionId;
	VxMyFileInfo				m_FileInfo;
	QWidget *					m_Widget;
	bool						m_IsShared;
	bool						m_IsInLibrary;
};

