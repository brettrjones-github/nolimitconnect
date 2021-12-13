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

#include "IdentWidget.h"
#include "AppletPeerChangeFriendship.h"
#include "GuiHelpers.h"
#include "GuiUser.h"
#include "MyIcons.h"
#include "AppCommon.h"

//============================================================================
IdentWidget::IdentWidget(QWidget *parent)
: IdentLogicInterface(parent)
{
	ui.setupUi(this);
	setupIdentLogic( eButtonSizeMedium );
}

//============================================================================
VxPushButton* IdentWidget::getIdentAvatarButton( void ) { return ui.m_AvatarButton; }
//============================================================================
VxPushButton* IdentWidget::getIdentFriendshipButton( void ) { return ui.m_FriendshipButton; }
//============================================================================
VxPushButton* IdentWidget::getIdentOfferButton( void ) { return ui.m_OfferButton; }
//============================================================================
VxPushButton* IdentWidget::getIdentPushToTalkButton( void ) { return ui.m_PushToTalkButton; }
//============================================================================
VxPushButton* IdentWidget::getIdentMenuButton( void ) { return ui.m_FriendMenuButton; }

//============================================================================
QLabel* IdentWidget::getIdentLine1( void ) { return ui.m_FriendNameLabel; }
//============================================================================
QLabel* IdentWidget::getIdentLine2( void ) { return ui.m_DescTextLabel; }
//============================================================================
QLabel* IdentWidget::getIdentLine3( void ) { return ui.m_TodLabel; }
