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

#include <QObject>

class AppCommon;

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

    GuiUserJoin *               getMyIdent( void )                          { return m_MyIdent; }  
    VxGUID                      getMyOnlineId( void )                       { return m_MyOnlineId; }  

    bool                        isUserJoinInSession( VxGUID& onlineId );
    void                        setUserJoinOffline( VxGUID& onlineId );

    void                        onUserJoinAdded( GuiUserJoin* user );
    void                        onUserJoinRemoved( VxGUID& onlineId, EHostType hostType );
    void                        onUserJoinUpdated( GuiUserJoin* user );
    void                        onUserJoinOnlineStatusChange( GuiUserJoin* user, bool isOnline );
    void                        onMyIdentUpdated( GuiUserJoin* user );

    GuiUserJoin*                    getUserJoin( VxGUID& onlineId );
    std::map<VxGUID, GuiUserJoin*>& getUserJoinList( void )             { return m_UserJoinList; }

    void                        updateMyIdent( VxNetIdent* myIdent );


signals:
    void				        signalMyIdentUpdated( GuiUserJoin* user ); 

    void				        signalUserJoinRequested( GuiUserJoin* user ); 
    void                        signalUserJoinUpdated( GuiUserJoin* user );
    void				        signalUserJoinRemoved( VxGUID onlineId, EHostType hostType );
    void                        signalUserJoinOfferStateChange( VxGUID& userOnlineId, EHostType hostType, EJoinState hostOfferState );
    void                        signalUserJoinOnlineStatus( GuiUserJoin* user, bool isOnline );

    void                        signalInternalUserJoinRequested( UserJoinInfo* userJoinInfo );
    void                        signalInternalUserJoinUpdated( UserJoinInfo* userJoinInfo );
    void                        signalInternalUserJoinRemoved( VxGUID hostOnlineId, EPluginType pluginType );
    void                        signalInternalUserJoinOfferState( VxGUID hostOnlineId, EPluginType pluginType, EJoinState hostOfferState );
    void                        signalInternalUserJoinOnlineState( VxGUID hostOnlineId, EPluginType pluginType, EOnlineState onlineState, VxGUID connectionId );

private slots:
    void                        slotInternalUserJoinRequested( UserJoinInfo* userJoinInfo );
    void                        slotInternalUserJoinUpdated( UserJoinInfo* userJoinInfo );
    void                        slotInternalUserJoinRemoved( VxGUID hostOnlineId, EPluginType pluginType );
    void                        slotInternalUserJoinOfferState( VxGUID userOnlineId, EPluginType pluginType, EJoinState hostOfferState );
    void                        slotInternalUserJoinOnlineState( VxGUID userOnlineId, EPluginType pluginType, EOnlineState onlineState, VxGUID connectionId );

protected:
    void                        removeUserJoin( VxGUID& onlineId );
    GuiUserJoin*                findUserJoin( VxGUID& onlineId );
    GuiUserJoin*                updateUserJoin( VxNetIdent* hisIdent, EHostType hostType = eHostTypeUnknown );
    GuiUserJoin*                updateUserJoin( UserJoinInfo* userJoinInfo );
    
    virtual void				callbackUserJoinRequested( UserJoinInfo* userJoinInfo ) override;
    virtual void				callbackUserJoinUpdated( UserJoinInfo* userJoinInfo ) override;
    virtual void				callbackUserJoinRemoved( VxGUID& userOnlineId, EPluginType pluginType ) override;
    virtual void				callbackUserJoinOfferState( VxGUID& userOnlineId, EPluginType pluginType, EJoinState userOfferState ) override;
    virtual void				callbackUserJoinOnlineState( VxGUID& userOnlineId, EPluginType pluginType, EOnlineState onlineState, VxGUID& connectionId ) override;

    AppCommon&                  m_MyApp;
    VxNetIdent*                 m_NetIdent{ nullptr };
    // map of online id to GuiUserJoin
    std::map<VxGUID, GuiUserJoin*>  m_UserJoinList;
    GuiUserJoin*                m_MyIdent{ nullptr };
    VxGUID                      m_MyOnlineId;
};
