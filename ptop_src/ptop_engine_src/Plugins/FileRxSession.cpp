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

#include "FileRxSession.h"

#include <CoreLib/VxFileUtil.h>

#include <stdio.h>

//============================================================================
FileRxSession::FileRxSession()
: FileShareXferSession()
{
	getXferInfo().setXferDirection( eXferDirectionRx );
}

//============================================================================
FileRxSession::FileRxSession( VxSktBase * sktBase, VxNetIdent * netIdent )
: FileShareXferSession( sktBase, netIdent )
{
	getXferInfo().setXferDirection( eXferDirectionRx );
}

//============================================================================
FileRxSession::FileRxSession( VxGUID& lclSessionId, VxSktBase * sktBase, VxNetIdent * netIdent )
: FileShareXferSession( lclSessionId, sktBase, netIdent )
{
	getXferInfo().setXferDirection( eXferDirectionRx );
}

//============================================================================
FileRxSession::~FileRxSession()
{
}

//============================================================================
void FileRxSession::cancelDownload( VxGUID& lclSessionId )
{
	VxFileXferInfo& xferInfo = getXferInfo();
	if( xferInfo.m_hFile )
	{
		fclose( xferInfo.m_hFile );
	}

	VxFileUtil::deleteFile( xferInfo.getLclFileName().c_str() );

	std::vector<FileToXfer>::iterator iter;

	for( iter = m_astrFilesToXfer.begin(); iter != m_astrFilesToXfer.end(); ++iter )
	{
		if( (*iter).getLclSessionId() == lclSessionId )
		{
			m_astrFilesToXfer.erase(iter);
			break;
		}
	}

	for( iter = m_astrFilesXfered.begin(); iter != m_astrFilesXfered.end(); ++iter )
	{
		if( (*iter).getLclSessionId() == lclSessionId )
		{
			m_astrFilesToXfer.erase(iter);
			break;
		}
	}
}
