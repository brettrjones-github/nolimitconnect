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

#include "GuiParams.h"
#include <QApplication>
#include <QDesktopWidget>
#include <QScreen>

#include <CoreLib/VxDebug.h>

//============================================================================
QColor GuiParams::m_OnlineBkgColor( 176, 255, 176 );
QColor GuiParams::m_OfflineBkgColor( 192, 192, 192 );
QColor GuiParams::m_NearbyBkgColor( 176, 176, 255 );
int GuiParams::m_DisplayScale{1};

//============================================================================
GuiParams::GuiParams()
{
    //initGuiParams();
}

//============================================================================
void GuiParams::initGuiParams()
{
    QScreen *screen = QGuiApplication::primaryScreen();
    if( screen )
    {
        float dpi = screen->physicalDotsPerInch();
        QRect  screenGeometry = screen->availableGeometry();
        float iconSizeInches = ( 48.0f / dpi );
        float maxPixels = screenGeometry.width() > screenGeometry.height() ? screenGeometry.width() : screenGeometry.height();
        float screenSizeInches = maxPixels / dpi;
        float normalPercentOfScreenForIcon = 48.0f/1280.0f;
        float curPercentOfScreenForIcon = 48.0f/maxPixels;

        if( iconSizeInches < 0.3 )
        {
            // 48 pixel icons are less than a quarter inch wide
            // scale up
            m_DisplayScale = 2;
        }
        else if( ( screenSizeInches > 17 ) && ( curPercentOfScreenForIcon < normalPercentOfScreenForIcon ) )
        {
            m_DisplayScale = 2;
        }

        LogMsg( LOG_VERBOSE, "Screen dpi %3.0f pixels %3.0f screen size %3.0f icon size 0x%3f scale %d",
                dpi, maxPixels, screenSizeInches, iconSizeInches, m_DisplayScale );
    }
}

//============================================================================
int GuiParams::getControlIndicatorWidth( void )
{ 
    return 10; 
}

//============================================================================
QSize GuiParams::getThumbnailSize( void )
{
    return QSize( 240, 240 ); 
}

//============================================================================
QSize GuiParams::getSnapshotSize( void )
{
    return QSize( 320 * m_DisplayScale, 240 * m_DisplayScale );
}