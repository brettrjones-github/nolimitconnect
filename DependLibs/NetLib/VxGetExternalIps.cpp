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

#include "VxGetExternalIps.h"
#include "VxResolveHost.h"
#include "VxSktUtil.h"

#include <CoreLib/VxDebug.h>
#include <CoreLib/VxGlobals.h>

#include <string.h>
#include <stdio.h>

#define ANCHOR_RX_DATA_TIMEOUT 5000

#define WHATS_MY_IP_POST_STR "POST /%s HTTP/1.1\r\n\
Content-Type: application/x-www-form-urlencoded\r\n\
Host: %s\r\n\
Content-Length: %d\r\n\r\n%s"

#define WHATS_MY_IP_DATA_STR "anchor_action=%s&contact_id=%s"

//============================================================================
//! thread function to announce to web anchor
static bool WhatsMyIP(	InetAddrAndPort& oLclAddr, 
                        InetAddrAndPort& oRmtAddr, 
						uint16_t u16Port,
						const char* pHost, 
						const char* pWebPage, 
						std::string& strRetMyIpAddr )
{
	if( VxIsAppShuttingDown() )
	{
		return false;
	}

	SOCKET oSkt = VxConnectTo(	oLclAddr, 
								oRmtAddr,					// remote ip or url
								u16Port );					// port to connect to
	if( INVALID_SOCKET == oSkt )
	{
		LogMsg( LOG_ERROR, "ERROR: failed to connect to whats my ip website\n");
		return false;
	}

	LogMsg( LOG_INFO, "--- Connected to whats my ip website\n" );
	char as8Content[2048];
	char as8TxRx[2048];

	// generate the form data
	sprintf( as8Content, WHATS_MY_IP_DATA_STR,
		"List", "MyP2PWeb" );

	// generate post 
	sprintf( as8TxRx, WHATS_MY_IP_POST_STR, 
		pWebPage, 
		pHost, 
        (int)strlen( as8Content ),
		as8Content );

	RCODE rc = VxSendSktData( oSkt, as8TxRx, (int)strlen( as8TxRx), SKT_SEND_TIMEOUT );
	if( rc )
	{
		LogMsg( LOG_ERROR, "whatsMyIPv4: ERROR: failed to post to %s %s\n", pHost, VxDescribeSktError( rc ));
		VxCloseSktNow( oSkt );
		return false;
	}
	//writeWholeFile("post_send.txt", (void *)as8TxRx, (uint32_t)strlen( as8TxRx ) );
	int iRxed = 0;
	bool bGotCrLfCrLf = false;
	int iMaxToReceive = (int)(sizeof( as8TxRx )-8);
	rc = VxReceiveSktData(	oSkt,
		as8TxRx,				// data buffer to read into
		iMaxToReceive,			// length of data	
		&iRxed,					// number of bytes actually received
		ANCHOR_RX_DATA_TIMEOUT,	// timeout attempt to receive
		true, 					// if true then abort receive when \r\n\r\n is received
		&bGotCrLfCrLf );

	if( iRxed <= 0 )
	{
		LogMsg( LOG_ERROR,"whatsMyIPv4: Disconnected or read error %d\n", rc ); 
		VxCloseSktNow( oSkt );
		return false; 
	}

	VxCloseSktNow( oSkt );
	as8TxRx[iRxed] = 0; 
	//LogMsg( LOG_INFO, as8TxRx );
	//#ifdef _DEBUG
	//	std::string strPostReply = "F:/post_reply.txt";
	//	writeWholeFile( strPostReply.c_str(), as8TxRx, iRxed );
	//#endif // _DEBUG

	//=== parse the data ===//
	// first entry is ourself.. get real ip
	char * pTemp = strstr( as8TxRx, "!MyIPv4," );
	if( NULL == pTemp )
	{
		// no valid entries
		LogMsg( LOG_ERROR, "whatsMyIPv4 Failed to find ip in website response\n");
		return false;
	}
	int iAmtUsed = 0;
	if( 0 != strncmp( pTemp, "!MyIPv4,", 8 ) )
	{
		LogMsg( LOG_ERROR, "whatsMyIPv4 Failed to find ip in website response\n");
		return false;
	}
	iAmtUsed += 8;

	// ip address
	pTemp = strtok( (char *)(&pTemp[ iAmtUsed ]), "," );
	if( (NULL == pTemp) || 
		(strlen( pTemp ) < 3 ) ||
		(strlen( pTemp ) > 17 ) )
	{
		LogMsg( LOG_ERROR, "RcWhatsMyIp::parseErrpr: ip\n");
		return false;
	}
	strRetMyIpAddr = pTemp;
	iAmtUsed += (int)strlen( pTemp ) + 1;
	return true;
}

//============================================================================
bool VxGetExternalIps( const char* pUrl, std::vector<std::string>& aoRetExternalIps )
{
	if( NULL == pUrl )
	{
		LogMsg( LOG_ERROR, "--- VxGetExternalIps: NULL URL \n" );
		return false;
	}

	std::string strHost;
	std::string strWebPage;
	uint16_t u16Port = 0;

	if( false == VxSplitHostAndFile( pUrl, strHost, strWebPage, u16Port ) )
	{
		return false;
	}

	if( 0 == strWebPage.length() )
	{
		LogMsg( LOG_ERROR, "---  VxGetExternalIps: Zero Len URL \n" );
		return false;
	}

	LogMsg( LOG_INFO, "--- VxGetExternalIps: Attempting to connect to %s...\n", pUrl);

	// get both ipv4 and ipv6 addresses of website
	std::vector<InetAddress> aoIps; 
	VxResolveHostToIps( strHost.c_str(), u16Port, aoIps );

    InetAddrAndPort oLclIp;
	std::string strExternalIp;
	for( unsigned int i = 0; i < aoIps.size(); i++ )
	{
        InetAddrAndPort rmtIp( aoIps[i] );
		strExternalIp = "";
		WhatsMyIP(	oLclIp, 
                    rmtIp, 
					u16Port,
					strHost.c_str(),
					strWebPage.c_str(), 
					strExternalIp );
		if( strExternalIp.length() )
		{
			aoRetExternalIps.push_back( strExternalIp );
		}
	}

    std::vector<InetAddress> aoLclIps;
    VxGetLocalIps( aoLclIps );
	std::vector<InetAddress>::iterator iter;
	for( iter = aoLclIps.begin(); iter != aoLclIps.end(); ++iter )
	{
		if( (*iter).isIPv6GlobalAddress() )
		{
			std::string strGlobalIp;
			strGlobalIp = (*iter).toStdString();

			bool bAlreadyExists = false;
			std::vector<std::string>::iterator oIter2;
			for( oIter2 = aoRetExternalIps.begin(); oIter2 != aoRetExternalIps.end(); ++oIter2 )
			{
				std::string strThisIp = *oIter2;
				if( strGlobalIp == strThisIp )
				{
					bAlreadyExists = true;
					break;
				}
			}
			if( false == bAlreadyExists )
			{
				aoRetExternalIps.push_back( strGlobalIp );
			}
		}
	}
	
	return aoRetExternalIps.size() ? true : false;
}
