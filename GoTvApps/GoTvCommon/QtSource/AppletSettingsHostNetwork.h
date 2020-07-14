#pragma once
//============================================================================
// Copyright (C) 2018 Brett R. Jones
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

#include "AppletBase.h"
#include "PluginSettingsWidget.h"
#include "PermissionWidget.h"

#include <GoTvInterface/IToGui.h>
#include <GoTvCore/GoTvP2P/P2PEngine/PluginSettingMgr.h>

#include "ui_AppletSettingsHostNetwork.h"


class AppletSettingsHostNetwork: public AppletBase
{
	Q_OBJECT
public:
    AppletSettingsHostNetwork( AppCommon& app, QWidget * parent );
	virtual ~AppletSettingsHostNetwork();

    PluginSettingsWidget*       getPluginSettingsWidget()           { return ui.m_PluginSettingsWidget; }
    PermissionWidget*           getGroupListingWidget()             { return ui.m_GroupListPermissionWidget; }
    PermissionWidget*           getConnectionTestWidget()           { return ui.m_ConnectTestPermissinWidget; }

protected:
    void setupApplet();
    void loadFromSettings();
    void saveToSettings();

    Ui::AppletSettingsHostNetworkUi     ui;
};


