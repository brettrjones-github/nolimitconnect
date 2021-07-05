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

#include "PluginSessionBase.h"

#include <CoreLib/VxSemaphore.h>

class PktVideoFeedPic;

class RxSession : public PluginSessionBase
{
public:
	RxSession( VxSktBase * sktBase, VxNetIdent * netIdent, EPluginType pluginType );
	RxSession( VxGUID& lclSessionId, VxSktBase * sktBase, VxNetIdent * netIdent, EPluginType pluginType );
	virtual ~RxSession();

	bool						waitForResponse( int iTimeoutMs );
	void						signalResponseRecieved( void )			{ m_Semaphore.signal(); }

	PktVideoFeedPic *			getVideoFeedPkt( void )						{ return m_PktVideoFeedPic; }
	void						setVideoFeedPkt( PktVideoFeedPic * pkt )	{ m_PktVideoFeedPic = pkt; }

protected:
	//=== vars ===//
	VxSemaphore					m_Semaphore;
	PktVideoFeedPic *			m_PktVideoFeedPic;
};
