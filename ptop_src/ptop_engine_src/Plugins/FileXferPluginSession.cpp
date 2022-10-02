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

#include "FileXferPluginSession.h"
#include "PluginBase.h"

#include <PktLib/PktsFileShare.h>

#include <stdio.h>

//============================================================================
FileXferPluginSession::FileXferPluginSession( VxSktBase* sktBase, VxNetIdent* netIdent, EPluginType pluginType )
: P2PSession( sktBase, netIdent, pluginType )
, m_Error( 0 )
{
}

//============================================================================
FileXferPluginSession::FileXferPluginSession( VxGUID& lclSessionId, VxSktBase* sktBase, VxNetIdent* netIdent, EPluginType pluginType )
: P2PSession( lclSessionId, sktBase, netIdent, pluginType )
, m_Error( 0 )
{
}

//============================================================================
FileXferPluginSession::~FileXferPluginSession()
{
}

//============================================================================
//! return true if file is already in que to be sent or has been sent
bool FileXferPluginSession::isFileQuedOrSent( const char* pFileName )
{
	return false;
}

//============================================================================
void FileXferPluginSession::cancelUpload( PluginBase& pluginBase, VxGUID& lclSessionId )
{
	PktFileChunkReq oPkt;
	oPkt.setDataLen( 0 );
	oPkt.setError( eXferErrorCanceled );
	oPkt.setLclSessionId( lclSessionId );
	oPkt.setRmtSessionId( m_RmtSessionId );
	pluginBase.txPacket( getIdent(), getSkt(), &oPkt );

	if( m_TxFileInfo.m_hFile )
	{
		fclose( m_TxFileInfo.m_hFile );
	}

	std::vector<FileToXfer>::iterator iter;
	for( iter = m_astrFilesToSend.begin(); iter != m_astrFilesToSend.end(); ++iter )
	{
		if( (*iter).getLclSessionId() == lclSessionId )
		{
			m_astrFilesToSend.erase(iter);
			break;
		}
	}

	for( iter = m_astrFilesSent.begin(); iter != m_astrFilesSent.end(); ++iter )
	{
		if( (*iter).getLclSessionId() == lclSessionId )
		{
			m_astrFilesSent.erase(iter);
			break;
		}
	}	

	for( iter = m_astrFilesReceived.begin(); iter != m_astrFilesReceived.end(); ++iter )
	{
		if( (*iter).getLclSessionId() == lclSessionId )
		{
			m_astrFilesReceived.erase(iter);
			break;
		}
	}	
}

//============================================================================
void FileXferPluginSession::cancelDnload( PluginBase& pluginBase, VxGUID& lclSessionId )
{
	PktFileChunkReply oPkt;
	oPkt.setDataLen( 0 );
	oPkt.setError( eXferErrorCanceled );
	oPkt.setLclSessionId( lclSessionId );
	oPkt.setRmtSessionId( m_RmtSessionId );
	pluginBase.txPacket( getIdent(), getSkt(), &oPkt );

	if( m_RxFileInfo.m_hFile )
	{
		fclose( m_RxFileInfo.m_hFile );
	}

	std::vector<FileToXfer>::iterator iter;
	for( iter = m_astrFilesToSend.begin(); iter != m_astrFilesToSend.end(); ++iter )
	{
		if( (*iter).getLclSessionId() == lclSessionId )
		{
			m_astrFilesToSend.erase(iter);
			break;
		}
	}

	for( iter = m_astrFilesSent.begin(); iter != m_astrFilesSent.end(); ++iter )
	{
		if( (*iter).getLclSessionId() == lclSessionId )
		{
			m_astrFilesSent.erase(iter);
			break;
		}
	}	

	for( iter = m_astrFilesReceived.begin(); iter != m_astrFilesReceived.end(); ++iter )
	{
		if( (*iter).getLclSessionId() == lclSessionId )
		{
			m_astrFilesReceived.erase(iter);
			break;
		}
	}	
}
