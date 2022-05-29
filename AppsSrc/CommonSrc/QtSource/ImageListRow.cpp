//============================================================================
// Copyright (C) 2009 Brett R. Jones 
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


#include "ImageListRow.h"
#include "ThumbnailViewWidget.h"
#include "GuiParams.h"

#include <CoreLib/VxDebug.h>

#include <QResizeEvent>

//============================================================================
ImageListRow::ImageListRow( QListWidget * parent  )
: QWidget( 0 )
, QListWidgetItem( parent )
, m_SizeHint( ( GuiParams::getThumbnailSize().width() + 20 ) * 2, GuiParams::getThumbnailSize().height() + 30 )
{
    ui.setupUi( this );
}

//============================================================================
void ImageListRow::slotImageClicked( ThumbnailViewWidget * thumbnail )
{
    emit signalImageClicked( thumbnail );
}

//============================================================================
void ImageListRow::addThumbnail( ThumbnailViewWidget * thumbnail )
{
    if( thumbnail )
    {
        ui.m_RowFrame->layout()->addWidget( thumbnail );
        m_ThumbnailCnt++;
        connect( thumbnail, SIGNAL( signalImageClicked( ThumbnailViewWidget * ) ), this, SLOT( slotImageClicked( ThumbnailViewWidget * ) ) );
    }
}

//============================================================================
void ImageListRow::setRowNum( int rowNum )
{
    //ui.m_RowLabel->setText( QString::number( rowNum ) );
}

//============================================================================
bool ImageListRow::hasRoomForThumbnail( int idx )
{
    int requiredWidth = ( ( m_ThumbnailCnt + 1 ) * ( GuiParams::getThumbnailSize().width() + 20 ) + 20 );
    // int curWidth = width();
    // int curHeight = height();
    // LogMsg( LOG_DEBUG, "ImageListRow hasRoomForThumbnail idx %d ? %d cnt %d w %d h %d\n", idx, requiredWidth < curWidth, m_ThumbnailCnt, curWidth, curHeight );
    return requiredWidth < sizeHint().width();
}

//============================================================================
bool ImageListRow::thumbExistsInList( VxGUID& assetId )
{
    for( int i = 0; i < ui.m_RowFrame->layout()->count(); ++i )
    {
        ThumbnailViewWidget * thumbWidget = dynamic_cast< ThumbnailViewWidget * >(ui.m_RowFrame->layout()->itemAt( i )->widget());
        if( thumbWidget && thumbWidget->getThumbnailId() == assetId )
        {
            return true;
        }
    }

    return false;
}

//============================================================================
void ImageListRow::clearImages( void )
{
    QLayoutItem * layoutItem;
    while( ( layoutItem = ui.m_RowFrame->layout()->takeAt( 0 ) ) != nullptr )
    {
        delete layoutItem->widget();
        delete layoutItem;
    }

    m_ThumbnailCnt = 0;
}

//============================================================================
void ImageListRow::recalculateSizeHint( int listWidth, float displayScale )
{
    if( listWidth > 40 )
    {
        m_SizeHint.setWidth( listWidth - 40 );
        QListWidgetItem::setSizeHint( sizeHint() );
    }
}

//============================================================================
QSize ImageListRow::sizeHint( void ) const
{
    return m_SizeHint;
}
