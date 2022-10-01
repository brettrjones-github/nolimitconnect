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

#include "ui_NetworkKeyWidget.h"

class NetworkKeyWidget : public QWidget
{
	Q_OBJECT

public:
    NetworkKeyWidget( QWidget* parent = 0 );

    QString						getNetworkKey( void ) { return ui.m_NetworkKeyEdit->text();  }

protected slots:
    void                        slotShowNetworkKeyInformation( void );
    void                        slotApplyNetworkKey( void );

protected:
	void						initNetworkKeyWidget( void );
    void						fillNetworkKeyEditField( void );

    void						updateNetworkKeyIcon( void );
    void                        updateUi( void );
    bool                        verifyNetworkKey( QString& keyVal );

    AppCommon&                  m_MyApp;
    EPluginType                 m_PluginType = ePluginTypeInvalid;
	Ui::NetworkKeyWidgetUi	    ui;
};
