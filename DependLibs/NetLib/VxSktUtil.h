#pragma once
//============================================================================
// Copyright (C) 2003 Brett R. Jones 
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

#include <AppDependLibrariesConfig.h>
#ifdef TARGET_OS_WINDOWS
# include "WS2tcpip.h"
#endif // TARGET_OS_WINDOWS
// #define USE_BIND_LOCAL_IP 1  // bind to local ip address when using vpn causes connect fail with error 110 (Connection timed out)

#define EIM_ALIVE_TIMEDOUT          20000

#define WEBSITE_CONNECT_TIMEOUT		12000
#define NETSERVICE_CONNECT_TIMEOUT	15000
#define ANCHOR_CONNECT_TIMEOUT		15000
#define SKT_CONNECT_TIMEOUT			6000
#define SKT_IPV6_CONNECT_TIMEOUT	4000
#define SKT_SEND_TIMEOUT			3000
#define SKT_RECEIVE_TIMEOUT			3000
#define ROUTER_RECIEVE_TIMEOUT		8000

// error codes
#define VX_NO_HOST_IPS_FOUND	    -1
#define VX_INVALID_SOCK_ADDRESS     -2

#define VX_MAX_HOST_IPS             10 //maximum host ips returned by VxGetLocalIps

#ifdef __cplusplus

#include "InetAddress.h"

//! initialize sockets
RCODE							VxSocketsStartup( void );

bool                            VxIsIpValid( std::string& ipAddr );
std::string						VxGetRemoteIpAddress( SOCKET skt );
std::string						VxGetRmtHostName( SOCKET& skt );
//! split host name from website file path
bool							VxSplitHostAndFile( const char * pFullUrl,			// full url.. example http://www.mysite.com/index.html or www.mysite.com/images/me.jpg
													std::string& strRetHost,		// return host name.. example http://www.mysite.com/index.htm returns www.mysite.com
													std::string& strRetFileName,	// return file name.. images/me.jpg
													uint16_t& u16RetPort );				// return port if specified else return 80 as default	
void							VxIpInNetOrderToString( uint32_t u32IpAddr, std::string& retIp );
uint32_t						VxStringToIpInNetOrder( std::string ip );
std::string						VxIpToString( struct sockaddr * addr );
void							VxFillHints( struct addrinfo& oHints, bool bUdpSkt = false, bool ipv6Only = false );
void							VxGetLocalIps( std::vector<InetAddress>& aRetIpAddress );

InetAddress						VxGetSelectedLocalIp( void );
InetAddress						VxGetMyGlobalIPv6Address( void );
InetAddress						VxGetDefaultIPv4Address( void );
InetAddress						VxGetDefaultIPv6Address( void );
bool							VxTestConnectionOnSpecificLclAddress( InetAddress &oLclAddr );
bool							VxResolveUrl( const char* pUrl, uint16_t u16Port, InetAddress& oRetAddr );
bool							VxResolveUrl( std::string& urlIn, uint16_t& retPort, std::string& retIpAddr );
bool							VxResolveUrl( const char* pUrl, uint16_t u16Port, std::string& resolvedIp ); // assumes pUrl is just host name

//! return true if ip is in list of local ips
bool							VxLocalIpExists( std::string& strIpAddress );
// connects to website and returns socket.. if fails returns INVALID_SOCKET
SOCKET							VxConnectToWebsite( InetAddrAndPort&	oLclIp,			// ip of adapter to use
													InetAddrAndPort&	oRmtIp,			// return ip and port url resolves to
													const char *		pWebsiteUrl,
													std::string&		strHost,		// return host name.. example http://www.mysite.com/index.htm returns www.mysite.com
													std::string&		strFile,		// return file name.. images/me.jpg
                                                    uint16_t&			u16Port,		// return port
													int					iConnectTimeoutMs );

bool							VxBindSkt( SOCKET sktHandle, InetAddress & oLclAddr, uint16_t u16Port );
SOCKET							VxConnectTo( InetAddrAndPort& oLclIp, InetAddrAndPort& oRmtIp, uint16_t u16Port, int iTimeoutMs = SKT_CONNECT_TIMEOUT, RCODE * retSktErr = nullptr );
SOCKET							VxConnectTo( InetAddrAndPort&	oLclIp,
											 InetAddrAndPort&	oRmtIp,
											 const char *		pIpOrUrl,				// remote ip or url
                                             uint16_t			u16Port,				// port to connect to
                                             int				iTimeoutMilliSeconds,	// milli seconds before connect attempt times out
                                             RCODE *			retSktErr = 0 );		// return connect error if retSktErr is not null
SOCKET                          VxConnectToAddr(SOCKET sktHandle, struct sockaddr* sktAddr, socklen_t sktAddrLen, int iConnectTimeoutMs = SKT_CONNECT_TIMEOUT, RCODE * retSktErr = nullptr);
std::string                     VxSktAddrToString( struct sockaddr* sktAddr, int sktAddrLen );

RCODE							VxGetLclAddress( SOCKET sktHandle, InetAddrAndPort& oRetAddr );
std::string						VxGetLclIpAddress( SOCKET sktHandle, uint16_t *	retPort = nullptr );
RCODE							VxGetRmtAddress( SOCKET sktHandle, InetAddrAndPort& oRetAddr );

bool							VxIsIPv6Address( const char *addr );
bool							VxIsIPv4Address( const char *addr );
int								VxGetIPv6ScopeID( const char *addr );

void							VxRefreshDefaultIps( void );
bool							VxCanConnectUsingIPv6( void );
SOCKET							VxConnectToIPv6( const char * ipv6, uint16_t u16Port, int iTimeoutMs = SKT_IPV6_CONNECT_TIMEOUT, RCODE * retSktErr = nullptr );
																										//! receive data.. if timeout is set then will keep trying till buffer is full or error or timeout expires
RCODE							VxReceiveSktData( SOCKET&			oSkt,
												  char *			pRetBuf,				// buffer to receive data into
												  int				iBufLenIn,				// length of buffer
                                                  int *             iRetBytesReceived,		// number of bytes actually received
												  int				iTimeoutMilliSeconds = SKT_RECEIVE_TIMEOUT,	// milliseconds before receive attempt times out ( 0 = do not wait )
                                                  bool              bAbortIfCrLfCrLf = false,		// if true then abort receive when \r\n\r\n is received
												  bool *			pbRetGotCrLfCrLf = nullptr );		// if received \r\n\r\n set to truevoid							VxFillHints( struct addrinfo& oHints, bool bUdpSkt = false, bool ipv6Only = false );

bool							VxBindSkt( SOCKET oSocket, struct sockaddr_storage * poAddr );
bool							VxIsIpPortInUse( uint16_t u16Port, const char * pLocalIp = nullptr, bool useBind = false);

bool							VxMakeBroadcastIp( std::string localIp, std::string& retBroadcastIp );
#endif // __cplusplus

//============================================================================
// C functions
//============================================================================
GOTV_BEGIN_CDECLARES

RCODE							VxSetSktBlocking( SOCKET sktHandle, bool bBlock );

void							VxFlushThenCloseSkt( SOCKET oSocket );
void							VxCloseSktNow( SOCKET oSocket );
void							VxCloseSkt( SOCKET oSocket );

socklen_t						VxGetSktAddressLength( struct sockaddr_storage * poAddr );
void							VxSetSktAddressPort( struct sockaddr_storage * poAddr, uint16_t u16Port );
RCODE							VxSendSktData( SOCKET			oSkt,
											   const char *		pData,					// data to send
											   int				iDataLen,				// length of data
											   int				iTimeoutSeconds ); // = SKT_SEND_TIMEOUT // seconds before send attempt times out



void							VxSetSktAllowReusePort( SOCKET skt );

uint16_t						VxGetRmtPort( SOCKET skt );

const char *					VxDescribeSktError( int iErr );

//============================================================================
void							VxIP_ntop( int family, void * pvBinary, char * pBuf, int iBufLen, bool isHostOrder );
void							VxIPv4_ntop( void * pvBinary, char * pBuf, int iBufLen, bool isHostOrder );
void							VxIPv6_ntop( void * pvBinary, char * pBuf, int iBufLen );
void							VxIPv4_pton( const char * pIpString,  void * pvBinary, bool wantHostOrder );
void							VxIPv6_pton( const char * pIpString,  void * pvBinary );

GOTV_END_CDECLARES
