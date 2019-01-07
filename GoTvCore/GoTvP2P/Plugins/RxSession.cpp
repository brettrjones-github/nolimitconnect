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
// brett.jones@engineer.com
// http://www.gotvptop.net
//============================================================================

#include "RxSession.h"

//============================================================================
RxSession::RxSession( VxSktBase * sktBase, VxNetIdent * netIdent )
: PluginSessionBase( sktBase, netIdent )
, m_PktVideoFeedPic( 0 )
{
	setSessionType(ePluginSessionTypeRx);
	setIsSessionStarted( true );
}

//============================================================================
RxSession::RxSession( VxGUID& lclSessionId, VxSktBase * sktBase, VxNetIdent * netIdent )
: PluginSessionBase( lclSessionId, sktBase, netIdent )
, m_PktVideoFeedPic( 0 )
{
	setSessionType(ePluginSessionTypeRx);
	setIsSessionStarted( true );
}

//============================================================================
RxSession::~RxSession()
{
}

//============================================================================
bool RxSession::waitForResponse( int iTimeoutMs )
{
	return m_Semaphore.wait( iTimeoutMs );
}
