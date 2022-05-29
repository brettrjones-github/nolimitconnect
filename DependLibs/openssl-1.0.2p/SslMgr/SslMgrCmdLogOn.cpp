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

#include "SslMgrCmdLogOn.h"

#include <CoreLib/VxDebug.h>
#include <CoreLib/AppErr.h>
#include <string.h>

namespace
{
	std::string SSLMGR_LOGIN_INFO_REQ		= "INFO 1.0 ABC\r\n";
    float SSLMGR_LOGIN_TIMEOUT_MS			=	6000.0f;
}

//============================================================================
SslMgrCmdLogOn::SslMgrCmdLogOn( SslMgrCmdMgr& cmdMgr )
: SslMgrCmdBase( cmdMgr )
, m_eSvrLoginState( eSvrLoginStateWaitGetUser )
{
	setCmdType( eSslMgrCmdLogOn );
}

//============================================================================
void SslMgrCmdLogOn::enterState( void )
{
	//LogMsg( 0, "SslMgrCmdLogOn enterState User: %s Pwd: %s\n", m_strUserName.c_str(), m_strPassword.c_str() );
}

//============================================================================
void SslMgrCmdLogOn::setUserName( const char * userName )
{
	m_strUserName = userName;
}

//============================================================================
void SslMgrCmdLogOn::setPassword( const char * pass )
{
	m_strPassword = pass;
}

//============================================================================
int SslMgrCmdLogOn::getDataToWriteToSslMgrServer( char * buf, int bufLen )
{
	int retVal = 0;
	switch( m_eSvrLoginState )
	{
	case eSvrLoginStateWaitGetUser:
		strcpy( buf, "USER " );
		strcat( buf, m_strUserName.c_str() );
		retVal = (int)strlen( buf );
		m_eSvrLoginState = eSvrLoginStateGetUserResponse;
		m_Timer.startTimer( );
		break;

	case eSvrLoginStateWaitGetPass:
		strcpy( buf, "PASS " );
		strcat( buf, m_strPassword.c_str() );
		retVal = (int)strlen( buf );
		m_eSvrLoginState = eSvrLoginStateGetPassResponse;
		m_Timer.startTimer( );
		break;

	case eSvrLoginStateWaitGetInfo:
		strcpy( buf, SSLMGR_LOGIN_INFO_REQ.c_str() );
		retVal = (int)SSLMGR_LOGIN_INFO_REQ.size();
		m_eSvrLoginState = eSvrLoginStateGetInfoResponse;
		m_Timer.startTimer( );
		break;

	case eSvrLoginStateGetUserResponse:
	case eSvrLoginStateGetPassResponse:
	case eSvrLoginStateGetInfoResponse:
	case eSvrLoginStateSuccess:
	case eSvrLoginStateError:
	default:
		break;
	}

	return retVal;
}

//============================================================================
void SslMgrCmdLogOn::handleDataFromSslMgrServer( char * data, int dataLen )
{
	if( hasError() )
	{
		return;
	}

	switch( m_eSvrLoginState )
	{
	case eSvrLoginStateWaitGetUser:
		//LogMsg( LOG_ERROR, "SslMgrCmdLogOn: got %d bytes in state eSvrLoginStateWaitGetUser %s %s\n", m_strUserName.c_str(), m_strPassword.c_str());
		break;

	case eSvrLoginStateGetUserResponse:
		if( accumulateOk( data, dataLen ) )
		{
			//LogMsg( LOG_ERROR, "SslMgrCmdLogOn: got ok user name %s %s\n", m_strUserName.c_str(), m_strPassword.c_str());
			clearDataAccum();
			m_eSvrLoginState = eSvrLoginStateWaitGetPass;
		}
		else 
		{
			handleDelays();
		}
		break;

	case eSvrLoginStateWaitGetPass:
		LogMsg( LOG_ERROR, "SslMgrCmdLogOn: got %d bytes in state eSvrLoginStateWaitGetPass %s %s\n", m_strUserName.c_str(), m_strPassword.c_str());
		break;

	case eSvrLoginStateGetPassResponse:
		if( accumulateOk( data, dataLen ) )
		{
			//LogMsg( LOG_ERROR, "SslMgrCmdLogOn: got ok password %s %s\n", m_strUserName.c_str(), m_strPassword.c_str() );
			clearDataAccum();
			m_eSvrLoginState = eSvrLoginStateWaitGetInfo;
		}
		else 
		{
			handleDelays();
		}
		break;

	case eSvrLoginStateWaitGetInfo:
		LogMsg( LOG_ERROR, "SslMgrCmdLogOn: got %d bytes in state eSvrLoginStateWaitGetInfo %s %s\n", m_strUserName.c_str(), m_strPassword.c_str());
		break;

	case eSvrLoginStateGetInfoResponse:
		accumulateData( data, dataLen );
		if( accumulatedOkPlusAssigned() )
		{
			LogMsg( LOG_ERROR, "SslMgrCmdLogOn: got ok assigned log on %s %s\n", m_strUserName.c_str(), m_strPassword.c_str() );
			clearDataAccum();
			LogMsg( LOG_INFO, "SslMgr Log On Success Name: %s Pass: %s", m_strUserName.c_str(), m_strPassword.c_str() );
			m_eSvrLoginState = eSvrLoginStateSuccess;
			setIsCmdComplete( true );
		}
		else if( accumulatedOkPlusUnassigned() )
		{
			LogMsg( LOG_ERROR, "SslMgrCmdLogOn: unassigned log on %s %s", m_strUserName.c_str(), m_strPassword.c_str() );
			clearDataAccum();
			completeCommandWithError( eAppErrSslMgrUnassignedLogOn, "SslMgr Acct UNASSIGNED Name:%s Pwd:%s\n", m_strUserName.c_str(), m_strPassword.c_str() );
			AppErr( eAppErrSslMgrUnassignedLogOn, "%s", m_strUserName.c_str());
		}
		else 
		{
			handleDelays();
		}
		break;

	case eSvrLoginStateSuccess:
		LogMsg( LOG_ERROR, "SslMgrCmdLogOn: got %d bytes in state eSvrLoginStateSuccess %s %s\n", m_strUserName.c_str(), m_strPassword.c_str());
		break;

	case eSvrLoginStateError:
	default:
		break;
	}
}

//============================================================================
delayState_e SslMgrCmdLogOn::handleDelays( void )
{
	delayState_e retVal = DS_WAITING;

	const char* timeoutStr = "";

	switch( m_eSvrLoginState )
	{
	case eSvrLoginStateGetUserResponse:
		timeoutStr = "Timed out during Get User Response";
		break;

	case eSvrLoginStateGetPassResponse:
		timeoutStr = "Timed out during Get Password Response";
		break;

	// case eSvrLoginStateGetInfoResponse:
	// 	timeoutStr = "Timed out during Get Info Response";
	// 	break;

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
bool SslMgrCmdLogOn::timedOut( void )
{
	return m_Timer.elapsedMs() > SSLMGR_LOGIN_TIMEOUT_MS ? true : false;
}

//============================================================================
bool SslMgrCmdLogOn::accumulatedOkPlusAssigned( void )
{
	if( strstr( m_as8AccumBuf, "+OK" ) )
	{
		if( strstr( m_as8AccumBuf, " assigned" ) )
		{
			return true;
		}
	}
	return false;
}

//============================================================================
bool SslMgrCmdLogOn::accumulatedOkPlusUnassigned( void )
{
	if( strstr( m_as8AccumBuf, "+OK" ) )
	{
		if( strstr( m_as8AccumBuf, " unassigned" ) )
		{
			return true;
		}
	}
	return false;
}
