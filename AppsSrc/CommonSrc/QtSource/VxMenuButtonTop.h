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

#include "VxMenuButton.h"

class VxMenuButtonTop : public VxMenuButton
{
    Q_OBJECT
public:
    VxMenuButtonTop( QWidget*parent = 0 );
    VxMenuButtonTop( const QString & text, QWidget*parent = 0 );
    virtual ~VxMenuButtonTop() {};

    // setMenuParams  must be called before add menu item
    void                        setMenuParams( const QPoint & globalPoint, int menuId = 0 );

protected:
};

