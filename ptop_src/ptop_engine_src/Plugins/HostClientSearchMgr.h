#pragma once
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
// http://www.nolimitconnect.org
//============================================================================

#include "HostBaseMgr.h"

#include <CoreLib/VxGUIDList.h>
#include <CoreLib/VxMutex.h>

class ConnectionMgr;
class P2PEngine;
class PluginMgr;
class VxNetIdent;
class PluginBase;
class VxPktHdr;

class HostClientSearchMgr : public HostBaseMgr
{
public:
    HostClientSearchMgr( P2PEngine& engine, PluginMgr& pluginMgr, VxNetIdent * myIdent, PluginBase& pluginBase );
	virtual ~HostClientSearchMgr() = default;

    virtual void                onPktHostSearchReply( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent ) {};
    virtual void                onPktPluginSettingReply( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent ) {};

protected:

    //=== vars ===//
    VxMutex                     m_SearchMutex;
    VxGUIDList                  m_SearchList;
};

