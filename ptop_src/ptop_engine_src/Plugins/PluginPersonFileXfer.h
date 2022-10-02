#pragma once
//============================================================================
// Copyright (C) 2019 Brett R. Jones
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

#include "PluginBase.h"

class PluginPersonFileXfer : public PluginBase
{
public:

    PluginPersonFileXfer( P2PEngine& engine, PluginMgr& pluginMgr, VxNetIdent* myIdent, EPluginType pluginType );
	virtual ~PluginPersonFileXfer() override = default;

    virtual void				onContactWentOnline( VxNetIdent* netIdent, VxSktBase* sktBase ) override {};
    virtual void				onContactWentOffline( VxNetIdent* netIdent, VxSktBase* sktBase ) override {};
    virtual void				replaceConnection( VxNetIdent* netIdent, VxSktBase* poOldSkt, VxSktBase* poNewSkt ) override {};
    virtual void				onConnectionLost( VxSktBase* sktBase ) override {};

};
