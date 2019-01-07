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
// brett.jones@engineer.com
// http://www.gotvptop.net
//============================================================================

#include "PluginBase.h"

#include <GoTvCore/GoTvP2P/NetServices/NetServicesMgr.h>

class PluginNetServices : public PluginBase
{
public:
	PluginNetServices( P2PEngine& engine, PluginMgr& pluginMgr, IToGui& toGui, VxNetIdent * myIdent );
	virtual ~PluginNetServices();

	virtual RCODE				handleHttpConnection( VxSktBase * sktBase, NetServiceHdr& netServiceHdr );

	void						queryWhatsMyIp( void );
	void						testIsMyPortOpen( void );

protected:
	RCODE						internalHandleHttpConnection( VxSktBase * sktBase, NetServiceHdr& netServiceHdr );

	virtual void				replaceConnection( VxNetIdent * netIdent, VxSktBase * poOldSkt, VxSktBase * poNewSkt )	{};
	virtual void				onContactWentOffline( VxNetIdent * netIdent, VxSktBase * sktBase )	{};
	virtual void				onConnectionLost( VxSktBase * sktBase )	{};

	//=== vars ===//
	NetServicesMgr&				m_NetServicesMgr;
	//NetServiceUtils&			m_NetServiceUtil;

private:
	PluginNetServices(); // don't allow default constructor
	PluginNetServices(const PluginNetServices&); // don't allow copy constructor
};

