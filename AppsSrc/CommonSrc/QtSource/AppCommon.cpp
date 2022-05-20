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

#include "AppletPeerSessionFileOffer.h"

#include "GuiOfferSession.h"

#include "AccountMgr.h"
#include "AppSettings.h"
#include "AppletMgr.h"

#include "FileListReplySession.h"
#include "VxPushButton.h"

#include <ptop_src/ptop_engine_src/P2PEngine/FileShareSettings.h>
#include <ptop_src/ptop_engine_src/P2PEngine/EngineSettings.h>
#include <ptop_src/ptop_engine_src/Membership/Membership.h>
#include <ptop_src/ptop_engine_src/NetworkMonitor/NetworkMonitor.h>
#include <ptop_src/ptop_engine_src/ThumbMgr/ThumbInfo.h>

#include <PktLib/VxCommon.h>
#include <PktLib/ConnectId.h>

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
			return QObject::tr( "No Limit Connect" );
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
			return "NoLimitPlayer";
		case eAppModeGoTvProvider:
			return "NoLimitProvider";
		case eAppModeGoTvStation:
			return "NoLimitStation";
		case eAppModeGoTvNetworkHost:
			return "NoLimitNetHost";
		case eAppModeUnknown:
		default:
			return "NoLimitUnknownApp";
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
, m_ConnectIdListMgr( *this )
, m_ThumbMgr( *this )
, m_MembershipAvailableMgr( *this )
, m_OfferClientMgr( *this )
, m_OfferHostMgr( *this )
, m_UserMgr( *this )
, m_GroupieListMgr( *this )
, m_HostedListMgr( *this )
, m_HostJoinMgr( *this )
, m_UserJoinMgr( *this )
, m_WebPageMgr( *this )
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

	connect( this, SIGNAL( signalInternalNetAvailStatus( ENetAvailStatus ) ), this, SLOT( slotInternalNetAvailStatus( ENetAvailStatus ) ), Qt::QueuedConnection );

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
    QString strSettingsDbFileName = VxGetAppNoLimitDataDirectory().c_str() + m_AppShortName + "_settings.db3";
    m_AppSettings.appSettingStartup( strSettingsDbFileName.toUtf8().constData(), m_AppDefaultMode );

    // Now that settings are set up we can enable logging with log settings
    GetLogMgrInstance().startupLogMgr();

    // database of multiple accounts
    // create accounts database appshortname_accounts.db3 in /appshortName/data/
    QString strAccountDbFileName = VxGetAppNoLimitDataDirectory().c_str() + m_AppShortName + "_accounts.db3";
    m_AccountMgr.startupAccountMgr( strAccountDbFileName.toUtf8().constData() );

    // asset database and user specific setting database will be created in sub directory of account login
    // after user has logged into account

    uint64_t loadingMs = GetApplicationAliveMs();
    LogMsg( LOG_DEBUG, "LoadSettings %" PRId64 " ms alive ms %" PRId64 "", loadingMs - startMs, loadingMs );

	if( getAppSettings().getFeatureEnable( eAppFeatureTheme ) )
	{
		getAppTheme().selectTheme( getAppSettings().getLastSelectedTheme() );
	}

    // load icons from resources
    m_MyIcons.myIconsStartup();

    uint64_t iconsMs = GetApplicationAliveMs();
    LogMsg( LOG_DEBUG, "Load Icons %" PRId64 " ms alive ms %" PRId64 "", iconsMs - loadingMs, iconsMs );

	// QT 6.2.0 has broken scroll bars.. TODO fix when 6.2.0 is no longer beta
	if( getAppSettings().getFeatureEnable( eAppFeatureTheme ) )
	{
		getQApplication().setStyle( &m_AppStyle );
	}

    // load sounds to play and sound hardware
    m_MySndMgr.sndMgrStartup();

    uint64_t styleMs = GetApplicationAliveMs();
    LogMsg( LOG_DEBUG, "Setup Style %" PRId64 " ms alive ms %" PRId64 "", styleMs - iconsMs, styleMs );

	// make sure the engine has been created
	int retryCnt = 0;
	while( !GetPtoPEngine().isEngineCreated() )
	{
		retryCnt++;
		if( retryCnt > 10 )
		{
			LogMsg( LOG_FATAL, "Engine failed to be created" );
			break;
		}

		VxSleep( 200 );	
	}

	m_ThumbMgr.onAppCommonCreated();
	m_UserMgr.onAppCommonCreated();
	m_OfferClientMgr.onAppCommonCreated();
	m_OfferHostMgr.onAppCommonCreated();
	m_HostedListMgr.onAppCommonCreated();
	m_HostJoinMgr.onAppCommonCreated();
	m_UserJoinMgr.onAppCommonCreated();
	m_WebPageMgr.onAppCommonCreated();
	m_ConnectIdListMgr.onAppCommonCreated();
	m_GroupieListMgr.onAppCommonCreated();

    m_HomePage.initializeHomePage();
    connect( &m_HomePage, SIGNAL( signalMainWindowResized() ), this, SLOT( slotMainWindowResized() ) );
    m_HomePage.show();

    uint64_t homePageMs = GetApplicationAliveMs();
    LogMsg( LOG_DEBUG, "Initialize Home Page %" PRId64 " ms alive ms %" PRId64 "", homePageMs - styleMs, homePageMs );
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
void AppCommon::switchWindowFocus( QWidget * appIconButton )
{
	m_HomePage.switchWindowFocus( appIconButton );
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
ActivityBase* AppCommon::launchApplet( EApplet applet, QWidget* parent, QString launchParam, VxGUID& assetId )
{
	return m_AppletMgr.launchApplet( applet, parent, launchParam, assetId );
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

	case ePluginTypePushToTalk:
		{
			/*
				if( false == getIsPluginVisible( ePluginTypePushToTalk ) )
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
		ActivityMessageBox errMsgBox( *this, this, LOG_ERROR, "AppCommon::startActivity UNKNOWN plugin type %d", ePluginType );
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
	case ePluginTypePersonFileXfer:
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
		ActivityMessageBox errMsgBox( *this, this, LOG_ERROR, "AppCommon::executeActivity UNKNOWN plugin type %d", offer->getPluginType() );
		errMsgBox.exec();
	}
}

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
VxNetIdent* AppCommon::getMyIdentity( void )
{
	return getAppGlobals().getUserIdent();
}

//============================================================================
VxGUID& AppCommon::getMyOnlineId( void )
{
    return getMyIdentity()->getMyOnlineId();
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
		LogMsg( LOG_ERROR, "AppCommon::insertKeystroke 0 keyCode" );
		return;
	}

	QWidget *receiver = QApplication::focusWidget();
	if( 0 == receiver )
	{
		receiver = getQApplication().activeWindow();
	}

	if( 0 == receiver )
	{
		LogMsg( LOG_DEBUG,  "AppCommon::insertKeystroke no reciever" );
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
		StdStringFormat( formatedErr, "#App Error %d", eAppErr );
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
void AppCommon::toGuiPluginMsg( EPluginType pluginType, VxGUID& onlineId, EPluginMsgType msgType, std::string& paramMsg )
{
	if( VxIsAppShuttingDown() )
	{
		return;
	}

	QString paramValue = paramMsg.empty() ? "" : paramMsg.c_str();
	emit signalInternalPluginMessage( pluginType, onlineId, msgType, paramValue );
}

//============================================================================
void AppCommon::toGuiPluginCommError( EPluginType pluginType, VxGUID& onlineId, EPluginMsgType msgType, ECommErr commErr )
{
	if( VxIsAppShuttingDown() )
	{
		return;
	}

	emit signalInternalPluginErrorMsg( pluginType, onlineId, msgType, commErr );
}

//============================================================================
void AppCommon::slotInternalPluginMessage( EPluginType pluginType, VxGUID onlineId, EPluginMsgType msgType, QString paramValue )
{
	for( auto client : m_ToGuiActivityInterfaceList )
	{
		client->toGuiPluginMsg( pluginType, onlineId, msgType, paramValue );
	}
}

//============================================================================
void AppCommon::slotInternalPluginErrorMsg( EPluginType pluginType, VxGUID onlineId, EPluginMsgType msgType, ECommErr commError )
{
	QString commErrDescription = GuiParams::describeCommError( commError );
	for( auto client : m_ToGuiActivityInterfaceList )
	{
		client->toGuiPluginMsg( pluginType, onlineId, msgType, commErrDescription );
	}
}

//============================================================================
/// Send Network available status to GUI for display
void AppCommon::toGuiNetAvailableStatus( ENetAvailStatus netAvailStatus )
{
    if( VxIsAppShuttingDown() )
    {
        return;
    }

    emit signalInternalNetAvailStatus( netAvailStatus );
}

//============================================================================
void AppCommon::slotInternalNetAvailStatus( ENetAvailStatus netAvailStatus )
{
	emit signalNetAvailStatus( netAvailStatus );
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
	case eNetworkStateTypeWaitForRelay:
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
    case eNetworkStateTypeIpChange:
        //ui.notifyButton->setIcon( getMyIcons().getIcon( eMyIconNetworkStateHostFail ) );
        break;
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
        StdStringFormat( formatedMsg, "#%s", hostStatus );
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
void AppCommon::toGuiHostSearchResult( EHostType hostType, VxGUID& sessionId, HostedInfo& hostedInfo )
{
    if( VxIsAppShuttingDown() )
    {
        return;
    }

	getHostedListMgr().toGuiHostSearchResult( hostType, sessionId, hostedInfo );
}

//============================================================================
void AppCommon::toGuiHostSearchComplete( EHostType hostType, VxGUID& sessionId )
{
	if( VxIsAppShuttingDown() )
	{
		return;
	}

	getHostedListMgr().toGuiHostSearchComplete( hostType, sessionId );
}

//============================================================================
void AppCommon::toGuiGroupieSearchStatus( EHostType hostType, VxGUID& sessionId, EHostSearchStatus searchStatus, ECommErr commErr, const char* msg )
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

	emit signalGroupieSearchStatus( hostType, sessionId, searchStatus, hostStatus );
}

//============================================================================
void AppCommon::toGuiGroupieSearchResult( EHostType hostType, VxGUID& sessionId, GroupieInfo& groupieInfo )
{
	if( VxIsAppShuttingDown() )
	{
		return;
	}

	getGroupieListMgr().toGuiGroupieSearchResult( hostType, sessionId, groupieInfo );
}

//============================================================================
void AppCommon::toGuiGroupieSearchComplete( EHostType hostType, VxGUID& sessionId )
{
	if( VxIsAppShuttingDown() )
	{
		return;
	}

	getGroupieListMgr().toGuiGroupieSearchComplete( hostType, sessionId );
}

//============================================================================
void AppCommon::toGuiUserOnlineStatus( VxNetIdent* netIdent, bool isOnline )
{
    if( VxIsAppShuttingDown() )
    {
        return;
    }

	if( isOnline )
	{
		emit signalInternalToGuiContactOnline( *netIdent );
	}
	else
	{
		emit signalInternalToGuiContactOffline( netIdent->getMyOnlineId() );
	}
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
            getEngine().fromGuiOncePerSecond();
        }
    }
    else
    {
        waitCnt++;
        LogMsg( LOG_DEBUG, "Wait to login seconds %d alive ms %" PRId64 "", waitCnt, GetApplicationAliveMs() );
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
void AppCommon::toGuiSetGameValueVar( EPluginType ePluginType, VxGUID& onlineId, int32_t s32VarId, int32_t s32VarValue )
{
	if( VxIsAppShuttingDown() )
	{
		return;
	}

	emit signalInternalToGuiSetGameValueVar( ePluginType, onlineId, s32VarId, s32VarValue );
}

//============================================================================
void AppCommon::slotInternalToGuiSetGameValueVar( EPluginType ePluginType, VxGUID onlineId, int32_t s32VarId, int32_t s32VarValue )
{
	if( VxIsAppShuttingDown() )
	{
		return;
	}

	for( auto iter = m_ToGuiActivityInterfaceList.begin(); iter != m_ToGuiActivityInterfaceList.end(); ++iter )
	{
		ToGuiActivityInterface* client = *iter;
		client->toGuiSetGameValueVar( ePluginType, onlineId, s32VarId, s32VarValue );
	}
}

//============================================================================
void AppCommon::toGuiSetGameActionVar(	EPluginType	ePluginType, 
										VxGUID&		onlineId, 
										int32_t		s32VarId, 
										int32_t		s32VarValue )
{
	if( VxIsAppShuttingDown() )
	{
		return;
	}

	emit signalInternalToGuiSetGameActionVar( ePluginType, onlineId, s32VarId, s32VarValue );
}

//============================================================================
void AppCommon::slotInternalToGuiSetGameActionVar( EPluginType ePluginType, VxGUID onlineId, int32_t s32VarId, int32_t s32VarValue )
{
	if( VxIsAppShuttingDown() )
	{
		return;
	}

	for( auto iter = m_ToGuiActivityInterfaceList.begin(); iter != m_ToGuiActivityInterfaceList.end(); ++iter )
	{
		ToGuiActivityInterface* client = *iter;
		client->toGuiSetGameActionVar( ePluginType, onlineId, s32VarId, s32VarValue );
	}
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
void AppCommon::toGuiAssetAdded( AssetBaseInfo* assetInfo )
{
	if( VxIsAppShuttingDown() )
	{
		return;
	}

	emit slotInternalToGuiAssetAdded( *assetInfo );
}

//============================================================================
void AppCommon::slotInternalToGuiAssetAdded( AssetBaseInfo assetInfo )
{
	for( auto iter = m_ToGuiActivityInterfaceList.begin(); iter != m_ToGuiActivityInterfaceList.end(); ++iter )
	{
		ToGuiActivityInterface* client = *iter;
		client->toGuiAssetAdded( assetInfo );
	}
}

//============================================================================
void AppCommon::toGuiAssetSessionHistory( AssetBaseInfo* assetInfo )
{
	if( VxIsAppShuttingDown() )
	{
		return;
	}

	emit slotInternalToGuiAssetSessionHistory( *assetInfo );
}

//============================================================================
void AppCommon::slotInternalToGuiAssetSessionHistory( AssetBaseInfo assetInfo )
{
	if( VxIsAppShuttingDown() )
	{
		return;
	}

	for( auto iter = m_ToGuiActivityInterfaceList.begin(); iter != m_ToGuiActivityInterfaceList.end(); ++iter )
	{
		ToGuiActivityInterface* client = *iter;
		client->toGuiAssetSessionHistory( assetInfo );
	}
}

//============================================================================
void AppCommon::toGuiAssetAction( EAssetAction assetAction, VxGUID& assetId, int pos0to100000 )
{
	if( VxIsAppShuttingDown() )
	{
		return;
	}

	emit signalInternalToGuiAssetAction( assetAction, assetId, pos0to100000 );
}

//============================================================================
void AppCommon::slotInternalToGuiAssetAction( EAssetAction assetAction, VxGUID assetId, int pos0to100000 )
{
    if( ( eAssetActionRxNotifyNewMsg == assetAction )
        || ( eAssetActionRxViewingMsg == assetAction ) )
    {
        emit signalAssetViewMsgAction( assetAction, assetId, pos0to100000 );
        return;
    }

    for( auto iter = m_ToGuiActivityInterfaceList.begin(); iter != m_ToGuiActivityInterfaceList.end(); ++iter )
    {
        ToGuiActivityInterface* client = *iter;
        client->toGuiClientAssetAction( assetAction, assetId, pos0to100000 );
    }
}

//============================================================================
void AppCommon::toGuiMultiSessionAction( EMSessionAction mSessionAction, VxGUID& onlineId, int pos0to100000 )
{
	if( VxIsAppShuttingDown() )
	{
		return;
	}

	VxGUID idPro( onlineId.getVxGUIDHiPart(), onlineId.getVxGUIDLoPart() );
	emit signalInternalMultiSessionAction( idPro, mSessionAction, pos0to100000 );
}

//============================================================================
void AppCommon::slotInternalMultiSessionAction( VxGUID onlineId, EMSessionAction mSessionAction, int pos0to100000 )
{
	if( VxIsAppShuttingDown() )
	{
		return;
	}

	emit signalMultiSessionAction( onlineId, mSessionAction, pos0to100000 );

    for( auto iter = m_ToGuiActivityInterfaceList.begin(); iter != m_ToGuiActivityInterfaceList.end(); ++iter )
    {
        ToGuiActivityInterface* client = *iter;
        client->toGuiMultiSessionAction(  mSessionAction, onlineId, pos0to100000 );
    }
}

//============================================================================
void AppCommon::toGuiBlobAction( EAssetAction assetAction, VxGUID& assetId, int pos0to100000 )
{
	if( VxIsAppShuttingDown() )
	{
		return;
	}

	emit signalInternalBlobAction( assetAction, assetId, pos0to100000 );
}

//============================================================================
void AppCommon::slotInternalBlobAction( EAssetAction assetAction, VxGUID assetId, int pos0to100000 )
{
	if( VxIsAppShuttingDown() )
	{
		return;
	}

	if( ( eAssetActionRxNotifyNewMsg == assetAction )
		|| ( eAssetActionRxViewingMsg == assetAction ) )
	{
		emit signalBlobViewMsgAction( assetAction, assetId, pos0to100000 );
		return;
	}

	for( auto iter = m_ToGuiActivityInterfaceList.begin(); iter != m_ToGuiActivityInterfaceList.end(); ++iter )
	{
		ToGuiActivityInterface* client = *iter;
		client->toGuiClientBlobAction( assetAction, assetId, pos0to100000 );
	}
}

//============================================================================
void AppCommon::toGuiBlobAdded( BlobInfo* blobInfo )
{
	if( VxIsAppShuttingDown() )
	{
		return;
	}

	emit signalInternalBlobAdded( *blobInfo );
}

//============================================================================
void AppCommon::slotInternalBlobAdded( BlobInfo blobInfo )
{
	if( VxIsAppShuttingDown() )
	{
		return;
	}

    for( auto iter = m_ToGuiActivityInterfaceList.begin(); iter != m_ToGuiActivityInterfaceList.end(); ++iter )
    {
        ToGuiActivityInterface* client = *iter;
        client->toGuiBlobAdded( blobInfo );
    }
}

//============================================================================
void AppCommon::toGuiBlobSessionHistory( BlobInfo* blobInfo )
{
	if( VxIsAppShuttingDown() )
	{
		return;
	}

	emit signalInternalBlobSessionHistory( *blobInfo );
}


//============================================================================
void AppCommon::slotInternalBlobSessionHistory( BlobInfo blobInfo )
{
	if( VxIsAppShuttingDown() )
	{
		return;
	}

    for( auto iter = m_ToGuiActivityInterfaceList.begin(); iter != m_ToGuiActivityInterfaceList.end(); ++iter )
    {
        ToGuiActivityInterface* client = *iter;
        client->toGuiBlobSessionHistory( blobInfo );
    }
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
void AppCommon::wantToGuiActivityCallbacks(	ToGuiActivityInterface *	callback, 
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
		clearToGuiActivityInterfaceList();
		return;
	}

	if( wantCallback )
	{
		for( ToGuiActivityInterface* client : m_ToGuiActivityInterfaceList )
		{
			if( client == callback )
			{
				LogMsg( LOG_INFO, "WARNING. Ignoring New ToGuiActivityInterface.h because already in list" );
				return;
			}
		}

		m_ToGuiActivityInterfaceList.push_back( callback );
		return;
	}

	for( auto iter = m_ToGuiActivityInterfaceList.begin(); iter != m_ToGuiActivityInterfaceList.end(); ++iter )
	{
		ToGuiActivityInterface* client = *iter;
		if( client == callback )
		{
			m_ToGuiActivityInterfaceList.erase( iter );
			return;
		}
	}

	LogMsg( LOG_INFO, "WARNING. ToGuiActivityInterface.h remove not found in list" );
	return;
}

//============================================================================
void AppCommon::clearToGuiActivityInterfaceList( void )
{
	if( m_ToGuiActivityInterfaceList.size() )
	{
		m_ToGuiActivityInterfaceList.clear();
	}
}

//============================================================================
void AppCommon::clearFileXferClientList( void )
{
	if( m_ToGuiFileXferInterfaceList.size() )
	{
		m_ToGuiFileXferInterfaceList.clear();
	}
}

//============================================================================
void AppCommon::wantToGuiFileXferCallbacks(	ToGuiFileXferInterface*	    callback, 
											bool						wantCallback )
{
	if( VxIsAppShuttingDown() )
	{
		return;
	}

	if( wantCallback )
	{
		for( ToGuiFileXferInterface* client : m_ToGuiFileXferInterfaceList )
		{
			if( client == callback )
			{
				LogMsg( LOG_INFO, "WARNING. Ignoring New m_ToGuiActivityInterface.h because already in list" );
				return;
			}
		}

		m_ToGuiFileXferInterfaceList.push_back( callback );
		return;
	}

	for( auto iter = m_ToGuiFileXferInterfaceList.begin(); iter != m_ToGuiFileXferInterfaceList.end(); ++iter )
	{
		ToGuiFileXferInterface* client = *iter;
		if( client == callback )
		{
			m_ToGuiFileXferInterfaceList.erase( iter );
			return;
		}
	}

	LogMsg( LOG_INFO, "WARNING. ToGuiFileXferInterface remove not found in list" );
	return;
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

	if( wantCallback )
	{
		for( auto iter = m_ToGuiHardwareCtrlList.begin(); iter != m_ToGuiHardwareCtrlList.end(); ++iter )
		{
			ToGuiHardwareControlInterface* client = *iter;
			if( client == callback )
			{
				LogMsg( LOG_INFO, "WARNING. Ignoring New wantToGuiHardwareCtrlCallbacks because already in list" );
				return;
			}
		}

		m_ToGuiHardwareCtrlList.push_back( callback );
		return;
	}

	for( auto iter = m_ToGuiHardwareCtrlList.begin(); iter != m_ToGuiHardwareCtrlList.end(); ++iter )
	{
		ToGuiHardwareControlInterface* client = *iter;
		if( client == callback )
		{
			m_ToGuiHardwareCtrlList.erase( iter );
			return;
		}
	}

	LogMsg( LOG_INFO, "WARNING. ToGuiHardwareControlInterface remove not found in list" );
	return;
}

//============================================================================
void AppCommon::clearHardwareCtrlList( void )
{
	if( m_ToGuiHardwareCtrlList.size() )
	{
		m_ToGuiHardwareCtrlList.clear();
	}
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

    if( wantCallback )
    {
        for( auto iter = m_ToGuiUserUpdateClientList.begin(); iter != m_ToGuiUserUpdateClientList.end(); ++iter )
        {
            ToGuiUserUpdateInterface* client = *iter;
            if( client == callback )
            {
                LogMsg( LOG_INFO, "WARNING. Ignoring New wantToGuiUserUpdateCallbacks because already in list" );
                return;
            }
        }

        m_ToGuiUserUpdateClientList.push_back( callback );
        return;
    }

    for( auto iter = m_ToGuiUserUpdateClientList.begin(); iter != m_ToGuiUserUpdateClientList.end(); ++iter )
    {
        ToGuiUserUpdateInterface* client = *iter;
        if( client == callback )
        {
            m_ToGuiUserUpdateClientList.erase( iter );
            return;
        }
    }

    LogMsg( LOG_INFO, "WARNING. ToGuiUserUpdateInterface remove not found in list" );
    return;
}

//============================================================================
void AppCommon::clearUserUpdateClientList( void )
{
    if( m_ToGuiUserUpdateClientList.size() )
    {
        m_ToGuiUserUpdateClientList.clear();
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
		m_WebPageMgr.onMessengerReady( isReady );
		m_ConnectIdListMgr.onMessengerReady( isReady );
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
bool AppCommon::checkSystemReady( void )
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
		m_WebPageMgr.onSystemReady( m_IsSystemReady );
		m_ConnectIdListMgr.onSystemReady( m_IsSystemReady );

		emit signalSystemReady( m_IsSystemReady );
	}

	return m_IsSystemReady;
}
