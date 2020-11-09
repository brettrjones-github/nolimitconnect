#pragma once
//============================================================================
// Copyright (C) 2020 Brett R. Jones 
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

#include "HostInfoBase.h"
#include <CoreLib/VxMutex.h>

class P2PEngine;

class MyHostSrvMgr
{
public:
    MyHostSrvMgr( P2PEngine& engine );

    void                        addHostInfo( EPluginType ePluginType, VxGUID onlineId, std::string& hostIp, uint16_t hostPort, const char * hostUrl = "" );
    void                        requestHostConnection( EHostConnectType connectType, IHostConnectCallback* callback, bool enableCallback );

    virtual void                onSktConnectedWithPktAnn( VxSktBase* sktBase );
    virtual void                onSktDisconnected( VxSktBase* sktBase );
protected:


    //=== vars ===//
    P2PEngine&                  m_Engine;
    VxMutex                     m_CallbackMutex;
    std::vector<IHostConnectCallback *> m_ConnectionCallbacks;
};
