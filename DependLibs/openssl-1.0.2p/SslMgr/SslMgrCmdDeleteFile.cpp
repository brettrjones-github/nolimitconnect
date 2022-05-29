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

#include "SslMgrCmdDeleteFile.h"
#include "SslMgrCmdMgr.h"
#include "SslWrapper.h"

#include <CoreLib/VxDebug.h>

namespace
{
	float SSLMGR_CMD_DELETE_FILE_TIMEOUT_MS		=	3000.0f;
}

//============================================================================
SslMgrCmdDeleteFile::SslMgrCmdDeleteFile( SslMgrCmdMgr& cmdMgr, const char * fileName, uint32_t u32CheckSum )
: SslMgrCmdBase( cmdMgr )
, m_eCmdState( eCmdDeleteFileStateWaitSendCmd )
, m_strFileName( fileName )
, m_u32FileCrc( u32CheckSum )
{
	setCmdType( eSslMgrCmdDeleteFile );
}

//============================================================================
void SslMgrCmdDeleteFile::enterState( void )
{
}

//============================================================================
int SslMgrCmdDeleteFile::getDataToWriteToSslMgrServer( char * buf, int bufLen )
{
	int retVal = 0;
	switch( m_eCmdState )
	{
	case eCmdDeleteFileStateWaitSendCmd:
		sprintf( buf, "DELE %s %8.8X\r\n", m_strFileName.c_str(), m_u32FileCrc );
		retVal = (int)strlen( buf );
		m_eCmdState = eCmdDeleteFileStateSendCmdResponse;
		m_Timer.startTimer( );
		break;

	case eCmdDeleteFileStateSendCmdResponse:
	case eCmdDeleteFileStateSuccess:
	case eCmdDeleteFileStateError:
	default:
		break;
	}

	return retVal;
}

//============================================================================
void SslMgrCmdDeleteFile::handleDataFromSslMgrServer( char * data, int dataLen )
{
	if( hasError() 
		|| ( 0 >= dataLen ) )
	{
		return;
	}

	switch( m_eCmdState )
	{
	case eCmdDeleteFileStateWaitSendCmd:
		LogMsg( LOG_ERROR, "SslMgrCmdDeleteFile: got %d bytes in state eCmdDeleteFileStateWaitSendCmd\n");
		break;

	case eCmdDeleteFileStateSendCmdResponse:
		if( accumulateOk( data, dataLen ) )
		{
			LogMsg( LOG_INFO, "SslMgrCmdDeleteFile: got ok eCmdDeleteFileStateSendCmdResponse\n");
			clearOkFromAccumulator();
			m_eCmdState = eCmdDeleteFileStateSuccess;
			setIsCmdComplete( true );
		}
		else
		{
			handleDelays();
		}

		break;


	case eCmdDeleteFileStateSuccess:
		LogMsg( LOG_ERROR, "SslMgrCmdDeleteFile: got %d bytes in state eCmdDeleteFileStateSuccess\n");
		break;

	case eCmdDeleteFileStateError:
	default:
		break;
	}
}

//============================================================================
void SslMgrCmdDeleteFile::updateState( void )
{
	handleDelays();
}

//============================================================================
delayState_e SslMgrCmdDeleteFile::handleDelays( void )
{
	delayState_e retVal = DS_WAITING;

	const char* timeoutStr = "";

	switch( m_eCmdState )
	{
	case eCmdDeleteFileStateSendCmdResponse:
		timeoutStr = "Timed out during eCmdDeleteFileStateSendCmdResponse OK Response";
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
bool SslMgrCmdDeleteFile::timedOut( void )
{
	return m_Timer.elapsedMs() > SSLMGR_CMD_DELETE_FILE_TIMEOUT_MS ? true : false;
}


