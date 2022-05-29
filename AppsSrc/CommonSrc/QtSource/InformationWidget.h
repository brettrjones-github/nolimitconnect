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
// http://www.nolimitconnect.org
//============================================================================

#include "AppCommon.h"

#include "ui_InformationWidget.h"

class ActivityInformation;

class InformationWidget : public QWidget
{
	Q_OBJECT

public:
    InformationWidget( QWidget * parent = 0 );

    void						setPluginType( EPluginType pluginType ) { m_PluginType = pluginType; updateInformation(); }
    EPluginType                 getPluginType() { return m_PluginType;  }

protected slots:
    void                        slotShowInformation();

protected:
	void						initInformationWidget( void );
    void						updateInformation( void );

    Ui::InformationWidgetUi	    ui;
    AppCommon&                  m_MyApp;
    ActivityInformation *       m_ActivityInfo = nullptr;
    EPluginType                 m_PluginType = ePluginTypeInvalid;
};
