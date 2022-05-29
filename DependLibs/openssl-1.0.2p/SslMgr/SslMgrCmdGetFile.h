#ifndef SSLMGR_CMD_GET_FILE_H
#define SSLMGR_CMD_GET_FILE_H

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

#include "SslMgrCmdBase.h"

#include <CoreLib/VxDefs.h>


enum ESslMgrCmdGetFileState
{
	eCmdGetFileStateWaitSendCmd,
	eCmdGetFileStateWaitOk,
	eCmdGetFileStateWaitFileLen,
	eCmdGetFileStateWaitCheckSum,
	eCmdGetFileStateRxFileBytes,

	eCmdGetFileStateSuccess,
	eCmdGetFileStateError
};

class SslMgrCmdGetFile : public SslMgrCmdBase
{
public:
	SslMgrCmdGetFile( SslMgrCmdMgr& cmdMgr, bool deleteFromSslMgrIfSuccess );

	void						setFileName( const char * fileName );
	std::string&				getFileName( void );
	std::string&				getDownloadCompleteFileName( void );
	uint32_t							getFileCheckSum( void );

	virtual void				enterState( void );
	virtual void				exitState( void );

	virtual int					getDataToWriteToSslMgrServer( char * buf, int bufLen );
	virtual void				handleDataFromSslMgrServer( char * data, int dataLen );
	virtual delayState_e		handleDelays( void );
	bool						deleteFromSslMgrIfSuccess( void )			{ return m_bDeleteFromSslMgrIfSuccess; }

protected:
	bool						writeSslStreamToFile( char * data, int dataLen );
	bool						timedOut( void );
	void						doFileWriteError( void );
	void						checkForFileLen( void );
	void						checkForCheckSum( void );
	void						startFileReceive( void );
	void						checkForFileComplete( void );
	bool						isFileFinishedDownloading( void );
	void						finishFileReceive( void );
	bool						verifyChecksum( void );

	//=== vars ===//
	ESslMgrCmdGetFileState		m_eCmdState;
	std::string					m_strFileName;

	uint32_t							m_u32FileLength;
	uint32_t							m_u32FileCrc;
	std::string					m_strDownloadFileName;
	FILE *						m_hFile;
	uint32_t							m_u32FileOffs;
	bool						m_bDeleteFromSslMgrIfSuccess;
};

#endif // SSLMGR_CMD_GET_FILE_H

