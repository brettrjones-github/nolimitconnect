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
// http://www.gotvptop.net
//============================================================================

#include "RcMulticastListen.h"

#include <PktLib/PktAnnounce.h>
#include <CoreLib/VxGlobals.h>

namespace
{
	void RcMulticastListenSktCallbackHandler( VxSktBase *  sktBase, void * pvRxCallbackUserData )
	{
		if( eSktCallbackReasonData == sktBase->getCallbackReason() )
		{
			if( pvRxCallbackUserData )
			{
				((RcMulticastListen *)pvRxCallbackUserData)->doUdpDataCallback( sktBase );
			}
		}
	}
}

//============================================================================
RcMulticastListen::RcMulticastListen(  NetworkMgr& networkMgr, IMulticastListenCallback& multicastListenCallback )
: RcMulticastBase( networkMgr)
, m_ListenCallback( multicastListenCallback )
{
	m_SktUdp.setReceiveCallback( RcMulticastListenSktCallbackHandler, this );
}

//============================================================================
RcMulticastListen::~RcMulticastListen()
{
	stopListen();
}

//============================================================================
int RcMulticastListen::beginListen( void )
{
	RCODE rc = m_SktUdp.udpOpen( m_LclIp, m_u16MulticastPort );
	if( 0 == rc )
	{
		rc = m_SktUdp.joinMulticastGroup( m_LclIp,  m_strMulticastIp.c_str() );
	}

	return rc;
}

//============================================================================
void RcMulticastListen::stopListen( void )
{
	m_SktUdp.closeSkt();
}

//============================================================================
void RcMulticastListen::doUdpDataCallback( VxSktBase * skt )
{
	unsigned char *	data = skt->getSktReadBuf();
	int dataLen = skt->getSktBufDataLen();
	
	attemptDecodePktAnnounce( skt, data, dataLen );
	
	skt->sktBufAmountRead( dataLen );
}

//============================================================================
void RcMulticastListen::attemptDecodePktAnnounce( VxSktBase * skt, unsigned char * data, int dataLen )
{
	if( VxIsAppShuttingDown() )
	{
		return;
	}

	if( dataLen != sizeof( PktAnnounce ) )
	{
		return;
	}

	skt->decryptReceiveData();

	PktAnnounce * pktAnnounce = (PktAnnounce *)data;
	if( false == pktAnnounce->isValidPktAnn() )
	{
		LogMsg( LOG_INFO, "attemptDecodePktAnnounce invalid PktAnn\n" );
		return;
	}

	m_ListenCallback.multicastPktAnnounceAvail( skt, pktAnnounce );
}
