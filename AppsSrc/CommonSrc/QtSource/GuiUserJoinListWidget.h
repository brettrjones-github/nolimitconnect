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

#include <QWidget> // must be declared first or linux Qt 6.2.4 will error in qmetatype.h 2167:23: array subscript value ‘53’ is outside the bounds

#include "GuiUserUpdateCallback.h"
#include <GuiInterface/IDefs.h>

#include <CoreLib/VxTimer.h>
#include <PktLib/GroupieId.h>

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

class GuiUserJoinListItem;
class GuiUserJoin;
class AppCommon;
class MyIcons;
class P2PEngine;
class GuiUser;
class GuiUserJoinSession;
class GuiUserJoinMgr;
class GuiThumb;
class GuiThumbMgr;

class GuiUserJoinListWidget : public QListWidget, public GuiUserUpdateCallback
{
	Q_OBJECT

public:
	GuiUserJoinListWidget( QWidget * parent );
    virtual ~GuiUserJoinListWidget();

	AppCommon&					getMyApp( void ) { return m_MyApp; }
	MyIcons&					getMyIcons( void );
    void                        clearUserJoinList( void );

    void                        addUserJoinToList( VxGUID& sessionId, GuiUserJoin* guiUserJoin );
    GuiUserJoinListItem*        addOrUpdateUserJoinSession( GuiUserJoinSession* hostSession );

    GuiUserJoinSession*         findSession( VxGUID& lclSessionId );
    GuiUserJoinListItem*        findListEntryWidgetBySessionId( VxGUID& sessionId );
    GuiUserJoinListItem*        findListEntryWidgetByGroupieId( GroupieId& groupieId );

    void                        updateUserJoin( GuiUserJoin* guiUserJoin );
    void                        removeUserJoin( GroupieId& groupieId );

    void                        setShowMyself( bool show )                      { m_ShowMyself = show; }
    bool                        getShowMyself( void )                           { return m_ShowMyself; }

    void                        setUserJoinViewType( EUserJoinViewType viewType );
    EUserJoinViewType           getUserJoinViewType( void )                     { return m_UserJoinViewType; }

    void                        updateUser( GuiUser* guiUser );

signals:
    void                        signalAvatarButtonClicked( GuiUserJoinSession* hostSession, GuiUserJoinListItem* hostItem );
    void                        signalMenuButtonClicked( GuiUserJoinSession* hostSession, GuiUserJoinListItem* hostItem );

protected slots:
    void				        slotMyIdentUpdated( GuiUserJoin* guiUserJoin );

    void				        slotUserJoinAdded( GuiUserJoin* guiUserJoin );
    void				        slotUserJoinRemoved( GroupieId& groupieId );
    void                        slotUserJoinUpdated( GuiUserJoin* guiUserJoin );
    void                        slotUserJoinOnlineStatus( GuiUserJoin* guiUserJoin, bool isOnline );

    void				        slotThumbAdded( GuiThumb* thumb ); 
    void                        slotThumbUpdated( GuiThumb* thumb );
    void				        slotThumbRemoved( VxGUID thumbId ); 

    void                        slotAvatarButtonClicked( GuiUserJoinListItem* hostItem );
    void                        slotMenuButtonClicked( GuiUserJoinListItem* hostItem );

protected:
    GuiUserJoinListItem*        sessionToWidget( GuiUserJoinSession* hostSession );
    GuiUserJoinSession*			widgetToSession( GuiUserJoinListItem* hostItem );

    virtual void                onListItemAdded( GuiUserJoinSession* userSession, GuiUserJoinListItem* userItem );
    virtual void                onListItemUpdated( GuiUserJoinSession* userSession, GuiUserJoinListItem* userItem );

    virtual void                onAvatarButtonClicked( GuiUserJoinListItem* hostItem );
    virtual void                onMenuButtonClicked( GuiUserJoinListItem* hostItem );

    void                        refreshList( void );
    bool                        isListViewMatch( GuiUser * user );

    void                        updateThumb( GuiThumb* thumb );
 
	//=== vars ===//
	AppCommon&					m_MyApp;
    P2PEngine&					m_Engine;
    GuiUserJoinMgr&				m_UserJoinMgr;
    GuiThumbMgr&				m_ThumbMgr;
	VxTimer						m_ClickEventTimer; // avoid duplicate clicks
    std::map<GroupieId, GuiUserJoinSession*> m_UserJoinCache;
    bool                        m_ShowMyself{ true };
    EUserJoinViewType           m_UserJoinViewType{ eUserJoinViewTypeNone };
};

