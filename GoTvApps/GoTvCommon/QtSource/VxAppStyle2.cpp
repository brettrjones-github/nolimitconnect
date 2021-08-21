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
#include <app_precompiled_hdr.h>
#include "VxAppStyle2.h"
#include "AppCommon.h"
#include "VxAppTheme.h"
#include "AppGlobals.h"
#include "GuiParams.h"

#include <QPainter>
#include <QStyleOption>
#include <QPushButton>
#include <QAbstractSpinBox>
#include <QFocusFrame>
#include <QPalette>
#include <QEvent>
#include <QPaintEvent>
#include <QApplication>
#include <QFocusFrame>
#include <QPointer>
#include <QPaintEvent>
#include <QPixmap>
#include <QBitmap>
#include <QListWidget>
#include <QDebug>

#include <qdrawutil.h>

class AppFocusFrame;

namespace
{
    QPointer<AppFocusFrame>		g_FocusFrame;
    const int					g_FocusFrameWidth = 2;
}

//============================================================================
class AppFocusFrame : public QFocusFrame
{
public:
    AppFocusFrame( VxAppStyle2& appStyle, QWidget* parent )
        : QFocusFrame( parent )
        , m_AppStyle( appStyle )
    {
    }


protected:
    virtual void paintEvent( QPaintEvent* ev ) override
    {
        QPainter painter( this );
        painter.save();
        m_AppStyle.drawFocusFrame( painter, ev->rect() );
        painter.restore();
    }

    VxAppStyle2&					m_AppStyle;
};

//============================================================================
VxAppStyle2::VxAppStyle2( AppCommon& appCommon, VxAppTheme& appTheme )
#ifdef TARGET_OS_WINDOWS
    : QProxyStyle()
#else
    : QCommonStyle()
#endif // TARGET_OS_WINDOWS
    , m_MyApp( appCommon )
    , m_AppTheme( appTheme )
{
}

//============================================================================
int VxAppStyle2::focusFrameBoarderWidth()
{
    return g_FocusFrameWidth;
}

//============================================================================
void VxAppStyle2::drawFocusFrame( QPainter& painter, const QRect& focusRect ) const
{
    m_AppTheme.drawFocusRect( painter, focusRect, focusFrameBoarderWidth() );
}

//============================================================================
void VxAppStyle2::drawArrow( QPainter* painter, const QRect& rect, QColor arrowColor, GuiDrawDirection drawDir ) const
{
    QPolygon points( 3 );

    int x = rect.x();
    int y = rect.y();
    int w = rect.width() / 2;
    int h = rect.height() / 2;
    x += ( rect.width() - w ) / 2;
    y += ( rect.height() - h ) / 2;

    if( drawDir == eGuiDirUp )
    {
        points[ 0 ] = QPoint( x, y + h );
        points[ 1 ] = QPoint( x + w, y + h );
        points[ 2 ] = QPoint( x + w / 2, y );
    }
    else if( drawDir == eGuiDirDown )
    {
        points[ 0 ] = QPoint( x, y );
        points[ 1 ] = QPoint( x + w, y );
        points[ 2 ] = QPoint( x + w / 2, y + h );
    }

    painter->save();
    painter->setRenderHint( QPainter::Antialiasing, true );

    painter->setBrush( arrowColor );
    painter->drawPolygon( points );

    painter->restore();
}

//============================================================================
void VxAppStyle2::drawComboBoxArrow( QPainter* painter, const QRect& rect, const QPalette& palette, bool enabled ) const
{
    QColor arrowColor = enabled ? palette.buttonText().color() : palette.mid().color();
    drawArrow( painter, rect, arrowColor, eGuiDirDown );
}

//============================================================================
bool VxAppStyle2::event( QEvent* e )
{
    bool result = QCommonStyle::event( e );

    if( e->type() == QEvent::FocusIn )
    {
        QWidget* focusedWidget = QApplication::focusWidget();

        if( focusedWidget
            && !( focusedWidget->property( "NoFocusFrame" ) == true ) )
        {
            if( true == focusedWidget->property( "GoTvIcon" ) )
            {
                m_MyApp.switchWindowFocus( focusedWidget );
            }
            if( true == focusedWidget->property( "GoTvFrame" ) )
            {
                focusedWidget->nextInFocusChain();
            }
            else
            {
                if( g_FocusFrame == NULL )
                {
                    g_FocusFrame = new AppFocusFrame( *this, focusedWidget );
                }

                g_FocusFrame->setWidget( focusedWidget );
            }
        }
        else
        {
            if( g_FocusFrame )
            {
                g_FocusFrame->setWidget( NULL );
            }
        }
    }
    else if( e->type() == QEvent::FocusOut )
    {
        if( g_FocusFrame )
        {
            g_FocusFrame->setWidget( NULL );
        }
    }

    return result;
}

//============================================================================
void VxAppStyle2::polish( QPalette &palette )
{
    palette.setBrush( QPalette::Button, Qt::red );
}

//============================================================================
void VxAppStyle2::drawControl( ControlElement			element,
                              const QStyleOption*		option,
                              QPainter*				    painter,
                              const QWidget*			widget ) const
{
    painter->save();
    // setting the pen here sets text color for progress bar, comobo boxe
    painter->setPen( m_AppTheme.getColor( eWindowTextColor ) );
    painter->setBrush( m_AppTheme.getColor( eWindowBackground ) );

    painter->setBackground( m_AppTheme.getColor( eWindowBackground ) );
    QPalette pal( option->palette );
    pal.setColor( QPalette::Window, m_AppTheme.getColor( eWindowBackground ) );


    if( element == CE_ScrollBarSubLine || element == CE_ScrollBarAddLine )
    {
        if( ( option->state & State_Sunken ) )
        {
            painter->setPen( option->palette.dark().color() );
            painter->setBrush( option->palette.brush( QPalette::Button ) );
            painter->drawRect( option->rect.adjusted( 0, 0, -1, -1 ) );
        }
        else
        {
            QStyleOption buttonOpt = *option;
            if( !( buttonOpt.state & State_Sunken ) )
            {
                buttonOpt.state |= State_Raised;
            }

            QPalette pal( option->palette );
            pal.setColor( QPalette::Button, option->palette.light().color() );
            pal.setColor( QPalette::Light, option->palette.button().color() );
            qDrawWinButton( painter, option->rect, pal, option->state & ( State_Sunken | State_On ),
                            &option->palette.brush( QPalette::Button ) );
        }

        PrimitiveElement arrow;
        if( option->state & State_Horizontal )
        {
            if( element == CE_ScrollBarAddLine )
            {
                arrow = option->direction == Qt::LeftToRight ? PE_IndicatorArrowRight : PE_IndicatorArrowLeft;
            }
            else
            {
                arrow = option->direction == Qt::LeftToRight ? PE_IndicatorArrowLeft : PE_IndicatorArrowRight;
            }
        }
        else
        {
            if( element == CE_ScrollBarAddLine )
            {
                arrow = PE_IndicatorArrowDown;
            }
            else
            {
                arrow = PE_IndicatorArrowUp;
            }
        }

        QStyleOption arrowOpt = *option;
        arrowOpt.rect = option->rect.adjusted( 2, 2, -2, -2 );
        QCommonStyle::drawPrimitive( arrow, &arrowOpt, painter, widget );
        painter->restore();
        return;
    }

    /*
    if( element == CE_ScrollBarSlider )
    {
        if( !( option->state & State_Enabled ) )
        {
            QBrush br;
            const QBrush paletteBrush = option->palette.brush( QPalette::Light );
            if( paletteBrush.style() == Qt::TexturePattern )
            {
                if( qHasPixmapTexture( paletteBrush ) )
                {
                    br = QBrush( paletteBrush.texture() );
                }
                else
                {
                    br = QBrush( paletteBrush.textureImage() );
                }
            }
            else
            {
                br = QBrush( option->palette.light().color(), Qt::Dense4Pattern );
            }

            painter->setPen( Qt::NoPen );
            painter->setBrush( br );
            painter->setBackgroundMode( Qt::OpaqueMode );
            painter->drawRect( option->rect );
        }
        else
        {
            QStyleOptionButton buttonOpt;
            buttonOpt.QStyleOption::operator=( *option );
            buttonOpt.state = State_Enabled | State_Raised;

            QPalette pal( option->palette );
            pal.setColor( QPalette::Button, option->palette.light().color() );
            pal.setColor( QPalette::Light, option->palette.button().color() );
            qDrawWinButton( painter, option->rect, pal, false, &option->palette.brush( QPalette::Button ) );
        }

        painter->restore();
        return;
    }
    */

    if( element == CE_ScrollBarAddPage || element == CE_ScrollBarSubPage )
    {
        QBrush br;
        QBrush bg = painter->background();
        Qt::BGMode bg_mode = painter->backgroundMode();
        painter->setPen( Qt::NoPen );
        painter->setBackgroundMode( Qt::OpaqueMode ); // otherwise scroll area will be transparent

        if( option->state & State_Sunken )
        {
            br = QBrush( option->palette.shadow().color(), Qt::Dense4Pattern );
            painter->setBackground( option->palette.dark().color() );
            painter->setBrush( br );
        }
        else
        {
            const QBrush paletteBrush = option->palette.brush( QPalette::Light );
            if( paletteBrush.style() == Qt::TexturePattern )
            {
                if( qHasPixmapTexture( paletteBrush ) )
                {
                    br = QBrush( paletteBrush.texture() );
                }
                else
                {
                    br = QBrush( paletteBrush.textureImage() );
                }
            }
            else
            {
                br = QBrush( option->palette.light().color(), Qt::Dense4Pattern );
            }
#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
            painter->setBackground(option->palette.base().color());
#else
            painter->setBackground( option->palette.background().color() );
#endif // QT_VERSION >= QT_VERSION_CHECK(6,0,0)
            painter->setBrush( br );
        }

        painter->drawRect( option->rect );
        painter->setBackground( bg );
        painter->setBackgroundMode( bg_mode );
        painter->restore();
        return;
    }

    if( element == CE_CheckBox )
    {
#ifdef TARGET_OS_WINDOWS
        QProxyStyle::drawControl( element, option, painter, widget );
#else
        QCommonStyle::drawControl( element, option, painter, widget );
#endif // TARGET_OS_WINDOWS

        // draw custom check mark if checked
        if( const QStyleOptionButton * butOpt = qstyleoption_cast<const QStyleOptionButton  *>( option ) )
        {
            QRect checkRect = subElementRect( SE_ItemViewItemCheckIndicator, butOpt, widget );
            QColor bkgColor = m_AppTheme.getColor( eWindowBackground );
            painter->setBrush( bkgColor );
            painter->setPen( m_AppTheme.getColor( eShadowColor ) );

            painter->drawRect( checkRect );

            if( butOpt->state & QStyle::State_On )
            {
                painter->setClipRect( option->rect );

                QRect checkRect = subElementRect( SE_ItemViewItemCheckIndicator, butOpt, widget );

                QPalette palette( option->palette );
                QColor checkColor = widget->isEnabled() ? ( palette.buttonText().color().lighter() )  : palette.mid().color();
                QColor bkgColor = palette.window().color();
                m_MyApp.getMyIcons().drawIcon( eMyIconCheckMark, painter, checkRect, checkColor );

            }
        }

        painter->restore();
        return;
    }

    if( element == CE_ShapedFrame )
    {
        // allow default
    }

    if( element == CE_FocusFrame )
    {
        // do not draw focus frame from within focus frame paint
        return;
    }

    //painter->setBackground( QColor( COLOR_GREEN ) );
    //painter->setPen( QColor( COLOR_BLUE ) );
    //painter->setBrush( QColor( COLOR_RED ) );
    if( element == QStyle::CE_ProgressBar )
    {
        //pal.setColor( QPalette::Window, m_AppTheme.getColor( eWindowBackground ) );
        //m_AppTheme.setEveryColorPossible( const_cast<QPalette&>( option->palette ), QColor( COLOR_PURPLE ), QColor( COLOR_PINK_LIGHT ) );
        //QColor bkgColor = m_AppTheme.getColor( eWindowBackground );
        //painter->setBrush( bkgColor );
        //painter->setPen( m_AppTheme.getColor( eShadowColor ) );
        //painter->drawRect( option->rect );
        //painter->restore();
        painter->setPen( m_AppTheme.getColor( eShadowColor ) );
        painter->setBrush( m_AppTheme.getColor( eWindowBackground ) );
        painter->drawRect( option->rect );
        painter->restore();
        return;


    }
    else if( element == QStyle::CE_ProgressBarLabel )
    {
        // allow default
        // would be nice to draw percentage in middle of bar instead of at end
        //painter->setPen( m_AppTheme.getColor( eShadowColor ) );
        //painter->setBrush( m_AppTheme.getColor( eWindowBackground ) );
    }
    else if( element == QStyle::CE_ProgressBarGroove )
    {
       // painter->drawRect( option->rect );
        //painter->restore();
        //return;

        //pal.setColor( QPalette::Window, m_AppTheme.getColor( eWindowBackground ) );
        //m_AppTheme.setEveryColorPossible( const_cast<QPalette&>( option->palette ), QColor( COLOR_PURPLE ), QColor( COLOR_PINK_LIGHT ) );
        //painter->setBackground( QColor( COLOR_PURPLE ) );
        //painter->setPen( QColor( COLOR_PINK_LIGHT ) );
        //painter->setBrush( QColor( COLOR_PINK_LIGHT ) );
    }
    else if( element == QStyle::CE_ProgressBarContents )
    {
        //pal.setColor( QPalette::Window, m_AppTheme.getColor( eWindowBackground ) );
        //m_AppTheme.setEveryColorPossible( const_cast<QPalette&>(option->palette), QColor( COLOR_PURPLE ), QColor( COLOR_PINK_LIGHT ) );
        //painter->setBackground( QColor( COLOR_PURPLE ) );
        //painter->setPen( QColor( COLOR_PINK_LIGHT ) );
        //painter->setBrush( QColor( COLOR_PINK_LIGHT ) );
        //QPalette pal( option->palette );
        //pal.setColor( QPalette::Button, option->palette.light().color() );
        //pal.setColor( QPalette::Light, option->palette.button().color() );
        //painter->setBackground( QColor( COLOR_PURPLE ) );
        //painter->setPen( QColor( COLOR_PINK_LIGHT ) );
        //painter->setBrush( QColor( COLOR_PINK_LIGHT ) );

        //qDrawWinButton( painter, option->rect, pal, false, &option->palette.brush( QPalette::Button ) );
        //painter->restore();

        //return;
        //painter->save();

        //painter->setPen( m_AppTheme.getColor( eShadowColor ) );
        //painter->setBrush( m_AppTheme.getColor( eWindowBackground ) );

        //painter->drawRect( option->rect );
        //painter->restore();
        //return;

    }
    else if( element == QStyle::CE_PushButtonLabel )
    {
        //if( element->pressed() )
        //// could do something custom to the text if button is disabled
        //painter->setPen( m_AppTheme.getColor( eButtonTextNormal ) );
        //painter->setBrush( m_AppTheme.getColor( eButtonBackgroundNormal ) );

        //painter->drawRect( option->rect );        
        QPalette palette( option->palette );
        painter->save();
        qDrawWinButton( painter, option->rect, palette, false, &option->palette.brush( QPalette::Button ) );
        painter->restore();
    }

    /*
    QStyle::CE_MenuBarItem	20	A menu item in a QMenuBar.
        QStyle::CE_MenuBarEmptyArea	21	The empty area of a QMenuBar.
        QStyle::CE_MenuItem	14	A menu item in a QMenu.
        QStyle::CE_MenuScroller	15	Scrolling areas in a QMenu when the style supports scrolling.
        QStyle::CE_MenuTearoff	18	A menu item representing the tear off section of a QMenu.
        QStyle::CE_MenuEmptyArea	19	The area in a menu without menu items.
        QStyle::CE_MenuHMargin	17	The horizontal extra space on the left / right of a menu.
        QStyle::CE_MenuVMargin	16	The vertical extra space on the top / bottom of a menu.
*/
#ifdef TARGET_OS_WINDOWS
    QProxyStyle::drawControl( element, option, painter, widget );
#else
    QCommonStyle::drawControl( element, option, painter, widget );
#endif // TARGET_OS_WINDOWS
    painter->restore();
}

//============================================================================
QPalette VxAppStyle2::standardPalette() const
{
    return m_AppTheme.getBasePalette(); //QCommonStyle::standardPalette();
}

//============================================================================
int VxAppStyle2::styleHint( StyleHint hint, const QStyleOption* option,
                           const QWidget* widget, QStyleHintReturn* returnData ) const
{

    if( hint == QStyle::SH_FocusFrame_AboveWidget )
    {
        // we do our own focus frame
        return true;
    }

    if( hint == QStyle::SH_ItemView_ShowDecorationSelected )
    {
        //return true;
    }

    if( hint == QStyle::SH_GroupBox_TextLabelColor )
    {
        // Qt does not seem to set the right color for groupbox text
        return  int( m_AppTheme.getColor( eWindowTextColor ).rgba() );
    }

#ifdef TARGET_OS_WINDOWS
    return QProxyStyle::styleHint( hint, option, widget, returnData );
#else
    return QCommonStyle::styleHint( hint, option, widget, returnData );
#endif // TARGET_OS_WINDOWS
}

//============================================================================
void VxAppStyle2::drawComplexControl( ComplexControl				control,
                                     const QStyleOptionComplex*	option,
                                     QPainter*					painter,
                                     const QWidget*				widget ) const
{

    if( control == CC_TitleBar )
    {
        painter->save();
        const QPalette pal = option->palette;

        const QColor color1 = pal.color( QPalette::Inactive, QPalette::Button ).darker( 110 );
        const QColor color2 = pal.color( QPalette::Inactive, QPalette::Light );

        QStyleOptionComplex* opt = (QStyleOptionTitleBar*)option;
        opt->palette.setColor( QPalette::Highlight, color1 );
        opt->palette.setColor( QPalette::Base, color2 );
        opt->palette.setColor( QPalette::HighlightedText, pal.color( QPalette::Text ) );
        opt->palette = pal;
        painter->restore();

        return;
    }

    if( control == CC_ComboBox )
    {
        QStyleOptionComboBox* opt = (QStyleOptionComboBox*)option;

        QStyle::State state = opt->state;
        opt->state &= ~QStyle::State_HasFocus;

        bool drawArrow = opt->subControls & SC_ComboBoxArrow;

        if( drawArrow )
        {
            opt->subControls &= ~SC_ComboBoxArrow;
        }

        QBrush editBrush = opt->palette.brush( QPalette::Base );
        if( ( opt->subControls & SC_ComboBoxFrame ) )
        {
            if( opt->frame )
            {
                QPalette shadePal = opt->palette;
                shadePal.setColor( QPalette::Midlight, shadePal.button().color() );

                qDrawWinPanel( painter, opt->rect, shadePal, true, &editBrush );
            }
            else
            {
                painter->fillRect( opt->rect, editBrush );
            }
        }

        if( opt->subControls & SC_ComboBoxEditField )
        {
            QRect re = proxy()->subControlRect( CC_ComboBox, opt, SC_ComboBoxEditField, widget );
            if( opt->state & State_HasFocus && !opt->editable )
            {
                painter->fillRect( re.x(), re.y(), re.width(), re.height(), opt->palette.brush( QPalette::Highlight ) );
            }

            if( opt->state & State_HasFocus )
            {
                painter->setPen( opt->palette.highlightedText().color() );
                painter->setBackground( opt->palette.highlight() );
            }
            else
            {
                painter->setPen( opt->palette.text().color() );
#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
                painter->setBackground(opt->palette.base().color());
#else
                painter->setBackground(opt->palette.background().color());
#endif // QT_VERSION >= QT_VERSION_CHECK(6,0,0)
            }

            if( opt->state & State_HasFocus && !opt->editable )
            {
                QStyleOptionFocusRect focus;
                focus.QStyleOption::operator=( *opt );
                focus.rect = subElementRect( SE_ComboBoxFocusRect, opt, widget );
                focus.state |= State_FocusAtBorder;
                focus.backgroundColor = opt->palette.highlight().color();
                proxy()->drawPrimitive( PE_FrameFocusRect, &focus, painter, widget );
            }
        }

        opt->state = state;

        if( drawArrow )
        {
            if( widget->isEnabled() )
            {
                const QVariant readOnly = widget->property( "readOnly" );

                if( ( readOnly.type() == QVariant::Bool ) && ( readOnly == true ) )
                {
                    drawArrow = false;
                }
            }
            else
            {
                drawArrow = false;
            }
        }

        if( drawArrow )
        {
            opt->subControls |= SC_ComboBoxArrow;
            const QRect rect = subControlRect( CC_ComboBox, opt, SC_ComboBoxArrow, widget );

            drawComboBoxArrow( painter, rect, opt->palette, widget->isEnabled() );
        }

        return;
    }

    if( control == CC_ScrollBar )
    {
        if( const QStyleOptionSlider *scrollbar = qstyleoption_cast<const QStyleOptionSlider *>( option ) )
        {
            QStyleOptionSlider newScrollbar = *scrollbar;
            State saveFlags = scrollbar->state;

            if( scrollbar->subControls & SC_ScrollBarSubLine )
            {
                newScrollbar.state = saveFlags;
                newScrollbar.rect = subControlRect( control, &newScrollbar, SC_ScrollBarSubLine, widget );
                if( newScrollbar.rect.isValid() )
                {
                    if( !( scrollbar->activeSubControls & SC_ScrollBarSubLine ) )
                    {
                        newScrollbar.state &= ~( State_Sunken | State_MouseOver );
                    }

                    drawControl( CE_ScrollBarSubLine, &newScrollbar, painter, widget );
                }
            }

            if( scrollbar->subControls & SC_ScrollBarAddLine )
            {
                newScrollbar.rect = scrollbar->rect;
                newScrollbar.state = saveFlags;
                newScrollbar.rect = subControlRect( control, &newScrollbar, SC_ScrollBarAddLine, widget );
                if( newScrollbar.rect.isValid() )
                {
                    if( !( scrollbar->activeSubControls & SC_ScrollBarAddLine ) )
                    {
                        newScrollbar.state &= ~( State_Sunken | State_MouseOver );
                    }

                    drawControl( CE_ScrollBarAddLine, &newScrollbar, painter, widget );
                }
            }

            if( scrollbar->subControls & SC_ScrollBarSubPage )
            {
                newScrollbar.rect = scrollbar->rect;
                newScrollbar.state = saveFlags;
                newScrollbar.rect = subControlRect( control, &newScrollbar, SC_ScrollBarSubPage, widget );
                if( newScrollbar.rect.isValid() )
                {
                    if( !( scrollbar->activeSubControls & SC_ScrollBarSubPage ) )
                    {
                        newScrollbar.state &= ~( State_Sunken | State_MouseOver );
                    }

                    drawControl( CE_ScrollBarSubPage, &newScrollbar, painter, widget );
                }
            }

            if( scrollbar->subControls & SC_ScrollBarAddPage )
            {
                newScrollbar.rect = scrollbar->rect;
                newScrollbar.state = saveFlags;
                newScrollbar.rect = subControlRect( control, &newScrollbar, SC_ScrollBarAddPage, widget );
                if( newScrollbar.rect.isValid() )
                {
                    if( !( scrollbar->activeSubControls & SC_ScrollBarAddPage ) )
                    {
                        newScrollbar.state &= ~( State_Sunken | State_MouseOver );
                    }

                    drawControl( CE_ScrollBarAddPage, &newScrollbar, painter, widget );
                }
            }

            if( scrollbar->subControls & SC_ScrollBarSlider )
            {
                newScrollbar.rect = scrollbar->rect;
                newScrollbar.state = saveFlags;
                newScrollbar.rect = subControlRect( control, &newScrollbar, SC_ScrollBarSlider, widget );
                if( newScrollbar.rect.isValid() )
                {
                    if( !( scrollbar->activeSubControls & SC_ScrollBarSlider ) )
                    {
                        newScrollbar.state &= ~( State_Sunken | State_MouseOver );
                    }

                    drawControl( CE_ScrollBarSlider, &newScrollbar, painter, widget );
                }
            }
        }

        return;
    }

    if( control == CC_Slider )
    {
        const QStyleOptionSlider *slider = qstyleoption_cast<const QStyleOptionSlider *>( option );
        if( slider )
        {
            int thickness = proxy()->pixelMetric( PM_SliderControlThickness, slider, widget );
            int len = proxy()->pixelMetric( PM_SliderLength, slider, widget );
            int ticks = slider->tickPosition;

            QRect groove = subControlRect( CC_Slider, slider, SC_SliderGroove, widget );
            QRect handle = subControlRect( CC_Slider, slider, SC_SliderHandle, widget );


            if( ( slider->subControls & SC_SliderGroove ) && groove.isValid() )
            {
                int mid = thickness / 2;

                if( ticks & QSlider::TicksAbove )
                {
                    mid += len / 8;
                }

                if( ticks & QSlider::TicksBelow )
                {
                    mid -= len / 8;
                }

                painter->setPen( slider->palette.shadow().color() );
                if( slider->orientation == Qt::Horizontal )
                {
                    qDrawWinPanel( painter, groove.x(), groove.y() + mid - 2, groove.width(), 4, slider->palette, true );
                    painter->drawLine( groove.x() + 1, groove.y() + mid - 1, groove.x() + groove.width() - 3, groove.y() + mid - 1 );
                }
                else
                {
                    qDrawWinPanel( painter, groove.x() + mid - 2, groove.y(), 4, groove.height(), slider->palette, true );
                    painter->drawLine( groove.x() + mid - 1, groove.y() + 1, groove.x() + mid - 1, groove.y() + groove.height() - 3 );
                }
            }

            if( slider->subControls & SC_SliderTickmarks )
            {
                QStyleOptionSlider tmpSlider = *slider;
                tmpSlider.subControls = SC_SliderTickmarks;
#ifdef TARGET_OS_WINDOWS
                QProxyStyle::drawComplexControl( control, &tmpSlider, painter, widget );
#else
                QCommonStyle::drawComplexControl( control, &tmpSlider, painter, widget );
#endif // TARGET_OS_WINDOWS

                return;
            }

            if( slider->subControls & SC_SliderHandle )
            {
                const QColor color1 = slider->palette.shadow().color();
                const QColor color2 = slider->palette.dark().color();
                const QColor color3 = slider->palette.midlight().color();
                const QColor color4 = slider->palette.light().color();
                QBrush handleBrush;

                if( slider->state & State_Enabled )
                {
                    handleBrush = slider->palette.color( QPalette::Button );
                }
                else
                {
                    handleBrush = QBrush( slider->palette.color( QPalette::Button ), Qt::Dense4Pattern );
                }

                int x = handle.x(), y = handle.y(), wi = handle.width(), he = handle.height();

                int x1 = x;
                int x2 = x + wi - 1;
                int y1 = y;
                int y2 = y + he - 1;

                Qt::Orientation orient = slider->orientation;
                bool tickAbove = slider->tickPosition == QSlider::TicksAbove;
                bool tickBelow = slider->tickPosition == QSlider::TicksBelow;

                if( slider->state & State_HasFocus )
                {
                    QStyleOptionFocusRect fropt;
                    fropt.QStyleOption::operator=( *slider );
                    fropt.rect = subElementRect( SE_SliderFocusRect, slider, widget );
                    drawPrimitive( PE_FrameFocusRect, &fropt, painter, widget );
                }

                if( ( tickAbove && tickBelow ) || ( !tickAbove && !tickBelow ) )
                {
                    Qt::BGMode oldMode = painter->backgroundMode();
                    painter->setBackgroundMode( Qt::OpaqueMode );
                    qDrawWinButton( painter, QRect( x, y, wi, he ), slider->palette, false, &handleBrush );
                    painter->setBackgroundMode( oldMode );
                    return;
                }

                GuiDrawDirection dir;

                if( orient == Qt::Horizontal )
                {
                    if( tickAbove )
                    {
                        dir = eGuiDirUp;
                    }
                    else
                    {
                        dir = eGuiDirDown;
                    }
                }
                else
                {
                    if( tickAbove )
                    {
                        dir = eGuiDirLeft;
                    }
                    else
                    {
                        dir = eGuiDirRight;
                    }
                }

                QPolygon a;

                int d = 0;
                switch( dir )
                {
                case eGuiDirUp:
                    y1 = y1 + wi / 2;
                    d = ( wi + 1 ) / 2 - 1;
                    a.setPoints( 5, x1, y1, x1, y2, x2, y2, x2, y1, x1 + d, y1 - d );
                    break;

                case eGuiDirDown:
                    y2 = y2 - wi / 2;
                    d = ( wi + 1 ) / 2 - 1;
                    a.setPoints( 5, x1, y1, x1, y2, x1 + d, y2 + d, x2, y2, x2, y1 );
                    break;

                case eGuiDirLeft:
                    d = ( he + 1 ) / 2 - 1;
                    x1 = x1 + he / 2;
                    a.setPoints( 5, x1, y1, x1 - d, y1 + d, x1, y2, x2, y2, x2, y1 );
                    break;

                case eGuiDirRight:
                    d = ( he + 1 ) / 2 - 1;
                    x2 = x2 - he / 2;
                    a.setPoints( 5, x1, y1, x1, y2, x2, y2, x2 + d, y1 + d, x2, y1 );
                    break;
                }

                QBrush oldBrush = painter->brush();
                painter->setPen( Qt::NoPen );
                painter->setBrush( handleBrush );
                Qt::BGMode oldMode = painter->backgroundMode();
                painter->setBackgroundMode( Qt::OpaqueMode );
                painter->drawRect( x1, y1, x2 - x1 + 1, y2 - y1 + 1 );
                painter->drawPolygon( a );
                painter->setBrush( oldBrush );
                painter->setBackgroundMode( oldMode );

                if( dir != eGuiDirUp )
                {
                    painter->setPen( color4 );
                    painter->drawLine( x1, y1, x2, y1 );
                    painter->setPen( color3 );
                    painter->drawLine( x1, y1 + 1, x2, y1 + 1 );
                }

                if( dir != eGuiDirLeft )
                {
                    painter->setPen( color3 );
                    painter->drawLine( x1 + 1, y1 + 1, x1 + 1, y2 );
                    painter->setPen( color4 );
                    painter->drawLine( x1, y1, x1, y2 );
                }

                if( dir != eGuiDirRight )
                {
                    painter->setPen( color1 );
                    painter->drawLine( x2, y1, x2, y2 );
                    painter->setPen( color2 );
                    painter->drawLine( x2 - 1, y1 + 1, x2 - 1, y2 - 1 );
                }

                if( dir != eGuiDirDown )
                {
                    painter->setPen( color1 );
                    painter->drawLine( x1, y2, x2, y2 );
                    painter->setPen( color2 );
                    painter->drawLine( x1 + 1, y2 - 1, x2 - 1, y2 - 1 );
                }

                switch( dir )
                {
                case eGuiDirUp:
                    painter->setPen( color4 );
                    painter->drawLine( x1, y1, x1 + d, y1 - d );
                    painter->setPen( color1 );
                    d = wi - d - 1;
                    painter->drawLine( x2, y1, x2 - d, y1 - d );
                    d--;
                    painter->setPen( color3 );
                    painter->drawLine( x1 + 1, y1, x1 + 1 + d, y1 - d );
                    painter->setPen( color2 );
                    painter->drawLine( x2 - 1, y1, x2 - 1 - d, y1 - d );
                    break;

                case eGuiDirDown:
                    painter->setPen( color4 );
                    painter->drawLine( x1, y2, x1 + d, y2 + d );
                    painter->setPen( color1 );
                    d = wi - d - 1;
                    painter->drawLine( x2, y2, x2 - d, y2 + d );
                    d--;
                    painter->setPen( color3 );
                    painter->drawLine( x1 + 1, y2, x1 + 1 + d, y2 + d );
                    painter->setPen( color2 );
                    painter->drawLine( x2 - 1, y2, x2 - 1 - d, y2 + d );
                    break;

                case eGuiDirLeft:
                    painter->setPen( color4 );
                    painter->drawLine( x1, y1, x1 - d, y1 + d );
                    painter->setPen( color1 );
                    d = he - d - 1;
                    painter->drawLine( x1, y2, x1 - d, y2 - d );
                    d--;
                    painter->setPen( color3 );
                    painter->drawLine( x1, y1 + 1, x1 - d, y1 + 1 + d );
                    painter->setPen( color2 );
                    painter->drawLine( x1, y2 - 1, x1 - d, y2 - 1 - d );
                    break;

                case eGuiDirRight:
                    painter->setPen( color4 );
                    painter->drawLine( x2, y1, x2 + d, y1 + d );
                    painter->setPen( color1 );
                    d = he - d - 1;
                    painter->drawLine( x2, y2, x2 + d, y2 - d );
                    d--;
                    painter->setPen( color3 );
                    painter->drawLine( x2, y1 + 1, x2 + d, y1 + 1 + d );
                    painter->setPen( color2 );
                    painter->drawLine( x2, y2 - 1, x2 + d, y2 - 1 - d );
                    break;
                }
            }
        }

        return;
    }

    if( control == CC_SpinBox )
    {
        if( const QStyleOptionSpinBox * optSpinBox = qstyleoption_cast<const QStyleOptionSpinBox *>( option ) )
        {
            QStyleOptionSpinBox copy = *optSpinBox;
            PrimitiveElement primativeElem;
            bool enabled = option->state & State_Enabled;
            if( optSpinBox->frame && ( optSpinBox->subControls & SC_SpinBoxFrame ) )
            {
                QBrush editBrush = optSpinBox->palette.brush( QPalette::Base );
                QRect r = proxy()->subControlRect( CC_SpinBox, optSpinBox, SC_SpinBoxFrame, widget );
                QPalette shadePal = optSpinBox->palette;
                shadePal.setColor( QPalette::Midlight, shadePal.button().color() );
                qDrawWinPanel( painter, r, shadePal, true, &editBrush );
            }

            QPalette shadePal( option->palette );
            shadePal.setColor( QPalette::Button, option->palette.light().color() );
            shadePal.setColor( QPalette::Light, option->palette.button().color() );

            if( optSpinBox->subControls & SC_SpinBoxUp )
            {
                copy.subControls = SC_SpinBoxUp;
                QPalette pal2 = optSpinBox->palette;
                if( !( optSpinBox->stepEnabled & QAbstractSpinBox::StepUpEnabled ) )
                {
                    pal2.setCurrentColorGroup( QPalette::Disabled );
                    copy.state &= ~State_Enabled;
                }

                copy.palette = pal2;

                if( optSpinBox->activeSubControls == SC_SpinBoxUp && ( optSpinBox->state & State_Sunken ) )
                {
                    copy.state |= State_On;
                    copy.state |= State_Sunken;
                }
                else
                {
                    copy.state |= State_Raised;
                    copy.state &= ~State_Sunken;
                }

                primativeElem = ( optSpinBox->buttonSymbols == QAbstractSpinBox::PlusMinus ? PE_IndicatorSpinPlus : PE_IndicatorSpinUp );

                copy.rect = proxy()->subControlRect( CC_SpinBox, optSpinBox, SC_SpinBoxUp, widget );
                qDrawWinButton( painter, copy.rect, shadePal, copy.state & ( State_Sunken | State_On ), &copy.palette.brush( QPalette::Button ) );
                copy.rect.adjust( 4, 1, -5, -1 );
                if( ( !enabled || !( optSpinBox->stepEnabled & QAbstractSpinBox::StepUpEnabled ) ) && proxy()->styleHint( SH_EtchDisabledText, option, widget ) )
                {
                    QStyleOptionSpinBox lightCopy = copy;
                    lightCopy.rect.adjust( 1, 1, 1, 1 );
                    lightCopy.palette.setBrush( QPalette::ButtonText, copy.palette.light() );

                    proxy()->drawPrimitive( primativeElem, &lightCopy, painter, widget );
                }

                proxy()->drawPrimitive( primativeElem, &copy, painter, widget );
            }

            if( optSpinBox->subControls & SC_SpinBoxDown )
            {
                copy.subControls = SC_SpinBoxDown;
                copy.state = optSpinBox->state;
                QPalette palette2 = optSpinBox->palette;
                if( !( optSpinBox->stepEnabled & QAbstractSpinBox::StepDownEnabled ) )
                {
                    palette2.setCurrentColorGroup( QPalette::Disabled );
                    copy.state &= ~State_Enabled;
                }

                copy.palette = palette2;

                if( optSpinBox->activeSubControls == SC_SpinBoxDown && ( optSpinBox->state & State_Sunken ) )
                {
                    copy.state |= State_On;
                    copy.state |= State_Sunken;
                }
                else
                {
                    copy.state |= State_Raised;
                    copy.state &= ~State_Sunken;
                }

                primativeElem = ( optSpinBox->buttonSymbols == QAbstractSpinBox::PlusMinus ? PE_IndicatorSpinMinus : PE_IndicatorSpinDown );

                copy.rect = proxy()->subControlRect( CC_SpinBox, optSpinBox, SC_SpinBoxDown, widget );
                qDrawWinButton( painter, copy.rect, shadePal, copy.state & ( State_Sunken | State_On ), &copy.palette.brush( QPalette::Button ) );

                copy.rect.adjust( 4, 0, -5, -1 );
                if( ( !enabled || !( optSpinBox->stepEnabled & QAbstractSpinBox::StepDownEnabled ) )
                    && proxy()->styleHint( SH_EtchDisabledText, option, widget ) )
                {
                    QStyleOptionSpinBox lightCopy = copy;
                    lightCopy.rect.adjust( 1, 1, 1, 1 );
                    lightCopy.palette.setBrush( QPalette::ButtonText, copy.palette.light() );
                    proxy()->drawPrimitive( primativeElem, &lightCopy, painter, widget );
                }

                proxy()->drawPrimitive( primativeElem, &copy, painter, widget );
            }
        }

        return;
    }

    if( control == CC_GroupBox )
    {
        // allow default
    }

#ifdef TARGET_OS_WINDOWS
    QProxyStyle::drawComplexControl( control, option, painter, widget );
#else
    QCommonStyle::drawComplexControl( control, option, painter, widget );
#endif // TARGET_OS_WINDOWS
}
