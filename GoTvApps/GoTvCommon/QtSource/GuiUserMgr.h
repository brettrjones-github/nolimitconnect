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

#include "ToGuiUserUpdateInterface.h"
#include "GuiUser.h"

#include <CoreLib/VxMutex.h>

#include <QObject>

class AppCommon;

class GuiUserMgr : public QObject, public ToGuiUserUpdateInterface
{
    Q_OBJECT
public:
    GuiUserMgr() = delete;
    GuiUserMgr( AppCommon& app );
    GuiUserMgr( const GuiUserMgr& rhs ) = delete;
	virtual ~GuiUserMgr() = default;
    void                        onAppCommonCreated( void );

    void                        setMessengerReady( bool ready )             { m_MessengerReady = ready; }
    bool                        isMessengerReady( void )                    { return m_MessengerReady; }
    GuiUser *                   getMyIdent( void )                          { return m_MyIdent; }  
    VxGUID                      getMyOnlineId( void )                       { return m_MyOnlineId; }  

    virtual void				toGuiContactAdded( VxNetIdent * netIdent ) override; 
    virtual void				toGuiContactRemoved( VxGUID& onlineId ) override; 
    virtual void				toGuiContactOnline( VxNetIdent * netIdent, EHostType hostType, bool newContact ) override; 
    virtual void				toGuiContactOffline( VxNetIdent * netIdent ) override; 
    virtual void				toGuiContactNearby( VxNetIdent * netIdent ) override; 
    virtual void				toGuiContactNotNearby( VxNetIdent * netIdent ) override; 
    virtual void				toGuiContactNameChange( VxNetIdent * netIdent ) override; 
    virtual void				toGuiContactDescChange( VxNetIdent * netIdent ) override; 
    virtual void				toGuiContactMyFriendshipChange( VxNetIdent * netIdent ) override; 
    virtual void				toGuiContactHisFriendshipChange( VxNetIdent * netIdent ) override; 
    virtual void				toGuiPluginPermissionChange( VxNetIdent * netIdent ) override; 
    virtual void				toGuiContactSearchFlagsChange( VxNetIdent * netIdent ) override; 
    virtual void				toGuiContactLastSessionTimeChange( VxNetIdent * netIdent ) override; 
    virtual void				toGuiUpdateMyIdent( VxNetIdent * netIdent ) override; 

    bool                        isUserInSession( VxGUID& onlineId );
    void                        setUserOffline( VxGUID& onlineId );

    void                        onUserAdded( GuiUser* user );
    void                        onUserRemoved( VxGUID& onlineId );
    void                        onUserUpdated( GuiUser* user );
    void                        onUserOnlineStatusChange( GuiUser* user, bool isOnline );
    void                        onMyIdentUpdated( GuiUser* user );

    void                        lockUserMgr( void )             { m_UserListMutex.lock(); }
    void                        unlockUserMgr( void )           { m_UserListMutex.lock(); }
    GuiUser*                    getUser( VxGUID& onlineId );
    std::map<VxGUID, GuiUser*>& getUserList( void )             { return m_UserList; }

signals:
    void				        signalMyIdentUpdated( GuiUser* user ); 

    void				        signalUserAdded( GuiUser* user ); 
    void				        signalUserRemoved( VxGUID onlineId ); 
    void                        signalUserUpdated( GuiUser* user );
    void                        signalUserOnlineStatus( GuiUser* user, bool isOnline );

    void                        signalInternalUpdateUser( VxNetIdent* netIdent, EHostType hostType );
    void                        signalInternalUpdateMyIdent( VxNetIdent* netIdent );
    void                        signalInternalUserRemoved( VxGUID onlineId );
    void                        signalInternalUserOnlineStatus( VxNetIdent* netIdent, EHostType hostType, bool online );

private slots:
    void                        slotInternalUpdateUser( VxNetIdent* netIdent, EHostType hostType );
    void                        slotInternalUpdateMyIdent( VxNetIdent* netIdent );
    void                        slotInternalUserRemoved( VxGUID onlineId );
    void                        slotInternalUserOnlineStatus( VxNetIdent* netIdent, EHostType hostType, bool online );

protected:
    void                        removeUser( VxGUID& onlineId );
    GuiUser*                    findUser( VxGUID& onlineId );
    GuiUser*                    updateUser( VxNetIdent* hisIdent, EHostType hostType = eHostTypeUnknown );
    void                        updateMyIdent( VxNetIdent* myIdent );

    AppCommon&                  m_MyApp;
    VxMutex                     m_UserListMutex;
    // map of online id to GuiUser
    std::map<VxGUID, GuiUser*>  m_UserList;
    bool                        m_MessengerReady{ false };
    GuiUser*                    m_MyIdent{ nullptr };
    VxGUID                      m_MyOnlineId;
};
