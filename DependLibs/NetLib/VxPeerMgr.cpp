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

#include "VxPeerMgr.h"
#include "VxSktConnect.h"

#include <CoreLib/VxGUID.h>
#include <PktLib/VxPktHdr.h>

namespace
{
	void VxPeerMgrRxCallbackHandler( VxSktBase *  sktBase, void * pvUserCallbackData )
	{
		VxPeerMgr * peerMgr = ( VxPeerMgr * )pvUserCallbackData;
		if( peerMgr )
		{
			peerMgr->handleSktCallback( sktBase );
		}
	}
}

//============================================================================
VxPeerMgr::VxPeerMgr()
{
	m_eSktMgrType = eSktMgrTypeTcpConnect;
	setReceiveCallback( VxPeerMgrRxCallbackHandler, this );
}

//============================================================================
VxPeerMgr::~VxPeerMgr()
{
	sktMgrShutdown();
}


//============================================================================
/// if skt exists in connection list then lock access to connection list
bool VxPeerMgr::lockSkt( VxSktBase* sktBase )
{
    if( sktBase )
    {
        if( sktBase->isAcceptSocket() )
        {
            // one we handle
            return VxSktBaseMgr::lockSkt( sktBase );
        }
        else
        {
            // client skt
            return m_ClientMgr.lockSkt( sktBase );
        }
    }
    else
    {
        return false;
    }
}

//============================================================================
void VxPeerMgr::unlockSkt( VxSktBase* sktBase )
{
    if( sktBase )
    {
        if( sktBase->isAcceptSocket() )
        {
            // one we handle
            return VxSktBaseMgr::unlockSkt( sktBase );
        }
        else
        {
            // client skt
            return m_ClientMgr.unlockSkt( sktBase );
        }
    }
}

//============================================================================
void VxPeerMgr::sktMgrShutdown( void )
{
	stopListening();
	m_ClientMgr.sktMgrShutdown();
	VxSktBaseMgr::sktMgrShutdown();
}

//============================================================================
void VxPeerMgr::setReceiveCallback( VX_SKT_CALLBACK pfnReceive, void * pvUserData )
{
	VxServerMgr::setReceiveCallback( pfnReceive, pvUserData );
	m_ClientMgr.setReceiveCallback( pfnReceive, pvUserData );
}

//============================================================================
void VxPeerMgr::setLocalIp( InetAddress& newLocalIp )
{
	sktMgrSetLocalIp( newLocalIp );
	m_ClientMgr.sktMgrSetLocalIp( newLocalIp );
}

//============================================================================
//! make a new socket... give derived classes a chance to override
VxSktBase * VxPeerMgr::makeNewSkt( void )
{ 
	return new VxSktConnect(); 
}

//============================================================================
//! Connect to ip or url and return socket.. if cannot connect return NULL
VxSktConnect * VxPeerMgr::connectTo(	const char *	pIpOrUrl,				// remote ip or url 
										uint16_t		u16Port,				// port to connect to
										int				iTimeoutMilliSeconds )	// milli seconds before connect attempt times out
{
	if( NULL ==  m_pfnUserReceive )
	{
		LogMsg( LOG_INFO, "VxPeerMgr::VxConnectTo: you must call setReceiveCallback first\n" );
		vx_assert( m_pfnUserReceive );
	}
		
	VxSktConnect * sktBase	= (VxSktConnect *)makeNewSkt();
	sktBase->m_SktMgr		= this;
	sktBase->setReceiveCallback( m_pfnOurReceive, this );
	sktBase->setTransmitCallback( m_pfnOurTransmit, this );
	RCODE rc = sktBase->connectTo(	m_LclIp,
									pIpOrUrl, 
									u16Port, 
									iTimeoutMilliSeconds );
	if( rc )
	{
		delete sktBase;
		return NULL;
	}

	addSkt( sktBase );
	return sktBase;
}

//============================================================================
VxSktConnect * VxPeerMgr::createConnectionUsingSocket( SOCKET skt, const char * rmtIp, uint16_t port )
{
	if( NULL ==  m_pfnUserReceive )
	{
		LogMsg( LOG_ERROR, "VxPeerMgr::createConnectionUsingSocket: you must call setReceiveCallback first\n" );
		vx_assert( m_pfnUserReceive );
	}

	VxSktConnect * sktBase	= (VxSktConnect *)makeNewSkt();
	sktBase->m_SktMgr		= this;
	//VxVerifyCodePtr( m_pfnOurReceive );
	sktBase->setReceiveCallback( m_pfnOurReceive, this );
	sktBase->setTransmitCallback( m_pfnOurTransmit, this );
	sktBase->createConnectionUsingSocket(	skt, rmtIp, port );
	addSkt( sktBase );
	LogMsg( LOG_INFO, "VxPeerMgr::createConnectionUsingSocket: done skt id %d rmt ip %s port %d\n", sktBase->getSktId(), rmtIp, port  );
	return sktBase;
}

//============================================================================
void VxPeerMgr::handleSktCallback( VxSktBase * sktBase )
{
}

//============================================================================
bool VxPeerMgr::txPacket(	VxSktBase *			sktBase,
							VxGUID&				destOnlineId,			
							VxPktHdr *			pktHdr, 				
							bool				bDisconnect )
{
	pktHdr->setDestOnlineId( destOnlineId );
	return txPacketWithDestId( sktBase, pktHdr, bDisconnect );
}

//============================================================================
bool VxPeerMgr::txPacketWithDestId(	VxSktBase *			sktBase,
									VxPktHdr *			pktHdr, 		
									bool				bDisconnect )
{
	if( false == isSktActive( sktBase ) )
	{
		if( false == m_ClientMgr.isSktActive( sktBase ) )
		{
			LogMsg( LOG_ERROR, "ERROR VxPeerMgr::txPacketWithDestId: skt no longer active\n" );
			if( sktBase->isConnected() )
			{
				vx_assert( false );
			}

			return false;;
		}
	}

	RCODE rc = sktBase->txPacketWithDestId( pktHdr, bDisconnect );
	if( 0 != rc )
	{
		LogMsg( LOG_INFO, "VxPeerMgr::txPacketWithDestId: skt %d returned error %d %s\n", sktBase->getSktId(), rc, sktBase->describeSktError( rc ) );
	}

	return  ( 0 == rc );
}

//============================================================================
void VxPeerMgr::dumpSocketStats( const char *reason )
{
    std::string reasonMsg = reason ? reason : "";
    VxSktBaseMgr::dumpSocketStats( std::string( reasonMsg + " server: " ).c_str() );
    m_ClientMgr.dumpSocketStats( std::string( reasonMsg + " client: " ).c_str() );
}