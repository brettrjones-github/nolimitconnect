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

#include "AppletBase.h"
#include "PluginSettingsWidget.h"
#include "PermissionWidget.h"

#include <GuiInterface/IToGui.h>
#include <ptop_src/ptop_engine_src/PluginSettings/PluginSettingMgr.h>
 
class AppletSettingsBase : public AppletBase
{
	Q_OBJECT
public:
    AppletSettingsBase( const char * ObjName, AppCommon& app, QWidget * parent );
	virtual ~AppletSettingsBase() = default;

    virtual PluginSettingsWidget*   getPluginSettingsWidget() = 0;
    virtual PermissionWidget*       getConnectionTestWidget() = 0;

protected slots:
    void                            slotApplyServiceSettings();

protected:
    void                            connectServiceWidgets();
    void                            loadPluginSetting();
    void                            savePluginSetting();
    void                            loadUiFromSetting();
    void                            saveUiToSetting();

    PluginSetting                   m_PluginSetting;
    EFriendState                    m_OrigPermissionLevel = eFriendStateIgnore;
    EFriendState                    m_OrigRelayPermission = eFriendStateIgnore;
    EFriendState                    m_OrigConnectTestPermission = eFriendStateIgnore;
};


