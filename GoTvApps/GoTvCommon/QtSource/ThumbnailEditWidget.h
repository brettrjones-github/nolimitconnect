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
#pragma once

#include <QWidget>
#include "ui_ThumbnailEditWidget.h"

#include <CoreLib/VxGUID.h>


class IVxVidCap;
class AssetInfo;  
class AppletBase;

class ThumbnailEditWidget : public QWidget
{
    Q_OBJECT
public:
    ThumbnailEditWidget( QWidget * parent );
    ~ThumbnailEditWidget() override = default;

    void                        setIsUserPickedImage( bool userPicked )         { ui.m_ThumbnailViewWidget->setIsUserPickedImage( userPicked ); }
    bool                        getIsUserPickedImage( void )                    { return ui.m_ThumbnailViewWidget->getIsUserPickedImage(); }

    bool                        loadFromAsset( AssetInfo * thumbAsset );
    bool                        saveToPngFile( QString& fileName )              { return ui.m_ThumbnailViewWidget->saveToPngFile( fileName ); }

    void                        setAssetId( VxGUID& assetGuid )                 { m_AsssetId = assetGuid; }
    VxGUID&                     getAssetId( void )                              { return m_AsssetId; }
    void                        clearAssetId( void )                            { m_AsssetId.clearVxGUID(); }
    bool                        isAssetIdValid( void )                          { return m_AsssetId.isVxGUIDValid(); }

    bool                        generateThumbAsset( AssetInfo& assetInfo );

    bool                        loadThumbnail( VxGUID& assetGuid );
    VxGUID                      updateAndGetThumbnailId( void );

protected slots:
    void                        slotThumbGalleryClick( void );
    void                        slotSnapShotButClick( void );
    void                        slotBrowseButClick( void );
    void                        slotMakeCircleButClick( void );
    void                        slotUndoCircleClick( void );
    void                        slotThumbSelected( AppletBase * thumbGallery, ThumbnailViewWidget * thumb );

protected:
    QPixmap                     makeCircleImage( QPixmap& pixmap );

    Ui::ThumnailEditWidgetUi	ui;
    AppCommon&					m_MyApp;
    IVxVidCap *					m_VidCap = nullptr;
    bool 					    m_CameraSourceAvail = false;
    bool 					    m_IsCircle = false;
    QPixmap                     m_SquarePixmap;
    VxGUID                      m_AsssetId;
    bool                        m_WasModifiedByUser = false;
    uint8_t *					m_pu8BitmapData = nullptr;	// snapshot bitmap
    uint32_t					m_u32BitmapLen = 0;		// bitmap length
};
