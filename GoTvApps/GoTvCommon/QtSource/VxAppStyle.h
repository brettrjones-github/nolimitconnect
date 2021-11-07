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

#include <config_gotvapps.h>

#include <QtGlobal>
#include <QProxyStyle>
#include <QProxyStyle>

class AppCommon;
class VxAppTheme;
class AppFocusFrame;
class QRect;

// QT 6.2.0 had bugs in QCommonStyle.. derive from QProxyStyle instead of QCommonStye
class VxAppStyle : public QProxyStyle
{
public:
	VxAppStyle( AppCommon& appCommon, VxAppTheme& appTheme );
	virtual ~VxAppStyle() override = default;

    enum GuiDrawDirection { eGuiDirUp, eGuiDirDown, eGuiDirLeft, eGuiDirRight };

    static int					focusFrameBoarderWidth( void ); // get line width of focus frame
    static void					clearFocusFrameWidget( void ); // clear focus frame widget so no focus frame is drawn until new widget gains focus

    void						drawFocusFrame( QPainter& painter, const QRect& focusRect ) const;
    void					    drawArrow( QPainter*, const QRect& rect, QColor arrowColor, GuiDrawDirection drawDir ) const;
    void					    drawComboBoxArrow( QPainter*, const QRect& rect, const QPalette& palette, bool enabled ) const;

    void                        drawPrimitive( PrimitiveElement element, const QStyleOption *option,
                                                QPainter *painter, const QWidget *widget ) const override;

    void                        drawControl( ControlElement	element, const QStyleOption* option,
                                                QPainter* painter, const QWidget* widget ) const override;

    virtual void				drawComplexControl( ComplexControl control,
                                                    const QStyleOptionComplex* option, QPainter* painter,
                                                    const QWidget* widget ) const override;

    virtual int					pixelMetric( PixelMetric metric,
                                            const QStyleOption* option, const QWidget* widget ) const override;

    virtual QSize				sizeFromContents( ContentsType type,
                                                    const QStyleOption* option, const QSize& contentsSize,
                                                    const QWidget* widget ) const override;

    virtual int					styleHint( StyleHint hint, const QStyleOption* option,
                                                const QWidget* widget, QStyleHintReturn* returnData = nullptr ) const override;


    virtual void				drawItemPixmap( QPainter* painter, const QRect& rectangle,
                                                int alignment, const QPixmap& pixmap ) const override;

    virtual void				drawItemText( QPainter* painter, const QRect& rectangle,
                                                int alignment, const QPalette& palette, bool enabled,
                                                const QString& text, QPalette::ColorRole textRole ) const override;

    virtual QPixmap				generatedIconPixmap( QIcon::Mode iconMode,
                                                        const QPixmap& pixmap, const QStyleOption* option ) const override;

    virtual SubControl			hitTestComplexControl( ComplexControl control,
                                                        const QStyleOptionComplex* option, const QPoint& position,
                                                        const QWidget* widget ) const override;

    virtual QRect				itemPixmapRect( const QRect& rectangle, int alignment,
                                                const QPixmap& pixmap ) const override;

    virtual QRect				itemTextRect( const QFontMetrics& metrics,
                                                const QRect& rectangle, int alignment, bool enabled,
                                                const QString& text ) const override;

    virtual QPalette			standardPalette() const override;

    virtual QPixmap				standardPixmap( StandardPixmap standardPixmap,
                                                const QStyleOption* option, const QWidget* widget ) const override;

    virtual QRect				subControlRect( ComplexControl control,
                                                const QStyleOptionComplex* option, SubControl subControl,
                                                const QWidget* widget ) const override;

    virtual QRect				subElementRect( SubElement element,
                                                const QStyleOption* option, const QWidget* widget ) const override;

protected:
	virtual bool				event( QEvent* ) override;

private:
	AppCommon&					m_MyApp;
	VxAppTheme&					m_AppTheme;
};

