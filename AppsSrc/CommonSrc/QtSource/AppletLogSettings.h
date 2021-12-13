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

#include "ui_AppletLogSettings.h"

#include "AppletBase.h"
#include "AppDefs.h"

class LogMgr;

class AppletLogSettings : public AppletBase
{
	Q_OBJECT
public:
    AppletLogSettings( AppCommon& app, QWidget * parent );
	virtual ~AppletLogSettings();

protected slots:
	void						slotSaveSettings( void );
    void                        slotCancel( void );
    void						slotExitButtonClick( void );

    void						slotSelectAllLevels( void );
    void						slotClearAllLevels( void );
    void						slotSelectAllModules( void );
    void						slotClearAllModules( void );

protected:
    void                        showEvent( QShowEvent * ev ) override;

    void						connectSignals( void );
    void						updateDlgFromSettings( void );
    void						updateSettingsFromDlg( void );


	Ui::AppletLogSettingsWidget	ui;
    LogMgr&                     m_LogMgr;
    QVector<QCheckBox*>         m_LogLevelList;
    QVector<QCheckBox*>         m_LogModuleList;
};


