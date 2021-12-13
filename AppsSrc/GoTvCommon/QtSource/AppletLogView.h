#pragma once
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

#include "ui_AppletLogView.h"

#include "AppletBase.h"
#include "AppDefs.h"
#include "AppCommon.h"

class LogMgr;

class AppletLogView : public AppletBase
{
	Q_OBJECT
public:
    AppletLogView( AppCommon& app, QWidget * parent );
	virtual ~AppletLogView();

protected slots:
    void						slotExitButtonClick( void );
    void						slotLaunchLogSettings( void );
    void                        slotLogMsg( uint32_t u32LogFlags, const QString& logMsg );

protected:
    void						closeEvent( QCloseEvent *event );

    void						connectSignals( void );

    void						updateDlgFromSettings( bool initialSettings );
    void						updateSettingsFromDlg( void );

	Ui::AppletLogViewWidget		ui;
    LogMgr&                     m_LogMgr;
};


