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
// http://www.nolimitconnect.org
//============================================================================

#include "ListWidgetBase.h"
#include "AppDefs.h"
#include "GuiUserUpdateCallback.h"

class GuiUserMgr;
class GuiUserSessionBase;
class GuiUserListItem;
class PluginSetting;
class VxNetIdent;

class GuiUserListWidget : public ListWidgetBase, public GuiUserUpdateCallback
{
	Q_OBJECT

public:
	GuiUserListWidget( QWidget * parent );
    virtual ~GuiUserListWidget();

    void						setAppletType( EApplet appletType )     { m_AppletType = appletType; };
    EApplet						getAppletType( void )                   { return m_AppletType; };

    void                        setUserViewType( EUserViewType viewType );
    EUserViewType               getUserViewType( void )                 { return m_ViewType; };

    void                        clearUserList( void );

    void                        setShowMyself( bool showMe )            { m_ShowMyself = showMe; }
    bool                        getShowMyself( void )                   { return m_ShowMyself; }

    void                        addSessionToList( EHostType hostType, VxGUID& sessionId, GuiUser* userIdent );
    GuiUserListItem*            addOrUpdateSession( GuiUserSessionBase* userSession );

    GuiUserSessionBase*         findSession( VxGUID& lclSessionId );
    GuiUserListItem*            findListEntryWidgetBySessionId( VxGUID& sessionId );
    GuiUserListItem*            findListEntryWidgetByOnlineId( VxGUID& onlineId );

    void                        updateUser( GuiUser* user );
    void                        removeUser( VxGUID& onlineId );
    virtual GuiUserSessionBase* makeSession( GuiUser* user );

    virtual void				callbackOnUserUpdated( GuiUser* guiUser ) override;
    virtual void				callbackPushToTalkStatus( VxGUID& onlineId, EPushToTalkStatus pushToTalkStatus ) override;

signals:
    void                        signalUserListItemClicked( GuiUserSessionBase* userSession, GuiUserListItem* userItem );
    void                        signalAvatarButtonClicked( GuiUserSessionBase* userSession, GuiUserListItem* userItem );
    void                        signalOfferButtonClicked( GuiUserSessionBase* userSession, GuiUserListItem* userItem );
    void                        signalPushToTalkButtonPressed( GuiUserSessionBase* userSession, GuiUserListItem* userItem );
    void                        signalPushToTalkButtonReleased( GuiUserSessionBase* userSession, GuiUserListItem* userItem );
    void                        signalMenuButtonClicked( GuiUserSessionBase* userSession, GuiUserListItem* userItem ); 

protected slots:
    void                        slotMyIdentUpdated( GuiUser* user );

    void				        slotUserAdded( GuiUser* user ); 
    void				        slotUserRemoved( VxGUID onlineId ); 
    void                        slotUserUpdated( GuiUser* user );
    void                        slotUserOnlineStatus( GuiUser* user );
    
	void						slotItemClicked( QListWidgetItem* item );
    void                        slotUserListItemClicked( GuiUserListItem* userItem );
    void                        slotAvatarButtonClicked( GuiUserListItem* userItem );
    void                        slotOfferButtonClicked( GuiUserListItem* userItem );
    void                        slotPushToTalkButtonPressed( GuiUserListItem* userItem );
    void                        slotPushToTalkButtonReleased( GuiUserListItem* userItem );
    void                        slotMenuButtonClicked( GuiUserListItem* userItem ); 

    void				        slotThumbAdded( GuiThumb* thumb );
    void                        slotThumbUpdated( GuiThumb* thumb );
    void				        slotThumbRemoved( VxGUID thumbId );

protected:
    virtual void				showEvent( QShowEvent * ev ) override;

    bool                        isListViewMatch( GuiUser* user );

    GuiUserListItem*            sessionToWidget( GuiUserSessionBase* userSession );
    GuiUserSessionBase*			widgetToSession( GuiUserListItem* userItem );

    virtual void                onUserListItemClicked( GuiUserListItem* userItem );
    virtual void                onAvatarButtonClicked( GuiUserListItem* userItem );
    virtual void                onOfferButtonClicked( GuiUserListItem* userItem );
    virtual void                onPushToTalkButtonPressed( GuiUserListItem* userItem );
    virtual void                onPushToTalkButtonReleased( GuiUserListItem* userItem );
    virtual void                onMenuButtonClicked( GuiUserListItem* userItem );

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

