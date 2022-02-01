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

#include "ui_GuiHostedListItem.h"
#include "IdentLogicInterface.h"

#include <QListWidgetItem>

class GuiHostedListSession;

class GuiHostedListItem : public IdentLogicInterface, public QListWidgetItem
{
	Q_OBJECT;
public:
	GuiHostedListItem( QWidget *parent=0 );
	virtual ~GuiHostedListItem();

    AppCommon&					getMyApp( void ) { return m_MyApp; }
    MyIcons&					getMyIcons( void );

    void                        setHostSession( GuiHostedListSession* hostSession );
    GuiHostedListSession*       getHostSession( void );

    VxPushButton*               getIdentAvatarButton( void ) override         { return ui.m_IconButton; }
    VxPushButton*               getIdentFriendshipButton( void ) override { return ui.m_FriendshipButton; }
    VxPushButton*               getIdentMenuButton( void ) override { return ui.m_MenuButton; }

    virtual QLabel*             getIdentLine1( void ) override { return ui.m_TitlePart1; }
    virtual QLabel*             getIdentLine2( void ) override { return ui.m_DescPart1; }

    void						onIdentAvatarButtonClicked( void ) override;
    void						onIdentMenuButtonClicked( void ) override;

    void                        setJoinedState( EJoinState joinState );

    void						updateWidgetFromInfo( void );

    QSize                       calculateSizeHint( void );

    void                        updateUser( GuiUser* guiUser );

    void                        setIsThumbUpdated( bool updated )   { m_IsThumbUpdated = updated; }
    bool                        getIsThumbUpdated( void )           { return m_IsThumbUpdated; }

signals:
    void						signalGuiHostedListItemClicked( QListWidgetItem * poItemWidget );
	void						signalIconButtonClicked( GuiHostedListItem* listEntryWidget );
    void						signalFriendshipButtonClicked( GuiHostedListItem* listEntryWidget );
	void						signalMenuButtonClicked( GuiHostedListItem* listEntryWidget );
    void						signalJoinButtonClicked( GuiHostedListItem* listEntryWidget );
    void						signalConnectButtonClicked( GuiHostedListItem* listEntryWidget );
    void						signalKickButtonClicked( GuiHostedListItem* listEntryWidget );

public slots:
    void						slotJoinButtonClicked( void );
    void						slotConnectButtonClicked( void );
    void						slotKickButtonClicked( void );
 
protected:
    virtual void				mousePressEvent( QMouseEvent * event ) override;
    virtual void				resizeEvent( QResizeEvent* resizeEvent ) override;

    void                        showConnectButton( bool isAccepted );
    void                        showKickButton( bool isVisible );

	//=== vars ===//
    Ui::GuiHostedListItemUi	    ui;
    AppCommon&					m_MyApp;
    bool                        m_IsThumbUpdated{ false };
};




