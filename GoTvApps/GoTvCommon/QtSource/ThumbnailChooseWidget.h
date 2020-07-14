//============================================================================
// Copyright (C) 2020 Brett R. Jones
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

#include <CoreLib/VxGUID.h>

#include "ui_ThumbnailChooseWidget.h"
#include <QLabel>


class AppCommon;
class QPixmap;
class AssetInfo;
class AppletBase;

class ThumbnailChooseWidget : public QLabel
{
    Q_OBJECT
public:
    ThumbnailChooseWidget( QWidget * parent );
    virtual ~ThumbnailChooseWidget() override = default;

    void                        setIsUserPickedImage( bool userPicked )         { ui.m_ThumbnailViewWidget->setIsUserPickedImage( userPicked ); }
    bool                        getIsUserPickedImage( void )                    { return ui.m_ThumbnailViewWidget->getIsUserPickedImage(); }

    bool                        loadFromAsset( AssetInfo * thumbAsset );
    bool                        saveToPngFile( QString& fileName )              { return ui.m_ThumbnailViewWidget->saveToPngFile( fileName ); }

    void                        setAssetId( VxGUID& assetGuid )                 { m_AsssetId = assetGuid; }
    VxGUID&                     getAssetId( void )                              { return m_AsssetId; }
    void                        clearAssetId( void )                            { m_AsssetId.clearVxGUID(); }
    bool                        isAssetIdValid( void )                          { return m_AsssetId.isVxGUIDValid(); }

    VxGUID&                     getThumbnailId( void )                          { return m_ThumbnailId; }
    VxGUID                      updateAndGetThumbnailId( void );

    bool                        loadThumbnail( VxGUID& assetGuid );

protected slots:
    void                        slotChooseThumb();
    void                        slotThumbSelected( AppletBase * thumbGallery, ThumbnailViewWidget * thumb );

protected:
    Ui::ThumnailChooseWidgetUi	ui;
    AppCommon&					m_MyApp;
    AppletBase*                 m_ParentApplet{ nullptr };
    VxGUID                      m_AsssetId;
    VxGUID                      m_ThumbnailId;

    bool                        m_bUserPickedImage = false;
};
