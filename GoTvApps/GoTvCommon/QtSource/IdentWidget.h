#pragma once
//============================================================================
// Copyright (C) 2013 Brett R. Jones
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
#include "ui_IdentWidget.h"
#include <QPushButton>

class VxNetIdent;
class AppCommon;
class MyIcons;
class VxGUID;

class IdentWidget : public QPushButton, public Ui::IdentWidget
{
	Q_OBJECT

public:
	IdentWidget(QWidget *parent=0);

	AppCommon&					getMyApp( void ) { return m_MyApp; }
	MyIcons&					getMyIcons( void );

	void						setIdentWidgetSize( EButtonSize buttonSize );

	void						setOnlineState( bool isOnline );
	void						setAvatarButtonVisible( bool visible );
	void						setFriendshipButtonVisible( bool visible );
	void						setOfferButtonVisible( bool visible );
	void						setMenuButtonVisible( bool visible );

	void						setAvatarThumbnail( VxGUID& thumbId );
	void						setAvatarIcon( EMyIcons myIcon );
	void						setFriendshipIcon( EMyIcons myIcon );
	void						setOfferIcon( EMyIcons myIcon );
	void						setMenuIcon( EMyIcons myIcon );

	//! fill gui widgets from friend data
	void						updateGuiFromData( GuiUser * poFriend );

signals:
	void						signalAvatarButtonClicked( void );
	void						signalOfferButtonClicked( void );
	void						signalFriendMenuButtonClicked( void );

protected slots:
	void						slotFrienshipButtonClicked( void );

protected:
	Ui::IdentWidget				ui;
	AppCommon&					m_MyApp;
	GuiUser*					m_NetIdent{ nullptr };
};
