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

enum EUserJoinViewType
{
    eUserJoinViewTypeNone,
    eUserJoinViewTypeFriends,
    eUserJoinViewTypeGroup,
    eUserJoinViewTypeChatRoom,
    eUserJoinViewTypeRandomConnect,
    eUserJoinViewTypeEverybody,
    eUserJoinViewTypeIgnored,

    eMaxUserJoinViewTypeNone,
};

class UserJoinListItem;
class GuiUserJoin;
class AppCommon;
class MyIcons;
class P2PEngine;
class GuiUser;
class GuiUserJoinSession;
class GuiUserJoinMgr;
class GuiThumb;
class GuiThumbMgr;

class UserJoinListWidget : public QListWidget
{
	Q_OBJECT

public:
	UserJoinListWidget( QWidget * parent );

	AppCommon&					getMyApp( void ) { return m_MyApp; }
	MyIcons&					getMyIcons( void );
    void                        clearUserJoinList( void );

    void                        addUserJoinToList( EHostType hostType, VxGUID& sessionId,  GuiUserJoin * userIdent );
    UserJoinListItem*           addOrUpdateUserJoinSession( GuiUserJoinSession* hostSession );

    GuiUserJoinSession*         findSession( VxGUID& lclSessionId );
    UserJoinListItem*           findListEntryWidgetBySessionId( VxGUID& sessionId );
    UserJoinListItem*           findListEntryWidgetByOnlineId( VxGUID& onlineId );

    void                        updateUserJoin( GuiUserJoin * user );
    void                        removeUserJoin( VxGUID& onlineId );

    void                        setShowMyself( bool show )                  { m_ShowMyself = show; }
    bool                        getShowMyself( void )                       { return m_ShowMyself; }

    void                        setUserJoinViewType( EUserJoinViewType viewType );
    EUserJoinViewType           getUserJoinViewType( void )                     { return m_UserJoinViewType; }

signals:
    void                        signalAvatarButtonClicked( GuiUserJoinSession* hostSession, UserJoinListItem* hostItem );
    void                        signalMenuButtonClicked( GuiUserJoinSession* hostSession, UserJoinListItem* hostItem );

protected slots:
    void				        slotMyIdentUpdated( GuiUserJoin* user ); 

    void				        slotUserJoinAdded( GuiUserJoin* user ); 
    void				        slotUserJoinRemoved( VxGUID onlineId ); 
    void                        slotUserJoinUpdated( GuiUserJoin* user );
    void                        slotUserJoinOnlineStatus( GuiUserJoin* user, bool isOnline );

    void				        slotThumbAdded( GuiThumb* thumb ); 
    void                        slotThumbUpdated( GuiThumb* thumb );
    void				        slotThumbRemoved( VxGUID thumbId ); 

    void                        slotAvatarButtonClicked( UserJoinListItem* hostItem );
    void                        slotMenuButtonClicked( UserJoinListItem* hostItem );

protected:
    UserJoinListItem*           sessionToWidget( GuiUserJoinSession* hostSession );
    GuiUserJoinSession*			widgetToSession( UserJoinListItem* hostItem );

    virtual void                onListItemAdded( GuiUserJoinSession* userSession, UserJoinListItem* userItem );
    virtual void                onListItemUpdated( GuiUserJoinSession* userSession, UserJoinListItem* userItem );

    virtual void                onAvatarButtonClicked( UserJoinListItem* hostItem );
    virtual void                onMenuButtonClicked( UserJoinListItem* hostItem );

    void                        refreshList( void );
    bool                        isListViewMatch( GuiUser * user );

    void                        updateThumb( GuiThumb* thumb );
 
	//=== vars ===//
	AppCommon&					m_MyApp;
    P2PEngine&					m_Engine;
    GuiUserJoinMgr&				m_UserJoinMgr;
    GuiThumbMgr&				m_ThumbMgr;
	VxTimer						m_ClickEventTimer; // avoid duplicate clicks
    std::map<VxGUID, GuiUserJoinSession*> m_UserJoinCache;
    bool                        m_ShowMyself{ true };
    EUserJoinViewType           m_UserJoinViewType{ eUserJoinViewTypeNone };
};

