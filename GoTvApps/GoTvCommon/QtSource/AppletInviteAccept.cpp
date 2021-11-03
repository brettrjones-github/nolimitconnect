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
#include "AppletInviteAccept.h"
#include "AppCommon.h"
#include "AppSettings.h"
#include "GuiHelpers.h"
#include "GuiParams.h"
#include "MyIcons.h"

#include <ptop_src/ptop_engine_src/P2PEngine/P2PEngine.h>
#include <CoreLib/IsBigEndianCpu.h>
#include <CoreLib/VxGlobals.h>
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
    const int MAX_INFO_MSG_SIZE = 2048;
}

//============================================================================
AppletInviteAccept::AppletInviteAccept( AppCommon& app, QWidget * parent )
: AppletBase( OBJNAME_APPLET_INVITE_ACCEPT, app, parent )
{
    setAppletType( eAppletInviteAccept );
    ui.setupUi( getContentItemsFrame() );
	setTitleBarText( DescribeApplet( m_EAppletType ) );

    setupApplet();
    // Log is seperate now VxAddLogHandler( this );
    m_MyApp.activityStateChange( this, true );
}

//============================================================================
AppletInviteAccept::~AppletInviteAccept()
{
    // Log is seperate now VxRemoveLogHandler( this );
    m_MyApp.activityStateChange( this, false );
}

//============================================================================
void AppletInviteAccept::setupApplet( void )
{
    VxNetIdent oMyIdent;
    m_FromGui.fromGuiQueryMyIdent( &oMyIdent );
    ui.m_MyUrlEdit->setText( QString( oMyIdent.getMyOnlineUrl().c_str() ) );

    std::string strValue;
    m_MyApp.getAppSettings().getLastUsedTestUrl( strValue );
    if( !strValue.empty() )
    {

        ui.m_TestUrlEdit->setText( strValue.c_str() );
    }

    /*
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

    connect( this, SIGNAL( signalLogMsg( const QString& ) ), this, SLOT( slotInfoMsg( const QString& ) ) );
    connect( this, SIGNAL( signalInfoMsg( const QString& ) ), this, SLOT( slotInfoMsg( const QString& ) ) );

    connect( ui.m_TestUrlsComboBox, SIGNAL(currentText(const QString &)),	this,	SLOT(slotNewUrlSelected(const QString &)) );
    connect( &m_MyApp, SIGNAL(signalRunTestStatus( QString,ERunTestStatus,QString )),
            this, SLOT(slotRunTestStatus( QString,ERunTestStatus,QString )) );

    updateDlgFromSettings();
    */
}
/*
//============================================================================
void AppletInviteAccept::slotPingTestButtonClicked( void )
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
void AppletInviteAccept::slotIsMyPortOpenButtonClicked( void )
{
    startUrlTest( eNetCmdIsMyPortOpenReq );
}

//============================================================================
void AppletInviteAccept::slotQueryHostIdButtonClicked( void )
{
    startUrlTest( eNetCmdQueryHostOnlineIdReq );
}

//============================================================================
void AppletInviteAccept::startUrlTest( ENetCmdType netCmdType )
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
void AppletInviteAccept::slotBrowseFilesButtonClicked( void )
{
    m_MyApp.launchApplet( eAppletBrowseFiles, getContentFrameOfOppositePageFrame() );
}

//============================================================================
void AppletInviteAccept::onLogEvent( uint32_t u32LogFlags, const char * logMsg )
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
*/

//============================================================================
void AppletInviteAccept::toGuiInfoMsg( char * infoMsg )
{

    QString infoStr( infoMsg );
#if QT_VERSION > QT_VERSION_CHECK(6,0,0)
    infoStr.remove(QRegularExpression("[\\n\\r]"));
#else
    infoStr.remove(QRegExp("[\\n\\r]"));
#endif // QT_VERSION > QT_VERSION_CHECK(6,0,0)
    emit signalInfoMsg( infoStr );
}

//============================================================================
void  AppletInviteAccept::gotoWebsite( void )
{
    QDesktopServices::openUrl( QUrl( VxGetCompanyWebsite() ) );
}

//============================================================================
void AppletInviteAccept::slotCopyMyUrlToClipboardClicked( void )
{
    QClipboard * clipboard = QApplication::clipboard();
    clipboard->setText( ui.m_MyUrlEdit->text() );
}

//============================================================================
void AppletInviteAccept::slotCopyTestUrlToClipboardClicked( void )
{
    QClipboard * clipboard = QApplication::clipboard();
    clipboard->setText( ui.m_TestUrlEdit->text() );
}

//============================================================================
void AppletInviteAccept::slotCopyResultToClipboardClicked( void )
{
    QClipboard * clipboard = QApplication::clipboard();
    clipboard->setText( getInfoEdit()->toPlainText() );
}

//============================================================================
void AppletInviteAccept::infoMsg( const char* errMsg, ... )
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
void AppletInviteAccept::fillBasicInfo( void )
{
    infoMsg( "website: %s", VxGetCompanyWebsite() );
    infoMsg( "app: %s version %s", VxGetApplicationTitle(), VxGetAppVersionString() );
    std::string strExePathAndFileName;
    if( 0 == VxFileUtil::getExecuteFullPathAndName( strExePathAndFileName ) )
    {
        infoMsg( "app exe: %s", strExePathAndFileName.c_str() );
    }

    infoMsg( "directories:" );
    infoMsg( "app: %s", VxGetAppDirectory( eAppDirAppExe ).c_str() );
    infoMsg( "storage: %s", VxGetAppDirectory( eAppDirRootDataStorage ).c_str() );
    infoMsg( "user: %s", VxGetAppDirectory( eAppDirUserSpecific ).c_str() );
    infoMsg( "kodi: %s", VxGetAppDirectory( eAppDirExeKodiAssets ).c_str() );
    infoMsg( "python exe: %s", VxGetAppDirectory( eAppDirExePython ).c_str() );
    infoMsg( "python dlls: %s", VxGetAppDirectory( eAppDirExePythonDlls ).c_str() );
    infoMsg( "python libs: %s", VxGetAppDirectory( eAppDirExePythonLibs ).c_str() );
}

//============================================================================
void AppletInviteAccept::fillExtraInfo( void )
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

