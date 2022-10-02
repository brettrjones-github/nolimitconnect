#pragma once
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

#include <string.h>
#include <string>

//=== proxy defs ===//
#ifndef RCODE
	#define RCODE long
#endif

//Errorcodes
#define PROXYERROR_NOERROR			0
#define PROXYERROR_BASE				0x123767

#define PROXYERROR_NOCONN			PROXYERROR_BASE + 1 //Can't connect to proxy server, use GetLastError for more information
#define PROXYERROR_REQUESTFAILED	PROXYERROR_BASE + 2 //Request failed, can't send data
#define PROXYERROR_AUTHREQUIRED		PROXYERROR_BASE + 3 //Authentication required
#define PROXYERROR_AUTHTYPEUNKNOWN	PROXYERROR_BASE + 4 //Authtype unknown or not supported
#define PROXYERROR_AUTHFAILED		PROXYERROR_BASE + 5  //Authentication failed
#define PROXYERROR_AUTHNOLOGON		PROXYERROR_BASE + 6
#define PROXYERROR_CANTRESOLVEHOST	PROXYERROR_BASE + 7
#define PROXYERROR_INVALID_PARAM	PROXYERROR_BASE + 8

//Proxytypes
#define PROXYTYPE_NOPROXY	0
#define PROXYTYPE_HTTP11	1
#define PROXYTYPE_SOCKS4	2
#define PROXYTYPE_SOCKS4A	3
#define PROXYTYPE_SOCKS5	4


#define BASE64_END 	'='
#define PROXY_MAX_HOST_NAME_LEN		512
#define PROXY_MAX_ACCT_NAME_LEN		255
#define PROXY_MAX_ACCT_PWD_LEN		255
#define PROXY_RX_BUF_LEN 4096

//forward declare
class VxSkt;

class VxProxy
{
public:
	VxProxy()
	{

		m_iProxyType = 0;
		m_u16ProxyPort = 0;
		m_as8ProxyHost[ 0 ] = 0;
		m_as8ProxyAcctName[ 0 ] = 0;
		m_as8ProxyAcctPwd[ 0 ] = 0;
		m_s32ProxyError = PROXYERROR_NOERROR;
		m_s32ProxyOpID=0;
		m_s32ProxyOpState=0;
		m_s32RecvBufferPos=0;
		m_s32ProxyPeerIp=0;
		m_u16ProxyPeerPort=0;
		m_ProxyPeerHost[0 ] = 0;
		m_u32ProxyIp = 0;
		m_bUseLogon = false;
	}

	RCODE SetProxy(	int iProxyType,	//Type of proxy	
						const char* pProxyHost,		//firewall Host name
						const unsigned short u16ProxyPort,//firewall port
						const char* pProxyAcctName,	//firewall Account Name
						const char* pProxyAcctPwd );	//firewall Account Pwd

	int GetProxyType(){ return m_iProxyType; }
	char * GetProxyHost(){ return m_as8ProxyHost; }
	unsigned short GetProxyPort(){ return m_u16ProxyPort; }
	unsigned long GetProxyIp(){ return m_u32ProxyIp; }
	char * GetProxyUser(){ return m_as8ProxyAcctName; }
	char * GetProxyPwd(){ return m_as8ProxyAcctPwd; }
	void SetProxyType(int iProxyType ){ m_iProxyType = iProxyType; }
	void SetProxyHost( const char* pProxyHost ){ strcpy( m_as8ProxyHost, pProxyHost ); }
	void SetProxyUser( const char* pProxyUser ){ strcpy( m_as8ProxyAcctName, pProxyUser ); }
	void SetProxyPwd( const char* pProxyPwd ){ strcpy( m_as8ProxyAcctPwd, pProxyPwd ); }
	char * GetBasicAuth( void );

	RCODE OnConnect( VxSkt * sktBase, int iErrorCode );
	RCODE OnReceive( VxSkt * sktBase, int iErrorCode );

	char * DescribeErr( long rc );

	int		m_iProxyType;
	unsigned short m_u16ProxyPort;
	char	m_as8ProxyHost[ PROXY_MAX_HOST_NAME_LEN ];
	char	m_as8ProxyAcctName[ PROXY_MAX_ACCT_NAME_LEN ];
	char	m_as8ProxyAcctPwd[ PROXY_MAX_ACCT_PWD_LEN ];
	long	m_s32ProxyError;
	long	m_s32ProxyOpID;
	long	m_s32ProxyOpState;
	long	m_s32RecvBufferPos;
	long	m_s32ProxyPeerIp;
	unsigned short m_u16ProxyPeerPort;
	char	m_ProxyPeerHost[ PROXY_MAX_HOST_NAME_LEN ];
	char	m_pRecvBuffer[ PROXY_RX_BUF_LEN ];
	unsigned long m_u32ProxyIp;
	char	m_as8BasicAuth[ PROXY_MAX_ACCT_NAME_LEN + PROXY_MAX_ACCT_PWD_LEN + 6 ];
	bool	m_bUseLogon;
};

//=== proxy functions ===//
void VxToBase64(	std::string & csOrigStr,			//string to convert
					std::string & csRetBase64Str );	//return base 64 string


//=== proxy globals ===//
extern const char g_as8Base64Table[67]; 
extern VxProxy g_oProxy;

