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

#include "AppCommon.h"	
#include "AppSettings.h"

#include "AppletGalleryEmoticon.h"

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
AppletGalleryEmoticon::AppletGalleryEmoticon(	AppCommon& app, QWidget * parent )
: AppletBase( OBJNAME_APPLET_GALLERY_EMOTICON, app, parent )
, m_ThumbMgr( app.getEngine().getThumbMgr() )
{
    setAppletType( eAppletGalleryEmoticon );
    ui.setupUi( getContentItemsFrame() );
    setTitleBarText( DescribeApplet( m_EAppletType ) );

    connect( ui.m_ImageListWidget, SIGNAL( signalImageClicked( ThumbnailViewWidget * ) ), this, SLOT( slotImageClicked( ThumbnailViewWidget * ) ) );

    m_MyApp.activityStateChange( this, true );
}

//============================================================================
AppletGalleryEmoticon::~AppletGalleryEmoticon()
{
    m_MyApp.activityStateChange( this, false );
}

//============================================================================
void AppletGalleryEmoticon::resizeEvent( QResizeEvent * ev )
{
    AppletBase::resizeEvent( ev );
    //LogMsg( LOG_DEBUG, "AppletGalleryEmoticon Resize w %d h %d\n", ev->size().width(), ev->size().height() );
    if( m_isShown && !m_isLoaded )
    {
        m_isLoaded = true;
        loadAssets();
    }
}

//============================================================================
void AppletGalleryEmoticon::showEvent( QShowEvent * ev )
{
    //LogMsg( LOG_DEBUG, "AppletGalleryEmoticon show event\n" );
    m_isShown = true;
    AppletBase::showEvent( ev );
}

//============================================================================
void AppletGalleryEmoticon::loadAssets( void )
{
    m_MyApp.getThumbMgr().generateEmoticonsIfNeeded( this );
    std::vector<VxGUID>& emoticonIdList = m_ThumbMgr.getEmoticonIdList();
    for( auto &assetId : emoticonIdList )
    {
        AssetBaseInfo* assetInfo = m_ThumbMgr.findAsset( assetId );
        if( assetInfo && eAssetTypeThumbnail == assetInfo->getAssetType() )
        {
            ui.m_ImageListWidget->addAsset( dynamic_cast<ThumbInfo*>(assetInfo) );
        }
    }
}

//============================================================================
void AppletGalleryEmoticon::slotImageClicked( ThumbnailViewWidget * thumb )
{
    if( thumb )
    {
        emit signalThumbSelected( this, thumb );
    }
}