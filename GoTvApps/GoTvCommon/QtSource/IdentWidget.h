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

class IdentWidget : public QPushButton, public Ui::IdentWidget
{
	Q_OBJECT

public:
	IdentWidget(QWidget *parent=0);

	AppCommon&					getMyApp( void ) { return m_MyApp; }
	MyIcons&					getMyIcons( void );

	void						setOnlineState( bool isOnline );
	void						setMenuButtonVisible( bool visible );
	//! fill gui widgets from friend data
	void						updateGuiFromData( GuiUser * poFriend );

signals:
	void						signalFriendMenuClicked( void );

protected:
	Ui::IdentWidget				ui;
	AppCommon&					m_MyApp;
};
