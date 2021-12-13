#pragma once
//============================================================================
// Copyright (C) 2020 Brett R. Jones
// Issued to MIT style license by Brett R. Jones in 2017
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

#include "ui_AppletPermissionList.h"

#include "AppletBase.h"
#include "AppDefs.h"

#include <QTimer>

class AppletPermissionList : public AppletBase
{
	Q_OBJECT
public:
	AppletPermissionList( AppCommon& app, QWidget * parent );
	virtual ~AppletPermissionList();

protected:
    void                        initializePermissionList( void );
    void                        createPermissionItem( EPluginType pluginType );

    void						connectSignals( void );

	Ui::AppletPermissionListtUi	ui;
    QVector<EPluginType>        m_PluginList;
};


