#pragma once
//============================================================================
// Copyright (C) 2018 Brett R. Jones
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

#include "AppletServiceBaseSettings.h"

class AppletSettingsAboutMe : public AppletServiceBaseSettings
{
	Q_OBJECT
public:
	AppletSettingsAboutMe( AppCommon& app, QWidget* parent );
	virtual ~AppletSettingsAboutMe();

protected:

};


