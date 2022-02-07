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
	setReceiveCallback( VxPeerMgrRxCallbackHandler, this );
}

//============================================================================
VxPeerMgr::~VxPeerMgr()
{
	sktMgrShutdown();
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
// find a socket.. assumes list has been locked
VxSktBase* VxPeerMgr::findSktBase( const VxGUID& connectId, bool acceptSktsOnly )
{
	if( !connectId.isVxGUIDValid() )
	{
		LogMsg( LOG_ERROR, "VxPeerMgr::findSktBase invalid connectId" );
		return nullptr;
	}

	VxSktBase* sktBase = VxSktBaseMgr::findSktBase( connectId, acceptSktsOnly );
	if( !sktBase )
	{
		sktBase = m_ClientMgr.findSktBase( connectId, acceptSktsOnly );
	}

	return sktBase;
}

//============================================================================
//! Connect to ip or url and return socket.. if cannot connect return NULL
VxSktConnect * VxPeerMgr::connectTo(	const char *	pIpOrUrl,				// remote ip or url 
										uint16_t		u16Port,				// port to connect to
										int				iTimeoutMilliSeconds )	// milli seconds before connect attempt times out
{
	if( NULL ==  m_pfnUserReceive )
	{
		LogMsg( LOG_INFO, "VxPeerMgr::VxConnectTo: you must call setReceiveCallback first" );
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
		LogMsg( LOG_ERROR, "VxPeerMgr::createConnectionUsingSocket: you must call setReceiveCallback first" );
		vx_assert( m_pfnUserReceive );
	}

	VxSktConnect * sktBase	= (VxSktConnect *)makeNewSkt();
	sktBase->m_SktMgr		= this;
	//VxVerifyCodePtr( m_pfnOurReceive );
	sktBase->setReceiveCallback( m_pfnOurReceive, this );
	sktBase->setTransmitCallback( m_pfnOurTransmit, this );
	sktBase->createConnectionUsingSocket(	skt, rmtIp, port );
	addSkt( sktBase );
	LogMsg( LOG_INFO, "VxPeerMgr::createConnectionUsingSocket: done skt id %d rmt ip %s port %d", sktBase->getSktId(), rmtIp, port  );
	return sktBase;
}

//============================================================================
void VxPeerMgr::handleSktCallback( VxSktBase * sktBase )
{
}

//============================================================================
bool VxPeerMgr::txPacket(	VxSktBase *			sktBase,
							const VxGUID&		destOnlineId,
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
    if( !sktBase || false == isSktActive( sktBase ) )
	{
		if( false == m_ClientMgr.isSktActive( sktBase ) )
		{
            LogMsg( LOG_ERROR, "ERROR VxPeerMgr::txPacketWithDestId: skt no longer active" );
		}

        return false;
	}

    if( sktBase->isConnected() )
    {
        RCODE rc = sktBase->txPacketWithDestId( pktHdr, bDisconnect );
        if( 0 != rc )
        {
            LogMsg( LOG_VERBOSE, "VxPeerMgr::txPacketWithDestId: skt %d returned error %d %s", sktBase->getSktId(), rc, sktBase->describeSktError( rc ) );
        }

        return  ( 0 == rc );
    }
    else
    {
        LogMsg( LOG_ERROR, "ERROR VxPeerMgr::txPacketWithDestId: skt no longer connecte" );
    }

    return false;
}

//============================================================================
void VxPeerMgr::dumpSocketStats( const char *reason, bool fullDump )
{
    std::string reasonMsg = reason ? reason : "";
    VxSktBaseMgr::dumpSocketStats( std::string( reasonMsg + " server: " ).c_str(), fullDump );
    m_ClientMgr.dumpSocketStats( std::string( reasonMsg + " client: " ).c_str(), fullDump );
}

//============================================================================
bool VxPeerMgr::closeConnection( VxGUID& connectId, ESktCloseReason closeReason )
{
	bool wasClosed = m_ClientMgr.closeConnection( connectId, closeReason );
	if( !wasClosed )
	{
		wasClosed = VxSktBaseMgr::closeConnection( connectId, closeReason );
	}

	return wasClosed;
}