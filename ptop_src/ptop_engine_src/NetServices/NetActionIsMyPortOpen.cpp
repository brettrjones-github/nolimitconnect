//============================================================================
// Copyright (C) 2014 Brett R. Jones
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

#include "NetActionIsMyPortOpen.h"
#include "NetServicesMgr.h"

#include <ptop_src/ptop_engine_src/P2PEngine/P2PEngine.h>

#include <CoreLib/VxParse.h>

//============================================================================
NetActionIsMyPortOpen::NetActionIsMyPortOpen( NetServicesMgr& netServicesMgr )
: NetActionBase( netServicesMgr )
{
}

//============================================================================
void NetActionIsMyPortOpen::doAction( void )
{
	std::string myIp = "";
	ENetCmdError eResult = m_NetServicesMgr.doIsMyPortOpen( myIp, false );
	LogModule( eLogIsPortOpenTest, LOG_INFO, "NetActionIsMyPortOpen::doAction result %d ip %s", eResult, myIp.c_str() );
	m_NetServicesMgr.netActionResultIsMyPortOpen( eResult, myIp );
}


