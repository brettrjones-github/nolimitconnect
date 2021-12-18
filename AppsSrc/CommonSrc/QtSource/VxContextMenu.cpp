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
// http://www.nolimitconnect.com
//============================================================================


#include "VxContextMenu.h"
#include "AppCommon.h"
#include "VxAppTheme.h"
#include "VxMenu.h"
#include "GuiParams.h"

#include <QWidgetAction>
#include <CoreLib/VxDebug.h>

//============================================================================
VxContextMenu::VxContextMenu( AppCommon& appCommon, QWidget * parent )
: QWidget( parent )
, m_MyApp( appCommon )
, m_MenuId( 0 )
, m_GlobalPoint( 0, 0 )
, m_ShowAboveButton( false )
{
    setObjectName( "VxContextMenu" );
}

//============================================================================
void VxContextMenu::setMenuLocation( bool showAboveButton )
{
    m_ShowAboveButton = showAboveButton;
}

//============================================================================
void VxContextMenu::setMenuId( int menuId )
{
    m_MenuItemList.clear();
    if( menuId && !m_MenuId )
    {
        m_MenuId = menuId;
    }
}

//============================================================================
void VxContextMenu::addMenuItem( EMenuItemType eMenuItem, QString menuText )
{
    if( !menuText.isEmpty() )
    {
        addMenuAction( eMenuItem, menuText );
    }
    else
    {
        QString itemText = describeMenuItem( eMenuItem );
        addMenuAction( eMenuItem, itemText );
    }
}

//============================================================================
void VxContextMenu::addMenuAction( EMenuItemType eMenuItem, QString & menuText )
{
    QPair< EMenuItemType, QString > menuPair( eMenuItem, menuText );
    m_MenuItemList.push_back( menuPair );
    //addAction( menuText, this );
}

//============================================================================
void VxContextMenu::showMenu( const QPoint & globalPoint )
{
    m_GlobalPoint = globalPoint;
    if( !m_MenuItemList.isEmpty() )
    {
        m_NewMenu = new VxMenu( (QWidget *)parent() );
        m_NewMenu->setShowAbove( m_ShowAboveButton );
        m_NewMenu->setPalette( m_MyApp.getAppTheme().getBasePalette() );
        //QColor textColor( 255,0,255 );

        QVector< QPair< EMenuItemType, QString > >::iterator iter;
        for( iter = m_MenuItemList.begin(); iter != m_MenuItemList.end(); ++iter )
        {
            QString menuItemText =  ( *iter ).second;
            //QAction  * menuAction = new QAction( menuItemText, parent() );
            QWidgetAction *menuAction = new QWidgetAction( parent() );
            QLabel *label = new QLabel( menuItemText);
            label->setPalette( m_MyApp.getAppTheme().getBasePalette() );
            //label->setStyleSheet( "color: red" );
            menuAction->setDefaultWidget( label );
            menuAction->setShortcuts( QKeySequence::New );
            menuAction->setStatusTip( menuItemText);
            connect( menuAction, SIGNAL( triggered() ), this, SLOT( slotMenuItemTriggered() ) );

            m_NewMenu->addAction( menuAction);
            //menuAction->setEnabled( true );
            //menuAction->setText( menuItemText );
       }


        m_NewMenu->move( calculatePos( m_NewMenu )  );
        QWidgetAction * selectedItem = ( QWidgetAction * ) m_NewMenu->exec( calculatePos( m_NewMenu ) );
        if( selectedItem )
        {
            QLabel * menuLabel = ( QLabel * ) selectedItem->defaultWidget();
            if( menuLabel )
            {
                // something was chosen, do stuff
                QString menuText = menuLabel->text();
                LogMsg( LOG_DEBUG, "VxContextMenu::showMenu selection %s\n", menuText.toUtf8().constData() );
                if( !menuText.isEmpty() )
                {
                    QVector< QPair< EMenuItemType, QString > >::iterator iter;
                    for( iter = m_MenuItemList.begin(); iter != m_MenuItemList.end(); ++iter )
                    {
                        if( ( *iter ).second == menuText )
                        {
                            emit signalMenuItemSelected( m_MenuId, ( *iter ).first );
                        }
                    }
                }
                else
                {
                    LogMsg( LOG_DEBUG, "VxContextMenu::showMenu selection is empty\n" );
                }
            }
        }

        delete m_NewMenu;
    }
    else
    {
        // nothing was chosen
        LogMsg( LOG_DEBUG, "VxContextMenu::showMenu no selection\n" );
    }
}

//============================================================================
void VxContextMenu::slotMenuItemTriggered()
{

}

//============================================================================
QPoint VxContextMenu::calculatePos( QWidget * menuWidget )
{
    QRect widgetRect = menuWidget->geometry();
    int widgetWidth = widgetRect.width();
    int widgetHeight = widgetRect.height();

    if( 0 != m_GlobalPoint.x() )
    {
        if( m_ShowAboveButton )
        {
            return QPoint( m_GlobalPoint.x() - widgetWidth,  m_GlobalPoint.y() - widgetHeight );
        }
        else
        {
            return QPoint( m_GlobalPoint.x() - widgetWidth, ( int )( m_GlobalPoint.y() + 10.0f * GuiParams::getGuiScale() ) );
        }
    }
    else
    {
        if( m_ShowAboveButton )
        {
            return this->geometry().topLeft();
        }
        else
        {
            return this->geometry().bottomLeft();
        }
    }
}

//============================================================================
QString VxContextMenu::describeMenuItem( EMenuItemType eMenuItem )
{
    switch( eMenuItem )
    {
    case eMenuItemShowRecent:
        return QObject::tr( "Recent" );
    case eMenuItemShowWatched:
        return QObject::tr( "Watched" );
    case eMenuItemShowLibrary:
        return QObject::tr( "Library" );
    case eMenuItemBrowse:
        return QObject::tr( "Browse" );

    default:
        return QObject::tr( "Cancel" );
    }
}

//============================================================================
void VxContextMenu::slotMenuItemSelected( int menuId, EMenuItemType eMenuItem )
{

}

