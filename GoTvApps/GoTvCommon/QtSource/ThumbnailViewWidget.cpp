//============================================================================
// Copyright (C) 2019 Brett R. Jones
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
#include "ThumbnailViewWidget.h"
#include "AppCommon.h"
#include "AppSettings.h"
#include "GuiHelpers.h"
#include "GuiParams.h"

#include <ptop_src/ptop_engine_src/P2PEngine/P2PEngine.h>
#include <ptop_src/ptop_engine_src/ThumbMgr/ThumbMgr.h>
#include <ptop_src/ptop_engine_src/ThumbMgr/ThumbInfo.h>

#include <CoreLib/VxDebug.h>
#include <CoreLib/VxFileUtil.h>
#include <CoreLib/VxGlobals.h>

#include <QMessageBox>
#include <QUuid>
#include <QFileDialog>
#include <QResizeEvent>

//============================================================================
ThumbnailViewWidget::ThumbnailViewWidget( QWidget * parent )
    : QLabel( parent )
    , m_MyApp( GetAppInstance() )
{
    setFixedSize( GuiParams::getThumbnailSize() );
}

//============================================================================
void ThumbnailViewWidget::mousePressEvent( QMouseEvent * event )
{
    QWidget::mousePressEvent( event );
    emit clicked();
    emit signalImageClicked( this );
}

//============================================================================
bool ThumbnailViewWidget::loadFromAsset( ThumbInfo * asset )
{
    if( asset && ( asset->isPhotoAsset() || asset->isThumbAsset() ) )
    {
        setThumbnailId( asset->getAssetUniqueId() );
        return loadFromFile( asset->getAssetName().c_str() );
    }

    return false;
}

//============================================================================
void ThumbnailViewWidget::updateImage( VxGUID& thumbGuid, bool isCircle )
{
    m_ThumbnailIsCircular = isCircle;
    cropAndUpdateImage( m_ThumbPixmap );
}

//============================================================================
bool ThumbnailViewWidget::loadFromFile( QString fileName )
{
    QPixmap pixmap;
    bool result = pixmap.load( fileName );
    if( result )
    {
        cropAndUpdateImage( pixmap );
    }

    return result;
}

//============================================================================
void ThumbnailViewWidget::slotJpgSnapshot( uint8_t* pu8JpgData, uint32_t u32DataLen, int iWidth, int iHeight )
{
    QPixmap bitmap;
    if( bitmap.loadFromData( pu8JpgData, u32DataLen, "JPG" ) )
    {
        cropAndUpdateImage( bitmap );
        setIsUserPickedImage( true );
    }
    else
    {
        QString msgText = QObject::tr( "Failed to read snapshot " );
        QMessageBox::warning( this, QObject::tr( "Error Reading snapshot" ), msgText );
    }
}

//============================================================================
QPixmap ThumbnailViewWidget::makeCircleImage( QPixmap& pixmap )
{
    QPixmap target( pixmap.width(), pixmap.height() );
    target.fill( Qt::transparent );

    QPainter painter( &target );

    // Set clipped region (circle) in the center of the target image
    QRegion clipRegion( QRect( 0, 0, pixmap.width(), pixmap.height() ), QRegion::Ellipse );
    painter.setClipRegion( clipRegion );

    painter.drawPixmap( 0, 0, pixmap );  
    return target;
}

//============================================================================
void ThumbnailViewWidget::cropAndUpdateImage( QPixmap& pixmap )
{
    QSize thumbSize = GuiParams::getThumbnailSize();
    QSize origSize = pixmap.size();

    if( thumbSize == origSize )
    {
        // no need to scale or crop image
        m_ThumbPixmap = pixmap;
        if( m_ThumbnailIsCircular )
        {
            setPixmap( makeCircleImage( m_ThumbPixmap ) );
        }
        else
        {
            setPixmap( m_ThumbPixmap );
        }
    }
    else if( !pixmap.isNull() )
    {
        int minSize = origSize.width() < origSize.height() ? origSize.width() : origSize.height();
        int leftMargin = ( origSize.width() - minSize ) / 2;
        int topMargin = ( origSize.height() - minSize ) / 2;
        QRect rect( leftMargin, topMargin, origSize.width() - leftMargin, origSize.height() - topMargin );
        QPixmap cropped = pixmap.copy( rect );
        QPixmap scaledPixmap = cropped.scaled( GuiParams::getThumbnailSize() );
        if( !scaledPixmap.isNull() )
        {
            if( m_ThumbnailIsCircular )
            {
                setPixmap( makeCircleImage( scaledPixmap ) );
            }
            else
            {
                setPixmap( scaledPixmap );
            }
        }
        else
        {
            QString msgText = QObject::tr( "Failed to scale and crop image " );
            QMessageBox::warning( this, QObject::tr( "Error scaling image." ), msgText );
        }
    }
    else
    {
        QString msgText = QObject::tr( "Null image " );
        QMessageBox::warning( this, QObject::tr( "Null image." ), msgText );
    }
}

//============================================================================
void ThumbnailViewWidget::browseForImage( void )
{
    QString startPath = QDir::current().path();
    std::string lastGalleryPath;
    m_MyApp.getAppSettings().getLastGalleryDir( lastGalleryPath );
    if( ( 0 != lastGalleryPath.length() )
        && ( VxFileUtil::directoryExists( lastGalleryPath.c_str() ) ) )
    {
        startPath = lastGalleryPath.c_str();
    }

    // Get a filename from the file dialog.
    QString filename = QFileDialog::getOpenFileName( this,
                                                     QObject::tr( "Open Image" ),
                                                     startPath,
                                                     SUPPORTED_IMAGE_FILES );
    if( filename.length() > 0 )
    {
        QPixmap oBitmap;
        if( false == oBitmap.load( filename ) )
        {
            QString msgText = QObject::tr( "Failed To Read Image File " ) + filename;
            QMessageBox::critical( this, QObject::tr( "Error Reading Image" ), msgText );
        }
        else
        {
            std::string justFileName;
            VxFileUtil::seperatePathAndFile( filename.toUtf8().constData(), lastGalleryPath, justFileName );
            if( ( 0 != lastGalleryPath.length() )
                && ( VxFileUtil::directoryExists( lastGalleryPath.c_str() ) ) )
            {
                m_MyApp.getAppSettings().setLastGalleryDir( lastGalleryPath );
            }

            cropAndUpdateImage( oBitmap );
            setIsUserPickedImage( true );
        }
    }
}

//============================================================================
bool ThumbnailViewWidget::saveToPngFile( QString pngFileName )
{
    bool isOk = false;
#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
    const QPixmap bitmap = pixmap();
    if (!bitmap.isNull())
    {
        isOk = bitmap.save(pngFileName, "PNG");
#else
    const QPixmap * bitmap = pixmap();
    if( bitmap )
    {
        isOk = bitmap->save( pngFileName, "PNG" );
#endif // QT_VERSION >= QT_VERSION_CHECK(6,0,0)
        if( !isOk )
        {
            QString msgText = QObject::tr( "Failed to write into " ) + pngFileName;
            QMessageBox::warning( this, QObject::tr( "Error Writing" ), msgText );
        }
    }
    else
    {
        QString msgText = QObject::tr( "Failed to save to file " ) + pngFileName;
        QMessageBox::warning( this, QObject::tr( "Error Writing" ), msgText );
    }

    return isOk;
}
