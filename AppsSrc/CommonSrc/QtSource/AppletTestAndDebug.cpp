//============================================================================
// Copyright (C) 2020 Brett R. Jones
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

#include "AppletTestAndDebug.h"
#include "AppCommon.h"
#include "AppSettings.h"
#include "GuiHelpers.h"
#include "GuiParams.h"
#include "MyIcons.h"

#include <ptop_src/ptop_engine_src/P2PEngine/P2PEngine.h>
#include <CoreLib/IsBigEndianCpu.h>
#include <CoreLib/VxGlobals.h>
#include <CoreLib/VxGUID.h>
#include <CoreLib/VxDebug.h>
#include <CoreLib/VxUrl.h>

#include <QDesktopServices>
#include <QUrl>
#include <QClipboard>
#include <QScrollBar>
#include <QApplication>
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
#include <QDesktopWidget>
#endif // QT_VERSION < QT_VERSION_CHECK(6,0,0)

namespace
{
    const int MAX_LOG_EDIT_BLOCK_CNT = 1000;
    const int MAX_INFO_MSG_SIZE = 2048;
}

//============================================================================
AppletTestAndDebug::AppletTestAndDebug( AppCommon& app, QWidget * parent )
: AppletBase( OBJNAME_APPLET_TEST_AND_DEBUG, app, parent )
{
    setAppletType( eAppletTestAndDebug );
    ui.setupUi( getContentItemsFrame() );
	setTitleBarText( DescribeApplet( m_EAppletType ) );

    setupApplet();
    // Log is seperate now VxAddLogHandler( this );
    m_MyApp.activityStateChange( this, true );
}

//============================================================================
AppletTestAndDebug::~AppletTestAndDebug()
{
    // Log is seperate now VxRemoveLogHandler( this );
    m_MyApp.activityStateChange( this, false );
}

//============================================================================
void AppletTestAndDebug::setupApplet( void )
{
    getInfoEdit()->setMaximumBlockCount( MAX_LOG_EDIT_BLOCK_CNT );
    getInfoEdit()->setReadOnly( true );

    VxNetIdent oMyIdent;
    m_FromGui.fromGuiQueryMyIdent( &oMyIdent );
    ui.m_MyUrlEdit->setText( QString( oMyIdent.getMyOnlineUrl().c_str() ) );

    //fillBasicInfo();
    //fillExtraInfo();
    fillCpuInfo();
    std::string strValue;
    m_MyApp.getAppSettings().getLastUsedTestUrl( strValue );
    if( !strValue.empty() )
    {
        ui.m_TestUrlEdit->setText( strValue.c_str() );
        ui.m_TestUrlsComboBox->addItem( strValue.c_str() );
    }

    m_Engine.getEngineSettings().getNetworkHostUrl( strValue );
    if( !strValue.empty() )
    {
        ui.m_TestUrlsComboBox->addItem( strValue.c_str() );
    }

    m_Engine.getEngineSettings().getConnectTestUrl( strValue );
    if( !strValue.empty() )
    {
        ui.m_TestUrlsComboBox->addItem( strValue.c_str() );
    }

    m_Engine.getEngineSettings().getRandomConnectUrl( strValue );
    if( !strValue.empty() )
    {
        ui.m_TestUrlsComboBox->addItem( strValue.c_str() );
    }

    m_Engine.getEngineSettings().getGroupHostUrl( strValue );
    if( !strValue.empty() )
    {
        ui.m_TestUrlsComboBox->addItem( strValue.c_str() );
    }

    m_Engine.getEngineSettings().getChatRoomHostUrl( strValue );
    if( !strValue.empty() )
    {
        ui.m_TestUrlsComboBox->addItem( strValue.c_str() );
    }

    if( !ui.m_TestUrlsComboBox->currentText().isEmpty() )
    {
        if( ui.m_TestUrlEdit->text().isEmpty() )
        {
            ui.m_TestUrlEdit->setText( ui.m_TestUrlsComboBox->currentText() );
        }     
    }

    ui.m_AnnouncedCheckBox->setChecked( true );
    ui.m_HostJoinedCheckBox->setChecked( true );
    ui.m_ClientJoinedCheckBox->setChecked( true );

    connect( ui.gotoWebsiteButton, SIGNAL( clicked() ), this, SLOT( gotoWebsite() ) );
    connect( ui.m_ShowLogButton, SIGNAL( clicked() ), this, SLOT( slotShowLogButtonClick() ) );
    connect( ui.m_ShowAppInfoButton, SIGNAL( clicked() ), this, SLOT( slotShowAppInfoButtonClick() ) );
    connect( ui.m_CopyMyUrlButton, SIGNAL( clicked() ), this, SLOT( slotCopyMyUrlToClipboardClicked() ) );
    connect( ui.m_CopyTestUrlButton, SIGNAL( clicked() ), this, SLOT( slotCopyTestUrlToClipboardClicked() ) );
    connect( ui.m_CopyResultToClipboardButton, SIGNAL( clicked() ), this, SLOT( slotCopyResultToClipboardClicked() ) );
    connect( ui.m_ClearResultsButton, SIGNAL( clicked() ), this, SLOT( slotClearResultsButtonClicked() ) );

    connect( ui.m_BrowseFilesButton, SIGNAL( clicked() ), this, SLOT( slotBrowseFilesButtonClicked() ) );
    connect( ui.m_IsMyPortOpenButton, SIGNAL( clicked() ), this, SLOT( slotIsMyPortOpenButtonClicked() ) );
    connect( ui.m_QueryHostIdButton, SIGNAL( clicked() ), this, SLOT( slotQueryHostIdButtonClicked() ) );
    connect( ui.m_GenerateGuidButton, SIGNAL( clicked() ), this, SLOT( slotGenerateGuidButtonClicked() ) );
    connect( ui.m_PurgeCacheButton, SIGNAL( clicked() ), this, SLOT( slotPurgeCacheButtonClicked() ) );
    connect( ui.m_GenKeyButton, SIGNAL( clicked() ), this, SLOT( slotGenKeyButtonClicked() ) );
    connect( ui.m_ListActionButton, SIGNAL( clicked() ), this, SLOT( slotListActionButtonClicked() ) );
    connect( ui.m_HostClientTestButton, SIGNAL( clicked() ), this, SLOT( slotHostClientTestButtonClicked() ) );
    connect( ui.m_PingRemoteUrlButton, SIGNAL( clicked() ), this, SLOT( slotPingRemoteUrlButtonClicked() ) );

    connect( this, SIGNAL( signalLogMsg( const QString& ) ), this, SLOT( slotInfoMsg( const QString& ) ) );
    connect( this, SIGNAL( signalInfoMsg( const QString& ) ), this, SLOT( slotInfoMsg( const QString& ) ) );

    connect( ui.m_TestUrlsComboBox, SIGNAL( currentIndexChanged( int ) ),	this,	SLOT(slotNewUrlSelected( int )) );
    connect( &m_MyApp, SIGNAL(signalRunTestStatus( QString,ERunTestStatus,QString )),
            this, SLOT(slotRunTestStatus( QString,ERunTestStatus,QString )) );

    updateDlgFromSettings();
}

//============================================================================
void AppletTestAndDebug::updateDlgFromSettings()
{
    std::string lastTestUrl;
    m_MyApp.getAppSettings().getLastUsedTestUrl( lastTestUrl );
    if( !lastTestUrl.empty() )
    {
        ui.m_TestUrlEdit->setText( lastTestUrl.c_str() );
    }
}

//============================================================================
void AppletTestAndDebug::updateSettingsFromDlg()
{
    if( !ui.m_TestUrlEdit->text().isEmpty() )
    {
        std::string testUrl( ui.m_TestUrlEdit->text().toUtf8().constData() );
        m_MyApp.getAppSettings().setLastUsedTestUrl( testUrl );
    }
}

//============================================================================
void AppletTestAndDebug::slotNewUrlSelected( int comboBoxIdx )
{
    QString urlText = ui.m_TestUrlsComboBox->currentText();
    if( !urlText.isEmpty() )
    {
        ui.m_TestUrlEdit->setText( urlText );
    }
}

//============================================================================
void AppletTestAndDebug::slotPingTestButtonClicked( void )
{
    startUrlTest( eNetCmdPing );
}

//============================================================================
void AppletTestAndDebug::slotIsMyPortOpenButtonClicked( void )
{
    getInfoEdit()->clear();
    QString urlText = ui.m_TestUrlsComboBox->currentText();

    VxUrl myUrl( ui.m_MyUrlEdit->text().toUtf8().constData() );
    VxUrl testUrl( urlText.toUtf8().constData() );

    if( testUrl.validateUrl( false ) )
    {
        infoMsg( "Testing %s with url %s", DescribeNetCmdType( eNetCmdIsMyPortOpenReq ), urlText.toUtf8().constData() );
        VxGUID::generateNewVxGUID( m_SessionId );
        m_MyApp.getEngine().fromGuiRunUrlAction( m_SessionId, myUrl.getUrl().c_str(), testUrl.getUrl().c_str(), eNetCmdIsMyPortOpenReq );
    }
    else
    {
        if( !myUrl.validateUrl( true ) )
        {
            infoMsg( "Invalid My URL" );
        }

        if( !testUrl.validateUrl( false ) )
        {
            infoMsg( "Invalid Test URL" );
        }
    }
}

//============================================================================
void AppletTestAndDebug::slotQueryHostIdButtonClicked( void )
{
    startUrlTest( eNetCmdQueryHostOnlineIdReq );
}

//============================================================================
void AppletTestAndDebug::startUrlTest( ENetCmdType netCmdType )
{
    getInfoEdit()->clear();
    VxUrl myUrl( ui.m_MyUrlEdit->text().toUtf8().constData() );
    VxUrl testUrl( ui.m_TestUrlEdit->text().toUtf8().constData() );

    if( myUrl.validateUrl( true ) && testUrl.validateUrl( false ) )
    {
        infoMsg( "Testing %s", DescribeNetCmdType( netCmdType ) );
        VxGUID::generateNewVxGUID(m_SessionId);
        m_MyApp.getEngine().fromGuiRunUrlAction( m_SessionId, myUrl.getUrl().c_str(), testUrl.getUrl().c_str(), netCmdType );
    }
    else
    {
        if( !myUrl.validateUrl( true ) )
        {
            infoMsg( "Invalid My URL" );
        }

        if( !testUrl.validateUrl( false ) )
        {
            infoMsg( "Invalid Test URL" );
        }
    }
}

//============================================================================
void AppletTestAndDebug::slotBrowseFilesButtonClicked( void )
{
    m_MyApp.launchApplet( eAppletBrowseFiles, getContentFrameOfOppositePageFrame() );
}

//============================================================================
void AppletTestAndDebug::onLogEvent( uint32_t u32LogFlags, const char * logMsg )
{
    // Log is seperate now 

    //m_LogMutex.lock();
    //if( m_VerboseLog
    //    || ( u32LogFlags & ~LOG_VERBOSE ) )
    //{
    //    QString logStr( logMsg );
    //    logStr.remove( QRegExp( "[\\n\\r]" ) );
    //    emit signalLogMsg( logStr );
    //}

    //m_LogMutex.unlock();
}

//============================================================================
void AppletTestAndDebug::toGuiInfoMsg( char * infoMsg )
{
    m_LogMutex.lock();

    QString infoStr( infoMsg );
#if QT_VERSION > QT_VERSION_CHECK(6,0,0)
    infoStr.remove(QRegularExpression("[\\n\\r]"));
#else
    infoStr.remove(QRegExp("[\\n\\r]"));
#endif // QT_VERSION > QT_VERSION_CHECK(6,0,0)
    emit signalInfoMsg( infoStr );

    m_LogMutex.unlock();
}

//============================================================================
void AppletTestAndDebug::slotLogMsg( const QString& text )
{
    //getLogEdit()->appendPlainText( text ); // Adds the message to the widget
    //getLogEdit()->verticalScrollBar()->setValue( getLogEdit()->verticalScrollBar()->maximum() ); // Scrolls to the bottom
    //m_LogFile.write( text ); // Logs to file
}

//============================================================================
void AppletTestAndDebug::slotInfoMsg( const QString& text )
{
    getInfoEdit()->appendPlainText( text ); // Adds the message to the widget
    getInfoEdit()->verticalScrollBar()->setValue( getInfoEdit()->verticalScrollBar()->maximum() ); // Scrolls to the bottom
    //m_LogFile.write( text ); // Logs to file
}

//============================================================================
void AppletTestAndDebug::slotRunTestStatus( QString testName, ERunTestStatus testStatus, QString testMsg )
{
    getInfoEdit()->appendPlainText( testMsg ); 
    if( testStatus != eRunTestStatusLogMsg )
    {
        QString testStat = QObject::tr( "Test Status: " );
        testStat += DescribeRunTestStatus( testStatus );
        getInfoEdit()->appendPlainText( testStat );
    }
}

//============================================================================
void  AppletTestAndDebug::gotoWebsite( void )
{
    fillBasicInfo();
    fillExtraInfo();
    fillCpuInfo();

    QDesktopServices::openUrl( QUrl( VxGetCompanyWebsite() ) );
}

//============================================================================
void AppletTestAndDebug::slotShowLogButtonClick( void )
{
    fillBasicInfo();
    fillExtraInfo();
    fillCpuInfo();

    m_MyApp.launchApplet( eAppletLog, getContentFrameOfOppositePageFrame() );
}

//============================================================================
void AppletTestAndDebug::slotShowAppInfoButtonClick( void )
{
    fillBasicInfo();
    fillExtraInfo();
    fillCpuInfo();

    m_MyApp.launchApplet( eAppletApplicationInfo, getContentFrameOfOppositePageFrame() );
}

//============================================================================
void AppletTestAndDebug::slotCopyMyUrlToClipboardClicked( void )
{
    QClipboard * clipboard = QApplication::clipboard();
    clipboard->setText( ui.m_MyUrlEdit->text() );
}

//============================================================================
void AppletTestAndDebug::slotCopyTestUrlToClipboardClicked( void )
{
    QClipboard * clipboard = QApplication::clipboard();
    clipboard->setText( ui.m_TestUrlEdit->text() );
}

//============================================================================
void AppletTestAndDebug::slotCopyResultToClipboardClicked( void )
{
    QClipboard * clipboard = QApplication::clipboard();
    clipboard->setText( getInfoEdit()->toPlainText() );
}

//============================================================================
void AppletTestAndDebug::slotClearResultsButtonClicked( void )
{
    getInfoEdit()->setPlainText( "" );
}

//============================================================================
void AppletTestAndDebug::infoMsg( const char* errMsg, ... )
{
    char as8Buf[ MAX_INFO_MSG_SIZE ];
    va_list argList;
    va_start( argList, errMsg );
    vsnprintf( as8Buf, sizeof( as8Buf ), errMsg, argList );
    as8Buf[ sizeof( as8Buf ) - 1 ] = 0;
    va_end( argList );

    toGuiInfoMsg( as8Buf );
}

//============================================================================
void AppletTestAndDebug::logMsg( const char* logMsg, ... )
{
    char as8Buf[ MAX_INFO_MSG_SIZE ];
    va_list argList;
    va_start( argList, logMsg );
    vsnprintf( as8Buf, sizeof( as8Buf ), logMsg, argList );
    as8Buf[ sizeof( as8Buf ) - 1 ] = 0;
    va_end( argList );

    onLogEvent( LOG_INFO, as8Buf );
}

//============================================================================
void AppletTestAndDebug::fillBasicInfo( void )
{
    infoMsg( "website: %s", VxGetCompanyWebsite() );
    infoMsg( "app: %s version %s", VxGetApplicationTitle(), VxGetAppVersionString() );
    infoMsg( "disk space available: %s", GuiParams::describeFileLength( m_MyApp.getFromGuiInterface().fromGuiGetDiskFreeSpace() ).toUtf8().constData() );

    infoMsg( "directories:" );
    std::string strExePathAndFileName;
    if( 0 == VxFileUtil::getExecuteFullPathAndName( strExePathAndFileName ) )
    {
        infoMsg( "app exe 1: %s", strExePathAndFileName.c_str() );
    }
    infoMsg( "app exe 2: %s", VxGetAppDirectory( eAppDirAppExe ).c_str() );
    infoMsg( "storage: %s", VxGetAppDirectory( eAppDirRootDataStorage ).c_str() );
    infoMsg( "personal records: %s", VxGetAppDirectory( eAppDirPersonalRecords ).c_str() );
    infoMsg( "downloads: %s", VxGetAppDirectory( eAppDirDownloads ).c_str() );
    infoMsg( "user account specific: %s", VxGetAppDirectory( eAppDirUserSpecific ).c_str() );
    infoMsg( "nolimit assets: %s", VxGetAppDirectory( eAppDirExeNoLimitAssets ).c_str() );
    infoMsg( "thumbs: %s", VxGetAppDirectory( eAppDirThumbs ).c_str() );
    infoMsg( "kodi assets: %s", VxGetAppDirectory( eAppDirExeKodiAssets ).c_str() );
    infoMsg( "python exe: %s", VxGetAppDirectory( eAppDirExePython ).c_str() );
    infoMsg( "python dlls: %s", VxGetAppDirectory( eAppDirExePythonDlls ).c_str() );
    infoMsg( "python libs: %s", VxGetAppDirectory( eAppDirExePythonLibs ).c_str() );
}

//============================================================================
void AppletTestAndDebug::fillCpuInfo( void )
{
    infoMsg( "C++ value sizes: int %d long %d long long %d int64_t %d PRId64 %s", sizeof( int ), sizeof( long ), sizeof( long long ), sizeof( int64_t ), PRId64 );
}

//============================================================================
void AppletTestAndDebug::fillExtraInfo( void )
{
#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
    QRect rec = QApplication::primaryScreen()->availableGeometry();
    int screenHeight = rec.height();
    int screenWidth = rec.width();
    int xDpi = QApplication::primaryScreen()->physicalDotsPerInchX();
    int yDpi = QApplication::primaryScreen()->physicalDotsPerInchY();
    int ratioDpi = QApplication::primaryScreen()->devicePixelRatio();
#else
    QRect rec = QApplication::desktop()->screenGeometry();
    int screenHeight = rec.height();
    int screenWidth = rec.width();
    int xDpi = QApplication::desktop()->physicalDpiX();
    int yDpi = QApplication::desktop()->physicalDpiY();
    int ratioDpi = QApplication::desktop()->devicePixelRatio();
#endif // QT_VERSION >= QT_VERSION_CHECK(6,0,0)

    infoMsg( "screen size: width %d heigth %d", screenWidth, screenHeight );
    infoMsg( "screen dpi: x %d y %d ratio %d", xDpi, yDpi, ratioDpi );
    infoMsg( "dpi scale: %3.1f", GuiParams().getGuiScale() );
    infoMsg( "thumbnail size: %d", GuiParams().getThumbnailSize().width() );
}

//============================================================================
void AppletTestAndDebug::slotGenerateGuidButtonClicked( void )
{
    VxGUID guid;
    guid.initializeWithNewVxGUID();
    infoMsg( "{ %llu, %llu }, ", guid.getVxGUIDHiPart(), guid.getVxGUIDLoPart() );
    infoMsg( "%s", guid.toOnlineIdString().c_str() );
}

//============================================================================
void AppletTestAndDebug::slotPurgeCacheButtonClicked( void )
{
    if( yesNoMessageBox( QObject::tr( "Are you sure?" ), QObject::tr( "This will delete all thumbnail images not currently in use by your identity or hosts" ) ) )
    {
        int64_t bytesPurged = m_MyApp.getEngine().fromGuiClearCache( eCacheTypeThumbnail );
        infoMsg( QString( GuiParams::describeFileLength( bytesPurged ) + QObject::tr( " of disk space freed by deleting cached thumbnails" ) ).toUtf8().constData() );
    }
}

//============================================================================
void AppletTestAndDebug::slotGenKeyButtonClicked( void )
{
    std::string keyPwd1( "4512448476bf2836979cf61b7c88f8fc70099NoLimitNet172.94.58.10345124" );
    VxKey key1;
    key1.setKeyFromPassword( keyPwd1.c_str(), keyPwd1.length() );
    infoMsg( "Gen Key 1 pwd %s", keyPwd1.c_str() );
    infoMsg( "Gen Key 1 result %s", key1.describeKey().c_str() );

    std::string keyPwd2 = ui.m_GenKeyPwdEdit->text().toUtf8().constData();
    key1.setKeyFromPassword( keyPwd2.c_str(), keyPwd2.length() );
    infoMsg( "Gen Key 1 pwd %s", keyPwd2.c_str() );
    infoMsg( "Gen Key 1 result %s", key1.describeKey().c_str() );
}

//============================================================================
void AppletTestAndDebug::slotListActionButtonClicked( void )
{
    if( m_ShowListMsg )
    {
        m_ShowListMsg = false;
        okMessageBox( QObject::tr( "List Action" ),
            QObject::tr( "To see list result click Show Log. Also review log setting to make sure logging is enabled" ) );
    }

    if( ui.m_AnnouncedCheckBox->isChecked() )
    {
        getFromGuiInterface().fromGuiListAction( eListActionAnnounced );
    }

    if( ui.m_HostJoinedCheckBox->isChecked() )
    {
        getFromGuiInterface().fromGuiListAction( eListActionJoinedHost );
    }

    if( ui.m_ClientJoinedCheckBox->isChecked() )
    {
        getFromGuiInterface().fromGuiListAction( eListActionJoinedClient );
    }
}

//============================================================================
void AppletTestAndDebug::slotHostClientTestButtonClicked( void )
{
    m_MyApp.launchApplet( eAppletTestHostClient, getContentFrameOfOppositePageFrame() );
}

//============================================================================
void AppletTestAndDebug::slotPingRemoteUrlButtonClicked( void )
{
    startUrlTest( eNetCmdHostPingReq );
}
