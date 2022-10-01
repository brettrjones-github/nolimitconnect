//============================================================================
// Copyright (C) 2018 Brett R. Jones
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


#include "NetAvailStatusWidget.h"
#include "AppCommon.h"
#include "GuiParams.h"

#include <CoreLib/VxDebug.h>

#include <QPainter>

//============================================================================
NetAvailStatusWidget::NetAvailStatusWidget( QWidget* parent )
: QWidget( parent )
, m_MyApp( GetAppInstance() )
, m_NotAvailColor( COLOR_GREY_MEDIUM )
, m_InProgressColor( COLOR_YELLOW_MEDIUM )
, m_NoRelayColor( COLOR_ORANGE )
, m_WithRelayColor( COLOR_GREEN_MEDIUM )
, m_DirectConnectColor( COLOR_GREEN )
{
    toGuiNetAvailStatus( m_MyApp.getEngine().getNetStatusAccum().getNetAvailStatus() );
}

//============================================================================
void NetAvailStatusWidget::toGuiNetAvailStatus( ENetAvailStatus netStatus )
{
    m_NetAvailStatus = netStatus;
    update();
}

//============================================================================
void NetAvailStatusWidget::mousePressEvent( QMouseEvent * event )
{
    QWidget::mousePressEvent( event );
    emit clicked();
}

//============================================================================
void NetAvailStatusWidget::paintEvent( QPaintEvent* ev )
{
    QPainter painter( this );
    painter.save();
    painter.setRenderHint( QPainter::Antialiasing, true );
    drawNetBars( painter );
    painter.restore();
    painter.end();
}

//============================================================================
void NetAvailStatusWidget::resizeEvent( QResizeEvent* ev )
{
    QWidget::resizeEvent( ev );
    update();
}

//============================================================================
void NetAvailStatusWidget::drawNetBars( QPainter& painter )
{
    QRect widgetRect( 0, 0, size().width(), size().height() );
    for( int barNum = 0; barNum < (eMaxNetAvailStatus - 1); barNum++ )
    {
        drawNetBar( painter, widgetRect, barNum );
    }
}

//============================================================================
void NetAvailStatusWidget::drawNetBar( QPainter& painter, QRect& widgetRect, int barNum )
{
    const int gapBetweenBars = 1;
    const int margin = 1;
    int totalBarCnt = ( int )(eMaxNetAvailStatus - 1);
    int barPlusGapWidth = ( widgetRect.width() - margin * 2 ) / totalBarCnt;
    int barWidth = barPlusGapWidth - gapBetweenBars;

    if( barWidth >= 1 && ( widgetRect.width() > margin * 2 ) && ( widgetRect.height() > margin * 2 ) )
    {
        QRect contentRect;
        contentRect.setCoords( widgetRect.left() + margin, widgetRect.top() + margin, widgetRect.right() - margin, widgetRect.bottom() -  margin );
        if( ( totalBarCnt * gapBetweenBars < contentRect.width() ) &&
            ( ( totalBarCnt + 1 ) * gapBetweenBars < contentRect.height() ) )
        {
            int barSegHeight = contentRect.height() / totalBarCnt;
            QColor barColor = determineBarColor( barNum );
            int leftPos = contentRect.left() + barNum * barPlusGapWidth;
            int topPos = contentRect.bottom() - ( ( barNum + 1 ) * barSegHeight );

            QRect barRect( leftPos, topPos, barWidth, ( ( barNum + 1 ) * barSegHeight ) );

            painter.fillRect( barRect, barColor );
        }
    }
}

//============================================================================
QColor NetAvailStatusWidget::determineBarColor( int barNum )
{
    bool isInactive = ( barNum > m_NetAvailStatus );
    QColor barColor = isInactive ? m_NotAvailColor : m_InProgressColor;
    if( !isInactive )
    {
        switch( m_NetAvailStatus )
        {
        case eNetAvailOnlineButNoRelay:
            barColor = m_NoRelayColor;
            break;

        case eNetAvailFullOnlineWithRelay:
        case eNetAvailRelayGroupHost:
            barColor = m_WithRelayColor;
            break;

        case eNetAvailFullOnlineDirectConnect:
        case eNetAvailDirectGroupHost:
            barColor = m_DirectConnectColor;
            break;
        
        default:
            break;
        }
    }

    return barColor;
}

//============================================================================
int NetAvailStatusWidget::heightForWidth( int width ) const
{
    if( width >= ( int )( GuiParams::MIN_TITLE_BAR_BUTTON_SIZE * GuiParams::getGuiScale() ) )
    {
        return width;
    }

    return ( int )( GuiParams::MIN_TITLE_BAR_BUTTON_SIZE  * GuiParams::getGuiScale() );
}

//============================================================================
QSize NetAvailStatusWidget::sizeHint( void ) const
{
    int buttonSize = ( int )( GuiParams::MIN_TITLE_BAR_BUTTON_SIZE * GuiParams::getGuiScale() );
    return QSize( buttonSize, buttonSize );
}

//============================================================================
void NetAvailStatusWidget::setFixedSize( const QSize & fixedSize )
{
    if( fixedSize.width() == fixedSize.height() )
    {
        QWidget::setFixedSize( ( int )( fixedSize.width() * GuiParams::getGuiScale() ),
            ( int )( fixedSize.height() * GuiParams::getGuiScale() ) );
    }
    else
    {
        this->setFixedWidth( fixedSize.width() );
        this->setFixedHeight( fixedSize.height() );
    }
}

//============================================================================
void NetAvailStatusWidget::setFixedSize( int width, int height )
{
    if( width == height )
    {
        QWidget::setFixedSize( ( int )( width * GuiParams::getGuiScale() ),
            ( int )( height * GuiParams::getGuiScale() ) );
    }
    else
    {
        this->setFixedWidth( width );
        this->setFixedHeight( height );
    }
}

//============================================================================
void NetAvailStatusWidget::setFixedWidth( int width )
{
    QWidget::setFixedWidth( ( int )( width * GuiParams::getGuiScale() ) );
}

//============================================================================
void NetAvailStatusWidget::setFixedSizeAbsolute( const QSize & fixedSize )
{
    setFixedSizeAbsolute( fixedSize.width(), fixedSize.height() );
}

//============================================================================
void NetAvailStatusWidget::setFixedSizeAbsolute( int fixedWidth, int fixedHeight )
{
    QWidget::setFixedSize( fixedWidth, fixedHeight );
}

//============================================================================
void NetAvailStatusWidget::setFixedHeight( int height )
{
    QWidget::setFixedWidth( ( int )( height * GuiParams::getGuiScale() ) );
}

//============================================================================
void NetAvailStatusWidget::setMinimumSize( const QSize & minSize )
{
    if( minSize.height() == minSize.width() )
    {
        QWidget::setMinimumSize( ( int )( minSize.width() * GuiParams::getGuiScale() ),
            ( int )( minSize.height() * GuiParams::getGuiScale() ) );
    }
    else
    {
        QWidget::setMinimumSize( ( int )( minSize.width() * GuiParams::getGuiScale() ),
            ( int )( minSize.height() * GuiParams::getGuiScale() ) );
    }
}

//============================================================================
void NetAvailStatusWidget::setMinimumSize( int minw, int minh )
{
    if( minw == minh )
    {
        QWidget::setMaximumSize( ( int )( minw * GuiParams::getGuiScale() ),
            ( int )( minh * GuiParams::getGuiScale() ) );
    }
    else
    {
        QWidget::setMaximumSize( ( int )( minw * GuiParams::getGuiScale() ),
            ( int )( minh * GuiParams::getGuiScale() ) );
    }
}

//============================================================================
void NetAvailStatusWidget::setMaximumSize( const QSize & maxSize )
{
    if( maxSize.width() < GuiParams::getMaxWidgetSize() && maxSize.height() < GuiParams::getMaxWidgetSize() )
    {
        QWidget::setMaximumSize( ( int )( maxSize.width() * GuiParams::getGuiScale() ),
            ( int )( maxSize.height() * GuiParams::getGuiScale() ) );
    }
    else
    {
        // probably setup ui setting to maximum.. do not exceed maximum { width = 16777215, height = 16777215 } with scaling 
        QWidget::setMaximumSize( GuiParams::getMaxWidgetSize(), GuiParams::getMaxWidgetSize() );
    }
}

//============================================================================
void NetAvailStatusWidget::setMaximumSize( int maxw, int maxh )
{
    if( maxw < GuiParams::getMaxWidgetSize() && maxh < GuiParams::getMaxWidgetSize() )
    {
        QWidget::setMaximumSize( ( int )( maxw * GuiParams::getGuiScale() ),
            ( int )( maxh * GuiParams::getGuiScale() ) );
    }
    else
    {
        // probably setup ui setting to maximum.. do not exceed maximum { width = 16777215, height = 16777215 } with scaling 
        QWidget::setMaximumSize( GuiParams::getMaxWidgetSize(), GuiParams::getMaxWidgetSize() );
    }
}
