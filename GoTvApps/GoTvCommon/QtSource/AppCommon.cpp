//============================================================================
// Copyright (C) 2009 Brett R. Jones 
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


#include "LogMgr.h"

#include "ToGuiActivityInterface.h"
#include "GuiOfferClientMgr.h"
#include "GuiOfferHostMgr.h"
#include "GuiParams.h"
#include "MySndMgr.h"
#include "MyIcons.h"

#include "VxTilePositioner.h"

#include "HomeWindow.h"

#include "ActivityCreateAccount.h"
#include "ActivityMessageBox.h"
#include "ActivityPermissions.h"
#include "ActivitySoundOptions.h"
#include "ActivityShowHelp.h"
#include "ActivityTimedMessage.h"
#include "ActivityViewLibraryFiles.h"

#include "AppletDownloads.h"
#include "AppletMultiMessenger.h"
#include "AppletUploads.h"

//#include "ActivityWebCamClient.h"
//#include "ActivityReplyFileOffer.h"
#include "AppletPeerSessionFileOffer.h"

//#include "ActivityToFriendTodGame.h"
//#include "ActivityToFriendVoicePhone.h"
//#include "ActivityToFriendVideoPhone.h"


#include "GuiOfferSession.h"

#include "AccountMgr.h"
#include "AppSettings.h"
#include "AppletMgr.h"

#include "FileListReplySession.h"
#include "VxPushButton.h"

#include <ptop_src/ptop_engine_src/P2PEngine/FileShareSettings.h>
#include <ptop_src/ptop_engine_src/P2PEngine/EngineSettings.h>
#include <ptop_src/ptop_engine_src/NetworkMonitor/NetworkMonitor.h>

#include <PktLib/VxCommon.h>

#include <CoreLib/VxFileUtil.h>
#include <CoreLib/VxParse.h>
#include <CoreLib/VxGlobals.h>
#include <CoreLib/IsBigEndianCpu.h>
#include <CoreLib/VxGUID.h>

#include <GuiInterface/IGoTv.h>

#include <NetLib/VxPeerMgr.h>

#include <QApplication>
#include <QMainWindow>
#include <QCloseEvent>
#include <QDesktopServices>
#include <QDebug>
#include <QSettings>

#include "RenderGlWidget.h"

namespace
{
	AppCommon * g_AppCommon = 0;

	QString GetAppTitle( EDefaultAppMode appMode )
	{
		switch( appMode )
		{
		case eAppModeDefault:
			return QObject::tr( "GoTv PtoP" );
		case eAppModeGoTvViewer:
			return QObject::tr( "NoLimitConnect Player" );
		case eAppModeGoTvProvider:
			return QObject::tr( "NoLimitConnect Provider" );
		case eAppModeGoTvStation:
			return QObject::tr( "NoLimitConnect Station" );
		case eAppModeGoTvNetworkHost:
			return QObject::tr( "NoLimitConnect Network Host" );
		case eAppModeUnknown:
		default:
			return QObject::tr( "NoLimitConnect Unknown App" );	
		}
	}

	QString GetAppShortName( EDefaultAppMode appMode )
	{
		//NOTE: do not translate or will cause new settings each time user changes languages
		QString appShortName = VxGetApplicationNameNoSpaces(); 
		switch( appMode )
		{
		case eAppModeDefault:
			return appShortName;
		case eAppModeGoTvViewer:
			return "GoTvPlayer";
		case eAppModeGoTvProvider:
			return "GoTvProvider";
		case eAppModeGoTvStation:
			return "GoTvStation";
		case eAppModeGoTvNetworkHost:
			return "GoTvNetHost";
		case eAppModeUnknown:
		default:
			return "GoTvUnknownApp";
		}
	}
}

//============================================================================
AppCommon& CreateAppInstance( IGoTv& gotv, QApplication* myApp )
{
static AppSettings appSettings;
static AccountMgr accountMgr;
    if( !g_AppCommon )
    {
        // constructor of AppCommon will set g_AppCommon
        new AppCommon( *myApp, eAppModeDefault, appSettings, accountMgr, gotv );
    }

    return *g_AppCommon;
}

//============================================================================
AppCommon& GetAppInstance( void )
{
	return *g_AppCommon;
}

//============================================================================
void DestroyAppInstance()
{
    delete g_AppCommon;
    g_AppCommon = nullptr;
}

//============================================================================
AppCommon::AppCommon(	QApplication&	myQApp,
						EDefaultAppMode appDefaultMode,
						AppSettings&	appSettings, 
                        AccountMgr&	    accountMgr,
						IGoTv&		    gotv )
: QWidget()
, m_QApp( myQApp )
, m_AppDefaultMode( appDefaultMode )
, m_AppGlobals( *this )
, m_AppSettings( appSettings )
, m_AppShortName( GetAppShortName( appDefaultMode ) )
, m_AppTitle( GetAppTitle( appDefaultMode ) )
, m_AccountMgr( accountMgr )
, m_GoTv( gotv )
, m_VxPeerMgr( gotv.getPeerMgr() )
, m_ThumbMgr( *this )
, m_OfferClientMgr( *this )
, m_OfferHostMgr( *this )
, m_UserMgr( *this )
, m_HostJoinMgr( *this )
, m_UserJoinMgr( *this )
, m_ConnectMgr( *this )

, m_MyIcons( *this )
, m_AppTheme( *this )
, m_AppStyle( *this, m_AppTheme )
, m_AppDisplay( *this )
, m_TilePositioner( * new VxTilePositioner( *this ) )
, m_CamLogic( *this )

, m_Engine( gotv.getPtoP() )
, m_MySndMgr( * new MySndMgr( *this ) )

, m_HomePage( *this, m_AppTitle )

, m_VidCap( NULL )
, m_VidCapTimer( NULL )
, m_IdleTimer( new QTimer( this ) )
, m_OncePerSecondTimer( new QTimer( this ) )
, m_eLastSelectedWhichContactsToView( eFriendViewEverybody )
, m_bUserCanceledCreateProfile( false )
, m_LastNetworkState( eNetworkStateTypeUnknown )
, m_CamSourceId( 1 )
, m_CamCaptureRotation( 0 )
, m_AppletMgr( *( new AppletMgr( *this, this) ) )
, m_CheckSetupTimer( new QTimer( this ) )
{
    g_AppCommon = this; // crap.. need a global instance that can accessed immediately with GetAppInstance() for objects created in ui files

    m_ElapsedTimer.start();

    connect( m_OncePerSecondTimer, SIGNAL( timeout() ), &m_OfferClientMgr, SLOT( slotOncePerSecond() ) );
    connect( m_CheckSetupTimer, SIGNAL( timeout() ), this, SLOT( slotCheckSetupTimer() ) );
}

//============================================================================
IFromGui& AppCommon::getFromGuiInterface( void )
{
    return m_Engine.getFromGuiInterface();
}

//============================================================================
void AppCommon::loadWithoutThread( void )
{
    uint64_t startMs = elapsedMilliseconds();

    registerMetaData();

    // set application short name used for directory paths
    VxSetApplicationNameNoSpaces( m_AppShortName.toUtf8().constData() );
    m_AppSettings.setAppShortName( m_AppShortName );

    // this just loads the ini file.
    // the AppSettings database is not initialized until loadAccountSpecificSettings
    m_AppSettings.loadProfile();
    // sets root of application data and transfer directories
    VxSetRootUserDataDirectory( m_AppSettings.m_strRootUserDataDir.c_str() );

    // Documents Directory/appshortName/xfer/		app data transfer directory
    VxSetRootXferDirectory( m_AppSettings.m_strRootXferDir.c_str() );

    // create settings database appshortname_settings.db3 in /appshortName/data/
    QString strSettingsDbFileName = VxGetAppGoTvDataDirectory().c_str() + m_AppShortName + "_settings.db3";
    m_AppSettings.appSettingStartup( strSettingsDbFileName.toUtf8().constData(), m_AppDefaultMode );

    // Now that settings are set up we can enable logging with log settings
    GetLogMgrInstance().startupLogMgr();

    // database of multiple accounts
    // create accounts database appshortname_accounts.db3 in /appshortName/data/
    QString strAccountDbFileName = VxGetAppGoTvDataDirectory().c_str() + m_AppShortName + "_accounts.db3";
    m_AccountMgr.startupAccountMgr( strAccountDbFileName.toUtf8().constData() );

    // asset database and user specific setting database will be created in sub directory of account login
    // after user has logged into account

    uint64_t loadingMs = GetApplicationAliveMs();
    LogMsg( LOG_DEBUG, "LoadSettings %lld ms alive ms %lld", loadingMs - startMs, loadingMs );

	if( getAppSettings().getFeatureEnable( eAppFeatureTheme ) )
	{
		getAppTheme().selectTheme( getAppSettings().getLastSelectedTheme() );
	}

    // load icons from resources
    m_MyIcons.myIconsStartup();

    uint64_t iconsMs = GetApplicationAliveMs();
    LogMsg( LOG_DEBUG, "Load Icons %lld ms alive ms %lld", iconsMs - loadingMs, iconsMs );

#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
	// QT 6.2.0 has broken scroll bars.. TODO fix when 6.2.0 is no longer beta
	if( getAppSettings().getFeatureEnable( eAppFeatureTheme ) )
	{
		getQApplication().setStyle( &m_AppStyle );
	}
#endif // QT_VERSION < QT_VERSION_CHECK(6,0,0)

    // load sounds to play and sound hardware
    m_MySndMgr.sndMgrStartup();

    uint64_t styleMs = GetApplicationAliveMs();
    LogMsg( LOG_DEBUG, "Setup Style %lld ms alive ms %lld", styleMs - iconsMs, styleMs );

	m_ThumbMgr.onAppCommonCreated();
	m_UserMgr.onAppCommonCreated();
	m_OfferClientMgr.onAppCommonCreated();
	m_OfferHostMgr.onAppCommonCreated();
	m_HostJoinMgr.onAppCommonCreated();
	m_UserJoinMgr.onAppCommonCreated();
	m_ConnectMgr.onAppCommonCreated();

    m_HomePage.initializeHomePage();
    connect( &m_HomePage, SIGNAL( signalMainWindowResized() ), this, SLOT( slotMainWindowResized() ) );
    m_HomePage.show();


    uint64_t homePageMs = GetApplicationAliveMs();
    LogMsg( LOG_DEBUG, "Initialize Home Page %lld ms alive ms %lld", homePageMs - styleMs, homePageMs );


}

//============================================================================
void AppCommon::slotStartLoadingFromThread( void )
{
	registerMetaData();

	QObject::connect( this, SIGNAL( signalFinishedLoadingGui() ), this, SLOT( slotFinishedLoadingGui() ) );
	QObject::connect( this, SIGNAL( signalFinishedLoadingEngine() ), this, SLOT( slotFinishedLoadingEngine() ) );

	// set application short name used for directory paths
	VxSetApplicationNameNoSpaces( m_AppShortName.toUtf8().constData() );
	m_AppSettings.setAppShortName( m_AppShortName );

	// this just loads the ini file.
	// the AppSettings database is not initialized until loadAccountSpecificSettings
	m_AppSettings.loadProfile();
	// sets root of application data and transfer directories
	VxSetRootUserDataDirectory(m_AppSettings.m_strRootUserDataDir.c_str());

	// Documents Directory/appshortName/xfer/		app data transfer directory
	VxSetRootXferDirectory( m_AppSettings.m_strRootXferDir.c_str() );

	// create settings database appshortname_settings.db3 in /appshortName/data/
	QString strSettingsDbFileName = VxGetAppGoTvDataDirectory().c_str() + m_AppShortName + "_settings.db3";
	m_AppSettings.appSettingStartup( strSettingsDbFileName.toUtf8().constData(), m_AppDefaultMode );

	// database of multiple accounts
	// create accounts database appshortname_accounts.db3 in /appshortName/data/
	QString strAccountDbFileName = VxGetAppGoTvDataDirectory().c_str() + m_AppShortName + "_accounts.db3";
	m_AccountMgr.startupAccountMgr( strAccountDbFileName.toUtf8().constData() );

	// asset database and user specific setting database will be created in sub directory of account login
	// after user has logged into account

    getAppTheme().selectTheme( getAppSettings().getLastSelectedTheme() );

	// load icons from resources
	m_MyIcons.myIconsStartup();
	// load sounds to play and sound hardware
	m_MySndMgr.sndMgrStartup(); 

	m_ThumbMgr.onAppCommonCreated();
	m_UserMgr.onAppCommonCreated();
	m_OfferClientMgr.onAppCommonCreated();
	m_OfferHostMgr.onAppCommonCreated();
	m_HostJoinMgr.onAppCommonCreated();
	m_UserJoinMgr.onAppCommonCreated();
	m_ConnectMgr.onAppCommonCreated();

	// should have enough to show home page
	emit signalFinishedLoadingGui();

	// finish initializing engine and start GoTvCore loading with its threads
	emit signalFinishedLoadingEngine();
}

//============================================================================
void AppCommon::slotFinishedLoadingGui( void )
{
	// theme must be loaded from gui thread
    //m_AppTheme.selectTheme( getAppSettings().getLastSelectedTheme() );

	m_HomePage.initializeHomePage();
	connect( &m_HomePage, SIGNAL( signalMainWindowResized() ), this, SLOT(slotMainWindowResized() ) );
    connect( &m_HomePage, SIGNAL( signalMainWindowMoved() ), this, SLOT( slotMainWindowMoved() ) );
	m_HomePage.show();
}

//============================================================================
void AppCommon::slotFinishedLoadingEngine( void )
{

}

//============================================================================
void AppCommon::startupAppCommon( QFrame * appletFrame, QFrame * messangerFrame )
{
    if( m_AppCommonInitialized )
    {
        return;
    }

    m_AppCommonInitialized = true;
    // need to setup theme before the first window or dialog is created
    getAppTheme().selectTheme( getAppSettings().getLastSelectedTheme() );

	m_AppletDownloads = new AppletDownloads( *this, appletFrame );
    m_AppletDownloads->hide();

	m_AppletUploads = new AppletUploads( *this, appletFrame );
	m_AppletUploads->hide();

	m_CreateAccountDlg = new ActivityCreateAccount( *this, appletFrame );

	connectSignals();

	m_IdleTimer->setInterval( 300 ); // we want about 20 frames per second 1000/20 = 50ms
	connect( m_IdleTimer, SIGNAL(timeout()), this, SLOT( onIdleTimer() ) );
	m_IdleTimer->start();

	m_OncePerSecondTimer->setInterval( 1000 ); 
	connect( m_OncePerSecondTimer, SIGNAL(timeout()), this, SLOT( onOncePerSecond() ) );
	m_OncePerSecondTimer->start();

	std::string strAssetDir = m_AppSettings.m_strRootUserDataDir + "assets/";
	VxFileUtil::makeDirectory( strAssetDir );

	m_Engine.fromGuiAppStartup( strAssetDir.c_str(), m_AppSettings.m_strRootUserDataDir.c_str() );

    m_MySndMgr.initAudioIoSystem();
}

//============================================================================
void AppCommon::shutdownAppCommon( void )
{
    static bool hasBeenShutdown = false;
    if( false == hasBeenShutdown )
    {
        hasBeenShutdown = true;
        VxSetAppIsShuttingDown( true );
        m_IdleTimer->stop();
        fromGuiCloseEvent( eAppModuleAll );
        VxSleep( 2000 );
        m_MySndMgr.sndMgrShutdown();
        QApplication::closeAllWindows();
        m_Engine.fromGuiAppShutdown();
    }
}

//============================================================================
void AppCommon::startLogin()
{
    doLogin();
}

//============================================================================
void AppCommon::connectSignals( void )
{
	connect(this, SIGNAL(signalPlaySound(ESndDef)),			this, SLOT(slotPlaySound(ESndDef)) );
	connect(this, SIGNAL(signalStatusMsg(QString)),			this, SLOT(slotStatusMsg(QString)) );
	connect(this, SIGNAL(signalUserMsg(QString)),			this, SLOT(slotStatusMsg(QString)) );
	connect(this, SIGNAL(signalAppErr(EAppErr,QString)),	this, SLOT(slotAppErr(EAppErr,QString)) );
	connect(this, SIGNAL(signalNetworkStateChanged(ENetworkStateType)),	this, SLOT(slotNetworkStateChanged(ENetworkStateType)) );

	connect(this, SIGNAL(signalToGuiRxedPluginOffer(GuiOfferSession *)),		this, SLOT(slotToGuiRxedPluginOffer(GuiOfferSession *)) );
	connect(this, SIGNAL(signalToGuiRxedOfferReply(GuiOfferSession *)),			this, SLOT(slotToGuiRxedOfferReply(GuiOfferSession *)) );
	connect(this, SIGNAL(signalToGuiInstMsg(GuiUser*,EPluginType,QString)),		this, SLOT(slotToGuiInstMsg(GuiUser*,EPluginType,QString)) );

    /*
	connect(this, SIGNAL(signalRemoveContact(VxNetIdent*)),							this, SLOT(slotRemoveContact(VxNetIdent*)));
	connect(this, SIGNAL(signalContactOffline(VxNetIdent*)),						this, SLOT(slotContactOffline(VxNetIdent*)));
	connect(this, SIGNAL(signalContactOnline(VxNetIdent*,bool)),					this, SLOT(slotContactOnline(VxNetIdent*,bool)));
	connect(this, SIGNAL(signalContactNearby(VxNetIdent*)),							this, SLOT(slotContactNearby(VxNetIdent*)));
	connect(this, SIGNAL(signalContactNotNearby(VxNetIdent*)),						this, SLOT(onContactNotNearby(VxNetIdent*)));
	connect(this, SIGNAL(signalContactNameChange(VxNetIdent*)),						this, SLOT(onContactNameChange(VxNetIdent*)));
	connect(this, SIGNAL(signalContactDescChange(VxNetIdent*)),						this, SLOT(onContactDescChange(VxNetIdent*)));
	connect(this, SIGNAL(signalContactMyFriendshipChange(VxNetIdent*)),				this, SLOT(onContactMyFriendshipChange(VxNetIdent*)));
	connect(this, SIGNAL(signalContactHisFriendshipChange(VxNetIdent*)),			this, SLOT(onContactHisFriendshipChange(VxNetIdent*)));
	connect(this, SIGNAL(signalContactPluginPermissionChange(VxNetIdent*)),			this, SLOT(onContactPluginPermissionChange(VxNetIdent*)));
	connect(this, SIGNAL(signalContactSearchFlagsChange(VxNetIdent*)),				this, SLOT(onContactSearchFlagsChange(VxNetIdent*)));
	connect(this, SIGNAL(signalContactLastSessionTimeChange(VxNetIdent*)),			this, SLOT(onContactLastSessionTimeChange(VxNetIdent*)));

	connect(this, SIGNAL(signalUpdateMyIdent(VxNetIdent*)),							this, SLOT(onUpdateMyIdent(VxNetIdent*)));
    */

	//   connect(this, SIGNAL(signalStartDownload(GuiFileXferSession*)),					m_Downloads,	SLOT(slotUpdateDownload(GuiFileXferSession*)));
	//   connect(this, SIGNAL(signalStartUpload(GuiFileXferSession*)),					m_Uploads,		SLOT(onStartUpload(GuiFileXferSession*)));
	//	connect(this, SIGNAL(signalUpdateFileDownload(unsigned long, float, unsigned long)), m_Downloads,	SLOT(onUpdateFileDownload(unsigned long, float, unsigned long)));
	//	connect(this, SIGNAL(signalUpdateFileUpload(unsigned long, float, unsigned long)),	m_Uploads,		SLOT(onUpdateFileUpload(unsigned long, float, unsigned long)));

	//	connect( this, SIGNAL(signalFileDownloadComplete(unsigned long, unsigned long)),m_Downloads,	SLOT(onFileDownloadComplete(unsigned long, unsigned long)));
	//	connect( this, SIGNAL(signalFileUploadComplete(unsigned long, unsigned long)),	m_Uploads,		SLOT(onFileUploadComplete(unsigned long, unsigned long)));

	connect( this, SIGNAL(signalEnableVideoCapture(bool)),							this, SLOT(slotEnableVideoCapture(bool)), Qt::QueuedConnection );
	connect( this, SIGNAL(signalEnableMicrophoneRecording(bool)),					this, SLOT(slotEnableMicrophoneRecording(bool)), Qt::QueuedConnection);
	connect( this, SIGNAL(signalEnableSpeakerOutput(bool)),							this, SLOT(slotEnableSpeakerOutput(bool)), Qt::QueuedConnection);

	//connect( //ui.m_RelayHelpButton, SIGNAL(clicked()),								this, SLOT(slotRelayHelpButtonClicked()));
	connect( this, SIGNAL(signalSetRelayHelpButtonVisibility(bool)),				this, SLOT(slotSetRelayHelpButtonVisibility(bool)));

	//ui.m_ListViewTypeComboBox->addItem( QObject::tr( "List Everybody Except Ignored" ) );
	//ui.m_ListViewTypeComboBox->addItem( QObject::tr( "List Administrators" ) );
	//ui.m_ListViewTypeComboBox->addItem( QObject::tr( "List Friends And Guests" ) );
	//ui.m_ListViewTypeComboBox->addItem( QObject::tr( "List Anonymous Contacts" ) );
	//ui.m_ListViewTypeComboBox->addItem( QObject::tr( "List People I Ignore" ) );
	//ui.m_ListViewTypeComboBox->addItem( QObject::tr( "List My Preferred Relays" ) );
	//ui.m_ListViewTypeComboBox->addItem( QObject::tr( "List All Possible Relays" ) );
	//ui.m_RefreshListButton->setVisible( false );
}

//============================================================================
void AppCommon::setIsMaxScreenSize( bool isMessagerFrame, bool isFullSizeWindow )
{
    m_HomePage.setIsMaxScreenSize( isMessagerFrame, isFullSizeWindow );
    emit signalMainWindowResized();
}

//============================================================================
bool AppCommon::getIsMaxScreenSize( bool isMessagerFrame )
{
    return m_HomePage.getIsMaxScreenSize( isMessagerFrame );
}

//============================================================================
void AppCommon::switchWindowFocus( QWidget * goTvButton )
{
	m_HomePage.switchWindowFocus( goTvButton );
}

//============================================================================
void AppCommon::launchLibraryActivity( uint8_t fileTypeFilter )
{
	if( false == getIsLibraryActivityActive() )
	{
		ActivityViewLibraryFiles * dlg = new ActivityViewLibraryFiles( *this, this, true );
		if( fileTypeFilter )
		{
			EFileFilterType eFilterType = eFileFilterAll;
			switch( fileTypeFilter )
			{
			case VXFILE_TYPE_PHOTO:
				eFilterType = eFileFilterPhoto;
				break;
			case VXFILE_TYPE_AUDIO:
				eFilterType = eFileFilterAudio;
				break;
			case VXFILE_TYPE_VIDEO:
				eFilterType = eFileFilterVideo;
				break;
			case VXFILE_TYPE_DOC:
				eFilterType = eFileFilterDocuments;
				break;
			case VXFILE_TYPE_ARCHIVE_OR_CDIMAGE:
				eFilterType = eFileFilterArchive;
				break;
			case VXFILE_TYPE_OTHER:
				eFilterType = eFileFilterOther;
				break;
			}

			dlg->setFileFilter( eFilterType );
		}

		dlg->show();
	}
}

//============================================================================
bool AppCommon::getIsPluginVisible( EPluginType ePluginType )
{
	QVector<EPluginType>::iterator iter;
	for( iter = m_VisiblePluginsList.begin(); iter != m_VisiblePluginsList.end(); ++iter )
	{
		if( ePluginType == *iter )
		{
			return true;
		}
	}

	return false;
}

//============================================================================
void AppCommon::setPluginVisible( EPluginType ePluginType, bool isVisible )
{
    if( ePluginType == ePluginTypeInvalid )
    {
        return;
    }

	QVector<EPluginType>::iterator iter;
	for( iter = m_VisiblePluginsList.begin(); iter != m_VisiblePluginsList.end(); ++iter )
	{
		if( ePluginType == *iter )
		{
			if( isVisible )
			{
				// already in list
				return;
			}
			else
			{
				// remove from list
				m_VisiblePluginsList.erase( iter );
				return;
			}
		}
	}

	if( isVisible )
	{
		m_VisiblePluginsList.push_back( ePluginType );
	}
}

//============================================================================
QFrame * AppCommon::getAppletFrame( EApplet applet )
{
	return getHomePage().getAppletFrame( applet );
}

//============================================================================
ActivityBase* AppCommon::launchApplet( EApplet applet, QWidget * parent )
{
	return m_AppletMgr.launchApplet( applet, parent );
}

//============================================================================
ActivityBase* AppCommon::launchApplet( EApplet applet, QWidget* parent, VxGUID& assetId )
{
	return m_AppletMgr.launchApplet( applet, parent, 0, assetId );
}

//============================================================================
void AppCommon::activityStateChange( ActivityBase * activity, bool isCreated )
{
	m_AppletMgr.activityStateChange( activity, isCreated );
}

//============================================================================
void AppCommon::startActivity( EPluginType ePluginType, GuiUser * netIdent, QWidget * parent )
{
	if( 0 == parent )
	{
		parent = getCentralWidget();
	}

	bool haveExistingOffer = false;
    /*
	GuiOfferSession * exitingOffer = getOfferMgr().findActiveAndAvailableOffer( m_UserMgr.getUser( netIdent->getMyOnlineId() ), ePluginType );
	if( exitingOffer )
	{
		haveExistingOffer = true;
	}*/

	switch( ePluginType )
	{
	case ePluginTypeCamServer:
		{
        /*
			ActivityWebCamClient * poDlg = new ActivityWebCamClient( *this, netIdent, 0 );
			poDlg->show();
            */
		}

		break;

	case ePluginTypeMessenger:
		{
            /*
			if( false == getIsPluginVisible( ePluginTypeMessenger ) )
			{
				ActivityToFriendMultiSession * poDlg;
				haveExistingOffer ? poDlg = new ActivityToFriendMultiSession( *this, exitingOffer, parent ) 
									: poDlg = new ActivityToFriendMultiSession( *this, netIdent, parent );
				poDlg->show();
			}
            */
		}

		break;

	case ePluginTypeVoicePhone:
		{
        /*
			if( false == getIsPluginVisible( ePluginTypeVoicePhone ) )
			{
				ActivityToFriendVoicePhone * poDlg;
				haveExistingOffer ? poDlg = new ActivityToFriendVoicePhone( *this, exitingOffer, parent ) 
									: poDlg = new ActivityToFriendVoicePhone( *this, netIdent, parent );
				poDlg->show();
			}
            */
		}

		break;

	case ePluginTypeVideoPhone:
		{
        /*
			if( false == getIsPluginVisible( ePluginTypeVideoPhone ) )
			{
				ActivityToFriendVideoPhone * poDlg;
				haveExistingOffer ? poDlg = new ActivityToFriendVideoPhone( *this, exitingOffer, parent ) 
									: poDlg = new ActivityToFriendVideoPhone( *this, netIdent, parent );
				poDlg->show();
			}
            */
		}

		break;

	case ePluginTypeTruthOrDare:
		{
        /*
			if( false == getIsPluginVisible( ePluginTypeTruthOrDare ) )
			{
				ActivityToFriendTodGame * poDlg;
				haveExistingOffer ? poDlg = new ActivityToFriendTodGame( *this, exitingOffer, parent ) 
									: poDlg = new ActivityToFriendTodGame( *this, netIdent, parent );
				poDlg->show();
			}
            */
		}

		break;

	default:
		ActivityMessageBox errMsgBox( *this, this, LOG_ERROR, "AppCommon::startActivity UNKNOWN plugin type %d\n", ePluginType );
		errMsgBox.exec();
	}

	if( haveExistingOffer )
	{
		removePluginSessionOffer( ePluginType, m_UserMgr.getUser( netIdent->getMyOnlineId() ) );
	}
}

//============================================================================
void AppCommon::executeActivity( GuiOfferSession * offer, QWidget * parent )
{
	switch( offer->getPluginType() )
	{
	case ePluginTypeFileXfer:
		{
			//ActivitySessionFileOffer * poDlg = new ActivitySessionFileOffer( *this, offer, parent );
			//poDlg->exec();
		}

		break;

	case ePluginTypeMessenger:
		{
        /*
			if( false == getIsPluginVisible( ePluginTypeMessenger ) )
			{
				ActivityToFriendMultiSession * poDlg = new ActivityToFriendMultiSession( *this, offer, parent );
				poDlg->exec();
			}
            */
		}

		break;

	case ePluginTypeVoicePhone:
		{
			if( false == getIsPluginVisible( ePluginTypeVoicePhone ) )
			{
                /*
				ActivityToFriendVoicePhone * poDlg = new ActivityToFriendVoicePhone( *this, offer, parent );
				poDlg->exec();
                */
			}
		}

		break;

	case ePluginTypeVideoPhone:
		{
			if( false == getIsPluginVisible( ePluginTypeVideoPhone ) )
			{
                /*
				ActivityToFriendVideoPhone * poDlg = new ActivityToFriendVideoPhone( *this, offer, parent );
				poDlg->exec();
                */
			}
		}

		break;

	case ePluginTypeTruthOrDare:
		{
			if( false == getIsPluginVisible( ePluginTypeTruthOrDare ) )
			{
                /*
				ActivityToFriendTodGame * poDlg = new ActivityToFriendTodGame( *this, offer, parent );
				poDlg->exec();
                */
			}
		}

		break;

	default:
		ActivityMessageBox errMsgBox( *this, this, LOG_ERROR, "AppCommon::executeActivity UNKNOWN plugin type %d\n", offer->getPluginType() );
		errMsgBox.exec();
	}
}

/*
//============================================================================
void AppCommon::slotRefreshListButtonClick( void )
{
	//ui.mainFriendList->clear();
	//ui.mainFriendList->update();
	//LogMsg( LOG_INFO, "AppCommon::slotRefreshListButtonClick count %d\n", //ui.mainFriendList->count() );
	getEngine().fromGuiRefreshContactList( 500 );
}*/

//============================================================================
void AppCommon::setCamCaptureRotation( uint32_t rot )
{ 
	m_CamCaptureRotation = rot; 
}

//============================================================================
void AppCommon::slotMainWindowResized( void )
{
	emit signalMainWindowResized();
}

//============================================================================
void AppCommon::slotMainWindowMoved( void )
{
    emit signalMainWindowMoved();
}

//============================================================================
VxNetIdent * AppCommon::getMyIdentity( void )
{
	return getAppGlobals().getUserIdent();
}

//============================================================================
VxGUID AppCommon::getMyOnlineId( void )
{
    return getAppGlobals().getUserIdent()->getMyOnlineId();
}

//============================================================================
void AppCommon::applySoundSettings( bool useDefaultsInsteadOfSettings )
{
	if( useDefaultsInsteadOfSettings )
	{
		m_MySndMgr.muteNotifySound( false );
		m_MySndMgr.mutePhoneRing( false );
	}
	else
	{
		m_MySndMgr.muteNotifySound( m_AppSettings.getMuteNotifySound() );
		m_MySndMgr.mutePhoneRing( m_AppSettings.getMutePhoneRing() );
	}
}

//============================================================================
void AppCommon::playSound( ESndDef sndDef )
{
	emit signalPlaySound( sndDef );
}

//============================================================================
void AppCommon::slotPlaySound( ESndDef sndDef )
{
	getSoundMgr().playSnd( sndDef );
}

//============================================================================
void AppCommon::insertKeystroke( int keyCode )
{
	if( 0 == keyCode )
	{
		LogMsg( LOG_ERROR, "AppCommon::insertKeystroke 0 keyCode\n" );
		return;
	}

	QWidget *receiver = QApplication::focusWidget();
	if( 0 == receiver )
	{
		receiver = getQApplication().activeWindow();
	}

	if( 0 == receiver )
	{
		LogMsg( LOG_DEBUG,  "AppCommon::insertKeystroke no reciever\n" );
		return;
	}

#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
    Qt::KeyboardModifiers keyFlags = 0;
#else
    Qt::KeyboardModifiers keyFlags;
#endif // QT_VERSION < QT_VERSION_CHECK(6,0,0)

	QString keyText = "";

	QKeyEvent * qtKeyEvent = new QKeyEvent( QEvent::KeyPress, keyCode, keyFlags, keyText );
	QCoreApplication::postEvent( receiver, qtKeyEvent );
	qtKeyEvent = new QKeyEvent( QEvent::KeyRelease, keyCode, keyFlags, keyText );
	QCoreApplication::postEvent( receiver, qtKeyEvent );
}

//============================================================================
void AppCommon::toGuiLog( int logFlags, const char * pMsg )
{
	if( VxIsAppShuttingDown() 
		|| ( 0 == pMsg ) )
	{
		return;
	}

    QString strMsg = pMsg;
#if QT_VERSION > QT_VERSION_CHECK(6,0,0)
	strMsg.remove(QRegularExpression("[\\n\\r]"));
#else
	strMsg.remove(QRegExp("[\\n\\r]"));
#endif // QT_VERSION > QT_VERSION_CHECK(6,0,0)

#ifdef TARGET_OS_WINDOWS
    qDebug() << strMsg;
#endif // TARGET_OS_WINDOWS
    emit signalLog( logFlags, strMsg );

	//m_AppMutex.lock();
	//m_AppErrLogQue.push_back( pMsg );
	//if( m_DebugLogQue.size() > 500 )
	//{
	//	m_DebugLogQue.erase( m_DebugLogQue.begin() );
	//}
	//m_AppMutex.unlock();
}

//============================================================================
void AppCommon::toGuiAppErr( EAppErr eAppErr, const char* errMsg )
{
	if( VxIsAppShuttingDown() )
	{
		return;
	}

	std::string formatedErr;
	if( errMsg )
	{
		StdStringFormat( formatedErr, "#App Error %d %s", eAppErr, errMsg );
	}
	else
	{
		StdStringFormat( formatedErr, "#App Error %d\n", eAppErr );
	}

	//emit signalAppErr( eAppErr, formatedErr.c_str() );
	emit signalLog( LOG_ERROR, formatedErr.c_str() );
	emit signalStatusMsg( formatedErr.c_str() );
}

//============================================================================
void AppCommon::toGuiStatusMessage( const char * statusMsg )
{
	if( VxIsAppShuttingDown() )
	{
		return;
	}

	if( statusMsg )
	{
		emit signalLog( LOG_INFO, statusMsg );
		emit signalStatusMsg( statusMsg );
	}
}

//============================================================================
// NOTE: toGuiUserMessage should be called from in gui on gui thread only
void AppCommon::toGuiUserMessage( const char * userMsg, ... )
{
	if( VxIsAppShuttingDown() )
	{
		return;
	}

	char szBuffer[2048];
	szBuffer[0] = 0;
	va_list arg_ptr;
	va_start(arg_ptr, userMsg);
#ifdef TARGET_OS_WINDOWS
	vsnprintf(szBuffer, 2048, userMsg,(char *) arg_ptr);
#else
    vsnprintf(szBuffer, 2048, userMsg, arg_ptr);
#endif //  TARGET_OS_WINDOWS
	szBuffer[2047] = 0;
	va_end(arg_ptr);

	if( 0 != szBuffer[0] )
	{
		emit signalStatusMsg( szBuffer );
	}
}
//============================================================================
/// Send Network available status to GUI for display
void AppCommon::toGuiNetAvailableStatus( ENetAvailStatus eNetAvailStatus )
{
    if( VxIsAppShuttingDown() )
    {
        return;
    }

    emit signalNetAvailStatus( eNetAvailStatus );
}

//============================================================================
void AppCommon::toGuiNetworkState( ENetworkStateType eNetworkState, const char* stateMsg )
{
	if( VxIsAppShuttingDown() )
	{
		return;
	}

	m_LastNetworkState = eNetworkState;
	if( eNetworkStateTypeOnlineDirect == eNetworkState )
	{
		emit signalSetRelayHelpButtonVisibility( false );
	}
	else if( eNetworkStateTypeOnlineThroughRelay == eNetworkState )
	{
		emit signalSetRelayHelpButtonVisibility( true );
	}

	emit signalNetworkStateChanged( eNetworkState );

    if( IsLogEnabled( eLogNetworkState ) )
    {
        const char * networkState = DescribeNetworkState( eNetworkState );
        std::string formatedMsg;
        if( stateMsg )
        {
            StdStringFormat( formatedMsg, "#Network %s %s", networkState, stateMsg );
        }
        else
        {
            StdStringFormat( formatedMsg, "#Network %s\n", networkState );
        }

        emit signalStatusMsg( formatedMsg.c_str() );
    }
}

//============================================================================
void AppCommon::slotNetworkStateChanged( ENetworkStateType eNetworkState )
{
	switch( eNetworkState )
	{
	case eNetworkStateTypeUnknown:
	case eNetworkStateTypeLost:
	case eNetworkStateTypeNoInternetConnection:
		//ui.notifyButton->setIcon( getMyIcons().getIcon( eMyIconNetworkStateDisconnected ) );
		//ui.searchButton->setEnabled( false );
		//ui.searchButton->setIcon( getMyIcons().getIcon( eMyIconSearchDisabled ) );
		break;

	case eNetworkStateTypeAvail:
	case eNetworkStateTypeTestConnection:
	case eNetworkStateTypeRelaySearch:
	case eNetworkStateTypeAnnounce:
	case eNetworkStateTypeGetRelayList:
		//ui.notifyButton->setIcon( getMyIcons().getIcon( eMyIconNetworkStateDiscover ) );
		break;

	case eNetworkStateTypeOnlineDirect:
		//ui.notifyButton->setIcon( getMyIcons().getIcon( eMyIconNetworkStateDirectConnected ) );
		//ui.searchButton->setEnabled( true );
		//ui.searchButton->setIcon( getMyIcons().getIcon( eMyIconSearch ) );
		break;

	case eNetworkStateTypeOnlineThroughRelay:
		//ui.notifyButton->setIcon( getMyIcons().getIcon( eMyIconNetworkStateRelayConnected ) );
		//ui.searchButton->setEnabled( true );
		//ui.searchButton->setIcon( getMyIcons().getIcon( eMyIconSearch ) );
		break;

	case eNetworkStateTypeFailedResolveHostNetwork:
		//ui.notifyButton->setIcon( getMyIcons().getIcon( eMyIconNetworkStateHostFail ) );
		break;
    case eNetworkStateTypeFailedResolveHostGroup:
        //ui.notifyButton->setIcon( getMyIcons().getIcon( eMyIconNetworkStateHostFail ) );
        break;
    case eNetworkStateTypeFailedResolveHostGroupList:
        //ui.notifyButton->setIcon( getMyIcons().getIcon( eMyIconNetworkStateHostFail ) );
        break;
    case eMaxNetworkStateType:
        //ui.notifyButton->setIcon( getMyIcons().getIcon( eMyIconNetworkStateHostFail ) );
        break;
	}
}

//============================================================================
void AppCommon::toGuiMyRelayStatus( EMyRelayStatus eRelayStatus, const char * msg )
{
	if( VxIsAppShuttingDown() )
	{
		return;
	}

	if( eMyRelayStatusSearching == eRelayStatus )
	{
		emit signalSetRelayHelpButtonVisibility( true );
	}

	const char * relayStatus = DescribeRelayStatus( eRelayStatus );
	std::string formatedMsg;
	if( msg )
	{
		StdStringFormat( formatedMsg, "#Relay %s %s", relayStatus, msg );
	}
	else
	{
		StdStringFormat( formatedMsg, "#Relay %s\n", relayStatus );
	}

	emit signalLog( 0, formatedMsg.c_str() );
	emit signalStatusMsg( formatedMsg.c_str() );

	if( eMyRelayStatusConnected == eRelayStatus )
	{
		emit signalStatusMsg( "Relay Connected " );
		if( ( eFriendViewMyProxies == m_eLastSelectedWhichContactsToView ) ||
			( eFriendViewAllProxies == m_eLastSelectedWhichContactsToView ) )
		{
			//ui.mainFriendList->refreshFriendList( eFriendViewEverybody );
		}
	}
}

//============================================================================
void AppCommon::toGuiHostAnnounceStatus( EHostType hostType, VxGUID& sessionId, EHostAnnounceStatus annStatus, const char * msg )
{
    if( VxIsAppShuttingDown() )
    {
        return;
    }

    const char * hostStatus = DescribeHostAnnounceStatus( annStatus );
    std::string formatedMsg;
    if( msg )
    {
        StdStringFormat( formatedMsg, "#%s %s", hostStatus, msg );
    }
    else
    {
        StdStringFormat( formatedMsg, "#%s\n", hostStatus );
    }

    emit signalLog( 0, formatedMsg.c_str() );
    emit signalStatusMsg( formatedMsg.c_str() );

    emit signalHostAnnounceStatus( hostType, sessionId, annStatus, formatedMsg.c_str() );
}

//============================================================================
void AppCommon::toGuiHostJoinStatus( EHostType hostType, VxGUID& sessionId, EHostJoinStatus joinStatus, const char * msg )
{
	if( VxIsAppShuttingDown() )
	{
		return;
	}

	const char * hostStatus = DescribeHostJoinStatus( joinStatus );
	std::string formatedMsg;
	if( msg )
	{
		StdStringFormat( formatedMsg, "#%s %s", hostStatus, msg );
	}
	else
	{
		StdStringFormat( formatedMsg, "#%s\n", hostStatus );
	}

	emit signalLog( 0, formatedMsg.c_str() );
	emit signalStatusMsg( formatedMsg.c_str() );

	emit signalHostJoinStatus( hostType, sessionId, joinStatus, formatedMsg.c_str() );
}

//============================================================================
void AppCommon::toGuiHostSearchStatus( EHostType hostType, VxGUID& sessionId, EHostSearchStatus searchStatus, ECommErr commErr, const char * msg )
{
    if( VxIsAppShuttingDown() )
    {
        return;
    }

    QString hostStatus = GuiParams::describeHostSearchStatus( searchStatus );
    if( eCommErrNone != commErr )
    {
        hostStatus += GuiParams::describeCommError( commErr );
    }

    if( msg )
    {
        hostStatus += msg;
    }

    emit signalLog( 0, hostStatus );
    emit signalStatusMsg( hostStatus );

    emit signalHostSearchStatus( hostType, sessionId, searchStatus, hostStatus );
}

//============================================================================
void AppCommon::toGuiHostSearchResult( EHostType hostType, VxGUID& sessionId, VxNetIdent &hostIdent, PluginSetting &pluginSetting )
{
    if( VxIsAppShuttingDown() )
    {
        return;
    }

    emit signalHostSearchResult( hostType, sessionId, hostIdent, pluginSetting );
}


//============================================================================
void AppCommon::toGuiUserOnlineStatus( EHostType hostType, VxNetIdent *hostIdent, VxGUID& sessionId, bool isOnline )
{
    if( VxIsAppShuttingDown() )
    {
        return;
    }

    emit signalUserOnlineStatus( hostType, sessionId, *hostIdent, isOnline );
}

//============================================================================
void AppCommon::toGuiIsPortOpenStatus( EIsPortOpenStatus eIsPortOpenStatus, const char * msg )
{
	if( VxIsAppShuttingDown() )
	{
		return;
	}

	const char * portOpenStatus = DescribePortOpenStatus( eIsPortOpenStatus );
	std::string formatedMsg;
	if( msg )
	{
		StdStringFormat( formatedMsg, "#%s %s", portOpenStatus, msg );
	}
	else
	{
		StdStringFormat( formatedMsg, "#%s", portOpenStatus );
	}

	emit signalLog( 0, formatedMsg.c_str() );
	emit signalStatusMsg( formatedMsg.c_str() );

	emit signalIsPortOpenStatus( eIsPortOpenStatus, formatedMsg.c_str() );
}

//============================================================================
void AppCommon::toGuiRunTestStatus( const char *testName, ERunTestStatus eRunTestStatus, const char * msg )
{
    if( VxIsAppShuttingDown() )
    {
        return;
    }

    const char * runTestStatus = DescribeRunTestStatus( eRunTestStatus );
    std::string formatedMsg;
    if( msg )
    {
        StdStringFormat( formatedMsg, "#%s %s", runTestStatus, msg );
    }
    else
    {
        StdStringFormat( formatedMsg, "#%s", runTestStatus );
    }

    emit signalLog( 0, formatedMsg.c_str() );
    emit signalStatusMsg( formatedMsg.c_str() );

    emit signalRunTestStatus( testName,  eRunTestStatus, formatedMsg.c_str() );
}

//============================================================================
void AppCommon::toGuiRandomConnectStatus( ERandomConnectStatus eRandomConnectStatus, const char * msg )
{
	if( VxIsAppShuttingDown() )
	{
		return;
	}

	const char * phoneShakeStatus = DescribeRandomConnectStatus( eRandomConnectStatus );
	std::string formatedMsg;
	if( msg )
	{
		StdStringFormat( formatedMsg, "#%s", msg );
	}
	else
	{
		StdStringFormat( formatedMsg, "#%s", phoneShakeStatus );
	}

	emit signalLog( 0, formatedMsg.c_str() );
	emit signalStatusMsg( formatedMsg.c_str() );

	emit signalRandomConnectStatus( eRandomConnectStatus, formatedMsg.c_str() );
}

//============================================================================
void AppCommon::startNetworkMonitor( void )
{
	std::string overrideWirelessIp = m_Engine.getEngineSettings().getPreferredNetworkAdapterIp();
	m_Engine.getNetworkMonitor().networkMonitorStartup( overrideWirelessIp.c_str() );
}

//============================================================================
	// prompt user to confirm wants to shutdown app.. caller must call appCommonShutdown if answer is yes
bool AppCommon::confirmAppShutdown( QWidget * parentWindow )
{
	if( m_bUserCanceledCreateProfile )
	{
		// we are shutting down because user canceled creating profile
		return true;
	}
		
	//event->ignore();	    
	//Setting parent will keep message box in the center of QMainWindow
	QMessageBox* msgBox = new QMessageBox(this);
	msgBox->setWindowTitle( QObject::tr( "Warning" ) );
	msgBox->setText( QObject::tr( "Are you sure you want to exit?" ) );    

	QPushButton *yesButton = msgBox->addButton( QObject::tr("Yes"), QMessageBox::ActionRole );
	msgBox->addButton( QObject::tr("No"), QMessageBox::ActionRole );  
	msgBox->exec();  

	//If user clicks 'Yes'  button , accept QCloseEvent (Close Window)
	if ((QPushButton*)msgBox->clickedButton() == yesButton) 
	{
		playSound( eSndDefByeBye );

		//shutdownAppCommon();
		//event->accept();
		//QApplication::closeAllWindows();
		return true;
	}

	return false;
}; 

//============================================================================
bool AppCommon::userCanceled( void )
{
	return m_bUserCanceledCreateProfile;
}

//============================================================================
//! idle timer 20 frames per second
void AppCommon::onIdleTimer()
{
    if( VxIsAppShuttingDown() )
    {
        return;
    }

	m_Engine.fromGuiAppIdle();
}

//============================================================================
void AppCommon::onOncePerSecond( void )
{
    // the wait is probably no longer needed since not running seperate threads for loading
    // but it does give other things a chance to run a bit
    static int waitCnt = 0;
    if( waitCnt > 1 )
    {
        if( !m_LoginBegin )
        {
            m_LoginBegin = true;
            startLogin();
        }

        if( getLoginCompleted() )
        {
            getEngine().onOncePerSecond();
        }
    }
    else
    {
        waitCnt++;
        LogMsg( LOG_DEBUG, "Wait to login seconds %d alive ms %lld", waitCnt, GetApplicationAliveMs() );
    }
}

//============================================================================
//! time to flash the notify icon
void AppCommon::slotOnNotifyIconFlashTimeout( bool bWhite )
{
	if( bWhite )
	{
		////ui.notifyButton->setIcon( getMyIcons().getIcon( eMyIconNotifyStarWhite ) );	
	}
	else
	{
		////ui.notifyButton->setIcon( getMyIcons().getIcon( eMyIconNotifyStarGreen ) );	
	}
}

//============================================================================
void AppCommon::slotStatusMsg( QString strMsg )
{
	LogMsg( LOG_INFO, strMsg.toStdString().c_str() );
	//ui.StatusMsgLabel->setText( strMsg );
	if(strMsg.isEmpty())
	{
		//ui.StatusMsgLabel->setVisible( false );
	}
	else
	{
		//ui.StatusMsgLabel->setVisible( true );
	}
}

//============================================================================
void AppCommon::slotAppErr( EAppErr eAppErr, QString errMsg )
{
	//errMsg.remove(QRegExp("[\\n\\r]"));
	QMessageBox::warning(this, tr("Application Error"), errMsg );
}

//============================================================================
bool AppCommon::toGuiSetGameValueVar(			EPluginType	ePluginType, 
												VxGUID&		onlineId, 
												int32_t			s32VarId, 
												int32_t			s32VarValue )
{
	if( VxIsAppShuttingDown() )
	{
		return false;
	}

#ifdef DEBUG_TOGUI_CLIENT_MUTEX
	LogMsg( LOG_INFO, "toGuiSetGameValueVar: toGuiActivityClientsLock\n" );
#endif // DEBUG_TOGUI_CLIENT_MUTEX
	toGuiActivityClientsLock();
	std::vector<ToGuiActivityClient>::iterator iter;
	for( iter = m_ToGuiActivityClientList.begin(); iter != m_ToGuiActivityClientList.end(); ++iter )
	{
		ToGuiActivityClient& client = *iter;
		client.m_Callback->toGuiSetGameValueVar( client.m_UserData, ePluginType, onlineId, s32VarId, s32VarValue );
	}

	toGuiActivityClientsUnlock();
	return true;
}

//============================================================================
bool AppCommon::toGuiSetGameActionVar(	EPluginType	ePluginType, 
											VxGUID&		onlineId, 
											int32_t			s32VarId, 
											int32_t			s32VarValue )
{
	if( VxIsAppShuttingDown() )
	{
		return false;
	}

#ifdef DEBUG_TOGUI_CLIENT_MUTEX
	LogMsg( LOG_INFO, "toGuiSetGameActionVar: toGuiActivityClientsLock\n" );
#endif // DEBUG_TOGUI_CLIENT_MUTEX
	toGuiActivityClientsLock();
	std::vector<ToGuiActivityClient>::iterator iter;
	for( iter = m_ToGuiActivityClientList.begin(); iter != m_ToGuiActivityClientList.end(); ++iter )
	{
		ToGuiActivityClient& client = *iter;
		client.m_Callback->toGuiSetGameActionVar( client.m_UserData, ePluginType, onlineId, s32VarId, s32VarValue );
	}

	toGuiActivityClientsUnlock();
	return true;
}

//============================================================================
void AppCommon::slotRelayHelpButtonClicked( void )
{
	ActivityShowHelp * helpDlg = new ActivityShowHelp( *this, this );
	helpDlg->show();
}

//============================================================================
void AppCommon::slotSetRelayHelpButtonVisibility( bool isVisible )
{
	//ui.m_RelayHelpButton->setVisible( isVisible );
}

//============================================================================
void AppCommon::toGuiAssetAdded( AssetBaseInfo * assetInfo )
{
	if( VxIsAppShuttingDown() )
	{
		return;
	}

	if( IsLogEnabled( eLogAssets ) )
		LogMsg( LOG_INFO, "toGuiAssetAdded: toGuiActivityClientsLock\n" );
	//#endif // DEBUG_TOGUI_CLIENT_MUTEX

	toGuiActivityClientsLock();
	std::vector<ToGuiActivityClient>::iterator iter;
	for( iter = m_ToGuiActivityClientList.begin(); iter != m_ToGuiActivityClientList.end(); ++iter )
	{
		ToGuiActivityClient& client = *iter;
		client.m_Callback->toGuiAssetAdded( client.m_UserData, assetInfo );
	}

	if( IsLogEnabled( eLogAssets ) )
		LogMsg( LOG_INFO, "toGuiAssetAdded toGuiActivityClientsUnlock\n");

	toGuiActivityClientsUnlock();

	//emit signalAssetAdded( assetInfo );
}

//============================================================================
void AppCommon::toGuiAssetSessionHistory( AssetBaseInfo * assetInfo )
{
	if( VxIsAppShuttingDown() )
	{
		return;
	}

	//emit signalSessionHistory( assetInfo );
	//#ifdef DEBUG_TOGUI_CLIENT_MUTEX
	LogMsg( LOG_INFO, "toGuiAssetSessionHistory: toGuiActivityClientsLock\n" );
	//#endif // DEBUG_TOGUI_CLIENT_MUTEX

	toGuiActivityClientsLock();
	std::vector<ToGuiActivityClient>::iterator iter;
	for( iter = m_ToGuiActivityClientList.begin(); iter != m_ToGuiActivityClientList.end(); ++iter )
	{
		ToGuiActivityClient& client = *iter;
		client.m_Callback->toGuiAssetSessionHistory( client.m_UserData, assetInfo );
	}

	//#ifdef DEBUG_TOGUI_CLIENT_MUTEX
	LogMsg( LOG_INFO, "toGuiAssetSessionHistory toGuiActivityClientsUnlock\n");
	//#endif // DEBUG_TOGUI_CLIENT_MUTEX

	toGuiActivityClientsUnlock();
}

//============================================================================
void AppCommon::toGuiAssetAction( EAssetAction assetAction, VxGUID& assetId, int pos0to100000 )
{
    if( VxIsAppShuttingDown() )
    {
        return;
    }

    if( ( eAssetActionRxNotifyNewMsg == assetAction )
        || ( eAssetActionRxViewingMsg == assetAction ) )
    {
        VxGUID qAssetViewId( assetId.getVxGUIDHiPart(), assetId.getVxGUIDLoPart() );
        emit signalAssetViewMsgAction( assetAction, qAssetViewId, pos0to100000 );
        return;
    }

#ifdef DEBUG_TOGUI_CLIENT_MUTEX
    LogMsg( LOG_INFO, "toGuiAssetAction: toGuiActivityClientsLock\n" );
#endif // DEBUG_TOGUI_CLIENT_MUTEX
    toGuiActivityClientsLock();
    std::vector<ToGuiActivityClient>::iterator iter;
    for( iter = m_ToGuiActivityClientList.begin(); iter != m_ToGuiActivityClientList.end(); ++iter )
    {
        ToGuiActivityClient& client = *iter;
        client.m_Callback->toGuiClientAssetAction( client.m_UserData, assetAction, assetId, pos0to100000 );
    }

#ifdef DEBUG_TOGUI_CLIENT_MUTEX
    LogMsg( LOG_INFO, "toGuiAssetAction toGuiActivityClientsUnlock\n" );
#endif // DEBUG_TOGUI_CLIENT_MUTEX
    toGuiActivityClientsUnlock();
}

//============================================================================
void AppCommon::toGuiMultiSessionAction( EMSessionAction mSessionAction, VxGUID& onlineId, int pos0to100000 )
{
    if( VxIsAppShuttingDown() )
    {
        return;
    }

    VxGUID idPro( onlineId.getVxGUIDHiPart(), onlineId.getVxGUIDLoPart() );
    emit signalMultiSessionAction( idPro, mSessionAction, pos0to100000 );
#ifdef DEBUG_TOGUI_CLIENT_MUTEX
    LogMsg( LOG_INFO, "toGuiMultiSessionAction: toGuiActivityClientsLock\n" );
#endif // DEBUG_TOGUI_CLIENT_MUTEX
    toGuiActivityClientsLock();
    std::vector<ToGuiActivityClient>::iterator iter;
    for( iter = m_ToGuiActivityClientList.begin(); iter != m_ToGuiActivityClientList.end(); ++iter )
    {
        ToGuiActivityClient& client = *iter;
        client.m_Callback->toGuiMultiSessionAction( client.m_UserData, mSessionAction, onlineId, pos0to100000 );
    }

    toGuiActivityClientsUnlock();
}

//============================================================================
void AppCommon::toGuiBlobAction( EAssetAction assetAction, VxGUID& assetId, int pos0to100000 )
{
	if( VxIsAppShuttingDown() )
	{
		return;
	}

	if( ( eAssetActionRxNotifyNewMsg == assetAction )
		|| ( eAssetActionRxViewingMsg == assetAction ) )
	{
		VxGUID qAssetViewId( assetId.getVxGUIDHiPart(), assetId.getVxGUIDLoPart() );
		emit signalBlobViewMsgAction( assetAction, qAssetViewId, pos0to100000 );
		return;
	}

#ifdef DEBUG_TOGUI_CLIENT_MUTEX
	LogMsg( LOG_INFO, "toGuiAssetAction: toGuiActivityClientsLock\n" );
#endif // DEBUG_TOGUI_CLIENT_MUTEX
	toGuiActivityClientsLock();
	std::vector<ToGuiActivityClient>::iterator iter;
	for( iter = m_ToGuiActivityClientList.begin(); iter != m_ToGuiActivityClientList.end(); ++iter )
	{
		ToGuiActivityClient& client = *iter;
		client.m_Callback->toGuiClientBlobAction( client.m_UserData, assetAction, assetId, pos0to100000 );
	}

#ifdef DEBUG_TOGUI_CLIENT_MUTEX
	LogMsg( LOG_INFO, "toGuiAssetAction toGuiActivityClientsUnlock\n");
#endif // DEBUG_TOGUI_CLIENT_MUTEX
	toGuiActivityClientsUnlock();
}

//============================================================================
void AppCommon::toGuiBlobAdded( BlobInfo * assetInfo )
{
    if( VxIsAppShuttingDown() )
    {
        return;
    }

    if( IsLogEnabled( eLogAssets ) )
        LogMsg( LOG_INFO, "toGuiBlobAdded: toGuiActivityClientsLock\n" );
    //#endif // DEBUG_TOGUI_CLIENT_MUTEX

    toGuiActivityClientsLock();
    std::vector<ToGuiActivityClient>::iterator iter;
    for( iter = m_ToGuiActivityClientList.begin(); iter != m_ToGuiActivityClientList.end(); ++iter )
    {
        ToGuiActivityClient& client = *iter;
        client.m_Callback->toGuiBlobAdded( client.m_UserData, assetInfo );
    }

    if( IsLogEnabled( eLogAssets ) )
        LogMsg( LOG_INFO, "toGuiBlobAdded toGuiActivityClientsUnlock\n" );

    toGuiActivityClientsUnlock();

    //emit signalAssetAdded( assetInfo );
}

//============================================================================
void AppCommon::toGuiBlobSessionHistory( BlobInfo * assetInfo )
{
    if( VxIsAppShuttingDown() )
    {
        return;
    }

    //emit signalSessionHistory( assetInfo );
    //#ifdef DEBUG_TOGUI_CLIENT_MUTEX
    LogMsg( LOG_INFO, "toGuiBlobSessionHistory: toGuiActivityClientsLock\n" );
    //#endif // DEBUG_TOGUI_CLIENT_MUTEX

    toGuiActivityClientsLock();
    std::vector<ToGuiActivityClient>::iterator iter;
    for( iter = m_ToGuiActivityClientList.begin(); iter != m_ToGuiActivityClientList.end(); ++iter )
    {
        ToGuiActivityClient& client = *iter;
        client.m_Callback->toGuiBlobSessionHistory( client.m_UserData, assetInfo );
    }

    //#ifdef DEBUG_TOGUI_CLIENT_MUTEX
    LogMsg( LOG_INFO, "toGuiBlobSessionHistory toGuiActivityClientsUnlock\n" );
    //#endif // DEBUG_TOGUI_CLIENT_MUTEX

    toGuiActivityClientsUnlock();
}

//============================================================================
void AppCommon::okMessageBox( QString title, QString msg )
{
	QMessageBox::information( this, title, msg, QMessageBox::Ok );
}

//============================================================================
void AppCommon::okMessageBox2( QString title, const char * msg, ... )
{
	char szBuffer[2048];
	va_list argList;
	va_start(argList, msg);
	vsnprintf( szBuffer, sizeof( szBuffer ), msg, argList );
	va_end(argList);

	okMessageBox( title, szBuffer );
}

//============================================================================
bool AppCommon::yesNoMessageBox( QString title, QString msg )
{
	if( QMessageBox::Yes == QMessageBox::question( this, title, msg, QMessageBox::Yes | QMessageBox::No ) )
	{
		return true;
	}

	return false;
}

//============================================================================
bool AppCommon::yesNoMessageBox2( QString title, const char * msg, ... )
{
	char szBuffer[2048];
	va_list argList;
	va_start(argList, msg);
	vsnprintf( szBuffer, sizeof( szBuffer ), msg, argList );
	va_end(argList);

	return yesNoMessageBox( title, szBuffer );
}

//============================================================================
void AppCommon::errMessageBox( QString title, QString msg )
{
	QMessageBox::warning( this, title, msg, QMessageBox::Ok );
}

//============================================================================
void AppCommon::errMessageBox2( QString title, const char * msg, ... )
{
	char szBuffer[2048];
	va_list argList;
	va_start(argList, msg);
	vsnprintf( szBuffer, sizeof( szBuffer ), msg, argList );
	va_end(argList);

	errMessageBox( title, szBuffer );
}

//============================================================================
void AppCommon::toGuiActivityClientsLock( void )
{
	if( VxIsAppShuttingDown() )
	{
		return;
	}

#ifdef DEBUG_TOGUI_CLIENT_MUTEX
	LogMsg( LOG_INFO, "m_ToGuiActivityClientMutex.lock() start\n");
#endif // DEBUG_TOGUI_CLIENT_MUTEX
	m_ToGuiActivityClientMutex.lock();
#ifdef DEBUG_TOGUI_CLIENT_MUTEX
	LogMsg( LOG_INFO, "m_ToGuiActivityClientMutex.lock() done\n");
#endif // DEBUG_TOGUI_CLIENT_MUTEX
}

//============================================================================
void AppCommon::toGuiActivityClientsUnlock( void )
{
#ifdef DEBUG_TOGUI_CLIENT_MUTEX
	LogMsg( LOG_INFO, "m_ToGuiActivityClientMutex.unlock()\n");
#endif // DEBUG_TOGUI_CLIENT_MUTEX
	m_ToGuiActivityClientMutex.unlock();
}

//============================================================================
void AppCommon::wantToGuiActivityCallbacks(	ToGuiActivityInterface *	callback, 
											void *						userData,
											bool						wantCallback )
{
static bool actCallbackShutdownComplete = false;
	if( VxIsAppShuttingDown() )
	{
		if( actCallbackShutdownComplete )
		{
			return;
		}

		actCallbackShutdownComplete = true;
		clearToGuiActivityClientList();
		return;
	}

#ifdef DEBUG_TOGUI_CLIENT_MUTEX
	LogMsg( LOG_INFO, "wantToGuiActivityCallbacks: toGuiActivityClientsLock\n" );
#endif // DEBUG_TOGUI_CLIENT_MUTEX
	toGuiActivityClientsLock();

	if( wantCallback )
	{
		std::vector<ToGuiActivityClient>::iterator iter;
		for( iter = m_ToGuiActivityClientList.begin(); iter != m_ToGuiActivityClientList.end(); ++iter )
		{
			ToGuiActivityClient& client = *iter;
			if( ( client.m_Callback == callback )
				&& ( client.m_UserData == userData ) )
			{
				LogMsg( LOG_INFO, "WARNING. Ignoring New ToGuiActivityClient because already in list\n" );
				toGuiActivityClientsUnlock();
				return;
			}
		}

		ToGuiActivityClient newClient( callback, userData );
		m_ToGuiActivityClientList.push_back( newClient );
		toGuiActivityClientsUnlock();
		return;
	}

	std::vector<ToGuiActivityClient>::iterator iter;
	for( iter = m_ToGuiActivityClientList.begin(); iter != m_ToGuiActivityClientList.end(); ++iter )
	{
		ToGuiActivityClient& client = *iter;
		if( ( client.m_Callback == callback )
			&& ( client.m_UserData == userData ) )
		{
			m_ToGuiActivityClientList.erase( iter );
			toGuiActivityClientsUnlock();
			return;
		}
	}

	LogMsg( LOG_INFO, "WARNING. ToGuiActivityClient remove not found in list\n" );
	toGuiActivityClientsUnlock();
	return;
}

//============================================================================
void AppCommon::clearToGuiActivityClientList( void )
{
	if( m_ToGuiActivityClientList.size() )
	{
		toGuiActivityClientsLock();
		m_ToGuiActivityClientList.clear();
		toGuiActivityClientsUnlock();
	}
}

//============================================================================
void AppCommon::clearFileXferClientList( void )
{
	if( m_ToGuiFileXferClientList.size() )
	{
        m_ToGuiFileXferClientMutex.lock();
		m_ToGuiFileXferClientList.clear();
		m_ToGuiFileXferClientMutex.unlock();
	}
}

//============================================================================
void AppCommon::toGuiFileXferClientsLock( void )
{
	if( VxIsAppShuttingDown() )
	{
		return;
	}

#ifdef DEBUG_TOGUI_CLIENT_MUTEX
	LogMsg( LOG_INFO, "m_ToGuiFileXferClientMutex.lock() start\n");
#endif // DEBUG_TOGUI_CLIENT_MUTEX
	m_ToGuiFileXferClientMutex.lock();
#ifdef DEBUG_TOGUI_CLIENT_MUTEX
	LogMsg( LOG_INFO, "m_ToGuiFileXferClientMutex.lock() done\n");
#endif // DEBUG_TOGUI_CLIENT_MUTEX
}

//============================================================================
void AppCommon::toGuiFileXferClientsUnlock( void )
{
#ifdef DEBUG_TOGUI_CLIENT_MUTEX
	LogMsg( LOG_INFO, "m_ToGuiFileXferClientMutex.unlock()\n");
#endif // DEBUG_TOGUI_CLIENT_MUTEX
	m_ToGuiFileXferClientMutex.unlock();
}

//============================================================================
void AppCommon::wantToGuiFileXferCallbacks(	ToGuiFileXferInterface *	callback, 
											void *						userData,
											bool						wantCallback )
{
	if( VxIsAppShuttingDown() )
	{
		return;
	}
#ifdef DEBUG_TOGUI_CLIENT_MUTEX
	LogMsg( LOG_INFO, "wantToGuiFileXferCallbacks: toGuiFileXferClientsLock\n" );
#endif // DEBUG_TOGUI_CLIENT_MUTEX
	toGuiFileXferClientsLock();

	if( wantCallback )
	{
		std::vector<ToGuiFileXferClient>::iterator iter;
		for( iter = m_ToGuiFileXferClientList.begin(); iter != m_ToGuiFileXferClientList.end(); ++iter )
		{
			ToGuiFileXferClient& client = *iter;
			if( ( client.m_Callback == callback )
				&& ( client.m_UserData == userData ) )
			{
				LogMsg( LOG_INFO, "WARNING. Ignoring New m_ToGuiActivityClient because already in list\n" );
				toGuiFileXferClientsUnlock();
				return;
			}
		}

		ToGuiFileXferClient newClient( callback, userData );
		m_ToGuiFileXferClientList.push_back( newClient );
		toGuiFileXferClientsUnlock();
		return;
	}

	std::vector<ToGuiFileXferClient>::iterator iter;
	for( iter = m_ToGuiFileXferClientList.begin(); iter != m_ToGuiFileXferClientList.end(); ++iter )
	{
		ToGuiFileXferClient& client = *iter;
		if( ( client.m_Callback == callback )
			&& ( client.m_UserData == userData ) )
		{
			m_ToGuiFileXferClientList.erase( iter );
			toGuiFileXferClientsUnlock();
			return;
		}
	}

	LogMsg( LOG_INFO, "WARNING. ToGuiFileXferClient remove not found in list\n" );
	toGuiFileXferClientsUnlock();
	return;
}

//============================================================================
void AppCommon::toGuiHardwareCtrlLock( void )
{
	if( VxIsAppShuttingDown() )
	{
		return;
	}

	m_ToGuiHardwareCtrlMutex.lock();
}

//============================================================================
void AppCommon::toGuiHardwareCtrlUnlock( void )
{
	m_ToGuiHardwareCtrlMutex.unlock();
}

//============================================================================
void AppCommon::wantToGuiHardwareCtrlCallbacks(	ToGuiHardwareControlInterface *	callback, 
												bool							wantCallback )
{
static bool actCallbackShutdownComplete = false;
	if( VxIsAppShuttingDown() )
	{
		if( actCallbackShutdownComplete )
		{
			return;
		}

		actCallbackShutdownComplete = true;
		clearHardwareCtrlList();
		return;
	}

	toGuiHardwareCtrlLock();

	if( wantCallback )
	{
		for( auto iter = m_ToGuiHardwareCtrlList.begin(); iter != m_ToGuiHardwareCtrlList.end(); ++iter )
		{
			ToGuiHardwareCtrlClient& client = *iter;
			if( client.m_Callback == callback )
			{
				LogMsg( LOG_INFO, "WARNING. Ignoring New wantToGuiHardwareCtrlCallbacks because already in list\n" );
				toGuiHardwareCtrlUnlock();
				return;
			}
		}

		ToGuiHardwareCtrlClient newClient( callback, 0 );
		m_ToGuiHardwareCtrlList.push_back( newClient );
		toGuiHardwareCtrlUnlock();
		return;
	}

	for( auto iter = m_ToGuiHardwareCtrlList.begin(); iter != m_ToGuiHardwareCtrlList.end(); ++iter )
	{
		ToGuiHardwareCtrlClient& client = *iter;
		if( client.m_Callback == callback )
		{
			m_ToGuiHardwareCtrlList.erase( iter );
			toGuiHardwareCtrlUnlock();
			return;
		}
	}

	LogMsg( LOG_INFO, "WARNING. ToGuiHardwareCtrlClient remove not found in list\n" );
	toGuiHardwareCtrlUnlock();
	return;
}

//============================================================================
void AppCommon::clearHardwareCtrlList( void )
{
	if( m_ToGuiHardwareCtrlList.size() )
	{
		toGuiHardwareCtrlLock();
		m_ToGuiHardwareCtrlList.clear();
		toGuiHardwareCtrlUnlock();
	}
}

//============================================================================
void AppCommon::toGuiUserUpdateClientsLock( void )
{
    if( VxIsAppShuttingDown() )
    {
        return;
    }

    m_ToGuiUserUpdateClientMutex.lock();
}

//============================================================================
void AppCommon::toGuiUserUpdateClientsUnlock( void )
{
    m_ToGuiUserUpdateClientMutex.unlock();
}

//============================================================================
void AppCommon::wantToGuiUserUpdateCallbacks( ToGuiUserUpdateInterface * callback, bool wantCallback )
{
    static bool userCallbackShutdownComplete = false;
    if( VxIsAppShuttingDown() )
    {
        if( userCallbackShutdownComplete )
        {
            return;
        }

        userCallbackShutdownComplete = true;
        clearUserUpdateClientList();
        return;
    }

    toGuiUserUpdateClientsLock();

    if( wantCallback )
    {
        for( auto iter = m_ToGuiUserUpdateClientList.begin(); iter != m_ToGuiUserUpdateClientList.end(); ++iter )
        {
            ToGuiUserUpdateClient& client = *iter;
            if( client.m_Callback == callback )
            {
                LogMsg( LOG_INFO, "WARNING. Ignoring New wantToGuiUserUpdateCallbacks because already in list" );
                toGuiUserUpdateClientsUnlock();
                return;
            }
        }

        ToGuiUserUpdateClient newClient( callback );
        m_ToGuiUserUpdateClientList.push_back( newClient );
        toGuiUserUpdateClientsUnlock();
        return;
    }

    for( auto iter = m_ToGuiUserUpdateClientList.begin(); iter != m_ToGuiUserUpdateClientList.end(); ++iter )
    {
        ToGuiUserUpdateClient& client = *iter;
        if( client.m_Callback == callback )
        {
            m_ToGuiUserUpdateClientList.erase( iter );
            toGuiUserUpdateClientsUnlock();
            return;
        }
    }

    LogMsg( LOG_INFO, "WARNING. ToGuiUserUpdateClient remove not found in list" );
    toGuiUserUpdateClientsUnlock();
    return;
}

//============================================================================
void AppCommon::clearUserUpdateClientList( void )
{
    if( m_ToGuiUserUpdateClientList.size() )
    {
        toGuiUserUpdateClientsLock();
        m_ToGuiUserUpdateClientList.clear();
        toGuiUserUpdateClientsUnlock();
    }
}

//============================================================================
void AppCommon::refreshFriend( VxGUID& onlineId )
{
	if( VxIsAppShuttingDown() )
	{
		return;
	}

	VxGUID friendId( onlineId.getVxGUIDHiPart(), onlineId.getVxGUIDLoPart() );
	emit signalRefreshFriend( friendId );
}

//============================================================================
void AppCommon::onMessengerReady( bool isReady )
{
	if( isReady != m_IsMessengerReady )
	{
		m_IsMessengerReady = isReady;
		m_ThumbMgr.onMessengerReady( isReady );
		m_UserMgr.onMessengerReady( isReady );
		m_OfferClientMgr.onMessengerReady( isReady );
		m_OfferHostMgr.onMessengerReady( isReady );
		m_HostJoinMgr.onMessengerReady( isReady );
		m_UserJoinMgr.onMessengerReady( isReady );
		m_ConnectMgr.onMessengerReady( isReady );
		checkSystemReady();
	}
}

//============================================================================
void AppCommon::onUserLoggedOn( void )
{
	if( !m_IsLoggedOn )
	{
		m_IsLoggedOn = true;
		checkSystemReady();
	}
}

//============================================================================
void AppCommon::checkSystemReady( void )
{
	if( !m_IsSystemReady && m_IsMessengerReady && m_IsLoggedOn )
	{
		// one time only each application and user ready at startup
		m_IsSystemReady = true;
		
		m_ThumbMgr.onSystemReady( m_IsSystemReady );
		m_UserMgr.onSystemReady( m_IsSystemReady );
		m_OfferClientMgr.onSystemReady( m_IsSystemReady );
		m_OfferHostMgr.onSystemReady( m_IsSystemReady );
		m_HostJoinMgr.onSystemReady( m_IsSystemReady );
		m_UserJoinMgr.onSystemReady( m_IsSystemReady );
		m_ConnectMgr.onSystemReady( m_IsSystemReady );

		emit signalSystemReady( m_IsSystemReady );
	}
}

//============================================================================
void  AppCommon::registerMetaData(void)
{
	qRegisterMetaType<EAppErr>("EAppErr");
	qRegisterMetaType<EApplet>("EApplet");
	qRegisterMetaType<EAssetAction>("EAssetAction");
	qRegisterMetaType<EAssetType>("EAssetType");
	qRegisterMetaType<EConnectReason>("EConnectReason");
	qRegisterMetaType<EConnectStatus>("EConnectStatus");
	qRegisterMetaType<EContentCatagory>("EContentCatagory");
	qRegisterMetaType<EContentRating>("EContentRating");
	qRegisterMetaType<EFileFilterType>("EFileFilterType");
	qRegisterMetaType<EFriendState>("EFriendState");
	qRegisterMetaType<EFriendViewType>("EFriendViewType");
	qRegisterMetaType<EGenderType>("EGenderType");
	qRegisterMetaType<EHostAnnounceStatus>("EHostAnnounceStatus");
	qRegisterMetaType<EHostJoinStatus>("EHostJoinStatus");
	qRegisterMetaType<EHostSearchStatus>("EHostSearchStatus");
	qRegisterMetaType<EHostServiceType>("EHostServiceType");
	qRegisterMetaType<EHostTestStatus>("EHostTestStatus");
	qRegisterMetaType<EHostType>("EHostType");
	qRegisterMetaType<EInternetStatus>("EInternetStatus");
	qRegisterMetaType<EIsPortOpenStatus>("EIsPortOpenStatus");
	qRegisterMetaType<ERunTestStatus>("ERunTestStatus");
	qRegisterMetaType<EMyRelayStatus>("EMyRelayStatus");
	qRegisterMetaType<ENetAvailStatus>("ENetAvailStatus");
	qRegisterMetaType<ENetworkStateType>("ENetworkStateType");
	qRegisterMetaType<EOfferResponse>("EOfferResponse");
	qRegisterMetaType<EOfferState>("EOfferState");
	qRegisterMetaType<EOfferType>("EOfferType");
	qRegisterMetaType<EPluginAccess>("EPluginAccess");
	qRegisterMetaType<EPluginType>("EPluginType");
	qRegisterMetaType<ERandomConnectStatus>("ERandomConnectStatus");
	qRegisterMetaType<EMSessionAction>("EMSessionAction");
	qRegisterMetaType<ESndDef>("ESndDef");
	qRegisterMetaType<EXferError>("EXferError");
	qRegisterMetaType<EXferState>("EXferState");
	qRegisterMetaType<PluginSetting>("PluginSetting");
	qRegisterMetaType<VxGUID>("VxGUID");
	qRegisterMetaType<VxNetIdent>("VxNetIdent");
	qRegisterMetaType<uint32_t>("uint32_t");
}
