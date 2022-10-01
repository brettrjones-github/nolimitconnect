#pragma once
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

#include "MenuDefs.h"

#include <QMenu>
#include <QPair>

class AppCommon;
class VxMenu;

class VxContextMenu : public QWidget
{
    Q_OBJECT

public:
    VxContextMenu( AppCommon& appCommon, QWidget* parent = 0 );
    virtual ~VxContextMenu() {};

    void                        setMenuLocation( bool showAboveButton );
    bool                        getIsShowMenuAbove( void )      { return m_ShowAboveButton; }
    // setMenuParams  must be called before add menu item ( can be 0 )
    void                        setMenuId( int menuId = 0 );
    void                        addMenuItem( EMenuItemType eMenuItem, QString menuText = "" );
    void                        showMenu( const QPoint & globalPoint );
    static QString              describeMenuItem( EMenuItemType eMenuItem );

signals:
    void                        signalMenuItemSelected( int menuId, EMenuItemType eMenuItem );

protected slots:
    void                        slotMenuItemSelected( int menuId, EMenuItemType eMenuItem );
    void                        slotMenuItemTriggered();

protected:
    void                        addMenuAction( EMenuItemType eMenuItem, QString& menuText );
    QPoint                      calculatePos( QWidget* menuWidget );


    AppCommon &					m_MyApp;
    int                         m_MenuId;
    QPoint                      m_GlobalPoint;
    bool                        m_ShowAboveButton; // bottom point or top point if false
    QVector< QPair< EMenuItemType, QString > >      m_MenuItemList;
    VxMenu *                     m_NewMenu;
};

