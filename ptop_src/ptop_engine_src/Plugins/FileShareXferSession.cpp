//============================================================================
// Copyright (C) 2010 Brett R. Jones
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

#include "FileShareXferSession.h"

#include <stdio.h>

//============================================================================
FileShareXferSession::FileShareXferSession()
: m_astrFilesToXfer()
, m_astrFilesXfered()
, m_FileXferInfo()
, m_iPercentComplete(0)
, m_Skt(NULL)
, m_Ident(NULL)
, m_Error( 0 )
{
	initLclSessionId();
}

//============================================================================
FileShareXferSession::FileShareXferSession( VxSktBase* sktBase, VxNetIdent* netIdent )
: m_astrFilesToXfer()
, m_astrFilesXfered()
, m_FileXferInfo()
, m_iPercentComplete(0)
, m_Skt( sktBase )
, m_Ident( netIdent )
, m_Error( 0 )
{
	initLclSessionId();
}

//============================================================================
FileShareXferSession::FileShareXferSession( VxGUID& lclSessionId, VxSktBase* sktBase, VxNetIdent* netIdent )
: m_astrFilesToXfer()
, m_astrFilesXfered()
, m_FileXferInfo( lclSessionId )
, m_iPercentComplete(0)
, m_Skt( sktBase )
, m_Ident( netIdent )
, m_Error( 0 )
{
	initLclSessionId();
}

//============================================================================
void FileShareXferSession::reset( void )
{
	m_iPercentComplete = 0;
}

//============================================================================
void FileShareXferSession::initLclSessionId( void )
{
	if( false == m_FileXferInfo.getLclSessionId().isVxGUIDValid() )
	{
		m_FileXferInfo.getLclSessionId().initializeWithNewVxGUID();
	}
}

//============================================================================
bool FileShareXferSession::isXferingFile( void )
{
	if( m_FileXferInfo.m_hFile )
	{
		return true;
	}
	return false;
}
