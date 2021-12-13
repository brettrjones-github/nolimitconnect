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

#include "NlcPingResponseServer.h"
#include "NetTestUtil.h"

#include <CoreLib/VxParse.h>
#include <NetLib/VxSktBase.h>

namespace
{
	void NlcPingResponseCallbackHandler( VxSktBase *  sktBase, void * pvUserCallbackData )
	{
        NlcPingResponseServer * peerMgr = ( NlcPingResponseServer * )pvUserCallbackData;
		if( peerMgr )
		{
			peerMgr->handleTcpSktCallback( sktBase );
		}
	}
}

//============================================================================
NlcPingResponseServer::NlcPingResponseServer( AppLogic& appLogic )
: m_AppLogic( appLogic )
{
	setReceiveCallback( NlcPingResponseCallbackHandler, this );
}

//============================================================================
bool NlcPingResponseServer::startListening( uint16_t u16ListenPort, const char * ip )
{
    if( ip && strlen(ip) )
    {
        LogMsg( LOG_DEBUG, "startListening on port %d and adapter ip %s", u16ListenPort, ip );
        return VxServerMgr::startListening( u16ListenPort, ip );
    }
    else
    {
        LogMsg( LOG_DEBUG, "startListening port %d", u16ListenPort );
        return VxServerMgr::startListening( u16ListenPort );
    }
}

//============================================================================
RCODE NlcPingResponseServer::stopListening( void )
{
    if( isListening() )
    {
        LogMsg( LOG_DEBUG, "stopListening on port %d", getListenPort() );
    }
    else
    {
        LogMsg( LOG_DEBUG, "stopListening.. not currently listening" );
    }

   return  VxServerMgr::stopListening();
}


//============================================================================
void NlcPingResponseServer::handleTcpSktCallback( VxSktBase * sktBase )
{
    if( sktBase )
    {
        LogMsg( LOG_DEBUG, "handleTcpSktCallback reason %s", VxSktBase::describeSktCallbackReason( sktBase->getCallbackReason() ) );
        switch( sktBase->getCallbackReason() )
        {
        case eSktCallbackReasonData:
            handleTcpRxData( sktBase );
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
    else
    {
        LogMsg( LOG_DEBUG, "handleTcpSktCallback null sktBase");
    }
}

//============================================================================
void NlcPingResponseServer::handleTcpRxData( VxSktBase * sktBase )
{
    if( !sktBase )
    {
        LogMsg( LOG_DEBUG, "handleTcpRxData null sktBase" );
        return;
    }

    LogModule( eLogIsPortOpenTest, LOG_VERBOSE, "IsPortOpenTest::handleTcpData thread 0x%x" );
    if( false == NetTestUtil::verifyAllDataArrivedOfNetServiceUrl( sktBase ) )
    {
        LogModule( eLogIsPortOpenTest, LOG_VERBOSE, "IsPortOpenTest::handleTcpData not all data arrived thread 0x%x", VxGetCurrentThreadId() );
        return;
    }

    NetServiceHdr netServiceHdr;
    EPluginType pluginType = NetTestUtil::parseHttpNetServiceUrl( sktBase, netServiceHdr );
    if( ( ePluginTypeNetServices == pluginType )
        && ( eNetCmdPing == netServiceHdr.m_NetCmdType ) )
    {
        LogModule( eLogIsPortOpenTest, LOG_VERBOSE, "IsPortOpenTest::handleTcpData got pinged thread 0x%x", VxGetCurrentThreadId() );
        LogMsg( LOG_DEBUG, "got pinged\n" );
        std::string content;
        StdStringFormat( content, "PONG-%s", sktBase->getRemoteIp() );
        NetTestUtil::buildAndSendCmd( sktBase, eNetCmdPong, content, 0, 1 );
    }
    else
    {
        LogModule( eLogIsPortOpenTest, LOG_VERBOSE, "IsPortOpenTest::handleTcpData got unknown data from ip %s thread 0x%x", sktBase->getRemoteIp(), VxGetCurrentThreadId() );
        LogMsg( LOG_DEBUG, "got unknown data from ip %s\n", sktBase->getRemoteIp() );
    }
}
