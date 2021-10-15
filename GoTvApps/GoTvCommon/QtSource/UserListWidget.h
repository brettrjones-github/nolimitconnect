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
#include "AppDefs.h"

enum EUserViewType
{
    eUserViewTypeNone,
    eUserViewTypeFriends,
    eUserViewTypeGroup,
    eUserViewTypeChatRoom,
    eUserViewTypeRandomConnect,
    eUserViewTypeEverybody,
    eUserViewTypeIgnored,

    eMaxUserViewTypeNone,
};

class UserListItem;
class GuiUserSessionBase;

class UserListWidget : public ListWidgetBase
{
	Q_OBJECT

public:
	UserListWidget( QWidget * parent );

    void						setAppletType( EApplet appletType ) { m_AppletType = appletType; };
    EApplet						getAppletType( void ) { return m_AppletType; };

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

    void				        slotThumbAdded( GuiThumb* thumb ); 
    void                        slotThumbUpdated( GuiThumb* thumb );
    void				        slotThumbRemoved( VxGUID thumbId ); 

    void                        slotUserListItemClicked( UserListItem* hostItem );
    void                        slotAvatarButtonClicked( UserListItem* hostItem );
    void                        slotMenuButtonClicked( UserListItem* hostItem );
    void                        slotFriendshipButtonClicked( UserListItem* hostItem );

protected:
    UserListItem*               sessionToWidget( GuiUserSessionBase* hostSession );
    GuiUserSessionBase*			widgetToSession( UserListItem* hostItem );

    virtual void                onListItemAdded( GuiUserSessionBase* userSession, UserListItem* userItem );
    virtual void                onListItemUpdated( GuiUserSessionBase* userSession, UserListItem* userItem );

    virtual void                onUserListItemClicked( UserListItem* hostItem );
    virtual void                onAvatarButtonClicked( UserListItem* hostItem );
    virtual void                onFriendshipButtonClicked( UserListItem* hostItem );
    virtual void                onMenuButtonClicked( UserListItem* hostItem );

    void                        refreshList( void );
    bool                        isListViewMatch( GuiUser * user );

    void                        updateThumb( GuiThumb* thumb );
 
	//=== vars ===//
    std::map<VxGUID, GuiUserSessionBase*> m_UserCache;
    bool                        m_ShowMyself{ true };
    EUserViewType               m_UserViewType{ eUserViewTypeNone };
    EApplet						m_AppletType{ eAppletUnknown };
};

