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
// http://www.nolimitconnect.org
//============================================================================

#include "AppCommon.h"	
#include "AppSettings.h"

#include "AppletGalleryThumb.h"

#include "FileShareItemWidget.h"
#include "MyIcons.h"
#include "AppletPopupMenu.h"
#include "AppGlobals.h"
#include "FileItemInfo.h"
#include "FileActionMenu.h"
#include "GuiHelpers.h"

#include <ptop_src/ptop_engine_src/P2PEngine/P2PEngine.h>
#include <ptop_src/ptop_engine_src/ThumbMgr/ThumbInfo.h>
#include <ptop_src/ptop_engine_src/ThumbMgr/ThumbMgr.h>

#include <PktLib/VxSearchDefs.h>
#include <NetLib/VxFileXferInfo.h>
#include <CoreLib/VxFileInfo.h>
#include <CoreLib/VxGlobals.h>

#include <QResizeEvent>

//============================================================================
AppletGalleryThumb::AppletGalleryThumb(	AppCommon& app, QWidget * parent )
: AppletBase( OBJNAME_APPLET_GALLERY_THUMB, app, parent )
, m_ThumbMgr( app.getEngine().getThumbMgr() )
{
    setAppletType( eAppletGalleryThumb );
    ui.setupUi( getContentItemsFrame() );
    setTitleBarText( DescribeApplet( m_EAppletType ) );
    ui.m_ThumbDirLabel->setText( VxGetAppDirectory( eAppDirThumbs ).c_str() );

    connect( ui.m_ImageListWidget, SIGNAL( signalImageClicked( ThumbnailViewWidget * ) ), this, SLOT( slotImageClicked( ThumbnailViewWidget * ) ) );

    m_MyApp.activityStateChange( this, true );
}

//============================================================================
AppletGalleryThumb::~AppletGalleryThumb()
{
    m_MyApp.activityStateChange( this, false );
}

//============================================================================
void AppletGalleryThumb::resizeEvent( QResizeEvent * ev )
{
    AppletBase::resizeEvent( ev );
    //LogMsg( LOG_DEBUG, "AppletGalleryThumb Resize w %d h %d\n", ev->size().width(), ev->size().height() );
    if( m_isShown && !m_isLoaded )
    {
        m_isLoaded = true;
        loadAssets();
    }
}

//============================================================================
void AppletGalleryThumb::showEvent( QShowEvent * ev )
{
    //LogMsg( LOG_DEBUG, "AppletGalleryThumb show event\n" );
    m_isShown = true;
    AppletBase::showEvent( ev );
}

//============================================================================
void AppletGalleryThumb::loadAssets( void )
{
    std::vector<VxGUID>& emoticonIdList = m_ThumbMgr.getEmoticonIdList();
    std::vector<AssetBaseInfo*>& assetList = m_ThumbMgr.getAssetBaseInfoList();
    for( AssetBaseInfo* assetInfo : assetList )
    {
        if( assetInfo && eAssetTypeThumbnail == assetInfo->getAssetType() )
        {
            // dont include emoticons
            if( emoticonIdList.end() == std::find( emoticonIdList.begin(), emoticonIdList.end(), assetInfo->getAssetUniqueId() ) )
            {
                ui.m_ImageListWidget->addAsset( dynamic_cast< ThumbInfo* >( assetInfo ) );
            }
        }
    }
}

//============================================================================
void AppletGalleryThumb::slotImageClicked( ThumbnailViewWidget * thumb )
{
    if( thumb )
    {
        emit signalThumbSelected( this, thumb );
    }
}