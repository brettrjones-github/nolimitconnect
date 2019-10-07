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
// http://www.gotvptop.com
//============================================================================

#include "AppCommon.h"

#include "ui_PermissionWidget.h"

class PermissionWidget : public QWidget
{
	Q_OBJECT

public:
    PermissionWidget( QWidget * parent = 0 );

    void						setPluginType( EPluginType pluginType, int subType = 0 ) { m_PluginType = pluginType; m_SubPluginType = subType;  updateUi();  updatePermissionIcon(); }

protected slots:
    void                        slotHandleSelectionChanged( int );
    void                        slotShowPermissionInformation();

protected:
	void						initPermissionWidget( void );
    void						fillPermissionComboBox( void );

    void						updatePermissionIcon( void );
    void                        updateUi( void );

    AppCommon&                  m_MyApp;
    EPluginType                 m_PluginType = ePluginTypeInvalid;
    int                         m_SubPluginType = 0;
	Ui::PermissionWidgetUi	    ui;
};
