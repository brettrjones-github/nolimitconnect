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

#include <CoreLib/VxGUID.h>
#include <CoreLib/VxMutex.h>

#include <vector>
#include <map>
#include <utility>

class P2PEngine;
class IConnectRequestCallback;
class BigListInfo;

class ConnectedInfo
{
public:
    ConnectedInfo() = delete;
    ConnectedInfo( P2PEngine& engine, BigListInfo* bigListInfo );
    ConnectedInfo( const ConnectedInfo& rhs );
    virtual ~ConnectedInfo() = default;

    ConnectedInfo&				operator=( const ConnectedInfo& rhs );
    bool                        operator==( const ConnectedInfo& rhs );

    P2PEngine&                  getEngine() { return m_Engine; }
    VxSktBase *                 getSktBase( void );

    void                        addConnectReason( IConnectRequestCallback* callback, EConnectReason connectReason );
    void                        removeConnectReason( IConnectRequestCallback* callback, EConnectReason connectReason, bool disconnectIfNotInUse );

    void                        onSktConnected( VxSktBase * sktBase );
    void                        onSktDisconnected( VxSktBase * sktBase );

protected:
    P2PEngine&                  m_Engine;
    BigListInfo*                m_BigListInfo{ nullptr };
    VxGUID                      m_PeerOnlineId;
    VxMutex                     m_CallbackListMutex;
    std::vector<EPluginType>    m_RmtPlugins;
    std::vector<std::pair<IConnectRequestCallback*, EConnectReason>>    m_LclList;
    std::vector<std::pair<IConnectRequestCallback*, EConnectReason>>    m_RmtList;
    std::vector<VxSktBase *>    m_SktList;
};



