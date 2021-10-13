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

#include "config_gotvapps.h"
#include "ui_HostJoinRequestListItem.h"

#include <QListWidgetItem>
#include <QWidget>

class GuiHostSession;

class HostJoinRequestListItem : public QWidget, public QListWidgetItem
{
	Q_OBJECT;
public:
	HostJoinRequestListItem( QWidget *parent=nullptr );
	virtual ~HostJoinRequestListItem();

    AppCommon&					getMyApp( void ) { return m_MyApp; }
    MyIcons&					getMyIcons( void );

    void                        setHostSession( GuiHostSession* hostSession );
    GuiHostSession*             getHostSession( void );

    VxPushButton*               getAvatarButton( void )         { return ui.m_IconButton; }
    //VxPushButton*               getFriendshipButton( void )     { return ui.m_FriendshipButton; }
    VxPushButton*               getMenuButton( void )           { return ui.m_MenuButton; }

    void                        setJoinedState( EJoinState joinState );

    void						updateWidgetFromInfo( void );

signals:
    void						signalHostJoinRequestListItemClicked( QListWidgetItem * poItemWidget );
	void						signalIconButtonClicked( HostJoinRequestListItem* listEntryWidget );
	void						signalMenuButtonClicked( HostJoinRequestListItem* listEntryWidget );
    void						signalJoinButtonClicked( HostJoinRequestListItem* listEntryWidget );
    void						signalConnectButtonClicked( HostJoinRequestListItem* listEntryWidget );

public slots:
	void						slotIconButtonClicked( void );
	void						slotMenuButtonPressed( void );
	void						slotMenuButtonReleased( void );
    void						slotJoinButtonPressed( void );
    void						slotConnectButtonPressed( void );
 
protected:
    virtual void				mousePressEvent( QMouseEvent * event ) override;
    virtual void				resizeEvent( QResizeEvent* resizeEvent ) override;
    virtual void                showConnectButton( bool isAccepted );

	//=== vars ===//
    Ui::HostJoinRequestListItemUi	        ui;
    AppCommon&					m_MyApp;
};




