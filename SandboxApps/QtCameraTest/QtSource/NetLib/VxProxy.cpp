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


#include "VxProxy.h"


#ifdef VXNET_ENABLE_PROXY

//=== proxy globals ===//
const char g_as8Base64Table[]= "ABCDEFGHIJKLMNOPQRSTUVWXYZ" 
"abcdefghijklmnopqrstuvwxyz"
"0123456789+/";



VxProxy g_oProxy;

//=== proxy functions ===//
void VxToBase64(	std::string & csOrigStr,			//string to convert
					std::string & csRetBase64Str )	//return base 64 string
{
	int iLen = (int)csOrigStr.size();
	char * pBuf= new char[ ( iLen + 2 ) / 3 * 4 + 1 ];
	//char * pBuf= csRetBase64Str.GetBuffer( ( iLen + 2 ) / 3 * 4 + 1 );
	unsigned char * pu8Str = (unsigned char *)(const char *)csOrigStr.c_str();
	int i = 0;
	int j = 0;
	
	while( j < iLen - 2 )
	{
		pBuf[ i++ ] = g_as8Base64Table[ pu8Str[ j ] >> 2 ];
		pBuf[ i++ ] = g_as8Base64Table[(( pu8Str[ j ] & 3 ) << 4 ) |( pu8Str[ j + 1 ] >> 4 ) ];
		pBuf[ i++ ] = g_as8Base64Table[(( pu8Str[ j + 1 ] & 15 ) << 2 ) | ( pu8Str[ j + 2 ] >> 6 ) ];
		pBuf[ i++ ] = g_as8Base64Table[ pu8Str[ j + 2 ] & 63 ];
		j += 3;
	}
	switch( iLen - j )
	{
	case 1:
		pBuf[ i++ ] = g_as8Base64Table[ pu8Str[ j ] >> 2 ];
		pBuf[ i++ ] = g_as8Base64Table[( pu8Str[ j ] & 3 ) << 4 ];
		pBuf[ i++ ] = BASE64_END;
		pBuf[ i++ ] = BASE64_END;
		break;
	case 2:
		pBuf[ i++ ] = g_as8Base64Table[ pu8Str[ j ] >> 2 ];
		pBuf[ i++ ] = g_as8Base64Table[ ( ( pu8Str[ j ] & 3 ) << 4 ) | ( pu8Str[ j + 1 ] >> 4 ) ];
		pBuf[ i++ ] = g_as8Base64Table[ ( pu8Str[ j + 1 ] & 15 ) << 2 ];
		pBuf[ i++ ] = BASE64_END;
		break;
	case 0:
		break;
	}
	pBuf[i]='\0';
	csRetBase64Str = pBuf;
	delete pBuf;
}

char * VxProxy::GetBasicAuth( void )
{
	char as8Buf[ PROXY_MAX_ACCT_NAME_LEN + PROXY_MAX_ACCT_PWD_LEN + 6 ];
	sprintf( as8Buf, "%s:%s", m_as8ProxyAcctName, m_as8ProxyAcctPwd );
	int iLen = (int)strlen( as8Buf );
	char * pBuf= m_as8BasicAuth;
	unsigned char * pu8Str = (unsigned char *)as8Buf;
	int i = 0;
	int j = 0;
	
	while( j < iLen - 2 )
	{
		pBuf[ i++ ] = g_as8Base64Table[ pu8Str[ j ] >> 2 ];
		pBuf[ i++ ] = g_as8Base64Table[(( pu8Str[ j ] & 3 ) << 4 ) |( pu8Str[ j + 1 ] >> 4 ) ];
		pBuf[ i++ ] = g_as8Base64Table[(( pu8Str[ j + 1 ] & 15 ) << 2 ) | ( pu8Str[ j + 2 ] >> 6 ) ];
		pBuf[ i++ ] = g_as8Base64Table[ pu8Str[ j + 2 ] & 63 ];
		j += 3;
	}
	switch( iLen - j )
	{
	case 1:
		pBuf[ i++ ] = g_as8Base64Table[ pu8Str[ j ] >> 2 ];
		pBuf[ i++ ] = g_as8Base64Table[( pu8Str[ j ] & 3 ) << 4 ];
		pBuf[ i++ ] = BASE64_END;
		pBuf[ i++ ] = BASE64_END;
		break;
	case 2:
		pBuf[ i++ ] = g_as8Base64Table[ pu8Str[ j ] >> 2 ];
		pBuf[ i++ ] = g_as8Base64Table[ ( ( pu8Str[ j ] & 3 ) << 4 ) | ( pu8Str[ j + 1 ] >> 4 ) ];
		pBuf[ i++ ] = g_as8Base64Table[ ( pu8Str[ j + 1 ] & 15 ) << 2 ];
		pBuf[ i++ ] = BASE64_END;
		break;
	case 0:
		break;
	}
	pBuf[i]='\0';
	return m_as8BasicAuth;
}

RCODE VxProxy::SetProxy(	int iProxyType,	//Type of proxy	
							const char * pProxyHost,		//Proxy Host name
							const unsigned short u16ProxyPort,//Proxy port
							const char * pProxyAcctName,	//Proxy Account Name
							const char * pProxyAcctPwd )	//Proxy Account Pwd
{
	RCODE rc = 0;
	m_iProxyType = iProxyType;

	vx_assert( iProxyType >= 0 && iProxyType <= PROXYTYPE_HTTP11 );

	if( m_iProxyType )
	{
		strcpy( m_as8ProxyHost, pProxyHost );
		m_u16ProxyPort = u16ProxyPort;
		strcpy( m_as8ProxyAcctName, pProxyAcctName );
		strcpy( m_as8ProxyAcctPwd, pProxyAcctPwd );
		m_u32ProxyIp = VxResolveHostToIp( pProxyHost );
		if( 0 == m_u32ProxyIp )
		{
			wxMessageBox( "Could not resolve proxy host", "Proxy Error", wxICON_STOP );
			rc = -1;
		}
	}
	else
	{
		m_as8ProxyHost[ 0 ] = 0;
		m_as8ProxyAcctName[ 0 ] = 0;
		m_as8ProxyAcctPwd[ 0 ] = 0;
		m_u32ProxyIp = 0;
	}

	return rc;
}

RCODE VxProxy::OnConnect( VxSkt * sktBase, int iErrorCode ) 
{
	/*
	RCODE rc = PROXYERROR_INVALID_PARAM;
	int iResult;
	if(	iErrorCode && 
		WSAEWOULDBLOCK != iErrorCode )
	{
		return iErrorCode;
	}

	if( 0 == m_iProxyType )
	{
		//no proxy
		vx_assert( false );
		return rc;
	}
	if( VXSKT_STATE_PROXY_CONNECT == sktBase->m_u8State || 
		VXSKT_STATE_PROXY_LISTEN == sktBase->m_u8State )
	{
		if( sktBase->m_s8ProxyOpState )
		{
			//Somehow OnConnect has been called more than once
			vx_assert( false );
			return rc;
		}
		//convert ip address to dotted string
		//Send the initial request
		if( PROXYTYPE_SOCKS4 == m_iProxyType ||  
			PROXYTYPE_SOCKS4A == m_iProxyType )
		{ //SOCKS4 proxy

			//Send request
			vx_assert( sktBase->m_pPeerHost );
			int iPeerHostLen = (int)strlen( sktBase->m_pPeerHost );
			vx_assert( iPeerHostLen );

			char *command = new char [ 9 + iPeerHostLen + 1 ];
			memset( command, 0, 9 + iPeerHostLen + 1 );
			int len = 9;
			command[0]=4;
			command[1]=(VXSKT_STATE_PROXY_CONNECT == sktBase->m_u8State)?1:2; //CONNECT or BIND request
			memcpy( &command[2], &sktBase->m_u16PeerListenPort, 2 ); //Copy target address
			if( ! sktBase->m_u32PeerIp )
			{
				vx_assert( m_iProxyType == PROXYTYPE_SOCKS4A );
				//Set the IP to 0.0.0.x (x is nonzero)
				command[4]=0;
				command[5]=0;
				command[6]=0;
				command[7]=1;
				//Add host as URL
				strcpy( &command[9], sktBase->m_pPeerHost );
				len += iPeerHostLen + 1;
			}
			else
			{
				memcpy( &command[4],&sktBase->m_u32PeerIp, 4 );
			}
			iResult = send( sktBase->m_hSocket, command, len, 0 ); //Send command
			delete [] command;
			if( SOCKET_ERROR == iResult || iResult < len )
			{
				rc = WSAGetLastError();
				if( WSAEWOULDBLOCK != rc || 
					iResult < len)
				{
					sktBase->Close(679);
					return PROXYERROR_REQUESTFAILED;
				}
			}
			rc = 0;
		}
		else if( m_iProxyType ==PROXYTYPE_SOCKS5 )
		{ //SOCKS5 proxy
			//Send initialization request
			unsigned char command[ 10 ];
			memset( command, 0, 10 );
			command[ 0 ] = 5;
			//CAsyncProxySocket supports to logon types: No logon and 
			//cleartext username/password (if set) logon
			command[ 1 ]=m_bUseLogon ? 2 : 1; //Number of logon types
			command[ 2 ]=m_bUseLogon ? 2 : 0; //2=user/pass, 0=no logon
			int len = m_bUseLogon ? 4 : 3; //length of request
			iResult = send( sktBase->m_hSocket, (const char *)command, len, 0 ); //Send command
			if( SOCKET_ERROR == iResult || 
				iResult < len )
			{
				rc = WSAGetLastError();
				if( WSAEWOULDBLOCK != rc  || 
					iResult < len )
				{
					sktBase->Close(680);
					return PROXYERROR_REQUESTFAILED;
				}
			}
			rc = 0;
		}
		else if( PROXYTYPE_HTTP11 == m_iProxyType )
		{
			std::string str;
			
			if( ! m_bUseLogon )
			{
				StdStringFormat( str, "CONNECT %s:%d HTTP/1.1\r\nHost: %s:%d\r\n\r\n", 
						sktBase->m_pPeerHost,
						ntohs( sktBase->m_u16PeerListenPort ),
						sktBase->m_pPeerHost,
						ntohs( sktBase->m_u16PeerListenPort ));
			}
			else
			{
				StdStringFormat( str, "CONNECT %s:%d HTTP/1.1\r\nHost: %s:%d\r\n", 
						sktBase->m_pPeerHost,
						ntohs( sktBase->m_u16PeerListenPort ),
						sktBase->m_pPeerHost,
						ntohs( sktBase->m_u16PeerListenPort ));

				str += "Authorization: Basic ";
				str += GetBasicAuth();
				str += "\r\n\r\n";
				str += "Proxy-Authorization: Basic ";
				str += GetBasicAuth();
				str += "\r\n\r\n";
			}
			int numsent = send( sktBase->m_hSocket, str.c_str(), str.size(), 0 ); //Send command
			if( SOCKET_ERROR == numsent || 
				numsent < str.size() )
			{
				rc = WSAGetLastError();
				if( WSAEWOULDBLOCK != rc || 
					numsent < str.size() )
				{
					sktBase->Close(681);
					return PROXYERROR_REQUESTFAILED;
				}
			}
			rc = 0;
		}
		else
		{
			//invalid proxy Type
			vx_assert(FALSE);
		}
		//Now we'll wait for the response, handled in OnReceive
		sktBase->m_s8ProxyOpState++;
	}
	return rc;
	*/
	return 0;
}


RCODE VxProxy::OnReceive( VxSkt * sktBase, int iErrorCode) 
{
	/*
	RCODE rc  = PROXYERROR_INVALID_PARAM;
	if( 0 == m_iProxyType )
	{
		//no proxy
		vx_assert( false );
		return rc;
	}
	if(	iErrorCode && 
		WSAEWOULDBLOCK != iErrorCode )
	{
		return iErrorCode;
	}

	//Here we handle the responses from the SOCKS proxy
	if( !sktBase->m_s8ProxyOpState )
	{
		//if we didnt connect to proxy we shouldnt have data
		return PROXYERROR_INVALID_PARAM;
	}
	if(	PROXYTYPE_SOCKS4 == m_iProxyType ||  
		PROXYTYPE_SOCKS4A == m_iProxyType )
	{
		//Socks 4 or 4a
		if( 1 == sktBase->m_s8ProxyOpState )
		{

			int numread = sktBase->GetDataLength();				
			if( 8 == numread )
			{
				char * pRxBuf = sktBase->m_pBuf;
				if( pRxBuf[1]!=90 || pRxBuf[0]!=0)
				{
					//invalid response
					return PROXYERROR_REQUESTFAILED;
				}
				if( VXSKT_STATE_PROXY_CONNECT == sktBase->m_u8State )
				{
					//OK, we are connected with the remote server
					sktBase->ClearBuffer();
					sktBase->m_u8State = VXSKT_STATE_CONNECTED;
					sktBase->m_s8ProxyOpState = 0;
					sktBase->OnConnect( 0 );
					return 0;
				}
				else
				{
					//Listen socket created
					sktBase->m_s8ProxyOpState++;
//					unsigned long ip;
//					int port;
//					memcpy( &ip, &pRxBuf[ 4 ], 4 );
//					if( ! ip )
//					{  //No IP return, use the IP of the proxy server
//						SOCKADDR SockAddr;
//						memset(&SockAddr,0,sizeof(SockAddr));
//						int SockAddrLen=sizeof(SockAddr);
//						if( GetPeerName( &SockAddr, &SockAddrLen ) )
//						{
//							ip=((LPSOCKADDR_IN)&SockAddr)->sin_addr.S_un.S_addr;
//						}
//						else
//						{
//							return PROXYERROR_REQUESTFAILED;
//						}
//					}
//					memcpy( &port, &pRxBuf[2], 2 );
//					OnListenFinished(ip,port);					
					sktBase->ClearBuffer();
					sktBase->m_u8State = VXSKT_STATE_CONNECTED;
					sktBase->m_s8ProxyOpState = 0;
					sktBase->OnListenFinished( 0 );
					return 0;
				}
			}
			else
			{
				//unexpected length recieved
				vx_assert( false );
			}
		}
		else if( 2 == sktBase->m_s8ProxyOpState )
		{
			int numread = sktBase->GetDataLength();				
			if( 8 == numread )
			{
				char * pRxBuf = sktBase->m_pBuf;
				if( pRxBuf[1]!=90 || pRxBuf[0]!=0)
				{
					//invalid response
					return PROXYERROR_REQUESTFAILED;
				}
				//Connection to remote server established
				sktBase->ClearBuffer();
				sktBase->OnAccept(0);
				return 0;
			}
			else
			{
				//unexpected length recieved
				vx_assert( false );
			}
		}
	}
	else if( PROXYTYPE_SOCKS5 == m_iProxyType )
	{	
		//Socks 5
		if( 1 == sktBase->m_s8ProxyOpState )
		{
			int numread = sktBase->GetDataLength();				
			if( 2 != numread )
			{
				return PROXYERROR_INVALID_PARAM;
			}
			else
			{
				char * pRxBuf = sktBase->m_pBuf;
				if( pRxBuf[1]==0xFF || pRxBuf[0] != 5 )
				{
					return PROXYERROR_AUTHREQUIRED;
				}
				if(  pRxBuf[1] )
				{ //Auth needed
					if( pRxBuf[1]!=2)
					{ //Unknown auth type
						return PROXYERROR_AUTHTYPEUNKNOWN;
					}
					if( ! m_bUseLogon )
					{
						return PROXYERROR_AUTHNOLOGON;
					}
					//Send authentication
					int iProxyUserLen = (int)strlen( m_as8ProxyAcctName );
					int iProxyPwdLen = (int)strlen( m_as8ProxyAcctPwd );

					unsigned char *buffer=new unsigned char[3+iProxyUserLen+iProxyPwdLen+1];
					sprintf((char *)buffer,"  %s %s",
							m_as8ProxyAcctName,
							m_as8ProxyAcctPwd );
					buffer[0]=5;
					buffer[1]=static_cast<unsigned char>( iProxyUserLen );
					vx_assert( iProxyUserLen <= 255 );
					buffer[ 2+iProxyUserLen]=static_cast<unsigned char>(iProxyPwdLen);
					int len=3+iProxyUserLen+iProxyPwdLen;

					//clear old data
					sktBase->ClearBuffer();

					//send request
					int iResult = send( sktBase->m_hSocket, (const char *)buffer, len, 0 ); //Send command
					delete [] buffer;
					if( SOCKET_ERROR == iResult || iResult < len )
					{
						rc = WSAGetLastError();
						if( WSAEWOULDBLOCK != rc || iResult < len )
						{
							return PROXYERROR_REQUESTFAILED;
						}
					}
					sktBase->m_s8ProxyOpState++;
					return 0;
				}
			}
			//No auth needed
			//Send connection request
			int iPeerHostLen = (int)strlen( sktBase->m_pPeerHost );
			char *command=new char[ 10 + iPeerHostLen + 1 ];
			memset( command, 0, 10 + iPeerHostLen + 1 );
			command[0]=5;
			command[1]=( VXSKT_STATE_PROXY_CONNECT == sktBase->m_u8State ) ? 1 : 2;
			command[2]=0;
			command[3] = sktBase->m_u32PeerIp ? 1 : 3;
			int len = 4;
			if( sktBase->m_u32PeerIp )
			{
				memcpy( &command[len], &sktBase->m_u32PeerIp, 4 );
				len+=4;
			}
			else
			{
				command[len ] = iPeerHostLen;
				strcpy( &command[ len+1 ], sktBase->m_pPeerHost );
				len += iPeerHostLen + 1;
			}
			memcpy( &command[len], &sktBase->m_u16PeerListenPort, 2 );
			len += 2;
			sktBase->ClearBuffer();
			int iResult = send( sktBase->m_hSocket, command, len, 0 ); //Send command
			delete [] command;
			if( SOCKET_ERROR == iResult || iResult < len )
			{
				rc = WSAGetLastError();
				if( WSAEWOULDBLOCK != rc || iResult < len )
				{
					return PROXYERROR_REQUESTFAILED;
				}
			}
			sktBase->m_s8ProxyOpState += 2;
			return 0;
		}
		else if( 2 == sktBase->m_s8ProxyOpState )
		{//Response to the auth request
			int numread = sktBase->GetDataLength();
			if( 2 != numread )
			{
				return PROXYERROR_INVALID_PARAM;
			}
			else
			{
				char * pRxBuf = sktBase->m_pBuf;
				if( pRxBuf[1] !=0 )
				{
					return PROXYERROR_REQUESTFAILED;
				}
				int iPeerHostLen = (int)strlen( sktBase->m_pPeerHost );
				char *command=new char[ 10 + iPeerHostLen + 1 ];
				memset( command, 0, 10 + iPeerHostLen + 1 );
				command[0]=5;
				command[1]=( VXSKT_STATE_PROXY_CONNECT == sktBase->m_u8State ) ? 1 : 2;
				command[2]=0;
				command[3] = sktBase->m_u32PeerIp ? 1 : 3;
				int len=4;
				if( sktBase->m_u32PeerIp )
				{
					memcpy( &command[ len ], &sktBase->m_u32PeerIp ,4);
					len+=4;
				}
				else
				{
					command[len]= iPeerHostLen;
					strcpy( &command[ len + 1 ], sktBase->m_pPeerHost);
					len+= iPeerHostLen + 1;
				}
				memcpy(&command[len],& sktBase->m_u16PeerListenPort ,2);
				len+=2;

				sktBase->ClearBuffer();
				int iResult = send( sktBase->m_hSocket, command, len, 0 ); //Send command
				delete [] command;
				if( SOCKET_ERROR == iResult || iResult < len )
				{
					rc = WSAGetLastError();
					if( WSAEWOULDBLOCK != rc || iResult < len )
					{
						return PROXYERROR_REQUESTFAILED;
					}
				}
				sktBase->m_s8ProxyOpState++;
				return rc;				
			}
		}
		else if( 3 == sktBase->m_s8ProxyOpState )
		{//Response to the connection request
			int numread = sktBase->GetDataLength();
			if( 5 > numread )
			{
				return PROXYERROR_INVALID_PARAM;
			}
			else
			{
				char * pRxBuf = sktBase->m_pBuf;

				//Check for errors
				if( pRxBuf[1]!=0 || pRxBuf[0]!=5)
				{
					return  PROXYERROR_REQUESTFAILED;
				}

				//Check which kind of address the response contains
//				if( 5 == numread )
//				{
//					if (m_pRecvBuffer[3]==1)
//						m_nRecvBufferLen=10;
//					else
//					{
//						char *tmp=new char[m_nRecvBufferLen+=m_pRecvBuffer[4]+2];
//						memcpy(tmp,m_pRecvBuffer,5);
//						delete [] m_pRecvBuffer;
//						m_pRecvBuffer=tmp;
//						m_nRecvBufferLen+=m_pRecvBuffer[4]+2;
//					}
//					return rc;
//				}
				
				if( VXSKT_STATE_PROXY_CONNECT == sktBase->m_u8State )
				{
					//OK, we are connected with the remote server
					sktBase->ClearBuffer();
					sktBase->m_u8State = VXSKT_STATE_CONNECTED;
					sktBase->m_s8ProxyOpState = 0;
					sktBase->OnConnect( 0 );
					return 0;
				}
				else
				{
					//Listen socket created
					sktBase->m_s8ProxyOpState++;
					unsigned long ip;
					int port;
					vx_assert( 1 == pRxBuf[ 3 ] );
					memcpy( &ip, &pRxBuf[4], 4);
					memcpy( &port, &pRxBuf[8], 2);
					sktBase->ClearBuffer();
					sktBase->m_u8State = VXSKT_STATE_CONNECTED;
					sktBase->m_s8ProxyOpState = 0;
//					OnListenFinished( ip, port );
					sktBase->OnListenFinished( 0 );
				}
			}
		}
		else if( 4 == sktBase->m_s8ProxyOpState )
		{
			int numread = sktBase->GetDataLength();
			if( 10 != numread )
			{
				return PROXYERROR_INVALID_PARAM;
			}
			else
			{
				char * pRxBuf = sktBase->m_pBuf;

				//Check for errors
				if( pRxBuf[1]!=0 )
				{
					return  PROXYERROR_REQUESTFAILED;
				}
				//Connection to remote server established
				sktBase->ClearBuffer();
				sktBase->m_u8State = VXSKT_STATE_CONNECTED;
				sktBase->m_s8ProxyOpState = 0;
				sktBase->OnAccept( 0 );
				return 0;
			}
		}
	}
	else if( PROXYTYPE_HTTP11 == m_iProxyType )
	{
		//=== http proxy ===//
		char buffer[2];
		buffer[1]=0;
		int iDataLen = sktBase->GetDataLength();
		std::string m_strBuffer;
		for( int i = 0; i < iDataLen; i++ )
		{
			char * pRxBuf = sktBase->m_pBuf;
			buffer[ 0 ] = pRxBuf[ i ];

			//Response begins with HTTP/
			m_strBuffer+=buffer;
			std::string start="HTTP/";
			start=StdStringLeft( start, m_strBuffer.size());
			if (start!= StdStringLeft( m_strBuffer, start.size()))
			{
				return PROXYERROR_REQUESTFAILED;
			}
			int pos = StdStringFind( m_strBuffer, "\r\n");
			if( pos != -1 )
			{
				int pos2 = StdStringFind( m_strBuffer, " ");
				if (pos2==-1 || m_strBuffer[pos2+1]!='2' || pos2>pos)
				{
					return PROXYERROR_REQUESTFAILED;
				}
			}
			if( StdStringRight( m_strBuffer, 4) == "\r\n\r\n") //End of the HTTP header
			{
				sktBase->ClearBuffer();
				sktBase->m_u8State = VXSKT_STATE_CONNECTED;
				sktBase->m_s8ProxyOpState = 0;
				sktBase->OnConnect( 0 );
				return 0;
			}
		}
		return PROXYERROR_REQUESTFAILED;

	}
	return rc;
	*/
	return 0;

}
char * VxProxy::DescribeErr( long rc )
{
static	char as8Buf[ 200 ];
	switch( rc )
	{
	case PROXYERROR_NOCONN:
		return "Unable to connect to proxy server";

	case PROXYERROR_REQUESTFAILED:
		return "Proxy Request failed.. can't send data";
	case PROXYERROR_AUTHREQUIRED:
		return "Proxy Authentication required";
	case PROXYERROR_AUTHTYPEUNKNOWN:
		return "Proxy Authenication type unknown or not supported";
	case PROXYERROR_AUTHFAILED:
		return "Proxy Authentication failed";
	case PROXYERROR_AUTHNOLOGON:
		return "Proxy Logon Failed";
	case PROXYERROR_CANTRESOLVEHOST:
		return "Could not resolve Proxy host";
	case PROXYERROR_INVALID_PARAM:
		return "Proxy Invalid Param";
	default:
		sprintf( as8Buf, "Unknown Proxy Error %d", rc );
		return as8Buf;
	}
}
#endif // VXNET_ENABLE_PROXY



