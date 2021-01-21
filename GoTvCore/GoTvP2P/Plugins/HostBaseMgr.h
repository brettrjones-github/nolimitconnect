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

#include <CoreLib/VxGUIDList.h>
#include <CoreLib/VxMutex.h>

class ConnectionMgr;
class P2PEngine;
class PluginMgr;
class VxNetIdent;
class PluginBase;
class VxPktHdr;

class HostBaseMgr : public IConnectRequestCallback
{
public:
    HostBaseMgr( P2PEngine& engine, PluginMgr& pluginMgr, VxNetIdent * myIdent, PluginBase& pluginBase );
	virtual ~HostBaseMgr() = default;

    //=== hosting ===//
    virtual void				fromGuiJoinHost( EHostType hostType, std::string ptopUrl );

    virtual EPluginAccess	    getPluginAccessState( VxNetIdent * netIdent );

    virtual void                connectToHost( EHostType hostType, std::string& url, EConnectReason connectReason );

protected:

    virtual void                onUrlActionQueryIdSuccess( std::string& url, VxGUID& onlineId, EConnectReason connectReason = eConnectReasonUnknown ) override;
    virtual void                onUrlActionQueryIdFail( std::string& url, ERunTestStatus testStatus, EConnectReason connectReason = eConnectReasonUnknown ) override;

    /// returns false if one time use and packet has been sent. Connect Manager will disconnect if nobody else needs the connection
    virtual bool                onContactConnected( VxSktBase* sktBase, VxGUID& onlineId, EConnectReason connectReason = eConnectReasonUnknown ) override;
    virtual void                onContactDisconnected( VxSktBase* sktBase, VxGUID& onlineId, EConnectReason connectReason = eConnectReasonUnknown ) override;
    virtual void                onConnectRequestFail( VxGUID& onlineId, EConnectStatus connectStatus, EConnectReason connectReason = eConnectReasonUnknown ) override;

    virtual void                onConnectToHostFail( EHostType hostType, EConnectReason connectReason, EHostJoinStatus hostJoinStatus );
    virtual void                onConnectToHostSuccess( EHostType hostType, VxSktBase* sktBase, VxGUID& onlineId, EConnectReason connectReason );
    virtual void                onConnectionToHostDisconnect( EHostType hostType, VxSktBase* sktBase, VxGUID& onlineId, EConnectReason connectReason );

    void                        sendJoinRequest( VxSktBase* sktBase, VxGUID& onlineId, EConnectReason connectReason );

    virtual void                onPktHostJoinReply( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent ) {};

    virtual bool                addContact( VxSktBase * sktBase, VxNetIdent * netIdent );
    virtual bool                removeContact( VxGUID& onlineId );
    EHostType                   connectReasonToHostType( EConnectReason connectReason );

    //=== vars ===//
    P2PEngine&                  m_Engine; 
    PluginMgr&                  m_PluginMgr; 
    VxNetIdent*                 m_MyIdent{ nullptr };
    PluginBase&                 m_Plugin;
    ConnectionMgr&              m_ConnectionMgr; 
    VxMutex                     m_MgrMutex;
    VxGUIDList                  m_ContactList;
};

