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
#include <QWidget>

class VxNetIdent;
class AppCommon;
class MyIcons;
class VxGUID;

class IdentWidget : public QWidget
{
	Q_OBJECT

public:
	IdentWidget(QWidget *parent=0);

	AppCommon&					getMyApp( void ) { return m_MyApp; }
	MyIcons&					getMyIcons( void );

	void						setDisableFriendshipChange( bool disable )		{ m_DisableFriendshipChange = disable;  }
	void						setIdentWidgetSize( EButtonSize buttonSize );

	void						setIdentOnlineState( bool isOnline );
	void						setIdentGroupState( bool isInGroup );
	void						setIdentDirectConnectState( bool canDirectConnect );

	VxPushButton*				getIdentAvatarButton( void );
	VxPushButton*				getIdentFriendshipButton( void );
	VxPushButton*				getIdentOfferButton( void );
	VxPushButton*				getIdentMenuButton( void );

	void						setIdentAvatarButtonVisible( bool visible );
	void						setIdentFriendshipButtonVisible( bool visible );
	void						setIdentOfferButtonVisible( bool visible );
	void						setIdentMenuButtonVisible( bool visible );

	void						setIdentAvatarThumbnail( VxGUID& thumbId );
	void						setIdentAvatarIcon( EMyIcons myIcon );
	void						setIdentFriendshipIcon( EMyIcons myIcon );
	void						setIdentOfferIcon( EMyIcons myIcon );
	void						setIdentMenuIcon( EMyIcons myIcon );

	//! fill gui widgets from friend data
	void						updateIdentity( GuiUser * guiUser );
	void						updateIdentity( VxNetIdent * netIdent );

	virtual void				onIdentWidgetSetup( void ){};

	virtual void				onIdentAvatarButtonClicked( void );
	virtual void				onIdentFriendshipButtonClicked( void );
	virtual void				onIdentOfferButtonClicked( void );
	virtual void				onIdentMenuButtonClicked( void );


signals:
	void						signalIdentAvatarButtonClicked( void );
	void						signalIdentOfferButtonClicked( void );
	void						signalIdentMenuButtonClicked( void );

protected slots:
	void						slotIdentAvatarButtonClicked( void );
	void						slotIdentFrienshipButtonClicked( void );
	void						slotIdentOfferButtonClicked( void );
	void						slotIdentMenuButtonClicked( void );

protected:
	Ui::IdentWidget				ui;
	AppCommon&					m_MyApp;
	GuiUser*					m_NetIdent{ nullptr };
	bool						m_DisableFriendshipChange{ false };
};
