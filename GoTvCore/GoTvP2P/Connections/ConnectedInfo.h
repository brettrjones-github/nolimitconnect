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
class BigListInfo;

class ConnectedInfo
{
public:
    ConnectedInfo() = delete;
    ConnectedInfo( P2PEngine& engine, BigListInfo* bigListInfo );
    virtual ~ConnectedInfo() = default;

    ConnectedInfo( const ConnectedInfo& rhs );
    ConnectedInfo( P2PEngine& engine, EPluginType ePluginType, VxGUID onlineId, std::string& hostIp, uint16_t hostPort, const char * hostUrl );

    ConnectedInfo&				operator=( const ConnectedInfo& rhs );
    bool                        operator==( const ConnectedInfo& rhs );

    P2PEngine&                  getEngine() { return m_Engine; }

    void                        onSktConnected( VxSktBase * sktBase );
    void                        onSktDisconnected( VxSktBase * sktBase );

protected:
    P2PEngine&                  m_Engine;
    BigListInfo*                m_BigListInfo{ nullptr };
    VxGUID                      m_PeerOnlineId;
    VxMutex                     m_CallbackListMutex;
    std::vector<EPluginType>    m_RmtPlugins;
    std::vector<EPluginType>    m_LclPlugins;
    
};



