#ifndef MULTICAST_LISTEN_H
#define MULTICAST_LISTEN_H

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

#include "RcMulticastBase.h"

class PktAnnounce;

class IMulticastListenCallback
{
public:
	virtual void				multicastPktAnnounceAvail( VxSktBase * skt, PktAnnounce * pktAnnounce ) = 0;
};

class RcMulticastListen : public RcMulticastBase
{
public:
	RcMulticastListen(  NetworkMgr& networkMgr, IMulticastListenCallback& multicastListenCallback );
	virtual ~RcMulticastListen();

	bool 						enableListen( bool enable );
	void						doUdpDataCallback( VxSktBase * skt );

protected:
	int 						startListen( void );
	void 						stopListen( void );
	void						attemptDecodePktAnnounce( VxSktBase * skt, unsigned char * data, int dataLen );

	bool						m_ListenEnabled{ false };
	VxSktUdp					m_SktUdp;
	IMulticastListenCallback&	m_ListenCallback;
};

#endif // MULTICAST_LISTEN_H
