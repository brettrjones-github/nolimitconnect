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
// http://www.nolimitconnect.org
//============================================================================

#include "AppletEditAvatarImage.h"
#include "AppCommon.h"
#include "AppSettings.h"
#include "MyIcons.h"
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
    ui.m_ThumbnailEditWidget->setThumnailIsCircular( true );
    ui.m_StatusLabel->setVisible( false );

    m_MyIdent = m_MyApp.getAppGlobals().getUserIdent();
    m_strOrigOnlineName = m_MyIdent->getOnlineName();
    m_strOrigMoodMessage = m_MyIdent->getOnlineDescription();
    if( m_MyIdent->getAvatarThumbGuid().isVxGUIDValid() )
    {
        ThumbInfo * thumbAsset = dynamic_cast<ThumbInfo *>(m_ThumbMgr.findAsset( m_MyIdent->getAvatarThumbGuid() ));
        if( thumbAsset )
        {
            ui.m_ThumbnailEditWidget->loadFromAsset( thumbAsset );
        }
    }

    connect( ui.m_ApplyAvatarButton, SIGNAL( clicked() ), this, SLOT( onApplyButClick() ) );
    connect( ui.m_RemoveAvatarButton, SIGNAL( clicked() ), this, SLOT( onRemoveButClick() ) );

	m_MyApp.activityStateChange( this, true );
}

//============================================================================    
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
                emit signalAvatarImageChanged( existingAsset );

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
            emit signalAvatarImageChanged( &thumbInfo );

            QString msgText = QObject::tr( "Applied Avatar Image Changes " );
            QMessageBox::information( this, QObject::tr( "Applied Avatar Image Success" ), msgText );
        }
    }

    closeApplet();
}

//============================================================================    
void AppletEditAvatarImage::onRemoveButClick( void )
{
    QString removeConfirmmsgText = QObject::tr( "Are you sure you want to remove your avatar image? " );
    if( QMessageBox::Yes == QMessageBox::question( this, QObject::tr( "Remove Avatar Image" ), removeConfirmmsgText ) )
    {
        // setup identity with null avatar image
        VxGUID nullGuid;
        m_MyIdent->setAvatarGuid( nullGuid, 0 );

        // technically this should have resource lock but because there is no damage if read wrong by another thread during write it is ok to set directly
        m_Engine.getMyNetIdent()->setAvatarGuid( nullGuid, 0 );

        // notify others of change to identity
        m_MyApp.updateMyIdent( m_MyIdent );
        emit signalAvatarImageRemoved();

        QString msgText = QObject::tr( "Remove Avatar Image Success" );
        QMessageBox::information( this, QObject::tr( "Remove Avatar Image" ), msgText );  
        closeApplet();
    }
    else
    {
        QString msgText = QObject::tr( "Remove Avatar Image canceled " );
        QMessageBox::information( this, msgText, msgText );
    }
}
