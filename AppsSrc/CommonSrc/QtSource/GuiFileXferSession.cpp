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

#include <QWidget> // must be declared first or Qt 6.2.4 will error in qmetatype.h 2167:23: array subscript value ‘53’ is outside the bounds

#include "GuiFileXferSession.h"
#include "GuiHelpers.h"
#include "GuiParams.h"

#include <PktLib/VxSearchDefs.h>
#include <CoreLib/VxParse.h>
#include <CoreLib/VxFileInfo.h>

#include <QObject>

//============================================================================
GuiFileXferSession::GuiFileXferSession( EPluginType		pluginType,
										GuiUser* guiUser,
										FileInfo& fileInfo )
	: m_ePluginType( pluginType )
	, m_Ident( guiUser )
	, m_FileInfo( fileInfo )
{
}

//============================================================================
GuiFileXferSession::GuiFileXferSession(		EPluginType		pluginType, 
                                            GuiUser*		guiUser,
											VxGUID&			lclSessionId, 
											FileInfo&		fileInfo )
: m_ePluginType( pluginType )
, m_Ident( guiUser )
, m_LclSessionId( lclSessionId )
, m_FileInfo( fileInfo )
{
}

//============================================================================
GuiFileXferSession::GuiFileXferSession( const GuiFileXferSession &rhs )
    : QObject()
{
	*this = rhs;
}

//============================================================================
GuiFileXferSession& GuiFileXferSession::operator =( const GuiFileXferSession &rhs )
{
	if( this != &rhs )   
	{

		m_ePluginType		= rhs.m_ePluginType;
		m_Ident				= rhs.m_Ident;
		m_LclSessionId		= rhs.m_LclSessionId;
		m_FileInfo			= rhs.m_FileInfo;
		m_Widget			= rhs.m_Widget;
		m_eXferState		= rhs.m_eXferState;
		m_XferStateParam1	= rhs.m_XferStateParam1;
		m_XferStateParam2	= rhs.m_XferStateParam2;
		m_XferErrorCode		= rhs.m_XferErrorCode;
	}

	return *this;
}

//============================================================================
QString GuiFileXferSession::describeFileType()
{
	switch( m_FileInfo.getFileType() )
	{
	case VXFILE_TYPE_PHOTO:
		return QObject::tr("Photo: ");
	case VXFILE_TYPE_AUDIO:
		return QObject::tr("Audio: ");
	case VXFILE_TYPE_VIDEO:
		return QObject::tr("Video: ");
	case VXFILE_TYPE_DOC:
		return QObject::tr("Documents: ");
	case VXFILE_TYPE_ARCHIVE_OR_CDIMAGE:
		return QObject::tr("Archive Or ISO: ");
	case VXFILE_TYPE_EXECUTABLE:
		return QObject::tr("Executable: ");
	case VXFILE_TYPE_DIRECTORY:
		return QObject::tr("Folder: ");
	default:
		return QObject::tr("Other: ");
	}
}

//============================================================================
QString GuiFileXferSession::describeFileLength()
{
	return GuiParams::describeFileLength( m_FileInfo.getFileLength() );
}

//============================================================================
bool GuiFileXferSession::getIsCompleted( void )
{
	return ( eXferStateCompletedUpload == m_eXferState ) || ( eXferStateCompletedDownload == m_eXferState );
}

//============================================================================
bool GuiFileXferSession::getIsInError( void )
{
	return ( eXferStateDownloadError == m_eXferState ) || ( eXferStateUploadError == m_eXferState );
}

//============================================================================
QString GuiFileXferSession::describeXferState( void )
{
	return GuiParams::describeEXferState( m_eXferState );
}
