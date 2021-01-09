#pragma once
//============================================================================
// Copyright (C) 2010 Brett R. Jones
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

#include "ui_AppletNetworkSettings.h"

#include "AppletBase.h"
#include "AppDefs.h"

#include <NetLib/NetHostSetting.h>

#include <QTimer>

class AppletNetworkSettings : public AppletBase
{
	Q_OBJECT
public:
	AppletNetworkSettings( AppCommon& app, QWidget * parent );
	virtual ~AppletNetworkSettings();

protected slots:
	void						slotNetworkSettingsSelectionChanged( int idx );

    void						slotExitButtonClick( void );
    void						slotGoToNetHostSettingsButtonClick( void );
    void						slotRandomPortButtonClick( void );
    void						slotTestIsMyPortOpenButtonClick( void );

    void						slotAutoDetectProxyClick( void );
    void						slotNoProxyClick( void );
    void						slotYesProxyClick( void );
    void						slotUseUpnpCheckBoxClick( void );

    void						onSaveButtonClick( void );
    void						onDeleteButtonClick( void );
    void						slotApplySettingsButtonClick( void );
    void						onComboBoxSelectionChange( const QString& anchorSettingName );
    void						onComboBoxTextChanged( const QString & text );

    void                        slotShowNetworkHostInformation( void );
    void                        slotShowNetworkKeyInformation( void );
    void                        slotShowConnectTestUrlInformation( void );
    void                        slotShowConnectTestSettingsInformation( void );
    void						slotShowRandomConnectUrlInformation( void );
    void						slotShowDefaultGroupHostUrlInformation( void );
    void						slotShowDefaultChatRoomUrlInformation( void );

    void						slotCopyMyUrlToClipboard( void );

    void						slotUpdateTimer( void );

protected:
    void						closeEvent( QCloseEvent *event );

    void						connectSignals( void );

    QString						getNetworkKey( void ) { return ui.m_NetworkKeyEdit->text(); }
    bool                        verifyNetworkKey( QString& keyVal );

    void						updateDlgFromSettings( bool initialSettings );
    void						updateSettingsFromDlg( void );
    void						populateDlgFromNetHostSetting( NetHostSetting& netSettings );
    void						setFirewallTest( FirewallSettings::EFirewallTestType eFirewallType );
    void                        populateNetHostSettingsFromDlg( NetHostSetting& netHostSetting );
    void                        applyEngineSettingsFromHostSetting( NetHostSetting& netHostSetting );

    void                        fillNetHostSettingFromEngine( NetHostSetting& netSettings );

	Ui::AppletNetworkSettingsWidget		ui;
    NetHostSetting              m_OriginalSettings;
    QTimer *                    m_UpdateTimer{ nullptr };
};


