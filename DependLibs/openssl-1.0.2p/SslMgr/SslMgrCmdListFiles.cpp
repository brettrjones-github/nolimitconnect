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

#include "SslMgrCmdListFiles.h"
#include "SslMgrCmdMgr.h"
#include "SslWrapper.h"

#include <CoreLib/VxDebug.h>

namespace
{
	float SSLMGR_CMD_LIST_FILES_TIMEOUT_MS		=	3000.0f;
}

//============================================================================
SslMgrCmdListFiles::SslMgrCmdListFiles( SslMgrCmdMgr& cmdMgr )
: SslMgrCmdBase( cmdMgr )
, m_eCmdState( eCmdListStateWaitSendCmd )
{
	setCmdType( eSslMgrCmdListFiles );
}

//============================================================================
int SslMgrCmdListFiles::getDataToWriteToSslMgrServer( char * buf, int bufLen )
{
	int retVal = 0;
	switch( m_eCmdState )
	{
	case eCmdListStateWaitSendCmd:
		strcpy( buf, "LIST\r\n" );
		retVal = (int)strlen( buf );
		m_eCmdState = eCmdListStateSendCmdResponse;
		m_Timer.startTimer( );
		break;

	case eCmdListStateSendCmdResponse:
	case eCmdListStateWaitFileList:
	case eCmdListStateSuccess:
	case eCmdListStateError:
	default:
		break;
	}

	return retVal;
}

//============================================================================
void SslMgrCmdListFiles::handleDataFromSslMgrServer( char * data, int dataLen )
{
	if( hasError() 
		|| ( 0 >= dataLen ) )
	{
		return;
	}

	switch( m_eCmdState )
	{
	case eCmdListStateWaitSendCmd:
		LogMsg( LOG_ERROR, "SslMgrCmdListFiles: got %d bytes in state eCmdListStateWaitGetListCmd\n");
		break;

	case eCmdListStateSendCmdResponse:
		if( accumulateOk( data, dataLen ) )
		{
			LogMsg( LOG_INFO, "SslMgrCmdListFiles: got ok file list\n");
			clearOkFromAccumulator();
			m_eCmdState = eCmdListStateWaitFileList;
			checkAccumForFileList();
			//setIsCmdComplete( true );
			m_Timer.startTimer( );
		}
		else
		{
			handleDelays();
		}

		break;

	case eCmdListStateWaitFileList:
		accumulateData( data, dataLen );
		if( checkAccumForFileList() )
		{
			setIsCmdComplete( true );
			m_eCmdState = eCmdListStateSuccess;
		}

		if( DS_TIME_EXCEEDED != handleDelays() )
		{
			m_Timer.startTimer( );
		}
		break;

	case eCmdListStateSuccess:
		LogMsg( LOG_ERROR, "SslMgrCmdListFiles: got %d bytes in state eCmdListStateGetListSuccess\n");
		break;

	case eCmdListStateError:
	default:
		break;
	}
}

//============================================================================
void SslMgrCmdListFiles::updateState( void )
{
	handleDelays();
}

//============================================================================
delayState_e SslMgrCmdListFiles::handleDelays( void )
{
	delayState_e retVal = DS_WAITING;

	const char* timeoutStr = "";

	switch( m_eCmdState )
	{
	case eCmdListStateSendCmdResponse:
		timeoutStr = "Timed out during Get File List OK Response";
		break;

	case eCmdListStateWaitFileList:
		timeoutStr = "Timed out during Get File List Response";
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
bool SslMgrCmdListFiles::timedOut( void )
{
	return m_Timer.elapsedMs() > SSLMGR_CMD_LIST_FILES_TIMEOUT_MS ? true : false;
}

//============================================================================
bool SslMgrCmdListFiles::checkAccumForFileList( void )
{
	std::string cmdRxed;
	bool cmdFound = getAccumulatedCmd( cmdRxed );
	while( cmdFound )
	{
		if( 3 == cmdRxed.length()
			&& '.' == cmdRxed[0] )
		{
			// end of file list
			LogMsg( LOG_INFO, "End Of File List Found\n" );
			return true;
		}
		else
		{
			m_CmdMgr.getSslWrapper().getSslCallback().sslMgrListFile( cmdRxed.c_str() );
		}

		cmdFound = getAccumulatedCmd( cmdRxed );
	}
	return false;
}
