//============================================================================
// Copyright (C) 2010 Brett R. Jones
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


#include "MessengerPage.h"
#include "AppCommon.h"
#include "AppSettings.h"
#include "MyIcons.h"

#include <CoreLib/VxDebug.h>

//============================================================================
MessengerPage::MessengerPage( AppCommon& app, QWidget * parent )
: ActivityBase( OBJNAME_APPLET_MESSAGER_PAGE, app, parent )
, m_IsVisible( false )
, m_IsFullSize( false )
{
	setTitleBarText( QObject::tr( "Messenger" ) );
	m_IsFullSize = getMyApp().getAppSettings().getIsMessengerFullScreen();
	setExpandWindowVisibility( true );
	setMenuBottomVisibility( true );
}

//============================================================================
void MessengerPage::showEvent( QShowEvent * )
{
	if( false == m_IsVisible )
	{
		m_IsVisible = true;
		emit signalMessengerPageChanged( m_IsVisible, m_IsFullSize );
	}
}

//============================================================================
void MessengerPage::hideEvent( QHideEvent * )
{
	if( true == m_IsVisible )
	{
		m_IsVisible = false;
		emit signalMessengerPageChanged( m_IsVisible, m_IsFullSize );
	}
}

//============================================================================
void MessengerPage::slotBackButtonClicked( void )
{
	m_IsVisible = false;
	emit signalMessengerPageChanged( m_IsVisible, m_IsFullSize );
}

