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
// http://www.nolimitconnect.com
//============================================================================

#include <QMenu>
#include <QResizeEvent>

class VxMenu : public QMenu
{
    Q_OBJECT
public:
    VxMenu( QWidget *parent = 0 );

    QAction *                   exec( const QPoint &pos, QAction *at = Q_NULLPTR );
    void                        setShowAbove( bool showAbove );

protected:
    virtual void                resizeEvent( QResizeEvent * ev ) override;

    bool                        m_ShowAbove{ false };
    QPoint                      m_MenuPos;
    QSize                       m_OrigSize;
};
