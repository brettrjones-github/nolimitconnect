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
// http://www.nolimitconnect.org
//============================================================================

#include "InetAddress.h"

#include <string>

bool VxResolveHostToIp(	const char * pUrl,			// web name to resolve
						std::string & strRetIp,		// return dotted string 
						uint16_t& u16RetPort );			// return port

bool VxResolveHostToIp(	const char * pHostOnly,			//web host name to resolve
					    uint16_t u16Port,
						InetAddress& oRetIp );

bool VxResolveHostToIps(	const char * pHostOnly,			//web host name to resolve
							uint16_t u16Port,
							std::vector<InetAddress>& aoRetIps );


