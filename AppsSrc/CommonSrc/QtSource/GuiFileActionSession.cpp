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
// http://www.nolimitconnect.com
//============================================================================

#include <QWidget> // must be declared first or Qt 6.2.4 will error in qmetatype.h 2167:23: array subscript value ‘53’ is outside the bounds

#include "GuiFileActionSession.h"

#include <PktLib/VxSearchDefs.h>
#include <CoreLib/VxParse.h>
#include <CoreLib/VxFileInfo.h>

//============================================================================
GuiFileActionSession::GuiFileActionSession()
: m_ePluginType(ePluginTypeFileShareServer)
, m_Ident(NULL)
, m_Widget(0)
, m_IsShared( false )
, m_IsInLibrary( false )
{
}

//============================================================================
GuiFileActionSession::GuiFileActionSession(		EPluginType		ePluginType, 
												VxNetIdent *	netIdent, 
												VxGUID&			lclSessionId, 
												uint8_t			u8FileType, 
												uint64_t		u64FileLen, 
												const char *	pFileName,
												VxGUID			assetId,
												VxSha1Hash&		fileHashId )
: m_ePluginType( ePluginType )
, m_Ident( netIdent )
, m_LclSessionId( lclSessionId )
, m_FileInfo( pFileName, u8FileType, u64FileLen, assetId, fileHashId )
, m_Widget(0)
, m_IsShared( false )
, m_IsInLibrary( false )
{
}

//============================================================================
GuiFileActionSession::GuiFileActionSession(		EPluginType		ePluginType, 
												VxNetIdent *	netIdent, 
												VxGUID&			lclSessionId, 
												uint8_t			u8FileType, 
												uint64_t		u64FileLen, 
												const char *	pFileName,
												VxGUID			assetId,
												uint8_t *		fileHashId )
: m_ePluginType( ePluginType )
, m_Ident( netIdent )
, m_LclSessionId( lclSessionId )
, m_FileInfo( pFileName, u8FileType, u64FileLen, assetId, fileHashId )
, m_Widget(0)
, m_IsShared( false )
, m_IsInLibrary( false )
{
}

//============================================================================
GuiFileActionSession::GuiFileActionSession(		EPluginType		ePluginType, 
												VxNetIdent *	netIdent, 
												VxGUID&			lclSessionId, 
												VxMyFileInfo&	fileInfo )
: m_ePluginType( ePluginType )
, m_Ident( netIdent )
, m_LclSessionId( lclSessionId )
, m_FileInfo( fileInfo )
, m_Widget(0)
, m_IsShared( false )
, m_IsInLibrary( false )
{
}

//============================================================================
GuiFileActionSession::GuiFileActionSession(	VxMyFileInfo& fileInfo, bool isShared, bool isInLibrary )
: m_ePluginType( ePluginTypeFileShareServer )
, m_Ident( 0 )
, m_LclSessionId()
, m_FileInfo( fileInfo )
, m_Widget(0)
, m_IsShared( isShared )
, m_IsInLibrary( isInLibrary )
{
}
