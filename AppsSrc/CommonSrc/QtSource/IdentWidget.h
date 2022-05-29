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
// http://www.nolimitconnect.org
//============================================================================

#include "ui_IdentWidget.h"
#include "IdentLogicInterface.h"

class IdentWidget :  public IdentLogicInterface
{
	Q_OBJECT

public:
	IdentWidget(QWidget *parent=0);

	VxPushButton*				getIdentAvatarButton( void ) override;
	VxPushButton*				getIdentFriendshipButton( void ) override;
	VxPushButton*				getIdentOfferButton( void ) override;
	VxPushButton*				getIdentPushToTalkButton( void ) override;
	VxPushButton*				getIdentMenuButton( void ) override;

	virtual QLabel*				getIdentLine1( void ) override;
	virtual QLabel*				getIdentLine2( void ) override;
	virtual QLabel*				getIdentLine3( void ) override;

protected:
	Ui::IdentWidget				ui;
};
