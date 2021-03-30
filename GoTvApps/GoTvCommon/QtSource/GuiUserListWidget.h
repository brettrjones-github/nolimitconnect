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

enum EUserListViewType
{
    eUserListViewTypeNone,
    eUserListViewTypeFriends,
    eUserListViewTypeGroupHosted,
    eUserListViewTypeChatRoomHosted,
    eUserListViewTypeRandomConnectHosted,

    eMaxUserListViewType
};

class AppCommon;
class GuiUser;
class GuiUserMgr;
class GuiHostSession;
class UserListEntryWidget;
class MyIcons;
class P2PEngine;
class PluginSetting;
class VxNetIdent;

class GuiUserListWidget : public QListWidget
{
	Q_OBJECT

public:
	GuiUserListWidget( QWidget * parent );

	AppCommon&					getMyApp( void ) { return m_MyApp; }
	MyIcons&					getMyIcons( void );
    void                        clearUserList( void );

    void                        addHostAndSettingsToList( EHostType hostType, VxGUID& sessionId, VxNetIdent& hostIdent, PluginSetting& pluginSetting );
    UserListEntryWidget*        addOrUpdateHostSession( GuiHostSession* hostSession );

    GuiHostSession*             findSession( VxGUID& lclSessionId );
    UserListEntryWidget*        findListEntryWidgetBySessionId( VxGUID& sessionId );
    UserListEntryWidget*        findListEntryWidgetByOnlineId( VxGUID& onlineId );

    void                        setUserListViewType( EUserListViewType viewType );

signals:
    void                        signalAvatarButtonClicked( GuiHostSession* hostSession, UserListEntryWidget* hostItem );
    void                        signalFriendshipButtonClicked( GuiHostSession* hostSession, UserListEntryWidget* hostItem );
    void                        signalMenuButtonClicked( GuiHostSession* hostSession, UserListEntryWidget* hostItem ); 

protected slots:
    void				        slotUserAdded( GuiUser* user ); 
    void				        slotUserRemoved( VxGUID onlineId ); 
    void                        slotUserUpdated( GuiUser* user );
    void                        slotUserOnlineStatus( GuiUser* user, bool isOnline );

	void						slotItemClicked( QListWidgetItem* item );
    void                        slotHostListItemClicked( QListWidgetItem* hostItem );
    void                        slotAvatarButtonClicked( UserListEntryWidget* hostItem );
    void                        slotFriendshipButtonClicked( UserListEntryWidget* hostItem );
    void                        slotMenuButtonClicked( UserListEntryWidget* hostItem ); 

protected:
    virtual void				showEvent( QShowEvent * ev ) override;

    bool                        isUserAListMatch( GuiUser* user );

    UserListEntryWidget*        sessionToWidget( GuiHostSession* hostSession );
    GuiHostSession*				widgetToSession( UserListEntryWidget* hostItem );

    virtual void                onHostListItemClicked( UserListEntryWidget* hostItem );
    virtual void                onAvatarButtonClicked( UserListEntryWidget* hostItem );
    virtual void                onFriendshipButtonClicked( UserListEntryWidget* hostItem );
    virtual void                onMenuButtonClicked( UserListEntryWidget* hostItem );

    void                        refreshUserList( void );
    void                        addUser( GuiUser* user );

	//=== vars ===//
	AppCommon&					m_MyApp;
    GuiUserMgr&					m_UserMgr;
	P2PEngine&					m_Engine;
	VxTimer						m_ClickEventTimer; // avoid duplicate clicks
    EUserListViewType           m_ViewType{ eUserListViewTypeNone };
    bool                        m_FirstShow{ false };
};

