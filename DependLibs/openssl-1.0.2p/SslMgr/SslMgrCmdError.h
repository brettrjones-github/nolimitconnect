#ifndef SSLMGR_CMD_ERROR_H
#define SSLMGR_CMD_ERROR_H

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

class SslMgrCmdError : public SslMgrCmdBase
{
public:
	SslMgrCmdError( SslMgrCmdMgr& cmdMgr );

	virtual void				enterState( void );
	virtual bool				getIsCmdComplete( void )				{ return true; }

	virtual void				setErrorText( const char * errorText );
	virtual void				setErrorCode( int errCode )				{ m_ErrorCode = errCode; }
	virtual void				setCmdTypeWithError( ESslMgrCmdType eCmdType );

	//=== vars ===//
	std::string					m_strErrorText;
	ESslMgrCmdType				m_eCmdTypeWithError;
	int							m_ErrorCode;
};

#endif // SSLMGR_CMD_ERROR_H

