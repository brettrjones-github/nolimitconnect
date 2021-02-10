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
// http://www.nolimitconnect.com
//============================================================================

#include "config_gotvapps.h"

#include "AppDefs.h"
#include <QtSvg/QSvgRenderer>
#include <QPixmap>
#include <QIcon>
#include <QVector>
#include <QColor>
#include <QWidget>
#include <QPalette>

#define COLOR_TRANSPARENT						0,0,0,0
#define COLOR_BLACK								0,0,0,255

#define COLOR_BLUE								0,0,255,255
#define COLOR_BLUE_DARK						    1,108,180,255		// 
#define COLOR_BLUE_LIGHT						1,102,254,255		// 
#define COLOR_BLUE_MEDIUM						0,195,203,255		// 
#define COLOR_BLUE_VERY_LIGHT					5,152,254,255

#define COLOR_GREEN								0,255,0,255
#define COLOR_GREEN_DARK						66,166,42,255		// 
#define COLOR_GREEN_MEDIUM						0,180,0,255
#define COLOR_GREEN_LIGHT						66,210,0,255
#define COLOR_GREEN_LIME						67,200,56,255

#define COLOR_GOLDENROD							218,165,32,255		// 

#define COLOR_GREY_DISABLED						217,220,220,255		// 
#define COLOR_GREY_VERY_LIGHT					180,180,180,255	
#define COLOR_GREY_LIGHT						160,160,160,255		
#define COLOR_GREY_MEDIUM						128,128,128,255		// 
#define COLOR_GREY_DARK							80,80,80,255		// 
#define COLOR_GREY_VERY_DARK					64,64,64,255		// 

#define COLOR_LAVENDER							230,230,250,255

#define COLOR_ORANGE							255,128,0,255
#define COLOR_ORANGE_BURNT						191,87,0,255		// 
#define COLOR_ORANGE_LIGHT						254,216,177,255		// 
#define COLOR_ORANGE_MEDIUM						127,108,88,255		// 
#define COLOR_ORANGE_DARK						63,54,44,255		// 

#define COLOR_PINK_LIGHT						254,195,203,255		//

#define COLOR_PURPLE							128,0,128,255
#define COLOR_PURPLE_DARK						0,0,153,255			// 

#define COLOR_RED								255,0,0,255
#define COLOR_RED_DARK							228,1,29,255		//  

#define COLOR_SEASHELL							255,245,238,255		// 
#define COLOR_SILVER							192,192,192,255		// 

#define COLOR_TAN_LIGHT							255,192,129,255		// 
#define COLOR_TAN_DARK							205,116,54,255		// 
#define COLOR_TURQUOISE_LIGHT					130,226,217,255		// 
#define COLOR_TURQUOISE_DARK					80,176,167,255		// 

#define COLOR_WHITE								255,255,255,255
#define COLOR_WHITE_ALMOST						240,240,240,255
#define COLOR_WHITE_CREAM						243,243,243,255		// 

#define COLOR_YELLOW							255,255,0,255
#define COLOR_YELLOW_LIGHT						255,235,121,255		// 
#define COLOR_YELLOW_MEDIUM						255,215,0,255		// 
#define COLOR_YELLOW_DARK						189,182,107,255		// 

#define COLOR_UNKNOWN							254,153,254,254			// pink color to indicate has not been initialized
/*
enum ColorRole {
    WindowText, Button, Light, Midlight, Dark, Mid,
    Text, BrightText, ButtonText, Base, Window, Shadow,
    Highlight, HighlightedText,
    Link, LinkVisited,
    AlternateBase,
    NoRole,
    ToolTipBase, ToolTipText,
    PlaceholderText,*/

enum EThemeColorRole
{
	eFocusRect = 0,					

	eWindowBackground,
	eWindowTextColor,
	eWindowHighlightTextColor,

	eTitleBarBackground,
	eTitleBarTextText,

	// note: use button colors for icons also
	eButtonBackgroundNormal,
	eButtonBackgroundDisabled,
	eButtonBackgroundPressed,

	eButtonForegroundNormal,
	eButtonForegroundDisabled,
	eButtonForegroundPressed,

	eButtonTextNormal,
	eButtonTextDisabled,
	eButtonTextPressed,

	eLayerOverlayColor,
	eLayerNotifyColor, 
	
	eProgressBarColor,

    eShadowColor,
    ePlaceholderText,
    eAlternateBase,


	MaxThemeColorRole
};

enum EColorLayerType
{
	eColorLayerUnknown = 0,
	eColorLayerBackground,
	eColorLayerForeground,
	eColorLayerIcon,
	eColorLayerText,
	eColorLayerOverlay,
	eColorLayerNotify,

	eColorLayerCount // must be last
};

enum EContrastType
{
    eContrastTypeUnknown = 0,
    eContrastTypeBlackOnWhite,
    eContrastTypeWhiteOnBlack,

    eMaxContrastType // must be last
};

class AppCommon;
class VxFlatButton;
class VxProgressBar;
class QProgressBar;

class QWidget;
class QPalette;
class QBrush;
class QFrame;
class QLabel;
class QStyleOption;
class QPushButton;

class VxAppTheme : public QWidget
{
	Q_OBJECT
public:
	VxAppTheme( AppCommon& appCommon );

    virtual void                applyTheme( QWidget * widget );

	virtual void				selectTheme( EThemeType eThemeType );
	virtual QString				describeTheme( EThemeType eThemeType );
    virtual bool				isBlackOnWhite( void ) { return eContrastTypeBlackOnWhite == m_ContrastType; }
    virtual bool				isWhiteOnBlack( void ) { return eContrastTypeWhiteOnBlack == m_ContrastType; }

	virtual void				updateWindowTheme( void );
	virtual void				updateButton( QPushButton * button );
	virtual void				updateProgressBar( QProgressBar * progressBar );

	// draw background of button
	virtual	void				drawButton( QPushButton * button, QPainter& painter );
	// get button color for button layer depending on state ( pressed, hasFocus, etc. ) 
	virtual	QColor				getButtonColor( QPushButton * button, EColorLayerType colorLayer );
	// draw focus rectangle around widget
	virtual	void				drawFocusRect( QPainter& painter, const QRect& focusRect, int lineWidth );

	// render a svg resource into QPixmap of given size and color
	virtual QPixmap				svgRenderer( QString svgName, QSize imageSize, QColor iconColor );
	// render a svg resource into QPixmap of given size
	virtual QPixmap				svgRenderer( QString svgName, QSize imageSize );
	// draw a pixmap of equal width and height scaled to size of widget
	virtual void				drawBitmapOnWidget( QPainter& painter, QPixmap& widgetPixmap, QSize sizeOfWidget );
	// replace all non transparent pixels with newColor
	QPixmap						replaceImageColor( QPixmap srcImageIn, QColor newColor );

	virtual QPalette&			getBasePalette( void )						{ return m_BasePalette; }
	virtual QColor				getColor( EThemeColorRole colorRole );
	virtual bool				isValidColorRole( EThemeColorRole colorRole );
	// get color value as text suitable for Qt style sheets
	virtual QString				getColorValueAsText( const QColor& colorVal );
	// return true if colors are exactly equal ( including alpha channel )
	virtual bool				areColorsEqual( const QColor& colorVal1, const QColor& colorVal2 );

	virtual QColor&				getTransparentColor( void )					{ return m_TransparentColor; }
	virtual bool				isTransparentColor( const QColor& colorVal );
 
	// given a resource name and extension.. get the full path to that resource
	virtual QString				getResourceFullFileName( QString resourceName );

	// set highlight color ( mainly progress bar color )
	virtual void				setHighlightColor( QWidget* widget, const QColor highlightColor );
	// set background color of widget
	virtual void				setBackgroundColor( QWidget * widget, const QColor backgroundColor );
	// set background color of frame
	virtual void 				setBackgroundColor( QFrame * widget, const QColor backgroundColor );
	// set background color to transparent
	virtual void 				setWidgetBackgroundToTransparent( QWidget * widget );
	virtual void				setTextColor( const QColor& color, QWidget* widget );

	virtual void				setPainterPen( QPainter& painter, QColor penColor );
	virtual void				setPainterBrush( QPainter& painter, QColor brushColor );

	virtual QBrush				initBrush( const QColor& color );

    // set every color role for testing
    static void				    setEveryColorPossible( QPalette& palette, const QColor& bkgColor, const QColor& fgdColor );

protected:
	void						setWidgetPallette( QWidget* widget, QPalette & palette );
	QPalette					getWidgetPalette( QWidget* widget );

	AppCommon&					m_MyApp;
	QColor						m_TransparentColor;
	QColor						m_InvalidColor;

	QPalette					m_BasePalette;
	QSvgRenderer				m_SvgRenderer;
	QVector<QColor>				m_ThemeColors;
    EContrastType               m_ContrastType = eContrastTypeBlackOnWhite;
};
