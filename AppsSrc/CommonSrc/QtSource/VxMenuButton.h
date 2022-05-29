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

#include "SoundDefs.h"
#include "MyIconsDefs.h"
#include "VxContextMenu.h"

#include "VxPushButton.h"

class VxContextMenu;
class AppCommon;
class MyIcons;
class QTimer;

class VxMenuButton : public VxPushButton
{
    Q_OBJECT
public:
    VxMenuButton( QWidget *parent = 0 );
    VxMenuButton( const QString & text, QWidget *parent = 0 );
    virtual ~VxMenuButton() {};

    // setMenuId  must be called before add menu item ( can be 0 )
    void                        setMenuId( int menuId );
    void                        addMenuItem( EMenuItemType eMenuItem, QString menuText = "" );
    void                        showMenu( const QPoint & globalPoint );

signals:
    void                        signalMenuItemSelected( int menuId, EMenuItemType eMenuItem );

protected slots:
    void                        slotMenuItemSelected( int menuId, EMenuItemType eMenuItem );
    void                        slotMenuButtonClicked( void );

protected:
    void                        setMenuLocation( bool showAboveButton );

    VxContextMenu               m_ContextMenu;
};

