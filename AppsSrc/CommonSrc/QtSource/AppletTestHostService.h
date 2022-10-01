#pragma once
//============================================================================
// Copyright (C) 2021 Brett R. Jones
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

#include "AppletTestBase.h"
#include "ui_AppletTestHostService.h"

class AppletTestHostService : public AppletTestBase
{
	Q_OBJECT
public:
    AppletTestHostService( AppCommon& app, QWidget* parent );
	virtual ~AppletTestHostService();

protected:
    Ui::AppletTestHostServiceUi          ui;
};


