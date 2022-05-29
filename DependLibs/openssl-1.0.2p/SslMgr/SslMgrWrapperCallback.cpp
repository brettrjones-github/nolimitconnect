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

#include "SslMgrWrapper.h"

//============================================================================
void SslMgrWrapper::LogMsg( unsigned long u32LogMsgType, const char * logMsg )
{
	m_SslMgrCallback.LogMsg( u32LogMsgType, logMsg );
}

//============================================================================
void SslMgrWrapper::connectedToSslMgrServer( void )
{
	m_SslMgrCallback.connectedToSslMgrServer();
}

//============================================================================
void SslMgrWrapper::disconnectedFromSslMgrServer( void )
{
	m_SslMgrCallback.disconnectedFromSslMgrServer();
}

//============================================================================
void SslMgrWrapper::sslMgrCmdComplete( ESslMgrCmdType cmdType )
{
	m_SslMgrCallback.LogMsg( LOG_DEBUG, "sslMgrCmdComplete %d %s\n", cmdType, SslMgrCmdBase::describeSslMgrCmdType( cmdType ) );
	m_SslMgrCallback.sslMgrCmdComplete( cmdType );
}

//============================================================================
void SslMgrWrapper::sslMgrCmdError( ESslMgrCmdType cmdType, int errCode, const char * errMsg )
{
	m_SslMgrCallback.LogMsg( LOG_DEBUG, "sslMgrCmdError %d %d %s\n", cmdType, errCode, errMsg );
	m_SslMgrCallback.sslMgrCmdError( cmdType, errCode, errMsg );
}

//============================================================================
void SslMgrWrapper::sslMgrWentIdle( void )
{
	m_SslMgrCallback.LogMsg( LOG_DEBUG, "sslMgrWentIdle\n" );
	m_SslMgrCallback.sslMgrWentIdle();
}

//============================================================================
void SslMgrWrapper::sslMgrListFile( const char * fileName )
{
	m_SslMgrCallback.LogMsg( LOG_DEBUG, "sslMgrListFile %s\n", fileName );
	m_SslMgrCallback.sslMgrListFile( fileName );
}

//============================================================================
void SslMgrWrapper::sslMgrFileGetBegin( const char * fileName )
{
	m_SslMgrCallback.LogMsg( LOG_DEBUG, "sslMgrFileGetBegin %s\n", fileName );
	m_SslMgrCallback.sslMgrFileGetBegin( fileName );
}

//============================================================================
void SslMgrWrapper::sslMgrFileGetProgress( const char * fileName, int percentComplete )
{
	m_SslMgrCallback.LogMsg( LOG_DEBUG, "sslMgrFileGetProgress %d\n", percentComplete );
	m_SslMgrCallback.sslMgrFileGetProgress( fileName, percentComplete );
}

//============================================================================
void SslMgrWrapper::sslMgrFileGetComplete( const char * fileName )
{
	m_SslMgrCallback.LogMsg( LOG_DEBUG, "sslMgrFileGetComplete %s\n", fileName );
	m_SslMgrCallback.sslMgrFileGetComplete( fileName );
}

//============================================================================
void SslMgrWrapper::sslMgrFileSendBegin( const char * fileName )
{
	m_SslMgrCallback.LogMsg( LOG_DEBUG, "sslMgrFileSendBegin %s\n", fileName );
	m_SslMgrCallback.sslMgrFileSendBegin( fileName );
}

//============================================================================
void SslMgrWrapper::sslMgrFileSendProgress( const char * fileName, int percentComplete )
{
	m_SslMgrCallback.LogMsg( LOG_DEBUG, "sslMgrFileSendProgress %d\n", percentComplete );
	m_SslMgrCallback.sslMgrFileSendProgress( fileName, percentComplete );
}

//============================================================================
void SslMgrWrapper::sslMgrFileSendComplete( const char * fileName )
{
	m_SslMgrCallback.LogMsg( LOG_DEBUG, "sslMgrFileSendComplete %s\n", fileName );
	m_SslMgrCallback.sslMgrFileSendComplete( fileName );
}
