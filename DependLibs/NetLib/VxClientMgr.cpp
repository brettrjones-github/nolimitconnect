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
// http://www.nolimitconnect.org
//============================================================================

#include "VxClientMgr.h"
#include "VxSktConnect.h"

//============================================================================
VxClientMgr::VxClientMgr()
{
	m_eSktMgrType = eSktMgrTypeTcpConnect;
}

//============================================================================
//! make a new socket... give derived classes a chance to override
VxSktBase * VxClientMgr::makeNewSkt( void )
{ 
	return new VxSktConnect(); 
}

//============================================================================
//! Connect to ip or url and return socket.. if cannot connect return NULL
VxSktConnect * VxClientMgr::connectTo(	const char *	pIpOrUrl,				// remote ip or url 
										uint16_t		u16Port,				// port to connect to
										int				iTimeoutMilliSeconds )	// milli seconds before connect attempt times out
{
	if( NULL ==  m_pfnUserReceive )
	{
		LogMsg( LOG_INFO, "VxClientMgr::VxConnectTo: you must call setReceiveCallback first\n" );
		vx_assert( m_pfnUserReceive );
	}

	VxSktConnect * sktBase	= (VxSktConnect *)makeNewSkt();
	sktBase->m_SktMgr		= this;
	RCODE rc = sktBase->connectTo(	m_LclIp,
									pIpOrUrl, 
									u16Port, 
									iTimeoutMilliSeconds );

	if( rc )
	{
#ifdef DEBUG_SKTS
		LogMsg( LOG_INFO, "VxClientMgr::VxConnectTo: error %d\n", rc );
#endif // DEBUG_SKTS
		delete sktBase;
		return NULL;
	}

	addSkt( sktBase );
	return sktBase;
}
