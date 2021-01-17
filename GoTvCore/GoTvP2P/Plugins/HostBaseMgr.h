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
// http://www.nolimitconnect.com
//============================================================================


#include <GoTvCore/GoTvP2P/Connections/IConnectRequest.h>

#include <CoreLib/VxMutex.h>

class ConnectionMgr;
class P2PEngine;
class PluginMgr;
class VxNetIdent;
class PluginBase;

class HostBaseMgr : public IConnectRequestCallback
{
public:
    HostBaseMgr( P2PEngine& engine, PluginMgr& pluginMgr, VxNetIdent * myIdent, PluginBase& pluginBase );
	virtual ~HostBaseMgr() = default;

    //=== hosting ===//
    virtual void				fromGuiJoinHost( EHostType hostType, const char * ptopUrl );

protected:
    //=== callback overrides ==//
    virtual void                onUrlActionQueryIdSuccess( std::string& url, VxGUID& onlineId, EConnectReason connectReason = eConnectReasonUnknown ) override;
    virtual void                onUrlActionQueryIdFail( std::string& url, ERunTestStatus testStatus, EConnectReason connectReason = eConnectReasonUnknown ) override;

    /// returns false if one time use and packet has been sent. Connect Manager will disconnect if nobody else needs the connection
    virtual bool                onContactConnected( VxSktBase* sktBase, VxGUID& onlineId, EConnectReason connectReason = eConnectReasonUnknown ) override;
    virtual void                onConnectRequestFail( VxGUID& onlineId, EConnectStatus connectStatus, EConnectReason connectReason = eConnectReasonUnknown ) override;
    virtual void                onContactDisconnected( VxSktBase* sktBase, VxGUID& onlineId, EConnectReason connectReason = eConnectReasonUnknown ) override;

    void                        sendJoinRequest( VxSktBase* sktBase, VxGUID& onlineId, EConnectReason connectReason );

    //=== vars ===//
    P2PEngine&                  m_Engine; 
    PluginMgr&                  m_PluginMgr; 
    VxNetIdent*                 m_MyIdent{ nullptr };
    PluginBase&                 m_Plugin;
    ConnectionMgr&              m_ConnectionMgr; 
 
    VxMutex                     m_MgrMutex;
};

