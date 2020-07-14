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

#include "AppletBase.h"
#include <GoTvInterface/IToGui.h>
#include <GoTvCore/GoTvP2P/P2PEngine/PluginSettingMgr.h>

#include "ui_PluginSettingsWidget.h"

#include "ThumbnailChooseWidget.h"

class PluginSettingsWidget : public QWidget
{
	Q_OBJECT
public:
    PluginSettingsWidget( QWidget * parent );
	virtual ~PluginSettingsWidget() = default;

    void						setupSettingsWidget( EApplet applet, EPluginType pluginType );

    virtual QLabel *            getServiceUrlLabel() { return ui.m_UrlDescriptionLabel; }
    virtual QLineEdit *         getServiceUrlEdit() { return ui.m_UrlEdit; }

    virtual QLabel *            getServiceNameLabel() { return ui.m_UserDisplayedNameLabel; }
    virtual QLineEdit *         getServiceNameEdit() { return ui.m_NameEdit; }

    virtual QLabel *            getServiceDescriptionLabel() { return ui.m_DescriptionLabel; }
    virtual QPlainTextEdit *    getServiceDescriptionEdit() { return ui.m_DescriptionEdit; }

    virtual QPushButton *       getStartButton() { return ui.m_StartButton; }
    virtual QPushButton *       getStopButton() { return ui.m_StopButton; }

    virtual PermissionWidget *  getPermissionWidget() { return ui.m_PermissionWidget; }
    virtual InformationWidget * getInformationWidget() { return ui.m_InfoWidget; }

    virtual ThumbnailChooseWidget *   getThumbChooseWidget() { return ui.m_ThumbnailChooseWidget; }

protected slots:
    virtual void                slotApplyServiceSettings();

protected:
    EPluginType                 getPluginType() { return m_PluginType; }

    virtual void                connectServiceWidgets();
    virtual void                loadPluginSetting();
    virtual void                savePluginSetting();

    virtual void                loadUiFromSetting();
    virtual void                saveUiToSetting();

    Ui::PluginSettingsUi        ui;
    AppCommon&                  m_MyApp;
    EApplet                     m_EAppletType = eAppletUnknown;
    EPluginType                 m_PluginType = ePluginTypeInvalid;
    AppletBase*                 m_ParentApplet{ nullptr };
    PluginSetting               m_PluginSetting;
    int                         m_SubPluginType = 0;
    bool                        m_OrigPermissionIsSet = false;
    EFriendState                m_OrigPermissionLevel = eFriendStateIgnore;
    bool                        m_PermissionsConnected = false;

};


