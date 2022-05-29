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


#include "VxMenu.h"
#include <QLabel>
#include <QWidgetAction>

//============================================================================
VxMenu::VxMenu( QWidget *parent )
: QMenu( parent )
, m_ShowAbove( false )
{
    QString menuStyle(
        "QMenu::item{"
        "color: rgb(0, 0, 255);"
        "}"
    );

    this->setStyleSheet( menuStyle );
}

//============================================================================
QAction * VxMenu::exec( const QPoint &pos, QAction *at )
{
    QLabel* label = new QLabel( tr( "<b>Title</b>" ), this );
    label->setAlignment( Qt::AlignCenter );

    QWidgetAction* a = new QWidgetAction( this );
    a->setDefaultWidget( label );

    m_MenuPos = pos;
    m_OrigSize = geometry().size();
    return QMenu::exec( pos, at );
}

//============================================================================
void VxMenu::setShowAbove( bool showAbove )
{
    m_ShowAbove = showAbove;
}

//============================================================================
void VxMenu::resizeEvent( QResizeEvent * ev )
{
    QSize newSize = ev->size();
    QMenu::resizeEvent( ev );
    if( m_ShowAbove )
    {
        int oldHeight = m_OrigSize.height();
        int newHeight = newSize.height();
        QPoint newPos = m_MenuPos;
        if( oldHeight != newHeight )
        {
            int oldWidth = m_OrigSize.width();
            int newWidth = newSize.width();
            newPos.setY( m_MenuPos.y() - ( newHeight - oldHeight ) );
            newPos.setX( m_MenuPos.x() - ( newWidth - oldWidth ) );
            this->move( newPos );
        }

        m_MenuPos = newPos;
    }


    m_OrigSize = newSize;
}
