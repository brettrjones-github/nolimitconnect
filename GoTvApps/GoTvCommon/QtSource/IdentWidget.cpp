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

#include <app_precompiled_hdr.h>
#include "IdentWidget.h"
#include "GuiUser.h"
#include "MyIcons.h"
#include "AppCommon.h"

//============================================================================
IdentWidget::IdentWidget(QWidget *parent)
: QPushButton(parent)
, m_MyApp( GetAppInstance() )
{
	setupUi(this);
	connect( m_FriendMenuButton, SIGNAL(clicked()), this, SIGNAL(signalFriendMenuClicked()) );
}

//============================================================================
MyIcons&  IdentWidget::getMyIcons( void )
{
	return m_MyApp.getMyIcons();
}

//============================================================================
void IdentWidget::updateGuiFromData( GuiUser * netIdent )
{
	setOnlineState( netIdent->getIsOnline() );
	this->FriendPrefixLabel->setText( netIdent->describeMyFriendshipToHim() );
	this->FriendNameLabel->setText( netIdent->getOnlineName() );
	this->DescTextLabel->setText( netIdent->getOnlineDescription() );
	this->m_FriendIconButton->setIcon( getMyIcons().getFriendshipIcon( netIdent->getMyFriendshipToHim() ) );
	this->m_TodLabel->setText( QString("Truths: %1 Dares: %2").arg(netIdent->getTruthCount()).arg(netIdent->getDareCount()) );
}

//============================================================================
void IdentWidget::setMenuButtonVisible( bool visible )
{
	if( 0 != m_FriendMenuButton )
	{
		this->m_FriendMenuButton->setVisible( visible );
	}
}


//============================================================================
void IdentWidget::setOnlineState( bool isOnline )
{
	if( isOnline )
	{
		this->setStyleSheet("border-image: 0; background-color: rgb(184, 231, 249); color: rgb(0, 0, 0); font: 14px;\n");
		m_FriendMenuButton->setIcon( eMyIconMenuNormal );
	}
	else
	{
		this->setStyleSheet("border-image: 0; background-color: rgb(192, 192, 192); color: rgb(0, 0, 0); font: 14px;\n");
		m_FriendMenuButton->setIcon( eMyIconMenuDisabled );
	}
}
