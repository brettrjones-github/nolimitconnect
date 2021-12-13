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

#include "AppCommon.h"
#include "AppDefs.h"

#include "ui_ServiceSettingsWidget.h"

class ServiceSettingsWidget : public QWidget
{
	Q_OBJECT

public:
    ServiceSettingsWidget( QWidget * parent = 0 );

    void						setPluginType( EPluginType pluginType ) { m_PluginType = pluginType;  updateUi();  updateIcons(); }
    void						setViewServiceVisible( bool visible )   { ui.m_ViewServiceFrame->setVisible( visible ); }

protected slots:
    void                        slotServiceSettingsClicked();
    void                        slotViewServiceClicked();

protected:
	void						initServiceSettingsWidget( void );
    void						launchApplet( EApplet appletType );

    void						updateIcons( void );
    void                        updateUi( void );

    AppCommon&                  m_MyApp;
    EPluginType                 m_PluginType = ePluginTypeInvalid;
	Ui::ServiceSettingsWidgetUi	ui;
};
