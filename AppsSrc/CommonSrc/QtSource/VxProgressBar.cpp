//============================================================================
// Copyright (C) 2020 Brett R. Jones
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


#include "VxProgressBar.h"

#include "AppCommon.h"
#include "GuiParams.h"

#include <CoreLib/VxDebug.h>

//============================================================================
VxProgressBar::VxProgressBar( QWidget *parent ) 
: QProgressBar( parent ) 
, m_MyApp( GetAppInstance() )
{
}

//============================================================================
void VxProgressBar::slotProgressValue( EAppModule appModule, int peekVal0to32768 )
{
    setValue( peekVal0to32768 );
}

//============================================================================
QSize VxProgressBar::sizeHint( void ) const
{
 //   if( orientation  )
 //   int barHeight = height();
	//return QSize( barHeight / 5, barHeight );
    return QProgressBar::sizeHint();
}

//============================================================================
void VxProgressBar::setFixedSize( const QSize & fixedSize )
{
    if( fixedSize.width() == fixedSize.height() )
    {
        QProgressBar::setFixedSize( ( int )( fixedSize.width() * GuiParams::getGuiScale() ),
            ( int )( fixedSize.height() * GuiParams::getGuiScale() ) );
    }
    else
    {
        this->setFixedWidth( fixedSize.width() );
        this->setFixedHeight( fixedSize.height() );
    }
}

//============================================================================
void VxProgressBar::setFixedSize( int width, int height )
{
    if( width == height )
    {
        QProgressBar::setFixedSize(  (int)( width * GuiParams::getGuiScale() ),
                                    (int)( height * GuiParams::getGuiScale() ) );
    }
    else
    {
        this->setFixedWidth( width );
        this->setFixedHeight( height );
    }
}

//============================================================================
void VxProgressBar::setFixedWidth( int width )
{
    QProgressBar::setFixedWidth( ( int )( width * GuiParams::getGuiScale() ) );
}

//============================================================================
void VxProgressBar::setFixedHeight( int height )
{
    QProgressBar::setFixedWidth( ( int )( height * GuiParams::getGuiScale() ) );
}

//============================================================================
void VxProgressBar::setFixedSizeAbsolute( const QSize & fixedSize )
{
    setFixedSizeAbsolute( fixedSize.width(), fixedSize.height() );
}

//============================================================================
void VxProgressBar::setFixedSizeAbsolute( int fixedWidth, int fixedHeight )
{
    QProgressBar::setFixedSize( fixedWidth, fixedHeight );
}

//============================================================================
void VxProgressBar::setMinimumSize( const QSize & minSize )
{
    if( minSize.height() == minSize.width() )
    {
        QProgressBar::setMinimumSize( ( int )( minSize.width() * GuiParams::getGuiScale() ),
            ( int )( minSize.height() * GuiParams::getGuiScale() ) );
    }
    else
    {
        QProgressBar::setMinimumSize( ( int )( minSize.width() * GuiParams::getGuiScale() ),
            ( int )( minSize.height() * GuiParams::getGuiScale() ) );
    }
}

//============================================================================
void VxProgressBar::setMinimumSize( int minw, int minh )
{
    if( minw == minh )
    {
        QProgressBar::setMaximumSize( ( int )( minw * GuiParams::getGuiScale() ),
            ( int )( minh * GuiParams::getGuiScale() ) );
    }
    else
    {
        QProgressBar::setMaximumSize( ( int )( minw * GuiParams::getGuiScale() ),
            ( int )( minh * GuiParams::getGuiScale() ) );
    }
}

//============================================================================
void VxProgressBar::setMaximumSize( const QSize & maxSize )
{
    if( maxSize.width() == maxSize.height() )
    {
        QProgressBar::setMaximumSize( ( int )( maxSize.width() * GuiParams::getGuiScale() ),
            ( int )( maxSize.height() * GuiParams::getGuiScale() ) );
    }
    else
    {
        QProgressBar::setMaximumSize( ( int )( maxSize.width()  * GuiParams::getGuiScale() ),
            ( int )( maxSize.height() * GuiParams::getGuiScale() ) );
    }
}

//============================================================================
void VxProgressBar::setMaximumSize( int maxw, int maxh )
{
    QProgressBar::setMaximumSize( ( int )( maxw * GuiParams::getGuiScale() ),
        ( int )( maxh * GuiParams::getGuiScale() ) );
}

