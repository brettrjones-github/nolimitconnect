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
// http://www.nolimitconnect.com
//============================================================================
#include <app_precompiled_hdr.h>
#include "AppletEditAvatarImage.h"
#include "AppCommon.h"
#include "AppSettings.h"
#include "MyIcons.h"
#include "ActivitySnapShot.h"
#include "AppGlobals.h"
#include "AppCommon.h"
#include "AccountMgr.h"
#include "GuiHelpers.h"

#include <QMessageBox>
#include <QUuid>
#include <QFileDialog>

#include <ptop_src/ptop_engine_src/P2PEngine/EngineSettings.h>
#include <ptop_src/ptop_engine_src/AssetMgr/AssetMgr.h>
#include <ptop_src/ptop_engine_src/ThumbMgr/ThumbMgr.h>
#include <ptop_src/ptop_engine_src/ThumbMgr/ThumbInfo.h>

#include <CoreLib/VxFileUtil.h>
#include <CoreLib/VxDebug.h>
#include <CoreLib/VxGlobals.h>

#define SUPPORTED_IMAGE_FILES "Image files (*.bmp *.gif *.jpg *.jpeg *.png *.pbm *.pgm *.ppm *.xbm *.xpm)"

//============================================================================
AppletEditAvatarImage::AppletEditAvatarImage( AppCommon& app, QWidget * parent )
: AppletBase( OBJNAME_APPLET_EDIT_AVATAR_IMAGE, app, parent )
, m_ThumbMgr( app.getEngine().getThumbMgr() )
{
    setAppletType( eAppletEditAvatarImage );
    ui.setupUi( getContentItemsFrame() );
	setTitleBarText( DescribeApplet( m_EAppletType ) );
    ui.m_ServiceSettingsWidget->setPluginType( ePluginTypeHostPeerUser );
    ui.m_ServiceSettingsWidget->setViewServiceVisible( false );

    m_MyIdent = m_MyApp.getAppGlobals().getUserIdent();
    m_strOrigOnlineName = m_MyIdent->getOnlineName();
    m_strOrigMoodMessage = m_MyIdent->getOnlineDescription();
    if( m_MyIdent->getAvatarGuid().isVxGUIDValid() )
    {
        ThumbInfo * thumbAsset = dynamic_cast<ThumbInfo *>(m_ThumbMgr.findAsset( m_MyIdent->getAvatarGuid() ));
        if( thumbAsset )
        {
            ui.m_ThumbnailEditWidget->loadFromAsset( thumbAsset );
        }
    }

    connect( ui.m_ApplyAboutMeButton, SIGNAL( clicked() ), this, SLOT( onApplyButClick() ) );

	m_MyApp.activityStateChange( this, true );
}

//============================================================================
//! Implement the OnClickListener callback    
void AppletEditAvatarImage::onApplyButClick( void )
{
    bool assetExists = ui.m_ThumbnailEditWidget->isAssetIdValid();
    if( assetExists )
    {
        ThumbInfo * existingAsset = dynamic_cast<ThumbInfo *>( m_ThumbMgr.findAsset( ui.m_ThumbnailEditWidget->getAssetId() ) );
        if( existingAsset )
        {
            if( ui.m_ThumbnailEditWidget->updateThumbAsset( *existingAsset ) )
            {
                // setup identity with updated avatar image
                m_MyIdent->setAvatarGuid( existingAsset->getAssetUniqueId(), existingAsset->getModifiedTime() );

                m_Engine.getThumbMgr().fromGuiThumbUpdated( *existingAsset );

                // technically this should have resource lock but because there is no damage if read wrong by another thread during write it is ok to set directly
                m_Engine.getMyNetIdent()->setAvatarGuid( existingAsset->getAssetUniqueId(), existingAsset->getModifiedTime() );

                // notify others of change to identity
                m_MyApp.updateMyIdent( m_MyIdent );

                QString msgText = QObject::tr( "Applied Avatar Image Changes " );
                QMessageBox::information( this, QObject::tr( "Applied Avatar Image Success" ), msgText );
            }
        }
        else
        {
            assetExists = false;
        }
    }
    
    if( !assetExists && ui.m_ThumbnailEditWidget->getIsUserPickedImage()  )
    {
        ThumbInfo thumbInfo;
        if( ui.m_ThumbnailEditWidget->generateThumbAsset( thumbInfo ) )
        {
            // setup identity with new avatar image
            m_MyIdent->setAvatarGuid( thumbInfo.getAssetUniqueId(), thumbInfo.getModifiedTime() );

            m_Engine.getThumbMgr().fromGuiThumbCreated( thumbInfo );

            // technically this should have resource lock but because there is no damage if read wrong by another thread during write it is ok to set directly
            m_Engine.getMyNetIdent()->setAvatarGuid( thumbInfo.getAssetUniqueId(), thumbInfo.getModifiedTime() );

            // notify others of change to identity
            m_MyApp.updateMyIdent( m_MyIdent );

            QString msgText = QObject::tr( "Applied Avatar Image Changes " );
            QMessageBox::information( this, QObject::tr( "Applied Avatar Image Success" ), msgText );
        }
    }
}
