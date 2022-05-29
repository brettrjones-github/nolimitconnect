//============================================================================
// Copyright (C) 2019 Brett R. Jones
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


#include "VxPlainTextEdit.h"
#include "AppCommon.h"
#include "VxAppTheme.h"

//============================================================================
VxPlainTextEdit::VxPlainTextEdit( QWidget *parent )
: QPlainTextEdit( parent )
{
}

//============================================================================
VxPlainTextEdit::VxPlainTextEdit( const QString &text, QWidget *parent )
    : QPlainTextEdit( text, parent )
{
}

//============================================================================
void VxPlainTextEdit::contextMenuEvent( QContextMenuEvent *event )
{
    QMenu *menu = createStandardContextMenu();
    GetAppInstance().getAppTheme().applyTheme( menu );
    menu->addAction( tr( "My Menu Item" ) );
    menu->exec( event->globalPos() );
    delete menu;
}
