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

#include "FileListReplySession.h"

//============================================================================
FileListReplySession::FileListReplySession()
: QObject()
, m_ePluginType(ePluginTypeInvalid)
, m_Ident(0)
, m_FileInfo()
{
}

//============================================================================
FileListReplySession::FileListReplySession(	EPluginType		ePluginType,
                                            GuiUser *	    netIdent,
                                            VxMyFileInfo&   fileInfo )
: QObject()
, m_ePluginType(ePluginTypeInvalid)
, m_Ident(netIdent)
, m_FileInfo(fileInfo)
{
}


//============================================================================
FileListReplySession::FileListReplySession(	 EPluginType	ePluginType, 
                                             GuiUser *	    netIdent, 
											 uint8_t		u8FileType, 
											 uint64_t		u64FileLen, 
											 const char *	pFileName,
											 VxGUID			assetId,
											 uint8_t *		fileHashData )
: QObject()
, m_ePluginType(ePluginType)
, m_Ident(netIdent)
, m_FileInfo( pFileName, u8FileType, u64FileLen, assetId, fileHashData )
{
}

//============================================================================
FileListReplySession::FileListReplySession(const FileListReplySession& rhs)
: QObject()
{
	*this = rhs;
}

//============================================================================
FileListReplySession& FileListReplySession::operator=(const FileListReplySession& rhs)
{
	if( this != &rhs )
	{
		m_ePluginType			= rhs.m_ePluginType;
		m_Ident					= rhs.m_Ident;
        m_FileInfo              = rhs.m_FileInfo;
	}

	return *this;
}
