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

#include "GuiHosted.h"

#include <ptop_src/ptop_engine_src/HostListMgr/HostedListCallbackInterface.h>
#include <ptop_src/ptop_engine_src/HostListMgr/HostedInfo.h>

#include <CoreLib/VxMutex.h>

#include <QObject>

class AppCommon;
class VxNetIdent;

class GuiHostedListMgr : public QObject, public HostedListCallbackInterface
{
    Q_OBJECT
public:
    GuiHostedListMgr() = delete;
    GuiHostedListMgr( AppCommon& app );
    GuiHostedListMgr( const GuiHostedListMgr& rhs ) = delete;
	virtual ~GuiHostedListMgr() = default;
    void                        onAppCommonCreated( void );
    void                        onMessengerReady( bool ready ) { }
    bool                        isMessengerReady( void );
    virtual void                onSystemReady( bool ready ) { }

    bool                        isHostedInSession( VxGUID& onlineId );
    void                        setHostedOffline( VxGUID& onlineId );

    void                        onHostedAdded( GuiHosted* user );
    void                        onHostedRemoved( VxGUID& onlineId, EHostType hostType );
    void                        onHostedUpdated( GuiHosted* user );
    void                        onUserOnlineStatusChange( GuiHosted* user, bool isOnline );
    void                        onMyIdentUpdated( GuiHosted* user );

    GuiHosted*                getHosted( VxGUID& onlineId );
    std::map<VxGUID, GuiHosted*>& getHostedList( void )             { return m_HostedList; }
    GuiHosted*                updateHosted( VxNetIdent* hisIdent, EHostType hostType = eHostTypeUnknown );

signals:
    void				        signalMyIdentUpdated( GuiHosted* user ); 

    void				        signalHostedRequested( GuiHosted* user ); 
    void                        signalHostedUpdated( GuiHosted* user );
    void				        signalHostedRemoved( VxGUID onlineId, EHostType hostType );
    void                        signalHostedOfferStateChange( VxGUID& userOnlineId, EHostType hostType, EJoinState hostOfferState );
    void                        signalHostedOnlineStatus( GuiHosted* user, bool isOnline );

    void                        signalInternalHostedRequested( HostedInfo* userJoinInfo );
    void                        signalInternalHostedUpdated( HostedInfo* userJoinInfo );
    void                        signalInternalHostedRemoved( VxGUID hostOnlineId, EPluginType pluginType );
    void                        signalInternalHostedOfferState( VxGUID hostOnlineId, EPluginType pluginType, EJoinState hostOfferState );
    void                        signalInternalHostedOnlineState( VxGUID hostOnlineId, EPluginType pluginType, EOnlineState onlineState, VxGUID connectionId );

private slots:
    void                        slotInternalHostedRequested( HostedInfo* userJoinInfo );
    void                        slotInternalHostedUpdated( HostedInfo* userJoinInfo );
    void                        slotInternalHostedRemoved( VxGUID hostOnlineId, EPluginType pluginType );
    void                        slotInternalHostedOfferState( VxGUID userOnlineId, EPluginType pluginType, EJoinState hostOfferState );
    void                        slotInternalHostedOnlineState( VxGUID userOnlineId, EPluginType pluginType, EOnlineState onlineState, VxGUID connectionId );

protected:
    void                        removeHosted( VxGUID& onlineId );
    GuiHosted*                findHosted( VxGUID& onlineId );
    GuiHosted*                updateHosted( HostedInfo* userJoinInfo );
    
    virtual void				callbackHostedRequested( HostedInfo* userJoinInfo ) override;
    virtual void				callbackHostedUpdated( HostedInfo* userJoinInfo ) override;
    virtual void				callbackHostedRemoved( VxGUID& userOnlineId, EPluginType pluginType ) override;
    virtual void				callbackHostedOfferState( VxGUID& userOnlineId, EPluginType pluginType, EJoinState userOfferState ) override;
    virtual void				callbackHostedOnlineState( VxGUID& userOnlineId, EPluginType pluginType, EOnlineState onlineState, VxGUID& connectionId ) override;

    AppCommon&                  m_MyApp;
    // map of online id to GuiHosted
    std::map<VxGUID, GuiHosted*>  m_HostedList;
};
