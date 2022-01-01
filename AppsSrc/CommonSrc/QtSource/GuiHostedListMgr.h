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

    void                        signalInternalHostedUpdated( HostedInfo* userJoinInfo );
    void                        signalInternalHostedRemoved( VxGUID hostOnlineId, EHostType hostType );

private slots:
    void                        slotInternalHostedUpdated( HostedInfo* userJoinInfo );
    void                        slotInternalHostedRemoved( VxGUID hostOnlineId, EHostType hostType );

protected:
    void                        removeHosted( VxGUID& onlineId );
    GuiHosted*                  findHosted( VxGUID& onlineId );
    GuiHosted*                  updateHosted( HostedInfo* userJoinInfo );
    
    virtual void				callbackHostedInfoListUpdated( HostedInfo* userJoinInfo ) override;
    virtual void				callbackHostedInfoListRemoved( VxGUID& userOnlineId, EHostType hostType ) override;

    AppCommon&                  m_MyApp;
    // map of online id to GuiHosted
    std::map<VxGUID, GuiHosted*>  m_HostedList;
};
