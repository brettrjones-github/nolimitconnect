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

#include "VxGuidQt.h"
#include "VxMyFileInfo.h"

#include <PktLib/VxCommon.h>
#include <CoreLib/VxSha1Hash.h>
#include <GoTvInterface/IDefs.h>

#include <QString>
#include <QObject>

class P2PEngine;

class GuiFileXferSession : public QObject
{
public:
	GuiFileXferSession();

	GuiFileXferSession(	EPluginType		ePluginType, 
						VxNetIdent *	netIdent, 
						VxGuidQt&		lclSessionId, 
						uint8_t			u8FileType, 
						uint64_t		u64FileLen, 
						const char *	pFileName,
						VxSha1Hash&		fileHashId );

	GuiFileXferSession(	EPluginType		ePluginType, 
						VxNetIdent *	netIdent, 
						VxGuidQt&		lclSessionId, 
						uint8_t			u8FileType, 
						uint64_t		u64FileLen, 
						const char *	pFileName,
						uint8_t *		fileHashId );

	GuiFileXferSession(	EPluginType		ePluginType, 
						VxNetIdent *	netIdent, 
						VxGuidQt&		lclSessionId, 
						VxMyFileInfo&	fileInfo );

	GuiFileXferSession( const GuiFileXferSession &rhs );
	GuiFileXferSession&			operator =( const GuiFileXferSession &rhs );

	QString						describeFileType( void );
	QString						describeFileLength( void );

	void						setLclSessionId( VxGuidQt lclSessionId )	{ m_LclSessionId = lclSessionId; }
	VxGuidQt&					getLclSessionId( void )						{ return m_LclSessionId; }

	void						setIdent( VxNetIdent * ident )				{ m_Ident = ident; }
	VxNetIdent *				getIdent( void )							{ return m_Ident; }

	void						setWidget( QWidget * widget )				{ m_Widget = widget; }
	QWidget *					getWidget( void )							{ return m_Widget; }

	void						setXferDirection( EXferDirection direction ){ m_XferDirection = direction; }
	EXferDirection				getXferDirection( void )					{ return m_XferDirection; }

	void						setFileInfo( VxMyFileInfo& fileInfo )		{ m_FileInfo = fileInfo; }
	VxMyFileInfo&				getFileInfo( void )							{ return m_FileInfo; }

	void						setFullFileName( QString fileName )			{ m_FileInfo.setFullFileName( fileName ); }
	QString&					getFullFileName( void )						{ return m_FileInfo.getFullFileName(); }
	void						setFileType( uint8_t	fileType )			{ m_FileInfo.setFileType( fileType ); }
	uint8_t						getFileType( void ) const					{ return m_FileInfo.getFileType(); }
	void						setFileLength( uint64_t fileLen )			{ m_FileInfo.setFileLength( fileLen ); }
	uint64_t					getFileLength( void ) const					{ return m_FileInfo.getFileLength(); }
	void						setFileHashId( VxSha1Hash& id )				{ m_FileInfo.getFileHashId() = id; }
	void						setFileHashId( uint8_t * fileHashData )		{ m_FileInfo.setFileHashId( fileHashData ); }
	VxSha1Hash&					getFileHashId( void )						{ return m_FileInfo.getFileHashId(); }

	void						setIsShared( bool isShared )				{ m_FileInfo.setIsShared( isShared ); }
	bool						getIsShared( void )							{ return m_FileInfo.getIsShared(); }
	void						setIsInLibrary( bool isInLibrary )			{ m_FileInfo.setIsInLibrary( isInLibrary ); }
	bool						getIsInLibrary( void )						{ return m_FileInfo.getIsInLibrary(); }

	void						setJustFileName( QString fileName )			{ m_FileInfo.setJustFileName( fileName ); }
	QString&					getJustFileName( void )						{ return m_FileInfo.getJustFileName(); }
	void						setFilePath( QString filePath )				{ m_FileInfo.setFilePath( filePath ); }
	QString&					getFilePath( void )							{ return m_FileInfo.getFilePath(); }

	bool						getIsCompleted( void );
	bool						getIsInError( void );
	void						setXferErrorCode( RCODE xferErrCode )		{ m_XferErrorCode = xferErrCode; }
	RCODE						getXferErrorCode( void ) const				{ return m_XferErrorCode; }

	void						setXferState( EXferState xferState, int param1, int param2 ) { m_eXferState = xferState; m_XferStateParam1 = param1; m_XferStateParam2 = param2; }
	EXferState					getXferState( void ) const					{ return m_eXferState; }
	int							getXferStateParam1( void ) const			{ return m_XferStateParam1; }
	int							getXferStateParam2( void ) const			{ return m_XferStateParam2; }
	QString						describeXferState( void );

	bool						isDirectory( void )							{ return m_FileInfo.isDirectory(); }

protected:
	//=== vars ===//
	EPluginType					m_ePluginType;
	VxNetIdent *				m_Ident;
	VxGuidQt					m_LclSessionId;
	VxMyFileInfo				m_FileInfo;
	QWidget *					m_Widget;
	EXferState					m_eXferState;
	int							m_XferStateParam1;
	int							m_XferStateParam2;
	RCODE						m_XferErrorCode;
	EXferDirection				m_XferDirection;
};

