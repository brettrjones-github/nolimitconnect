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

#include "SslMgrCmdError.h"
#include "SslWrapper.h"

//============================================================================
SslMgrCmdError::SslMgrCmdError(SslMgrCmdMgr& cmdMgr )
: SslMgrCmdBase( cmdMgr )
{
	setCmdType( eSslMgrCmdIdle );
}

//============================================================================
void SslMgrCmdError::enterState( void )
{
	if( m_strErrorText.length() )
	{
		m_SslWrapper.getSslCallback().sslMgrCmdError( m_eCmdTypeWithError, m_ErrorCode, m_strErrorText.c_str() );
	}
}

//============================================================================
void SslMgrCmdError::setErrorText( const char * errorText )
{
	m_strErrorText = errorText;
}

//============================================================================
void SslMgrCmdError::setCmdTypeWithError( ESslMgrCmdType eCmdType )
{
	m_eCmdTypeWithError = eCmdType;
}
