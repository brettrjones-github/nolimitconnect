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

#include <CoreLib/VxParse.h>
#include <CoreLib/VxDebug.h>

#if defined(_MSC_VER)
# pragma warning( disable: 4355 ) //'this' : used in base member initializer list
#endif // _MSC_VER


//============================================================================
SslMgrWrapper::SslMgrWrapper( SslMgrWrapperCallback& sslMgrCallback )
: m_SslWrapper( *this, m_FileUtil )
, m_SslMgrCallback( sslMgrCallback )
{
}

//============================================================================
SslMgrWrapper::~SslMgrWrapper()
{
}

//============================================================================
void SslMgrWrapper::setCertFileName( const char * certFileName )
{
	m_SslWrapper.setSslMgrCertFileName( certFileName );
}

//============================================================================
void SslMgrWrapper::setKeyFileName( const char * keyFileName )
{
	m_SslWrapper.setSslMgrKeyFileName( keyFileName );
}

//============================================================================
void SslMgrWrapper::setSslMgrServerIpAddr( const char * ipAddr )
{
	m_SslWrapper.setSslMgrServerIpAddr( ipAddr );
}

//============================================================================
void SslMgrWrapper::setSslMgrServerPort( uint16_t port )
{
	m_SslWrapper.setSslMgrServerPort( port );
}

//============================================================================
void SslMgrWrapper::setSslMgrConnectRetryIntervalMs( int intervalMs )
{
	m_SslWrapper.setSslMgrConnectRetryIntervalMs( intervalMs );
}

//============================================================================
void SslMgrWrapper::connectToSslMgrServer( void )
{
	m_SslWrapper.connectToSslMgrServer();
}

//============================================================================
void SslMgrWrapper::disconnectFromSslMgrServer( void )
{
	m_SslWrapper.disconnectFromSslMgrServer();
}

//============================================================================
bool SslMgrWrapper::isConnectedToSslMgrServer( void )
{
	return m_SslWrapper.isConnectedToSslMgrServer();
}

//============================================================================
void SslMgrWrapper::sslMgrCmdLogOn( const char * userName, const char * pass )
{
	m_SslWrapper.getCmdMgr().sslMgrCmdLogOn( userName, pass );
}

//============================================================================
void SslMgrWrapper::sslMgrCmdLogOff(  bool logOffRightNow, bool fromSslMgrThread )
{
	m_SslWrapper.getCmdMgr().sslMgrCmdLogOff( logOffRightNow, fromSslMgrThread );
}

//============================================================================
void SslMgrWrapper::sslMgrCmdSendFile( const char * userFileName )
{
	m_SslWrapper.getCmdMgr().sslMgrCmdSendFile( userFileName );
}

//============================================================================
void SslMgrWrapper::sslMgrCmdListFiles( void )
{
	m_SslWrapper.getCmdMgr().sslMgrCmdListFiles();
}

//============================================================================
void SslMgrWrapper::sslMgrCmdGetFile( const char * userFileName, bool deleteFromSslMgrIfSuccess )
{
	m_SslWrapper.getCmdMgr().sslMgrCmdGetFile( userFileName, deleteFromSslMgrIfSuccess );
}

//============================================================================
void SslMgrWrapper::sslMgrCmdDeleteFile( const char * userFileName, uint32_t u32CheckSum )
{
	m_SslWrapper.getCmdMgr().sslMgrCmdDeleteFile( userFileName, u32CheckSum );
}

//============================================================================
void SslMgrWrapper::sslMgrCmdSendText( const char * textToSendToServer )
{
	m_SslWrapper.getCmdMgr().sslMgrCmdSendText( textToSendToServer );
}

//============================================================================
bool SslMgrWrapper::haveAllSslMgrCmdsCompleted( void )
{
	return m_SslWrapper.getCmdMgr().haveAllSslMgrCmdsCompleted();
}

