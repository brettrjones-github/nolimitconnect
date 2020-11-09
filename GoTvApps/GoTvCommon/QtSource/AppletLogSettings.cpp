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
// http://www.nolimitconnect.com
//============================================================================
#include <app_precompiled_hdr.h>
#include "AppletLogSettings.h"

#include "LogMgr.h"
#include "AppGlobals.h"
#include "AppCommon.h"
#include "AppSettings.h"
#include "MyIcons.h"

#include <CoreLib/VxDebug.h>
#include <CoreLib/ObjectCommon.h>

namespace
{
}

//============================================================================
AppletLogSettings::AppletLogSettings( AppCommon& app, QWidget * parent )
: AppletBase( OBJNAME_APPLET_LOG_SETTINGS, app, parent )
, m_LogMgr( GetLogMgrInstance() )
{
	setAppletType( eAppletLogSettings );
	ui.setupUi( getContentItemsFrame() );
    ui.m_LogModuleCheckBox_20->setVisible( false );

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
    connect( ui.m_CancelPushButton, SIGNAL( clicked() ), this, SLOT( close() ) );
    connect( ui.m_SelectAllLevelsButton, SIGNAL( clicked() ), this, SLOT( slotSelectAllLevels() ) );
    connect( ui.m_ClearAllLevelsButton, SIGNAL( clicked() ), this, SLOT( slotClearAllLevels() ) );
    connect( ui.m_SelectAllModulesButton, SIGNAL( clicked() ), this, SLOT( slotSelectAllModules() ) );
    connect( ui.m_ClearAllModulesButton, SIGNAL( clicked() ), this, SLOT( slotClearAllModules() ) );
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
    for( int i = 0; i < m_LogLevelList.size(); i++ )
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
    for( int i = 0; i < m_LogLevelList.size(); i++ )
    {
        if( m_LogModuleList[ i ]->isChecked() )
        {
            logModuleFlags |= logModuleMask;
        }

        logModuleMask = logModuleMask << 1;
    }

    m_LogMgr.setLogModules( logLevelFlags );
}

//============================================================================
void AppletLogSettings::slotSaveSettings( void )
{
    updateSettingsFromDlg();
    m_MyApp.activityStateChange( this, false );
    close();
}

//============================================================================
void AppletLogSettings::slotCancel( void )
{
    m_MyApp.activityStateChange( this, false );
    close();
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
    close();
}
