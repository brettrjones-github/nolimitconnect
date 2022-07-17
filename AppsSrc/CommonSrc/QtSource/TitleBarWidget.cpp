//============================================================================
// Copyright (C) 2015 Brett R. Jones 
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
// http://www.nolimitconnect.org
//============================================================================

#include "AppletMgr.h"
#include "TitleBarWidget.h"

#include "AppCommon.h"
#include "AppletPopupMenu.h"
#include "GuiOfferClientMgr.h"
#include "GuiOfferHostMgr.h"
#include "GuiHelpers.h"
#include "MyIcons.h"
#include "SoundMgr.h"

#include <CoreLib/VxGlobals.h>
#include <CoreLib/VxTime.h>
#include <QFrame>

//============================================================================
TitleBarWidget::TitleBarWidget( QWidget * parent )
: QWidget( parent )
, m_MyApp( GetAppInstance() )
, m_OfferClientMgr( m_MyApp.getOfferClientMgr() )
, m_CamTimer(new QTimer(this))
, m_MicrophonePeekTimer( new QTimer( this ) )
{
	ui.setupUi( this );

    setFixedHeight( GuiParams::getButtonSize( eButtonSizeSmall ).height() + 4 );
    ui.m_NoLimitAppButton->setFixedSize( eButtonSizeSmall );
    ui.m_PowerOffButton->setFixedSize( eButtonSizeSmall );
    ui.m_HomeButton->setFixedSize( eButtonSizeSmall );
    ui.m_PersonsOfferListButton->setFixedSize( eButtonSizeSmall );
    ui.m_ShareOfferListButton->setFixedSize( eButtonSizeSmall );
    ui.m_MuteMicButton->setFixedSize( eButtonSizeSmall );
    ui.m_MuteSpeakerButton->setFixedSize( eButtonSizeSmall );
    ui.m_CameraStartStopButton->setFixedSize( eButtonSizeSmall );
    ui.m_TrashButton->setFixedSize( eButtonSizeSmall );
    ui.m_ShareButton->setFixedSize( eButtonSizeSmall );
    ui.m_MenuTopButton->setFixedSize( eButtonSizeSmall );
    ui.m_MenuButton->setFixedSize( eButtonSizeSmall );
    ui.m_BackDlgButton->setFixedSize( eButtonSizeSmall );

    ui.m_NetAvailStatusWidget->setFixedSizeAbsolute( GuiParams::getButtonSize( eButtonSizeSmall ) );

    ui.m_MicVolPeakBar->setFixedSize( GuiParams::getDefaultFontHeight() / 2, GuiParams::getButtonSize( eButtonSizeSmall ).height() );
    ui.m_MicVolPeakBar->setOrientation( Qt::Vertical );
    ui.m_MicVolPeakBar->setRange( 0, 100 );

    float aspectRatio = 320.0f / 240.0f;
    ui.m_CamPreviewScreen->setFixedSize( (int)(GuiParams::getButtonSize( eButtonSizeSmall ).height() * aspectRatio + 1), GuiParams::getButtonSize( eButtonSizeSmall ).height() );

	ui.m_StatusLabel->setVisible( false );
    ui.m_OfferBarWidget->setVisible( false );

    ui.m_PersonsOfferListButton->setIcon( eMyIconPersonsOfferList );
    ui.m_ShareOfferListButton->setIcon( eMyIconOfferList );
    ui.m_MenuButton->setIcon( eMyIconMenu );

	ui.m_NoLimitAppButton->setUseTheme( false );
	ui.m_NoLimitAppButton->setProperty( "NoLimitConnectIcon", true );
	ui.m_NoLimitAppButton->setAppIcon( eMyIconApp, parent );

	setPowerButtonIcon();
	setHomeButtonIcon();
	setMicrophoneIcon( m_MuteMic ? eMyIconMicrophoneOff : eMyIconMicrophoneOn );
    setSpeakerIcon( m_MuteSpeaker ? eMyIconSpeakerOff : eMyIconSpeakerOn );
    setCameraIcon();
    setTrashButtonIcon();
    setShareButtonIcon();
    setTopMenuButtonIcon();
    setBackButtonIcon();

    enableAudioControls( true );
    enableVideoControls( true );

    setCameraButtonVisibility( false );
    setTrashButtonVisibility( false );
    setShareButtonVisibility( false );
    setMenuTopButtonVisibility( false );

    // everyone except home page has home button and back button but not power off button
    setPowerButtonVisibility( false );
    setHomeButtonVisibility( false );
    setBackButtonVisibility( true );

    // will show when have microphone peek signal
    setMuteMicrophoneVisibility( false );
    setMicrophoneVolumeVisibility( false );

    // will show when have cam frames signal
    enableVideoControls( false );

    connect( ui.m_NoLimitAppButton,         SIGNAL(clicked()), this, SLOT(slotApplicationIconClicked()) );

    connect( ui.m_PowerOffButton,           SIGNAL(clicked()), this, SLOT(slotPowerButtonClicked()) );
    connect( ui.m_HomeButton,               SIGNAL(clicked()), this, SLOT(slotHomeButtonClicked()) );
    connect( ui.m_PersonsOfferListButton,   SIGNAL(clicked()), this, SLOT(slotPersonsOfferListButtonClicked()) );
    connect( ui.m_ShareOfferListButton,     SIGNAL(clicked()), this, SLOT(slotShareOfferListButtonClicked()) );
    connect( ui.m_BackDlgButton,            SIGNAL(clicked()), this, SLOT(slotBackButtonClicked()) );
    connect( ui.m_MuteMicButton,            SIGNAL(clicked()), this, SLOT(slotMuteMicButtonClicked()) );
    connect( ui.m_MuteSpeakerButton,        SIGNAL(clicked()), this, SLOT(slotMuteSpeakerButtonClicked()) );
    connect( ui.m_CameraStartStopButton,    SIGNAL(clicked()), this, SLOT(slotCameraSnapshotButtonClicked()) );
    connect( ui.m_CamPreviewScreen,         SIGNAL(clicked()), this, SLOT(slotCamPreviewClicked()) );
    connect( &m_MyApp,                      SIGNAL(signalStatusMsg(QString)), this, SLOT(slotTitleStatusBarMsg(QString)) );
    connect( &m_MyApp,                      SIGNAL(signalToGuiPluginStatus(EPluginType,int,int)), this, SLOT(slotToGuiPluginStatus(EPluginType,int,int)) );
    connect( &m_MyApp,                      SIGNAL(signalNetAvailStatus(ENetAvailStatus)), this, SLOT(slotToGuiNetAvailStatus(ENetAvailStatus)) );

    connect( m_CamTimer,                    SIGNAL(timeout()),                          this, SLOT(slotCamTimeout()) );
    connect( this,                          SIGNAL(signalCamPlaying(bool) ),            this, SLOT(slotCamPlaying(bool)) );

    connect( m_MicrophonePeekTimer,         SIGNAL(timeout()),                          this, SLOT(slotMicrophonePeekTimeout()) );

    connect( ui.m_NetAvailStatusWidget,     SIGNAL(clicked()),                          this, SLOT(slotSignalHelpClick()) );

    connect( &m_OfferClientMgr,             SIGNAL(signalShareOfferCount(int) ),        this, SLOT(slotShareOfferListCount(int)) );
    connect( &m_MyApp.getHostJoinMgr(),     SIGNAL(signalHostJoinRequestCount(int)),    this, SLOT(slotHostJoinRequestCount(int)) );
    connect( ui.m_MenuButton,               SIGNAL(clicked()),                          this, SLOT(slotTitleBarMenuButtonClicked()) );

    ui.m_MicVolPeakBar->setFixedWidth( GuiParams::getDefaultFontHeight() / 2 );

    updateTitleBar();
    connect( &m_MyApp,                      SIGNAL(signalSystemReady(bool)),            this, SLOT(slotSystemReady(bool)) );
}

//============================================================================
void TitleBarWidget::updateTitleBar( void )
{
    checkTitleBarIconsFit();
    bool isMicEnabled = GetAppInstance().getSoundMgr().isMicrophoneEnabled();
    callbackToGuiWantMicrophoneRecording( isMicEnabled );

    m_MuteMic = GetAppInstance().getSoundMgr().fromGuiIsMicrophoneMuted();
    callbackToGuiMicrophoneMuted( m_MuteMic );

    m_MuteSpeaker = GetAppInstance().getSoundMgr().fromGuiIsSpeakerMuted();
    callbackToGuiSpeakerMuted( m_MuteSpeaker );

    bool isCamEnabled = GetAppInstance().getCamLogic().isCamCaptureRunning();
    callbackToGuiWantVideoCapture( isCamEnabled );

    m_EchoCancelEnabled = GetAppInstance().getEngine().fromGuiIsEchoCancelEnabled();
    checkTitleBarIconsFit();

    ui.m_CamPreviewScreen->setImageFromFile( ":/AppRes/Resources/ic_cam_black.png" );

    update();
}

//============================================================================
void TitleBarWidget::slotSystemReady( bool isReady )
{
    if( isReady )
    {
        updateTitleBar();
    }
}

//============================================================================
void TitleBarWidget::slotSignalHelpClick( void )
{
    m_MyApp.getAppletMgr().launchApplet( eAppletHelpNetSignalBars, getTitleBarParentPage() );
}

//============================================================================
void TitleBarWidget::toGuiClientPlayVideoFrame( VxGUID& onlineId, uint8_t * pu8Jpg, uint32_t u32JpgLen, int motion0To100000 )
{
    if( !m_MyOnlineId.isVxGUIDValid() )
    {
        m_MyOnlineId = m_MyApp.getMyOnlineId();
    }

    if( m_MyOnlineId == onlineId )
    {
        m_LastCamFrameTimeMs = GetApplicationAliveMs();
        if( !m_CamPlaying )
        {
            m_CamPlaying = true;
            // callback is from thread so use signals and slots
            emit signalCamPlaying( true );
        }

        ui.m_CamPreviewScreen->playVideoFrame( pu8Jpg, u32JpgLen, motion0To100000 );
    }
}

//============================================================================
void TitleBarWidget::showEvent( QShowEvent * showEvent )
{
    QWidget::showEvent( showEvent );
    if( ( false == VxIsAppShuttingDown() )
        && ( false == m_CallbacksRequested ) )
    {
        m_CallbacksRequested = true;
        m_MyApp.wantToGuiActivityCallbacks( this, true );
        m_MyApp.wantToGuiHardwareCtrlCallbacks( this, true );
        updateTitleBar();
    }
}

//============================================================================
void TitleBarWidget::hideEvent( QHideEvent * ev )
{
    if( m_CallbacksRequested && ( false == VxIsAppShuttingDown() ) )
    {
        m_MyApp.wantToGuiHardwareCtrlCallbacks( this, false );
        m_MyApp.wantToGuiActivityCallbacks( this, false );
        m_CallbacksRequested = false;
    }

    QWidget::hideEvent( ev );
}

//============================================================================
void TitleBarWidget::resizeEvent( QResizeEvent* ev )
{
    checkTitleBarIconsFit();
    QWidget::resizeEvent( ev );
}

//============================================================================
void TitleBarWidget::slotCamPlaying( bool isPlaying )
{

}

//============================================================================
void TitleBarWidget::slotCamTimeout()
{
    if( GetApplicationAliveMs() - m_LastCamFrameTimeMs > 3000 )
    {
        m_CamTimer->stop();
        emit signalCamPlaying( false );
    }
}

//============================================================================
MyIcons&  TitleBarWidget::getMyIcons( void )
{
	return m_MyApp.getMyIcons();
}

//============================================================================
QLabel * TitleBarWidget::getTitleStatusBarLabel( void )
{
	return ui.m_StatusLabel;
}

//============================================================================
VxPushButton * TitleBarWidget::getAppIconPushButton( void )
{
	return ui.m_NoLimitAppButton;
}

//============================================================================
void TitleBarWidget::setTitleBarText( QString titleText )
{
	ui.StyledDlgTitleLabel->setText( titleText );
}

//============================================================================
void TitleBarWidget::setTitleStatusBarMsg( QString statusMsg )
{
	slotTitleStatusBarMsg( statusMsg );
}

//============================================================================
void TitleBarWidget::slotTitleStatusBarMsg( QString msg )
{
	ui.m_StatusLabel->setText( msg );
}

//============================================================================
void TitleBarWidget::slotToGuiPluginStatus( EPluginType ePluginType, int statusType, int statusValue )
{
	if( ( ePluginTypeCamServer == ePluginType )
		&& ( 1 == statusType ) )
	{
		if( statusValue < 0 )
		{
			ui.m_CamClientCountLabel->setText("");
		}
		else
		{
			ui.m_CamClientCountLabel->setText( QString("%1").arg(statusValue) );
		}
	}
}

//============================================================================
void TitleBarWidget::enableVideoControls( bool enable )
{
	ui.m_CamPreviewScreen->setVisible( enable );
	ui.m_CamClientCountLabel->setVisible( enable );
}

//============================================================================
void TitleBarWidget::enableAudioControls( bool enable )
{
	ui.m_MuteMicButton->setVisible( enable );
    ui.m_MicVolPeakBar->setVisible( !m_MuteMic );
    ui.m_MicVolPeakBar->setValue( 0 );
	ui.m_MuteSpeakerButton->setVisible( enable );
}

//============================================================================
void TitleBarWidget::slotPowerButtonClicked( void )
{
	emit signalPowerButtonClicked();
}

//============================================================================
void TitleBarWidget::slotHomeButtonClicked( void )
{
	emit signalHomeButtonClicked();
}

//============================================================================
void TitleBarWidget::slotMuteMicButtonClicked( void )
{
	m_MuteMic = !m_MuteMic;
    m_MyApp.fromGuiMuteMicrophone( m_MuteMic );
}

//============================================================================
void TitleBarWidget::slotMuteSpeakerButtonClicked( void )
{
	m_MuteSpeaker = !m_MuteSpeaker;
    m_MyApp.fromGuiMuteSpeaker( m_MuteSpeaker );
}

//============================================================================
void TitleBarWidget::slotCameraSnapshotButtonClicked( void )
{
	emit signalCameraSnapshotButtonClicked();
}

//============================================================================
void TitleBarWidget::slotCamPreviewClicked( void )
{
	emit signalCamPreviewClicked();
    m_MyApp.getAppletMgr().launchApplet( eAppletCamSettings );
}

//============================================================================
void TitleBarWidget::slotTrashButtonClicked( void )
{
	emit signalTrashButtonClicked();
}

//============================================================================
void TitleBarWidget::slotShareButtonClicked( void )
{
	emit signalShareButtonClicked();
}

//============================================================================
void TitleBarWidget::slotMenuTopButtonClicked( void )
{
	emit signalMenuTopButtonClicked();
}

//============================================================================
void TitleBarWidget::slotBackButtonClicked( void )
{
	emit signalBackButtonClicked();
}

//======= button visibility ====//
//============================================================================
void TitleBarWidget::setPopupVisibility( void )
{
    setNetStatusVisibility( false );
    setPersonsOfferListButtonVisibility( false );
    setShareOfferListButtonVisibility( false );
    setShareButtonVisibility( false );

    enableAudioControls( false );
    setMicrophoneVolumeVisibility( false );
    enableVideoControls( false );
    setMenuTopButtonVisibility( false );
    setMenuListButtonVisibility( false );
}

//============================================================================
void TitleBarWidget::setBackButtonVisibility( bool visible )
{
    ui.m_BackDlgButton->setVisible( visible );
}

//============================================================================
void TitleBarWidget::setCamPreviewVisibility( bool visible )
{
	ui.m_CamPreviewScreen->setVisible( visible );
}

//============================================================================
void TitleBarWidget::setCamViewerCountVisibility( bool visible )
{
	ui.m_CamClientCountLabel->setVisible( visible );
}

//============================================================================
void TitleBarWidget::setPowerButtonVisibility( bool visible )
{
	ui.m_PowerOffButton->setVisible( visible );
}

//============================================================================
void TitleBarWidget::setHomeButtonVisibility( bool visible )
{
	ui.m_HomeButton->setVisible( visible );
}

//============================================================================
void TitleBarWidget::setNetStatusVisibility( bool visible )
{
    ui.m_NetAvailStatusWidget->setVisible( visible );
}


//============================================================================
void TitleBarWidget::setTrashButtonVisibility( bool visible )
{
	ui.m_TrashButton->setVisible( visible );
}

//============================================================================
void TitleBarWidget::setPersonsOfferListButtonVisibility( bool visible )
{
    ui.m_PersonsOfferListButton->setVisible( visible );
}

//============================================================================
void TitleBarWidget::setShareOfferListButtonVisibility( bool visible )
{
    ui.m_ShareOfferListButton->setVisible( visible );
}

//============================================================================
void TitleBarWidget::setShareButtonVisibility( bool visible )
{
	ui.m_ShareButton->setVisible( visible );
}

//============================================================================
void TitleBarWidget::setMenuTopButtonVisibility( bool visible )
{
	ui.m_MenuTopButton->setVisible( visible );
}

//============================================================================
void TitleBarWidget::setMenuListButtonVisibility( bool visible )
{
    ui.m_MenuButton->setVisible( visible );
}

//============================================================================
void TitleBarWidget::setMuteSpeakerVisibility( bool visible )
{
	ui.m_MuteSpeakerButton->setVisible( visible );
}

//============================================================================
void TitleBarWidget::setMuteMicrophoneVisibility( bool visible )
{
	ui.m_MuteMicButton->setVisible( visible );
}

//============================================================================
void TitleBarWidget::setMicrophoneVolumeVisibility( bool visible )
{
    ui.m_MicVolPeakBar->setVisible( visible );
}

//============================================================================
void TitleBarWidget::setCameraButtonVisibility( bool visible )
{
	ui.m_CameraStartStopButton->setVisible( visible );
}

//============ set button icon ============//
//============================================================================
void TitleBarWidget::setPowerButtonIcon( EMyIcons myIcon )
{
	ui.m_PowerOffButton->setIcon( myIcon );
}

//============================================================================
void TitleBarWidget::setHomeButtonIcon( EMyIcons myIcon )
{
	ui.m_HomeButton->setIcon( myIcon );
}

//============================================================================
void TitleBarWidget::setMicrophoneIcon( EMyIcons myIcon )
{
	ui.m_MuteMicButton->setIcon( myIcon );
}

//============================================================================
void TitleBarWidget::setSpeakerIcon( EMyIcons myIcon )
{
	ui.m_MuteSpeakerButton->setIcon( myIcon );
}

//============================================================================
void TitleBarWidget::setCameraIcon( EMyIcons myIcon )
{
	ui.m_CameraStartStopButton->setIcon( myIcon );
}

//============================================================================
void TitleBarWidget::setTrashButtonIcon( EMyIcons myIcon )
{
	ui.m_TrashButton->setIcon( myIcon );
}

//============================================================================
void TitleBarWidget::setShareButtonIcon( EMyIcons myIcon )
{
	ui.m_ShareButton->setIcon( myIcon );
}

//============================================================================
void TitleBarWidget::setTopMenuButtonIcon( EMyIcons myIcon )
{
	ui.m_MenuTopButton->setIcon( myIcon );
}

//============================================================================
void TitleBarWidget::setBackButtonIcon( EMyIcons myIcon )
{
	ui.m_BackDlgButton->setIcon( myIcon );
}

//============ set button color ============//
//============================================================================
void TitleBarWidget::setPowerButtonColor( QColor iconColor )
{
	ui.m_PowerOffButton->setIconOverrideColor( iconColor );
}

//============================================================================
void TitleBarWidget::setHomeButtonColor( QColor iconColor )
{
	ui.m_HomeButton->setIconOverrideColor( iconColor );
}

//============================================================================
void TitleBarWidget::setMicrophoneColor( QColor iconColor )
{
	ui.m_BackDlgButton->setIconOverrideColor( iconColor );
}

//============================================================================
void TitleBarWidget::setSpeakerColor( QColor iconColor )
{
	ui.m_MuteMicButton->setIconOverrideColor( iconColor );
}

//============================================================================
void TitleBarWidget::setCameraColor( QColor iconColor )
{
	ui.m_CameraStartStopButton->setIconOverrideColor( iconColor );
}

//============================================================================
void TitleBarWidget::setTrashButtonColor( QColor iconColor )
{
	ui.m_TrashButton->setIconOverrideColor( iconColor );
}

//============================================================================
void TitleBarWidget::setShareButtonColor( QColor iconColor )
{
	ui.m_ShareButton->setIconOverrideColor( iconColor );
}

//============================================================================
void TitleBarWidget::setTopMenuButtonColor( QColor iconColor )
{
	ui.m_MenuTopButton->setIconOverrideColor( iconColor );
}

//============================================================================
void TitleBarWidget::setBackButtonColor( QColor iconColor )
{
	ui.m_BackDlgButton->setIconOverrideColor( iconColor );
}

//============================================================================
void TitleBarWidget::slotToGuiNetAvailStatus( ENetAvailStatus eNetAvailStatus )
{
    ui.m_NetAvailStatusWidget->toGuiNetAvailStatus( eNetAvailStatus );
}

//============================================================================
void TitleBarWidget::slotShareOfferListCount( int activeCnt )
{
    ui.m_ShareOfferListButton->setNotifyOnlineEnabled( activeCnt > 0 );
}

//============================================================================
void TitleBarWidget::slotHostJoinRequestCount( int activeCnt )
{
    ui.m_PersonsOfferListButton->setNotifyOnlineEnabled( activeCnt > 0 );
}

//============================================================================
void TitleBarWidget::slotPersonsOfferListButtonClicked( void )
{
    m_MyApp.getAppletMgr().launchApplet( eAppletHostJoinRequestList, getTitleBarParentPage()  );
}

//============================================================================
void TitleBarWidget::slotShareOfferListButtonClicked( void )
{
    m_MyApp.getAppletMgr().launchApplet( eAppletShareOfferList, getTitleBarParentPage() );
}

//============================================================================
void TitleBarWidget::slotTitleBarMenuButtonClicked( void )
{
    LogMsg( LOG_VERBOSE, "slotTitleBarMenuButtonClicked" );
    AppletPopupMenu* popupMenu = dynamic_cast<AppletPopupMenu*>(m_MyApp.getAppletMgr().launchApplet( eAppletPopupMenu, getTitleBarParentFrame() ) );
    if( popupMenu )
    {
        popupMenu->showTitleBarMenu();
    }
}

//============================================================================
QWidget* TitleBarWidget::getTitleBarParentFrame( void )
{
    QWidget* parentWdiget = dynamic_cast<QWidget*>(parent());
    QWidget* frame = GuiHelpers::findParentContentFrame( parentWdiget );
    if( frame )
    {
        parentWdiget = frame;
    }

    return parentWdiget;
}

//============================================================================
QWidget* TitleBarWidget::getTitleBarParentPage( void )
{
    QWidget* parentWdiget = dynamic_cast<QWidget*>(parent());
    QWidget* page = GuiHelpers::findParentPage( parentWdiget );
    if( page )
    {
        parentWdiget = page;
    }

    return parentWdiget;
}

//============================================================================
void TitleBarWidget::checkTitleBarIconsFit( void )
{
    int iconCnt = 5;
    if( ui.m_PowerOffButton->isVisible() )
    {
        iconCnt++;
    }

    if( ui.m_BackDlgButton->isVisible() )
    {
        iconCnt++;
    }

    if( ui.m_MuteMicButton->isVisible() )
    {
        iconCnt += 2;
    }

    if( ui.m_CamPreviewScreen->isVisible() )
    {
        iconCnt += 2;
    }

    if( !GuiParams::canFitIcons( eButtonSizeSmall, iconCnt, geometry().width() ) )
    {
        ui.m_NoLimitAppButton->setVisible( false );
        ui.m_MuteSpeakerButton->setVisible( false );
        ui.m_NetAvailStatusWidget->setVisible( false );
    }
    else
    {
        ui.m_NoLimitAppButton->setVisible( true );
        ui.m_MuteSpeakerButton->setVisible( true );
        ui.m_NetAvailStatusWidget->setVisible( true );
    }
}

//============================================================================
void TitleBarWidget::slotApplicationIconClicked( void )
{
    if( m_MyApp.getIsAppInitialized() )
    {
        LogMsg( LOG_VERBOSE, "slotApplicationIconClicked" );
        // bring up friends list unless is already active
        if( !m_MyApp.getAppletMgr().findAppletDialog( eAppletFriendListClient ) )
        {
            m_MyApp.getAppletMgr().launchApplet( eAppletFriendListClient, getTitleBarParentPage() );
        }
    }
    else
    {
        QMessageBox::information( this, QObject::tr( "Application Not Ready" ), QObject::tr( "Cannot Launch Applet Until Application Has Initialized" ) );
    }
}

//============================================================================
void TitleBarWidget::callbackToGuiWantMicrophoneRecording( bool wantMicInput )
{
    m_MicrophonePlaying = wantMicInput;

    if( wantMicInput )
    {
        m_MicrophonePeekTimer->start( 500 );
        ui.m_MuteMicButton->setVisible( true );
        ui.m_MicVolPeakBar->setVisible( true );
        ui.m_MicVolPeakBar->setValue( 0 );
    }
    else
    {
        m_MicrophonePeekTimer->stop();
        ui.m_MuteMicButton->setVisible( false );
        ui.m_MicVolPeakBar->setVisible( false );
    }

    checkTitleBarIconsFit();
}

//============================================================================
void TitleBarWidget::slotMicrophonePeekTimeout()
{
    // LogMsg( LOG_VERBOSE, "slotMicrophonePeekTimeout %d", m_MyApp.getSoundMgr().getMicrophonePeakValue0To100() );
    ui.m_MicVolPeakBar->setValue( m_MyApp.getSoundMgr().getMicrophonePeakValue0To100() );
}

//============================================================================
void TitleBarWidget::callbackToGuiWantSpeakerOutput( bool wantSpeakerOutput )
{

}

//============================================================================
void TitleBarWidget::callbackToGuiWantVideoCapture( bool wantVideoCapture )
{
    if( m_CamPlaying != wantVideoCapture )
    {
        m_CamPlaying = wantVideoCapture;
        enableVideoControls( wantVideoCapture );
        if( wantVideoCapture )
        {
            m_CamTimer->start( 1500 );
        }
        else
        {
            m_CamTimer->stop();
            ui.m_CamPreviewScreen->setImageFromFile( ":/AppRes/Resources/ic_cam_black.png" );
        }

        checkTitleBarIconsFit();
    }
}

//============================================================================
void TitleBarWidget::callbackToGuiMicrophoneMuted( bool isMuted )
{
    m_MuteMic = isMuted;
    setMicrophoneIcon( m_MuteMic ? eMyIconMicrophoneOff : eMyIconMicrophoneOn );
}

//============================================================================
void TitleBarWidget::callbackToGuiSpeakerMuted( bool isMuted )
{
    m_MuteSpeaker = isMuted;
    setSpeakerIcon( m_MuteSpeaker ? eMyIconSpeakerOff : eMyIconSpeakerOn );
}