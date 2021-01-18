#pragma once
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

#include "VxSktBaseMgr.h"

class VxSktConnect;

// implements a manager to manage connect sockets
class VxClientMgr : public VxSktBaseMgr
{
public:
	VxClientMgr();
	virtual ~VxClientMgr() = default;

	virtual VxSktBase *		makeNewSkt( void );

	//! Connect to ip or URL and return socket.. if cannot connect return NULL
	virtual VxSktConnect *	connectTo(	const char *	pIpOrUrl,						// remote ip or url 
										uint16_t	    u16Port,						// port to connect to
										int				iTimeoutMilliSeconds = 1000 );	// seconds before connect attempt times out
};

