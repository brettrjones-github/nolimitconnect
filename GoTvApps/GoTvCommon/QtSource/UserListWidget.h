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

#include <GoTvInterface/IDefs.h>

#include <CoreLib/VxTimer.h>
#include <CoreLib/VxGUID.h>

#include <QListWidget>

enum EUserViewType
{
    eUserViewTypeNone,
    eUserViewTypeFriends,
    eUserViewTypeGroup,
    eUserViewTypeChatRoom,
    eUserViewTypeRandomConnect,

    eMaxUserViewTypeNone,
};

class UserListItem;
class GuiUser;
class AppCommon;
class MyIcons;
class P2PEngine;
class GuiUserSessionBase;
class GuiUserMgr;

class UserListWidget : public QListWidget
{
	Q_OBJECT

public:
	UserListWidget( QWidget * parent );

	AppCommon&					getMyApp( void ) { return m_MyApp; }
	MyIcons&					getMyIcons( void );
    void                        clearUserList( void );

    void                        addUserToList( EHostType hostType, VxGUID& sessionId,  GuiUser * userIdent );
    UserListItem*               addOrUpdateUserSession( GuiUserSessionBase* hostSession );

    GuiUserSessionBase*         findSession( VxGUID& lclSessionId );
    UserListItem*               findListEntryWidgetBySessionId( VxGUID& sessionId );
    UserListItem*               findListEntryWidgetByOnlineId( VxGUID& onlineId );

    void                        updateUser( GuiUser * user );
    void                        removeUser( VxGUID& onlineId );

    void                        setShowMyself( bool show )                  { m_ShowMyself = show; }
    bool                        getShowMyself( void )                       { return m_ShowMyself; }

    void                        setUserViewType( EUserViewType viewType );
    EUserViewType               getUserViewType( void )                     { return m_UserViewType; }

signals:
    void                        signalUserListItemClicked( GuiUserSessionBase* hostSession, UserListItem* hostItem );

    void                        signalAvatarButtonClicked( GuiUserSessionBase* hostSession, UserListItem* hostItem );
    void                        signalMenuButtonClicked( GuiUserSessionBase* hostSession, UserListItem* hostItem );
    void                        signalFriendshipButtonClicked( GuiUserSessionBase* hostSession, UserListItem* hostItem );

protected slots:
    void				        slotMyIdentUpdated( GuiUser* user ); 

    void				        slotUserAdded( GuiUser* user ); 
    void				        slotUserRemoved( VxGUID onlineId ); 
    void                        slotUserUpdated( GuiUser* user );
    void                        slotUserOnlineStatus( GuiUser* user, bool isOnline );

    void                        slotUserListItemClicked( UserListItem* hostItem );
    void                        slotAvatarButtonClicked( UserListItem* hostItem );
    void                        slotMenuButtonClicked( UserListItem* hostItem );
    void                        slotFriendshipButtonClicked( UserListItem* hostItem );

protected:
    UserListItem*               sessionToWidget( GuiUserSessionBase* hostSession );
    GuiUserSessionBase*			widgetToSession( UserListItem* hostItem );

    virtual void                onUserListItemClicked( UserListItem* hostItem );
    virtual void                onAvatarButtonClicked( UserListItem* hostItem );
    virtual void                onFriendshipButtonClicked( UserListItem* hostItem );
    virtual void                onMenuButtonClicked( UserListItem* hostItem );

    void                        refreshList( void );
    bool                        isListViewMatch( GuiUser * user );


	//=== vars ===//
	AppCommon&					m_MyApp;
    GuiUserMgr&					m_UserMgr;
	P2PEngine&					m_Engine;
	VxTimer						m_ClickEventTimer; // avoid duplicate clicks
    std::map<VxGUID, GuiUserSessionBase*> m_UserCache;
    bool                        m_ShowMyself{ true };
    EUserViewType               m_UserViewType{ eUserViewTypeNone };
};

