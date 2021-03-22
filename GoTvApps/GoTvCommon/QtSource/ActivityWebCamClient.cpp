//============================================================================
// Copyright (C) 2013 Brett R. Jones
// Issued to MIT style license by Brett R. Jones in 2017
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
#include "AppCommon.h"
#include "ActivityWebCamClient.h"
#include "AppGlobals.h"
#include "GuiOfferSession.h"

#include <VxVideoLib/VxVideoLib.h>
#include <CoreLib/VxGlobals.h>

#include <QMessageBox>

//============================================================================
ActivityWebCamClient::ActivityWebCamClient( AppCommon&			    app,
											VxNetIdent *			netIdent, 
											QWidget *				parent )
: AppletPeerBase( OBJNAME_ACTIVITY_WEB_CAM_CLIENT, app, ePluginTypeCamServer, netIdent, parent, eAppletMessenger, Qt::Dialog | Qt::WindowSystemMenuHint | Qt::WindowMinMaxButtonsHint )
, m_bIsMyself( false )
{
	m_bIsMyself = ( netIdent->getMyOnlineId() == m_MyApp.getAppGlobals().getUserIdent()->getMyOnlineId() );
	setupActivityWebCamClient();
	connect( this, SIGNAL(finished(int)), this, SLOT(onAboutToDestroyDialog()));
	connect( ui.m_TitleBarWidget, SIGNAL(signalBackButtonClicked()), this, SLOT(accept()) );
	connect( ui.m_TitleBarWidget, SIGNAL(signalMuteMicButtonClicked(bool)), this, SLOT(slotMuteMicButtonClicked(bool)) );

	if( m_bIsMyself )
	{
		m_OfferSessionLogic.setIsServerSession( true );
		setMuteSpeakerVisibility( false );
		setMuteMicrophoneVisibility( true );
		setCameraButtonVisibility( true );
		ui.m_FriendIdentWidget->setVisible( false );
		ui.m_CamVidWidget->setRecordFilePath( VxGetPersonalRecordDirectory().c_str() );
		ui.m_CamVidWidget->setRecordFriendName( m_HisIdent->getOnlineName() );
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
		ui.m_CamVidWidget->setRecordFriendName( m_HisIdent->getOnlineName() );
		m_OfferSessionLogic.sendOfferOrResponse();
	}

	connect( this, SIGNAL(signalToGuiRxedOfferReply(GuiOfferSession *)),		this, SLOT(slotToGuiRxedOfferReply(GuiOfferSession *)) );
	connect( this, SIGNAL(signalToGuiPluginSessionEnded(GuiOfferSession *)),	this, SLOT(slotToGuiPluginSessionEnded(GuiOfferSession *)) );
	connect( this, SIGNAL(signalToGuiContactOffline(VxNetIdent *)),				this, SLOT(slotToGuiContactOffline(VxNetIdent *)) );

    m_FromGui.fromGuiStartPluginSession( m_ePluginType, m_HisIdent->getMyOnlineId() );
	m_Engine.fromGuiWantMediaInput( m_HisIdent->getMyOnlineId(), eMediaInputVideoJpgSmall, true );
	this->resize( 400, 580 );
}

//============================================================================
void ActivityWebCamClient::setupActivityWebCamClient()
{
	ui.setupUi( this );
    setupBaseWidgets( ui.m_TitleBarWidget, ui.m_FriendIdentWidget, nullptr, nullptr );
    connectBarWidgets();

	ui.m_CamVidWidget->setVideoFeedId( m_HisIdent->getMyOnlineId() );
	setVidCamWidget( ui.m_CamVidWidget );

	if( m_bIsMyself )
	{
		ui.m_TitleBarWidget->setTitleBarText( tr("Web Cam Server") );
		this->setWindowTitle( tr("Web Cam Server") );
	}
	else
	{
		std::string titleText = m_HisIdent->getOnlineName();
		QString partText = QObject::tr("-Shared Web Cam");
		titleText += partText.toUtf8().constData();
		ui.m_TitleBarWidget->setTitleBarText( titleText.c_str() );
		this->setWindowTitle( titleText.c_str() );
	}

	ui.m_TitleBarWidget->enableAudioControls( true );
	ui.m_TitleBarWidget->enableVideoControls( true );
}

//============================================================================
void ActivityWebCamClient::showEvent( QShowEvent * ev )
{
	// don't call AppletPeerBase::showEvent ... we don't want plugin offer/response for web cam server or client
	AppletPeerBase::showEvent( ev );
	//m_Engine.fromGuiWantMediaInput( m_HisIdent->getMyOnlineId(), eMediaInputVideoJpgSmall, true );
}

//============================================================================
void ActivityWebCamClient::hideEvent( QHideEvent * ev )
{
	// don't call AppletPeerBase::hideEvent ... we don't want plugin offer/response for web cam server or client
	AppletPeerBase::hideEvent( ev );
	//m_Engine.fromGuiWantMediaInput( m_HisIdent->getMyOnlineId(), eMediaInputVideoJpgSmall, false );
}

//============================================================================
void ActivityWebCamClient::closeEvent( QCloseEvent * ev )
{
	// don't call AppletPeerBase::hideEvent ... we don't want plugin offer/response for web cam server or client
    m_FromGui.fromGuiStopPluginSession( m_ePluginType, m_HisIdent->getMyOnlineId(), 0 );
	if( m_bIsMyself )
	{
		m_MyApp.getAppGlobals().getUserIdent()->setHasSharedWebCam( false );
		// setHasSharedWebCam was handled in PluginWebCamServer::fromGuiStopPluginSession
		//PluginWebCamServer called m_Engine.setHasSharedWebCam(false); in fromGuiStopPluginSession
		//m_Engine.setHasSharedWebCam(false);
	}

	AppletPeerBase::closeEvent( ev );
}

//============================================================================
void ActivityWebCamClient::setMuteSpeakerVisibility( bool visible )
{
	ui.m_TitleBarWidget->setMuteSpeakerVisibility( visible );
}

//============================================================================
void ActivityWebCamClient::setMuteMicrophoneVisibility( bool visible )
{
	ui.m_TitleBarWidget->setMuteMicrophoneVisibility( visible );
}

//============================================================================
void ActivityWebCamClient::setCameraButtonVisibility( bool visible )
{
	ui.m_TitleBarWidget->setCameraButtonVisibility( visible );
}

////============================================================================
//void ActivityWebCamClient::slotMuteMicButtonClicked( bool muteMicrophone )
//{
//	// normally we would call m_Engine.fromGuiMuteMicrophone but that mutes all including any video chat sessions... so send custom ctrl to PluginWebCamServer
//	//m_Engine.fromGuiPluginControl(	m_ePluginType, 
//	//								m_HisIdent->getMyOnlineId(), 
//	//								"Mic", 
//	//								"Mute",
//	//								muteMicrophone ? 1 : 0 );
//
//	m_Engine.fromGuiMuteMicrophone( muteMicrophone );
//}
//
////============================================================================
//void ActivityWebCamClient::slotMuteSpeakerButtonClicked( bool muteSpeaker )
//{
//	m_Engine.fromGuiMuteSpeaker( muteSpeaker );
//}

//============================================================================
void ActivityWebCamClient::onAboutToDestroyDialog( void )
{
}

//============================================================================
void ActivityWebCamClient::resizeBitmapToFitScreen( QLabel * VideoScreen, QImage& oPicBitmap )
{
	QSize screenSize( VideoScreen->width(), VideoScreen->height() );
	oPicBitmap = oPicBitmap.scaled(screenSize, Qt::KeepAspectRatio);
}

//============================================================================
void ActivityWebCamClient::playVideoFrame( VxGUID& feedId, unsigned char * pu8Jpg, unsigned long u32JpgLen, int motion0To100000 )
{
	ui.m_CamVidWidget->playVideoFrame( feedId, pu8Jpg, u32JpgLen, motion0To100000 );
}

//============================================================================
void ActivityWebCamClient::webCamSourceOffline()
{
	std::string statMsg = m_HisIdent->getOnlineName();
	statMsg += "'s Shared Cam Is Offline";

	m_MyApp.toGuiUserMessage( statMsg.c_str() );
	ui.m_CamVidWidget->showOfflineImage();
}

//============================================================================
void ActivityWebCamClient::slotToGuiRxedOfferReply( GuiOfferSession * offerReply )
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
void ActivityWebCamClient::slotToGuiSessionEnded( GuiOfferSession * offer )
{
	if( ( ePluginTypeWebServer == offer->getPluginType() )
		&& ( m_HisIdent->getMyOnlineId() == offer->getHisIdent()->getMyOnlineId() ) )
	{
		webCamSourceOffline();
	}
}; 

//============================================================================
void ActivityWebCamClient::slotToGuiContactOffline( VxNetIdent * friendIdent )
{
	if( m_HisIdent->getMyOnlineId() == friendIdent->getMyOnlineId() )
	{
		webCamSourceOffline();
	}
}

