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
// http://www.nolimitconnect.org
//============================================================================

#include "AppletSettingsHostBase.h"
#include "PluginSettingsWidget.h"
#include "PermissionWidget.h"

#include <GuiInterface/IToGui.h>
#include <ptop_src/ptop_engine_src/PluginSettings/PluginSettingMgr.h>

#include "ui_AppletHostNetworkStatus.h"
 
class AppletHostGroupStatus : public AppletBase
{
	Q_OBJECT
public:
    AppletHostGroupStatus( AppCommon& app, QWidget * parent );
	virtual ~AppletHostGroupStatus();

    PermissionWidget*           getGroupHostPermissionWidget()      { return ui.m_HostPermissionWidget; }
    PermissionWidget*           getConnectionTestWidget()           { return ui.m_ConnectTestPermissionWidget; }

protected slots:
    void                        slotHostRequirementsButtonClicked();
    void                        slotUpdateStatusTimeout();

protected:
    Ui::AppletHostNetworkStatusUi     ui;

    QTimer*                     m_UpdateStatusTimer;
};


