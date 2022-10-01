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


#include "VxFrame.h"

#include <CoreLib/VxDebug.h>

//============================================================================
VxFrame::VxFrame( AppCommon& appCommon, QWidget* parent, Qt::WindowFlags f )
	: QFrame( parent, f )
    , m_MyApp( appCommon )
	, m_FrameId( 1 )
{
    setObjectName( "VxFrame" );
}

//============================================================================
void VxFrame::resizeEvent( QResizeEvent* ev )
{
	QFrame::resizeEvent( ev );
	emit signalFrameResized();
}

//============================================================================
void VxFrame::slotAppSystemMenuSelected( int menuId, QWidget* popupMenu )
{
    LogMsg( LOG_DEBUG, "slotAppSystemMenuSelected menu id %d", menuId );
    switch( menuId )
    {
    case 0: // debug settings
        LogMsg( LOG_DEBUG, "slotAppSystemMenuSelected debug settings menu id %d", menuId );
        break;

    case 1: // debug log
        LogMsg( LOG_DEBUG, "slotAppSystemMenuSelected debug log menu id %d", menuId );
        break;

    default:
        break;
    }
}
