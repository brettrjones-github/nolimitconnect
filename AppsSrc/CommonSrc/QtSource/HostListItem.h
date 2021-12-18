#pragma once
//============================================================================
// Copyright (C) 2021 Brett R. Jones 
// Issued to MIT style license by Brett R. Jones in 2017
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

#include "config_apps.h"
#include "ui_HostListItem.h"
#include "IdentLogicInterface.h"

#include <QListWidgetItem>

class GuiHostSession;

class HostListItem : public IdentLogicInterface, public QListWidgetItem
{
	Q_OBJECT;
public:
	HostListItem( QWidget *parent=0 );
	virtual ~HostListItem();

    AppCommon&					getMyApp( void ) { return m_MyApp; }
    MyIcons&					getMyIcons( void );

    void                        setHostSession( GuiHostSession* hostSession );
    GuiHostSession*             getHostSession( void );

    VxPushButton*               getIdentAvatarButton( void ) override         { return ui.m_IconButton; }
    VxPushButton*               getIdentFriendshipButton( void ) override { return ui.m_FriendshipButton; }
    VxPushButton*               getIdentMenuButton( void ) override { return ui.m_MenuButton; }

    virtual QLabel*             getIdentLine1( void ) override { return ui.m_TitlePart1; }
    virtual QLabel*             getIdentLine2( void ) override { return ui.m_DescPart1; }

    void						onIdentAvatarButtonClicked( void ) override;
    void						onIdentMenuButtonClicked( void ) override;

    void                        setJoinedState( EJoinState joinState );

    void						updateWidgetFromInfo( void );

signals:
    void						signalHostListItemClicked( QListWidgetItem * poItemWidget );
	void						signalIconButtonClicked( HostListItem* listEntryWidget );
    void						signalFriendshipButtonClicked( HostListItem* listEntryWidget );
	void						signalMenuButtonClicked( HostListItem* listEntryWidget );
    void						signalJoinButtonClicked( HostListItem* listEntryWidget );
    void						signalConnectButtonClicked( HostListItem* listEntryWidget );

public slots:
    void						slotJoinButtonPressed( void );
    void						slotConnectButtonPressed( void );
 
protected:
    virtual void				mousePressEvent( QMouseEvent * event ) override;
    virtual void				resizeEvent( QResizeEvent* resizeEvent ) override;
    virtual void                showConnectButton( bool isAccepted );

	//=== vars ===//
    Ui::HostListItemUi	        ui;
    AppCommon&					m_MyApp;
};




