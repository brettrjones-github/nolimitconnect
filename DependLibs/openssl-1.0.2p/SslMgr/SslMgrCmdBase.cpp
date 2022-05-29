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
#include "SslWrapper.h"

#include <CoreLib/VxParse.h>

#include <ctype.h>

//============================================================================
SslMgrCmdBase::SslMgrCmdBase( SslMgrCmdMgr& cmdMgr )
: m_eCmdType( eSslMgrCmdUnknown )
, m_CmdMgr( cmdMgr )
, m_SslWrapper( cmdMgr.getSslWrapper() )
, m_FileUtil( m_SslWrapper.getFileUtil() )
, m_bIsCmdComplete( false )
, m_iAccumIdx( 0 )
, m_ErrorCode( CMD_ERR_NONE )
{
	m_as8AccumBuf[0] = 0;
}

//============================================================================
SslMgrCmdBase::~SslMgrCmdBase()
{
}

//============================================================================
void SslMgrCmdBase::enterState( void )
{
	LogMsg( 0, "SslMgrCmd %s enterState\n", describeSslMgrCmdType( getCmdType() ) );
}

//============================================================================
int SslMgrCmdBase::getDataToWriteToSslMgrServer( char * buf, int bufLen )
{
	return 0;
}

//============================================================================
void SslMgrCmdBase::handleDataFromSslMgrServer( char * buf, int bufLen )
{
}

//============================================================================
delayState_e SslMgrCmdBase::handleDelays( void )
{
	return( DS_NOT_WAITING );
}

//============================================================================
void SslMgrCmdBase::setErrorMsg( const char * msg, ... )
{
	char as8Buf[ 1024 ];
	va_list argList;
	va_start(argList, msg);
	vsnprintf( as8Buf, sizeof( as8Buf ), msg, argList );
	as8Buf[sizeof( as8Buf ) - 1] = 0;
	va_end(argList);

	m_strErrorMsg = as8Buf;
}

//============================================================================
void SslMgrCmdBase::doTimeoutError( const char * timeoutErrMsg )
{
	completeCommandWithError( CMD_ERR_CMD_TIMEOUT, timeoutErrMsg );
}

//============================================================================
void SslMgrCmdBase::completeCommandWithError( int errCode, const char * msg, ... )
{
	char as8Buf[ 1024 ];
	va_list argList;
	va_start(argList, msg);
	vsnprintf( as8Buf, sizeof( as8Buf ), msg, argList );
	as8Buf[sizeof( as8Buf ) - 1] = 0;
	va_end(argList);

	m_strErrorMsg = as8Buf;
	setErrorCode( errCode );
	setIsCmdComplete( true );
}

//============================================================================
void SslMgrCmdBase::setErrorCode( int errCode )
{
	m_ErrorCode = errCode;
}
//============================================================================
int SslMgrCmdBase::getErrorCode( void )
{
	return m_ErrorCode;
}


//============================================================================
std::string& SslMgrCmdBase::getErrorMsg( void )
{
	return m_strErrorMsg;
}

//============================================================================
bool SslMgrCmdBase::hasError( void )
{
	return m_strErrorMsg.length() ? true : false;
}

//============================================================================
const char * SslMgrCmdBase::describeSslMgrCmdType( ESslMgrCmdType cmdNum )
{
	switch( cmdNum )
	{
	case eSslMgrCmdIdle:
		return "SslMgr Cmd Idle ";
	case eSslMgrCmdLogOn:
		return "SslMgr Cmd Log On ";
	case eSslMgrCmdLogOff:
		return "SslMgr Cmd Log Off ";
	case eSslMgrCmdSendFile:
		return "SslMgr Cmd Send File ";
	case eSslMgrCmdListFiles:
		return "SslMgr Cmd List Files ";
	case eSslMgrCmdGetFile:
		return "SslMgr Cmd Get File ";
	case eSslMgrCmdSendText:
		return "SslMgr Cmd Send Text ";
	case eSslMgrCmdError:
		return "SslMgr Cmd Error ";
	default:
		return "SslMgr Cmd Unknown ";
	}
}

//============================================================================
bool SslMgrCmdBase::accumulateData( char * data, int dataLen )
{
	if( dataLen <= 0 )
	{
		LogMsg( LOG_ERROR, "SslMgrCmdBase:accumulateData: invalid dataLen %d\n", dataLen );
		return false;
	}
	if( dataLen >= ( (int)sizeof( m_as8AccumBuf ) - m_iAccumIdx ) )
	{
		LogMsg( LOG_ERROR, "SslMgrCmdBase:accumulateData: dataLen %d to large to fit buffer len %d\n", dataLen, ( (int)sizeof( m_as8AccumBuf ) - m_iAccumIdx ) );
		return false;
	}

	memcpy( &m_as8AccumBuf[ m_iAccumIdx ], data, dataLen );
	m_iAccumIdx += dataLen;
	m_as8AccumBuf[ m_iAccumIdx ] = 0;
	LogMsg( LOG_INFO, "SslMgrCmdBase:Accumulated %s\n", m_as8AccumBuf );
	return true;
}

//============================================================================
bool SslMgrCmdBase::accumulateOk( char * data, int dataLen )
{
	accumulateData( data, dataLen );
	
	if( ( 5 <= m_iAccumIdx )
		&& ( 0 == strncmp( "+OK\r\n", m_as8AccumBuf, 5 ) ) )
	{
		return true;
	}

	return false;
}

//============================================================================
bool SslMgrCmdBase::clearOkFromAccumulator( void )
{
	if( ( 5 <= m_iAccumIdx )
		&& ( 0 == strncmp( "+OK\r\n", m_as8AccumBuf, 5 ) ) )
	{
		strcpy( m_as8AccumBuf, &m_as8AccumBuf[5] );
		m_iAccumIdx -= 5;
		return true;
	}
	return false;
}

//============================================================================
bool SslMgrCmdBase::getAccumulatedCmd( std::string& retCmd )
{
	bool gotCmd = false;
	if( m_iAccumIdx > 0 )
	{
		char * pCmdEnd = strstr( m_as8AccumBuf, "\r\n" );
		if( pCmdEnd )
		{
			pCmdEnd += 2;
			char cSaveChar = *pCmdEnd;
			*pCmdEnd = 0;
			retCmd = m_as8AccumBuf;
			*pCmdEnd = cSaveChar;
			removeFromFrontOfDataAccum( retCmd.length() );
			gotCmd = true;
		}
	}
	return gotCmd;
}

//============================================================================
void SslMgrCmdBase::clearDataAccum( void )
{
	m_as8AccumBuf[ 0 ] = 0;
	m_iAccumIdx = 0;
}

//============================================================================
void SslMgrCmdBase::LogMsg( unsigned long u32LogMsgType, const char * logMsg, ... )
{
	char as8Buf[ 1024 ];
	va_list argList;
	va_start(argList, logMsg);
	vsnprintf( as8Buf, sizeof( as8Buf ), logMsg, argList );
	as8Buf[sizeof( as8Buf ) - 1] = 0;
	va_end(argList);

	m_CmdMgr.LogMsg( u32LogMsgType, as8Buf );
}

//============================================================================
bool SslMgrCmdBase::checkAccumForAsciiDecimalNum( int& retDecNum, int& retLenOfParse, int startOffs )
{
	bool foundAsciiNum = false;
	bool foundAtLeastOneDigit = false;

	int iCurIdx = startOffs;
	iCurIdx += countWhitespace( iCurIdx );

	int iDecNumIdx = iCurIdx; // where decimal number should begin

	while( iCurIdx < m_iAccumIdx )
	{
		if( false == isdigit( m_as8AccumBuf[ iCurIdx ] ) )
		{
			if( isWhiteSpace( m_as8AccumBuf[ iCurIdx ] )
				&& foundAtLeastOneDigit )
			{
				retLenOfParse = iCurIdx - iDecNumIdx;
				retDecNum = atoi( &m_as8AccumBuf[ iDecNumIdx ] );
				return true;
			}
			else
			{
				// something else not digit and not whitespace
				return false;
			}
		}
		else
		{
			foundAtLeastOneDigit = true;
		}
		iCurIdx++;
	}
	return foundAsciiNum;
}

//============================================================================
int SslMgrCmdBase::countWhitespace( int curIdx )
{
	int whitespaceCnt = 0;
	while( curIdx < m_iAccumIdx )
	{
		if( isWhiteSpace( m_as8AccumBuf[ curIdx ] ) )
		{
			whitespaceCnt++;
			curIdx++;
		}
		else
		{
			break;
		}
	}
	return whitespaceCnt;
}

//============================================================================
void SslMgrCmdBase::removeFromFrontOfDataAccum( int byteCnt )
{
	if( byteCnt < m_iAccumIdx )
	{
		memcpy( m_as8AccumBuf, &m_as8AccumBuf[ byteCnt ], m_iAccumIdx - byteCnt );
		m_iAccumIdx -= byteCnt;
		m_as8AccumBuf[ m_iAccumIdx ] = 0;
	}
	else
	{
		m_iAccumIdx = 0;
	}
}
