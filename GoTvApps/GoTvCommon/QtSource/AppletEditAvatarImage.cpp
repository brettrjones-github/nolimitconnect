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

#include <GoTvCore/GoTvP2P/P2PEngine/EngineSettings.h>
#include <GoTvCore/GoTvP2P/AssetMgr/AssetMgr.h>

#include <CoreLib/VxFileUtil.h>
#include <CoreLib/VxDebug.h>
#include <CoreLib/VxGlobals.h>

#define SUPPORTED_IMAGE_FILES "Image files (*.bmp *.gif *.jpg *.jpeg *.png *.pbm *.pgm *.ppm *.xbm *.xpm)"

//============================================================================
AppletEditAvatarImage::AppletEditAvatarImage( AppCommon& app, QWidget * parent )
: AppletBase( OBJNAME_APPLET_EDIT_AVATAR_IMAGE, app, parent )
, m_AssetMgr( app.getEngine().getAssetMgr() )
{
    setAppletType( eAppletEditAvatarImage );
    ui.setupUi( getContentItemsFrame() );
	setTitleBarText( DescribeApplet( m_EAppletType ) );
    ui.m_ServiceSettingsWidget->setPluginType( ePluginTypeAvatarImage );
    ui.m_ServiceSettingsWidget->setViewServiceVisible( false );

    m_MyIdent = m_MyApp.getAppGlobals().getUserIdent();
    m_strOrigOnlineName = m_MyIdent->getOnlineName();
    m_strOrigMoodMessage = m_MyIdent->getOnlineDescription();
    if( m_MyIdent->getAvatarGuid().isVxGUIDValid() )
    {
        AssetInfo * thumbAsset = m_AssetMgr.findAsset( m_MyIdent->getAvatarGuid() );
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
        AssetInfo * existingAsset = m_MyApp.getEngine().getAssetMgr().findAsset( ui.m_ThumbnailEditWidget->getAssetId() );
        if( existingAsset )
        {
            if( m_MyIdent->getAvatarGuid().isVxGUIDValid() && m_MyIdent->getAvatarGuid() == existingAsset->getAssetUniqueId() )
            {
                QString msgText = QObject::tr( "No Avatar Image Changes " );
                QMessageBox::information( this, QObject::tr( "Avatar Image is not changed" ), msgText );
            }
            else
            {
                // setup identity with new avatar image
                m_MyIdent->setAvatarGuid( existingAsset->getAssetUniqueId() );
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
        AssetInfo assetInfo;
        if( ui.m_ThumbnailEditWidget->generateThumbAsset( assetInfo ) )
        {
            // setup identity with new avatar image
            m_MyIdent->setAvatarGuid( assetInfo.getAssetUniqueId() );
            m_MyApp.updateMyIdent( m_MyIdent );

            QString msgText = QObject::tr( "Applied Avatar Image Changes " );
            QMessageBox::information( this, QObject::tr( "Applied Avatar Image Success" ), msgText );
        }
    }
}
