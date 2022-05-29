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

#include "VxAspectFrame.h"
#include "VxLabel.h"

//============================================================================
VxAspectFrame::VxAspectFrame( QWidget *parent )
: QFrame( parent )
, m_PreviewAspectRatio( 1.333333333333 )
, m_ScreenAspectRatio( 1.333333333333 )
, m_ThumbnailPreview( 0 )
, m_ThumbnailScale( 0.25 )
, m_VideoScreen( 0 )
{
    setObjectName( "VxAspectFrame" );
}

//============================================================================
void VxAspectFrame::setPreviewAspectRatio( float aspectRatio )
{
    if( aspectRatio != m_PreviewAspectRatio )
    {
        m_PreviewAspectRatio = aspectRatio;
        updatePositions();
    } 
}

//============================================================================
void VxAspectFrame::setVideoScreenAspectRatio( float aspectRatio )
{
    if( aspectRatio != m_ScreenAspectRatio )
    {
        m_ScreenAspectRatio = aspectRatio;
        updatePositions();
    }
}

//============================================================================
void VxAspectFrame::resizeEvent( QResizeEvent * ev )
{
    QFrame::resizeEvent( ev );
    updatePositions();
}

//============================================================================
void VxAspectFrame::updatePositions( void )
{
    if( 0 == m_VideoScreen )
    {
        return;
    }

    positionWidget( this, m_ScreenAspectRatio, m_VideoScreen, true, 1.0f );
    positionWidget( m_VideoScreen, m_PreviewAspectRatio, m_ThumbnailPreview, false, m_ThumbnailScale );
}

//============================================================================
void VxAspectFrame::positionWidget( QWidget * parentWidget, float aspectRatio, VxLabel * widget, bool centerWidget, float scale )
{
    if( ( 0 == parentWidget ) || ( 0 == widget ) )
    {
        return;
    }

    QRect  parentRect = parentWidget->geometry();
    int parentWidth     = parentRect.width();
    int parentHeight    = parentRect.height();
    if( ( 10 > parentWidth )
        || ( 10 > parentHeight ) )
    {
        return;
    }

    parentWidth -= 2;
    parentHeight -= 2;
    // try maximum width first
    int widgetWidth = parentWidth;
    int widgetHeight = ( int ) ( parentWidth * ( 1 / aspectRatio ) );
    if( widgetHeight > parentHeight )
    {
        // screen is to big to fit.. have to scale down
        float screenScale = (float)parentHeight / widgetHeight;
        widgetWidth  = ( int ) ( screenScale * ( float ) widgetWidth );
        widgetHeight = ( int ) ( screenScale * ( float ) widgetHeight );
    }

    if( 1.0f != scale )
    {
        widgetWidth = ( int ) ( scale * widgetWidth );
        widgetHeight = ( int )( scale * widgetHeight );
    }
  
    widget->setFixedSize( widgetWidth,  widgetHeight );
    int xPos = 1;
    int yPos = 1;
    if( centerWidget )
    {
        xPos = ( parentWidth - widgetWidth ) / 2;
        yPos = ( parentHeight - widgetHeight ) / 2;
    }

    widget->move( xPos, yPos );
}

