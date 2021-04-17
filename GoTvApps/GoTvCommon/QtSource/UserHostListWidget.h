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

#include <GuiInterface/IDefs.h>

#include <CoreLib/VxTimer.h>
#include <CoreLib/VxGUID.h>

#include <QListWidget>

enum EUserHostViewType
{
    eUserHostViewTypeNone,
    eUserHostViewTypeFriends,
    eUserHostViewTypeGroup,
    eUserHostViewTypeChatRoom,
    eUserHostViewTypeRandomConnect,
    eUserHostViewTypeEverybody,
    eUserHostViewTypeIgnored,

    eMaxUserHostViewTypeNone,
};

class UserHostListItem;
class GuiUserHost;
class AppCommon;
class MyIcons;
class P2PEngine;
class GuiUserHostSession;
class GuiUserHostMgr;
class GuiThumb;
class GuiThumbMgr;

class UserHostListWidget : public QListWidget
{
	Q_OBJECT

public:
	UserHostListWidget( QWidget * parent );

	AppCommon&					getMyApp( void ) { return m_MyApp; }
	MyIcons&					getMyIcons( void );
    void                        clearUserHostList( void );

    void                        addUserHostToList( EHostType hostType, VxGUID& sessionId,  GuiUserHost * userIdent );
    UserHostListItem*               addOrUpdateUserHostSession( GuiUserHostSession* hostSession );

    GuiUserHostSession*         findSession( VxGUID& lclSessionId );
    UserHostListItem*               findListEntryWidgetBySessionId( VxGUID& sessionId );
    UserHostListItem*               findListEntryWidgetByOnlineId( VxGUID& onlineId );

    void                        updateUserHost( GuiUserHost * user );
    void                        removeUserHost( VxGUID& onlineId );

    void                        setShowMyself( bool show )                  { m_ShowMyself = show; }
    bool                        getShowMyself( void )                       { return m_ShowMyself; }

    void                        setUserHostViewType( EUserHostViewType viewType );
    EUserHostViewType               getUserHostViewType( void )                     { return m_UserHostViewType; }

signals:
    void                        signalUserHostListItemClicked( GuiUserHostSession* hostSession, UserHostListItem* hostItem );

    void                        signalAvatarButtonClicked( GuiUserHostSession* hostSession, UserHostListItem* hostItem );
    void                        signalMenuButtonClicked( GuiUserHostSession* hostSession, UserHostListItem* hostItem );
    void                        signalFriendshipButtonClicked( GuiUserHostSession* hostSession, UserHostListItem* hostItem );

protected slots:
    void				        slotMyIdentUpdated( GuiUserHost* user ); 

    void				        slotUserHostAdded( GuiUserHost* user ); 
    void				        slotUserHostRemoved( VxGUID onlineId ); 
    void                        slotUserHostUpdated( GuiUserHost* user );
    void                        slotUserHostOnlineStatus( GuiUserHost* user, bool isOnline );

    void				        slotThumbAdded( GuiThumb* thumb ); 
    void                        slotThumbUpdated( GuiThumb* thumb );
    void				        slotThumbRemoved( VxGUID thumbId ); 

    void                        slotUserHostListItemClicked( UserHostListItem* hostItem );
    void                        slotAvatarButtonClicked( UserHostListItem* hostItem );
    void                        slotMenuButtonClicked( UserHostListItem* hostItem );
    void                        slotFriendshipButtonClicked( UserHostListItem* hostItem );

protected:
    UserHostListItem*               sessionToWidget( GuiUserHostSession* hostSession );
    GuiUserHostSession*			widgetToSession( UserHostListItem* hostItem );

    virtual void                onListItemAdded( GuiUserHostSession* userSession, UserHostListItem* userItem );
    virtual void                onListItemUpdated( GuiUserHostSession* userSession, UserHostListItem* userItem );

    virtual void                onUserHostListItemClicked( UserHostListItem* hostItem );
    virtual void                onAvatarButtonClicked( UserHostListItem* hostItem );
    virtual void                onFriendshipButtonClicked( UserHostListItem* hostItem );
    virtual void                onMenuButtonClicked( UserHostListItem* hostItem );

    void                        refreshList( void );
    bool                        isListViewMatch( GuiUserHost * user );

    void                        updateThumb( GuiThumb* thumb );
 
	//=== vars ===//
	AppCommon&					m_MyApp;
    P2PEngine&					m_Engine;
    GuiUserHostMgr&					m_UserHostMgr;
    GuiThumbMgr&				m_ThumbMgr;
	VxTimer						m_ClickEventTimer; // avoid duplicate clicks
    std::map<VxGUID, GuiUserHostSession*> m_UserHostCache;
    bool                        m_ShowMyself{ true };
    EUserHostViewType               m_UserHostViewType{ eUserHostViewTypeNone };
};

