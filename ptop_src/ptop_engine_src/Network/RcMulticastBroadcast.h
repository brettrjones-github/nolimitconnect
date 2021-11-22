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

#include "RcMulticastBase.h"

#include <PktLib/PktAnnounce.h>

class RcMulticastBroadcast : public RcMulticastBase
{
public:
	RcMulticastBroadcast( NetworkMgr& networkMgr );
	virtual ~RcMulticastBroadcast();

    bool 						setBroadcastEnable( bool enable );
	void						onOncePerSecond( void );
	virtual void				onPktAnnUpdated( void );

protected:
	void						sendMulticast( void );

	bool						m_bBroadcastEnabled{ false };
	int							m_iBroadcastCountSec{ 0 };
	bool						m_bPktAnnUpdated{ false };
	PktAnnounce					m_PktAnnEncrypted;
};
