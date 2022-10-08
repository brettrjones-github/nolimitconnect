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

#include <QWidget> // must be declared first or Qt 6.2.4 will error in qmetatype.h 2167:23: array subscript value ‘53’ is outside the bounds

#include "GuiFileActionSession.h"
#include "GuiParams.h"

#include <PktLib/VxSearchDefs.h>
#include <CoreLib/VxParse.h>
#include <CoreLib/VxFileInfo.h>

//============================================================================
GuiFileActionSession::GuiFileActionSession()
: m_ePluginType(ePluginTypeFileShareServer)
{
}

//============================================================================
GuiFileActionSession::GuiFileActionSession( FileInfo& fileInfo )
	: m_ePluginType( ePluginTypeFileShareServer )
	, m_LclSessionId()
	, m_FileInfo( fileInfo )
{
}

//============================================================================
GuiFileActionSession::GuiFileActionSession(	EPluginType	pluginType, GuiUser* guiUser, VxGUID& lclSessionId, FileInfo& fileInfo )
: m_ePluginType( pluginType )
, m_Ident( guiUser )
, m_LclSessionId( lclSessionId )
, m_FileInfo( fileInfo )
{
}

//============================================================================
QString	GuiFileActionSession::describeFileType( void ) 
{ 
	return GuiParams::describeFileType( m_FileInfo.getFileType() ); 
}

//============================================================================
QString	GuiFileActionSession::describeFileLength( void ) 
{ 
	return GuiParams::describeFileLength( m_FileInfo.getFileLength() ); 
}