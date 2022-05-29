//============================================================================
// Copyright (C) 2020 Brett R. Jones
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

#include "AppletLogSettings.h"

#include "LogMgr.h"
#include "AppGlobals.h"
#include "AppCommon.h"
#include "AppSettings.h"
#include "GuiParams.h"
#include "MyIcons.h"

#include <CoreLib/VxDebug.h>
#include <CoreLib/ObjectCommon.h>

//============================================================================
QString describeLogModule( ELogModule logModule )
{
    switch( logModule )
    {
    case eLogNone: return QObject::tr( "Log None" );

    case eLogMulticast: return QObject::tr( "Log Multicast" );
    case eLogConnect: return QObject::tr( "Log Connect" );
    case eLogListen : return QObject::tr( "Log Listen" );
    case eLogAcceptConn: return QObject::tr( "Log Accept Connection" );

    case eLogSkt: return QObject::tr( "Log Skt" );
    case eLogNetAccessStatus: return QObject::tr( "Log Net Access Status" );
    case eLogNetworkState: return QObject::tr( "Log Network State" );
    case eLogNetworkMgr: return QObject::tr( "Log Network Mgr" );

    case eLogNetService: return QObject::tr( "Log Net Service" );
    case eLogThread : return QObject::tr( "Log Thread" );
    case eLogStorage: return QObject::tr( "Log Storage" );
    case eLogAssets: return QObject::tr( "Log Assets" );

    case eLogStartup: return QObject::tr( "Log Startup" );
    case eLogTcpData: return QObject::tr( "Log Tcp Data" );
    case eLogUdpData: return QObject::tr( "Log Udp Data" );
    case eLogPkt: return QObject::tr( "Log Pkt" );

    case eLogPlugins: return QObject::tr( "Log Plugins" );
    case eLogHosts: return QObject::tr( "Log Hosts" );
    case eLogClients: return QObject::tr( "Log Clients" );
    case eLogGroups: return QObject::tr( "Log Groups" );

    case eLogNetworkRelay: return QObject::tr( "Log Network Relay" );
    case eLogIsPortOpenTest: return QObject::tr( "Log Is Port Open Test" );
    case eLogRunTest: return QObject::tr( "Log Run Test" );
    case eLogHostConnect: return QObject::tr( "Log Host Connect" );

    case eLogHostSearch: return QObject::tr( "Log Host Search" );
    case eLogMediaStream: return QObject::tr( "Log Media Streams" );

    default:
        return QObject::tr( "Invalid Log Module" );
    }
}

//============================================================================
AppletLogSettings::AppletLogSettings( AppCommon& app, QWidget * parent )
: AppletBase( OBJNAME_APPLET_LOG_SETTINGS, app, parent )
, m_LogMgr( GetLogMgrInstance() )
{
	setAppletType( eAppletLogSettings );
	ui.setupUi( getContentItemsFrame() );
    setTitleBarText( QObject::tr( "Log Settings" ) );
    //ui.m_LogModuleCheckBox_20->setVisible( false );

    m_LogLevelList.push_back( ui.m_LogLevelCheckBox_1 );
    m_LogLevelList.push_back( ui.m_LogLevelCheckBox_2 );
    m_LogLevelList.push_back( ui.m_LogLevelCheckBox_3 );
    m_LogLevelList.push_back( ui.m_LogLevelCheckBox_4 );
    m_LogLevelList.push_back( ui.m_LogLevelCheckBox_5 );
    m_LogLevelList.push_back( ui.m_LogLevelCheckBox_6 );
    m_LogLevelList.push_back( ui.m_LogLevelCheckBox_7 );
    m_LogLevelList.push_back( ui.m_LogLevelCheckBox_8 );
    m_LogLevelList.push_back( ui.m_LogLevelCheckBox_9 );

    m_LogModuleList.push_back( ui.m_LogModuleCheckBox_1 );
    m_LogModuleList.push_back( ui.m_LogModuleCheckBox_2 );
    m_LogModuleList.push_back( ui.m_LogModuleCheckBox_3 );
    m_LogModuleList.push_back( ui.m_LogModuleCheckBox_4 );
    m_LogModuleList.push_back( ui.m_LogModuleCheckBox_5 );
    m_LogModuleList.push_back( ui.m_LogModuleCheckBox_6 );
    m_LogModuleList.push_back( ui.m_LogModuleCheckBox_7 );
    m_LogModuleList.push_back( ui.m_LogModuleCheckBox_8 );
    m_LogModuleList.push_back( ui.m_LogModuleCheckBox_9 );
    m_LogModuleList.push_back( ui.m_LogModuleCheckBox_10 );
    m_LogModuleList.push_back( ui.m_LogModuleCheckBox_11 );
    m_LogModuleList.push_back( ui.m_LogModuleCheckBox_12 );
    m_LogModuleList.push_back( ui.m_LogModuleCheckBox_13 );
    m_LogModuleList.push_back( ui.m_LogModuleCheckBox_14 );
    m_LogModuleList.push_back( ui.m_LogModuleCheckBox_15 );
    m_LogModuleList.push_back( ui.m_LogModuleCheckBox_16 );
    m_LogModuleList.push_back( ui.m_LogModuleCheckBox_17 );
    m_LogModuleList.push_back( ui.m_LogModuleCheckBox_18 );
    m_LogModuleList.push_back( ui.m_LogModuleCheckBox_19 );
    m_LogModuleList.push_back( ui.m_LogModuleCheckBox_20 );
    m_LogModuleList.push_back( ui.m_LogModuleCheckBox_21 );
    m_LogModuleList.push_back( ui.m_LogModuleCheckBox_22 );
    m_LogModuleList.push_back( ui.m_LogModuleCheckBox_23 );
    m_LogModuleList.push_back( ui.m_LogModuleCheckBox_24 );
    m_LogModuleList.push_back( ui.m_LogModuleCheckBox_25 );
    m_LogModuleList.push_back( ui.m_LogModuleCheckBox_26 );

    ui.m_LogModuleCheckBox_26->setVisible( false ); // not yet used
    uint32_t logModuleMask = 0x01;
    for( int i = 0; i < m_LogModuleList.size(); i++ )
    {
        m_LogModuleList[ i ]->setText( describeLogModule( (ELogModule)logModuleMask ) );
        m_LogModuleList[i]->update();
        logModuleMask = logModuleMask << 1;
    }

    connectSignals();
    updateDlgFromSettings();

    m_MyApp.activityStateChange( this, true );
}

//============================================================================
AppletLogSettings::~AppletLogSettings()
{
    m_MyApp.activityStateChange( this, false );
}

//============================================================================
void AppletLogSettings::connectSignals( void )
{
    connect( ui.m_SavePushButton, SIGNAL( clicked() ), this, SLOT( slotSaveSettings() ) );
    connect( ui.m_CancelPushButton, SIGNAL( clicked() ), this, SLOT( closeApplet() ) );
    connect( ui.m_SelectAllLevelsButton, SIGNAL( clicked() ), this, SLOT( slotSelectAllLevels() ) );
    connect( ui.m_ClearAllLevelsButton, SIGNAL( clicked() ), this, SLOT( slotClearAllLevels() ) );
    connect( ui.m_SelectAllModulesButton, SIGNAL( clicked() ), this, SLOT( slotSelectAllModules() ) );
    connect( ui.m_ClearAllModulesButton, SIGNAL( clicked() ), this, SLOT( slotClearAllModules() ) );
}

//============================================================================
void AppletLogSettings::showEvent( QShowEvent * ev )
{
    AppletBase::showEvent( ev );
    uint32_t logModuleMask = 0x01;
    for( int i = 0; i < m_LogModuleList.size(); i++ )
    {
        m_LogModuleList[ i ]->setText( describeLogModule( (ELogModule)logModuleMask ) );
        m_LogModuleList[i]->update();
        logModuleMask = logModuleMask << 1;
    }


}

//============================================================================
void AppletLogSettings::updateDlgFromSettings()
{
    uint32_t logLevelFlags = m_LogMgr.getLogLevels();
    uint32_t logLevelMask = 0x01;
    for( int i = 0; i < m_LogLevelList.size(); i++ )
    {
        m_LogLevelList[ i ]->setChecked( 0 != ( logLevelMask & logLevelFlags ) );
        logLevelMask = logLevelMask << 1;
    }

    uint32_t logModuleFlags = m_LogMgr.getLogModules();
    uint32_t logModuleMask = 0x01;
    for( int i = 0; i < m_LogModuleList.size(); i++ )
    {
        m_LogModuleList[ i ]->setChecked( 0 != ( logModuleMask & logModuleFlags ) );
        logModuleMask = logModuleMask << 1;
    }
}

//============================================================================
void AppletLogSettings::updateSettingsFromDlg()
{
    uint32_t logLevelFlags = 0;
    uint32_t logLevelMask = 0x01;
    for( int i = 0; i < m_LogLevelList.size(); i++ )
    {
        if( m_LogLevelList[ i ]->isChecked() )
        {
            logLevelFlags |= logLevelMask;
        }

        logLevelMask = logLevelMask << 1;
    }

    m_LogMgr.setLogLevels( logLevelFlags );

    uint32_t logModuleFlags = 0;
    uint32_t logModuleMask = 0x01;
    for( int i = 0; i < m_LogModuleList.size(); i++ )
    {
        if( m_LogModuleList[ i ]->isChecked() )
        {
            logModuleFlags |= logModuleMask;
        }

        logModuleMask = logModuleMask << 1;
    }

    m_LogMgr.setLogModules( logModuleFlags );
}

//============================================================================
void AppletLogSettings::slotSaveSettings( void )
{
    updateSettingsFromDlg();
    m_MyApp.activityStateChange( this, false );
    closeApplet();
}

//============================================================================
void AppletLogSettings::slotCancel( void )
{
    m_MyApp.activityStateChange( this, false );
    closeApplet();
}

//============================================================================
void AppletLogSettings::slotSelectAllLevels( void )
{
    for( int i = 0; i < m_LogLevelList.size(); i++ )
    {
        m_LogLevelList[ i ]->setChecked( true );
    }
}

//============================================================================
void AppletLogSettings::slotClearAllLevels( void )
{
    for( int i = 0; i < m_LogLevelList.size(); i++ )
    {
        m_LogLevelList[ i ]->setChecked( false );
    }
}

//============================================================================
void AppletLogSettings::slotSelectAllModules( void )
{
    for( int i = 0; i < m_LogModuleList.size(); i++ )
    {
        m_LogModuleList[ i ]->setChecked( true );
    }
}

//============================================================================
void AppletLogSettings::slotClearAllModules( void )
{
    for( int i = 0; i < m_LogModuleList.size(); i++ )
    {
        m_LogModuleList[ i ]->setChecked( false );
    }
}

//============================================================================
void AppletLogSettings::slotExitButtonClick()
{
    m_MyApp.activityStateChange( this, false );
    closeApplet();
}
