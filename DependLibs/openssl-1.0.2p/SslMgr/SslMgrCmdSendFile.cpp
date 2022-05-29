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

#include "SslMgrCmdSendFile.h"
#include "SslMgrCmdMgr.h"
#include "SslWrapper.h"

#include <CoreLib/VxDebug.h>
#include <CoreLib/VxGlobals.h>
#include <CoreLib/VxFileCrc32.h>
#include <CoreLib/VxFileUtil.h>

namespace
{
//	float SSLMGR_CMD_SEND_FILE_TIMEOUT_MS		=	3000.0f;
//    float SSLMGR_FILE_TRANSMIT_TIMEOUT_MS		=	5000.0f;
    float SSLMGR_CMD_SEND_FILE_TIMEOUT_MS		=	6000.0f;
    float SSLMGR_FILE_TRANSMIT_TIMEOUT_MS		=	7000.0f;
}

//============================================================================
SslMgrCmdSendFile::SslMgrCmdSendFile( SslMgrCmdMgr& cmdMgr )
: SslMgrCmdBase( cmdMgr )
, m_eCmdState( eCmdSendFileStateWaitSendCmd )
, m_hFile( 0 )
, m_u32FileOffs( 0 )
, m_u32FileLength( 0 )
{
    printf("SslMgrCmdSendFile constructor\n");
	setCmdType( eSslMgrCmdSendFile );
}

SslMgrCmdSendFile::~SslMgrCmdSendFile()
{
    printf("SslMgrCmdSendFile Desconstructor\n");
}

//============================================================================
void SslMgrCmdSendFile::setFileName( const char * fileName )
{
	m_strFileName = fileName;
}

//============================================================================
std::string& SslMgrCmdSendFile::getFileName( void )
{
	return m_strFileName;
}

//============================================================================
void SslMgrCmdSendFile::enterState( void )
{

}

//============================================================================
void SslMgrCmdSendFile::exitState( void )
{

}

//============================================================================
int SslMgrCmdSendFile::getDataToWriteToSslMgrServer( char * buf, int bufLen )
{
    static ESslMgrCmdSendFileState state = eCmdSendFileStateWaitSendCmd;
    
    if (m_eCmdState != state) {
        printf("SslMgrCmdSendFile::getDataToWriteToSslMgrServer: %d, %s\n", m_eCmdState, m_strFileName.c_str());
        state = m_eCmdState;
    }

	int retVal = 0;
	switch( m_eCmdState )
	{
	case eCmdSendFileStateWaitSendCmd:
		m_u32FileLength = (uint32_t)m_FileUtil.getFileLen( m_strFileName.c_str() );
		if( m_u32FileLength )
		{
			VxCalculateFileCheckSum( m_strFileName.c_str(), m_u32FileCrc );
		
			m_FileUtil.getJustFileName( m_strFileName.c_str(), m_strJustFileName );

			sprintf( buf, "SEND %s %u %8.8X\r\n", m_strJustFileName.c_str(), m_u32FileLength, m_u32FileCrc );
			retVal = (int)strlen( buf );
			m_eCmdState = eCmdSendFileStateWaitOkSend;
			LogMsg( LOG_INFO, buf );
			m_Timer.startTimer( );
		}
		else
		{
			completeCommandWithError( CMD_ERR_SEND_FILE_IO, "Failed getting length of file %s\n", m_strFileName.c_str() );
			LogMsg( LOG_ERROR, m_strErrorMsg.c_str() );
		}
		break;

	case eCmdSendFileStateTxFileBytes:
		retVal = writeToSslStreamFromFile( buf, bufLen  );
        if (0 == retVal) {
            completeCommandWithError( CMD_ERR_SEND_FILE_IO, "Failed writting to ssl stream %s\n", m_strFileName.c_str() );
            LogMsg( LOG_ERROR, m_strErrorMsg.c_str() );
        }
		//{
		//	doFileReadError();
		//	break;
		//}
		m_Timer.startTimer( );
		checkForFileComplete();
		break;

	default:
		break;

	}

	return retVal;
}

//============================================================================
void SslMgrCmdSendFile::handleDataFromSslMgrServer( char * data, int dataLen )
{
    static ESslMgrCmdSendFileState state = eCmdSendFileStateWaitSendCmd;
    
	if( hasError() 
		|| ( 0 >= dataLen ) )
	{
		return;
	}
    
    if (m_eCmdState != state) {
        printf("SslMgrCmdSendFile::handleDataFromSslMgrServer: %d, %s\n", m_eCmdState, m_strFileName.c_str());
        state = m_eCmdState;
    }

	switch( m_eCmdState )
	{
	case eCmdSendFileStateWaitSendCmd:
		LogMsg( LOG_ERROR, "SslMgrCmdSendFile: got %d bytes in state eCmdListStateWaitGetListCmd\n");
		break;

	case eCmdSendFileStateWaitOkSend:
		{
			accumulateData( data, dataLen );
			std::string cmdRxed;
			bool cmdFound = getAccumulatedCmd( cmdRxed );
			if( cmdFound )
			{
				if( ( 13 <= cmdRxed.size() ) 
					&& ( 0 == strncmp( "+OK send file", cmdRxed.c_str(), 13 ) ) )
				{
					LogMsg( LOG_INFO, "SslMgrCmdSendFile: got ok send file %s\n", m_strFileName.c_str() );
					m_eCmdState = eCmdSendFileStateTxFileBytes;
					m_Timer.startTimer( );
					beginFileSend();
				}
				else
				{
					completeCommandWithError( CMD_ERR_SEND_FILE_BAD_RESPONSE, "Failed send file response %s\n", m_strFileName.c_str() );
				}
			}
			else 
			{
				handleDelays();
			}

		}
		break;


	case eCmdSendFileStateConfirmServerOk:
		{
			accumulateData( data, dataLen );
			std::string cmdRxed;
			bool cmdFound = getAccumulatedCmd( cmdRxed );
			if( cmdFound )
			{
				if( ( 3 <= cmdRxed.size() ) 
					&& ( 0 == strncmp( "+OK ", cmdRxed.c_str(), 3 ) ) )
				{
					LogMsg( LOG_INFO, "SslMgrCmdSendFile: got ok send file %s\n", m_strFileName.c_str() );
					m_eCmdState = eCmdSendFileStateSuccess;
					setIsCmdComplete( true );
				}
				else
				{
					completeCommandWithError( CMD_ERR_SEND_FILE_BAD_RESPONSE, "confirm file send %s\n", m_strFileName.c_str() );
				}
			}
			else 
			{
				handleDelays();
			}

		}
		break;

	case eCmdSendFileStateSuccess:
		LogMsg( LOG_ERROR, "SslMgrCmdListFiles: got %d bytes in state eCmdSendFileStateSuccess\n");
		break;

	case eCmdSendFileStateTxFileBytes:
        if (m_Timer.elapsedMs() > SSLMGR_FILE_TRANSMIT_TIMEOUT_MS) {
            doTimeoutError( "TimeOut Error: Transmitting file");
        }
        break;
            
	case eCmdSendFileStateError:
	default:
		break;
	}
}

//============================================================================
delayState_e SslMgrCmdSendFile::handleDelays( void )
{
	delayState_e retVal = DS_WAITING;

	const char* timeoutStr = "";

	switch( m_eCmdState )
	{
	case eCmdSendFileStateWaitOkSend:
		timeoutStr = "Timed out during Get File Response";
		break;

	case eCmdSendFileStateConfirmServerOk:
		timeoutStr = "Timed out during confirm file send";
		break;

	default:
		retVal = DS_NOT_WAITING;
	}

	if( DS_WAITING == retVal )
	{

		if( timedOut() )
		{
			retVal = DS_TIME_EXCEEDED;
            printf("doTimeoutError: %s\n", timeoutStr);
			doTimeoutError( timeoutStr );
		}

	}

	return( retVal );
}

//============================================================================
bool SslMgrCmdSendFile::timedOut( void )
{
	return m_Timer.elapsedMs() > SSLMGR_CMD_SEND_FILE_TIMEOUT_MS ? true : false;
}

//============================================================================
void SslMgrCmdSendFile::beginFileSend( void )
{
	m_hFile = fopen( m_strFileName.c_str(), "rb" ); 
	if( NULL == m_hFile )
	{
		completeCommandWithError( CMD_ERR_SEND_FILE_IO, "send file %s could not open file to read\n", m_strFileName.c_str() );
		LogMsg( LOG_ERROR, m_strErrorMsg.c_str() );
		return;
	}

	m_eCmdState = eCmdSendFileStateTxFileBytes;

	m_u32FileOffs = 0;

	m_CmdMgr.getSslWrapper().getSslCallback().sslMgrFileSendBegin( m_strFileName.c_str() );

	checkForFileComplete();
}

//============================================================================
int SslMgrCmdSendFile::writeToSslStreamFromFile( char * data, int dataLen )
{
	int bytesToRead =  (unsigned int)dataLen < m_u32FileLength - m_u32FileOffs ? dataLen : m_u32FileLength - m_u32FileOffs;
	if( bytesToRead  > 0 )
	{

		// some data was rxed into accumulator that was meant for file.. write to file now
		int bytesActuallyRead = fread(	data,
										1,
										bytesToRead,
										m_hFile );
		if( bytesActuallyRead != bytesToRead )
		{
			completeCommandWithError( CMD_ERR_SEND_FILE_IO, "failed read %d bytes from file %s to ssl\n", dataLen, m_strFileName.c_str() );
			LogMsg( LOG_ERROR, m_strErrorMsg.c_str() );
		}
		else
		{
			m_u32FileOffs += bytesActuallyRead;
			return bytesActuallyRead;
		}

	}

	return 0;
}

//============================================================================
void SslMgrCmdSendFile::checkForFileComplete( void )
{
    static uint32_t fileOffset = 0;
    if (fileOffset == m_u32FileLength) {
        printf("checkForFileComplete: file offset is still %d\n", m_u32FileLength);
    }
    else
    {
        fileOffset = m_u32FileLength;
    }
	if( m_u32FileLength )
	{
		m_CmdMgr.getSslWrapper().getSslCallback().sslMgrFileSendProgress( m_strFileName.c_str(), (int)(((float)m_u32FileOffs * 100.0f) / (float)m_u32FileLength) );

		if( isFileFinishedUploading() )
		{
			LogMsg( LOG_INFO, "All bytes %d of file %s txed\n", m_u32FileLength, m_strFileName.c_str()  );
			finishFileSend();
		}
	}
	else
	{
		LogMsg( LOG_INFO, "checkForFileComplete 0 file length\n"  );
	}
}

//============================================================================
bool SslMgrCmdSendFile::isFileFinishedUploading( void )
{
	if( m_u32FileOffs >= m_u32FileLength )
	{
		return true;
	}
	return false;
}

//============================================================================
void SslMgrCmdSendFile::finishFileSend( void )
{
	if( m_hFile )
	{
		fclose( m_hFile );
		m_hFile = NULL;
	}

	m_CmdMgr.getSslWrapper().getSslCallback().sslMgrFileSendProgress( m_strFileName.c_str(), (int)(((float)m_u32FileOffs * 100.0f) / (float)m_u32FileLength) );
	LogMsg( LOG_INFO, "SslMgrCmdSendFile: file %s sent len %d\n", m_strFileName.c_str(), m_u32FileLength );
	m_CmdMgr.getSslWrapper().getSslCallback().sslMgrFileSendComplete( m_strFileName.c_str() );

	m_eCmdState = eCmdSendFileStateConfirmServerOk;
	m_Timer.startTimer( );
}
