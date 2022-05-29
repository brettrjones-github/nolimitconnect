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

#include "SslMgrCmdGetFile.h"
#include "SslMgrCmdMgr.h"
#include "SslWrapper.h"

#include <CoreLib/VxDebug.h>
#include <CoreLib/VxGlobals.h>
#include <CoreLib/VxFileCrc32.h>
#include <CoreLib/VxFileUtil.h>
#include <CoreLib/AppErr.h>



namespace
{
	float SSLMGR_CMD_GET_FILE_TIMEOUT_MS		=	3000.0f;
}

//============================================================================
SslMgrCmdGetFile::SslMgrCmdGetFile( SslMgrCmdMgr& cmdMgr, bool deleteFromSslMgrIfSuccess )
: SslMgrCmdBase( cmdMgr )
, m_bDeleteFromSslMgrIfSuccess( deleteFromSslMgrIfSuccess )
, m_eCmdState( eCmdGetFileStateWaitSendCmd )
, m_u32FileCrc( 0 )
, m_hFile( 0 )
, m_u32FileOffs( 0 )
, m_u32FileLength( 0 )
{
	setCmdType( eSslMgrCmdGetFile );
}

//============================================================================
void SslMgrCmdGetFile::setFileName( const char * fileName )
{
	m_strFileName = fileName;
}

//============================================================================
std::string& SslMgrCmdGetFile::getFileName( void )
{
	return m_strFileName;
}

//============================================================================
std::string& SslMgrCmdGetFile::getDownloadCompleteFileName( void )
{
	return m_strDownloadFileName;
}

//============================================================================
uint32_t SslMgrCmdGetFile::getFileCheckSum( void )
{
	return m_u32FileCrc;
}

//============================================================================
void SslMgrCmdGetFile::enterState( void )
{
    LogMsg(LOG_DEBUG, "SslMgrCmdGetFile::enterState, file name: %s\n", m_strFileName.c_str());
}

//============================================================================
void SslMgrCmdGetFile::exitState( void )
{

}

//============================================================================
int SslMgrCmdGetFile::getDataToWriteToSslMgrServer( char * buf, int bufLen )
{
	int retVal = 0;
	switch( m_eCmdState )
	{
	case eCmdGetFileStateWaitSendCmd:
		strcpy( buf, "RETR " );
		strcat( buf, m_strFileName.c_str() );
		strcat( buf, "\r\n" );
		retVal = (int)strlen( buf );
		m_eCmdState = eCmdGetFileStateWaitOk;
		m_Timer.startTimer( );
        LogMsg(LOG_DEBUG, "SslMgrCmdGetFile::getDataToWriteToSslMgrServer, send request: %s\n", m_strFileName.c_str());
		break;

	case eCmdGetFileStateWaitOk:
	case eCmdGetFileStateWaitFileLen:
	case eCmdGetFileStateWaitCheckSum:
	case eCmdGetFileStateRxFileBytes:
	case eCmdGetFileStateSuccess:
	case eCmdGetFileStateError:
	default:
		break;
	}

	return retVal;
}

//============================================================================
void SslMgrCmdGetFile::handleDataFromSslMgrServer( char * data, int dataLen )
{
    LogMsg(LOG_DEBUG, "SslMgrCmdGetFile::handleDataFromSslMgrServer, data length: %d, has error: %d\n", dataLen, hasError());
	if( hasError() 
		|| ( 0 >= dataLen ) )
	{
		return;
	}

	switch( m_eCmdState )
	{
	case eCmdGetFileStateWaitSendCmd:
		LogMsg( LOG_ERROR, "SslMgrCmdGetFile: got %d bytes in state eCmdListStateWaitGetListCmd\n");
		break;

	case eCmdGetFileStateWaitOk:
		accumulateData( data, dataLen );
		if( ( 4 <= m_iAccumIdx )
			&& ( 0 == strncmp( "+OK ", m_as8AccumBuf, 4 ) ) )
		{
			LogMsg( LOG_INFO, "SslMgrCmdGetFile: got ok get file %s\n", m_strFileName.c_str() );
			removeFromFrontOfDataAccum( 4 );
			m_eCmdState = eCmdGetFileStateWaitFileLen;
			m_Timer.startTimer( );
			checkForFileLen();
		}
        else if ( ( 4 <= m_iAccumIdx )
                   && ( 0 == strncmp( "-ERR", m_as8AccumBuf, 4 ) ) )
        {
            m_CmdMgr.getSslWrapper().getSslCallback().sslMgrCmdError( eSslMgrCmdGetFile, eAppErrSslMgrFileMissing, "handleDataFromSslMgrServer: cSslMgr file is missing in sslMgr server\n");
            completeCommandWithError( CMD_ERR_GET_FILE_IO, "handleDataFromSslMgrServer: file %s not found in sslMgr server\n", m_strFileName.c_str() );
        }
		else
		{
			handleDelays();
		}
		break;

	case eCmdGetFileStateWaitFileLen:
		{
			accumulateData( data, dataLen );
			checkForFileLen();

			if( DS_TIME_EXCEEDED != handleDelays() )
			{
				m_Timer.startTimer( );
			}

		}
		break;

	case eCmdGetFileStateRxFileBytes:
		if( false == writeSslStreamToFile( data, dataLen  ) )
		{
			doFileWriteError();
			break;
		}
		m_Timer.startTimer( );
		checkForFileComplete();
		break;

	case eCmdGetFileStateSuccess:
		LogMsg( LOG_ERROR, "SslMgrCmdListFiles: got %d bytes in state eCmdGetFileStateSuccess\n");
		break;

	case eCmdGetFileStateWaitCheckSum:
	case eCmdGetFileStateError:
	default:
		break;
	}
}

//============================================================================
delayState_e SslMgrCmdGetFile::handleDelays( void )
{
	delayState_e retVal = DS_WAITING;

	const char* timeoutStr = "";

	switch( m_eCmdState )
	{
	case eCmdGetFileStateWaitOk:
		timeoutStr = "Timed out during Get File Response";
		break;

	case eCmdGetFileStateWaitFileLen:
		timeoutStr = "Timed out during Get File Len";
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
bool SslMgrCmdGetFile::writeSslStreamToFile( char * data, int dataLen )
{
	bool writeOk = false;
	// some data was rxed into accumulator that was meant for file.. write to file now
	int bytesWritten = fwrite(	data,
								1,
								dataLen,
								m_hFile );
	if( bytesWritten != dataLen )
	{
		completeCommandWithError( CMD_ERR_GET_FILE_IO, "failed write %d bytes from ssl stream to file %s\n", dataLen, m_strDownloadFileName.c_str() );
		LogMsg( LOG_ERROR, m_strErrorMsg.c_str() );
	}
	else
	{
		m_u32FileOffs += bytesWritten;
		writeOk = true;
	}
	return writeOk;
}

//============================================================================
void SslMgrCmdGetFile::checkForFileLen( void )
{
	std::string strFileLen;
	if( getAccumulatedCmd( strFileLen ) )
	{
		m_u32FileLength = atoi( strFileLen.c_str() );

		startFileReceive();
	}
}

//============================================================================
void SslMgrCmdGetFile::checkForCheckSum( void )
{
	if( 4 <= m_iAccumIdx )
	{
		m_u32FileCrc =	(uint32_t)(m_as8AccumBuf[0] << 24)  
						| (uint32_t)(m_as8AccumBuf[1] << 16) 
						| (uint32_t)(m_as8AccumBuf[2] << 8)  
						| (uint32_t)(m_as8AccumBuf[3]);  
		removeFromFrontOfDataAccum( 4 );
		startFileReceive();
		LogMsg( LOG_INFO, "get file %s check sum 0x%8.8x\n", m_strFileName.c_str(), m_u32FileCrc );
	}
}

//============================================================================
void SslMgrCmdGetFile::startFileReceive( void )
{
	m_strDownloadFileName = VxGetIncompleteDirectory();
	m_strDownloadFileName += m_strFileName;

	m_eCmdState = eCmdGetFileStateError;

	m_CmdMgr.getSslWrapper().getSslCallback().sslMgrFileGetBegin( m_strDownloadFileName.c_str() );

	m_hFile = fopen( m_strDownloadFileName.c_str(), "wb+" ); 
	if( NULL == m_hFile )
	{
		completeCommandWithError( CMD_ERR_GET_FILE_IO, "get file %s could not open file %s to write\n", m_strFileName.c_str(), m_strDownloadFileName.c_str() );
		LogMsg( LOG_ERROR, m_strErrorMsg.c_str() );
		return;
	}

	m_eCmdState = eCmdGetFileStateRxFileBytes;

	if( m_iAccumIdx )
	{
		// some data was rxed into accumulator that was meant for file.. write to file now
		int bytesWritten = fwrite(	m_as8AccumBuf,
									1,
									m_iAccumIdx,
									m_hFile );
		if( bytesWritten != m_iAccumIdx )
		{
			completeCommandWithError( CMD_ERR_GET_FILE_IO, "failed write %d bytes from accum to file %s\n", m_iAccumIdx, m_strDownloadFileName.c_str() );
			LogMsg( LOG_ERROR, m_strErrorMsg.c_str() );
		}
		else
		{
			m_u32FileOffs += bytesWritten;
		}

		clearDataAccum();
	}

	checkForFileComplete();
}

//============================================================================
void SslMgrCmdGetFile::checkForFileComplete( void )
{
	if( m_u32FileLength )
	{
		int progress = (int)(((float)m_u32FileOffs * 100.0f) / (float)m_u32FileLength);
		LogMsg( LOG_INFO, "SslMgrCmdGetFile:bytes %d of len %d file %s rxed\n", m_u32FileOffs, m_u32FileLength, m_strDownloadFileName.c_str()  );

		m_CmdMgr.getSslWrapper().getSslCallback().sslMgrFileGetProgress( m_strDownloadFileName.c_str(), (int)(((float)m_u32FileOffs * 100.0f) / (float)m_u32FileLength) );

		if( isFileFinishedDownloading() )
		{
			LogMsg( LOG_INFO, "All bytes %d of file %s rxed\n", m_u32FileLength, m_strDownloadFileName.c_str()  );
			finishFileReceive();
		}
	}
	else
	{
		LogMsg( LOG_INFO, "checkForFileComplete 0 file length\n"  );
	}
}

//============================================================================
bool SslMgrCmdGetFile::timedOut( void )
{
	return m_Timer.elapsedMs() > SSLMGR_CMD_GET_FILE_TIMEOUT_MS ? true : false;
}


//============================================================================
void SslMgrCmdGetFile::doFileWriteError( void )
{
	completeCommandWithError( CMD_ERR_GET_FILE_IO, "SslMgrCmdGetFile failed at offset %d len %d file %s\n", m_u32FileOffs, m_u32FileLength, m_strDownloadFileName.c_str() );
}

//============================================================================
bool SslMgrCmdGetFile::isFileFinishedDownloading( void )
{
	if( m_u32FileOffs >= m_u32FileLength )
	{
		return true;
	}
	return false;
}

//============================================================================
void SslMgrCmdGetFile::finishFileReceive( void )
{
	m_eCmdState = eCmdGetFileStateSuccess;
	setIsCmdComplete( true );
	if( m_hFile )
	{
		fclose( m_hFile );
		m_hFile = NULL;
	}

	VxCalculateFileCheckSum( m_strDownloadFileName.c_str(), m_u32FileCrc );

	m_CmdMgr.getSslWrapper().getSslCallback().sslMgrFileGetProgress( m_strDownloadFileName.c_str(), (int)(((float)m_u32FileOffs * 100.0f) / (float)m_u32FileLength) );
	LogMsg( LOG_INFO, "SslMgrCmdGetFile: file %s received len %d\n", m_strDownloadFileName.c_str(), m_u32FileLength );
	m_CmdMgr.getSslWrapper().getSslCallback().sslMgrFileGetComplete( m_strDownloadFileName.c_str() );
}

