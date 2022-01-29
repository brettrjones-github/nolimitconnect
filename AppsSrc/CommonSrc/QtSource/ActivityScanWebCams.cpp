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

#include "ActivityScanWebCams.h"
#include "MyIcons.h"
#include "AppletPopupMenu.h"
#include "AppCommon.h"

#include <ptop_src/ptop_engine_src/P2PEngine/P2PEngine.h>
#include <PktLib/VxSearchDefs.h>

namespace
{
	const int SCAN_INTERVAL_SEC = 20;
	const int COUNTDOWN_INTERVAL_MS = 1000;
}

//============================================================================
ActivityScanWebCams::ActivityScanWebCams(	AppCommon&	app, 
											QWidget *		parent )
: ActivityBase( OBJNAME_ACTIVITY_SCAN_WEB_CAMS, app, parent, eAppletScanWebCam, true ) //Qt::Popup ) //
, m_HisIdent( NULL )
, m_iCountdownCnt( 0 )
, m_bPaused( false )
, m_CountdownTimer( new QTimer( this ) )
, m_eScanType(eScanTypeCamServer)
, m_bIconConnected( false )
, m_bSearchComplete( false )
, m_ShowNextCam( false )
, m_IsScanning( false )
{
	setPluginType( ePluginTypeCamServer );
	ui.setupUi(this);

    connectBarWidgets();

    connect( this, SIGNAL(signalNewWebCamSession( GuiUser * )), this, SLOT(slotNewWebCamSession( GuiUser * )));
    connect( this, SIGNAL(signalPlayVideoFrame( QImage, int )), this, SLOT(slotPlayVideoFrame( QImage, int )));
    connect( this, SIGNAL(signalPlayAudio(unsigned short *, unsigned short)), this, SLOT(signalPlayAudio(unsigned short *, unsigned short)));

    connect(m_CountdownTimer, SIGNAL(timeout()), this, SLOT(onCountdownTimer()));
    connect( ui.m_TitleBarWidget, SIGNAL(signalBackButtonClicked()), this, SLOT( closeApplet()));
	connect( this, SIGNAL(finished(int)), this, SLOT(slotHomeButtonClicked()));

	connect( ui.m_StartScanButton, SIGNAL(clicked()), this, SLOT(slotStartScanClicked()));
	connect( ui.m_PauseScanButton, SIGNAL(clicked()), this, SLOT(slotPauseScanClicked()));
	connect( ui.m_NextScanButton, SIGNAL(clicked()), this, SLOT(slotNextScanClicked()));
	connect( this, SIGNAL(signalSearchComplete()), this, SLOT(slotSearchComplete()) );

	ui.FriendIdentWidget->setVisible( false );

	setTitle( "Scan Web Cam Servers" );
	m_CountdownTimer->setInterval( COUNTDOWN_INTERVAL_MS );
	m_CountdownTimer->start();
	m_MyApp.wantToGuiActivityCallbacks( this, this, true );

	slotStartScanClicked();
}

//============================================================================
ActivityScanWebCams::~ActivityScanWebCams()
{
	m_MyApp.wantToGuiActivityCallbacks( this, this, false );
}

//============================================================================
void ActivityScanWebCams::setTitle( QString strTitle )
{
	ui.m_TitleBarWidget->setTitleBarText(strTitle);
}

//============================================================================
void ActivityScanWebCams::setScanStatusText( QString strMsg )
{
	m_MyApp.toGuiStatusMessage( strMsg.toUtf8().constData() );
}

//============================================================================
void ActivityScanWebCams::showEvent( QShowEvent * ev )
{
	// don't call AppletPeerBase::showEvent ... we don't want plugin offer/response for web cam server or client
	ActivityBase::showEvent( ev );
}

//============================================================================
void ActivityScanWebCams::hideEvent( QHideEvent * ev )
{
	// don't call AppletPeerBase::hideEvent ... we don't want plugin offer/response for web cam server or client
	ActivityBase::hideEvent( ev );
}

//============================================================================
void ActivityScanWebCams::toGuiClientScanSearchComplete( void * userData, EScanType eScanType )
{
	Q_UNUSED( userData );
	if( eScanTypeCamServer == eScanType )
	{
		emit signalSearchComplete();
	}
}

//============================================================================
void ActivityScanWebCams::slotSearchComplete()
{
	setScanStatusText( QObject::tr( "Search Complete" ) );
	m_bSearchComplete = true;
}

//============================================================================
void ActivityScanWebCams::setupIdentWidget( GuiUser * netIdent )
{
	m_HisIdent = netIdent;
	IdentWidget * poIdentWidget = ui.FriendIdentWidget;
	if( poIdentWidget )
	{
		poIdentWidget->setVisible( true );
		poIdentWidget->setIdentMenuButtonVisible( false );
		poIdentWidget->updateIdentity( netIdent );
		if( false == m_bIconConnected )
		{
			m_bIconConnected = true;
			connect( poIdentWidget, SIGNAL( signalIdentMenuButtonClicked()), this, SLOT( slotIdentMenuClicked()));
		}
	}
}

//============================================================================
void ActivityScanWebCams::toGuiScanResultSuccess( void * userData, EScanType eScanType, GuiUser * netIdent )
{
	Q_UNUSED( userData );
	if( eScanTypeCamServer == eScanType )
	{
		emit signalNewWebCamSession( netIdent );
	}
}

//============================================================================
void ActivityScanWebCams::slotNewWebCamSession( GuiUser * netIdent )
{
	m_ScanList.push_back( netIdent );
	showNextCam();
}

//============================================================================
void ActivityScanWebCams::showNextCam( void ) 
{
	if( m_bSearchComplete 
		&& ( 0 == m_ScanList.size() ) )
	{
		startStopScan( false );
		return;
	}

	if( m_ShowNextCam && m_ScanList.size() )
	{
		m_ShowNextCam = false;
		doCamConnect( m_ScanList[0] );
		m_ScanList.erase( m_ScanList.begin() );
	}

	updateAvailableGui();
}
//============================================================================
void ActivityScanWebCams::updateAvailableGui( void )
{
	ui.m_AvailableValueLabel->setText( QString("%1").arg( m_ScanList.size() ) );
}

//============================================================================
void ActivityScanWebCams::updateCountdownGui()
{
	ui.CountdownValue->setText( QString( "%1" ).arg( m_iCountdownCnt ) );
}

//============================================================================
void ActivityScanWebCams::doCamConnect( GuiUser * netIdent )
{
	if( 0 != m_HisIdent )
	{
		startWebCamSession( m_HisIdent->getMyOnlineId(), false );
        m_FromGui.fromGuiStopPluginSession( m_ePluginType, m_HisIdent->getMyOnlineId(), 0 );
		VxGUID nullGuid;
		ui.m_CamVidWidget->setVideoFeedId( nullGuid );
	}

	ui.FriendIdentWidget->setVisible( true );
	m_HisIdent = netIdent;
	if( 0 != m_HisIdent )
	{
		setupIdentWidget( m_HisIdent );
		startWebCamSession( m_HisIdent->getMyOnlineId(), true );
		ui.m_CamVidWidget->setVideoFeedId( m_HisIdent->getMyOnlineId() );
		m_Engine.fromGuiWantMediaInput( m_HisIdent->getMyOnlineId(), eMediaInputVideoJpgSmall, true );
        m_FromGui.fromGuiStartPluginSession( m_ePluginType, m_HisIdent->getMyOnlineId(), 0 );
	}
}

//============================================================================
void ActivityScanWebCams::startWebCamSession( VxGUID& onlineId, bool startSession )
{
	if( startSession )
	{
		if( 0 != ui.m_CamVidWidget )
		{
			ui.m_CamVidWidget->setVideoFeedId( onlineId );    			
		}

		m_LclSessionId.initializeWithNewVxGUID();
		m_Engine.fromGuiWantMediaInput( onlineId, eMediaInputVideoJpgSmall, true );
        m_FromGui.fromGuiStartPluginSession( m_ePluginType, onlineId, 0, m_LclSessionId );
	}
	else
	{
		m_FromGui.fromGuiStopPluginSession( m_ePluginType, onlineId, 0, m_LclSessionId );
		m_Engine.fromGuiWantMediaInput( onlineId, eMediaInputVideoJpgSmall, false );
		setCamViewToOfflineImage();			
	}		
}
//============================================================================
void ActivityScanWebCams::setCamViewToOfflineImage( void )
{
	if( 0 != ui.m_CamVidWidget )
	{
		ui.m_CamVidWidget->showOfflineImage();    			
	}
}

//============================================================================
void ActivityScanWebCams::slotHomeButtonClicked( void )
{
	m_FromGui.fromGuiStopScan( m_eScanType );
	m_MyApp.wantToGuiActivityCallbacks( this, this, false );
	if( 0 != m_HisIdent )
	{
		startWebCamSession( m_HisIdent->getMyOnlineId(), false );
	}
}

//============================================================================
void ActivityScanWebCams::toGuiClientPlayVideoFrame( void * userData, VxGUID& onlineId, uint8_t * pu8Jpg, uint32_t u32JpgDataLen, int motion0To100000 )
{
	Q_UNUSED( userData );
	ui.m_CamVidWidget->playVideoFrame( onlineId, pu8Jpg, u32JpgDataLen, motion0To100000 );
}

//============================================================================
void ActivityScanWebCams::slotPlayVideoFrame( QImage oPicBitmap, int iRotate )
{
	if( false == oPicBitmap.isNull() )
	{
		ui.m_CamVidWidget->getVideoScreen()->setPixmap( QPixmap::fromImage( oPicBitmap ) );
		ui.m_CamVidWidget->getVideoScreen()->repaint();
	}
}

//============================================================================
void ActivityScanWebCams::playAudio(  uint16_t * pu16PcmData, uint16_t u16PcmDataLen, VxGUID& onlineId )
{
	//emit signalPlayAudio(pu16PcmData, u16PcmDataLen);
}

//============================================================================
void ActivityScanWebCams::slotPlayAudio( unsigned short * pu16PcmData, unsigned short u16PcmDataLen )
{
}

//============================================================================
void ActivityScanWebCams::startCountdown()
{
	m_iCountdownCnt = SCAN_INTERVAL_SEC;
	updateCountdownGui();
}

//============================================================================
void ActivityScanWebCams::slotStartScanClicked()
{
	startStopScan( !m_IsScanning );
}

//============================================================================
void ActivityScanWebCams::startStopScan( bool startScan )
{
	if( m_IsScanning != startScan )
	{
		m_bPaused = false;
		m_IsScanning = startScan;
		if( false == m_IsScanning )
		{
			m_FromGui.fromGuiStopScan( m_eScanType );
			if( 0 != m_HisIdent )
			{
				startWebCamSession(  m_HisIdent->getMyOnlineId(), false );
				m_HisIdent = 0;
			}

			m_ShowNextCam = false;
			setScanStatusText( QObject::tr( "Scan For Shared Cams Stopped" ) );
			ui.m_StartScanButton->setText( QObject::tr( "Start Scan" ) );
			ui.m_PauseScanButton->setEnabled( false );
			m_iCountdownCnt = 0; // this stops countdown
			ui.CountdownValue->setText("");
			ui.m_AvailableValueLabel->setText( "" );
		}
		else
		{
			setScanStatusText( QObject::tr( "Scan For Shared Cams Started" ) );
			ui.m_StartScanButton->setText( QObject::tr( "Stop Scan" ) );
			ui.m_PauseScanButton->setEnabled( true );
			m_bSearchComplete = false;
			m_ShowNextCam = true;
			startCountdown();
			m_FromGui.fromGuiStartScan( m_eScanType, RC_FLAG_HAS_SHARED_WEBCAM, 0, "");
			showNextCam();
		}
	}
}

//============================================================================
void ActivityScanWebCams::slotPauseScanClicked()
{
	if( !m_IsScanning )
	{
		startStopScan( true );
	}
	else
	{
		m_bPaused = !m_bPaused;
		if( m_bPaused )
		{
			ui.m_PauseScanButton->setText( "Continue Scan" );
			setScanStatusText( QObject::tr( "Cam Scan Paused" ) );
		}
		else
		{
			if( m_bSearchComplete )
			{
				slotStartScanClicked();
			}
			else
			{
				ui.m_PauseScanButton->setText( QObject::tr( "Pause Scan" ) );
				setScanStatusText( QObject::tr( "Cam Scan Continue" ) );
				slotNextScanClicked();
			}
		}
	}
}

//============================================================================
void ActivityScanWebCams::slotNextScanClicked( void )
{
	m_bPaused = false;
	if( 0 == m_ScanList.size() )
	{
		setScanStatusText( QObject::tr( "No Cams To Show" ) );
	}
	else
	{
		startCountdown();
		m_iCountdownCnt = 1;
		m_ShowNextCam = true;
	}
}

//============================================================================
void ActivityScanWebCams::onCountdownTimer( void )
{
	if( m_iCountdownCnt )
	{
		m_iCountdownCnt--;

		if( 0 == m_iCountdownCnt )
		{
			m_iCountdownCnt = SCAN_INTERVAL_SEC;
			m_ShowNextCam = true;
			showNextCam();
		}

		updateCountdownGui();
	}
}

//============================================================================
void ActivityScanWebCams::slotIdentMenuClicked( void )
{
	AppletPopupMenu* popupMenu = dynamic_cast<AppletPopupMenu*>(m_MyApp.launchApplet( eAppletPopupMenu, dynamic_cast<QWidget*>(this->parent()) ));
	if( popupMenu )
	{
		popupMenu->showFriendMenu( m_HisIdent );
	}
}

