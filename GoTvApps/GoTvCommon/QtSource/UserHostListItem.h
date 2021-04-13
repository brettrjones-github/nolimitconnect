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
#include "ui_UserHostListItem.h"

#include <QListWidgetItem>
#include <QWidget>

class GuiConnectMgr;
class GuiOfferMgr;
class GuiUserHostSession;
class GuiUser;
class GuiUserHost;
class GuiUserHostMgr;
class GuiUserMgr;
class GuiThumb;

class UserHostListItem : public QWidget, public QListWidgetItem
{
	Q_OBJECT;
public:
	UserHostListItem( QWidget *parent=0 );
	virtual ~UserHostListItem();

    AppCommon&					getMyApp( void ) { return m_MyApp; }
    MyIcons&					getMyIcons( void );

    void                        setUserHostSession( GuiUserHostSession* hostSession );
    GuiUserHostSession*         getUserHostSession( void );

    VxPushButton *              getAvatarButton( void )                 { return ui.m_AvatarButton; }
    VxPushButton *              getFriendshipButton( void )             { return ui.m_FriendshipButton; }
    VxPushButton *              getMenuButton( void )                   { return ui.m_MenuButton; }

    void						updateWidgetFromInfo( void );
    void                        updateThumb( GuiThumb* thumb );

    QSize                       calculateSizeHint( void );

signals:
    void						signalUserHostListItemClicked( UserHostListItem * poItemWidget );
	void						signalAvatarButtonClicked( UserHostListItem* listEntryWidget );
    void						signalFriendshipButtonClicked( UserHostListItem* listEntryWidget );
	void						signalMenuButtonClicked( UserHostListItem* listEntryWidget );

public slots:
	void						slotAvatarButtonClicked( void );
    void						slotFriendshipButtonClicked( void );
	void						slotMenuButtonClicked( void );

protected:
    virtual void				mousePressEvent( QMouseEvent * event ) override;
    virtual void				resizeEvent( QResizeEvent* resizeEvent ) override;

	//=== vars ===//
    Ui::UserHostListItemUi	    ui;
    AppCommon&					m_MyApp;
    GuiConnectMgr&				m_ConnectMgr;
    GuiOfferMgr&				m_OfferMgr;
    GuiUserMgr&					m_UserMgr;
    GuiUserHostMgr&				m_UserHostMgr;
};




