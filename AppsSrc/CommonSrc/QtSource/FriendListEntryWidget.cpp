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
// http://www.nolimitconnect.org
//============================================================================

#include "FriendListEntryWidget.h"
#include "AppCommon.h"
#include "AppletMgr.h"
#include "AppletPopupMenu.h"
#include "GuiUser.h"

//============================================================================
FriendListEntryWidget::FriendListEntryWidget( QListWidget * parent, int type  )
: IdentWidget( parent )
, QListWidgetItem( parent, type )
{
}

//============================================================================
void FriendListEntryWidget::setUser( GuiUser* user )
{
	QListWidgetItem::setData( Qt::UserRole + 2, QVariant( ( qulonglong )( user ) ) );
}

//============================================================================
GuiUser* FriendListEntryWidget::getUser( void )
{
	return (GuiUser*)QListWidgetItem::data( Qt::UserRole + 2 ).toULongLong();
}

//============================================================================
void FriendListEntryWidget::onIdentAvatarButtonClicked()
{
	emit listButtonClicked( this );
}

//============================================================================
void FriendListEntryWidget::onIdentOfferButtonClicked()
{
	emit listButtonClicked( this );
}

//============================================================================
void FriendListEntryWidget::onIdentMenuButtonClicked()
{
	GuiUser* selectedFriend = getUser();
	if( selectedFriend )
	{
		AppletPopupMenu* applet = dynamic_cast<AppletPopupMenu*>(GetAppInstance().getAppletMgr().launchApplet( eAppletPopupMenu, dynamic_cast<QWidget*>(parent()) ));
		if( applet )
		{
			applet->showPersonOfferMenu( selectedFriend );
		}
	}
}
