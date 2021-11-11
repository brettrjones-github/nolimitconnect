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

#include <app_precompiled_hdr.h>
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

, m_MyIcon( eMyIconNone )
, m_MyIconLast( eMyIconNone )
, m_IconImage()
, m_LastIconColor( COLOR_TRANSPARENT)
, m_LastIconSize( 0, 0 )

, m_NotifyLastIconOnlineColor( COLOR_TRANSPARENT)
, m_NotifyLastIconDirectConnectColor( COLOR_TRANSPARENT )
, m_NotifyLastIconInGroupColor( COLOR_TRANSPARENT )

, m_NotifyIconOfferImage()
, m_NotifyLastIconOfferColor( COLOR_TRANSPARENT)
, m_NotifyLastIconOfferSize( 0, 0 )

, m_NotifyForbiddenIcon( eMyIconNone )
, m_LastNotifyForbiddenIcon( eMyIconNone )
, m_NotifyIconForbiddenImage()
, m_NotifyLastIconForbiddenColor( COLOR_TRANSPARENT)
, m_NotifyLastIconForbiddenSize( 0, 0 )

, m_NotifyErrorIcon( eMyIconNone )
, m_LastNotifyErrorIcon( eMyIconNone )
, m_NotifyIconErrorImage()
, m_NotifyLastIconErrorColor( COLOR_TRANSPARENT)
, m_NotifyLastIconErrorSize( 0, 0 )

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

, m_MyIcon( eMyIconNone )
, m_IconImage()
, m_LastIconColor( COLOR_TRANSPARENT )
, m_LastIconSize( 0, 0 )

, m_NotifyIconJoinImage()
, m_NotifyLastIconJoinColor( COLOR_TRANSPARENT)

, m_NotifyIconOnlineImage()
, m_NotifyLastIconOnlineColor( COLOR_TRANSPARENT)

, m_NotifyIconOfferImage()
, m_NotifyLastIconOfferColor( COLOR_TRANSPARENT)

, m_NotifyForbiddenIcon( eMyIconNone )
, m_LastNotifyForbiddenIcon( eMyIconNone )
, m_NotifyIconForbiddenImage()
, m_NotifyLastIconForbiddenColor( COLOR_TRANSPARENT)
, m_NotifyLastIconForbiddenSize( 0, 0 )

, m_NotifyErrorIcon( eMyIconNone )
, m_LastNotifyErrorIcon( eMyIconNone )
, m_NotifyIconErrorImage()
, m_NotifyLastIconErrorColor( COLOR_TRANSPARENT)
, m_NotifyLastIconErrorSize( 0, 0 )

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
    m_NotifyIconOnlineColor = m_MyApp.getAppTheme().getButtonColor( this, eColorLayerNotifyOnline );
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
    if( width >= (int)( GuiParams::getButtonSize( m_SquareButtonSize ).width() ) )
	{
		return width;
	}

    return ( int )( GuiParams::getButtonSize( m_SquareButtonSize ).width() );
}

//============================================================================
QSize VxPushButton::sizeHint( void ) const
{
	return GuiParams::getButtonSize( m_SquareButtonSize );
}

//============================================================================
void VxPushButton::setFixedSize( const QSize & fixedSize )
{
    if( fixedSize.width() == fixedSize.height() )
    {
        QPushButton::setFixedSize( GuiParams::getButtonSize( m_SquareButtonSize ) );
    }
    else
    {
        this->setFixedWidth( fixedSize.width() );
        this->setFixedHeight( fixedSize.height() );
    }
}

//============================================================================
void VxPushButton::setFixedSize( EButtonSize buttonSize )
{
    m_SquareButtonSize = buttonSize;
    QPushButton::setFixedSize( GuiParams::getButtonSize( m_SquareButtonSize ) );
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
void VxPushButton::setNotifyOnlineEnabled( bool enabled, EMyIcons eNotifyIcon )
{
	m_NotifyOnlineEnabled = enabled;
	if( enabled )
	{
		if( eMyIconNone != eNotifyIcon )
		{
			m_NotifyOnlineIcon = eNotifyIcon;
		}
	}

	update();
}

//============================================================================
void VxPushButton::setNotifyDirectConnectEnabled( bool enabled, EMyIcons eNotifyIcon )
{
    m_NotifyDirectConnectEnabled = enabled;
    if( enabled )
    {
        if( eMyIconNone != eNotifyIcon )
        {
            m_NotifyDirectConnectIcon = eNotifyIcon;
        }
    }

    update();
}

//============================================================================
void VxPushButton::setNotifyInGroupEnabled( bool enabled, EMyIcons eNotifyIcon )
{
    m_NotifyInGroupEnabled = enabled;
    if( enabled )
    {
        if( eMyIconNone != eNotifyIcon )
        {
            m_NotifyInGroupIcon = eNotifyIcon;
        }
    }

    update();
}

//============================================================================
void VxPushButton::setNotifyJoinEnabled( bool enabled, EMyIcons eNotifyIcon )
{
    m_NotifyJoinEnabled = enabled;
    if( enabled )
    {
        if( eMyIconNone != eNotifyIcon )
        {
            m_NotifyJoinIcon = eNotifyIcon;
        }
    }

    update();
}

//============================================================================
void VxPushButton::setNotifyOfferEnabled( bool enabled, EMyIcons eNotifyIcon )
{
    m_NotifyOfferEnabled = enabled;
    if( enabled )
    {
        if( eMyIconNone != eNotifyIcon )
        {
            m_NotifyOfferIcon = eNotifyIcon;
        }
    }

    update();
}

//============================================================================
void VxPushButton::setNotifyForbiddenEnabled( bool enabled, EMyIcons eNotifyIcon )
{
    m_NotifyForbiddenEnabled = enabled;
    if( enabled )
    {
        if( eMyIconNone != eNotifyIcon )
        {
            m_NotifyForbiddenIcon = eNotifyIcon;
        }
    }

    update();
}

//============================================================================
void VxPushButton::setNotifyErrorEnabled( bool enabled, EMyIcons eNotifyIcon )
{
    m_NotifyErrorEnabled = enabled;
    if( enabled )
    {
        if( eMyIconNone != eNotifyIcon )
        {
            m_NotifyErrorIcon = eNotifyIcon;
        }
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
void VxPushButton::setIconOverrideColor( QColor iconColor )
{
    m_IconOverrideColor = iconColor;
    m_IconOverrideColorWasSet = true;
    update();
}

//============================================================================
void VxPushButton::setIconOverrideImage( QImage& iconImage )
{
    if( !iconImage.isNull() )
    {
        m_IconOverrideImage = iconImage;
        m_IconOverrideImageWasSet = true;
        m_IconOverrideImageWasDrawn = false;
    }
    else
    {
        LogMsg( LOG_ERROR, "VxPushButton::setIconOverrideImage image is null" );
    }
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

	if( eMyIconNone == m_MyIcon && !m_IconOverrideImageWasSet)
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


    QColor iconColor;
    if( m_IconOverrideColorWasSet )
    {
        iconColor = m_IconOverrideColor;
    }
    else
    {
        iconColor = appTheme.getButtonColor( this, eColorLayerIcon );
    }

    drawBorder( appTheme, painter );

	// draw button icon
	painter.setRenderHint( QPainter::Antialiasing, true );
	painter.setRenderHint( QPainter::TextAntialiasing, true );
	painter.setRenderHint( QPainter::SmoothPixmapTransform, true );

    if( m_IconOverrideImageWasSet && ( !m_IconOverrideImageWasDrawn || m_IconImage.isNull() || ( drawRect.size() != m_LastIconSize ) ) )
    {
        QImage resizedPicmap = m_IconOverrideImage.scaled( drawRect.size(), Qt::KeepAspectRatio, Qt::SmoothTransformation );
        m_IconImage = QPixmap::fromImage( resizedPicmap );
        m_LastIconSize = drawRect.size();
        m_LastIconColor = iconColor;
        m_IconOverrideImageWasDrawn = true;
    }
	else if( !m_IconOverrideImageWasSet && (( m_LastIconColor != iconColor )
		|| m_IconImage.isNull()
        || ( m_MyIconLast != m_MyIcon )
		|| ( drawRect.size() != m_LastIconSize ) ) )
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

    if( m_NotifyJoinEnabled && ( eMyIconNone != m_NotifyJoinIcon ) )
    {
        // draw online notify dot
        iconColor = appTheme.getButtonColor( this, eColorLayerNotifyJoin );

        if( ( m_NotifyLastIconJoinColor != iconColor )
            || m_NotifyIconJoinImage.isNull()
            || ( drawRect.size() != m_NotifyLastIconJoinSize )
            || ( m_NotifyOfferIcon != m_LastNotifyJoinIcon ) )
        {
            m_NotifyIconJoinImage = getMyIcons().getIconPixmap( m_NotifyJoinIcon, drawRect.size(), iconColor );
            if( ! m_IconImage.isNull() )
            {
                m_LastNotifyJoinIcon = m_NotifyJoinIcon;
                m_NotifyLastIconJoinColor = iconColor;
                m_NotifyLastIconJoinSize = drawRect.size();
            }
        }

        if( ! m_NotifyIconJoinImage.isNull() )
        {
            painter.drawPixmap( drawRect, m_NotifyIconJoinImage );
        }
    }

	if( m_NotifyOnlineEnabled && ( eMyIconNone != m_NotifyOnlineIcon ) )
	{
		// draw online notify dot
        iconColor = m_NotifyIconOnlineColor; 

		if( ( m_NotifyLastIconOnlineColor != iconColor )
			|| m_NotifyIconOnlineImage.isNull()
			|| ( drawRect.size() != m_NotifyLastIconOnlineSize )
			|| ( m_NotifyOnlineIcon != m_LastNotifyOnlineIcon ) )
		{
            m_NotifyIconOnlineImage = getMyIcons().getIconPixmap( m_NotifyOnlineIcon, drawRect.size(), iconColor );
			if( ! m_IconImage.isNull() )
			{
                m_LastNotifyOnlineIcon = m_NotifyOnlineIcon;
                m_NotifyLastIconOnlineColor = iconColor;
                m_NotifyLastIconOnlineSize = drawRect.size();
			}
		}

		if( ! m_NotifyIconOnlineImage.isNull() )
		{
			painter.drawPixmap( drawRect, m_NotifyIconOnlineImage );
		}
	}

    if( m_NotifyDirectConnectEnabled && (eMyIconNone != m_NotifyDirectConnectIcon) )
    {
        // draw online notify dot
        iconColor = appTheme.getButtonColor( this, eColorLayerNotifyOnline );

        if( (m_NotifyLastIconDirectConnectColor != iconColor)
            || m_NotifyIconDirectConnectImage.isNull()
            || (drawRect.size() != m_NotifyLastIconDirectConnectSize)
            || (m_NotifyDirectConnectIcon != m_LastNotifyDirectConnectIcon) )
        {
            m_NotifyIconDirectConnectImage = getMyIcons().getIconPixmap( m_NotifyDirectConnectIcon, drawRect.size(), iconColor );
            if( !m_IconImage.isNull() )
            {
                m_LastNotifyDirectConnectIcon = m_NotifyDirectConnectIcon;
                m_NotifyLastIconDirectConnectColor = iconColor;
                m_NotifyLastIconDirectConnectSize = drawRect.size();
            }
        }

        if( !m_NotifyIconDirectConnectImage.isNull() )
        {
            painter.drawPixmap( drawRect, m_NotifyIconDirectConnectImage );
        }
    }

    if( m_NotifyInGroupEnabled && (eMyIconNone != m_NotifyInGroupIcon) )
    {
        // draw online notify dot
        iconColor = appTheme.getButtonColor( this, eColorLayerNotifyOnline );

        if( (m_NotifyLastIconInGroupColor != iconColor)
            || m_NotifyIconInGroupImage.isNull()
            || (drawRect.size() != m_NotifyLastIconInGroupSize)
            || (m_NotifyInGroupIcon != m_LastNotifyInGroupIcon) )
        {
            m_NotifyIconInGroupImage = getMyIcons().getIconPixmap( m_NotifyInGroupIcon, drawRect.size(), iconColor );
            if( !m_IconImage.isNull() )
            {
                m_LastNotifyInGroupIcon = m_NotifyInGroupIcon;
                m_NotifyLastIconInGroupColor = iconColor;
                m_NotifyLastIconInGroupSize = drawRect.size();
            }
        }

        if( !m_NotifyIconInGroupImage.isNull() )
        {
            painter.drawPixmap( drawRect, m_NotifyIconInGroupImage );
        }
    }

    if( m_NotifyOfferEnabled && ( eMyIconNone != m_NotifyOfferIcon ) )
    {
        // draw online notify dot
        iconColor = appTheme.getButtonColor( this, eColorLayerNotifyOffer );

        if( ( m_NotifyLastIconOfferColor != iconColor )
            || m_NotifyIconOfferImage.isNull()
            || ( drawRect.size() != m_NotifyLastIconOfferSize )
            || ( m_NotifyOfferIcon != m_LastNotifyOfferIcon ) )
        {
            m_NotifyIconOfferImage = getMyIcons().getIconPixmap( m_NotifyOfferIcon, drawRect.size(), iconColor );
            if( ! m_IconImage.isNull() )
            {
                m_LastNotifyOfferIcon = m_NotifyOfferIcon;
                m_NotifyLastIconOfferColor = iconColor;
                m_NotifyLastIconOfferSize = drawRect.size();
            }
        }

        if( ! m_NotifyIconOfferImage.isNull() )
        {
            painter.drawPixmap( drawRect, m_NotifyIconOfferImage );
        }
    }

    if( m_NotifyForbiddenEnabled && ( eMyIconNone != m_NotifyForbiddenIcon ) )
    {
        // draw online notify dot
        iconColor = appTheme.getButtonColor( this, eColorLayerNotifyForbidden );

        if( ( m_NotifyLastIconForbiddenColor != iconColor )
            || m_NotifyIconForbiddenImage.isNull()
            || ( drawRect.size() != m_NotifyLastIconForbiddenSize )
            || ( m_NotifyForbiddenIcon != m_LastNotifyForbiddenIcon ) )
        {
            m_NotifyIconForbiddenImage = getMyIcons().getIconPixmap( m_NotifyForbiddenIcon, drawRect.size(), iconColor );
            if( ! m_IconImage.isNull() )
            {
                m_LastNotifyForbiddenIcon = m_NotifyForbiddenIcon;
                m_NotifyLastIconForbiddenColor = iconColor;
                m_NotifyLastIconForbiddenSize = drawRect.size();
            }
        }

        if( ! m_NotifyIconForbiddenImage.isNull() )
        {
            painter.drawPixmap( drawRect, m_NotifyIconForbiddenImage );
        }
    }

    if( m_NotifyErrorEnabled && ( eMyIconNone != m_NotifyErrorIcon ) )
    {
        // draw online notify dot
        iconColor = appTheme.getButtonColor( this, eColorLayerNotifyError );

        if( ( m_NotifyLastIconErrorColor != iconColor )
            || m_NotifyIconErrorImage.isNull()
            || ( drawRect.size() != m_NotifyLastIconErrorSize )
            || ( m_NotifyErrorIcon != m_LastNotifyErrorIcon ) )
        {
            m_NotifyIconErrorImage = getMyIcons().getIconPixmap( m_LastNotifyErrorIcon, drawRect.size(), iconColor );
            if( ! m_IconImage.isNull() )
            {
                m_LastNotifyErrorIcon = m_NotifyErrorIcon;
                m_NotifyLastIconErrorColor = iconColor;
                m_NotifyLastIconErrorSize = drawRect.size();
            }
        }

        if( ! m_NotifyIconErrorImage.isNull() )
        {
            painter.drawPixmap( drawRect, m_NotifyIconErrorImage );
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
    if( !isVisible() )
    {
        return;
    }

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
    /* BRJ ???
	if( !m_IsToggleButton )
	{
		setNotifyOnlineEnabled( false );
	}
    */

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

//============================================================================
void VxPushButton::setSquareButtonSize( EButtonSize buttonSize )
{ 
    m_SquareButtonSize = buttonSize; 
    setFixedSize( GuiParams::getButtonSize( m_SquareButtonSize ) );
}

//============================================================================
void VxPushButton::setOverlayIcon( EMyIcons myIcon )
{
    m_OverlayIcon = myIcon;
}

//============================================================================
void VxPushButton::setOverlayColor( QColor iconColor )
{
    m_OverlayLastIconColor = iconColor;
}