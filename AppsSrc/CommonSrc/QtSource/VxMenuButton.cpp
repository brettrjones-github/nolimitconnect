//============================================================================
// Copyright (C) 2017 Brett R. Jones
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


#include "VxMenuButton.h"
#include "AppCommon.h"

//============================================================================
VxMenuButton::VxMenuButton( QWidget * parent )
: VxPushButton( parent )
, m_ContextMenu( getMyApp(), this )
{
    setObjectName( "VxMenuButton" );
    connect( this, SIGNAL( clicked() ), this, SLOT( slotMenuButtonClicked() ) );
    connect( &m_ContextMenu, SIGNAL( signalMenuItemSelected( int, EMenuItemType ) ), this, SLOT( slotMenuItemSelected( int, EMenuItemType ) ) );
}

//============================================================================
VxMenuButton::VxMenuButton( const QString & text, QWidget * parent )
: VxPushButton( text, parent )
, m_ContextMenu( getMyApp(), this )
{
    connect( this, SIGNAL( clicked() ), this, SLOT( slotMenuButtonClicked() ) );
    connect( &m_ContextMenu, SIGNAL( signalMenuItemSelected(int, EMenuItemType) ), this, SLOT( slotMenuItemSelected(int, EMenuItemType) ) );
}

//============================================================================
void VxMenuButton::slotMenuButtonClicked( void )
{
    QPoint rightPoint = m_MyApp.getHomePage().mapToGlobal( this->geometry().topRight() );
    QPoint topPoint;

    if( m_ContextMenu.getIsShowMenuAbove() )
    {
        topPoint = mapToGlobal( this->geometry().topRight() );
    }
    else
    {
        topPoint = mapToGlobal( this->geometry().bottomRight() );
    }

    m_ContextMenu.showMenu( QPoint( rightPoint.x(), topPoint.y() ) );
}

//============================================================================
void VxMenuButton::setMenuId( int menuId )
{
    m_ContextMenu.setMenuId( menuId );
}

//============================================================================
void VxMenuButton::slotMenuItemSelected( int menuId, EMenuItemType eMenuItem )
{
    emit signalMenuItemSelected( menuId, eMenuItem );
}

//============================================================================
void VxMenuButton::showMenu( const QPoint & globalPoint )
{
    m_ContextMenu.showMenu( globalPoint );
}

//============================================================================
void VxMenuButton::setMenuLocation( bool showAboveButton )
{
    m_ContextMenu.setMenuLocation( showAboveButton );
}

//============================================================================
void VxMenuButton::addMenuItem( EMenuItemType eMenuItem, QString menuText )
{
    m_ContextMenu.addMenuItem( eMenuItem, menuText );
}
