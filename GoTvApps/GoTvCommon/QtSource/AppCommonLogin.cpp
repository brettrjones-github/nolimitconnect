//============================================================================
// Copyright (C) 2018 Brett R. Jones 
// Issued to MIT style license by Brett R. Jones
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
#include "AppSettings.h"
#include "AppSetup.h"
#include "AppletMgr.h"

#include "ActivityAppSetup.h"
#include "ActivityCreateAccount.h"

#include "GuiHelpers.h"
#include "AccountMgr.h"

#include <GuiInterface/IGoTv.h>
#include <GoTvCore/GoTvP2P/P2PEngine/EngineSettings.h>
#include <CoreLib/VxFileUtil.h>
#include <CoreLib/VxGlobals.h>
#include <CoreLib/VxParse.h>

#include <QUuid>

//============================================================================
static uint64_t getQuuidLoPart( QUuid& uuid )
{
    uint64_t u64LoPart;
    memcpy( &u64LoPart, &uuid, sizeof( uint64_t ) );
    return u64LoPart;
}

//============================================================================
static uint64_t getQuuidHiPart( QUuid& uuid )
{
    uint64_t u64HiPart;
    char * pTmp = (char *)&uuid;
    pTmp += 8;
    memcpy( &u64HiPart, pTmp, sizeof( uint64_t ) );
    return u64HiPart;
}

//============================================================================
// updates my ident in database and engine and global ident
void AppCommon::updateMyIdent( VxNetIdent * myIdent, bool permissionAndStateOnly )
{
    if( myIdent )
    {
        if( myIdent != getAppGlobals().getUserIdent() )
        {
            memcpy( getAppGlobals().getUserIdent(), myIdent, sizeof( VxNetIdent ) );
            myIdent = getAppGlobals().getUserIdent();
        }

       getAccountMgr().updateAccount( *myIdent );
       getEngine().fromGuiUpdateMyIdent( myIdent, permissionAndStateOnly );
    }
}

//============================================================================
void AppCommon::doLogin()
{
    //AppSetup appSetup;
    //if( false == appSetup.areUserAssetsInitilized() )
    //{
    //    m_AppSetupDlg = new ActivityAppSetup( *this, &getHomePage() );
    //    m_AppSetupDlg->show();
    //}
    //else
    //{
        doLoginStep2();
    //}
}

//============================================================================
void AppCommon::slotCheckSetupTimer()
{
    if( m_AppSetupDlg && m_AppSetupDlg->isSetupCompleted() )
    {
        m_AppSetupDlg->close();
        doLoginStep2();
    }
}

//============================================================================
void AppCommon::doLoginStep2()
{
doover:
    bool bLastUserAccountLoaded = loadLastUserAccount();
    if( bLastUserAccountLoaded )
    {
        doAccountStartup();
    }
    else
    {
        applySoundSettings( true );
        // user needs to create login and profile
        m_CreateAccountDlg->setRootUserDataDirectory( m_AppSettings.m_strRootUserDataDir.c_str() );
        
        if( QDialog::Rejected == m_CreateAccountDlg->exec() )
        {
            m_bUserCanceledCreateProfile = true;
            if( QMessageBox::Yes == QMessageBox::question( this, QObject::tr( "Exit Application" ), QObject::tr( "Create account was canceled. Do you want to exit application?" ), QMessageBox::Yes | QMessageBox::No ) )
            {
                QCoreApplication::quit();
                return;
            }
            else
            {
                goto doover;
            }
        }

        if( false == m_CreateAccountDlg->wasLoginNameEntered() )
        {
            goto doover;
        }
        else
        {
            doAccountStartup();
            return;
        }
    }
}

//============================================================================
void AppCommon::doAccountStartup( void )
{
    LogModule( eLogStartup, LOG_DEBUG, "doAccountStartup");
    // tell engine were to load settings from
    loadAccountSpecificSettings( getAppGlobals().getUserIdent()->getOnlineName() );
    uint64_t startMs = GetApplicationAliveMs();

    getAppGlobals().getUserIdent()->setHasSharedWebCam( false ); // user must restart cam server each startup.. assume no shared cam yet

    applySoundSettings();
    LogModule( eLogStartup, LOG_DEBUG, "applySoundSettings" );

    sendAppSettingsToEngine();
    LogModule( eLogStartup, LOG_DEBUG, "sendAppSettingsToEngine" );

    LogModule( eLogStartup, LOG_DEBUG, "startNetworkMonitor" );
    startNetworkMonitor();
    LogModule( eLogStartup, LOG_DEBUG, "after startNetworkMonitor" );

    completeLogin();
    LogModule( eLogStartup, LOG_DEBUG, "completed Login" );

    uint64_t endMs = GetApplicationAliveMs();
    LogMsg( LOG_DEBUG, "Applied settings ms %lld alive ms %lld", endMs - startMs, endMs );
}

//============================================================================
void AppCommon::completeLogin( void )
{
    VxNetIdent * netIdent = getAppGlobals().getUserIdent();
    m_Engine.fromGuiUserLoggedOn( netIdent );
    setLoginCompleted( true );

    m_UserMgr.updateMyIdent( netIdent );

    // get settings from engine
    m_eLastSelectedWhichContactsToView = m_Engine.getEngineSettings().getWhichContactsToView();
    //ui.m_ListViewTypeComboBox->setCurrentIndex( m_eLastSelectedWhichContactsToView );
    //connect( ui.m_ListViewTypeComboBox, SIGNAL(currentIndexChanged(int)),	this,	SLOT(slotListViewTypeChanged(int)) );
    slotListViewTypeChanged( m_eLastSelectedWhichContactsToView );


    //if( netIdent->requiresRelay() && (false == netIdent->hasValidRelay()) )
    //{	
    //	netIdent->setHasRelay( false );
    //	emit signalStatusMsg( "User must select a relay" );
    //	m_Engine.fromGuiSendContactList( eFriendViewAllProxies, 500 );
    //}
    //else
    //{
    //	m_Engine.fromGuiSendContactList( m_eLastSelectedWhichContactsToView, 500 );
    //}

    EApplet lastLaunchedApplet = getAppSettings().getLastAppletLaunched();
    if( eAppletKodi == lastLaunchedApplet )
    {
        m_AppletMgr.launchApplet( lastLaunchedApplet, m_AppletMgr.getAppletFrame( lastLaunchedApplet ) );
    }
}

//============================================================================
//! load last successful user account
bool AppCommon::loadLastUserAccount( void )
{
    m_strAccountUserName = m_AccountMgr.getLastLogin();

    if( 0 != m_strAccountUserName.length() )
    {
        // get identity out of database
        if( true == m_AccountMgr.getAccountByName( m_strAccountUserName.c_str(), *getAppGlobals().getUserIdent() ) )
        {
            return true;
        }
        else
        {
            // remove old missing or corrupted account 
            m_AccountMgr.removeAccountByName( m_strAccountUserName.c_str() );
            LogMsg( LOG_INFO, "AppCommon:Could not retrieve user\n" );
        }
    }

    return false;
}

//============================================================================
void AppCommon::createAccountForUser( std::string& strUserName, VxNetIdent& userAccountIdent, const char * moodMsg, int gender, EAgeType age, int primaryLanguage, int contentType )
{

    QUuid uuidTmp = QUuid::createUuid();
    uint64_t u64HiPart = getQuuidHiPart( uuidTmp );
    uint64_t u64LoPart = getQuuidLoPart( uuidTmp );
    userAccountIdent.m_DirectConnectId.setVxGUID( u64HiPart, u64LoPart );

    if( getAppSettings().m_strUserGuid.length() )
    {
        userAccountIdent.m_DirectConnectId.setVxGUID( getAppSettings().m_strUserGuid.c_str() );
    }

    SafeStrCopy( userAccountIdent.getOnlineName(), strUserName.c_str(), MAX_ONLINE_NAME_LEN );
    SafeStrCopy( userAccountIdent.getOnlineDescription(), moodMsg, MAX_ONLINE_DESC_LEN );
    userAccountIdent.setGender( gender );
    userAccountIdent.setAgeType( age );
    userAccountIdent.setPrimaryLanguage( primaryLanguage );
    userAccountIdent.setPreferredContent( contentType );

    userAccountIdent.setPluginPermissionsToDefaultValues();
    setupAccountResources( userAccountIdent );
}

//============================================================================
void AppCommon::setupAccountResources( VxNetIdent& userAccountIdent )
{
    std::string strUserName = userAccountIdent.getOnlineName();
    m_Engine.fromGuiSetUserXferDir( getUserXferDirectoryFromAccountUserName( strUserName.c_str() ).c_str() );
    // gotv (kodi) also needs the directory
    getGoTv().fromGuiSetUserSpecificDir( getUserSpecificDataDirectoryFromAccountUserName( strUserName.c_str() ).c_str() );

    // get port to listen on 
    uint16_t tcpPort = m_Engine.getEngineSettings().getTcpIpPort();
    userAccountIdent.m_DirectConnectId.setPort( tcpPort );

    // get current default ips
    userAccountIdent.m_DirectConnectId.m_IPv4OnlineIp.setIp( getEngine().fromGuiGetMyIPv4Address().getIPv4AddressInHostOrder(), true );

    userAccountIdent.m_DirectConnectId.m_IPv6OnlineIp = getEngine().fromGuiGetMyIPv6Address();
    std::string myIPv4 = userAccountIdent.m_DirectConnectId.m_IPv4OnlineIp.toStdString();
    std::string myIPv6 = userAccountIdent.m_DirectConnectId.m_IPv6OnlineIp.toStdString();

    LogMsg( LOG_VERBOSE, "Account %s IPv4 %s IPv6 %s\n", strUserName.c_str(), myIPv4.c_str(), myIPv6.c_str() );

    std::string storyBoardFileName = VxGetUserProfileDirectory();
    storyBoardFileName += "story_board.htm";
    if( false == VxFileUtil::fileExists( storyBoardFileName.c_str() ) )
    {
        GuiHelpers::copyResourceToOnDiskFile( ":/AppRes/Resources/story_board.htm", storyBoardFileName.c_str() );
    }

    std::string storyBoardBackground = VxGetUserProfileDirectory();
    storyBoardBackground += "storyboard_background.png";
    if( false == VxFileUtil::fileExists( storyBoardBackground.c_str() ) )
    {
        GuiHelpers::copyResourceToOnDiskFile( ":/AppRes/Resources/storyboard_background.png", storyBoardBackground.c_str() );
    }

    getEngine().getNetStatusAccum().setIpPort( tcpPort );
}

//============================================================================
std::string AppCommon::getUserXferDirectoryFromAccountUserName( const char * userName )
{
    std::string strUserDownloadDir = VxGetRootXferDirectory();
    strUserDownloadDir += userName;
    strUserDownloadDir += "/";
    VxFileUtil::makeDirectory( strUserDownloadDir );
    return strUserDownloadDir;
}

//============================================================================
std::string AppCommon::getUserSpecificDataDirectoryFromAccountUserName( const char * userName )
{
    std::string strUserSpecificDir = VxGetRootUserDataDirectory();
    strUserSpecificDir += "accounts/";
    VxFileUtil::makeDirectory( strUserSpecificDir );

    strUserSpecificDir += userName;
    strUserSpecificDir += "/";
    VxFileUtil::makeDirectory( strUserSpecificDir );
    return strUserSpecificDir;
}

//============================================================================
void AppCommon::loadAccountSpecificSettings( const char * userName )
{
    uint64_t loadStartMs = GetApplicationAliveMs();

    m_Engine.fromGuiSetUserXferDir( getUserXferDirectoryFromAccountUserName( userName ).c_str() );
    std::string strUserSpecificDir = getUserSpecificDataDirectoryFromAccountUserName( userName );
    // gotv (kodi) also needs the directory
    getGoTv().fromGuiSetUserSpecificDir( strUserSpecificDir.c_str() );
    std::string userPicture = VxGetUserProfileDirectory();
    userPicture += "me.jpg";
    if( false == VxFileUtil::fileExists( userPicture.c_str() ) )
    {
        GuiHelpers::copyResourceToOnDiskFile( ":/AppRes/Resources/me.jpg", userPicture.c_str() );
    }

    std::string storyBoardFileName = VxGetUserProfileDirectory();
    storyBoardFileName += "story_board.htm";
    if( false == VxFileUtil::fileExists( storyBoardFileName.c_str() ) )
    {
        GuiHelpers::copyResourceToOnDiskFile( ":/AppRes/Resources/story_board.htm", storyBoardFileName.c_str() );
    }

    std::string storyBoardBackground = VxGetUserProfileDirectory();
    storyBoardBackground += "storyboard_background.png";
    if( false == VxFileUtil::fileExists( storyBoardBackground.c_str() ) )
    {
        GuiHelpers::copyResourceToOnDiskFile( ":/AppRes/Resources/storyboard_background.png", storyBoardBackground.c_str() );
    }

    std::string favIcon = VxGetUserProfileDirectory();
    favIcon += "favicon.ico";
    if( false == VxFileUtil::fileExists( favIcon.c_str() ) )
    {
        GuiHelpers::copyResourceToOnDiskFile( ":/AppRes/Resources/favicon.ico", favIcon.c_str() );
    }

    m_CamSourceId = m_AppSettings.getCamSourceId();
    m_CamCaptureRotation = m_AppSettings.getCamRotation( m_CamSourceId );

    // get port to listen on 
    uint16_t tcpPort = m_Engine.getEngineSettings().getTcpIpPort();
    getEngine().getMyNetIdent()->setMyOnlinePort( tcpPort );
    getEngine().getMyNetIdent()->m_DirectConnectId.m_IPv6OnlineIp = m_Engine.getFromGuiInterface().fromGuiGetMyIPv6Address();
    getEngine().setPktAnnLastModTime( GetTimeStampMs() );

    uint64_t aliveMs = GetApplicationAliveMs();
    LogMsg( LOG_DEBUG, "Account Loaded ms %lld alive ms %lld", aliveMs - loadStartMs, aliveMs );
    setIsAppInitialized( true );
    getEngine().getNetStatusAccum().setIpPort( tcpPort );
}

//============================================================================
void AppCommon::sendAppSettingsToEngine( void )
{
    if( m_AppSettings.m_u32EnableDebug )
    {
        m_Engine.fromGuiDebugSettings(
            m_AppSettings.m_u32LogFlags,
            m_AppSettings.m_strDebugFileName.c_str() );
    }
}

//============================================================================
void AppCommon::slotListViewTypeChanged( int viewSelectedIdx )
{
    m_eLastSelectedWhichContactsToView = (EFriendViewType)viewSelectedIdx;

    //EMyIcons whichIcon = eMyIconUnknown;
    QString statMsg = "Unknown List View";

    switch( m_eLastSelectedWhichContactsToView )
    {
    case eFriendViewEverybody: //Everybody
        //whichIcon = eMyIconGuest;
        statMsg = QObject::tr( "List View Everybody Except Ignored" );
        break;
    case eFriendViewAdministrators:
        //whichIcon = eMyIconAdministrator;
        statMsg = QObject::tr( "List View Administrators" );
        break;
    case eFriendViewFriendsAndGuests: //Friends And Guests
        //whichIcon = eMyIconFriend;
        statMsg = QObject::tr( "List View Friends And Guests" );
        break;
    case eFriendViewAnonymous: //Anonymous
        //whichIcon = eMyIconAnonymous;
        statMsg = QObject::tr( "List View Anonymous" );
        break;
    case eFriendViewIgnored: //Ignored
        //whichIcon = eMyIconIgnored;
        statMsg = QObject::tr( "List View People I Ignore" );
        break;
    case eFriendViewMyProxies: // my proxies
        //whichIcon = eMyIconRelay;
        statMsg = QObject::tr( "List View My Preferred Relays" );
        break;
    case eFriendViewAllProxies: // all proxies
        //whichIcon = eMyIconRelay;
        statMsg = QObject::tr( "List View All Possible Relays" );
        break;
        //case eFriendViewRefresh: // Refresh all
        //	break;
        //default:
        //	LogMsg( LOG_ERROR, "onMenuPeopleSelected: ERROR unknown menu id %d\n", iMenuId );
    default:
        break;
    }

    //ui.m_ListViewTypeButton->setIcon(  getMyIcons().getIcon( whichIcon ) );
    toGuiStatusMessage( statMsg.toUtf8().constData() );
    //ui.mainFriendList->refreshFriendList( m_eLastSelectedWhichContactsToView );
}

//============================================================================
void AppCommon::showUserNameInTitle()
{
    QString strTitle = VxGetApplicationTitle();
    strTitle += "-";
    strTitle += getAccountUserName().c_str();
    setWindowTitle( strTitle );
}
