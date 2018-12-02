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
// brett.jones@engineer.com
// http://www.gotvptop.net
//============================================================================

#include "VxMyFileInfo.h"

#include <PktLib/VxCommon.h>
#include <CoreLib/VxSha1Hash.h>

#include <QObject>

#include <string>

class P2PEngine;
class OfferWidget;
class VxNetIdent;

class FileListReplySession : public QObject
{
	Q_OBJECT
public:
	FileListReplySession();
	FileListReplySession(	EPluginType		ePluginType, 
							VxNetIdent *	netIdent, 
							uint8_t				u8FileType, 
							uint64_t				u64FileLen, 
							const char *	pFileName,
							uint8_t *			fileHashData );
    FileListReplySession(	EPluginType		ePluginType,
                            VxNetIdent *	netIdent,
                            VxMyFileInfo&    fileInfo );
	FileListReplySession(const FileListReplySession& rhs);
	FileListReplySession& operator=(const FileListReplySession& rhs); 

    void						setIdent( VxNetIdent * ident )			{ m_Ident = ident; }
    VxNetIdent *				getIdent( void )						{ return m_Ident; }

    void						setFileInfo( VxMyFileInfo& fileInfo )	{ m_FileInfo = fileInfo; }
    VxMyFileInfo&				getFileInfo( void )						{ return m_FileInfo; }

    QString&					getFullFileName( void )					{ return m_FileInfo.getFullFileName(); }
    void						setFileType( uint8_t	fileType )				{ m_FileInfo.setFileType( fileType ); }
    uint8_t							getFileType( void ) const				{ return m_FileInfo.getFileType(); }
    void						setFileLength( uint64_t fileLen )			{ m_FileInfo.setFileLength( fileLen ); }
    uint64_t							getFileLength( void ) const				{ return m_FileInfo.getFileLength(); }
    void						setFileHashId( VxSha1Hash& id )			{ m_FileInfo.getFileHashId() = id; }
    void						setFileHashId( uint8_t * fileHashData )		{ m_FileInfo.setFileHashId( fileHashData ); }
    VxSha1Hash&					getFileHashId( void )					{ return m_FileInfo.getFileHashId(); }

    void						setJustFileName( QString fileName )		{ m_FileInfo.setJustFileName( fileName ); }
    QString&					getJustFileName( void )					{ return m_FileInfo.getJustFileName(); }



	//=== vars ===//
    EPluginType                 m_ePluginType;

protected:
    VxNetIdent *                m_Ident;
    VxMyFileInfo					m_FileInfo;
};
