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

#include "AppletLogView.h"
#include "LogMgr.h"

#include "AppGlobals.h"
#include "AppCommon.h"
#include "AppSettings.h"
#include "MyIcons.h"

#include <CoreLib/VxDebug.h>
#include <CoreLib/ObjectCommon.h>

namespace
{
    //QString				DEFAULT_ADAPTER_IP_CHOICE = "default";
}

//============================================================================
AppletLogView::AppletLogView( AppCommon& app, QWidget * parent )
: AppletBase( OBJNAME_APPLET_LOG_VIEW, app, parent )
, m_LogMgr( GetLogMgrInstance() )
{
	setAppletType( eAppletLogView );
	ui.setupUi( getContentItemsFrame() );

    m_LogMgr.lockLogQue();
    QVector<LogQueEntry>& logQue = m_LogMgr.getLogQue();
    for( LogQueEntry& logEntry : logQue )
    {
        ui.m_LogWidget->onLogEvent( logEntry.getLogFlags(), logEntry.getLogText().toUtf8().constData() );
    }

    connectSignals();
    m_LogMgr.unlockLogQue();

    m_MyApp.activityStateChange( this, true );
}

//============================================================================
AppletLogView::~AppletLogView()
{
    m_MyApp.activityStateChange( this, false );
}

//============================================================================
void AppletLogView::connectSignals( void )
{
    connect( ui.m_LogSettingsPushButton, SIGNAL( clicked() ), this, SLOT( slotLaunchLogSettings() ) );
    connect( &m_LogMgr, SIGNAL( signalLogMsg( uint32_t, const QString& ) ), this, SLOT( slotLogMsg( uint32_t, const QString& ) ) );
}

//============================================================================
void AppletLogView::updateDlgFromSettings( bool origSettings )
{
}

//============================================================================
void AppletLogView::updateSettingsFromDlg()
{
}

//============================================================================
void AppletLogView::slotExitButtonClick()
{
    close();
}

//============================================================================
void AppletLogView::slotLogMsg( uint32_t u32LogFlags, const QString& logMsg )
{
    ui.m_LogWidget->onLogEvent( u32LogFlags, logMsg.toUtf8().constData() );
}

//============================================================================
void AppletLogView::slotLaunchLogSettings()
{
    m_MyApp.launchApplet( eAppletLogSettings, this );
}

//============================================================================   
void AppletLogView::closeEvent( QCloseEvent * ev )
{
    AppletBase::closeEvent( ev );
    m_MyApp.activityStateChange( this, false );
}
