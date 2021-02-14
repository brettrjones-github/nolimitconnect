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

#include <PktLib/SearchParams.h>
#include <CoreLib/VxGUIDList.h>
#include <CoreLib/VxMutex.h>

#include <map>

class ConnectionMgr;
class P2PEngine;
class PluginMgr;
class VxNetIdent;
class PluginBase;
class VxPktHdr;
class SearchParams;

class HostBaseMgr : public IConnectRequestCallback
{
public:
    HostBaseMgr( P2PEngine& engine, PluginMgr& pluginMgr, VxNetIdent * myIdent, PluginBase& pluginBase );
	virtual ~HostBaseMgr() = default;

    //=== hosting ===//
    virtual void				fromGuiAnnounceHost( EHostType hostType, VxGUID& sessionId, std::string ptopUrl );
    virtual void				fromGuiJoinHost( EHostType hostType, VxGUID& sessionId, std::string ptopUrl );
    virtual void				fromGuiSearchHost( EHostType hostType, SearchParams& searchParams, bool enable );

    virtual EPluginAccess	    getPluginAccessState( VxNetIdent * netIdent );

    virtual void                connectToHost( EHostType hostType, VxGUID& sessionId, std::string& url, EConnectReason connectReason );
    virtual void                removeSession( VxGUID& sessionId, EConnectReason connectReason = eConnectReasonUnknown ) {};

    // error handling for invalid packet
    virtual void				onInvalidRxedPacket( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent, const char * msg = "" );

protected:
    virtual void                onUrlActionQueryIdSuccess( VxGUID& sessionId, std::string& url, VxGUID& onlineId, EConnectReason connectReason = eConnectReasonUnknown ) override;
    virtual void                onUrlActionQueryIdFail( VxGUID& sessionId, std::string& url, ERunTestStatus testStatus, EConnectReason connectReason = eConnectReasonUnknown ) override;

    /// returns false if one time use and packet has been sent. Connect Manager will disconnect if nobody else needs the connection
    virtual bool                onContactConnected( VxGUID& sessionId, VxSktBase* sktBase, VxGUID& onlineId, EConnectReason connectReason = eConnectReasonUnknown ) override;
    virtual void                onContactDisconnected( VxGUID& sessionId, VxSktBase* sktBase, VxGUID& onlineId, EConnectReason connectReason = eConnectReasonUnknown ) override;
    virtual void                onConnectRequestFail( VxGUID& sessionId, VxGUID& onlineId, EConnectStatus connectStatus, EConnectReason connectReason = eConnectReasonUnknown ) override;

    virtual void                onConnectToHostFail( EHostType hostType, VxGUID& sessionId, EConnectReason connectReason, EHostAnnounceStatus hostJoinStatus );
    virtual void                onConnectToHostFail( EHostType hostType, VxGUID& sessionId, EConnectReason connectReason, EHostJoinStatus hostJoinStatus );
    virtual void                onConnectToHostFail( EHostType hostType, VxGUID& sessionId, EConnectReason connectReason, EHostSearchStatus hostSearchStatus );
    virtual void                onConnectToHostSuccess( EHostType hostType, VxGUID& sessionId, VxSktBase* sktBase, VxGUID& onlineId, EConnectReason connectReason );
    virtual void                onConnectionToHostDisconnect( EHostType hostType, VxGUID& sessionId, VxSktBase* sktBase, VxGUID& onlineId, EConnectReason connectReason );

    void                        sendAnnounceRequest( EHostType hostType, VxGUID& sessionId, VxSktBase* sktBase, VxGUID& onlineId, EConnectReason connectReason );
    void                        sendJoinRequest( EHostType hostType, VxGUID& sessionId, VxSktBase* sktBase, VxGUID& onlineId, EConnectReason connectReason );
    void                        sendSearchRequest( EHostType hostType, VxGUID& sessionId, VxSktBase* sktBase, VxGUID& onlineId, EConnectReason connectReason );

    virtual void                onPktHostJoinReply( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent ) {};

    virtual bool                addContact( VxSktBase * sktBase, VxNetIdent * netIdent );
    virtual bool                removeContact( VxGUID& onlineId );
    EHostType                   connectReasonToHostType( EConnectReason connectReason );

    virtual bool                isAnnounceConnectReason( EConnectReason connectReason );
    virtual bool                isJoinConnectReason( EConnectReason connectReason );
    virtual bool                isSearchConnectReason( EConnectReason connectReason );

    virtual bool                stopHostSearch( EHostType hostType, SearchParams& searchParams );
    virtual bool                updateOrAddSearchParms( std::map<VxGUID, SearchParams>& paramsList, SearchParams& searchParams );


    //=== vars ===//
    P2PEngine&                  m_Engine; 
    PluginMgr&                  m_PluginMgr; 
    VxNetIdent*                 m_MyIdent{ nullptr };
    PluginBase&                 m_Plugin;
    ConnectionMgr&              m_ConnectionMgr; 
    VxMutex                     m_MgrMutex;
    VxGUIDList                  m_ContactList;
    std::map<VxGUID, SearchParams> m_SearchParamsList;
};

