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

#include <PktLib/PktHostAnnounce.h>

class PluginBaseHostService : public PluginNetServices
{
public:
    PluginBaseHostService( P2PEngine& engine, PluginMgr& pluginMgr, VxNetIdent * myIdent );
    virtual ~PluginBaseHostService() override = default;

    void                        setHostType( EHostType hostType )   { m_HostType = hostType; }
    EHostType                   getHostType(  void )                { return m_HostType; }

    virtual void				onPktHostJoinReq                ( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent ) override;
    virtual void				onPktHostSearchReq              ( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent ) override;
    virtual void				onPktPluginSettingReq           ( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent ) override;
    virtual void				onPktHostOfferReq               ( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent ) override;
    virtual void				onPktHostOfferReply             ( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent ) override;

protected:
    virtual void				onContactWentOffline            ( VxNetIdent * netIdent, VxSktBase * sktBase ) override;
    virtual void				onConnectionLost                ( VxSktBase * sktBase ) override;
    virtual void				replaceConnection               ( VxNetIdent * netIdent, VxSktBase * poOldSkt, VxSktBase * poNewSkt ) override;

    virtual void                buildHostAnnounce( PluginSetting& pluginSetting );
    virtual void				sendHostAnnounce( void );

    //=== vars ===//
    EHostType                   m_HostType{ eHostTypeUnknown };

    HostServerMgr               m_HostServerMgr;
    bool                        m_SendAnnounceEnabled{ false };
    bool                        m_HostAnnounceBuilt{ false };
    PktHostAnnounce             m_PktHostAnnounce;
    VxMutex                     m_AnnMutex;
    VxGUID                      m_AnnounceSessionId;
};

