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

#include "SslMgrCmdMgr.h"
#include "SslWrapper.h"

#include "SslMgrCmdIdle.h"
#include "SslMgrCmdLogOn.h"
#include "SslMgrCmdLogOff.h"
#include "SslMgrCmdGetFile.h"
#include "SslMgrCmdListFiles.h"
#include "SslMgrCmdSendFile.h"
#include "SslMgrCmdSendText.h"
#include "SslMgrCmdDeleteFile.h"
#include "SslMgrCmdError.h"

#include <CoreLib/VxFileCrc32.h>
#include <CoreLib/VxGlobals.h>

#if defined(_MSC_VER)
# pragma warning( disable: 4355 ) //'this' : used in base member initializer list
#endif // _MSC_VER

//============================================================================
SslMgrCmdMgr::SslMgrCmdMgr( SslWrapper& wrapper )
: m_SslWrapper( wrapper )
, m_SslMgrCmdIdle( *this )
, m_SslMgrCurCmd( &m_SslMgrCmdIdle )
{
}

SslMgrCmdMgr::~SslMgrCmdMgr()
{
    endSslMgrCmds();
}

//============================================================================
void SslMgrCmdMgr::LogMsg( unsigned long u32LogMsgType, const char * logMsg, ... )
{
	char as8Buf[ 1024 ];
	va_list argList;
	va_start(argList, logMsg);
	vsnprintf( as8Buf, sizeof( as8Buf ), logMsg, argList );
	as8Buf[sizeof( as8Buf ) - 1] = 0;
	va_end(argList);

	m_SslWrapper.getSslCallback().LogMsg( u32LogMsgType, as8Buf );
}

//============================================================================
bool SslMgrCmdMgr::haveAllSslMgrCmdsCompleted( void )
{
	if( ( eSslMgrCmdIdle != m_SslMgrCurCmd->getCmdType() )
		|| ( false == m_SslMgrCmds.empty() ) )
	{
		return false;
	}
	return true;
}

//============================================================================
void SslMgrCmdMgr::beginSslMgrCmds( void )
{
}

//============================================================================
void SslMgrCmdMgr::updateSslMgrCmds( void )
{
	// crap to avoid deadlock if user adds commands durring callback
	bool doCompletedCallback = false;
	bool doErrorCallback = false;
	bool doIdleCallback = false;
	std::string callbackErrMsg;
	ESslMgrCmdType eCmdTypeForCallback;
	int callbackErrCode = 0;

	m_CmdMutex.lock();
	bool isComplete = false;

	m_SslMgrCurCmd->updateState();
    
    static size_t queue_depth = 0;
    if (m_SslMgrCmds.size() != queue_depth) 
	{
        queue_depth = m_SslMgrCmds.size();
        printf( "Queue depth %d, CmdType: %d\n", (int)m_SslMgrCmds.size(), (int)m_SslMgrCurCmd->getCmdType() );
    }
    

	// see if current command is not idle
	if( eSslMgrCmdIdle != m_SslMgrCurCmd->getCmdType() )
	{
		// not idle.. see if complete
		isComplete = m_SslMgrCurCmd->getIsCmdComplete();

		if( isComplete )
		{
			if( m_SslMgrCurCmd->hasError() 
				&&  ( eSslMgrCmdError != m_SslMgrCurCmd->getCmdType() ) )
			{
				// push error as next command to execute
				LogMsg( LOG_ERROR, "Cmd %d %s had error %s\n", m_SslMgrCurCmd->getCmdType(), m_SslMgrCurCmd->describeSslMgrCmdType( m_SslMgrCurCmd->getCmdType() ), m_SslMgrCurCmd->getErrorMsg().c_str() );
				addErrorCmd( m_SslMgrCurCmd->getCmdType(), m_SslMgrCurCmd->getErrorCode(), m_SslMgrCurCmd->getErrorMsg().c_str() );
			}
			else
			{
				if( eSslMgrCmdError != m_SslMgrCurCmd->getCmdType() )
				{
					LogMsg( LOG_INFO, "Cmd %d %s success\n", m_SslMgrCurCmd->getCmdType(), m_SslMgrCurCmd->describeSslMgrCmdType( m_SslMgrCurCmd->getCmdType() ) );
					eCmdTypeForCallback = m_SslMgrCurCmd->getCmdType();
					doCompletedCallback = true;
					if( eSslMgrCmdGetFile == m_SslMgrCurCmd->getCmdType() )
					{
						SslMgrCmdGetFile * getFileCmd = (SslMgrCmdGetFile *)m_SslMgrCurCmd;
						if( getFileCmd->deleteFromSslMgrIfSuccess() )
						{
							// make the next command to execute a delete file command
							std::string	strCompletedPath = VxGetUserXferDirectory();
							strCompletedPath += "from_sslMgr_complete/";
							strCompletedPath += getFileCmd->getFileName();

							uint32_t u32CheckSum = getFileCmd->getFileCheckSum();
							SslMgrCmdDeleteFile * sslMgrDeleteCmd = new SslMgrCmdDeleteFile( *this, getFileCmd->getFileName().c_str(), u32CheckSum );
							m_SslMgrCmds.insert( m_SslMgrCmds.begin(), sslMgrDeleteCmd );
						}
					}
				}
				else
				{
					eCmdTypeForCallback = ((SslMgrCmdError *)m_SslMgrCurCmd)->m_eCmdTypeWithError;
					callbackErrCode = ((SslMgrCmdError *)m_SslMgrCurCmd)->m_ErrorCode;
					callbackErrMsg = ((SslMgrCmdError *)m_SslMgrCurCmd)->m_strErrorText;
					doErrorCallback = true;
				}
			}

			// done with command.. move to idle
			m_SslMgrCurCmd->exitState();
            if (eSslMgrCmdLogOff == m_SslMgrCurCmd->getCmdType()) {
                m_SslWrapper.forceSSLDisconnect();
            }
			delete m_SslMgrCurCmd;
			if( m_SslMgrCmds.empty() )
			{
				doIdleCallback = true;
			}
			m_SslMgrCurCmd = &m_SslMgrCmdIdle;
		}
	}

	if( eSslMgrCmdIdle == m_SslMgrCurCmd->getCmdType() )
	{
		if( m_SslMgrCmds.size() )
		{
			// move to next command
			std::vector<SslMgrCmdBase *>::iterator iter;
			iter = m_SslMgrCmds.begin();

			m_SslMgrCurCmd = *iter;
			m_SslMgrCmds.erase( iter );
            printf("removed command %d\n", m_SslMgrCurCmd->getCmdType());

			m_SslMgrCurCmd->enterState();
		}
	}

	m_CmdMutex.unlock();
	if( doCompletedCallback )
	{
		m_SslWrapper.getSslCallback().sslMgrCmdComplete( eCmdTypeForCallback );
	}
	if( doErrorCallback )
	{
		m_SslWrapper.getSslCallback().sslMgrCmdError( eCmdTypeForCallback, callbackErrCode, callbackErrMsg.c_str() );
	}
	if( doIdleCallback )
	{
		m_SslWrapper.getSslCallback().sslMgrWentIdle();
	}
}

//============================================================================
void SslMgrCmdMgr::handleDelays( void )
{
    m_CmdMutex.lock();
	m_SslMgrCurCmd->handleDelays();
	m_CmdMutex.unlock();
    VxSleep(1);
}

//============================================================================
void SslMgrCmdMgr::endSslMgrCmds( void )
{
	m_CmdMutex.lock();
	// kill current command if not idle
	if( eSslMgrCmdIdle != m_SslMgrCurCmd->getCmdType() )
	{
		m_SslMgrCurCmd->exitState();
		delete m_SslMgrCurCmd;
		m_SslMgrCurCmd = &m_SslMgrCmdIdle;
	}

	//deleteAllQuedCommands();

	m_CmdMutex.unlock();
}

//============================================================================
void SslMgrCmdMgr::deleteAllQuedCommands( void )
{
	// delete any qued commands
    if (m_SslMgrCmds.size() != 0) {
        printf("deleteAllQuedCommands: m_SslMgrCmds size is not 0\n");
    }
    
	std::vector<SslMgrCmdBase *>::iterator iter;
	for( iter = m_SslMgrCmds.begin(); iter != m_SslMgrCmds.end(); ++iter )
	{
		delete (*iter);
	}

	m_SslMgrCmds.clear();
}

//============================================================================
void SslMgrCmdMgr::addErrorCmd( ESslMgrCmdType eCmdType, int errCode, const char * errMsg )
{
	SslMgrCmdError * sslMgrCmd = new SslMgrCmdError( *this );
	sslMgrCmd->setErrorText( errMsg );
	sslMgrCmd->setErrorCode( errCode );
	sslMgrCmd->setCmdTypeWithError( eCmdType );
	
	// NOTE do not lock m_CmdMutex here.. is locked by updateSslMgrCmds before this function is called
	m_SslMgrCmds.insert( m_SslMgrCmds.begin(), sslMgrCmd );
}

//============================================================================
int SslMgrCmdMgr::getDataToWriteToSslMgrServer( char * buf, int bufSize )
{
	m_CmdMutex.lock();
	int result = m_SslMgrCurCmd->getDataToWriteToSslMgrServer( buf, bufSize );
	m_CmdMutex.unlock();
	return result;
}

//============================================================================
void SslMgrCmdMgr::handleDataFromSslMgrServer( char * buf, int dataSize )
{
	m_CmdMutex.lock();
	m_SslMgrCurCmd->handleDataFromSslMgrServer( buf, dataSize );
	m_CmdMutex.unlock();
}

//============================================================================
void SslMgrCmdMgr::sslMgrCmdLogOn( const char * userName, const char * pass )
{

	SslMgrCmdLogOn * sslMgrCmd = new SslMgrCmdLogOn( *this );
	sslMgrCmd->setUserName( userName );
	sslMgrCmd->setPassword( pass );
	m_CmdMutex.lock();
    deleteAllQuedCommands();
    printf("SslMgrCmdMgr::sslMgrCmdLogOn: Adding logon to queue\n");
    m_SslMgrCmds.push_back( sslMgrCmd );
	m_CmdMutex.unlock();
}

//============================================================================
void SslMgrCmdMgr::sslMgrCmdLogOff( bool logOffRightNow, bool fromSslMgrThread )
{
	SslMgrCmdLogOff * sslMgrCmd = new SslMgrCmdLogOff( *this );
	if( false == fromSslMgrThread )
	{
		m_CmdMutex.lock();
	}

	if( logOffRightNow )
	{
		deleteAllQuedCommands();
	}
    printf("SslMgrCmdMgr::sslMgrCmdLogOff: Adding logoff to queue\n");
	m_SslMgrCmds.push_back( sslMgrCmd );

	if( false == fromSslMgrThread )
	{
		m_CmdMutex.unlock();
	}
}

//============================================================================
void SslMgrCmdMgr::sslMgrCmdSendFile( const char * userFileName )
{
	SslMgrCmdSendFile * sslMgrCmd = new SslMgrCmdSendFile( *this );
	sslMgrCmd->setFileName( userFileName );
	m_CmdMutex.lock();
    printf("SslMgrCmdMgr::sslMgrCmdSendFile: Adding send file to queue\n");
	m_SslMgrCmds.push_back( sslMgrCmd );
	m_CmdMutex.unlock();
}

//============================================================================
void SslMgrCmdMgr::sslMgrCmdGetFile( const char * userFileName, bool deleteFromSslMgrIfSuccess )
{
	SslMgrCmdGetFile * sslMgrCmd = new SslMgrCmdGetFile( *this, deleteFromSslMgrIfSuccess );
	sslMgrCmd->setFileName( userFileName );
    LogMsg(LOG_DEBUG, "SslMgrCmdMgr::sslMgrCmdGetFile, file name: %s\n", userFileName);
    
	m_CmdMutex.lock();
	m_SslMgrCmds.push_back( sslMgrCmd );
	m_CmdMutex.unlock();
}

//============================================================================
void SslMgrCmdMgr::sslMgrCmdDeleteFile( const char * userFileName, uint32_t u32CheckSum )
{
	SslMgrCmdDeleteFile * sslMgrCmd = new SslMgrCmdDeleteFile( *this, userFileName, u32CheckSum );
	m_CmdMutex.lock();
	m_SslMgrCmds.push_back( sslMgrCmd );
	m_CmdMutex.unlock();
}

//============================================================================
void SslMgrCmdMgr::sslMgrCmdListFiles( void )
{
	SslMgrCmdListFiles * sslMgrCmd = new SslMgrCmdListFiles( *this );
	m_CmdMutex.lock();
	m_SslMgrCmds.push_back( sslMgrCmd );
	m_CmdMutex.unlock();
}

//============================================================================
void SslMgrCmdMgr::sslMgrCmdSendText( const char * textToSendToServer )
{
	SslMgrCmdSendText * sslMgrCmd = new SslMgrCmdSendText( *this );
	sslMgrCmd->setCmdText(textToSendToServer );
	m_CmdMutex.lock();
	m_SslMgrCmds.push_back( sslMgrCmd );
	m_CmdMutex.unlock();
}

