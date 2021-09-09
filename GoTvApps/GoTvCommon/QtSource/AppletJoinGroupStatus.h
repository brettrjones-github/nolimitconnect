#pragma onceAppletJoinGroupStatus
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

#include "AppletSettingsHostBase.h"
#include "PluginSettingsWidget.h"
#include "PermissionWidget.h"

#include <GuiInterface/IToGui.h>
#include <ptop_src/ptop_engine_src/PluginSettings/PluginSettingMgr.h>

#include "ui_AppletJoinGroupStatus.h"
 
class AppletJoinGroupStatus : public AppletSettingsHostBase
{
	Q_OBJECT
public:
    AppletJoinGroupStatus( AppCommon& app, QWidget * parent );
	virtual ~AppletJoinGroupStatus();

    PluginSettingsWidget*       getPluginSettingsWidget()           { return ui.m_PluginSettingsWidget; }
    PermissionWidget*           getConnectionTestWidget()           { return ui.m_ConnectTestPermissinWidget; }

protected slots:
    void                        slotApplyServiceSettings();

protected:
    void                        connectServiceWidgets();
    void                        loadPluginSetting();
    void                        savePluginSetting();

    Ui::AppletJoinGroupStatusUi     ui;
};


