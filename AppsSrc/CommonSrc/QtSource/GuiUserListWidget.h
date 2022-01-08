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

class GuiUserMgr;
class GuiUserSessionBase;
class GuiUserListItem;
class PluginSetting;
class VxNetIdent;

class GuiUserListWidget : public ListWidgetBase
{
	Q_OBJECT

public:
	GuiUserListWidget( QWidget * parent );

    void						setAppletType( EApplet appletType )     { m_AppletType = appletType; };
    EApplet						getAppletType( void )                   { return m_AppletType; };

    void                        setUserViewType( EUserViewType viewType );
    EUserViewType               getUserViewType( void )                 { return m_ViewType; };

    void                        clearUserList( void );

    void                        setShowMyself( bool showMe )            { m_ShowMyself = showMe; }
    bool                        getShowMyself( void )                   { return m_ShowMyself; }

    void                        addSessionToList( EHostType hostType, VxGUID& sessionId, GuiUser* hostIdent );
    GuiUserListItem*            addOrUpdateSession( GuiUserSessionBase* hostSession );

    GuiUserSessionBase*         findSession( VxGUID& lclSessionId );
    GuiUserListItem*            findListEntryWidgetBySessionId( VxGUID& sessionId );
    GuiUserListItem*            findListEntryWidgetByOnlineId( VxGUID& onlineId );

    void                        updateUser( GuiUser* user );
    void                        removeUser( VxGUID& onlineId );
    virtual GuiUserSessionBase* makeSession( GuiUser* user );

signals:
    void                        signalUserListItemClicked( GuiUserSessionBase* hostSession, GuiUserListItem* hostItem );
    void                        signalAvatarButtonClicked( GuiUserSessionBase* hostSession, GuiUserListItem* hostItem );
    void                        signalFriendshipButtonClicked( GuiUserSessionBase* hostSession, GuiUserListItem* hostItem );
    void                        signalMenuButtonClicked( GuiUserSessionBase* hostSession, GuiUserListItem* hostItem ); 

protected slots:
    void                        slotMyIdentUpdated( GuiUser* user );

    void				        slotUserAdded( GuiUser* user ); 
    void				        slotUserRemoved( VxGUID onlineId ); 
    void                        slotUserUpdated( GuiUser* user );
    void                        slotUserOnlineStatus( GuiUser* user, bool isOnline );
    
	void						slotItemClicked( QListWidgetItem* item );
    void                        slotUserListItemClicked( GuiUserListItem* hostItem );
    void                        slotAvatarButtonClicked( GuiUserListItem* hostItem );
    void                        slotFriendshipButtonClicked( GuiUserListItem* hostItem );
    void                        slotMenuButtonClicked( GuiUserListItem* hostItem ); 

    void				        slotThumbAdded( GuiThumb* thumb );
    void                        slotThumbUpdated( GuiThumb* thumb );
    void				        slotThumbRemoved( VxGUID thumbId );

protected:
    virtual void				showEvent( QShowEvent * ev ) override;

    bool                        isListViewMatch( GuiUser* user );

    GuiUserListItem*            sessionToWidget( GuiUserSessionBase* hostSession );
    GuiUserSessionBase*			widgetToSession( GuiUserListItem* hostItem );

    virtual void                onUserListItemClicked( GuiUserListItem* hostItem );
    virtual void                onAvatarButtonClicked( GuiUserListItem* hostItem );
    virtual void                onFriendshipButtonClicked( GuiUserListItem* hostItem );
    virtual void                onMenuButtonClicked( GuiUserListItem* hostItem );

    virtual void                onListItemAdded( GuiUserSessionBase* userSession, GuiUserListItem* userItem );
    virtual void                onListItemUpdated( GuiUserSessionBase* userSession, GuiUserListItem* userItem );

    void                        refreshUserList( void );
    void                        updateEntryWidget( VxGUID& onlineId );

    void                        updateThumb( GuiThumb* thumb );

	//=== vars ===//
    EApplet						m_AppletType{ eAppletUnknown };
    EUserViewType               m_ViewType{ eUserViewTypeNone };
    bool                        m_ShowMyself{ false };
    std::map<VxGUID, GuiUserSessionBase*> m_UserCache;
};

