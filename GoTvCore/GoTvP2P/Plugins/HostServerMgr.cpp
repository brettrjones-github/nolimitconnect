//============================================================================
// Copyright (C) 2021 Brett R. Jones
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

#include "HostServerMgr.h"

#include <PktLib/VxCommon.h>

//============================================================================
HostServerMgr::HostServerMgr( P2PEngine& engine, PluginMgr& pluginMgr, VxNetIdent * myIdent, PluginBase& pluginBase )
    : HostBaseMgr( engine, pluginMgr, myIdent, pluginBase )
{
}

//============================================================================
void HostServerMgr::addClient( VxNetIdent * ident )
{
    m_ClientList.addGuid( ident->getMyOnlineId() );
}

//============================================================================
void HostServerMgr::removeClient( VxGUID& onlineId )
{
    m_ClientList.removeGuid( onlineId );
}