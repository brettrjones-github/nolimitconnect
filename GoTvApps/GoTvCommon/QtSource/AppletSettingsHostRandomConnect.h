#pragma once
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

#include "AppletSettingsBase.h"
#include "PluginSettingsWidget.h"
#include "PermissionWidget.h"

#include <GoTvInterface/IToGui.h>
#include <GoTvCore/GoTvP2P/PluginSettings/PluginSettingMgr.h>

#include "ui_AppletSettingsHostNetwork.h"

class AppletSettingsHostRandomConnect : public AppletSettingsBase
{
	Q_OBJECT
public:
    AppletSettingsHostRandomConnect( AppCommon& app, QWidget * parent );
	virtual ~AppletSettingsHostRandomConnect();

    PluginSettingsWidget*       getPluginSettingsWidget()           { return ui.m_PluginSettingsWidget; }
    PermissionWidget*           getGroupListingWidget()             { return ui.m_GroupListPermissionWidget; }
    PermissionWidget*           getConnectionTestWidget()           { return ui.m_ConnectTestPermissinWidget; }

protected slots:
    void                        slotApplyServiceSettings();

protected:
    void                        connectServiceWidgets();
    void                        loadPluginSetting();
    void                        savePluginSetting();

    Ui::AppletSettingsHostNetworkUi     ui;
};

