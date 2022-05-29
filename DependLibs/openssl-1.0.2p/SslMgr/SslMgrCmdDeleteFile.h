#ifndef SSLMGR_CMD_DELETE_FILE_H
#define SSLMGR_CMD_DELETE_FILE_H

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

enum ESslMgrCmdDeleteFileState
{
	eCmdDeleteFileStateWaitSendCmd,
	eCmdDeleteFileStateSendCmdResponse,

	eCmdDeleteFileStateSuccess,
	eCmdDeleteFileStateError
};

class SslMgrCmdDeleteFile : public SslMgrCmdBase
{
public:
	SslMgrCmdDeleteFile( SslMgrCmdMgr& cmdMgr, const char * fileName, uint32_t u32CheckSum );

	std::string&				getFileName( void );

	virtual void				enterState( void );
	virtual void				updateState( void );

	virtual int					getDataToWriteToSslMgrServer( char * buf, int bufLen );
	virtual void				handleDataFromSslMgrServer( char * data, int dataLen );
	virtual delayState_e		handleDelays( void );

protected:
	bool						timedOut( void );

	//=== vars ===//
	ESslMgrCmdDeleteFileState	m_eCmdState;
	std::string					m_strFileName;
	uint32_t							m_u32FileCrc;
	std::string					m_strDeleteFileCmd;

};

#endif // SSLMGR_CMD_DELETE_FILE_H

