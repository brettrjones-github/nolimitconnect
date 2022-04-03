//============================================================================
// Copyright (C) 2019 Brett R. Jones 
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

#include <QWidget> // must be declared first or linux Qt 6.2.4 will error in qmetatype.h 2167:23: array subscript value ‘53’ is outside the bounds

#include "ImageListEntry.h"
#include "ImageListWidget.h"
#include "ImageListRow.h"

#include "MyIcons.h"
#include "AppletPopupMenu.h"
#include "AppGlobals.h"
#include "AppCommon.h"
#include "GuiParams.h"
#include <CoreLib/VxDebug.h>

#include <ptop_src/ptop_engine_src/P2PEngine/P2PEngine.h>
#include <ptop_src/ptop_engine_src/AssetMgr/AssetMgr.h>
#include <ptop_src/ptop_engine_src/AssetMgr/AssetInfo.h>
#include <ptop_src/ptop_engine_src/ThumbMgr/ThumbMgr.h>
#include <ptop_src/ptop_engine_src/ThumbMgr/ThumbInfo.h>

#include <CoreLib/VxDebug.h>

#include <QResizeEvent>

 
//============================================================================
ImageListWidget::ImageListWidget( QWidget * parent )
: QListWidget( parent )
, m_MyApp( GetAppInstance() )
, m_Engine( m_MyApp.getEngine() )
, m_ThumbMgr( m_MyApp.getEngine().getThumbMgr() )
{
    connect( this, SIGNAL(itemClicked(QListWidgetItem *)),
                          this, SLOT(slotItemClicked(QListWidgetItem *))) ;

    connect( this, SIGNAL(itemDoubleClicked(QListWidgetItem *)),
                          this, SLOT(slotItemClicked(QListWidgetItem *))) ;
}

//============================================================================
void ImageListWidget::clearImages( void )
{
    ImageListRow * listRow;
    int iIdx = 0;
    while( iIdx < this->count() )
    {
        listRow = dynamic_cast< ImageListRow * >( this->item( iIdx ) );
        listRow->clearImages();
        iIdx++;
    }
}
//============================================================================
void ImageListWidget::clearItems( void )
{
    while( this->count() )
    {
        takeItem( 0 );
    }
}


//============================================================================
void ImageListWidget::resizeEvent( QResizeEvent * ev )
{
    QListWidget::resizeEvent( ev );
    LogMsg( LOG_DEBUG, "ImageListWidget Resize w %d h %d", ev->size().width(), ev->size().height() );
    ImageListRow * listRow;
    int iIdx = 0;
    while( iIdx < this->count() )
    {
        listRow = dynamic_cast< ImageListRow * >( this->item( iIdx ) );
        if( listRow )
        {
            listRow->recalculateSizeHint( width(), GuiParams::getGuiScale() );
        }

        iIdx++;
    }
}

//============================================================================
void ImageListWidget::loadThumbAssets( void )
{
    std::vector<AssetBaseInfo*>& assetList = m_ThumbMgr.getAssetBaseInfoList();
    for( AssetBaseInfo* assetInfo : assetList )
    {
        if( assetInfo && eAssetTypeThumbnail == assetInfo->getAssetType() )
        {
            addAsset( dynamic_cast<ThumbInfo*>(assetInfo) );
        }
    }
}

//============================================================================
void ImageListWidget::addAsset( ThumbInfo * asset )
{
    if( asset && ( eAssetTypeThumbnail == asset->getAssetType() ) )
    {
        if( !thumbExistsInList( asset->getAssetUniqueId() ) )
        {
            ImageListRow * listRow = getRowWithRoomForThumbnail();
            if( listRow )
            {
                ThumbnailViewWidget * thumbnail = new ThumbnailViewWidget( listRow );
                if( !thumbnail->loadFromAsset( asset ) )
                {
                    LogMsg( LOG_ERROR, "ImageListWidget failed load of asset %s %s", asset->getAssetUniqueId().toOnlineIdString().c_str(), asset->getAssetName().c_str() );
                }

                connect( thumbnail, SIGNAL( signalImageClicked( ThumbnailViewWidget * ) ), this, SLOT( slotImageClicked( ThumbnailViewWidget * ) ) );
                listRow->addThumbnail( thumbnail );
            }
            else
            {
                LogMsg( LOG_DEBUG, "ImageListWidget asset row not found for %s %s", asset->getAssetUniqueId().toOnlineIdString().c_str(), asset->getAssetName().c_str() );
            }
        }
        else
        {
            LogMsg( LOG_DEBUG, "ImageListWidget asset already exists %s %s", asset->getAssetUniqueId().toOnlineIdString().c_str(), asset->getAssetName().c_str() );
        }
    }
}

//============================================================================
bool ImageListWidget::thumbExistsInList( VxGUID& assetId )
{
    ImageListRow * listRow;
    int iIdx = 0;
    while( iIdx < this->count() )
    {
        listRow = dynamic_cast< ImageListRow * >( this->item( iIdx ) );
        if( listRow && listRow->thumbExistsInList( assetId ) )
        {
            return true;
        }

        iIdx++;
    }

    return false;
}

//============================================================================
ImageListRow * ImageListWidget::getRowWithRoomForThumbnail( void )
{
    ImageListRow * listRow;
    int iIdx = 0;
    while( iIdx < this->count() )
    {
        listRow = dynamic_cast< ImageListRow * >( this->item( iIdx ) );
        if( listRow && listRow->hasRoomForThumbnail( iIdx ) )
        {
            return listRow;
        }

        iIdx++;
    }

    listRow = new ImageListRow( this );
    QListWidgetItem * lineItem = dynamic_cast< QListWidgetItem * >( listRow );
    if( lineItem )
    {
        QWidget* lineWidget = dynamic_cast< QWidget* >( listRow );
        if( lineWidget )
        {
            addItem( lineItem );
            listRow->recalculateSizeHint( width(), GuiParams::getGuiScale() );
            listRow->setRowNum( count() );

            setItemWidget( lineItem, lineWidget );
            connect( listRow, SIGNAL( signalImageClicked( ThumbnailViewWidget* ) ), this, SLOT( slotImageClicked( ThumbnailViewWidget* ) ) );
        }
    }

    return listRow;
}

//============================================================================
void ImageListWidget::slotItemClicked(QListWidgetItem * item )
{
	if( 300 < m_ClickEventTimer.elapsedMs()  ) // avoid duplicate clicks
	{
		//slotImageListItemClicked( (ImageListEntry *) item );
	}
}

//============================================================================
void ImageListWidget::slotImageClicked( ThumbnailViewWidget * thumb )
{
	if( 300 > m_ClickEventTimer.elapsedMs()  ) // avoid duplicate clicks
	{
		return;
	}

	m_ClickEventTimer.startTimer();
    emit signalImageClicked( thumb );
}
