
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
// http://www.gotvptop.com
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
, m_InvalidColor( COLOR_LIGHT_PINK )
, m_SvgRenderer( this )
{
}

//============================================================================
void VxAppTheme::updateWindowTheme( void )
{
	QColor windowBkgColor( getColor( eWindowBackground ) );
	QColor windowTextColor( getColor( eWindowTextColor ) );

	//QColor buttonBkgColor( getColor( eButtonBackgroundNormal ) );
    QColor buttonBkgColor( QColor( COLOR_RED ) );
	QColor buttonTextColor( getColor( eButtonTextNormal ) );

	QPalette palette( buttonBkgColor, windowBkgColor );
	palette.setColor( QPalette::Base,  getColor( eButtonBackgroundNormal ) ); // Base drop down combo box background
	// palette.setColor( QPalette::Background, QColor( COLOR_RED ) );          // Background and Window
    // palette.setColor( QPalette::Foreground, QColor( COLOR_GREEN ) );        // Foreground and WindowText
	palette.setColor( QPalette::AlternateBase, QColor( COLOR_LIGHT_PINK ) );// AlternateBase
    palette.setColor( QPalette::Light, QColor( COLOR_LIGHT_PINK ) );        // Light
    palette.setColor( QPalette::Midlight, QColor( COLOR_LIGHT_PINK ) );     // Midlight
    palette.setColor( QPalette::Dark, QColor( COLOR_LIGHT_PINK ) );         // Dark
    palette.setColor( QPalette::Mid, QColor( COLOR_LIGHT_PINK ) );          // Mid

	palette.setColor( QPalette::Button, QColor( COLOR_DARK_GREY ) );      // Button

    palette.setColor( QPalette::ButtonText, QColor( COLOR_GREEN ) );  // ButtonText and drop down button icon color
	palette.setColor( QPalette::BrightText, QColor( COLOR_RED ) );  // BrightText
	palette.setColor( QPalette::WindowText, windowTextColor );  // WindowText
	palette.setColor( QPalette::Text, windowTextColor );        // Text and foreground
	palette.setColor( QPalette::Window, windowBkgColor );       // Window and background

    palette.setColor( QPalette::NoRole, QColor( COLOR_RED ) );  // NoRole
    palette.setColor( QPalette::Shadow, QColor( COLOR_GREEN ) );  // shadow

    palette.setColor( QPalette::Highlight, getColor( eProgressBarColor ) );		// Highlight and progress bar
    palette.setColor( QPalette::HighlightedText, getColor( eWindowHighlightTextColor ) );	// HighlightedText
    palette.setColor( QPalette::Link, QColor( COLOR_BLUE ) );               // Link
    palette.setColor( QPalette::LinkVisited, QColor( COLOR_PURPLE ) );      // LinkVisited
    palette.setColor( QPalette::ToolTipText, windowTextColor );                 // ToolTipText
    palette.setColor( QPalette::ToolTipBase, QColor( COLOR_DARK_YELLOW ) );     // ToolTipBase

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

	m_BasePalette = palette;
	m_MyApp.getHomePage().setPalette( m_BasePalette );
}

//============================================================================
void VxAppTheme::updateButton( QPushButton * button )
{

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
	QRect drawAreaRect( BUTTON_BKG_PADDING, BUTTON_BKG_PADDING, button->width() - ( BUTTON_BKG_PADDING * 2 ), button->height() - ( BUTTON_BKG_PADDING * 2 ) );

	painter.setRenderHint( QPainter::Antialiasing );

	QColor bkgColor( getButtonColor( button, eColorLayerBackground ) );
	QColor fgdColor( getButtonColor( button, eColorLayerForeground ) );

	painter.setPen( QPen( fgdColor, BUTTON_BORDER_WIDTH ) );
	painter.setBrush( initBrush( bkgColor ) );
	painter.drawRoundedRect( drawAreaRect, BUTTON_ROUNDING_RADIUS, BUTTON_ROUNDING_RADIUS );
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
		case eColorLayerNotify:
			return getColor( eLayerNotifyColor );

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
	widgetPalette.setColor( QPalette::Background, backgroundColor );
	widgetPalette.setColor( QPalette::Window, backgroundColor );
	widget->setPalette( widgetPalette );

	//QString styleSheet = QString( "QWidget { background-color %1; }" ).arg( getColorValueAsText( backgroundColor ) );
	//widget->setStyleSheet( styleSheet );
}

//============================================================================
void VxAppTheme::setBackgroundColor( QFrame * frame, const QColor backgroundColor )
{
	QPalette framePalette = frame->palette();
	framePalette.setColor( QPalette::Base, backgroundColor );
	framePalette.setColor( QPalette::Background, backgroundColor );
	framePalette.setColor( QPalette::Window, backgroundColor );
	frame->setPalette( framePalette );

//	QString styleSheet = QString( "QFrame { background-color %1; }" ).arg( getColorValueAsText( backgroundColor ) );
//	frame->setStyleSheet( styleSheet );
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
	QPalette palette;
	QBrush brush;

	brush = initBrush( color );
	palette = getWidgetPalette( widget );
	palette.setBrush( QPalette::Active, QPalette::Text, brush );
	palette.setBrush( QPalette::Inactive, QPalette::Text, brush );
	palette.setBrush( QPalette::Disabled, QPalette::Text, brush );
	palette.setBrush( QPalette::Active, QPalette::HighlightedText, brush );
	palette.setBrush( QPalette::Inactive, QPalette::HighlightedText, brush );
	palette.setBrush( QPalette::Disabled, QPalette::HighlightedText, brush );
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
void VxAppTheme::selectTheme( EThemeType eThemeType )
{
    LogMsg( LOG_DEBUG, "setupTheme %s ", describeTheme( eThemeType ).toUtf8().constData() );
	m_ThemeColors.clear();

	switch( eThemeType )
	{
	case eThemeTypeDark:
		m_ThemeColors.push_back( QColor( COLOR_BURNT_ORANGE ) ); //eFocusRect,
		m_ThemeColors.push_back( QColor( COLOR_BLACK ) ); //eWindowBackground,
		m_ThemeColors.push_back( QColor( COLOR_WHITE ) ); //eWindowTextColor,

		m_ThemeColors.push_back( QColor( COLOR_CREAM_WHITE ) ); //eWindowHighlightTextColor,
		m_ThemeColors.push_back( QColor( COLOR_MEDIUM_GREY ) ); //eTitleBarBackground,
		m_ThemeColors.push_back( QColor( COLOR_CREAM_WHITE ) ); //eTitleBarTextText,

		m_ThemeColors.push_back( QColor( COLOR_DARK_GREY ) ); //eButtonBackgroundNormal,
		m_ThemeColors.push_back( QColor( COLOR_MEDIUM_GREY ) ); //eButtonBackgroundDisabled,
		m_ThemeColors.push_back( QColor( COLOR_WHITE ) ); //eButtonBackgroundPressed,

		m_ThemeColors.push_back( QColor( COLOR_SILVER ) ); //eButtonForegroundNormal,
		m_ThemeColors.push_back( QColor( COLOR_MEDIUM_GREY ) ); //eButtonForegroundDisabled,
		m_ThemeColors.push_back( QColor( COLOR_BURNT_ORANGE ) ); //eButtonForegroundPressed,

		m_ThemeColors.push_back( QColor( COLOR_CREAM_WHITE ) ); //eButtonTextNormal,
		m_ThemeColors.push_back( QColor( COLOR_SILVER ) ); //eButtonTextDisabled,
		m_ThemeColors.push_back( QColor( COLOR_WHITE ) ); //eButtonTextPressed,

		m_ThemeColors.push_back( QColor( COLOR_ORANGE ) ); //eLayerOverlayColor,
		m_ThemeColors.push_back( QColor( COLOR_RED ) ); //eLayerNotifyColor,

		m_ThemeColors.push_back( QColor( COLOR_ORANGE ) ); //eProgressBarColor,
		break;

	case eThemeTypeBlueOnWhite:
		m_ThemeColors.push_back( QColor( COLOR_BURNT_ORANGE ) ); //eFocusRect,
		m_ThemeColors.push_back( QColor( COLOR_WHITE ) ); //eWindowBackground,
		m_ThemeColors.push_back( QColor( COLOR_MEDIUM_BLUE ) ); //eWindowTextColor,
		m_ThemeColors.push_back( QColor( COLOR_BLUE ) ); //eWindowHighlightTextColor,
		m_ThemeColors.push_back( QColor( COLOR_CREAM_WHITE ) ); //eTitleBarBackground,
		m_ThemeColors.push_back( QColor( COLOR_BLUE ) ); //eTitleBarTextText,

		m_ThemeColors.push_back( QColor( COLOR_CREAM_WHITE ) ); //eButtonBackgroundNormal,
		m_ThemeColors.push_back( QColor( COLOR_MEDIUM_GREY ) ); //eButtonBackgroundDisabled,
		m_ThemeColors.push_back( QColor( COLOR_BLUE ) ); //eButtonBackgroundPressed,

		m_ThemeColors.push_back( QColor( COLOR_BLUE ) ); //eButtonForegroundNormal,
		m_ThemeColors.push_back( QColor( COLOR_MEDIUM_GREY ) ); //eButtonForegroundDisabled,
		m_ThemeColors.push_back( QColor( COLOR_WHITE ) ); //eButtonForegroundPressed,

		m_ThemeColors.push_back( QColor( COLOR_BLUE ) ); //eButtonTextNormal,
		m_ThemeColors.push_back( QColor( COLOR_SILVER ) ); //eButtonTextDisabled,
		m_ThemeColors.push_back( QColor( COLOR_WHITE ) ); //eButtonTextPressed,

		m_ThemeColors.push_back( QColor( COLOR_BURNT_ORANGE ) ); //eLayerOverlayColor,
		m_ThemeColors.push_back( QColor( COLOR_RED ) ); //eLayerNotifyColor,

		m_ThemeColors.push_back( QColor( COLOR_BLUE ) ); //eProgressBarColor,
		break;

	case eThemeTypeGreenOnWhite:
		m_ThemeColors.push_back( QColor( COLOR_BURNT_ORANGE ) ); //eFocusRect,
		m_ThemeColors.push_back( QColor( COLOR_SEASHELL ) ); //eWindowBackground,
		m_ThemeColors.push_back( QColor( COLOR_LIME_GREEN ) ); //eWindowTextColor,
		m_ThemeColors.push_back( QColor( COLOR_GREEN ) ); //eWindowHighlightTextColor,
		m_ThemeColors.push_back( QColor( COLOR_CREAM_WHITE ) ); //eTitleBarBackground,
		m_ThemeColors.push_back( QColor( COLOR_GREEN ) ); //eTitleBarTextText,

		m_ThemeColors.push_back( QColor( COLOR_WHITE ) ); //eButtonBackgroundNormal,
		m_ThemeColors.push_back( QColor( COLOR_MEDIUM_GREY ) ); //eButtonBackgroundDisabled,
		m_ThemeColors.push_back( QColor( COLOR_LIME_GREEN ) ); //eButtonBackgroundPressed,

		m_ThemeColors.push_back( QColor( COLOR_GREEN ) ); //eButtonForegroundNormal,
		m_ThemeColors.push_back( QColor( COLOR_MEDIUM_GREY ) ); //eButtonForegroundDisabled,
		m_ThemeColors.push_back( QColor( COLOR_WHITE ) ); //eButtonForegroundPressed,

		m_ThemeColors.push_back( QColor( COLOR_GREEN ) ); //eButtonTextNormal,
		m_ThemeColors.push_back( QColor( COLOR_SILVER ) ); //eButtonTextDisabled,
		m_ThemeColors.push_back( QColor( COLOR_WHITE ) ); //eButtonTextPressed,

		m_ThemeColors.push_back( QColor( COLOR_BURNT_ORANGE ) ); //eLayerOverlayColor,
		m_ThemeColors.push_back( QColor( COLOR_RED ) ); //eLayerNotifyColor,

		m_ThemeColors.push_back( QColor( COLOR_ORANGE ) ); //eProgressBarColor,
		break;

	case eThemeTypeUnknown:
	case eThemeTypeLight:
	default:
		m_ThemeColors.push_back( QColor( COLOR_BURNT_ORANGE ) ); //eFocusRect,
		m_ThemeColors.push_back( QColor( COLOR_WHITE ) ); //eWindowBackground,
		m_ThemeColors.push_back( QColor( COLOR_BLACK ) ); //eWindowTextColor,
		m_ThemeColors.push_back( QColor( COLOR_BURNT_ORANGE ) ); //eWindowHighlightTextColor,
		m_ThemeColors.push_back( QColor( COLOR_CREAM_WHITE ) ); //eTitleBarBackground,
		m_ThemeColors.push_back( QColor( COLOR_BLACK ) ); //eTitleBarTextText,

		m_ThemeColors.push_back( QColor( COLOR_WHITE ) ); //eButtonBackgroundNormal,
		m_ThemeColors.push_back( QColor( COLOR_MEDIUM_GREY ) ); //eButtonBackgroundDisabled,
		m_ThemeColors.push_back( QColor( COLOR_BLACK ) ); //eButtonBackgroundPressed,

		m_ThemeColors.push_back( QColor( COLOR_BLACK ) ); //eButtonForegroundNormal,
		m_ThemeColors.push_back( QColor( COLOR_MEDIUM_GREY ) ); //eButtonForegroundDisabled,
		m_ThemeColors.push_back( QColor( COLOR_WHITE ) ); //eButtonForegroundPressed,

		m_ThemeColors.push_back( QColor( COLOR_BLACK ) ); //eButtonTextNormal,
		m_ThemeColors.push_back( QColor( COLOR_SILVER ) ); //eButtonTextDisabled,
		m_ThemeColors.push_back( QColor( COLOR_WHITE ) ); //eButtonTextPressed,

		m_ThemeColors.push_back( QColor( COLOR_BURNT_ORANGE ) ); //eLayerOverlayColor,
		m_ThemeColors.push_back( QColor( COLOR_RED ) ); //eLayerNotifyColor,

		m_ThemeColors.push_back( QColor( COLOR_ORANGE ) ); //eProgressBarColor,
		break;
	}

	updateWindowTheme();
}
