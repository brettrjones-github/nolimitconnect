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

#include "GuiUser.h"
#include "GuiUserUpdateCallback.h"
#include "ToGuiUserUpdateInterface.h"

#include <CoreLib/VxMutex.h>

#include <QObject>

class AppCommon;
class GuiUserUpdateCallback;

class GuiUserMgr : public QObject, public ToGuiUserUpdateInterface
{
    Q_OBJECT
public:
    GuiUserMgr() = delete;
    GuiUserMgr( AppCommon& app );
    GuiUserMgr( const GuiUserMgr& rhs ) = delete;
	virtual ~GuiUserMgr() = default;
    virtual void                onAppCommonCreated( void );
    virtual void                onMessengerReady( bool ready ) { }
    virtual bool                isMessengerReady( void );
    virtual void                onSystemReady( bool ready ) { }

    GuiUser *                   getMyIdent( void );
    VxGUID                      getMyOnlineId( void )                       { return m_MyOnlineId; }  

    virtual void				toGuiIndentListUpdate( EUserViewType listType, VxGUID& onlineId, uint64_t timestamp ) override;
    virtual void				toGuiIndentListRemove( EUserViewType listType, VxGUID& onlineId ) override;

    virtual void				toGuiContactAdded( VxNetIdent * netIdent ) override; 
    virtual void				toGuiContactRemoved( VxGUID& onlineId ) override; 
    virtual void				toGuiContactOnline( VxNetIdent * netIdent ) override; 
    virtual void				toGuiContactOffline( VxNetIdent * netIdent ) override; 
    virtual void				toGuiContactOffline( VxGUID& onlineId );
    virtual void				toGuiContactNameChange( VxNetIdent * netIdent ) override; 
    virtual void				toGuiContactDescChange( VxNetIdent * netIdent ) override; 
    virtual void				toGuiContactMyFriendshipChange( VxNetIdent * netIdent ) override; 
    virtual void				toGuiContactHisFriendshipChange( VxNetIdent * netIdent ) override; 
    virtual void				toGuiPluginPermissionChange( VxNetIdent * netIdent ) override; 
    virtual void				toGuiContactSearchFlagsChange( VxNetIdent * netIdent ) override; 
    virtual void				toGuiContactLastSessionTimeChange( VxNetIdent * netIdent ) override; 
    virtual void				toGuiUpdateMyIdent( VxNetIdent* netIdent ) override;
    virtual void				toGuiSaveMyIdent( VxNetIdent * netIdent ) override; 

    virtual void				toGuiUserOnlineStatus( VxNetIdent* hostIdent, bool isOnline );

    bool                        isUserRelayed( VxGUID& onlineId );
    bool                        isUserOnline( VxGUID& onlineId );
    bool                        isUserInSession( VxGUID& onlineId );
    void                        setUserOffline( VxGUID& onlineId );

    void                        onUserAdded( GuiUser* user );
    void                        onUserRemoved( VxGUID& onlineId );
    void                        onUserUpdated( GuiUser* user );
    void                        onUserNearbyStatusChange( GuiUser* user );
    void                        onUserRelayStatusChange( GuiUser* user );
    void                        onUserOnlineStatusChange( GuiUser* user );
    void                        onMyIdentUpdated( GuiUser* user );

    GuiUser*                    getUser( VxGUID& onlineId );
    GuiUser*                    getOrQueryUser( VxGUID& onlineId );
    std::string                 getUserOnlineName( VxGUID& onlineId );
    
    std::map<VxGUID, GuiUser*>& getUserList( void )             { return m_UserList; }

    GuiUser*                    updateMyIdent( VxNetIdent* myIdent );
    GuiUser*                    updateUser( VxNetIdent* hisIdent );
    void                        updateOnlineStatus( VxNetIdent* netIdent, bool online );

    void                        wantGuiUserUpdateCallbacks( GuiUserUpdateCallback* callback, bool wantCallback );

    void                        connnectIdNearbyStatusChange( VxGUID& onlineId, uint64_t nearbyTimeOrZeroIfNot );
    void                        connnectIdRelayStatusChange( VxGUID& onlineId );
    void                        connnectIdOnlineStatusChange( VxGUID& onlineId );

signals:
    void				        signalMyIdentUpdated( GuiUser* guiUser );


    void				        signalUserAdded( GuiUser* guiUser ); 
    void				        signalUserRemoved( VxGUID onlineId ); 
    void                        signalUserUpdated( GuiUser* guiUser );
    void                        signalUserOnlineStatus( GuiUser* guiUser );

    void				        signalInternalIndentListUpdate( EUserViewType listType, VxGUID onlineId, uint64_t timestamp );
    void				        signalInternalIndentListRemove( EUserViewType listType, VxGUID onlineId );

    void                        signalInternalUpdateUser( VxNetIdent* netIdent );
    void                        signalInternalSaveMyIdent( VxNetIdent* netIdent );
    void                        signalInternalUserRemoved( VxGUID onlineId );

private slots:
    void				        slotInternalIndentListUpdate( EUserViewType listType, VxGUID onlineId, uint64_t timestamp );
    void				        slotInternalIndentListRemove( EUserViewType listType, VxGUID onlineId );

    void                        slotInternalUpdateUser( VxNetIdent* netIdent );
    void                        slotInternalSaveMyIdent( VxNetIdent* netIdent );
    void                        slotInternalUserRemoved( VxGUID onlineId );

protected:
    void                        removeUser( VxGUID& onlineId );
    GuiUser*                    findUser( VxGUID& onlineId );

    void                        guiUserUpdateClientsLock( void );
    void                        guiUserUpdateClientsUnlock( void );
    void                        clearGuiUserUpdateClientList( void );

    void                        sendUserUpdatedToCallbacks( GuiUser* guiUser );
    
    AppCommon&                  m_MyApp;
    VxMutex                     m_GuiUserUpdateClientMutex;
    std::vector<GuiUserUpdateCallback*> m_GuiUserUpdateClientList;

    // map of online id to GuiUser
    std::map<VxGUID, GuiUser*>  m_UserList;
    GuiUser*                    m_MyIdent{ nullptr };
    VxGUID                      m_MyOnlineId;

};
