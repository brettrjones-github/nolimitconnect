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
#include <app_precompiled_hdr.h>
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
        ui.m_TestUrlsComboBox->addItem( strValue.c_str() );
        ui.m_TestUrlEdit->setText( strValue.c_str() );
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

    connect( ui.gotoWebsiteButton, SIGNAL( clicked() ), this, SLOT( gotoWebsite() ) );
    connect( ui.m_ShowLogButton, SIGNAL( clicked() ), this, SLOT( slotShowLogButtonClick() ) );
    connect( ui.m_ShowAppInfoButton, SIGNAL( clicked() ), this, SLOT( slotShowAppInfoButtonClick() ) );
    connect( ui.m_CopyMyUrlButton, SIGNAL( clicked() ), this, SLOT( slotCopyMyUrlToClipboardClicked() ) );
    connect( ui.m_CopyTestUrlButton, SIGNAL( clicked() ), this, SLOT( slotCopyTestUrlToClipboardClicked() ) );
    connect( ui.m_CopyResultToClipboardButton, SIGNAL( clicked() ), this, SLOT( slotCopyResultToClipboardClicked() ) );

    connect( ui.m_BrowseFilesButton, SIGNAL( clicked() ), this, SLOT( slotBrowseFilesButtonClicked() ) );
    connect( ui.m_IsMyPortOpenButton, SIGNAL( clicked() ), this, SLOT( slotIsMyPortOpenButtonClicked() ) );
    connect( ui.m_QueryHostIdButton, SIGNAL( clicked() ), this, SLOT( slotQueryHostIdButtonClicked() ) );
    connect( ui.m_GenerateGuidButton, SIGNAL( clicked() ), this, SLOT( slotGenerateGuidButtonClicked() ) );
    connect( ui.m_PurgeCacheButton, SIGNAL( clicked() ), this, SLOT( slotPurgeThumbnailsButtonClicked() ) );

    connect( this, SIGNAL( signalLogMsg( const QString& ) ), this, SLOT( slotInfoMsg( const QString& ) ) );
    connect( this, SIGNAL( signalInfoMsg( const QString& ) ), this, SLOT( slotInfoMsg( const QString& ) ) );

    connect( ui.m_TestUrlsComboBox, SIGNAL(currentText(const QString &)),	this,	SLOT(slotNewUrlSelected(const QString &)) );
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
void AppletTestAndDebug::slotNewUrlSelected( const QString& newUrl )
{
    if( !newUrl.isEmpty() )
    {
        ui.m_TestUrlEdit->setText( newUrl );
    }
}

//============================================================================
void AppletTestAndDebug::slotPingTestButtonClicked( void )
{
    getInfoEdit()->clear();
    VxUrl myUrl( ui.m_MyUrlEdit->text().toUtf8().constData() );
    VxUrl testUrl( ui.m_TestUrlEdit->text().toUtf8().constData() );

    if( myUrl.validateUrl( true ) && testUrl.validateUrl( false ) )
    {
        infoMsg( "Testing Ping" );
        VxGUID::generateNewVxGUID(m_SessionId);
        m_MyApp.getEngine().fromGuiRunUrlAction( m_SessionId, myUrl.getUrl().c_str(), testUrl.getUrl().c_str(), eNetCmdPing );
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
void AppletTestAndDebug::slotIsMyPortOpenButtonClicked( void )
{
    startUrlTest( eNetCmdIsMyPortOpenReq );
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
    infoMsg( "kodi assets: %s", VxGetAppDirectory( eAppDirExeKodiAssets ).c_str() );
    infoMsg( "nolimit assets: %s", VxGetAppDirectory( eAppDirExeNoLimitAssets ).c_str() );
    infoMsg( "python exe: %s", VxGetAppDirectory( eAppDirExePython ).c_str() );
    infoMsg( "python dlls: %s", VxGetAppDirectory( eAppDirExePythonDlls ).c_str() );
    infoMsg( "python libs: %s", VxGetAppDirectory( eAppDirExePythonLibs ).c_str() );
}

//============================================================================
void AppletTestAndDebug::fillCpuInfo( void )
{
    infoMsg( "C++ value sizes: int %d long %d long long %d int64_t %d", sizeof( int ), sizeof( long ), sizeof( long long ), sizeof( int64_t ) );
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
    infoMsg( "dpi scale: %d", GuiParams().getGuiScale() );
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
    if( yesNoMessageBox( QObject::tr( "Are you sure?" ), QObject::tr( "This will delete all thumbnail images not currently in use by application" ) ) )
    {
        int64_t bytesPurged = m_MyApp.getEngine().fromGuiClearCache( eCacheTypeThumbnail );
        infoMsg( QString( GuiParams::describeFileLength( bytesPurged ) + QObject::tr( " of disk space freed by deleting cached thumbnails" ) ).toUtf8().constData() );
    }
}
