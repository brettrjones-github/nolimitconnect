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
#include "IdentLogicInterface.h"

#include <QListWidgetItem>
#include <QWidget>

class GuiHostJoinSession;

class HostJoinRequestListItem : public IdentLogicInterface, public QListWidgetItem
{
	Q_OBJECT;
public:
	HostJoinRequestListItem( QWidget *parent=nullptr );
	virtual ~HostJoinRequestListItem();

    AppCommon&					getMyApp( void ) { return m_MyApp; }
    MyIcons&					getMyIcons( void );

    void                        setHostSession( GuiHostJoinSession* hostSession );
    GuiHostJoinSession*         getHostSession( void );

    VxPushButton*               getIdentAvatarButton( void ) override        { return ui.m_AvatarButton; }
    VxPushButton*               getIdentFriendshipButton( void ) override    { return ui.m_FriendshipButton; }
    VxPushButton*               getIdentMenuButton( void )  override         { return ui.m_MenuButton; }
    VxPushButton*               getAcceptButton( void )                     { return ui.m_AcceptButton; }
    VxPushButton*               getRejectButton( void )                     { return ui.m_RejectButton; }

    virtual QLabel*             getIdentLine1( void ) override { return ui.m_TitlePart1; }
    virtual QLabel*             getIdentLine2( void ) override { return ui.m_DescPart1; }

    void						onIdentAvatarButtonClicked( void ) override;
    void						onIdentMenuButtonClicked( void ) override;

    void                        setJoinedState( EJoinState joinState );

    void						updateWidgetFromInfo( void );

signals:
    void						signalHostJoinRequestListItemClicked( QListWidgetItem * poItemWidget );
	void						signalAvatarButtonClicked( HostJoinRequestListItem* listEntryWidget );
    void						signalFriendshipButtonClicked( HostJoinRequestListItem* listEntryWidget );
	void						signalMenuButtonClicked( HostJoinRequestListItem* listEntryWidget );
    void						signalAcceptButtonClicked( HostJoinRequestListItem* listEntryWidget );
    void						signalRejectButtonClicked( HostJoinRequestListItem* listEntryWidget );

public slots:
    void						slotAcceptButtonPressed( void );
    void						slotRejectButtonPressed( void );
 
protected:
    virtual void				mousePressEvent( QMouseEvent * event ) override;
    virtual void				resizeEvent( QResizeEvent* resizeEvent ) override;
    virtual void                showAcceptButton( bool makeVisible );
    virtual void                showRejectButton( bool makeVisible );

	//=== vars ===//
    Ui::HostJoinRequestListItemUi	        ui;
    AppCommon&					m_MyApp;
};




