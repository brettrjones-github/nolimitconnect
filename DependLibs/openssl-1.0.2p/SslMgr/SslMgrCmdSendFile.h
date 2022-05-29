#ifndef SSLMGR_CMD_SEND_FILE_H
#define SSLMGR_CMD_SEND_FILE_H

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

enum ESslMgrCmdSendFileState
{
	eCmdSendFileStateWaitSendCmd,
	eCmdSendFileStateWaitOkSend,
	eCmdSendFileStateTxFileBytes,

	eCmdSendFileStateConfirmServerOk,

	eCmdSendFileStateSuccess,
	eCmdSendFileStateError
};

class SslMgrCmdSendFile : public SslMgrCmdBase
{
public:
	SslMgrCmdSendFile( SslMgrCmdMgr& cmdMgr );
    ~SslMgrCmdSendFile();

	void						setFileName( const char * fileName );
	std::string&				getFileName( void );

	virtual void				enterState( void );
	virtual void				exitState( void );

	virtual int					getDataToWriteToSslMgrServer( char * buf, int bufLen );
	virtual void				handleDataFromSslMgrServer( char * data, int dataLen );
	virtual delayState_e		handleDelays( void );

protected:
	void						beginFileSend( void );

	int							writeToSslStreamFromFile( char * sslBuf, int bufLen );
	bool						timedOut( void );
	void						doFileReadError( void );


	void						checkForFileComplete( void );
	bool						isFileFinishedUploading( void );
	void						finishFileSend( void );

	//=== vars ===//
	ESslMgrCmdSendFileState	m_eCmdState;
	std::string					m_strFileName;
	uint32_t							m_u32FileLength;
	uint32_t							m_u32FileCrc;

	FILE *						m_hFile;
	uint32_t							m_u32FileOffs;
	std::string					m_strJustFileName;

};

#endif // SSLMGR_CMD_SEND_FILE_H

