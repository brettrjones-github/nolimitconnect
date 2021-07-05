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
#include <PktLib/VxSearchDefs.h>
#include <ptop_src/ptop_engine_src/P2PEngine/P2PEngine.h>

#include "ActivityScanProfiles.h"
#include "AppGlobals.h"
#include "AppCommon.h"
#include "MyIcons.h"
#include "PopupMenu.h"

namespace
{
	const int SCAN_INTERVAL_SEC			= 10;
	const int COUNTDOWN_INTERVAL_MS		= 1000;
}

//============================================================================
ActivityScanProfiles::ActivityScanProfiles(	AppCommon&	app, 
											QWidget *		parent )
: ActivityBase( OBJNAME_ACTIVITY_SCAN_PROFILES, app, parent, eAppletScanAboutMe, true )
, m_Ident( NULL )
, m_iCountdownCnt( 0 )
, m_bPaused( false )
, m_CountdownTimer( new QTimer( this ) )
, m_eScanType(eScanTypeProfilePic)
, m_bIconConnected( false )
, m_bSearchComplete( false )
, m_ShowNextImage( false )
, m_IsScanning( false )
{
	ui.setupUi(this);
	QRect oRect = parent->geometry();
	oRect.setRight(oRect.right() - oRect.left());
	oRect.setLeft(0);
	oRect.setBottom(oRect.bottom() - oRect.top());
	oRect.setTop(0);
	this->setGeometry(oRect);
    connectBarWidgets();

    connect(this, SIGNAL(signalSearchResultProfilePic(VxNetIdent*, QImage)), this, SLOT(slotSearchResultProfilePic(VxNetIdent*, QImage)));
    connect(m_CountdownTimer, SIGNAL(timeout()), this, SLOT(onCountdownTimer()));
    connect( ui.m_TitleBarWidget, SIGNAL(signalBackButtonClicked()), this, SLOT(close()));
	connect( this, SIGNAL(finished(int)), this, SLOT(slotHomeButtonClicked()));

	connect( ui.m_StartScanButton, SIGNAL(clicked()), this, SLOT(slotStartScanClicked()));
	connect( ui.m_PauseScanButton, SIGNAL(clicked()), this, SLOT(slotPauseScanClicked()));
	connect( ui.m_NextScanButton, SIGNAL(clicked()), this, SLOT(slotNextScanClicked()));
	connect( this, SIGNAL(signalSearchComplete()), this, SLOT(slotSearchComplete()) );

	connect( ui.FriendIdentWidget, SIGNAL(clicked()), this, SLOT(slotFriendClicked()));

	ui.FriendIdentWidget->setVisible( false );

	setTitle( "Scan Profiles" );
	m_CountdownTimer->setInterval( COUNTDOWN_INTERVAL_MS );
	m_MyApp.wantToGuiActivityCallbacks( this, this, true );
	slotStartScanClicked();
}

//============================================================================
void ActivityScanProfiles::setTitle( QString strTitle )
{
	ui.m_TitleBarWidget->setTitleBarText(strTitle);
}

//============================================================================
void ActivityScanProfiles::setScanStatusText( QString strMsg )
{
	m_MyApp.toGuiStatusMessage( strMsg.toUtf8().constData() );
}

//============================================================================
void ActivityScanProfiles::showEvent( QShowEvent * ev )
{
	ActivityBase::showEvent( ev );
	m_MyApp.wantToGuiActivityCallbacks( this, this, true );
}

//============================================================================
void ActivityScanProfiles::hideEvent( QHideEvent * ev )
{
	m_MyApp.wantToGuiActivityCallbacks( this, this, false );
	ActivityBase::hideEvent( ev );
}

//============================================================================
void ActivityScanProfiles::slotSearchComplete()
{
	setScanStatusText( QObject::tr( "Search Profile Pictures Complete" ) );
	m_bSearchComplete = true;
}

//============================================================================
void ActivityScanProfiles::setupIdentWidget( GuiUser * netIdent )
{
	m_Ident = netIdent;
	IdentWidget * poIdentWidget = ui.FriendIdentWidget;
	if( poIdentWidget )
	{
		poIdentWidget->setVisible( true );
		poIdentWidget->updateGuiFromData( netIdent );
		if( false == m_bIconConnected )
		{
			m_bIconConnected = true;
			connect( poIdentWidget->m_FriendIconButton, SIGNAL(clicked()), this, SLOT(slotFriendClicked()));
		}
	}
}

//============================================================================
void ActivityScanProfiles::updateCountdownGui()
{
	ui.CountdownValue->setText( QString( "%1" ).arg( m_iCountdownCnt ) );
}

//============================================================================
//! clicked the upper right x button
void ActivityScanProfiles::slotHomeButtonClicked( void )
{
	m_FromGui.fromGuiStopScan( m_eScanType );
}

//============================================================================
void ActivityScanProfiles::toGuiClientScanSearchComplete( void * userData, EScanType eScanType )
{
	Q_UNUSED( userData );
	emit signalSearchComplete();
}

//============================================================================
void ActivityScanProfiles::toGuiSearchResultProfilePic(	void *			userData, 
                                                        GuiUser *	    netIdent, 
														uint8_t *		pu8JpgData, 
														uint32_t		u32JpgDataLen )
{
	Q_UNUSED( userData );
	QImage	oPicBitmap;
	if( oPicBitmap.loadFromData( pu8JpgData, u32JpgDataLen, "JPG" ) )
	{
		emit signalSearchResultProfilePic( netIdent, oPicBitmap );
	}
}

//============================================================================
void ActivityScanProfiles::slotSearchResultProfilePic( GuiUser * netIdent, QImage oPicBitmap ) 
{
	ScanProfilePair profilePair( netIdent, oPicBitmap );
	m_ScanList.push_back( profilePair );
	showNextImage();
}

//============================================================================
void ActivityScanProfiles::showNextImage( void ) 
{
	if( m_bSearchComplete 
		&& ( 0 == m_ScanList.size() ) )
	{
		startStopScan( false );
		return;
	}

	if( m_ShowNextImage && m_ScanList.size() )
	{
		m_ShowNextImage = false;
		ScanProfilePair& profilePair = m_ScanList[0];
		showMatchedPair( profilePair.m_Ident, profilePair.m_Image );
		m_ScanList.erase( m_ScanList.begin() );
		startCountdown();
	}

	ui.m_AvailPicturesValueLabel->setText( QString("%1").arg( m_ScanList.size() ) );
}

//============================================================================
void ActivityScanProfiles::showMatchedPair( GuiUser * netIdent, QImage& oPicBitmap ) 
{
	setupIdentWidget( netIdent );
	QPixmap oPicPixmap = QPixmap::fromImage(oPicBitmap);
	ui.ProfileFrame->setPixmap( oPicPixmap );
	//ui.ProfileFrame->repaint();
}

//============================================================================
void ActivityScanProfiles::startCountdown()
{
	m_iCountdownCnt = SCAN_INTERVAL_SEC;
	m_CountdownTimer->start();
	updateCountdownGui();
}

//============================================================================
void ActivityScanProfiles::slotPauseScanClicked()
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
			m_CountdownTimer->stop();
			m_iCountdownCnt = SCAN_INTERVAL_SEC;
			updateCountdownGui();
			ui.m_PauseScanButton->setText( "Continue Scan" );
			setScanStatusText( QObject::tr( "Profile Picture Scan Paused" ) );
		}
		else
		{
			m_ShowNextImage = true;
			showNextImage();
			startCountdown();
			ui.m_PauseScanButton->setText( QObject::tr( "Pause Scan" ) );
			setScanStatusText( QObject::tr( "Profile Picture Scan Continue" ) );
		}
	}
}

//============================================================================
void ActivityScanProfiles::slotStartScanClicked()
{
	startStopScan( !m_IsScanning );
}

//============================================================================
void ActivityScanProfiles::startStopScan( bool startScan )
{
	if( m_IsScanning != startScan )
	{
		m_IsScanning = startScan;
		if( false == m_IsScanning )
		{
			m_ShowNextImage = false;
			setScanStatusText( QObject::tr( "Scan For Profile Pictures Stopped" ) );
			ui.m_StartScanButton->setText( QObject::tr( "Start Scan" ) );
			ui.m_PauseScanButton->setEnabled( false );
			m_iCountdownCnt = 0;
			m_CountdownTimer->stop();
			ui.CountdownValue->setText("");
			ui.m_AvailPicturesValueLabel->setText( "" );

			m_FromGui.fromGuiStopScan( m_eScanType );
		}
		else
		{
			m_ShowNextImage = true;
			startCountdown();
			setScanStatusText( QObject::tr( "Scan For Profile Pictures Started" ) );
			ui.m_StartScanButton->setText( QObject::tr( "Stop Scan" ) );
			ui.m_PauseScanButton->setEnabled( true );
			m_CountdownTimer->start();
			m_bSearchComplete = false;
			m_FromGui.fromGuiStartScan( m_eScanType, RC_FLAG_HAS_PROFILE_PIC, 0, "");
			showNextImage();
		}
	}
}

//============================================================================
void ActivityScanProfiles::slotNextScanClicked( void )
{
	if( 0 == m_ScanList.size() )
	{
		setScanStatusText( QObject::tr( "No Profile Pictures To Show" ) );
	}
	else
	{
		m_ShowNextImage = true;
		startCountdown();
		showNextImage();
	}
}

//============================================================================
void ActivityScanProfiles::onCountdownTimer( void )
{
	if( m_iCountdownCnt )
	{
		m_iCountdownCnt--;

		if( 0 == m_iCountdownCnt )
		{
			m_iCountdownCnt = SCAN_INTERVAL_SEC;
			m_ShowNextImage = true;
			showNextImage();
		}

		updateCountdownGui();
	}
}

//============================================================================
void ActivityScanProfiles::slotFriendClicked( void )
{
	if( m_Ident )
	{
		PopupMenu popupMenu( m_MyApp, this );
        popupMenu.setTitleBarWidget( this->getTitleBarWidget() );
        popupMenu.setBottomBarWidget( this->getBottomBarWidget() );
        popupMenu.setContentItemsFrame( this->getContentItemsFrame() );

        connect( &popupMenu, SIGNAL(menuItemClicked(int, PopupMenu*, ActivityBase*)), &popupMenu, SLOT(onFriendActionSelected(int, PopupMenu*, ActivityBase*)) );

        popupMenu.showFriendMenu( m_Ident );
	}
}

