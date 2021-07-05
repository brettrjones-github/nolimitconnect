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

#include "NetworkEventBase.h"

#include <string>

class NetworkEventAvail : public NetworkEventBase
{
public:
	NetworkEventAvail( NetworkStateMachine& stateMachine, const char * lclIp, bool isCellularNetwork );

	const char *				getLclIp( void )							{ return m_LclIp.c_str();  }
	bool						getIsCellNetwork( void )					{ return m_bIsCellNetwork; }

	virtual void				runNetworkEvent( void );

protected:
	//=== vars ===//
	std::string					m_LclIp; 
	bool						m_bIsCellNetwork;
};


