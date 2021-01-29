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

#include <app_precompiled_hdr.h>
#include "ThumbnailEditWidget.h"
#include "ActivitySnapShot.h"
#include "AppletGalleryThumb.h"
#include "AppletSnapshot.h"
#include "AppletMgr.h"

#include "AppCommon.h"
#include "GuiHelpers.h"
#include "GuiParams.h"

#include <GoTvCore/GoTvP2P/AssetMgr/AssetInfo.h>
#include <GoTvCore/GoTvP2P/AssetMgr/AssetMgr.h>
#include <GoTvCore/GoTvP2P/P2PEngine/P2PEngine.h>

#include <CoreLib/VxFileUtil.h>
#include <CoreLib/VxDebug.h>
#include <CoreLib/VxGlobals.h>
#include <VxVideoLib/VxVideoLib.h>

#include <QMessageBox>
#include <QPainter>

//============================================================================
ThumbnailEditWidget::ThumbnailEditWidget( QWidget * parent )
    : QWidget( parent )
    , m_MyApp( GetAppInstance() )
{
    m_ParentApplet = GuiHelpers::findParentApplet( parent );
    m_CameraSourceAvail = GuiHelpers::isCameraSourceAvailable();
    ui.setupUi( this );
    QSize frameSize( GuiParams::getThumbnailSize().width() + 30, GuiParams::getThumbnailSize().height() + 30 );
    ui.m_ThumbnailFrame->setFixedSize( frameSize );

    connect( ui.m_TakeSnapshotButton, SIGNAL( clicked() ), this, SLOT( slotSnapShotButClick() ) );
    connect( ui.m_BrowsePictureButton, SIGNAL( clicked() ), this, SLOT( slotBrowseButClick() ) );
    connect( ui.m_MakeCircleButton, SIGNAL( clicked() ), this, SLOT( slotMakeCircleButClick() ) );
    connect( ui.m_UndoCircleButton, SIGNAL( clicked() ), this, SLOT( slotUndoCircleClick() ) );
    connect( ui.m_PickThumbButton, SIGNAL( clicked() ), this, SLOT( slotThumbGalleryClick() ) );
}

//============================================================================
bool ThumbnailEditWidget::loadFromAsset( AssetInfo * thumbAsset )
{
    bool loadOk = false;
    if( thumbAsset )
    {
        loadOk = ui.m_ThumbnailViewWidget->loadFromFile( thumbAsset->getAssetName().c_str() );
        if( loadOk )
        {
            setAssetId( thumbAsset->getAssetUniqueId() );
        }
    }

    return loadOk;
}

//============================================================================
bool ThumbnailEditWidget::generateThumbAsset( AssetInfo& assetInfoOut )
{
    bool assetGenerated = false;
    VxGUID assetGuid;
    VxGUID::generateNewVxGUID( assetGuid );
    QString fileName = VxGetAppDirectory( eAppDirThumbs ).c_str();
    fileName += assetGuid.toHexString().c_str();
    fileName += ".nlt"; // use extension not known as image so thumbs will not be scanned by android image gallery etc
    if( saveToPngFile( fileName ) && VxFileUtil::fileExists( fileName.toUtf8().constData() ) )
    {
        AssetInfo assetInfo( ( const char * )fileName.toUtf8().constData(), VxFileUtil::fileExists( fileName.toUtf8().constData() ), ( uint16_t )eAssetTypeThumbnail );
        assetInfo.setAssetUniqueId( assetGuid );
        assetInfo.setCreatorId( m_MyApp.getAppGlobals().getUserIdent()->getMyOnlineId() );
        if( m_MyApp.getEngine().getAssetMgr().addAsset( assetInfo ) )
        {
            assetGenerated = true;
            assetInfoOut = assetInfo;
        }
        else
        {
            QString msgText = QObject::tr( "Could not create thumbnail asset" );
            QMessageBox::information( this, QObject::tr( "Error occured creating thumbnail asset " ) + fileName, msgText );
        }
    }
    else
    {
        QString msgText = QObject::tr( "Could not save thumbnail image" );
        QMessageBox::information( this, QObject::tr( "Error occured saving thumbnail to file " ) + fileName, msgText );
    }

    return assetGenerated;
}

//============================================================================
void ThumbnailEditWidget::slotSnapShotButClick( void )
{
    if( m_CameraSourceAvail )
    {
        AppletSnapshot * appletSnapshot = dynamic_cast< AppletSnapshot * >( m_MyApp.getAppletMgr().launchApplet( eAppletSnapshot, m_ParentApplet ) );
        if( appletSnapshot )
        {
            //connect( appletSnapshot, SIGNAL( signalJpgSnapshot( uint8_t*, uint32_t, int, int ) ), this, SLOT( slotJpgSnapshot( uint8_t*, uint32_t, int, int ) ) );
            connect( appletSnapshot, SIGNAL( signalSnapshotImage( QImage ) ), this, SLOT( slotImageSnapshot( QImage ) ) );
        }
    }
    else
    {
        QMessageBox::warning( this, QObject::tr( "Camera Capture" ), QObject::tr( "No Camera Source Available." ) );
    }
}

//============================================================================
void ThumbnailEditWidget::slotJpgSnapshot( uint8_t* pu8JpgData, uint32_t u32DataLen, int iWidth, int iHeight )
{
    QPixmap bitmap;
    if( bitmap.loadFromData( pu8JpgData, u32DataLen, "JPG" ) )
    {
        VxGUID nullGUID;
        ui.m_ThumbnailViewWidget->setThumbnailId( nullGUID );
        ui.m_ThumbnailViewWidget->setThumbnailImage( bitmap );
        ui.m_ThumbnailViewWidget->setIsUserPickedImage( true );
        emit signalImageChanged();
    }
}

//============================================================================
void ThumbnailEditWidget::slotImageSnapshot( QImage snapshotImage )
{
    if( !snapshotImage.isNull() )
    {
        VxGUID nullGUID;
        ui.m_ThumbnailViewWidget->setThumbnailId( nullGUID );
        ui.m_ThumbnailViewWidget->setThumbnailImage( QPixmap::fromImage( snapshotImage ) );
        ui.m_ThumbnailViewWidget->setIsUserPickedImage( true );
        emit signalImageChanged();
    }
}

//============================================================================
void ThumbnailEditWidget::slotBrowseButClick( void )
{
    ui.m_ThumbnailViewWidget->browseForImage();
    m_ThumbnailIsCircular = !ui.m_ThumbnailViewWidget->getIsUserPickedImage();
    if( ui.m_ThumbnailViewWidget->getIsUserPickedImage() )
    {
        clearAssetId();
        emit signalImageChanged();
    }
}

//============================================================================
void ThumbnailEditWidget::slotMakeCircleButClick( void )
{
    if( !m_ThumbnailIsCircular )
    {
        m_SquarePixmap = *ui.m_ThumbnailViewWidget->getThumbnailImage();
        if( !m_SquarePixmap.isNull() )
        {
            m_ThumbnailIsCircular = true;
            ui.m_ThumbnailViewWidget->setThumbnailImage( makeCircleImage( m_SquarePixmap ) );
        }
    }
}

//============================================================================
void ThumbnailEditWidget::slotUndoCircleClick( void )
{
    if( m_ThumbnailIsCircular && !m_SquarePixmap.isNull() )
    {
        m_ThumbnailIsCircular = false;
        ui.m_ThumbnailViewWidget->setThumbnailImage( m_SquarePixmap );
        clearAssetId();
    }
}

//============================================================================
QPixmap ThumbnailEditWidget::makeCircleImage( QPixmap& pixmap )
{
    QPixmap target( pixmap.width(), pixmap.height() );
    target.fill( Qt::transparent );

    QPainter painter( &target );

    // Set clipped region (circle) in the center of the target image
    QRegion clipRegion( QRect( 0, 0, pixmap.width(), pixmap.height() ), QRegion::Ellipse );
    painter.setClipRegion( clipRegion );

    painter.drawPixmap( 0, 0, pixmap );  
    clearAssetId();
    return target;
}

//============================================================================
void ThumbnailEditWidget::slotThumbGalleryClick( void )
{
    AppletGalleryThumb * galleryThumb = dynamic_cast< AppletGalleryThumb * >( m_MyApp.getAppletMgr().launchApplet( eAppletGalleryThumb, m_ParentApplet ) );
    if( galleryThumb )
    {
        connect( galleryThumb, SIGNAL( signalThumbSelected( AppletBase *, ThumbnailViewWidget * ) ), this, SLOT( slotThumbSelected( AppletBase *, ThumbnailViewWidget * ) ) );
    }
}

//============================================================================
void ThumbnailEditWidget::slotThumbSelected( AppletBase * thumbGallery, ThumbnailViewWidget * thumb )
{
    if( thumbGallery && thumb )
    {
        VxGUID assetGuid = thumb->getThumbnailId();
        AssetInfo * thumbAsset = m_MyApp.getEngine().getAssetMgr().findAsset( assetGuid );
        if( thumbAsset )
        {
            if( loadFromAsset( thumbAsset ) )
            {
                setAssetId( assetGuid );
            }
        }

        disconnect( thumbGallery, SIGNAL( signalThumbSelected( AppletBase *, ThumbnailViewWidget * ) ), this, SLOT( slotThumbSelected( AppletBase *, ThumbnailViewWidget * ) ) );
        thumbGallery->close();
        emit signalImageChanged();
    }
}

//============================================================================
bool ThumbnailEditWidget::loadThumbnail( VxGUID& assetId )
{
    bool result = false;
    if( false == assetId.isVxGUIDValid() )
    {
        AssetInfo * thumbAsset = m_MyApp.getEngine().getAssetMgr().findAsset( assetId );
        if( thumbAsset )
        {
            if( loadFromAsset( thumbAsset ) )
            {
                setAssetId( assetId );
                result = true;
            }
        }
    }

    return result;
}

//============================================================================
VxGUID ThumbnailEditWidget::updateAndGetThumbnailId( void )
{
    bool assetExists = isAssetIdValid();
    if( assetExists )
    {
        AssetInfo * existingAsset = m_MyApp.getEngine().getAssetMgr().findAsset( getAssetId() );
        if( existingAsset )
        {
            return existingAsset->getAssetUniqueId();
        }
        else
        {
            assetExists = false;
        }
    }

    if( !assetExists && getIsUserPickedImage() )
    {
        AssetInfo assetInfo;
        if( generateThumbAsset( assetInfo ) )
        {
            return assetInfo.getAssetUniqueId();
        }
    }

    VxGUID nullGuid;
    return nullGuid;
}
