//============================================================================
// Copyright (C) 2012 Brett R. Jones 
// All Rights Reserved ( After My Death I Brett R. Jones Issue this source code to Public Domain )
//
// You may not modify or redistribute this code for any reason 
// without written consent of Brett R. Jones
//
// This code is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
//
// brettjones1900@gmail.com
// http://www.p2panarchy.com
//============================================================================

#include "UpnpTcpSkt.h"
#include <CoreLib/VxUrl.h>
#include <NetLib/VxHttpRequest.h>
#include <NetLib/VxHttpParse.h>
#include <CoreLib/VxBuffer.h>
#include <CoreLib/VxDebug.h>

namespace
{
	int SKT_CONTENT_BUF_LEN = 4096;
}

//============================================================================
bool UpnpTcpSkt::requestFile( VxUrl& oUrl, VxHttpParse& oParser, int connectTimeoutMs, int recieveDataTimeoutMs )
{
	bool bResult = false;
	connectTo( oUrl.getHost(), oUrl.getPort(), connectTimeoutMs );
	if( isConnected() )
	{
		VxHttpRequest request( oUrl );
		std::string strRequest;
		request.buildGetHeader( strRequest );
		//strRequest += "User-Agent: Microsoft-Windows/6.1 UPnP/1.0\r\n";
		strRequest += VxHttpRequest::HTTP_CRLF;

		sendData( strRequest.c_str(), strRequest.length() );

		bResult = getHttpHeaderAndContents( oParser, recieveDataTimeoutMs );

		closeSkt( 565 );
	}
	else
	{
		LogMsg( LOG_DEBUG, "Failed to connect to %s port %d\n", oUrl.getHost(), oUrl.getPort() );
	}

	return bResult;
}

//============================================================================
bool UpnpTcpSkt::getHttpHeaderAndContents( VxHttpParse& oParser, int recieveDataTimeoutMs )
{
	bool bResult = getHttpHeader( oParser, recieveDataTimeoutMs );
	if( bResult )
	{
		bResult = getRemainingContents( oParser, recieveDataTimeoutMs );
	}

	return bResult;
}

//============================================================================
bool UpnpTcpSkt::getHttpHeader( VxHttpParse& oParser, int recieveDataTimeoutMs )
{
	oParser.getContentsBuffer().clear();
	if( false == isConnected() )
	{
		return false;
	}
	// get header
	int iBytesRxed = 0;
	bool bGotCrLf = false;
	VxBuffer& rxBuffer = oParser.getContentsBuffer();
	RCODE rc = recieveData( oParser.getContentsBuffer().getWriteBuffer(400 + 8), 
		400 + 8, 
		&iBytesRxed, 
		recieveDataTimeoutMs,
		true,
		&bGotCrLf );
	if( ( 0 == rc ) && 
		bGotCrLf && 
		iBytesRxed > 0 )
	{
		// header has arrived
		rxBuffer.bytesWritten( iBytesRxed );
		if( oParser.parseHttpHeaderFromContents() )
		{
			return true;
		}
	}
	return false;
}

//============================================================================
bool UpnpTcpSkt::getRemainingContents( VxHttpParse& oParser, int recieveDataTimeoutMs )
{
	if( false == isConnected() )
	{
		return false;
	}
	int iContentLen = oParser.getHttpContentLen();
	VxBuffer& rxBuffer = oParser.getContentsBuffer();

	int iLenRxed = rxBuffer.length();

	if( iContentLen )
	{
		// content length was given

		while( iLenRxed < iContentLen )
		{
			int iLenToGet = iContentLen - iLenRxed;
			iLenToGet = ( iLenToGet > SKT_CONTENT_BUF_LEN ) ? SKT_CONTENT_BUF_LEN : iLenToGet;

			int iBytesRxed = 0;

			RCODE rc = recieveData(	rxBuffer.getWriteBuffer( iLenToGet + 8 ),					// data buffer to read into
				iLenToGet + 8,						// length of data	
				&iBytesRxed,				// number of bytes actually received
				recieveDataTimeoutMs,	// timeout attempt to receive
				false );		// if true then abort receive when \r\n\r\n is received

			if( ( 0 == rc ) &&
				 ( 0 < iBytesRxed ) )
			{
				rxBuffer.bytesWritten( iBytesRxed );
				iLenRxed += iBytesRxed;
			}
			else
			{
				LogMsg( LOG_DEBUG, "UpnpTcpSkt::getRemainingContents error\n");
				break;
			}
		}
		return ( iLenRxed >= iContentLen ) ? true : false;
	}
	else
	{
		 // no content len given
		int iBytesRxed = 0;
		do
		{
			int iLenToGet = 1024;

			iBytesRxed = 0;

			RCODE rc = recieveData(	rxBuffer.getWriteBuffer( iLenToGet + 8 ),					// data buffer to read into
                                    iLenToGet + 8,						// length of data
                                    &iBytesRxed,				// number of bytes actually received
                                    recieveDataTimeoutMs,	// timeout attempt to receive
                                    false );		// if true then abort receive when \r\n\r\n is received

			if( 0 < iBytesRxed )
			{
				rxBuffer.bytesWritten( iBytesRxed );
				iLenRxed += iBytesRxed;
			}

            if( rc )
            {
                LogMsg( LOG_ERROR, "ERROR: UpnpTcpSkt::getRemainingContents error %d %s\n", rc, VxDescribeSktError( rc ) );
            }
		}
		while( iBytesRxed > 0 );
		return true;
	}
}

//============================================================================
bool UpnpTcpSkt::postRequestAndData( VxUrl& oUrl, std::string& strRequest, std::string& strData, int connectTimeoutMs )
{
	bool bResult = false;
	connectTo( oUrl.getHost(), oUrl.getPort(), connectTimeoutMs );
	if( isConnected() )
	{
		if( 0 == sendData( strRequest.c_str(), (int)strRequest.length() ) )
		{
			if( 0 == sendData( strData.c_str(), (int)strData.length() ) )
			{
				bResult = true;
			}
		}
	}

	return bResult;
}

//============================================================================
bool UpnpTcpSkt::postRequest( std::string& strHost, int iHostPort, const char* pRequest, int iRequestLen, int connectTimeoutMs )
{
	bool bResult = false;
	if( false == isConnected() )
	{
		connectTo( strHost.c_str(), iHostPort, connectTimeoutMs );
	}
	if( isConnected() )
	{
		if( 0 == sendData( pRequest, iRequestLen ) )
		{
			bResult = true;
		}
	}

	return bResult;
}
