#pragma once
//============================================================================
// Copyright (C) 2008 Brett R. Jones 
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
// http://www.nolimitconnect.com
//============================================================================

#include "VxSktUtil.h"

#include <CoreLib/VxCrypto.h>

class VxSktConnectSimple
{
public:
	VxSktConnectSimple();
	virtual ~VxSktConnectSimple();

	uint16_t							getRemotePort( void )							{ return m_RmtIp.getPort(); }
	const char *				getRemoteIpAddress( void )                      { return m_RmtIp.toStdString().c_str(); }
	uint16_t							getLocalPort( void )							{ return m_LclIp.getPort(); }
	const char *				getLocalIpAddress( void )						{ return m_LclIp.toStdString().c_str(); }

	uint16_t							getCryptoKeyPort( void )						{ return m_RmtIp.getPort(); }

	virtual bool				isConnected( void );

	virtual SOCKET				connectTo(	const char *	pIpOrUrl,						// remote ip or url
											uint16_t				u16Port,						// port to connect to
											int				iTimeoutMilliSeconds = WEBSITE_CONNECT_TIMEOUT );	// timeout attempt to connect

	virtual bool				connectToWebsite( 	const char *		pWebsiteUrl,
													std::string&		strHost,		// return host name.. example http://www.mysite.com/index.htm returns www.mysite.com
													std::string&		strFile,		// return file name.. images/me.jpg
													uint16_t&				u16Port,
													int					iConnectTimeoutMs = WEBSITE_CONNECT_TIMEOUT );

	virtual RCODE				sendData(	const char *	pData,							// data to send
											int				iDataLen,						// length of data	
											int				iTimeoutMilliSeconds = SKT_SEND_TIMEOUT );	// timeout attempt to send ( 0 = don't timeout )

	//! receive data.. if timeout is set then will keep trying till buffer is full or error or timeout expires
	virtual RCODE				recieveData(	char *		pRetDataBuf,					// data buffer to read into
												int			iBufLen,						// length of data	
												int *		iRetBytesReceived,				// number of bytes actually received
												int			iTimeoutMilliSeconds = SKT_SEND_TIMEOUT,	// timeout attempt to received
												bool		bAbortIfCrLfCrLf = false,		// if true then abort receive when \r\n\r\n is received
												bool *		pbRetGotCrLfCrLf = NULL );		// if received \r\n\r\n set to true

	virtual void				closeSkt( int iInstance = 0 );

	////! get remote port connection is on
	//virtual InetAddress			getRemoteIpBinary( void )	{ return m_RmtIp;}
	////! get local ip as string
	//virtual const char *		getLocalIp( void )			{ return m_strLclIp.c_str(); }

	//virtual InetAddress			getLocalIpBinary( void )	{ return m_RmtIp;};

	////! used internally to get the remote ip address and set it in the vars
	//RCODE						doGetRmtAddress( void );

	////! encrypt then send data using session crypto
	//RCODE						txEncrypted(	const char *	pDataIn, 		// data to send
	//											int				iDataLen,		// length of data
	//											bool			bDisconnect );	// if true disconnect after send

	void						dumpConnectionInfo( void );

	//=== vars ===//
	SOCKET						m_Socket;				// handle to socket
	bool						m_bIsConnected;			// return true if is connected

	InetAddrAndPort				m_LclIp;				// local ip address
	InetAddrAndPort				m_RmtIp;				// remote (peer) ip address
	
	////=== encryption vars ===//
	//VxKey						m_TxKey;				// encryption key for transmit
	//VxKey						m_RxKey;				// encryption key for receive
	//VxCrypto					m_TxCrypto;			// encryption object for transmit
	//VxCrypto					m_RxCrypto;			// encryption object for receive

};

