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

#include "SslMgrCmdSendText.h"
#include <CoreLib/VxDebug.h>
#include <string.h>

namespace
{
	float SSLMGR_CMD_SEND_TEXT_TIMEOUT_MS		=	3000.0f;
}

//============================================================================
SslMgrCmdSendText::SslMgrCmdSendText( SslMgrCmdMgr& cmdMgr )
: SslMgrCmdBase( cmdMgr )
, m_eCmdState( eCmdSendTextStateWaitSendTextCmd )
{
	setCmdType( eSslMgrCmdSendText );
}

//============================================================================
void SslMgrCmdSendText::setCmdText( const char * text )
{
	m_strCmdText = text;
}

//============================================================================
std::string& SslMgrCmdSendText::getCmdText( void )
{
	return m_strCmdText;
}

//============================================================================
int SslMgrCmdSendText::getDataToWriteToSslMgrServer( char * buf, int bufLen )
{
	int retVal = 0;
	switch( m_eCmdState )
	{
	case eCmdSendTextStateWaitSendTextCmd:
		if( m_strCmdText.length() )
		{
			strcpy( buf, m_strCmdText.c_str() );
			strcat( buf, "\r\n" );
			retVal = (int)strlen( buf );
		}
		m_eCmdState = eCmdSendTextStateSendTextResponse;
		m_Timer.startTimer();
		break;

	case eCmdSendTextStateSendTextResponse:
	case eCmdSendTextStateGetListSuccess:
	case eCmdSendTextStateGetListError:
	default:
		break;
	}

	return retVal;
}

//============================================================================
void SslMgrCmdSendText::handleDataFromSslMgrServer( char * data, int dataLen )
{
	if( hasError() )
	{
		return;
	}

	switch( m_eCmdState )
	{
	case eCmdSendTextStateWaitSendTextCmd:
		LogMsg( LOG_ERROR, "SslMgrCmdSendText: got %d bytes in state eCmdSendTextStateWaitSendTextCmd\n");
		break;

	case eCmdSendTextStateSendTextResponse:
		if( accumulateOk( data, dataLen ) )
		{
			LogMsg( LOG_INFO, "SslMgrCmdSendText: got ok\n");
			clearDataAccum();
			m_eCmdState = eCmdSendTextStateGetListSuccess;
			setIsCmdComplete( true );
		}
		else if( timedOut() )
		{
			completeCommandWithError( CMD_ERR_CMD_TIMEOUT, "Timed out during Get File List Response" );
		}
		break;


	case eCmdSendTextStateGetListSuccess:
		LogMsg( LOG_ERROR, "SslMgrCmdSendText: got %d bytes in state eCmdSendTextStateGetListSuccess\n");
		break;

	case eCmdSendTextStateGetListError:
	default:
		break;
	}
}

//============================================================================
bool SslMgrCmdSendText::timedOut( void )
{
	return m_Timer.elapsedMs() > SSLMGR_CMD_SEND_TEXT_TIMEOUT_MS ? true : false;
}
