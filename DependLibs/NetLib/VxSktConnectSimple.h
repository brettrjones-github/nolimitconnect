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
// http://www.nolimitconnect.org
//============================================================================

#include "VxSktUtil.h"

#include <CoreLib/VxCrypto.h>

class VxSktConnectSimple
{
public:
	VxSktConnectSimple();
	virtual ~VxSktConnectSimple();

	uint16_t					getRemotePort( void )							{ return m_RmtIp.getPort(); }
	const char *				getRemoteIpAddress( void )                      { return m_RmtIp.toStdString().c_str(); }
	uint16_t					getLocalPort( void )							{ return m_LclIp.getPort(); }
    void				        setLocalIpAddress( const char * lclIp )			{ m_LclIp.setIp( lclIp ); }
	std::string					getLocalIpAddress( void );
    void				        setLocalIpAndPort( const char * lclIp, uint16_t port )			{ m_LclIp.setIpAndPort( lclIp, port ); }

	uint16_t					getCryptoKeyPort( void )						{ return m_RmtIp.getPort(); }

	virtual bool				isConnected( void );

    virtual SOCKET				connectTo( const char *	pIpOrUrl,						// remote ip or url
                                           uint16_t		u16Port,						// port to connect to
                                           int			iTimeoutMilliSeconds = WEBSITE_CONNECT_TIMEOUT );	// timeout attempt to connect

    virtual SOCKET				connectTo( const char * lclAdapterIp,					// local adapter ip
                                           const char *	pIpOrUrl,						// remote ip or url
                                           uint16_t		u16Port,						// port to connect to
                                           int			iTimeoutMilliSeconds = WEBSITE_CONNECT_TIMEOUT );	// timeout attempt to connect

	virtual bool				connectToWebsite( 	const char *		pWebsiteUrl,
													std::string&		strHost,		// return host name.. example http://www.mysite.com/index.htm returns www.mysite.com
													std::string&		strFile,		// return file name.. images/me.jpg
													uint16_t&			u16Port,
													int					iConnectTimeoutMs = WEBSITE_CONNECT_TIMEOUT );

	virtual bool				connectToWebsite(	const char* pWebsiteUrl,
													std::string& strHost,		// return host name.. example http://www.mysite.com/index.htm returns www.mysite.com
													std::string& strFile,		// return file name.. images/me.jpg
													uint16_t& u16Port,
													std::string& strResolveIpAddr,
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

	void						dumpConnectionInfo( void );

	//=== vars ===//
	SOCKET						m_Socket;				// handle to socket
	bool						m_bIsConnected;			// return true if is connected

	InetAddrAndPort				m_LclIp;				// local ip address
	InetAddrAndPort				m_RmtIp;				// remote (peer) ip address
};

