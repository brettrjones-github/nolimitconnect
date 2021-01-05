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
#pragma once

#include <GoTvInterface/IDefs.h>
#include <GoTvCore/GoTvP2P/HostMgr/HostConnectInterface.h>

#include <CoreLib/VxGUID.h>
#include <CoreLib/VxMutex.h>

#include <vector>
#include <string>
#include <map>

class VxSktConnectSimple;
class P2PEngine;
class PktAnnounce;

class ConnectedInfo
{
public:
    ConnectedInfo();
    ConnectedInfo( P2PEngine& engine );
    virtual ~ConnectedInfo() = default;

    ConnectedInfo( const ConnectedInfo& rhs );
    ConnectedInfo( P2PEngine& engine, EPluginType ePluginType, VxGUID onlineId, std::string& hostIp, uint16_t hostPort, const char * hostUrl );

    ConnectedInfo&				operator=( const ConnectedInfo& rhs );
    bool                        operator==( const ConnectedInfo& rhs );

    P2PEngine&                  getEngine() { return m_Engine; }

protected:
    P2PEngine&                  m_Engine;
    VxMutex                     m_CallbackListMutex;

    EPluginType                 m_PluginType{ ePluginTypeInvalid };
    VxGUID                      m_OnlineId;

};



