//============================================================================
// Copyright (C) 2009 Brett R. Jones 
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
#include "FriendListEntryWidget.h"

//============================================================================
FriendListEntryWidget::FriendListEntryWidget( QListWidget * parent, int type  )
: QListWidgetItem( parent, type )
{
	SubWidget = new QPushButton( parent );
	ui.setupUi( SubWidget );
	m_MenuButton = ui.m_MenuButton;
	connect( ui.m_IconButton, SIGNAL(pressed()), this, SLOT(iconButtonPressed()) );
	connect( ui.m_IconButton, SIGNAL(released()), this, SLOT(iconButtonReleased()) );
	connect( m_MenuButton, SIGNAL(pressed()), this, SLOT(slotMenuButtonPressed()) );
	connect( m_MenuButton, SIGNAL(released()), this, SLOT(slotMenuButtonReleased()) );
	connect( SubWidget, SIGNAL(pressed()), this, SLOT(listButtonPressed()) );
	connect( SubWidget, SIGNAL(released()), this, SLOT(listButtonReleased()) );
}

//============================================================================
void FriendListEntryWidget::iconButtonPressed()
{
	SubWidget->setDown(true);
	emit listButtonClicked( this );
}

//============================================================================
void FriendListEntryWidget::iconButtonReleased()
{
	SubWidget->setDown(false);
}

//============================================================================
void FriendListEntryWidget::slotMenuButtonPressed()
{
	SubWidget->setDown(true);
	emit signalMenuButtonClicked( this );
}

//============================================================================
void FriendListEntryWidget::slotMenuButtonReleased()
{
	SubWidget->setDown(false);
}

//============================================================================
void FriendListEntryWidget::listButtonPressed()
{
	emit listButtonClicked( this );
}

//============================================================================
void FriendListEntryWidget::listButtonReleased()
{

}
