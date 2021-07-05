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
#include "AppletLog.h"
#include "AppCommon.h"
#include "AppSettings.h"
#include "GuiHelpers.h"
#include "GuiParams.h"
#include "MyIcons.h"

#include <ptop_src/ptop_engine_src/P2PEngine/P2PEngine.h>
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
AppletLog::AppletLog( AppCommon& app, QWidget * parent )
: AppletBase( OBJNAME_APPLET_LOG, app, parent )
{
    setAppletType( eAppletLog );
    ui.setupUi( getContentItemsFrame() );
	setTitleBarText( DescribeApplet( m_EAppletType ) );

	m_MyApp.activityStateChange( this, true );

    setupApplet();
    VxAddLogHandler( this );
}

//============================================================================
AppletLog::~AppletLog()
{
    VxRemoveLogHandler( this );

    m_MyApp.activityStateChange( this, false );
}

//============================================================================
void AppletLog::setupApplet( void )
{
    getLogEdit()->setMaximumBlockCount( MAX_LOG_EDIT_BLOCK_CNT );
    getLogEdit()->setReadOnly( true );

    connect( ui.gotoWebsiteButton, SIGNAL( clicked() ), this, SLOT( gotoWebsite() ) );
    connect( ui.m_CopyToClipboardButton, SIGNAL( clicked() ), this, SLOT( slotCopyToClipboardClicked() ) );
    connect( ui.m_LogSettingsButton, SIGNAL( clicked() ), this, SLOT( slotLogSettingButtonClick() ) );

    connect( this, SIGNAL( signalLogMsg( const QString& ) ), this, SLOT( slotLogMsg( const QString& ) ) );
    connect( this, SIGNAL( signalInfoMsg( const QString& ) ), this, SLOT( slotInfoMsg( const QString& ) ) );

    fillBasicInfo();
}

//============================================================================
void AppletLog::onLogEvent( uint32_t u32LogFlags, const char * logMsg )
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
void AppletLog::slotLogMsg( const QString& text )
{
    getLogEdit()->appendPlainText( text ); // Adds the message to the widget
    getLogEdit()->verticalScrollBar()->setValue( getLogEdit()->verticalScrollBar()->maximum() ); // Scrolls to the bottom
    //m_LogFile.write( text ); // Logs to file
}

//============================================================================
void  AppletLog::gotoWebsite( void )
{
    QDesktopServices::openUrl( QUrl( VxGetCompanyWebsite() ) );
}

//============================================================================
void AppletLog::slotLogSettingButtonClick( void )
{
    m_MyApp.launchApplet( eAppletLogSettings, getContentFrameOfMessengerFrame() );
}

//============================================================================
void AppletLog::slotCopyToClipboardClicked( void )
{
    QClipboard * clipboard = QApplication::clipboard();
    clipboard->setText( getLogEdit()->toPlainText() );
}

//============================================================================
void AppletLog::logMsg( const char* logMsg, ... )
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
void AppletLog::fillBasicInfo( void )
{
    logMsg( "app: %s version %s", VxGetApplicationTitle(), VxGetAppVersionString() );
    std::string strExePathAndFileName;
    if( 0 == VxFileUtil::getExecuteFullPathAndName( strExePathAndFileName ) )
    {
        logMsg( "app exe: %s", strExePathAndFileName.c_str() );
    }

    logMsg( "directories:" );
    logMsg( "app: %s", VxGetAppDirectory( eAppDirAppExe ).c_str() );
    logMsg( "storage: %s", VxGetAppDirectory( eAppDirRootDataStorage ).c_str() );
    logMsg( "user: %s", VxGetAppDirectory( eAppDirUserSpecific ).c_str() );
    logMsg( "kodi: %s", VxGetAppDirectory( eAppDirExeKodiAssets ).c_str() );
    logMsg( "python exe: %s", VxGetAppDirectory( eAppDirExePython ).c_str() );
    logMsg( "python dlls: %s", VxGetAppDirectory( eAppDirExePythonDlls ).c_str() );
    logMsg( "python libs: %s", VxGetAppDirectory( eAppDirExePythonLibs ).c_str() );
}
