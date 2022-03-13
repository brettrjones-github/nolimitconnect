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

#include "AppletCamSettings.h"

#include "FileShareItemWidget.h"
#include "MyIcons.h"
#include "AppletPopupMenu.h"
#include "AppGlobals.h"
#include "FileItemInfo.h"
#include "FileActionMenu.h"
#include "GuiHelpers.h"
#include "GuiParams.h"

#include <ptop_src/ptop_engine_src/P2PEngine/P2PEngine.h>
#include <ptop_src/ptop_engine_src/AssetMgr/AssetMgr.h>

#include <PktLib/VxSearchDefs.h>
#include <NetLib/VxFileXferInfo.h>
#include <CoreLib/VxFileInfo.h>
#include <CoreLib/VxGlobals.h>
#include <VxVideoLib/VxVideoLib.h>

#include <QResizeEvent>
#include <QMessageBox>
#include <QTimer>

//============================================================================
AppletCamSettings::AppletCamSettings( AppCommon& app, QWidget * parent )
: AppletBase( OBJNAME_APPLET_CAM_SETTINGS, app, parent )
, m_CloseAppletTimer( new QTimer( this ) )
{
    setAppletType( eAppletCamSettings );
    ui.setupUi( getContentItemsFrame() );
    setTitleBarText( DescribeApplet( m_EAppletType ) );

    if( !m_MyApp.getCamLogic().isCamAvailable() )
    {
        QMessageBox::warning( this, QObject::tr( "Camera Capture" ), QObject::tr( "No Camera Source Available." ) );
        connect( m_CloseAppletTimer, SIGNAL( timeout() ), this, SLOT( onCancelButClick() ) );
        m_CloseAppletTimer->setSingleShot( true );
        m_CloseAppletTimer->start( 1000 );
        return;
    }

    if( m_HisIdent  )
    {
        setupCamFeed( &m_HisIdent->getNetIdent() );
    }
    else
    {
        setupCamFeed( m_MyApp.getAppGlobals().getUserIdent() );
    }

    startCamFeed();

    m_MyApp.activityStateChange( this, true );
}

//============================================================================
AppletCamSettings::~AppletCamSettings()
{
    m_MyApp.activityStateChange( this, false );
}

//============================================================================
void AppletCamSettings::setupCamFeed( VxNetIdent* feedNetIdent )
{
    if( !feedNetIdent )
    {
        LogMsg( LOG_ERROR, "setupCamFeed null feed ident" );
        vx_assert( false );
        return;
    }

    m_CamFeedIdent = feedNetIdent;
    m_CamFeedId = feedNetIdent->getMyOnlineId();
    m_IsMyself = m_CamFeedId == m_MyApp.getMyOnlineId();
    if( m_IsMyself )
    {
        setMuteSpeakerVisibility( false );
        setMuteMicrophoneVisibility( true );
        setCameraButtonVisibility( true );
        ui.m_CamVidWidget->setRecordFilePath( VxGetPersonalRecordDirectory().c_str() );
        ui.m_CamVidWidget->setRecordFriendName( m_CamFeedIdent->getOnlineName() );
        ui.m_CamVidWidget->showAllControls( true );
        ui.m_CamVidWidget->enableCamFeedControls( false );
    }
    else
    {
        setMuteSpeakerVisibility( true );
        setMuteMicrophoneVisibility( false );
        setCameraButtonVisibility( false ); 
        ui.m_CamVidWidget->showAllControls( true );
        ui.m_CamVidWidget->enableCamSourceControls( false );
        ui.m_CamVidWidget->setRecordFilePath( VxGetDownloadsDirectory().c_str() );
        ui.m_CamVidWidget->setRecordFriendName( m_CamFeedIdent->getOnlineName() );
    }

    ui.m_CamVidWidget->setVideoFeedId( m_CamFeedId );
}

//============================================================================
void AppletCamSettings::startCamFeed( void )
{
    if( !isCamFeedStarted() )
    {
        if( m_CamFeedIdent )
        {
            if( ePluginTypeInvalid != m_ePluginType )
            {
                m_FromGui.fromGuiStartPluginSession( m_ePluginType, m_CamFeedIdent->getMyOnlineId() );
            }

            //m_Engine.fromGuiWantMediaInput( m_CamFeedIdent->getMyOnlineId(), eMediaInputVideoJpgSmall, true );
            //m_Engine.fromGuiWantMediaInput( eMediaInputVideoJpgSmall, this, this, true );

            if( isMyself() )
            {

            }

            setIsCamFeedStarted( true );
        }
        else
        {
            LogMsg( LOG_ERROR, "startCamFeed null feed ident" );
            vx_assert( false );
        }
    }
}

//============================================================================
void AppletCamSettings::stopCamFeed( void )
{
    if( isCamFeedStarted() )
    {
        setIsCamFeedStarted( false );
        if( m_CamFeedIdent )
        {
            //m_Engine.fromGuiWantMediaInput( m_CamFeedIdent->getMyOnlineId(), eMediaInputVideoJpgSmall, false );
            if( ePluginTypeInvalid != m_ePluginType )
            {
                m_FromGui.fromGuiStopPluginSession( m_ePluginType, m_CamFeedIdent->getMyOnlineId() );
            }
        }
        else
        {
            LogMsg( LOG_ERROR, "stopCamFeed null feed ident" );
            vx_assert( false );
        }
    }
}

//============================================================================
void AppletCamSettings::showEvent( QShowEvent * ev )
{
    // don't call AppletPeerBase::showEvent ... we don't want plugin offer/response for web cam server or client
    AppletBase::showEvent( ev );
    m_MyApp.wantToGuiActivityCallbacks( this, this, true );
}

//============================================================================
void AppletCamSettings::hideEvent( QHideEvent * ev )
{
    // don't call AppletPeerBase::hideEvent ... we don't want plugin offer/response for web cam server or client
    m_MyApp.wantToGuiActivityCallbacks( this, this, false );
    AppletBase::hideEvent( ev );
    //m_Engine.fromGuiWantMediaInput( m_HisIdent->getMyOnlineId(), eMediaInputVideoJpgSmall, false );
}

//============================================================================
void AppletCamSettings::closeEvent( QCloseEvent * ev )
{
    // don't call AppletPeerBase::hideEvent ... we don't want plugin offer/response for web cam server or client
    //m_FromGui.fromGuiStopPluginSession( m_ePluginType, m_HisIdent->getMyOnlineId(), 0 );
    //if( m_bIsMyself )
    //{
    //    m_MyApp.getAppGlobals().getUserIdent()->setHasSharedWebCam( false );
    //}

    stopCamFeed();
    AppletBase::closeEvent( ev );
}

//============================================================================
void AppletCamSettings::setMuteSpeakerVisibility( bool visible )
{
    //ui.m_TitleBarWidget->setMuteSpeakerVisibility( visible );
}

//============================================================================
void AppletCamSettings::setMuteMicrophoneVisibility( bool visible )
{
    //ui.m_TitleBarWidget->setMuteMicrophoneVisibility( visible );
}

//============================================================================
void AppletCamSettings::setCameraButtonVisibility( bool visible )
{
    //ui.m_TitleBarWidget->setCameraButtonVisibility( visible );
}

////============================================================================
//void AppletCamSettings::slotMuteMicButtonClicked( bool muteMicrophone )
//{
//	// normally we would call m_Engine.fromGuiMuteMicrophone but that mutes all including any video chat sessions... so send custom ctrl to PluginWebCamServer
//	//m_Engine.fromGuiPluginControl(	m_ePluginType, 
//	//								m_CamFeedIdent->getMyOnlineId(), 
//	//								"Mic", 
//	//								"Mute",
//	//								muteMicrophone ? 1 : 0 );
//
//	m_Engine.fromGuiMuteMicrophone( muteMicrophone );
//}
//
////============================================================================
//void AppletCamSettings::slotMuteSpeakerButtonClicked( bool muteSpeaker )
//{
//	m_Engine.fromGuiMuteSpeaker( muteSpeaker );
//}

//============================================================================
void AppletCamSettings::resizeBitmapToFitScreen( QLabel * VideoScreen, QImage& oPicBitmap )
{
    QSize screenSize( VideoScreen->width(), VideoScreen->height() );
    oPicBitmap = oPicBitmap.scaled(screenSize, Qt::KeepAspectRatio, Qt::SmoothTransformation );
}

//============================================================================
void AppletCamSettings::playVideoFrame( VxGUID& feedId, unsigned char * pu8Jpg, unsigned long u32JpgLen, int motion0To100000 )
{
    ui.m_CamVidWidget->playVideoFrame( feedId, pu8Jpg, u32JpgLen, motion0To100000 );
}

//============================================================================
void AppletCamSettings::webCamSourceOffline()
{
    if( m_CamFeedIdent )
    {
        std::string statMsg = m_CamFeedIdent->getOnlineName();
        statMsg += "Cam Is Offline";
    }

    ui.m_CamVidWidget->showOfflineImage();
}

//============================================================================
void AppletCamSettings::slotToGuiRxedOfferReply( GuiOfferSession * offerReply )
{
    if( ( ePluginTypeCamServer == offerReply->getPluginType() )
        && ( m_HisIdent->getMyOnlineId() == offerReply->getHisIdent()->getMyOnlineId() ) )
    {
        if( eOfferResponseBusy == offerReply->getOfferResponse() )
        {
            playSound( eSndDefBusy );
        }

        if( eOfferResponseAccept != offerReply->getOfferResponse() )
        {
            webCamSourceOffline();
        }
    }
}; 

//============================================================================
void AppletCamSettings::slotToGuiSessionEnded( GuiOfferSession * offer )
{
    if( ( ePluginTypeCamServer == offer->getPluginType() )
        && ( m_HisIdent->getMyOnlineId() == offer->getHisIdent()->getMyOnlineId() ) )
    {
        webCamSourceOffline();
    }
}; 

//============================================================================
void AppletCamSettings::slotToGuiContactOffline( VxNetIdent * friendIdent )
{
    if( m_HisIdent->getMyOnlineId() == friendIdent->getMyOnlineId() )
    {
        webCamSourceOffline();
    }
}

//============================================================================
//! Implement the OnClickListener callback    
void AppletCamSettings::onCancelButClick( void )
{
    onBackButtonClicked();
}

//============================================================================
void AppletCamSettings::onCloseEvent( void )
{
    stopCamFeed();
    AppletBase::onCloseEvent();
}

//============================================================================
void AppletCamSettings::callbackVideoJpgSmall( void * userData, VxGUID& vidFeedId, uint8_t * jpgData, uint32_t jpgDataLen, int motion0to100000 )
{
    if( jpgData && jpgDataLen && ( vidFeedId == m_CamFeedId ) )
    {
        VidWidget* camScreen = ui.m_CamVidWidget;
        if( camScreen )
        {
            camScreen->playVideoFrame( vidFeedId, jpgData, jpgDataLen, motion0to100000 );
        }
    }
}

//============================================================================
void AppletCamSettings::toGuiClientPlayVideoFrame( void * userData, VxGUID& vidFeedId, uint8_t * jpgData, uint32_t jpgDataLen, int motion0to100000 )
{
    callbackVideoJpgSmall( userData, vidFeedId, jpgData, jpgDataLen, motion0to100000 );
}
