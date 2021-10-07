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

#include "config_gotvapps.h"
#include "ui_UserJoinListItem.h"

#include <QListWidgetItem>
#include <QWidget>

class GuiConnectMgr;
class GuiOfferClientMgr;
class GuiUserJoinSession;
class GuiUser;
class GuiUserJoin;
class GuiUserJoinMgr;
class GuiUserMgr;
class GuiThumb;

class UserJoinListItem : public QWidget, public QListWidgetItem
{
	Q_OBJECT;
public:
	UserJoinListItem( QWidget *parent=0 );
	virtual ~UserJoinListItem();

    AppCommon&					getMyApp( void ) { return m_MyApp; }
    MyIcons&					getMyIcons( void );

    void                        setUserJoinSession( GuiUserJoinSession* hostSession );
    GuiUserJoinSession*         getUserJoinSession( void );

    VxPushButton *              getAvatarButton( void )                 { return ui.m_AvatarButton; }
    VxPushButton *              getFriendshipButton( void )             { return ui.m_FriendshipButton; }
    VxPushButton *              getMenuButton( void )                   { return ui.m_MenuButton; }

    void						updateWidgetFromInfo( void );
    void                        updateThumb( GuiThumb* thumb );

    QSize                       calculateSizeHint( void );

signals:
    void						signalUserJoinListItemClicked( UserJoinListItem * poItemWidget );
	void						signalAvatarButtonClicked( UserJoinListItem* listEntryWidget );
    void						signalFriendshipButtonClicked( UserJoinListItem* listEntryWidget );
	void						signalMenuButtonClicked( UserJoinListItem* listEntryWidget );

public slots:
	void						slotAvatarButtonClicked( void );
    void						slotFriendshipButtonClicked( void );
	void						slotMenuButtonClicked( void );

protected:
    virtual void				mousePressEvent( QMouseEvent * event ) override;
    virtual void				resizeEvent( QResizeEvent* resizeEvent ) override;

	//=== vars ===//
    Ui::UserJoinListItemUi	    ui;
    AppCommon&					m_MyApp;
    GuiConnectMgr&				m_ConnectMgr;
    GuiOfferClientMgr&			m_OfferClientMgr;
    GuiUserMgr&					m_UserMgr;
    GuiUserJoinMgr&				m_UserJoinMgr;
};




