#pragma once
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
#include <NetLib/VxSktConnectSimple.h>

class VxUrl;
class VxHttpParse;

class UpnpTcpSkt : public VxSktConnectSimple
{
public:
	bool						requestFile(	VxUrl&			url, 
												VxHttpParse&	parser,
												int				connectTimeoutMs		= WEBSITE_CONNECT_TIMEOUT,
												int				recieveDataTimeoutMs	= SKT_RECEIVE_TIMEOUT);

	bool						getHttpHeaderAndContents( VxHttpParse& oParser, int recieveDataTimeoutMs	= SKT_RECEIVE_TIMEOUT );
	bool						getHttpHeader( VxHttpParse& oParser,int recieveDataTimeoutMs = SKT_RECEIVE_TIMEOUT );
	bool						getRemainingContents( VxHttpParse& oParser, int recieveDataTimeoutMs = SKT_RECEIVE_TIMEOUT );

	bool						postRequestAndData( VxUrl& oUrl, std::string& strRequest, std::string& strData, int connectTimeoutMs = SKT_CONNECT_TIMEOUT  );
	bool						postRequest( std::string& strHost, int iHostPort, const char* pRequest, int iRequestLen, int connectTimeoutMs = SKT_CONNECT_TIMEOUT );
};