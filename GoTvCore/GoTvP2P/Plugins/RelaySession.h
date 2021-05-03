#pragma once
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
// http://www.nolimitconnect.com
//============================================================================

#include "PluginSessionBase.h"

#include <PktLib/PktsRelay.h>

// class implements virtual Base Pkt handlers for ease of coding
// forward declare
class VxSktBase;
class VxPktHdr;

class RelaySession : public PluginSessionBase
{
public:
	RelaySession( VxSktBase * sktBase, VxNetIdent * netIdent, EPluginType pluginType );
	RelaySession( VxGUID& lclSessionId, VxSktBase * sktBase, VxNetIdent * netIdent, EPluginType pluginType ); 
	virtual ~RelaySession();

	bool proxySendPkt( VxPktHdr * poPkt, VxGUID& destOnlineId, bool bDisconnectAfterSend = false );
};
