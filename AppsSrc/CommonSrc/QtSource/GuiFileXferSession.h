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

#include <PktLib/VxCommon.h>
#include <CoreLib/VxSha1Hash.h>
#include <GuiInterface/IDefs.h>

#include <QString>
#include <QObject>

class GuiUser;
class P2PEngine;

class GuiFileXferSession : public QObject
{
public:
	GuiFileXferSession() = default;

	GuiFileXferSession( EPluginType pluginType, GuiUser* guiUser, FileInfo& fileInfo );
	GuiFileXferSession( EPluginType pluginType, GuiUser* guiUser, VxGUID& lclSessionId, FileInfo& fileInfo );

	GuiFileXferSession( const GuiFileXferSession &rhs );
	GuiFileXferSession&			operator =( const GuiFileXferSession &rhs );

	EPluginType					getPluginType( void )						{ return m_ePluginType; }

	QString						describeFileType( void );
	QString						describeFileLength( void );

	void						setLclSessionId( VxGUID lclSessionId )	    { m_LclSessionId = lclSessionId; }
	VxGUID&					    getLclSessionId( void )						{ return m_LclSessionId; }

	void						setIdent( GuiUser* ident )				    { m_Ident = ident; }
    GuiUser*				    getIdent( void )							{ return m_Ident; }

	void						setWidget( QWidget* widget )				{ m_Widget = widget; }
	QWidget*					getWidget( void )							{ return m_Widget; }

	void						setXferDirection( EXferDirection direction ){ m_XferDirection = direction; }
	EXferDirection				getXferDirection( void )					{ return m_XferDirection; }

	void						setFileInfo( FileInfo& fileInfo )		    { m_FileInfo = fileInfo; }
	FileInfo&					getFileInfo( void )							{ return m_FileInfo; }

	void						setFullFileName( QString fileName )			{ m_FileInfo.setFullFileName( fileName.toUtf8().constData() ); }
	QString 					getFullFileName( void )						{ return m_FileInfo.getFullFileName().c_str(); }
	void						setFileType( uint8_t fileType )			    { m_FileInfo.setFileType( fileType ); }
	uint8_t						getFileType( void ) const					{ return m_FileInfo.getFileType(); }
	void						setFileLength( uint64_t fileLen )			{ m_FileInfo.setFileLength( fileLen ); }
	uint64_t					getFileLength( void ) const					{ return m_FileInfo.getFileLength(); }
	void						setFileHashId( VxSha1Hash& id )				{ m_FileInfo.getFileHashId() = id; }
	void						setFileHashId( uint8_t * fileHashData )		{ m_FileInfo.setFileHashId( fileHashData ); }
	VxSha1Hash&					getFileHashId( void )						{ return m_FileInfo.getFileHashId(); }

	VxGUID						getAssetId( void )							{ return m_FileInfo.getAssetId(); }

	void						setIsSharedFile( bool isShared )			{ m_FileInfo.setIsSharedFile( isShared ); }
	bool						getIsSharedFile( void )						{ return m_FileInfo.getIsSharedFile(); }
	void						setIsInLibrary( bool isInLibrary )			{ m_FileInfo.setIsInLibrary( isInLibrary ); }
	bool						getIsInLibrary( void )						{ return m_FileInfo.getIsInLibrary(); }

	QString					    getJustFileName( void )						{ return m_FileInfo.getJustFileName().c_str(); }
	QString						getFilePath( void )							{ return m_FileInfo.getFilePath().c_str(); }

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
    EPluginType					m_ePluginType{ ePluginTypeFileShareServer };
    GuiUser*				    m_Ident{ nullptr };
	VxGUID					    m_LclSessionId;
	FileInfo					m_FileInfo;
	QWidget*					m_Widget{ nullptr };
	EXferState					m_eXferState{ eXferStateUnknown };
    int							m_XferStateParam1{ 0 };
	int							m_XferStateParam2{ 0 };
	RCODE						m_XferErrorCode{ 0 };
    EXferDirection				m_XferDirection{ eXferDirectionRx };
};

