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
// http://www.nolimitconnect.org
//============================================================================

#include "PingResponseServer.h"
#include "IsPortOpenTest.h"

#include <NetLib/VxSktBase.h>

namespace
{
	void PingResponseCallbackHandler( VxSktBase *  sktBase, void * pvUserCallbackData )
	{
		PingResponseServer * peerMgr = ( PingResponseServer * )pvUserCallbackData;
		if( peerMgr )
		{
			peerMgr->handleTcpSktCallback( sktBase );
		}
	}
}

//============================================================================
PingResponseServer::PingResponseServer( IsPortOpenTest& isPortOpenTest )
: m_IsPortOpenTest( isPortOpenTest )
{
	setReceiveCallback( PingResponseCallbackHandler, this );
}

//============================================================================
PingResponseServer::~PingResponseServer()
{
}

//============================================================================
void PingResponseServer::handleTcpSktCallback( VxSktBase * sktBase )
{
	switch( sktBase->getCallbackReason() )
	{

	case eSktCallbackReasonData:
		m_IsPortOpenTest.handleTcpData( sktBase );
		break;
	case eSktCallbackReasonConnectError:
	case eSktCallbackReasonConnected:
	case eSktCallbackReasonClosed:
	case eSktCallbackReasonError:
	case eSktCallbackReasonClosing:
	case eSktCallbackReasonConnecting:
	default:
		break;
	}
}
