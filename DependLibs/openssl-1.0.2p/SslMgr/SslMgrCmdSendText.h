#ifndef SSLMGR_CMD_SEND_TEXT_H
#define SSLMGR_CMD_SEND_TEXT_H

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

enum ESslMgrCmdSendTextState
{
	eCmdSendTextStateWaitSendTextCmd,
	eCmdSendTextStateSendTextResponse,

	eCmdSendTextStateGetListSuccess,
	eCmdSendTextStateGetListError
};

class SslMgrCmdSendText : public SslMgrCmdBase
{
public:
	SslMgrCmdSendText( SslMgrCmdMgr& cmdMgr );

	void						setCmdText( const char * text );
	std::string&				getCmdText( void );

	virtual int					getDataToWriteToSslMgrServer( char * buf, int bufLen );
	virtual void				handleDataFromSslMgrServer( char * data, int dataLen );

protected:
	bool						timedOut( void );

	//=== vars ===//
	ESslMgrCmdSendTextState	m_eCmdState;
	std::string					m_strCmdText;
};

#endif // SSLMGR_CMD_SEND_TEXT_H

