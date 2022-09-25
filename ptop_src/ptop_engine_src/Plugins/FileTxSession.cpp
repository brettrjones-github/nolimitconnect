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

#include "FileTxSession.h"

#include <CoreLib/VxFileUtil.h>

#include <stdio.h>

//============================================================================
FileTxSession::FileTxSession()
: FileShareXferSession()
, m_iOutstandingAckCnt( 0 )
, m_bSendingPkts( false )
, m_bViewingFileList( false )
, m_QuePosition( 0 )
{
	setXferDirection( eXferDirectionTx );
}

//============================================================================
FileTxSession::FileTxSession( VxSktBase* sktBase, VxNetIdent* netIdent )
: FileShareXferSession( sktBase, netIdent )
, m_iOutstandingAckCnt( 0 )
, m_bSendingPkts( false )
, m_bViewingFileList( false )
, m_QuePosition( 0 )
{
	setXferDirection( eXferDirectionTx );
}

//============================================================================
FileTxSession::FileTxSession( VxGUID& lclSessionId, VxSktBase* sktBase, VxNetIdent* netIdent )
: FileShareXferSession( lclSessionId, sktBase, netIdent )
, m_iOutstandingAckCnt(0)
, m_bSendingPkts(false)
, m_bViewingFileList(false)
, m_QuePosition( 0 )
{
	setXferDirection( eXferDirectionTx );
}

//============================================================================
void FileTxSession::reset( void )
{
	FileShareXferSession::reset();
	m_iOutstandingAckCnt = 0;
	m_bSendingPkts = false;
	m_bViewingFileList = false;
	m_strOfferFile = "";
	m_strViewDirectory = "";
}

//============================================================================
void FileTxSession::cancelUpload( VxGUID& lclSessionId )
{
	if( m_FileXferInfo.m_hFile )
	{
		fclose( m_FileXferInfo.m_hFile );
	}

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
