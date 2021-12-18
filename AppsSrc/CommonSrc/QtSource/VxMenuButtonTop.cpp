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


#include "VxMenuButtonTop.h"

//============================================================================
VxMenuButtonTop::VxMenuButtonTop( QWidget * parent )
: VxMenuButton( parent )
{
    setObjectName( "VxMenuButtonTop" );
    setMenuLocation( false );
}

//============================================================================
VxMenuButtonTop::VxMenuButtonTop( const QString & text, QWidget * parent )
: VxMenuButton( text, parent )
{
    setMenuLocation( false );
}

