
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

#include "VxAppTheme.h"
#include "AppCommon.h"
#include "AppSettings.h"

#include "VxFlatButton.h"
#include "VxSpinProgress.h"
#include "VxPushButton.h"

#include <CoreLib/VxFileUtil.h>

#include <QApplication>
#include <QWidget>
#include <QDoubleSpinBox>
#include <QPen>
#include <QBrush>
#include <QFrame>
#include <QPalette>
#include <QPainter>
#include <QLabel>
#include <QBitmap>
#include <QPixmap>
#include <QImage>
#include <QStyleOption>
#include <QProgressBar>

#include <stdlib.h> 

namespace
{
	const int BUTTON_BKG_PADDING		= 2;
	const int BUTTON_ROUNDING_RADIUS	= 5;
	const int BUTTON_BORDER_WIDTH		= 2;
}

//============================================================================
VxAppTheme::VxAppTheme( AppCommon& appCommon )
: m_MyApp( appCommon )
, m_TransparentColor( COLOR_TRANSPARENT )
, m_InvalidColor( COLOR_PINK_LIGHT )
, m_SvgRenderer( this )
{
}

//============================================================================
void VxAppTheme::applyTheme( QWidget * widget )
{
    if( widget && m_MyApp.getAppSettings().getFeatureEnable( eAppFeatureTheme ) )
    {
        // make a copy so original is not disturbed
        QPalette widPalette( m_BasePalette );
        widget->setPalette( widPalette );
    }
}

//============================================================================
void VxAppTheme::updateProgressBar( QProgressBar * progressBar )
{
    setHighlightColor( progressBar, getColor( eProgressBarColor ) );
}

//============================================================================
QPixmap VxAppTheme::replaceImageColor( QPixmap srcImage, QColor newColor )
{
    if( srcImage.isNull() )
	{
		LogMsg( LOG_ERROR, "ERROR VxAppTheme::replaceImageColor srcImage is NULL " );
		return srcImage;
	}

	QPainter painter( &srcImage );
	painter.setCompositionMode( QPainter::CompositionMode_SourceIn );
	painter.fillRect( srcImage.rect(), newColor );
	painter.end();

	return srcImage;
}

//============================================================================
QPixmap VxAppTheme::svgRenderer( QString svgName, QSize imageSize, QColor iconColor )
{
	QPixmap renderdPixmap( imageSize );
	renderdPixmap.fill( getTransparentColor() );
	if( svgName.isEmpty()
		|| ( imageSize.width() < 1 )
		|| ( imageSize.height() < 1 ) )
	{
		LogMsg( LOG_ERROR, "ERROR VxAppTheme::svgRenderer invalid param %s", svgName.toUtf8().constData() );
		return renderdPixmap;
	}

	QIcon svgIcon( svgName );
	if( svgIcon.isNull() )
	{
		LogMsg( LOG_ERROR, "ERROR VxAppTheme::svgRenderer svgIcon.isNull() %s ", svgName.toUtf8().constData() );
		return renderdPixmap;
	}

	QPainter painter2( &renderdPixmap );
	painter2.setRenderHint( QPainter::Antialiasing, true );
	painter2.setRenderHint( QPainter::TextAntialiasing, true );
	painter2.setRenderHint( QPainter::SmoothPixmapTransform, true );
	svgIcon.paint( &painter2,  0, 0, renderdPixmap.width(), renderdPixmap.height() );
	painter2.end();
	if( !renderdPixmap.isNull() )
	{
		return replaceImageColor( renderdPixmap, iconColor );
	}

	return renderdPixmap;
}

//============================================================================
QPixmap VxAppTheme::svgRenderer( QString svgName, QSize imageSize )
{
	QPixmap renderdPixmap( imageSize );

    renderdPixmap.fill( getTransparentColor() );
	if( svgName.isEmpty()
		|| ( imageSize.width() < 1 )
		|| ( imageSize.height() < 1 ) )
	{
		LogMsg( LOG_ERROR, "ERROR VxAppTheme::svgRenderer invalid param %s", svgName.toUtf8().constData() );
		return renderdPixmap;
	}

	QIcon svgIcon( svgName );
	if( svgIcon.isNull() )
	{
		LogMsg( LOG_ERROR, "ERROR VxAppTheme::svgRenderer svgIcon.isNull() %s ", svgName.toUtf8().constData() );
		return renderdPixmap;
	}

	QPainter painter2( &renderdPixmap );
	painter2.setRenderHint( QPainter::Antialiasing, true );
	painter2.setRenderHint( QPainter::TextAntialiasing, true );
	painter2.setRenderHint( QPainter::SmoothPixmapTransform, true );
	svgIcon.paint( &painter2, 0, 0, renderdPixmap.width(), renderdPixmap.height() );
	painter2.end();

	return renderdPixmap;
}

//============================================================================
QString  VxAppTheme::getResourceFullFileName( QString resourceName )
{
	if( resourceName.length() < 2 )
	{
		LogMsg( LOG_ERROR, "VxAppTheme::getResourceFullFileName empty resourceName" ); 
		return "";
	}

	if( ('/' == resourceName[0] )
		|| (':' == resourceName[1] ) )
	{
		if(  VxFileUtil::fileExists( resourceName.toUtf8().constData() )  )
		{
			// already path and name
			return resourceName;
		}
	}

	if( resourceName.contains( GOTV_APP_RESOURCE_PREFIX ) )
	{
		if( VxFileUtil::fileExists( resourceName.toUtf8().constData() ) )
		{
			return resourceName;
		}
	}

	QString resourcePathAndName = GOTV_APP_RESOURCE_PREFIX + resourceName;
	if( VxFileUtil::fileExists( resourcePathAndName.toUtf8().constData() ) )
	{
		return resourcePathAndName;
	}

	LogMsg( LOG_ERROR, "ERROR VxAppTheme::getIconFullFileName resource does not exist ", resourceName.toUtf8().constData() );
	return "";
}

//============================================================================
void VxAppTheme::drawButton( QPushButton * button, QPainter& painter )
{
    painter.save();
    const QPalette& originalPallete = button->palette();
    button->setPalette( getBasePalette() );
	QRect drawAreaRect( BUTTON_BKG_PADDING, BUTTON_BKG_PADDING, button->width() - ( BUTTON_BKG_PADDING * 2 ), button->height() - ( BUTTON_BKG_PADDING * 2 ) );

	painter.setRenderHint( QPainter::Antialiasing );

	QColor bkgColor( getButtonColor( button, eColorLayerBackground ) );
	QColor fgdColor( getButtonColor( button, eColorLayerForeground ) );

	painter.setPen( QPen( fgdColor, BUTTON_BORDER_WIDTH ) );
	painter.setBrush( initBrush( bkgColor ) );
	painter.drawRoundedRect( drawAreaRect, BUTTON_ROUNDING_RADIUS, BUTTON_ROUNDING_RADIUS );
    button->setPalette( originalPallete );
    painter.restore();
}

//============================================================================
void VxAppTheme::drawFocusRect( QPainter& painter, const QRect& focusRect, int lineWidth )
{
    painter.save();
	QColor bkgColor( getTransparentColor() );
	QColor fgdColor( getColor( eFocusRect ) );

	painter.setPen( QPen( fgdColor, lineWidth ) );
	painter.setBrush( initBrush( bkgColor ) );
	painter.drawRect( focusRect );
    painter.restore();
}

//============================================================================
QColor VxAppTheme::getButtonColor( QPushButton * button, EColorLayerType colorLayer )
{
	if( false == button->isEnabled() )
	{
		switch( colorLayer )
		{
		case eColorLayerBackground:
			return getColor( eButtonBackgroundDisabled );

		case eColorLayerForeground:
		case eColorLayerIcon:
			return getColor( eButtonForegroundDisabled );

		case eColorLayerText:
		default:
			return getColor( eButtonTextDisabled );
		}
	}
	else if( button->isDown() )
	{
		switch( colorLayer )
		{
		case eColorLayerBackground:
			return getColor( eButtonBackgroundPressed );

		case eColorLayerForeground:
		case eColorLayerIcon:
			return getColor( eButtonForegroundPressed );

		case eColorLayerText:
		default:
			return getColor( eButtonTextPressed );
		}
	}
	else if( button->hasFocus() )
	{
		switch( colorLayer )
		{
		case eColorLayerBackground:
			return getColor( eButtonBackgroundNormal );

		case eColorLayerForeground:
		case eColorLayerIcon:
			return getColor( eButtonForegroundNormal );

		case eColorLayerOverlay:
			return getColor( eLayerOverlayColor );
		case eColorLayerNotifyOnline:
			return getColor( eLayerNotifyOnlineColor );
        case eColorLayerNotifyJoin:
            return getColor( eLayerNotifyOnlineColor );
        case eColorLayerNotifyOffer:
            return getColor( eLayerNotifyOfferColor );
        case eColorLayerNotifyForbidden:
            return getColor( eLayerNotifyForbiddenColor );
        case eColorLayerNotifyError:
            return getColor( eLayerNotifyErrorColor );

		case eColorLayerText:
		default:
			return getColor( eButtonTextNormal );
		}
	}
	else
	{
		// normal
		switch( colorLayer )
		{
		case eColorLayerBackground:
			return getColor( eButtonBackgroundNormal );

		case eColorLayerForeground:
		case eColorLayerIcon:
			return getColor( eButtonForegroundNormal );

		case eColorLayerText:
		default:
			return getColor( eButtonTextNormal );
		}
	}
}

//============================================================================
void VxAppTheme::drawBitmapOnWidget( QPainter& painter, QPixmap& widgetPixmap, QSize sizeOfWidget )
{
    int xOffset = 0;
	int yOffset = 0;
	int least = sizeOfWidget.width() < sizeOfWidget.height() ? sizeOfWidget.width() : sizeOfWidget.height();
	xOffset = ( sizeOfWidget.width() - least ) / 2;
	yOffset = ( sizeOfWidget.height() - least ) / 2;
	QSize sizeToDraw = QSize( least, least );
	painter.setRenderHint( QPainter::Antialiasing );
	if( xOffset || yOffset )
	{
		// need to resize the image
		QImage scaledImage = widgetPixmap.toImage().scaled( sizeToDraw );
		painter.drawImage( xOffset, yOffset, scaledImage );
	}
	else
	{ 
		// no need to rescale
		painter.drawPixmap( 0, 0, widgetPixmap );
	}
}

//============================================================================
void VxAppTheme::setHighlightColor( QWidget* widget, const QColor highlightColor )
{
    QPalette palette;
	QBrush brush;

	brush = initBrush( highlightColor );
	palette = getWidgetPalette( widget );
	palette.setBrush( QPalette::Active, QPalette::Highlight, brush );
	palette.setBrush( QPalette::Inactive, QPalette::Highlight, brush );
	palette.setBrush( QPalette::Disabled, QPalette::Highlight, brush );

	setWidgetPallette( widget, palette );
}

//============================================================================
void VxAppTheme::setBackgroundColor( QWidget * widget, const QColor backgroundColor )
{
    QPalette widgetPalette = widget->palette();
	widgetPalette.setColor( QPalette::Base, backgroundColor );
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
	widgetPalette.setColor( QPalette::Background, backgroundColor );
#endif // QT_VERSION < QT_VERSION_CHECK(6,0,0)
	widgetPalette.setColor( QPalette::Window, backgroundColor );
	widget->setPalette( widgetPalette );
}

//============================================================================
void VxAppTheme::setBackgroundColor( QFrame * frame, const QColor backgroundColor )
{
    QPalette framePalette = frame->palette();
	framePalette.setColor( QPalette::Base, backgroundColor );
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
	framePalette.setColor( QPalette::Background, backgroundColor );
#endif // QT_VERSION < QT_VERSION_CHECK(6,0,0)
	framePalette.setColor( QPalette::Window, backgroundColor );
	frame->setPalette( framePalette );
}

//============================================================================
void VxAppTheme::setWidgetBackgroundToTransparent( QWidget * widget )
{
    QColor transparentColor( Qt::transparent );
	QPalette transparentPalette = widget->palette();
	transparentPalette.setColor( QPalette::Base, transparentColor );
	widget->setPalette( transparentPalette );
}

//============================================================================
void VxAppTheme::setPainterPen( QPainter& painter, QColor penColor )
{
    QPen painterPen( penColor );
	painterPen.setStyle( Qt::SolidLine );
	painter.setPen( painterPen );
}

//============================================================================
void VxAppTheme::setPainterBrush( QPainter& painter, QColor brushColor )
{
    QBrush brush( brushColor );
	brush.setStyle( Qt::SolidPattern );
	painter.setBrush( brush );
}

//============================================================================
QBrush VxAppTheme::initBrush( const QColor& color )
{
	QBrush brush;
	brush.setStyle( Qt::SolidPattern );
	brush.setColor( color );
	return brush;
}

//============================================================================
QPalette VxAppTheme::getWidgetPalette( QWidget* widget )
{
	QPalette palette;

	if ( widget )
	{
		palette = widget->palette();
	}

	return palette;
}

//============================================================================
void VxAppTheme::setWidgetPallette( QWidget* widget, QPalette& palette )
{

    if( widget )
	{
		widget->setPalette( palette );
	}
}

//============================================================================
void VxAppTheme::setTextColor( const QColor& color, QWidget* widget )
{
	QBrush brush;

	brush = initBrush( color );
	QPalette palette = getWidgetPalette( widget );
	palette.setBrush( QPalette::Active, QPalette::Text, brush );
	palette.setBrush( QPalette::Inactive, QPalette::Text, brush );
	palette.setBrush( QPalette::Disabled, QPalette::Text, brush );
    palette.setBrush( QPalette::Normal, QPalette::Text, brush );
    palette.setBrush( QPalette::Active, QPalette::HighlightedText, brush );
	palette.setBrush( QPalette::Inactive, QPalette::HighlightedText, brush );
	palette.setBrush( QPalette::Disabled, QPalette::HighlightedText, brush );
    palette.setBrush( QPalette::Normal, QPalette::HighlightedText, brush );
    palette.setColor( QPalette::WindowText, color );

	setWidgetPallette( widget, palette );
}

//============================================================================
QString VxAppTheme::getColorValueAsText( const QColor& colorVal )
{
	if( isTransparentColor( colorVal ) )
	{
		return "transparent";
	}

	return QString( "rgba(%1,%2,%3,%4)" ).arg( colorVal.red() ).arg( colorVal.green() ).arg( colorVal.blue() ).arg( colorVal.alpha() );
}

//============================================================================
QColor VxAppTheme::getColor( EThemeColorRole colorRole )
{
	if( isValidColorRole( colorRole ) )
	{
		return m_ThemeColors.at( colorRole );
	}
	else
	{
		LogMsg( LOG_ERROR, "VxAppTheme::getColor Invalid Color Role %d", colorRole );
		return m_InvalidColor;
	}
}

//============================================================================
bool VxAppTheme::isTransparentColor( const QColor& colorVal )
{
	return ( 0 == colorVal.alpha() );
}

//============================================================================
bool VxAppTheme::areColorsEqual( const QColor& colorVal1, const QColor& colorVal2 )
{
	return ( ( colorVal1.alpha() == colorVal2.alpha() )
		&& ( colorVal1.red() == colorVal2.red() )
		&& ( colorVal1.green() == colorVal2.green() )
		&& ( colorVal1.blue() == colorVal2.blue() ) );
}

//============================================================================
bool VxAppTheme::isValidColorRole( EThemeColorRole colorRole )
{
	if( ( 0 <= colorRole )
		&& ( colorRole < m_ThemeColors.size() ) )
	{
		return true;
	}

	return false;
}

//============================================================================
QString VxAppTheme::describeTheme( EThemeType eThemeType )
{
	switch( eThemeType )
	{
	case eThemeTypeUnknown:
		return "Unknown Theme ";

	case eThemeTypeLight:
		return "Light Theme ";

	case eThemeTypeDark:
		return "Dark Theme ";

	case eThemeTypeBlueOnWhite:
		return "Blue/White Theme ";

	case eThemeTypeGreenOnWhite:
		return "Green/White Theme ";

	default:
		return "Theme Invalid Type ";
	}
}

//============================================================================
// set every color role for testing
void VxAppTheme::setEveryColorPossible( QPalette& palette, const QColor& bkgColor, const QColor& fgdColor )
{
    palette.setBrush( QPalette::Active, QPalette::Window, bkgColor );
    palette.setBrush( QPalette::Inactive, QPalette::Window, bkgColor );
    palette.setBrush( QPalette::Disabled, QPalette::Window, bkgColor );
    palette.setBrush( QPalette::Normal, QPalette::Window, bkgColor );
    palette.setColor( QPalette::Active, QPalette::Window, bkgColor );
    palette.setColor( QPalette::Inactive, QPalette::Window, bkgColor );
    palette.setColor( QPalette::Disabled, QPalette::Window, bkgColor );
    palette.setColor( QPalette::Normal, QPalette::Window, bkgColor );

    palette.setBrush( QPalette::Active, QPalette::WindowText, fgdColor );
    palette.setBrush( QPalette::Inactive, QPalette::WindowText, fgdColor );
    palette.setBrush( QPalette::Disabled, QPalette::WindowText, fgdColor );
    palette.setBrush( QPalette::Normal, QPalette::WindowText, fgdColor );
    palette.setColor( QPalette::Active, QPalette::WindowText, fgdColor );
    palette.setColor( QPalette::Inactive, QPalette::WindowText, fgdColor );
    palette.setColor( QPalette::Disabled, QPalette::WindowText, fgdColor );
    palette.setColor( QPalette::Normal, QPalette::WindowText, fgdColor );

    palette.setBrush( QPalette::Active, QPalette::Base, fgdColor );
    palette.setBrush( QPalette::Inactive, QPalette::Base, fgdColor );
    palette.setBrush( QPalette::Disabled, QPalette::Base, fgdColor );
    palette.setBrush( QPalette::Normal, QPalette::Base, fgdColor );
    palette.setColor( QPalette::Active, QPalette::Base, fgdColor );
    palette.setColor( QPalette::Inactive, QPalette::Base, fgdColor );
    palette.setColor( QPalette::Disabled, QPalette::Base, fgdColor );
    palette.setColor( QPalette::Normal, QPalette::Base, fgdColor );

    palette.setBrush( QPalette::Active, QPalette::AlternateBase, bkgColor );
    palette.setBrush( QPalette::Inactive, QPalette::AlternateBase, bkgColor );
    palette.setBrush( QPalette::Disabled, QPalette::AlternateBase, bkgColor );
    palette.setBrush( QPalette::Normal, QPalette::AlternateBase, bkgColor );
    palette.setColor( QPalette::Active, QPalette::AlternateBase, bkgColor );
    palette.setColor( QPalette::Inactive, QPalette::AlternateBase, bkgColor );
    palette.setColor( QPalette::Disabled, QPalette::AlternateBase, bkgColor );
    palette.setColor( QPalette::Normal, QPalette::AlternateBase, bkgColor );

    palette.setBrush( QPalette::Active, QPalette::ToolTipBase, bkgColor );
    palette.setBrush( QPalette::Inactive, QPalette::ToolTipBase, bkgColor );
    palette.setBrush( QPalette::Disabled, QPalette::ToolTipBase, bkgColor );
    palette.setBrush( QPalette::Normal, QPalette::ToolTipBase, bkgColor );
    palette.setColor( QPalette::Active, QPalette::ToolTipBase, bkgColor );
    palette.setColor( QPalette::Inactive, QPalette::ToolTipBase, bkgColor );
    palette.setColor( QPalette::Disabled, QPalette::ToolTipBase, bkgColor );
    palette.setColor( QPalette::Normal, QPalette::ToolTipBase, bkgColor );

    palette.setBrush( QPalette::Active, QPalette::ToolTipText, fgdColor );
    palette.setBrush( QPalette::Inactive, QPalette::ToolTipText, fgdColor );
    palette.setBrush( QPalette::Disabled, QPalette::ToolTipText, fgdColor );
    palette.setBrush( QPalette::Normal, QPalette::ToolTipText, fgdColor );
    palette.setColor( QPalette::Active, QPalette::ToolTipText, fgdColor );
    palette.setColor( QPalette::Inactive, QPalette::ToolTipText, fgdColor );
    palette.setColor( QPalette::Disabled, QPalette::ToolTipText, fgdColor );
    palette.setColor( QPalette::Normal, QPalette::ToolTipText, fgdColor );

    palette.setBrush( QPalette::Active, QPalette::PlaceholderText, fgdColor );
    palette.setBrush( QPalette::Inactive, QPalette::PlaceholderText, fgdColor );
    palette.setBrush( QPalette::Disabled, QPalette::PlaceholderText, fgdColor );
    palette.setBrush( QPalette::Normal, QPalette::PlaceholderText, fgdColor );
    palette.setColor( QPalette::Active, QPalette::PlaceholderText, fgdColor );
    palette.setColor( QPalette::Inactive, QPalette::PlaceholderText, fgdColor );
    palette.setColor( QPalette::Disabled, QPalette::PlaceholderText, fgdColor );
    palette.setColor( QPalette::Normal, QPalette::PlaceholderText, fgdColor );


    palette.setBrush( QPalette::Active, QPalette::Text, fgdColor );
    palette.setBrush( QPalette::Inactive, QPalette::Text, fgdColor );
    palette.setBrush( QPalette::Disabled, QPalette::Text, fgdColor );
    palette.setBrush( QPalette::Normal, QPalette::Text, fgdColor );
    palette.setColor( QPalette::Active, QPalette::Text, fgdColor );
    palette.setColor( QPalette::Inactive, QPalette::Text, fgdColor );
    palette.setColor( QPalette::Disabled, QPalette::Text, fgdColor );
    palette.setColor( QPalette::Normal, QPalette::Text, fgdColor );

    palette.setBrush( QPalette::Active, QPalette::Button, bkgColor );
    palette.setBrush( QPalette::Inactive, QPalette::Button, bkgColor );
    palette.setBrush( QPalette::Disabled, QPalette::Button, bkgColor );
    palette.setBrush( QPalette::Normal, QPalette::Button, bkgColor );
    palette.setColor( QPalette::Active, QPalette::Button, bkgColor );
    palette.setColor( QPalette::Inactive, QPalette::Button, bkgColor );
    palette.setColor( QPalette::Disabled, QPalette::Button, bkgColor );
    palette.setColor( QPalette::Normal, QPalette::Button, bkgColor );

    palette.setBrush( QPalette::Active, QPalette::ButtonText, fgdColor );
    palette.setBrush( QPalette::Inactive, QPalette::ButtonText, fgdColor );
    palette.setBrush( QPalette::Disabled, QPalette::ButtonText, fgdColor );
    palette.setBrush( QPalette::Normal, QPalette::ButtonText, fgdColor );
    palette.setColor( QPalette::Active, QPalette::ButtonText, fgdColor );
    palette.setColor( QPalette::Inactive, QPalette::ButtonText, fgdColor );
    palette.setColor( QPalette::Disabled, QPalette::ButtonText, fgdColor );
    palette.setColor( QPalette::Normal, QPalette::ButtonText, fgdColor );

    palette.setBrush( QPalette::Active, QPalette::BrightText, fgdColor );
    palette.setBrush( QPalette::Inactive, QPalette::BrightText, fgdColor );
    palette.setBrush( QPalette::Disabled, QPalette::BrightText, fgdColor );
    palette.setBrush( QPalette::Normal, QPalette::BrightText, fgdColor );
    palette.setColor( QPalette::Active, QPalette::BrightText, fgdColor );
    palette.setColor( QPalette::Inactive, QPalette::BrightText, fgdColor );
    palette.setColor( QPalette::Disabled, QPalette::BrightText, fgdColor );
    palette.setColor( QPalette::Normal, QPalette::BrightText, fgdColor );

    palette.setBrush( QPalette::Active, QPalette::Light, fgdColor );
    palette.setBrush( QPalette::Inactive, QPalette::Light, fgdColor );
    palette.setBrush( QPalette::Disabled, QPalette::Light, fgdColor );
    palette.setBrush( QPalette::Normal, QPalette::Light, fgdColor );
    palette.setColor( QPalette::Active, QPalette::Light, fgdColor );
    palette.setColor( QPalette::Inactive, QPalette::Light, fgdColor );
    palette.setColor( QPalette::Disabled, QPalette::Light, fgdColor );
    palette.setColor( QPalette::Normal, QPalette::Light, fgdColor );

    palette.setBrush( QPalette::Active, QPalette::Midlight, fgdColor.darker() );
    palette.setBrush( QPalette::Inactive, QPalette::Midlight, fgdColor.darker() );
    palette.setBrush( QPalette::Disabled, QPalette::Midlight, fgdColor.darker() );
    palette.setBrush( QPalette::Normal, QPalette::Midlight, fgdColor.darker() );
    palette.setColor( QPalette::Active, QPalette::Midlight, fgdColor.darker() );
    palette.setColor( QPalette::Inactive, QPalette::Midlight, fgdColor.darker() );
    palette.setColor( QPalette::Disabled, QPalette::Midlight, fgdColor.darker() );
    palette.setColor( QPalette::Normal, QPalette::Midlight, fgdColor.darker() );

    palette.setBrush( QPalette::Active, QPalette::Dark, bkgColor );
    palette.setBrush( QPalette::Inactive, QPalette::Dark, bkgColor );
    palette.setBrush( QPalette::Disabled, QPalette::Dark, bkgColor );
    palette.setBrush( QPalette::Normal, QPalette::Dark, bkgColor );
    palette.setColor( QPalette::Active, QPalette::Dark, bkgColor );
    palette.setColor( QPalette::Inactive, QPalette::Dark, bkgColor );
    palette.setColor( QPalette::Disabled, QPalette::Dark, bkgColor );
    palette.setColor( QPalette::Normal, QPalette::Dark, bkgColor );

    palette.setBrush( QPalette::Active, QPalette::Mid, bkgColor.lighter() );
    palette.setBrush( QPalette::Inactive, QPalette::Mid, bkgColor.lighter() );
    palette.setBrush( QPalette::Disabled, QPalette::Mid, bkgColor.lighter() );
    palette.setBrush( QPalette::Normal, QPalette::Mid, bkgColor.lighter() );
    palette.setColor( QPalette::Active, QPalette::Mid, bkgColor.lighter() );
    palette.setColor( QPalette::Inactive, QPalette::Mid, bkgColor.lighter() );
    palette.setColor( QPalette::Disabled, QPalette::Mid, bkgColor.lighter() );
    palette.setColor( QPalette::Normal, QPalette::Mid, bkgColor.lighter() );

    palette.setBrush( QPalette::Active, QPalette::Shadow, bkgColor.lighter() );
    palette.setBrush( QPalette::Inactive, QPalette::Shadow, bkgColor.lighter() );
    palette.setBrush( QPalette::Disabled, QPalette::Shadow, bkgColor.lighter() );
    palette.setBrush( QPalette::Normal, QPalette::Shadow, bkgColor.lighter() );
    palette.setColor( QPalette::Active, QPalette::Shadow, bkgColor.lighter() );
    palette.setColor( QPalette::Inactive, QPalette::Shadow, bkgColor.lighter() );
    palette.setColor( QPalette::Disabled, QPalette::Shadow, bkgColor.lighter() );
    palette.setColor( QPalette::Normal, QPalette::Shadow, bkgColor.lighter() );

    palette.setBrush( QPalette::Active, QPalette::Highlight, fgdColor.darker() );
    palette.setBrush( QPalette::Inactive, QPalette::Highlight, fgdColor.darker() );
    palette.setBrush( QPalette::Disabled, QPalette::Highlight, fgdColor.darker() );
    palette.setBrush( QPalette::Normal, QPalette::Highlight, fgdColor.darker() );
    palette.setColor( QPalette::Active, QPalette::Highlight, fgdColor.darker() );
    palette.setColor( QPalette::Inactive, QPalette::Highlight, fgdColor.darker() );
    palette.setColor( QPalette::Disabled, QPalette::Highlight, fgdColor.darker() );
    palette.setColor( QPalette::Normal, QPalette::Highlight, fgdColor.darker() );


    palette.setBrush( QPalette::Active, QPalette::HighlightedText, fgdColor.lighter() );
    palette.setBrush( QPalette::Inactive, QPalette::HighlightedText, fgdColor.lighter() );
    palette.setBrush( QPalette::Disabled, QPalette::HighlightedText, fgdColor.lighter() );
    palette.setBrush( QPalette::Normal, QPalette::HighlightedText, fgdColor.lighter() );
    palette.setColor( QPalette::Active, QPalette::HighlightedText, fgdColor.lighter() );
    palette.setColor( QPalette::Inactive, QPalette::HighlightedText, fgdColor.lighter() );
    palette.setColor( QPalette::Disabled, QPalette::HighlightedText, fgdColor.lighter() );
    palette.setColor( QPalette::Normal, QPalette::HighlightedText, fgdColor.lighter() );


    palette.setBrush( QPalette::Active, QPalette::Link, fgdColor );
    palette.setBrush( QPalette::Inactive, QPalette::Link, fgdColor );
    palette.setBrush( QPalette::Disabled, QPalette::Link, fgdColor );
    palette.setBrush( QPalette::Normal, QPalette::Link, fgdColor );
    palette.setColor( QPalette::Active, QPalette::Link, fgdColor );
    palette.setColor( QPalette::Inactive, QPalette::Link, fgdColor );
    palette.setColor( QPalette::Disabled, QPalette::Link, fgdColor );
    palette.setColor( QPalette::Normal, QPalette::Link, fgdColor );

    palette.setBrush( QPalette::Active, QPalette::LinkVisited, fgdColor.lighter() );
    palette.setBrush( QPalette::Inactive, QPalette::LinkVisited, fgdColor.lighter() );
    palette.setBrush( QPalette::Disabled, QPalette::LinkVisited, fgdColor.lighter() );
    palette.setBrush( QPalette::Normal, QPalette::LinkVisited, fgdColor.lighter() );
    palette.setColor( QPalette::Active, QPalette::LinkVisited, fgdColor.lighter() );
    palette.setColor( QPalette::Inactive, QPalette::LinkVisited, fgdColor.lighter() );
    palette.setColor( QPalette::Disabled, QPalette::LinkVisited, fgdColor.lighter() );
    palette.setColor( QPalette::Normal, QPalette::LinkVisited, fgdColor.lighter() );

    palette.setBrush( QPalette::Active, QPalette::NoRole, fgdColor );
    palette.setBrush( QPalette::Inactive, QPalette::NoRole, fgdColor );
    palette.setBrush( QPalette::Disabled, QPalette::NoRole, fgdColor );
    palette.setBrush( QPalette::Normal, QPalette::NoRole, fgdColor );
    palette.setColor( QPalette::Active, QPalette::NoRole, fgdColor );
    palette.setColor( QPalette::Inactive, QPalette::NoRole, fgdColor );
    palette.setColor( QPalette::Disabled, QPalette::NoRole, fgdColor );
    palette.setColor( QPalette::Normal, QPalette::NoRole, fgdColor );

//#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
    for( int i = QPalette::Active; i <= QPalette::All; i++ )
    {
        QPalette::ColorGroup colorGroup = (QPalette::ColorGroup)i;
        palette.setColor( colorGroup, QPalette::WindowText, fgdColor );
        palette.setColor( colorGroup, QPalette::Button, fgdColor );
        palette.setColor( colorGroup, QPalette::Light, fgdColor );
        palette.setColor( colorGroup, QPalette::Midlight, fgdColor );
        palette.setColor( colorGroup, QPalette::Dark, fgdColor );
        palette.setColor( colorGroup, QPalette::Mid, fgdColor );
        palette.setColor( colorGroup, QPalette::Text, fgdColor );
        palette.setColor( colorGroup, QPalette::BrightText, fgdColor );
        palette.setColor( colorGroup, QPalette::ButtonText, fgdColor );
        palette.setColor( colorGroup, QPalette::Base, fgdColor );
        palette.setColor( colorGroup, QPalette::Window, fgdColor );
        palette.setColor( colorGroup, QPalette::Shadow, fgdColor );
        palette.setColor( colorGroup, QPalette::Highlight, fgdColor );
        palette.setColor( colorGroup, QPalette::HighlightedText, fgdColor );
        palette.setColor( colorGroup, QPalette::Link, fgdColor );
        palette.setColor( colorGroup, QPalette::HighlightedText, fgdColor );
        palette.setColor( colorGroup, QPalette::Link, fgdColor );
        palette.setColor( colorGroup, QPalette::LinkVisited, fgdColor );
        palette.setColor( colorGroup, QPalette::AlternateBase, fgdColor );
        palette.setColor( colorGroup, QPalette::NoRole, fgdColor );
        palette.setColor( colorGroup, QPalette::ToolTipBase, fgdColor );
        palette.setColor( colorGroup, QPalette::ToolTipText, fgdColor );
        palette.setColor( colorGroup, QPalette::PlaceholderText, fgdColor );

        palette.setBrush( colorGroup, QPalette::WindowText, fgdColor );
        palette.setBrush( colorGroup, QPalette::Button, fgdColor );
        palette.setBrush( colorGroup, QPalette::Light, fgdColor );
        palette.setBrush( colorGroup, QPalette::Midlight, fgdColor );
        palette.setBrush( colorGroup, QPalette::Dark, fgdColor );
        palette.setBrush( colorGroup, QPalette::Mid, fgdColor );
        palette.setBrush( colorGroup, QPalette::Text, fgdColor );
        palette.setBrush( colorGroup, QPalette::BrightText, fgdColor );
        palette.setBrush( colorGroup, QPalette::ButtonText, fgdColor );
        palette.setBrush( colorGroup, QPalette::Base, fgdColor );
        palette.setBrush( colorGroup, QPalette::Window, fgdColor );
        palette.setBrush( colorGroup, QPalette::Shadow, fgdColor );
        palette.setBrush( colorGroup, QPalette::Highlight, fgdColor );
        palette.setBrush( colorGroup, QPalette::HighlightedText, fgdColor );
        palette.setBrush( colorGroup, QPalette::Link, fgdColor );
        palette.setBrush( colorGroup, QPalette::HighlightedText, fgdColor );
        palette.setBrush( colorGroup, QPalette::Link, fgdColor );
        palette.setBrush( colorGroup, QPalette::LinkVisited, fgdColor );
        palette.setBrush( colorGroup, QPalette::AlternateBase, fgdColor );

        palette.setBrush( colorGroup, QPalette::NoRole, fgdColor );
        palette.setBrush( colorGroup, QPalette::ToolTipBase, fgdColor );
        palette.setBrush( colorGroup, QPalette::ToolTipText, fgdColor );
        palette.setBrush( colorGroup, QPalette::PlaceholderText, fgdColor );
    }  

//#endif // QT_VERSION >= QT_VERSION_CHECK(6,0,0)
}

//============================================================================
void VxAppTheme::updateWindowTheme( void )
{
    QColor windowBkgColor( getColor( eWindowBackground ) );
    QColor windowTextColor( getColor( eWindowTextColor ) );

    QColor buttonBkgColor( getColor( eButtonBackgroundNormal ) );
    //QColor buttonBkgColor( QColor( COLOR_GREEN ) );
    QColor buttonTextColor( getColor( eButtonTextNormal ) );

    QPalette palette( buttonBkgColor, windowBkgColor );
    palette.setColor( QPalette::Base, getColor( eButtonBackgroundNormal ) );   // Base drop down combo box background

    //palette.setBrush( QPalette::All, QPalette::Button, buttonBkgColor );


    palette.setColor( QPalette::BrightText, isBlackOnWhite() ? buttonTextColor.darker() : buttonTextColor.lighter() );        // BrightText
    palette.setColor( QPalette::WindowText, windowTextColor );                  // WindowText
    palette.setColor( QPalette::Text, windowTextColor );                        // Text and foreground
    //palette.setColor( QPalette::WindowText, COLOR_RED );                      // WindowText
    //palette.setColor( QPalette::Text, COLOR_RED );                            // Text and foreground

    palette.setColor( QPalette::Window, windowBkgColor );                       // Window and background

    //palette.setColor( QPalette::Shadow, QColor( COLOR_GREEN ) );                // shadow

    palette.setColor( QPalette::Highlight, getColor( eProgressBarColor ) );		// Highlight and progress bar
    palette.setColor( QPalette::HighlightedText, getColor( eWindowHighlightTextColor ) );	// HighlightedText
    palette.setColor( QPalette::Link, QColor( COLOR_BLUE ) );                   // Link
    palette.setColor( QPalette::LinkVisited, QColor( COLOR_PURPLE ) );          // LinkVisited
    palette.setColor( QPalette::ToolTipText, windowTextColor );                 // ToolTipText
    palette.setColor( QPalette::ToolTipBase, QColor( COLOR_YELLOW_DARK ) );     // ToolTipBase

    palette.setColor( QPalette::Shadow, getColor( eShadowColor ) );             // shadow

    palette.setColor( QPalette::Light, windowTextColor );                       // Light
    palette.setColor( QPalette::Midlight, windowTextColor.darker() );           // Midlight.. mostly for 
#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
    palette.setColor(QPalette::Mid, windowTextColor.darker().darker());
    palette.setColor(QPalette::Dark, windowTextColor.darker().darker().darker());
#else
    palette.setColor( QPalette::Mid, windowTextColor.dark() );                  // Mid
    palette.setColor( QPalette::Dark, windowTextColor.dark().darker() );        // Dark .. mostly for frame shadow and radio button outline
#endif // QT_VERSION >= QT_VERSION_CHECK(6,0,0)

    palette.setColor( QPalette::AlternateBase, QColor( COLOR_PINK_LIGHT ) );     // AlternateBase ( for alternating row color )

    palette.setColor( QPalette::ButtonText, buttonTextColor );                  // ButtonText and drop down button icon color
    palette.setColor( QPalette::Button, buttonBkgColor );                       // Scroll and spin button background color

    palette.setColor( QPalette::Light, QColor( COLOR_RED ) );                   // Light
    palette.setColor( QPalette::Midlight, QColor( COLOR_RED ) );                // Midlight.. mostly for 
    palette.setColor( QPalette::Mid, QColor( COLOR_RED ) );                     // Mid
    palette.setColor( QPalette::Dark, QColor( COLOR_RED ) );                    // Dark .. mostly for frame shadow and radio button outline
    palette.setColor( QPalette::NoRole, QColor( COLOR_RED ) );                  // NoRole
    palette.setColor( QPalette::PlaceholderText, QColor( COLOR_RED ) );         // dont know

    // Light some of frame hightlight.. also scroll bar background color
    palette.setColor( QPalette::Light, isBlackOnWhite() ? getColor( eShadowColor ).lighter() : getColor( eShadowColor ).darker() );                 
    palette.setColor( QPalette::Midlight, isBlackOnWhite() ? getColor( eShadowColor ).lighter() : getColor( eShadowColor ).darker() );                // Midlight.. mostly for 
    palette.setColor( QPalette::Mid, getColor( eShadowColor ) );                     // Mid
     // Dark .. combo box upper frame line and spin buttons outline and scroll buttons bottom line
    palette.setColor( QPalette::Dark, isBlackOnWhite() ? getColor( eShadowColor ).lighter() : getColor( eShadowColor ).darker() );                   
    palette.setColor( QPalette::NoRole, QColor( COLOR_RED ) );                  // NoRole
    palette.setColor( QPalette::PlaceholderText, QColor( COLOR_RED ) );         // dont know


/*
    palette.setColor( QPalette::Shadow, QColor( COLOR_RED ) );     // ToolTipBase
    palette.setColor( QPalette::AlternateBase, QColor( COLOR_RED ) );                 // ToolTipText
    palette.setColor( QPalette::PlaceholderText, QColor( COLOR_PURPLE ) );      // LinkVisited

    QColor bronze( 207, 155, 95 );
    QColor veryLightBlue( 239, 239, 247 );
    QColor lightBlue( 223, 223, 239 );
    QColor darkBlue( 95, 95, 191 );
    palette.setColor( QPalette::BrightText, Qt::white );
    palette.setColor( QPalette::Base, veryLightBlue );
    palette.setColor( QPalette::AlternateBase, lightBlue );
    palette.setColor( QPalette::Highlight, darkBlue );
    palette.setColor( QPalette::Disabled, QPalette::Highlight,
        Qt::darkGray );

    palette.setColor( QPalette::ToolTipBase, lightBlue );
    palette.setColor( QPalette::WindowText, lightBlue );
    palette.setColor( QPalette::Light, QColor( COLOR_RED ) );
    palette.setColor( QPalette::Midlight, QColor( COLOR_RED ) );
    palette.setColor( QPalette::Dark, QColor( COLOR_RED ) );
    palette.setColor( QPalette::Mid, QColor( COLOR_RED ) );
    palette.setColor( QPalette::Text, QColor( COLOR_RED ) );
    palette.setColor( QPalette::Highlight, QColor( COLOR_RED ) );
    palette.setColor( QPalette::HighlightedText, QColor( COLOR_RED ) );
    */

    //m_ThemeColors.push_back( QColor( COLOR_RED ) ); //eShadowColor,
    //m_ThemeColors.push_back( QColor( COLOR_RED ) ); //ePlaceholderText,
    //m_ThemeColors.push_back( QColor( COLOR_RED ) ); //eAlternateBase,


  //palette.setColor( QPalette::NColorRoles, QColor( COLOR_RED ) );
 /*   enum ColorRole {
        WindowText, Button, Light, Midlight, Dark, Mid,
        Text, BrightText, ButtonText, Base, Window, Shadow,
        Highlight, HighlightedText,
        Link, LinkVisited,
        AlternateBase,
        NoRole,
        ToolTipBase, ToolTipText,
        NColorRoles = ToolTipText + 1,
        Foreground = WindowText, Background = Window
    };*/


    /*
        QBrush buttonNormalBackgroundBrush = initBrush( getColor( eButtonBackgroundNormal ) );
        QBrush buttonInactiveBackgroundBrush = initBrush( getColor( eButtonBackgroundNormal ) );
        QBrush buttonDisabledBackgroundBrush = initBrush( getColor( eButtonBackgroundDisabled ) );

        QBrush normalTextBrush = initBrush( windowTextColor );
        QBrush disabledTextBrush = initBrush( getColor( eButtonTextDisabled ) );
        QBrush inactiveTextBrush = initBrush( buttonTextColor );
        QBrush highlightTextBrush = initBrush( getColor( eWindowHighlightTextColor ) );

        QBrush uknownBrush = initBrush( QColor( COLOR_RED ) );
        palette.setBrush( QPalette::NColorGroups,   QPalette::Shadow, uknownBrush );
        palette.setBrush( QPalette::Current,        QPalette::Shadow, uknownBrush );
        palette.setBrush( QPalette::All,            QPalette::Shadow, uknownBrush );

        palette.setBrush( QPalette::Active,         QPalette::Button, buttonNormalBackgroundBrush );
        palette.setBrush( QPalette::Inactive,       QPalette::Button, buttonInactiveBackgroundBrush );
        palette.setBrush( QPalette::Disabled,       QPalette::Button, buttonDisabledBackgroundBrush );

        palette.setBrush( QPalette::Active,         QPalette::Text, normalTextBrush );
        palette.setBrush( QPalette::Inactive,       QPalette::Text, inactiveTextBrush );
        palette.setBrush( QPalette::Disabled,       QPalette::Text, disabledTextBrush );

        palette.setBrush( QPalette::Active,         QPalette::HighlightedText, highlightTextBrush );
        palette.setBrush( QPalette::Inactive,       QPalette::HighlightedText, highlightTextBrush );
        palette.setBrush( QPalette::Disabled,       QPalette::HighlightedText, disabledTextBrush );
    */
	// for test only
    // QColor fgdColor( 128, 0, 0, 255 );
    // QColor bgdColor( 0, 0, 128, 255 );
    // setEveryColorPossible( palette, fgdColor, bgdColor );

	// save standard pallette
    m_BasePalette = palette;
    m_MyApp.getHomePage().setPalette( palette );
}

//============================================================================
void VxAppTheme::selectTheme( EThemeType eThemeType )
{
    LogMsg( LOG_DEBUG, "setupTheme %s ", describeTheme( eThemeType ).toUtf8().constData() );
	m_ThemeColors.clear();

	switch( eThemeType )
	{
	case eThemeTypeDark:
        m_ContrastType = eContrastTypeWhiteOnBlack;
		m_ThemeColors.push_back( QColor( COLOR_ORANGE ) ); //eFocusRect,
		m_ThemeColors.push_back( QColor( COLOR_BLACK ) ); //eWindowBackground,
		m_ThemeColors.push_back( QColor( COLOR_GREY_VERY_LIGHT ) ); //eWindowTextColor,
		m_ThemeColors.push_back( QColor( COLOR_WHITE_CREAM ) ); //eWindowHighlightTextColor,
		m_ThemeColors.push_back( QColor( COLOR_GREY_MEDIUM ) ); //eTitleBarBackground,
		m_ThemeColors.push_back( QColor( COLOR_WHITE_CREAM ) ); //eTitleBarTextText,

		m_ThemeColors.push_back( QColor( COLOR_GREY_DARK ).darker() ); //eButtonBackgroundNormal,
		m_ThemeColors.push_back( QColor( COLOR_GREY_MEDIUM ) ); //eButtonBackgroundDisabled,
		m_ThemeColors.push_back( QColor( COLOR_WHITE ) ); //eButtonBackgroundPressed,

		m_ThemeColors.push_back( QColor( COLOR_SILVER ) ); //eButtonForegroundNormal,
		m_ThemeColors.push_back( QColor( COLOR_GREY_MEDIUM ) ); //eButtonForegroundDisabled,
		m_ThemeColors.push_back( QColor( COLOR_ORANGE_BURNT ) ); //eButtonForegroundPressed,

		m_ThemeColors.push_back( QColor( COLOR_GREEN_LIGHT ) ); //eButtonTextNormal,
		m_ThemeColors.push_back( QColor( COLOR_GREEN_MEDIUM ).darker() ); //eButtonTextDisabled,
		m_ThemeColors.push_back( QColor( COLOR_WHITE ).darker() ); //eButtonTextPressed,

		m_ThemeColors.push_back( QColor( COLOR_ORANGE_BURNT ) ); //eLayerOverlayColor,
        m_ThemeColors.push_back( QColor( COLOR_GREEN ) ); //eLayerNotifyOnlineColor, 
        m_ThemeColors.push_back( QColor( COLOR_GREY_MEDIUM ) ); //eLayerNotifyOfflineColor, 
        m_ThemeColors.push_back( QColor( COLOR_GREEN ) ); //eLayerNotifyOfferColor, 
        m_ThemeColors.push_back( QColor( COLOR_PINK_LIGHT ) ); //eLayerNotifyForbiddenColor, 
        m_ThemeColors.push_back( QColor( COLOR_RED ) ); //eLayerNotifyErrorColor, 

		m_ThemeColors.push_back( QColor( COLOR_ORANGE_BURNT ) ); //eProgressBarColor,
        m_ThemeColors.push_back( QColor( COLOR_GREY_VERY_LIGHT ) ); //eProgressBarBackgroundColor,

        m_ThemeColors.push_back( QColor( COLOR_GREEN_DARK ) ); //eShadowColor,
        m_ThemeColors.push_back( QColor( COLOR_RED ) ); //ePlaceholderText,
        m_ThemeColors.push_back( QColor( COLOR_PINK_LIGHT ) ); //eAlternateBase,
        break;

	case eThemeTypeBlueOnWhite:
        m_ContrastType = eContrastTypeBlackOnWhite;
		m_ThemeColors.push_back( QColor( COLOR_ORANGE_BURNT ) ); //eFocusRect,
		m_ThemeColors.push_back( QColor( COLOR_WHITE ) ); //eWindowBackground,
		m_ThemeColors.push_back( QColor( COLOR_BLUE_MEDIUM ) ); //eWindowTextColor,
		m_ThemeColors.push_back( QColor( COLOR_WHITE ) ); //eWindowHighlightTextColor,
		m_ThemeColors.push_back( QColor( COLOR_WHITE_CREAM ) ); //eTitleBarBackground,
		m_ThemeColors.push_back( QColor( COLOR_BLUE ) ); //eTitleBarTextText,

		m_ThemeColors.push_back( QColor( COLOR_WHITE_CREAM ) ); //eButtonBackgroundNormal,
		m_ThemeColors.push_back( QColor( COLOR_GREY_MEDIUM ) ); //eButtonBackgroundDisabled,
		m_ThemeColors.push_back( QColor( COLOR_BLUE ) ); //eButtonBackgroundPressed,

		m_ThemeColors.push_back( QColor( COLOR_BLUE ) ); //eButtonForegroundNormal,
		m_ThemeColors.push_back( QColor( COLOR_GREY_MEDIUM ) ); //eButtonForegroundDisabled,
		m_ThemeColors.push_back( QColor( COLOR_WHITE ) ); //eButtonForegroundPressed,

		m_ThemeColors.push_back( QColor( COLOR_BLUE ) ); //eButtonTextNormal,
		m_ThemeColors.push_back( QColor( COLOR_SILVER ) ); //eButtonTextDisabled,
		m_ThemeColors.push_back( QColor( COLOR_WHITE ) ); //eButtonTextPressed,

		m_ThemeColors.push_back( QColor( COLOR_ORANGE_BURNT ) ); //eLayerOverlayColor,
        m_ThemeColors.push_back( QColor( COLOR_GREEN ) ); //eLayerNotifyOnlineColor, 
        m_ThemeColors.push_back( QColor( COLOR_GREY_MEDIUM ) ); //eLayerNotifyOfflineColor, 
        m_ThemeColors.push_back( QColor( COLOR_GREEN ) ); //eLayerNotifyOfferColor, 
        m_ThemeColors.push_back( QColor( COLOR_PINK_LIGHT ) ); //eLayerNotifyForbiddenColor, 
        m_ThemeColors.push_back( QColor( COLOR_RED ) ); //eLayerNotifyErrorColor, 

		m_ThemeColors.push_back( QColor( COLOR_BLUE ) ); //eProgressBarColor,
        m_ThemeColors.push_back( QColor( COLOR_SILVER ) ); //eProgressBarBackgroundColor,

        m_ThemeColors.push_back( QColor( COLOR_BLUE_DARK ) ); //eShadowColor,
        m_ThemeColors.push_back( QColor( COLOR_RED ) ); //ePlaceholderText,
        m_ThemeColors.push_back( QColor( COLOR_PINK_LIGHT ) ); //eAlternateBase,

		break;

	case eThemeTypeGreenOnWhite:
        m_ContrastType = eContrastTypeBlackOnWhite;
		m_ThemeColors.push_back( QColor( COLOR_ORANGE_BURNT ) ); //eFocusRect,
		m_ThemeColors.push_back( QColor( COLOR_SEASHELL ) ); //eWindowBackground,
		m_ThemeColors.push_back( QColor( COLOR_GREEN_LIME ) ); //eWindowTextColor,
		m_ThemeColors.push_back( QColor( COLOR_GREEN ) ); //eWindowHighlightTextColor,
		m_ThemeColors.push_back( QColor( COLOR_WHITE_CREAM ) ); //eTitleBarBackground,
		m_ThemeColors.push_back( QColor( COLOR_GREEN ) ); //eTitleBarTextText,

		m_ThemeColors.push_back( QColor( COLOR_WHITE ) ); //eButtonBackgroundNormal,
		m_ThemeColors.push_back( QColor( COLOR_GREY_MEDIUM ) ); //eButtonBackgroundDisabled,
		m_ThemeColors.push_back( QColor( COLOR_GREEN_LIME ) ); //eButtonBackgroundPressed,

		m_ThemeColors.push_back( QColor( COLOR_GREEN ) ); //eButtonForegroundNormal,
		m_ThemeColors.push_back( QColor( COLOR_GREY_MEDIUM ) ); //eButtonForegroundDisabled,
		m_ThemeColors.push_back( QColor( COLOR_WHITE ) ); //eButtonForegroundPressed,

		m_ThemeColors.push_back( QColor( COLOR_GREEN ) ); //eButtonTextNormal,
		m_ThemeColors.push_back( QColor( COLOR_SILVER ) ); //eButtonTextDisabled,
		m_ThemeColors.push_back( QColor( COLOR_WHITE ) ); //eButtonTextPressed,

		m_ThemeColors.push_back( QColor( COLOR_ORANGE_BURNT ) ); //eLayerOverlayColor,
        m_ThemeColors.push_back( QColor( COLOR_GREEN ) ); //eLayerNotifyOnlineColor, 
        m_ThemeColors.push_back( QColor( COLOR_GREY_MEDIUM ) ); //eLayerNotifyOfflineColor, 
        m_ThemeColors.push_back( QColor( COLOR_GREEN ) ); //eLayerNotifyOfferColor, 
        m_ThemeColors.push_back( QColor( COLOR_PINK_LIGHT ) ); //eLayerNotifyForbiddenColor, 
        m_ThemeColors.push_back( QColor( COLOR_RED ) ); //eLayerNotifyErrorColor, 

		m_ThemeColors.push_back( QColor( COLOR_ORANGE ) ); //eProgressBarColor,
        m_ThemeColors.push_back( QColor( COLOR_SILVER ) ); //eProgressBarBackgroundColor,

        m_ThemeColors.push_back( QColor( COLOR_GREEN_MEDIUM ) ); //eShadowColor,
        m_ThemeColors.push_back( QColor( COLOR_RED ) ); //ePlaceholderText,
        m_ThemeColors.push_back( QColor( COLOR_PINK_LIGHT ) ); //eAlternateBase,
        break;

	case eThemeTypeUnknown:
	case eThemeTypeLight:
	default:
        m_ContrastType = eContrastTypeBlackOnWhite;
		m_ThemeColors.push_back( QColor( COLOR_ORANGE_BURNT ) ); //eFocusRect,
		m_ThemeColors.push_back( QColor( COLOR_WHITE ) ); //eWindowBackground,
		m_ThemeColors.push_back( QColor( COLOR_BLACK ) ); //eWindowTextColor,
		m_ThemeColors.push_back( QColor( COLOR_ORANGE_BURNT ) ); //eWindowHighlightTextColor,
		m_ThemeColors.push_back( QColor( COLOR_WHITE_CREAM ) ); //eTitleBarBackground,
		m_ThemeColors.push_back( QColor( COLOR_BLACK ) ); //eTitleBarTextText,

		m_ThemeColors.push_back( QColor( COLOR_WHITE ) ); //eButtonBackgroundNormal,
		m_ThemeColors.push_back( QColor( COLOR_GREY_MEDIUM ) ); //eButtonBackgroundDisabled,
		m_ThemeColors.push_back( QColor( COLOR_BLACK ) ); //eButtonBackgroundPressed,

		m_ThemeColors.push_back( QColor( COLOR_BLACK ) ); //eButtonForegroundNormal,
		m_ThemeColors.push_back( QColor( COLOR_GREY_MEDIUM ) ); //eButtonForegroundDisabled,
		m_ThemeColors.push_back( QColor( COLOR_WHITE ) ); //eButtonForegroundPressed,

		m_ThemeColors.push_back( QColor( COLOR_BLACK ) ); //eButtonTextNormal,
		m_ThemeColors.push_back( QColor( COLOR_SILVER ) ); //eButtonTextDisabled,
		m_ThemeColors.push_back( QColor( COLOR_WHITE ) ); //eButtonTextPressed,

		m_ThemeColors.push_back( QColor( COLOR_ORANGE_BURNT ) ); //eLayerOverlayColor,
        m_ThemeColors.push_back( QColor( COLOR_GREEN ) ); //eLayerNotifyOnlineColor, 
        m_ThemeColors.push_back( QColor( COLOR_GREY_MEDIUM ) ); //eLayerNotifyOfflineColor, 
        m_ThemeColors.push_back( QColor( COLOR_GREEN ) ); //eLayerNotifyOfferColor, 
        m_ThemeColors.push_back( QColor( COLOR_PINK_LIGHT ) ); //eLayerNotifyForbiddenColor, 
        m_ThemeColors.push_back( QColor( COLOR_RED ) ); //eLayerNotifyErrorColor, 

		m_ThemeColors.push_back( QColor( COLOR_ORANGE ) ); //eProgressBarColor,
        m_ThemeColors.push_back( QColor( COLOR_SILVER ) ); //eProgressBarBackgroundColor,

        m_ThemeColors.push_back( QColor( COLOR_GREEN_MEDIUM ) ); //eShadowColor,
        m_ThemeColors.push_back( QColor( COLOR_RED ) ); //ePlaceholderText,
        m_ThemeColors.push_back( QColor( COLOR_PINK_LIGHT ) ); //eAlternateBase,
        break;
	}

    updateWindowTheme();
}
