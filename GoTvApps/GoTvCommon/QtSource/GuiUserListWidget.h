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

#include "ListWidgetBase.h"

enum EUserListViewType
{
    eUserListViewTypeNone,
    eUserListViewTypeFriends,
    eUserListViewTypeGroupHosted,
    eUserListViewTypeChatRoomHosted,
    eUserListViewTypeRandomConnectHosted,

    eMaxUserListViewType
};

class GuiUserMgr;
class GuiUserSessionBase;
class UserListItem;
class PluginSetting;
class VxNetIdent;

class GuiUserListWidget : public ListWidgetBase
{
	Q_OBJECT

public:
	GuiUserListWidget( QWidget * parent );

    void                        clearUserList( void );

    void                        setShowMyself( bool showMe )        { m_ShowMyself = showMe; }
    bool                        getShowMyself( void )                  { return m_ShowMyself; }

    void                        addSessionToList( EHostType hostType, VxGUID& sessionId, GuiUser* hostIdent );
    UserListItem*               addOrUpdateSession( GuiUserSessionBase* hostSession );

    GuiUserSessionBase*         findSession( VxGUID& lclSessionId );
    UserListItem*               findListEntryWidgetBySessionId( VxGUID& sessionId );
    UserListItem*               findListEntryWidgetByOnlineId( VxGUID& onlineId );

    void                        setUserListViewType( EUserListViewType viewType );

    virtual GuiUserSessionBase* makeSession( GuiUser* user );

signals:
    void                        signalUserListItemClicked( GuiUserSessionBase* hostSession, UserListItem* hostItem );
    void                        signalAvatarButtonClicked( GuiUserSessionBase* hostSession, UserListItem* hostItem );
    void                        signalFriendshipButtonClicked( GuiUserSessionBase* hostSession, UserListItem* hostItem );
    void                        signalMenuButtonClicked( GuiUserSessionBase* hostSession, UserListItem* hostItem ); 

protected slots:
    void                        slotMyIdentUpdated( GuiUser* user );

    void				        slotUserAdded( GuiUser* user ); 
    void				        slotUserRemoved( VxGUID onlineId ); 
    void                        slotUserUpdated( GuiUser* user );
    void                        slotUserOnlineStatus( GuiUser* user, bool isOnline );
    
	void						slotItemClicked( QListWidgetItem* item );
    void                        slotUserListItemClicked( UserListItem* hostItem );
    void                        slotAvatarButtonClicked( UserListItem* hostItem );
    void                        slotFriendshipButtonClicked( UserListItem* hostItem );
    void                        slotMenuButtonClicked( UserListItem* hostItem ); 

protected:
    virtual void				showEvent( QShowEvent * ev ) override;

    bool                        isUserAListMatch( GuiUser* user );

    UserListItem*               sessionToWidget( GuiUserSessionBase* hostSession );
    GuiUserSessionBase*			widgetToSession( UserListItem* hostItem );

    virtual void                onUserListItemClicked( UserListItem* hostItem );
    virtual void                onAvatarButtonClicked( UserListItem* hostItem );
    virtual void                onFriendshipButtonClicked( UserListItem* hostItem );
    virtual void                onMenuButtonClicked( UserListItem* hostItem );

    void                        refreshUserList( void );
    void                        updateUser( GuiUser* user );
    void                        updateEntryWidget( VxGUID& onlineId );
    void                        removeUser( VxGUID& onlineId );

	//=== vars ===//
    EUserListViewType           m_ViewType{ eUserListViewTypeNone };
    bool                        m_ShowMyself{ false };
    std::map<VxGUID, GuiUserSessionBase*> m_UserCache;
};

