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

#include "SslMgrCmdLogOff.h"
#include "SslMgrCmdMgr.h"
#include "SslWrapper.h"

#include <CoreLib/VxDebug.h>

namespace
{
	float SSLMGR_CMD_LOG_OFF_TIMEOUT_MS		=	3000.0f;
}

//============================================================================
SslMgrCmdLogOff::SslMgrCmdLogOff( SslMgrCmdMgr& cmdMgr )
: SslMgrCmdBase( cmdMgr )
, m_eCmdState( eCmdLogOffStateWaitSendCmd )
{
	setCmdType( eSslMgrCmdLogOff );
}

//============================================================================
int SslMgrCmdLogOff::getDataToWriteToSslMgrServer( char * buf, int bufLen )
{
	int retVal = 0;
	switch( m_eCmdState )
	{
	case eCmdLogOffStateWaitSendCmd:
		strcpy( buf, "QUIT\r\n" );
		retVal = (int)strlen( buf );
		m_eCmdState = eCmdLogOffStateSendCmdResponse;
		m_Timer.startTimer( );
        printf("SslMgrCmdLogOff send log off\n");
		break;

	default:
		break;
	}

	return retVal;
}

//============================================================================
void SslMgrCmdLogOff::handleDataFromSslMgrServer( char * data, int dataLen )
{
	if( hasError() 
		|| ( 0 >= dataLen ) )
	{
		return;
	}

	switch( m_eCmdState )
	{
	case eCmdLogOffStateWaitSendCmd:
		LogMsg( LOG_ERROR, "SslMgrCmdLogOff: got %d bytes in state eCmdLogOffStateWaitGetListCmd\n");
		m_Timer.startTimer( );
		break;

	case eCmdLogOffStateSendCmdResponse:
		if( accumulateOk( data, dataLen ) )
		{
			//LogMsg( LOG_INFO, "SslMgrCmdLogOff: got ok\n");
			m_eCmdState = eCmdLogOffStateSuccess;
			setIsCmdComplete( true );
		}
		else
		{
			handleDelays();
		}

		break;

	case eCmdLogOffStateSuccess:
		LogMsg( LOG_INFO, "SslMgrCmdLogOff: got %d bytes in state eCmdLogOffStateSuccess\n");
		break;

	case eCmdLogOffStateError:
	default:
		break;
	}
}

//============================================================================
void SslMgrCmdLogOff::updateState( void )
{
	handleDelays();
}

//============================================================================
delayState_e SslMgrCmdLogOff::handleDelays( void )
{
	delayState_e retVal = DS_WAITING;

	const char* timeoutStr = "";

	switch( m_eCmdState )
	{
	case eCmdLogOffStateSendCmdResponse:
		timeoutStr = "Timed out during SslMgrCmdLogOff OK Response";
		break;

	default:
		retVal = DS_NOT_WAITING;
	}

	if( DS_WAITING == retVal )
	{
		if( timedOut() )
		{
			retVal = DS_TIME_EXCEEDED;
			doTimeoutError( timeoutStr );
		}
	}

	return( retVal );
}

//============================================================================
bool SslMgrCmdLogOff::timedOut( void )
{
	return m_Timer.elapsedMs() > SSLMGR_CMD_LOG_OFF_TIMEOUT_MS ? true : false;
}

