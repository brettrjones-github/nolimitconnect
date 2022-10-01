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

#include "ui_PersonOfferListItem.h"
#include "IdentLogicInterface.h"

#include <QListWidgetItem>

class GuiOfferSession;

class PersonOfferListItem : public IdentLogicInterface, public QListWidgetItem
{
	Q_OBJECT;
public:
	PersonOfferListItem( QWidget*parent=0 );
	virtual ~PersonOfferListItem();

    AppCommon&					getMyApp( void ) { return m_MyApp; }
    MyIcons&					getMyIcons( void );

    void                        setOfferSession( GuiOfferSession* hostSession );
    GuiOfferSession*            getOfferSession( void );

    void						updateWidgetFromInfo( void );
    QSize                       calculateSizeHint( void );

    VxPushButton*               getIdentAvatarButton( void ) override       { return ui.m_AvatarButton; }
    VxPushButton*               getIdentFriendshipButton( void ) override   { return ui.m_FriendshipButton; }
    VxPushButton*               getIdentMenuButton( void )  override        { return ui.m_MenuButton; }
    VxPushButton*               getAcceptButton( void )                     { return ui.m_AcceptButton; }
    VxPushButton*               getRejectButton( void )                     { return ui.m_RejectButton; }

    virtual QLabel*             getIdentLine1( void ) override              { return ui.m_TitlePart1; }
    virtual QLabel*             getIdentLine2( void ) override              { return ui.m_Line2Label; }

    void						onIdentAvatarButtonClicked( void ) override;
    void						onIdentMenuButtonClicked( void ) override;

signals:
    void						signalPersonOfferListItemClicked( PersonOfferListItem * poItemWidget );
	void						signalAvatarButtonClicked( PersonOfferListItem* listEntryWidget );
	void						signalMenuButtonClicked( PersonOfferListItem* listEntryWidget );

protected:
    virtual void				mousePressEvent( QMouseEvent * event ) override;
    virtual void				resizeEvent( QResizeEvent* resizeEvent ) override;

	//=== vars ===//
    Ui::PersonOfferListItemUi	ui;
    AppCommon&					m_MyApp;
};




