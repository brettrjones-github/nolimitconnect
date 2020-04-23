//============================================================================
// Copyright (C) 2019 Brett R. Jones
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

#include "AppletApplicationInfo.h"
#include "AppCommon.h"
#include "AppSettings.h"
#include "GuiHelpers.h"
#include "GuiParams.h"
#include "MyIcons.h"

#include <GoTvCore/GoTvP2P/P2PEngine/P2PEngine.h>
#include <CoreLib/IsBigEndianCpu.h>
#include <CoreLib/VxGlobals.h>
#include <CoreLib/VxDebug.h>

#include <QDesktopServices>
#include <QUrl>
#include <QClipboard>
#include <QScrollBar>
#include <QApplication>
#include <QDesktopWidget>

namespace
{
    const int MAX_LOG_EDIT_BLOCK_CNT = 1000;
    const int MAX_INFO_MSG_SIZE = 2048;
}

//============================================================================
AppletApplicationInfo::AppletApplicationInfo( AppCommon& app, QWidget * parent )
: AppletBase( OBJNAME_APPLET_APPLICATION_INFO, app, parent )
{
    setAppletType( eAppletApplicationInfo );
    ui.setupUi( getContentItemsFrame() );
	setTitleBarText( DescribeApplet( m_EAppletType ) );

	m_MyApp.activityStateChange( this, true );

    setupApplet();
    VxAddLogHandler( this );
}

//============================================================================
AppletApplicationInfo::~AppletApplicationInfo()
{
    VxRemoveLogHandler( this );

    m_MyApp.activityStateChange( this, false );
}

//============================================================================
void AppletApplicationInfo::setupApplet( void )
{
    getLogEdit()->setMaximumBlockCount( MAX_LOG_EDIT_BLOCK_CNT );
    getLogEdit()->setReadOnly( true );

    connect( ui.gotoWebsiteButton, SIGNAL( clicked() ), this, SLOT( gotoWebsite() ) );
    connect( ui.m_CopyToClipboardButton, SIGNAL( clicked() ), this, SLOT( slotCopyToClipboardClicked() ) );
    connect( ui.m_ExtraInfoButton, SIGNAL( clicked() ), this, SLOT( slotExtraInfoButtonClick() ) );

    connect( this, SIGNAL( signalLogMsg( const QString& ) ), this, SLOT( slotLogMsg( const QString& ) ) );
    connect( this, SIGNAL( signalInfoMsg( const QString& ) ), this, SLOT( slotInfoMsg( const QString& ) ) );

    fillBasicInfo();
    fillExtraInfo();
}

//============================================================================
void AppletApplicationInfo::onLogEvent( uint32_t u32LogFlags, const char * logMsg )
{
    m_LogMutex.lock();
    if( m_VerboseLog
        || ( u32LogFlags & ~LOG_VERBOSE ) )
    {
        QString logStr( logMsg );
        logStr.remove( QRegExp( "[\\n\\r]" ) );
        emit signalLogMsg( logStr );
    }

    m_LogMutex.unlock();
}

//============================================================================
void AppletApplicationInfo::toGuiInfoMsg( char * infoMsg )
{
    m_LogMutex.lock();

    QString infoStr( infoMsg );
    infoStr.remove( QRegExp( "[\\n\\r]" ) );
    emit signalInfoMsg( infoStr );

    m_LogMutex.unlock();
}

//============================================================================
void AppletApplicationInfo::slotLogMsg( const QString& text )
{
    getLogEdit()->appendPlainText( text ); // Adds the message to the widget
    getLogEdit()->verticalScrollBar()->setValue( getLogEdit()->verticalScrollBar()->maximum() ); // Scrolls to the bottom
    //m_LogFile.write( text ); // Logs to file
}

//============================================================================
void AppletApplicationInfo::slotInfoMsg( const QString& text )
{
    getInfoEdit()->appendPlainText( text ); // Adds the message to the widget
    getInfoEdit()->verticalScrollBar()->setValue( getInfoEdit()->verticalScrollBar()->maximum() ); // Scrolls to the bottom
    //m_LogFile.write( text ); // Logs to file
}

//============================================================================
void  AppletApplicationInfo::gotoWebsite( void )
{
    QDesktopServices::openUrl( QUrl( VxGetCompanyWebsite() ) );
}

//============================================================================
void AppletApplicationInfo::slotExtraInfoButtonClick( void )
{

}

//============================================================================
void AppletApplicationInfo::slotCopyToClipboardClicked( void )
{
    QClipboard * clipboard = QApplication::clipboard();
    clipboard->setText( getLogEdit()->toPlainText() );
}

//============================================================================
void AppletApplicationInfo::infoMsg( const char* errMsg, ... )
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
void AppletApplicationInfo::logMsg( const char* logMsg, ... )
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
void AppletApplicationInfo::fillBasicInfo( void )
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
void AppletApplicationInfo::fillExtraInfo( void )
{
    QRect rec = QApplication::desktop()->screenGeometry();
    int screenHeight = rec.height();
    int screenWidth = rec.width();
    int xDpi = QApplication::desktop()->physicalDpiX();
    int yDpi = QApplication::desktop()->physicalDpiY();
    int ratioDpi = QApplication::desktop()->devicePixelRatio();

    infoMsg( "screen size: width %d heigth %d", screenWidth, screenHeight );
    infoMsg( "screen dpi: x %d y %d ratio %d", xDpi, yDpi, ratioDpi );
    infoMsg( "dpi scale: %d", GuiParams().getGuiScale() );
    infoMsg( "thumbnail size: %d", GuiParams().getThumbnailSize().width() );
}

