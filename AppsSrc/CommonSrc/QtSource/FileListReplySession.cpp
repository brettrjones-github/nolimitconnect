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

#include "FileListReplySession.h"

//============================================================================
FileListReplySession::FileListReplySession()
: QObject()
{
}

//============================================================================
FileListReplySession::FileListReplySession(	EPluginType	pluginType, GuiUser* guiUser, FileInfo& fileInfo )                                       
: QObject()
, m_PluginType(ePluginTypeInvalid)
, m_Ident( guiUser )
, m_FileInfo(fileInfo)
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
		m_PluginType			= rhs.m_PluginType;
		m_Ident					= rhs.m_Ident;
        m_FileInfo              = rhs.m_FileInfo;
	}

	return *this;
}
