//============================================================================
// Copyright (C) 2010 Brett R. Jones
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
#include "PluginMgr.h"

#include <GoTvCore/GoTvP2P/P2PEngine/P2PEngine.h>

#include <NetLib/VxSktBase.h>

//============================================================================
bool PluginMgr::pluginApiSktConnectTo(		EPluginType			ePluginType,	// plugin id
											VxNetIdentBase *	netIdent,		// identity of contact to connect to
											int					pvUserData,		// plugin defined data
											VxSktBase **		ppoRetSkt, 		// returned Socket
											EConnectReason		connectReason )
{
	VxSktBase * sktBase = nullptr;
	* ppoRetSkt = nullptr;
	bool newConnection = false;
	if( true == m_Engine.connectToContact( netIdent->getConnectInfo(), &sktBase, newConnection, connectReason ) )
	{
		* ppoRetSkt = sktBase;
		return true;
	}

	return false;
}

//============================================================================
//! close socket connection
void PluginMgr::pluginApiSktClose( ESktCloseReason closeReason, VxSktBase * sktBase )
{
	sktBase->closeSkt(closeReason);
}

//============================================================================
//! close socket immediate.. don't bother to flush buffer
void PluginMgr::pluginApiSktCloseNow( ESktCloseReason closeReason, VxSktBase * sktBase )
{
	sktBase->closeSkt(closeReason,  false);
}


