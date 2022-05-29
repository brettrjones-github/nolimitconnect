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
// http://www.nolimitconnect.org
//============================================================================
#include "VxResolveHost.h"
#include "VxSktUtil.h"

#include <CoreLib/VxParse.h>
#include <CoreLib/VxGlobals.h>
#include <CoreLib/VxDebug.h>
#include <CoreLib/VxTimer.h>

#include <stdio.h>
#include <memory.h>

#ifdef TARGET_OS_WINDOWS
#include "Winsock2.h"
#include <Ws2tcpip.h>  
#else
#include <ctype.h>
#endif

//============================================================================
bool VxResolveHostToIp(	const char *	pUrl,			// web name to resolve
						std::string &	strRetIp,		// return dotted ip string 
						uint16_t&		u16RetPort )	// return port
{
	std::string strHost;
	std::string strFileName;

    strRetIp.clear();
    u16RetPort = 0;

	if( VxSplitHostAndFile( pUrl,
		strHost,
		strFileName,
		u16RetPort ) )
	{
		if( (':' == strHost.c_str()[0]) ||
			( isdigit( strHost.c_str()[0]) ) )
		{
			// already a ip address
			strRetIp = strHost;
			return true;
		}

		InetAddrAndPort oAddr;
		if( VxResolveHostToIp(	strHost.c_str(),			//web host name to resolve
			u16RetPort,
			oAddr ))
		{
			strRetIp = oAddr.toStdString();
			return true;
		}
	}
	return false;
}

//============================================================================
bool VxResolveHostToIp(	const char *	pHostOnly,			//web host name to resolve
						uint16_t		u16Port,
						InetAddress&	oRetIp )
{
	if( VxIsAppShuttingDown() )
	{
		return false;
	}

    if( ( 0 == pHostOnly ) || ( 0 == u16Port ) )
    {
        LogMsg( LOG_DEBUG, "VxResolveHostToIp bad parameter " );
        return false;
    }

	//resolve host into ip address
	struct addrinfo *res, *aip;
	struct addrinfo hints;
	//SOCKET sock = -1;
	int error;

	memset(&hints, 0, sizeof(hints));
	hints.ai_flags = AI_ADDRCONFIG;
	//hints.ai_family = AF_UNSPEC;
	//hints.ai_family = AF_INET
	//hints.ai_family = AF_INET6
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;


	char as8Port[ 10 ];
	sprintf( as8Port, "%d", u16Port );
	VxTimer resolveTimer;
#ifdef DEBUG_NETLIB
	LogMsg( LOG_INFO, "Resolving %s\n", pHostOnly ); 
#endif// DEBUG_NETLIB

	error = getaddrinfo(pHostOnly, as8Port, &hints, &res);
	if (error != 0) 
	{
        #ifdef DEBUG_NETLIB
            LogMsg( LOG_ERROR,
                    "VxResolveHostToIp: getaddrinfo: %s for host %s service %s\n",
                    gai_strerror(error), pHostOnly, as8Port );
        #endif// DEBUG_NETLIB
		return false;
	}

	bool bFoundAddr = false;;
	int tryCnt = 0;
	/* Try all returned addresses until one works */
	for( aip = res; aip != NULL; aip = aip->ai_next ) 
	{
		tryCnt++;
		if( VxIsAppShuttingDown() )
		{
			break;
		}

		InetAddress lclIp( VxGetLclIpAddress().c_str() );				// local ip address
		InetAddrAndPort rmtIp;				// local ip address

		rmtIp.setIp( *((struct sockaddr_storage *)aip->ai_addr) );
		std::string strRmtIp = rmtIp.toStdString();
        #ifdef DEBUG_NETLIB
        double elapsed = resolveTimer.elapsedSec();
            LogMsg( LOG_INFO, "Resolve %s to %s:%d in %3.3f sec on try %d\n",
                pHostOnly,
                strRmtIp.c_str(), u16Port,
                elapsed,
                tryCnt );
        #endif// DEBUG_NETLIB
		// NOTE:should do connect to verify but if internet connection is too overloaded it may fail
		// and the resolved address seems to be always correct so this is commented out for now
		/*
		sock		= VxConnectTo( lclIp, rmtIp, strRmtIp.c_str(), u16Port, 15000 );
		if( INVALID_SOCKET != sock )
		{
			bFoundAddr = true;
			VxCloseSkt( sock );
			oRetIp.setIp( strRmtIp.c_str() );
			double connectSuccessTime = resolveTimer.elapsedSec();
			LogMsg( LOG_INFO, "Resolve Connect Success to %s:%d in %3.3f sec on try %d\n", 
				oRetIp.toStdString().c_str(), u16Port,
				connectSuccessTime - elapsed,
				tryCnt );
			break;
		}
		else
		{
			double connectFailTime = resolveTimer.elapsedSec();
			LogMsg( LOG_INFO, "Resolve Connect Failed to %s:%d in %3.3f sec on try %d error %s\n", 
				strRmtIp.c_str(), u16Port,
				connectFailTime - elapsed,
				tryCnt,
				VxDescribeSktError( VxGetLastError() ) );
			continue;
		}
		*/
		if( rmtIp.isValid() 
			&& ( false == rmtIp.isLoopBack() ) )
		{
			bFoundAddr = true;
			oRetIp.setIp( strRmtIp.c_str() );
            #ifdef DEBUG_NETLIB
            double connectSuccessTime = resolveTimer.elapsedSec();
                LogMsg( LOG_INFO, "Resolve Connect Success to %s:%d in %3.3f sec on try %d\n",
                        oRetIp.toStdString().c_str(), u16Port,
                        connectSuccessTime - elapsed,
                        tryCnt );
            #endif// DEBUG_NETLIB
			break;

		}
		else
		{
        #ifdef DEBUG_NETLIB
            double connectFailTime = resolveTimer.elapsedSec();
                LogMsg( LOG_INFO, "Resolve Connect Failed to %s:%d in %3.3f sec on try %d\n",
														pHostOnly, 
														u16Port,
														connectFailTime - elapsed,
                                                        tryCnt );
        #endif // DEBUG_NETLIB

			continue;
		}
	}

	freeaddrinfo(res);
	return bFoundAddr;
}

//============================================================================
bool VxResolveHostToIps(	const char * pHostOnly,			//web host name to resolve
							uint16_t u16Port,
							std::vector<InetAddress>& aoRetIps )
{
	if( VxIsAppShuttingDown() )
	{
		return false;
	}

	//resolve host into ip address
	struct addrinfo *res, *aip;
	struct addrinfo hints;
	SOCKET sock = -1;
	int error;

	aoRetIps.clear();

	/* Get host address. Any type of address will do. */
	memset(&hints, 0, sizeof(hints));
	hints.ai_flags = AI_ADDRCONFIG;
	hints.ai_socktype = SOCK_STREAM;

	char as8Port[ 10 ];
	sprintf( as8Port, "%d", u16Port );

	//LogMsg( LOG_INFO, "VxResolveHostToIps %s\n", pHostOnly ); 
	error = getaddrinfo(pHostOnly, as8Port, &hints, &res);
	if (error != 0) 
	{
        #ifdef DEBUG_NETLIB
		LogMsg( LOG_ERROR,
			"getaddrinfo: %s for host %s service %s\n",
			gai_strerror(error), pHostOnly, as8Port);
        #endif // DEBUG_NETLIB
		return false;
	}
	/* Try all returned addresses until one works */
	for (aip = res; aip != NULL; aip = aip->ai_next) 
	{
		/*
		* Open socket. The address type depends on what
		* getaddrinfo() gave us.
		*/
		sock = socket(aip->ai_family, aip->ai_socktype, aip->ai_protocol);
		if (sock == -1) 
		{
            #ifdef DEBUG_NETLIB
                LogMsg( LOG_ERROR, "VxResolveHostToIps: could not create socket\n" );
            #endif // DEBUG_NETLIB
			freeaddrinfo(res);
			continue;
		}

		/* Connect to the host. */
		if( -1 == connect(sock, aip->ai_addr, (int)aip->ai_addrlen) ) 
		{
			VxCloseSkt( sock );
			continue;
		}

		// found a ip
		InetAddress oAddr;
		oAddr.setIp( *((struct sockaddr_storage *)aip->ai_addr) );
		aoRetIps.push_back( oAddr );
	}

	freeaddrinfo(res);
	return aoRetIps.size() ? true : false;
}
