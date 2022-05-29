//============================================================================
// Copyright (C) 2013 Brett R. Jones
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

#include "P2PSession.h"
#include <PktLib/VxPktHdr.h>

//============================================================================
P2PSession::P2PSession( VxSktBase * sktBase, VxNetIdent * netIdent, EPluginType pluginType )
: PluginSessionBase( sktBase, netIdent, pluginType )
, m_iOutstandingAckCnt( 0 )
, m_bSendingPkts( false )
, m_VideoCastPkt( 0 )
{
	setSessionType(ePluginSessionTypeP2P);
}

//============================================================================
P2PSession::P2PSession( VxGUID& lclSessionId, VxSktBase * sktBase, VxNetIdent * netIdent, EPluginType pluginType )
: PluginSessionBase( lclSessionId, sktBase, netIdent, pluginType )
, m_iOutstandingAckCnt( 0 )
, m_bSendingPkts( false )
, m_VideoCastPkt( 0 )
{
	setSessionType(ePluginSessionTypeP2P);
}

//============================================================================
P2PSession::~P2PSession()
{
	delete m_VideoCastPkt;
}
