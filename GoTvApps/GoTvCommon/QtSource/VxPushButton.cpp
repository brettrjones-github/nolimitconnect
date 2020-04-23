//============================================================================
// Copyright (C) 2015 Brett R. Jones
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

#include "VxPushButton.h"
#include "AppCommon.h"
#include "VxAppTheme.h"
#include "MyIcons.h"
#include "GuiParams.h"

#include <CoreLib/VxTime.h>
#include <CoreLib/VxDebug.h>

#include <QTimer>
#include <QDebug>
#include <QMouseEvent>
#include <QPainter>

namespace
{
    const int APP_CLICK_MAX_MS_BETWEEN_SHORT_CLICKS = 600;
    const int APP_CLICK_MIN_MS_BETWEEN_SEQUENCE_CLICKS = 1000;
    const int APP_CLICK_MAX_MS_BETWEEN_SEQUENCE_CLICKS = 4000;
}

//============================================================================
VxPushButton::VxPushButton( QWidget *parent ) 
: QPushButton( parent ) 
, m_MyApp( GetAppInstance() )
, m_DefaultIconColor( COLOR_UNKNOWN )

, m_MyIcon( eMyIconNone )
, m_MyIconLast( eMyIconNone )
, m_IconImage()
, m_LastIconColor( COLOR_TRANSPARENT)
, m_LastIconSize( 0, 0 )

, m_NotifyIcon( eMyIconNone )
, m_LastNotifyIcon( eMyIconNone )
, m_NotifyIconImage()
, m_NotifyLastIconColor( COLOR_TRANSPARENT)
, m_NotifyLastIconSize( 0, 0 )

, m_OverlayIcon( eMyIconNone )
, m_LastOverlayIcon( eMyIconNone )
, m_OverlayIconImage()
, m_OverlayLastIconColor( COLOR_TRANSPARENT)
, m_OverlayLastIconSize( 0, 0 )

, m_ESndDefPressed( eSndDefButtonClick )
, m_MousePressedPoint( 0, 0 )
, m_MouseCurPoint( 0, 0 )
, m_BlinkTimer( new QTimer( this ) )
, m_BlinkState( 0 )
{
	initQButtonPro();
}

//============================================================================
VxPushButton::VxPushButton( const QString &text, QWidget *parent ) 
: QPushButton( text, parent ) 
, m_MyApp( GetAppInstance() )
, m_DefaultIconColor( COLOR_UNKNOWN )

, m_MyIcon( eMyIconNone )
, m_IconImage()
, m_LastIconColor( COLOR_TRANSPARENT )
, m_LastIconSize( 0, 0 )

, m_NotifyIcon( eMyIconNone )
, m_LastNotifyIcon( eMyIconNone )
, m_NotifyIconImage()
, m_NotifyLastIconColor( COLOR_TRANSPARENT )
, m_NotifyLastIconSize( 0, 0 )

, m_OverlayIcon( eMyIconNone )
, m_LastOverlayIcon( eMyIconNone )
, m_OverlayIconImage()
, m_OverlayLastIconColor( COLOR_TRANSPARENT )
, m_OverlayLastIconSize( 0, 0 )

, m_ESndDefPressed( eSndDefButtonClick )
, m_MousePressedPoint( 0, 0 )
, m_MouseCurPoint( 0, 0 )
, m_BlinkTimer( new QTimer( this ) )
, m_BlinkState( 0 )
{
	initQButtonPro();
}

//============================================================================
MyIcons&  VxPushButton::getMyIcons( void )
{
	return m_MyApp.getMyIcons();
}

//============================================================================
void VxPushButton::initQButtonPro( void )
{
    setObjectName( "VxPushButton" );

	m_BlinkTimer->setInterval( 800 );
	connect( m_BlinkTimer, SIGNAL(timeout()), this, SLOT(slotBlinkTimeout()) );
}

//============================================================================
int VxPushButton::heightForWidth( int width ) const
{
    if( width >= (int)( GuiParams::getButtonSize() ) )
	{
		return width;
	}

    return ( int )( GuiParams::getButtonSize() );
}

//============================================================================
QSize VxPushButton::sizeHint( void ) const
{
    int buttonSize = (int)( GuiParams::getButtonSize() );
	return QSize( buttonSize, buttonSize );
}

//============================================================================
void VxPushButton::setFixedSize( const QSize & fixedSize )
{
    if( fixedSize.width() == fixedSize.height() )
    {
        QPushButton::setFixedSize( ( int )( fixedSize.width() * GuiParams::getGuiScale() ),
            ( int )( fixedSize.height() * GuiParams::getGuiScale() ) );
    }
    else
    {
        this->setFixedWidth( fixedSize.width() );
        this->setFixedHeight( fixedSize.height() );
    }
}

//============================================================================
void VxPushButton::setFixedSize( int width, int height )
{
    if( width == height )
    {
        QPushButton::setFixedSize(  (int)( width * GuiParams::getGuiScale() ),
                                    (int)( height * GuiParams::getGuiScale() ) );
    }
    else
    {
        this->setFixedWidth( width );
        this->setFixedHeight( height );
    }
}

//============================================================================
void VxPushButton::setFixedWidth( int width )
{
    QPushButton::setFixedWidth( ( int )( width * GuiParams::getGuiScale() ) );
}

//============================================================================
void VxPushButton::setFixedSizeAbsolute( const QSize & fixedSize )
{
    setFixedSizeAbsolute( fixedSize.width(), fixedSize.height() );
}

//============================================================================
void VxPushButton::setFixedSizeAbsolute( int fixedWidth, int fixedHeight )
{
    QPushButton::setFixedSize( fixedWidth, fixedHeight );
}

//============================================================================
void VxPushButton::setFixedHeight( int height )
{
    QPushButton::setFixedWidth( ( int )( height * GuiParams::getGuiScale() ) );
}

//============================================================================
void VxPushButton::setMinimumSize( const QSize & minSize )
{
    if( minSize.height() == minSize.width() )
    {
        QPushButton::setMinimumSize( ( int )( minSize.width() * GuiParams::getGuiScale() ),
                        ( int )( minSize.height() * GuiParams::getGuiScale() ) );
    }
    else
    {
       QPushButton:: setMinimumSize( ( int )( minSize.width() * GuiParams::getGuiScale() ),
                        ( int )( minSize.height() * GuiParams::getGuiScale() ) );
    }
}

//============================================================================
void VxPushButton::setMinimumSize( int minw, int minh )
{
    if( minw == minh )
    {
        QPushButton::setMaximumSize( ( int )( minw * GuiParams::getGuiScale() ),
            ( int )( minh * GuiParams::getGuiScale() ) );
    }
    else
    {
        QPushButton::setMaximumSize( ( int )( minw * GuiParams::getGuiScale() ),
            ( int )( minh * GuiParams::getGuiScale() ) );
    }
}

//============================================================================
void VxPushButton::setMaximumSize( const QSize & maxSize )
{
    if( maxSize.width() == maxSize.height() )
    {
        QPushButton::setMaximumSize( ( int )( maxSize.width() * GuiParams::getGuiScale() ),
            ( int )( maxSize.height() * GuiParams::getGuiScale() ) );
    }
    else
    {
        QPushButton::setMaximumSize( ( int )( maxSize.width()  * GuiParams::getGuiScale() ),
            ( int )( maxSize.height() * GuiParams::getGuiScale() ) );
    }
}

//============================================================================
void VxPushButton::setMaximumSize( int maxw, int maxh )
{
    QPushButton::setMaximumSize( ( int )( maxw * GuiParams::getGuiScale() ),
                                    ( int )( maxh * GuiParams::getGuiScale() ) );
}

//============================================================================
void VxPushButton::setEnabled( bool enabled )
{
	QPushButton::setEnabled( enabled );
	update();
}

//============================================================================
void VxPushButton::setNotifyEnabled( bool enabled, EMyIcons eNotifyIcon )
{
	m_NotifyEnabled = enabled;
	if( enabled )
	{
		if( eMyIconNone != eNotifyIcon )
		{
			m_NotifyIcon = eNotifyIcon;
		}

		m_BlinkState = 1;
		m_BlinkTimer->start();
	}
	else
	{
		m_BlinkTimer->stop();
		m_BlinkState = 0;
	}

	update();
}

//============================================================================
void VxPushButton::setIcon( EMyIcons myIcon )
{
	m_MyIcon = myIcon;
    update();
}

//============================================================================
void VxPushButton::setIconColor( QColor iconColor )
{
	m_DefaultIconColor = iconColor;
	m_IconColorWasSet = true;
    update();
}

//============================================================================
void VxPushButton::drawBorder(  VxAppTheme& appTheme, QPainter& painter )
{
    // draw button border if needed
    if( m_DrawButtonBorder )
    {
        painter.save();
        appTheme.drawButton( this, painter );
        painter.restore();
    }
}

//============================================================================
void VxPushButton::paintEvent( QPaintEvent* ev )
{
	QPainter painter( this );
    VxAppTheme& appTheme = m_MyApp.getAppTheme();
    if( !text().isEmpty() )
    {
        QPushButton::paintEvent( ev );
        drawBorder( appTheme, painter );
        painter.end();
        return;
    }

	if( eMyIconNone == m_MyIcon )
	{
		// no icon set yet
		return;
	}

#define IMAGE_PADDING  2
	QRect drawRect( IMAGE_PADDING, IMAGE_PADDING, this->width() - ( IMAGE_PADDING * 2 ), this->height() - ( IMAGE_PADDING * 2 ) );
	if( drawRect.width() < 8 )
	{
		LogMsg( LOG_ERROR, "VxPushButton::paintEvent invalid drawRect.width()  %d ", drawRect.width() );
		return;
	}


	QColor iconColor = m_IconColorWasSet ? m_DefaultIconColor : appTheme.getButtonColor( this, eColorLayerIcon );
    drawBorder( appTheme, painter );

	// draw button icon
	painter.setRenderHint( QPainter::Antialiasing, true );
	painter.setRenderHint( QPainter::TextAntialiasing, true );
	painter.setRenderHint( QPainter::SmoothPixmapTransform, true );

	if( ( m_LastIconColor != iconColor )
		|| m_IconImage.isNull()
        || ( m_MyIconLast != m_MyIcon )
		|| ( drawRect.size() != m_LastIconSize ) )
	{
		if( m_UseTheme )
		{
			m_IconImage = getMyIcons().getIconPixmap( m_MyIcon, drawRect.size(), iconColor );
		}
		else
		{
			m_IconImage = getMyIcons().getIconPixmap( m_MyIcon, drawRect.size() );
		}

		if( !m_IconImage.isNull() )
		{
            m_MyIconLast = m_MyIcon;
		    m_LastIconColor = iconColor;
			m_LastIconSize = drawRect.size();
		}
	}

	if( ! m_IconImage.isNull() )
	{
		painter.drawPixmap( drawRect, m_IconImage );
	}

	if( eMyIconNone != m_OverlayIcon )
	{
		// draw overlay
		iconColor = appTheme.getButtonColor( this, eColorLayerOverlay );
		if( ( m_OverlayLastIconColor != iconColor )
			|| m_OverlayIconImage.isNull()
			|| ( drawRect.size() != m_OverlayLastIconSize )
			|| ( m_OverlayIcon != m_LastOverlayIcon ) )
		{
			// get overlay pixmap
			m_OverlayIconImage = getMyIcons().getIconPixmap( m_OverlayIcon, drawRect.size(), iconColor );
			if( ! m_OverlayIconImage.isNull() )
			{
				m_LastOverlayIcon = m_OverlayIcon;
				m_LastIconColor = iconColor;
				m_OverlayLastIconSize = drawRect.size();
			}
		}

		if( ! m_OverlayIconImage.isNull() )
		{
			painter.drawPixmap( drawRect, m_OverlayIconImage );
		}
	}

	if( m_NotifyEnabled
		&& ( eMyIconNone != m_NotifyIcon ) 
		&& ( 1 == m_BlinkState ) )
	{
		// draw notify dot
		iconColor = appTheme.getButtonColor( this, eColorLayerNotify );

		if( ( m_LastIconColor != iconColor )
			|| m_NotifyIconImage.isNull()
			|| ( drawRect.size() != m_LastIconSize )
			|| ( m_NotifyIcon != m_LastNotifyIcon ) )
		{
			m_NotifyIconImage = getMyIcons().getIconPixmap( m_NotifyIcon, drawRect.size(), iconColor );
			if( ! m_IconImage.isNull() )
			{
				m_LastNotifyIcon = m_NotifyIcon;
				m_NotifyLastIconColor = iconColor;
				m_NotifyLastIconSize = drawRect.size();
			}
		}

		if( ! m_NotifyIconImage.isNull() )
		{
			painter.drawPixmap( drawRect, m_IconImage );
		}
	}

	painter.end();
}

//============================================================================
void VxPushButton::setIcons(	EMyIcons	normalIcon,
								EMyIcons	overlay1Icon )
{
	m_MyIcon = normalIcon;
	m_OverlayIcon = overlay1Icon;
	update();
}

//============================================================================
void VxPushButton::setAppIcon( EMyIcons appletIcon, QWidget * parentAppFrame )
{
    m_AppFrame = parentAppFrame;
    m_AppClickCount = 0;
    setIcons( appletIcon );
}

//============================================================================
void VxPushButton::mousePressEvent( QMouseEvent * event )
{
	if( m_IsSlideLeftButton )
	{
		m_MousePressedPoint = event->pos();
		m_MouseCurPoint.setX( 0 );
		m_MouseCurPoint.setY( 0 );
		m_ButtonPressedPos = this->geometry().topLeft();
		m_InSlideLeftMode = true;
	}

	//qDebug() << "VxPushButton::mousePressEvent " << event->pos();

	QPushButton::mousePressEvent( event );
	if( m_IsToggleButton )
	{
		//if( m_ToggleStateOn && !m_PressedOnIcon.isNull() )
		//{
		//	setIcon( m_PressedOnIcon );
		//}

		//if( !m_ToggleStateOn && !m_PressedOffIcon.isNull() )
		//{
		//	setIcon( m_PressedOffIcon );
		//}

		m_ToggleStateOn = !m_ToggleStateOn;
		emit buttonToggleState( m_ToggleStateOn );
	}

	//if( m_IsToggleButton && m_ToggleStateOn )
	//{
	//	setNotifyEnabled( true );
	//}
	//else
	//{
	//	setNotifyEnabled( false );
	//}

	m_MyApp.playSound( m_ESndDefPressed );

    if( m_AppFrame )
    {
        // detect 3 quick clicks then 1 second gap then 2 quick clicks
        m_AppClickCount++;
        int64_t timeNow = GetGmtTimeMs();
        int64_t elapsedMs = TimeElapsedMs( m_AppClickTime, timeNow );
        if( elapsedMs > APP_CLICK_MAX_MS_BETWEEN_SEQUENCE_CLICKS )
        {
            // reset click time
            m_AppClickCount = 1;
            m_AppClickTime = timeNow;
        }

        if( 1 == m_AppClickCount )
        {
            m_AppClickTime = timeNow;
        }
        else if( m_AppClickCount == 4 )
        {
            if( ( elapsedMs < APP_CLICK_MIN_MS_BETWEEN_SEQUENCE_CLICKS )
                || ( elapsedMs > APP_CLICK_MAX_MS_BETWEEN_SEQUENCE_CLICKS ) )
            {
                m_AppClickCount = 0;
            }
            else
            {
                m_AppClickTime = timeNow;
            }
        }
        else
        {
            // validate click time
            bool validClick = elapsedMs < APP_CLICK_MAX_MS_BETWEEN_SHORT_CLICKS;
            if( validClick )
            {
                if( m_AppClickCount == 5 )
                {
                    // finished special sequence
                    m_AppClickCount = 0;
                    emit signalAppIconSpecialClick();
                }
                else
                {
                    m_AppClickTime = timeNow;
                }
            }
            else
            {
                m_AppClickCount = 0;
            }
        }

        LogMsg( LOG_DEBUG, "VxPushButton::mousePressEvent cnt %d elapsedMs %lld", m_AppClickCount, elapsedMs );
    }
}

//============================================================================
void VxPushButton::slotBlinkTimeout( void )
{
	//qDebug() << "VxPushButton::slotBlinkTimeout ";
	m_BlinkState = m_BlinkState ? 0 : 1;
	update();
}

//============================================================================
void VxPushButton::mouseReleaseEvent( QMouseEvent * event )
{
	//qDebug() << "VxPushButton::mouseReleaseEvent " << event->pos();
	m_InSlideLeftMode = false;
	if( !m_IsToggleButton )
	{
		setNotifyEnabled( false );
	}

	QPushButton::mouseReleaseEvent( event );
}

//============================================================================
void VxPushButton::mouseMoveEvent( QMouseEvent * event )
{
	//qDebug() << "VxPushButton::mouseMoveEvent " << event->pos();
	m_MouseCurPoint = event->pos();
	QPushButton::mouseMoveEvent( event );
	if( m_InSlideLeftMode )
	{
		int dify = m_MousePressedPoint.y() - m_MouseCurPoint.y();
		if( abs(dify) > geometry().height()/2 )
		{
			// slide to much up or down
			m_InSlideLeftMode = false;
			this->move( m_ButtonPressedPos );
			return;
		}

		int difx = m_MousePressedPoint.x() - m_MouseCurPoint.x();
		if( difx < -5 )
		{
			// moving wrong direction
			m_InSlideLeftMode = false;
			this->move( m_ButtonPressedPos );
			return;
		}

		QPoint newPos( m_ButtonPressedPos.x() - difx, m_ButtonPressedPos.y() );
		this->move( newPos );
		if( abs( difx )  > geometry().width() * 2 )
		{
			// completed slide
			m_InSlideLeftMode = false;
			this->move( m_ButtonPressedPos );
			emit slideLeftCompleted();
			return;
		}
	}
}
