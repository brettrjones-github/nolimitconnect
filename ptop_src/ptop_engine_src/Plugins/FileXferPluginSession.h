#pragma once
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

#include "P2PSession.h"
#include "FileToXfer.h"

#include <NetLib/VxFileXferInfo.h>

class VxSktBase;
class VxPktHdr;
class PluginSessionFileXfer;
class FileXferDirectories;
class PluginBase;

class FileXferPluginSession : public P2PSession
{
public:
	FileXferPluginSession( VxSktBase * sktBase, VxNetIdent * netIdent, EPluginType pluginType );
	FileXferPluginSession( VxGUID& lclSessionId, VxSktBase * sktBase, VxNetIdent * netIdent, EPluginType pluginType );
	virtual ~FileXferPluginSession();

	//! return true if file is already in que to be sent or has been sent
	bool						isFileQuedOrSent( const char * pFileName );

	VxFileXferInfo&				getRxXferInfo( void )							{ return m_RxFileInfo; }
	VxFileXferInfo&				getTxXferInfo( void )							{ return m_TxFileInfo; }

	void						cancelUpload( PluginBase& pluginBase, VxGUID& lclSessionId );
	void						cancelDnload( PluginBase& pluginBase, VxGUID& lclSessionId );

	void						setErrorCode( RCODE error )					{ if( error ) m_Error = error; } 
	RCODE						getErrorCode( void )						{ return m_Error; }
	void						clearErrorCode( void )						{ m_Error = 0; }


	VxFileXferInfo				m_TxFileInfo;			// file being transmitted
	VxFileXferInfo				m_RxFileInfo;			// file being received

	std::vector<FileToXfer>		m_astrFilesToSend;		// list of files to send
	std::vector<FileToXfer>		m_astrFilesSent;		// list of files sent
	std::vector<FileToXfer>		m_astrFilesReceived;		// list of files sent
	uint32_t					m_Error;
};
