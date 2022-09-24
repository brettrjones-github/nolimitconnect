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

#include "AppletCamClient.h"

#include "AppCommon.h"

#include <CoreLib/VxDebug.h>
#include <CoreLib/VxGlobals.h>

//============================================================================
AppletCamClient::AppletCamClient( AppCommon& app, QWidget * parent )
: AppletBase( OBJNAME_APPLET_CAM_CLIENT, app, parent )
, m_CloseAppletTimer( new QTimer( this ) )
{
	setAppletType( eAppletCamClient );
	setPluginType( ePluginTypeCamClient );
    ui.setupUi( getContentItemsFrame() );
	setTitleBarText( DescribeApplet( m_EAppletType ) );
	connect( this, SIGNAL(signalBackButtonClicked()), this, SLOT( closeApplet()) );

	m_MyApp.activityStateChange( this, true );
	m_MyApp.wantToGuiActivityCallbacks( this, true );
}

//============================================================================
AppletCamClient::~AppletCamClient()
{
    m_MyApp.activityStateChange( this, false );
	m_MyApp.wantToGuiActivityCallbacks( this, false );
}

//============================================================================
void AppletCamClient::startCamFeed( void )
{
    if( !isCamFeedStarted() )
    {
        if( m_CamFeedIdent )
        {
            if( ePluginTypeInvalid != m_ePluginType )
            {
                m_FromGui.fromGuiStartPluginSession( m_ePluginType, m_CamFeedIdent->getMyOnlineId() );
            }

            setIsCamFeedStarted( true );
        }
        else
        {
            LogMsg( LOG_ERROR, "AppletCamClient::startCamFeed null feed ident" );
            vx_assert( false );
        }
    }
}

//============================================================================
void AppletCamClient::stopCamFeed( void )
{
    if( isCamFeedStarted() )
    {
        setIsCamFeedStarted( false );
        if( m_CamFeedIdent )
        {
            if( ePluginTypeInvalid != m_ePluginType )
            {
                m_FromGui.fromGuiStopPluginSession( m_ePluginType, m_CamFeedIdent->getMyOnlineId() );
            }
        }
        else
        {
            LogMsg( LOG_ERROR, "AppletCamClient::stopCamFeed null feed ident" );
            vx_assert( false );
        }
    }
}

//============================================================================
void AppletCamClient::showEvent( QShowEvent* ev )
{
    // don't call AppletPeerBase::showEvent ... we don't want plugin offer/response for web cam server or client
    AppletBase::showEvent( ev );
    m_MyApp.wantToGuiActivityCallbacks( this, true );
}

//============================================================================
void AppletCamClient::hideEvent( QHideEvent* ev )
{
    // don't call AppletPeerBase::hideEvent ... we don't want plugin offer/response for web cam server or client
    m_MyApp.wantToGuiActivityCallbacks( this, false );
    AppletBase::hideEvent( ev );
}

//============================================================================
void AppletCamClient::closeEvent( QCloseEvent* ev )
{
    // don't call AppletPeerBase::hideEvent ... we don't want plugin offer/response for web cam server or client

    stopCamFeed();
    AppletBase::closeEvent( ev );
}

//============================================================================
void AppletCamClient::setMuteSpeakerVisibility( bool visible )
{
    //ui.m_TitleBarWidget->setMuteSpeakerVisibility( visible );
}

//============================================================================
void AppletCamClient::setMuteMicrophoneVisibility( bool visible )
{
    //ui.m_TitleBarWidget->setMuteMicrophoneVisibility( visible );
}

//============================================================================
void AppletCamClient::setCameraButtonVisibility( bool visible )
{
    //ui.m_TitleBarWidget->setCameraButtonVisibility( visible );
}

//============================================================================
void AppletCamClient::resizeBitmapToFitScreen( QLabel* VideoScreen, QImage& oPicBitmap )
{
    QSize screenSize( VideoScreen->width(), VideoScreen->height() );
    oPicBitmap = oPicBitmap.scaled( screenSize, Qt::KeepAspectRatio, Qt::SmoothTransformation );
}

//============================================================================
void AppletCamClient::webCamSourceOffline()
{
    if( m_CamFeedIdent )
    {
        std::string statMsg = m_CamFeedIdent->getOnlineName();
        statMsg += "Cam Is Offline";
    }

    ui.m_CamVidWidget->showOfflineImage();
}

//============================================================================
void AppletCamClient::slotToGuiRxedOfferReply( GuiOfferSession* offerReply )
{
    if( (ePluginTypeCamServer == offerReply->getPluginType())
        && (m_HisIdent->getMyOnlineId() == offerReply->getHisIdent()->getMyOnlineId()) )
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
void AppletCamClient::slotToGuiSessionEnded( GuiOfferSession* offer )
{
    if( (ePluginTypeCamServer == offer->getPluginType())
        && (m_HisIdent->getMyOnlineId() == offer->getHisIdent()->getMyOnlineId()) )
    {
        webCamSourceOffline();
    }
};

//============================================================================
void AppletCamClient::slotToGuiContactOffline( VxNetIdent* friendIdent )
{
    if( m_HisIdent->getMyOnlineId() == friendIdent->getMyOnlineId() )
    {
        webCamSourceOffline();
    }
}

//============================================================================
void AppletCamClient::setupCamFeed( GuiUser* feedNetIdent )
{
    if( !feedNetIdent )
    {
        LogMsg( LOG_ERROR, "setupCamFeed null feed ident" );
        vx_assert( false );
        return;
    }

    m_CamFeedIdent = feedNetIdent;
    m_HisIdent = feedNetIdent;
    m_CamFeedId = feedNetIdent->getMyOnlineId();
    m_IsMyself = m_CamFeedId == m_MyApp.getMyOnlineId();
    if( m_IsMyself )
    {
        setMuteSpeakerVisibility( false );
        setMuteMicrophoneVisibility( true );
        setCameraButtonVisibility( true );
    }
    else
    {
        setMuteSpeakerVisibility( true );
        setMuteMicrophoneVisibility( false );
        setCameraButtonVisibility( false );
    }

    ui.m_CamVidWidget->showAllControls( true );
    ui.m_CamVidWidget->enableCamSourceControls( false );
    ui.m_CamVidWidget->setRecordFilePath( VxGetDownloadsDirectory().c_str() );
    ui.m_CamVidWidget->setRecordFriendName( m_CamFeedIdent->getOnlineName().c_str() );
    ui.m_CamVidWidget->setVideoFeedId( m_CamFeedId, eAppModuleCamClient );

    startCamFeed();
}
