#pragma once
//============================================================================
// Copyright (C) 2019 Brett R. Jones
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
#include <GuiInterface/IToGui.h>
#include <ptop_src/ptop_engine_src/PluginSettings/PluginSettingMgr.h>

#include "ui_AppletServiceBase.h"

class AppletServiceBase : public AppletBase
{
	Q_OBJECT
public:
    AppletServiceBase( const char * objName, AppCommon& app, QWidget * parent );
	virtual ~AppletServiceBase() = default;

    void						setupServiceBaseApplet( EApplet applet, EPluginType pluginType );

    virtual QLabel *            getServiceTitle() { return ui.m_ServiceTitleLabel; }

    virtual QLabel *            getServiceUrlLabel() { return ui.m_UrlDescriptionLabel; }
    virtual QLineEdit *         getServiceUrlEdit() { return ui.m_UrlEdit; }

    virtual QLabel *            getServiceNameLabel() { return ui.m_UserDisplayedNameLabel; }
    virtual QLineEdit *         getServiceNameEdit() { return ui.m_NameEdit; }

    virtual QLabel *            getServiceKeyWordsLabel() { return ui.m_KeyWordsLabel; }
    virtual QLineEdit *         getServiceKeyWordsEdit() { return ui.m_KeyWordsEdit; }

    virtual QLabel *            getServiceDescriptionLabel() { return ui.m_DescriptionLabel; }
    virtual QPlainTextEdit *    getServiceDescriptionEdit() { return ui.m_DescriptionEdit; }

    virtual QPushButton *       getStartButton() { return ui.m_StartButton; }
    virtual QPushButton *       getStopButton() { return ui.m_StopButton; }

    virtual PermissionWidget *  getPermissionWidget() { return ui.m_PermissionWidget; }
    virtual InformationWidget * getInformationWidget() { return ui.m_InfoWidget; }
    //virtual LogWidget *         getLogWidget() { return ui.m_LogWidget; }
    virtual ThumbnailEditWidget *   getThumbEditWidget() { return ui.m_ThumbnailEditWidget; }

protected slots:
    virtual void                slotApplyServiceSettings();

protected:
    virtual void                connectServiceWidgets();
    virtual void                loadPluginSetting();
    virtual void                savePluginSetting();

    virtual void                loadUiFromSetting();
    virtual void                saveUiToSetting();

    Ui::AppletServiceBaseUi     ui;
    QWidget *                   m_HostServiceWidget;
    PluginSetting               m_PluginSetting;
    EFriendState                m_OrigPermissionLevel = eFriendStateIgnore;
};


