//============================================================================
// Copyright (C) 2009 Brett R. Jones 
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

#include "P2PEngine.h"

#include <string.h>
#include <stdio.h>
#include <stdarg.h>

//============================================================================
//! called if hacker offense is detected
void P2PEngine::hackerOffense(	VxNetIdent *	poContactIdent,			// users identity info ( may be null if not known then use ipAddress )
                                EHackerLevel	hackLevel,			    // 1=severe 2=medium 3=suspicious
								InetAddress		IpAddr,					// ip address if identity not known
								const char *	pMsg, ... )				// message about the offense
{
	char szBuffer[4096];
    szBuffer[0] = 0;
    if( pMsg )
    {
        va_list argList;
        va_start( argList, pMsg );
        vsnprintf( szBuffer, sizeof( szBuffer ), pMsg, argList );
        szBuffer[sizeof( szBuffer ) - 1] = 0;
        va_end( argList );
    }

    InetAddress oIpAddr = IpAddr;
    if( poContactIdent )
    {
        oIpAddr = poContactIdent->getOnlineIpAddress();
    }

	std::string strIp = oIpAddr.toStdString();
	LogMsg( LOG_SEVERE, "Hacker Offense %s: ip %s %s\n", DescribeHackerLevel( hackLevel ), strIp.c_str(), szBuffer );
}


//============================================================================
//! called if hacker offense is detected
void P2PEngine::hackerOffense(	VxNetIdent *	poContactIdent,			// users identity info ( may be null if not known then use ipAddress )
                                EHackerLevel	hackLevel,			    // 1=severe 2=medium 3=suspicious
                                const char *	pMsg, ... )				// message about the offense
{
    char szBuffer[4096];
    szBuffer[0] = 0;
    if( pMsg )
    {
        va_list argList;
        va_start( argList, pMsg );
        vsnprintf( szBuffer, sizeof( szBuffer ), pMsg, argList );
        szBuffer[sizeof( szBuffer ) - 1] = 0;
        va_end( argList );
    }

    LogMsg( LOG_SEVERE, "Hacker Offense %s: %s\n", DescribeHackerLevel( hackLevel ), szBuffer );
}
