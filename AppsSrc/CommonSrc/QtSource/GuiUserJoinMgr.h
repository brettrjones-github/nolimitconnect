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

#include "GuiUserJoin.h"

#include <ptop_src/ptop_engine_src/UserJoinMgr/UserJoinCallbackInterface.h>
#include <ptop_src/ptop_engine_src/UserJoinMgr/UserJoinInfo.h>

#include <CoreLib/VxMutex.h>

#include <PktLib/GroupieId.h>

#include <QObject>

// client side manager of user join to host service states

class AppCommon;
class GuiUserJoinCallback;

class GuiUserJoinMgr : public QObject, public UserJoinCallbackInterface
{
    Q_OBJECT
public:
    GuiUserJoinMgr() = delete;
    GuiUserJoinMgr( AppCommon& app );
    GuiUserJoinMgr( const GuiUserJoinMgr& rhs ) = delete;
	virtual ~GuiUserJoinMgr() = default;
    void                        onAppCommonCreated( void );
    void                        onMessengerReady( bool ready ) { }
    bool                        isMessengerReady( void );
    virtual void                onSystemReady( bool ready ) { }

    bool                        isUserJoinInSession( GroupieId& groupieId );

    void                        onUserJoinAdded( GuiUserJoin* guiUserJoin );
    void                        onUserJoinRemoved( GroupieId& groupieId );
    void                        onUserJoinUpdated( GuiUserJoin* guiUserJoin );
    void                        onUserUnJoinUpdated( GuiUserJoin* guiUserJoin );
    void                        onUserOnlineStatusChange( GuiUserJoin* guiUserJoin, bool isOnline );
    void                        onMyIdentUpdated( GuiUserJoin* guiUserJoin );

    GuiUserJoin*                getUserJoin( GroupieId& groupieId )         { return findUserJoin( groupieId ); }
    std::map<GroupieId, GuiUserJoin*>& getUserJoinList( void )              { return m_UserJoinList; }
    GuiUserJoin*                updateUserJoin( VxNetIdent* hisIdent, EHostType hostType = eHostTypeUnknown );

    void                        wantUserJoinCallbacks( GuiUserJoinCallback* client, bool enable );

signals:
    void				        signalMyIdentUpdated( GuiUserJoin* guiUserJoin );

    void				        signalUserJoinRequested( GuiUserJoin* guiUserJoin );
    void                        signalUserJoinUpdated( GuiUserJoin* guiUserJoin );
    void				        signalUserJoinRemoved( GroupieId& groupieId );
    void                        signalUserJoinOfferStateChange( GroupieId& groupieId, EJoinState hostOfferState );
    void                        signalUserJoinOnlineStatus( GuiUserJoin* guiUserJoin, bool isOnline );

    void                        signalInternalUserJoinRequested( UserJoinInfo* userJoinInfo );
    void                        signalInternalUserJoinUpdated( UserJoinInfo* userJoinInfo );
    void                        signalInternalUserUnJoinUpdated( UserJoinInfo* userJoinInfo );
    void                        signalInternalUserJoinRemoved( GroupieId groupieId );
    void                        signalInternalUserJoinOfferState( GroupieId groupieId, EJoinState hostOfferState );
    void                        signalInternalUserJoinOnlineState( GroupieId groupieId, EOnlineState onlineState, VxGUID connectionId );

private slots:
    void                        slotInternalUserJoinRequested( UserJoinInfo* userJoinInfo );
    void                        slotInternalUserJoinUpdated( UserJoinInfo* userJoinInfo );
    void                        slotInternalUserUnJoinUpdated( UserJoinInfo* userJoinInfo );
    void                        slotInternalUserJoinRemoved( GroupieId groupieId );
    void                        slotInternalUserJoinOfferState( GroupieId groupieId, EJoinState hostOfferState );
    void                        slotInternalUserJoinOnlineState( GroupieId groupieId, EOnlineState onlineState, VxGUID connectionId );

protected:
    void                        removeUserJoin( GroupieId& groupieId );
    GuiUserJoin*                findUserJoin( GroupieId& groupieId );
    GuiUserJoin*                updateUserJoin( UserJoinInfo* userJoinInfo, bool unJoin = false );
    
    virtual void				callbackUserJoinAdded( UserJoinInfo* userJoinInfo ) override;
    virtual void				callbackUserJoinUpdated( UserJoinInfo* userJoinInfo ) override;
    virtual void				callbackUserUnJoinUpdated( UserJoinInfo* userJoinInfo ) override;
    virtual void				callbackUserJoinRemoved( GroupieId& groupieId ) override;
    virtual void				callbackUserJoinOfferState( GroupieId& groupieId, EJoinState userOfferState ) override;
    virtual void				callbackUserJoinOnlineState( GroupieId& groupieId, EOnlineState onlineState, VxGUID& connectionId ) override;

    void                        announceUserJoinState( EJoinState joinState, GuiUserJoin* guiUserJoin );

    virtual void				announceUserJoinRequested( GroupieId& groupieId, GuiUserJoin* guiUserJoin );
    virtual void				announceUserJoinGranted( GroupieId& groupieId, GuiUserJoin* guiUserJoin );
    virtual void				announceUserUnJoinGranted( GroupieId& groupieId, GuiUserJoin* guiUserJoin );
    virtual void				announceUserJoinDenied( GroupieId& groupieId, GuiUserJoin* guiUserJoin );
    virtual void				announceUserJoinLeaveHost( GroupieId& groupieId );
    virtual void				announceUserJoinRemoved( GroupieId& groupieId );


    AppCommon&                  m_MyApp;
    // map of online id to GuiUserJoin
    std::map<GroupieId, GuiUserJoin*>  m_UserJoinList;

    std::vector<GuiUserJoinCallback*>  m_UserJoinClients;
};
