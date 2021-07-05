#pragma once
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

#include <ptop_src/ptop_engine_src/HostMgr/HostDb.h>
#include <CoreLib/VxDefs.h>

class NetServicesMgr;
class NetServiceUtils;
class VxSktBase;
class VxGUID;
class P2PEngine;
class NetServiceHdr;

class NetServiceHost
{
public:
	NetServiceHost( P2PEngine& engine, NetServicesMgr& netServicesMgr, NetServiceUtils& netServiceUtils );
	virtual ~NetServiceHost();

	void						netServiceHostStartup( void );
	void						netServiceHostShutdown( void );

	HostDb&					    getHostDb( void )						{ return m_HostDb; }

	RCODE						handleNetCmdHostReq( VxSktBase * sktBase, NetServiceHdr& netServiceHdr );
	RCODE						handleNetCmdHostReply( VxSktBase * sktBase, NetServiceHdr& netServiceHdr );

protected:
	bool						verifyHostList( HostList * anchorList, int contentDataLen );

	P2PEngine&					m_Engine;
	NetServicesMgr&				m_NetServicesMgr;
	NetServiceUtils&			m_NetServiceUtils;
	HostDb					    m_HostDb;

private:
	NetServiceHost() = delete; // don't allow default constructor
	NetServiceHost(const NetServiceHost&) = delete; // don't allow copy constructor
};
