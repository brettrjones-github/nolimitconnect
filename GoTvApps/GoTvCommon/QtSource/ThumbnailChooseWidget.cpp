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

#include "ThumbnailChooseWidget.h"
#include "AppletChooseThumbnail.h"
#include "AppletMgr.h"

#include "AppCommon.h"
#include "AppSettings.h"
#include "GuiHelpers.h"
#include "GuiParams.h"

#include <GoTvCore/GoTvP2P/P2PEngine/P2PEngine.h>
#include <GoTvCore/GoTvP2P/AssetMgr/AssetMgr.h>
#include <GoTvCore/GoTvP2P/AssetMgr/AssetInfo.h>

#include <CoreLib/VxDebug.h>
#include <CoreLib/VxFileUtil.h>
#include <CoreLib/VxGlobals.h>

#include <QMessageBox>
#include <QUuid>
#include <QFileDialog>
#include <QResizeEvent>
#include <QMessageBox>
#include <QPainter>

//============================================================================
ThumbnailChooseWidget::ThumbnailChooseWidget( QWidget * parent )
    : QLabel( parent )
    , m_MyApp( GetAppInstance() )
{
    m_ParentApplet = GuiHelpers::findParentApplet( parent );
    ui.setupUi( this );
    QSize frameSize( GuiParams::getThumbnailSize().width() + 20, GuiParams::getThumbnailSize().height() + 20 );
    ui.m_ThumbnailFrame->setFixedSize( frameSize );
    setFixedHeight( GuiParams::getThumbnailSize().height() + 30 );
    connect( ui.m_ChooseThumbButton, SIGNAL( clicked() ), this, SLOT( slotChooseThumb() ) );
}

//============================================================================
void ThumbnailChooseWidget::slotChooseThumb()
{
    AppletChooseThumbnail * galleryThumb = dynamic_cast< AppletChooseThumbnail * >( m_MyApp.getAppletMgr().launchApplet( eAppletChooseThumbnail, m_ParentApplet ) );
    if( galleryThumb )
    {
        connect( galleryThumb, SIGNAL( signalThumbSelected( AppletBase *, ThumbnailEditWidget * ) ), this, SLOT( slotThumbSelected( AppletBase *, ThumbnailEditWidget * ) ) );
    }
}

//============================================================================
bool ThumbnailChooseWidget::loadFromAsset( AssetInfo * thumbAsset )
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
void ThumbnailChooseWidget::slotThumbSelected( AppletBase * thumbGallery, ThumbnailEditWidget * thumb )
{
    if( thumbGallery && thumb )
    {
        VxGUID assetGuid = thumb->updateAndGetThumbnailId();
        AssetInfo * thumbAsset = m_MyApp.getEngine().getAssetMgr().findAsset( assetGuid );
        if( thumbAsset )
        {
            if( loadFromAsset( thumbAsset ) )
            {
                setAssetId( assetGuid );
                emit signalThumbnailAssetChanged();
            }
        }
        else
        {
            clearAssetId();
        }

        /*
        disconnect( thumbGallery, SIGNAL( signalThumbSelected( AppletBase *, ThumbnailViewWidget * ) ), this, SLOT( slotThumbSelected( AppletBase *, ThumbnailViewWidget * ) ) );
        thumbGallery->close();
        */
    }
}

//============================================================================
bool ThumbnailChooseWidget::loadThumbnail( VxGUID& assetId )
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
VxGUID ThumbnailChooseWidget::updateAndGetThumbnailId( void )
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

    VxGUID nullGuid;
    return nullGuid;
}