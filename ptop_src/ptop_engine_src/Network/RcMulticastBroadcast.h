#ifndef MULTICAST_BROADCAST_H
#define MULTICAST_BROADCAST_H

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

#include <PktLib/PktAnnounce.h>

class RcMulticastBroadcast : public RcMulticastBase
{
public:
	RcMulticastBroadcast( NetworkMgr& networkMgr );
	virtual ~RcMulticastBroadcast();

	void 						setBroadcastEnable( bool enable );
	void						onOncePerSecond( void );
	virtual void				onPktAnnUpdated( void );
	virtual void				goOnline( bool online );

protected:
	void						sendMulticast( void );


	bool						m_bBroadcastEnabled;
	int							m_iBroadcastCountSec;
	bool						m_bPktAnnUpdated;
	PktAnnounce					m_PktAnnEncrypted;
	bool						m_bOnline;
};

#endif // MULTICAST_BROADCAST_H