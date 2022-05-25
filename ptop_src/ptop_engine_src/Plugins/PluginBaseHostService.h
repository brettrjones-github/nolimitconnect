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
// http://www.nolimitconnect.com
//============================================================================

#include "PluginNetServices.h"
#include "HostServerMgr.h"

#include <PktLib/PktsHostInvite.h>

class PluginBaseHostService : public PluginNetServices
{
public:
    PluginBaseHostService( P2PEngine& engine, PluginMgr& pluginMgr, VxNetIdent * myIdent, EPluginType pluginType );
    virtual ~PluginBaseHostService() override = default;

    EPluginType                 getClientPluginType( void );
    virtual bool				getHostedInfo( HostedInfo& hostedInfo ) override;

    virtual void				onMyOnlineUrlIsValid( bool iValid ) override;

    virtual void				onPktHostJoinReq                ( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent ) override;
    virtual void				onPktHostLeaveReq               ( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent ) override;
    virtual void				onPktHostUnJoinReq              ( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent ) override;
    virtual void				onPktPluginSettingReq           ( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent ) override;

    virtual void				onPktHostOfferReq               ( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent ) override;
    virtual void				onPktHostOfferReply             ( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent ) override;

    virtual void				onPktHostInfoReq                ( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent ) override;
    virtual void				onPktHostInfoReply              ( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent ) override;
    virtual void				onPktHostSearchReq              ( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent ) override;

    virtual void				onPktGroupieInfoReq             ( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent ) override;
    virtual void				onPktGroupieInfoReply           ( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent ) override;

    virtual void				onPktGroupieAnnReq              ( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent ) override;
    virtual void				onPktGroupieAnnReply            ( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent ) override;
    virtual void				onPktGroupieSearchReq           ( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent ) override;
    virtual void				onPktGroupieSearchReply         ( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent ) override;
    virtual void				onPktGroupieMoreReq             ( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent ) override;
    virtual void				onPktGroupieMoreReply           ( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent ) override;

    virtual bool				fromGuiRequestPluginThumb       ( VxNetIdent* netIdent, VxGUID& thumbId ) override;
    virtual bool                ptopEngineRequestPluginThumb    ( VxSktBase* sktBase, VxNetIdent* netIdent, VxGUID& thumbId ) override;

    virtual bool                setPluginSetting( PluginSetting& pluginSetting, int64_t lastModifiedTime = 0 ) override;
    virtual	void				onPluginSettingChange( PluginSetting& pluginSetting, int64_t lastModifiedTime = 0 ) override;
    virtual void				onThreadOncePer15Minutes( void ) override;

    virtual void                onUserJoinedHost( GroupieId& groupieId, VxSktBase* sktBase, VxNetIdent* netIdent ) override;
    virtual void                onUserLeftHost( GroupieId& groupieId, VxSktBase* sktBase, VxNetIdent* netIdent ) override;
    virtual void                onUserUnJoinedHost( GroupieId& groupieId, VxSktBase* sktBase, VxNetIdent* netIdent ) override;
    virtual void                onGroupDirectUserAnnounce( GroupieId& groupieId, VxSktBase* sktBase, VxNetIdent* netIdent ) override;

protected:
    virtual void				onContactWentOffline            ( VxNetIdent* netIdent, VxSktBase* sktBase ) override;
    virtual void				onConnectionLost                ( VxSktBase* sktBase ) override;
    virtual void				replaceConnection               ( VxNetIdent* netIdent, VxSktBase* poOldSkt, VxSktBase* poNewSkt ) override;

    virtual void                buildHostAnnounce( PluginSetting& pluginSetting );
    virtual void				sendHostAnnounce( void );
    virtual bool				getHostTitleAndDescription( std::string& hostTitle, std::string& hostDesc, int64_t& lastModifiedTime );
    virtual void				onPluginSettingsChanged( int64_t modifiedTimeMs );

    virtual void				updateHostInvite( PluginSetting& pluginSetting );
    virtual void				updateHostInviteUrl( void );

    ECommErr                    getCommAccessState( VxNetIdent* netIdent );

    //=== vars ===//
    HostServerMgr               m_HostServerMgr;

    std::string                 m_HostInviteUrl{ "" };
    std::string                 m_HostTitle{ "" };
    std::string                 m_HostDescription{ "" };
    int64_t                     m_HostInfoModifiedTime{ 0 };
    bool                        m_PktHostInviteIsValid{ false };
    PktHostInviteAnnounceReq    m_PktHostInviteAnnounceReq;

    VxMutex                     m_AnnMutex;
    VxGUID                      m_AnnounceSessionId;
    int64_t                     m_PktAnnLastModTime{ 0 };
};

