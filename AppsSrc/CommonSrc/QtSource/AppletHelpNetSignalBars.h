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

#include "AppletBase.h"
#include <GuiInterface/IDefs.h>

#include "ui_AppletHelpNetSignalBars.h"

enum EInfoType
{
    eInfoTypeInvalid,
    eInfoTypePlugin,
    eInfoTypePermission,
    eInfoTypeNetworkKey,
    eInfoTypeNetworkHost,
    eInfoTypeConnectTestUrl,
    eInfoTypeConnectTestSettings,

    eMaxInfoType // must be last
};

class AppCommon;

class AppletHelpNetSignalBars : public AppletBase
{
	Q_OBJECT
public:
    AppletHelpNetSignalBars( AppCommon& app, QWidget * parent );
	virtual ~AppletHelpNetSignalBars() override;


protected:
    void						initAppletHelpNetSignalBars( void );

	//=== vars ===//
	Ui::AppletHelpNetSignalBarsUi		    ui;

    static QString              m_NoInfoAvailable;
    static QString              m_NetworkDesign;
    static QString              m_PluginDefinitions;
    static QString              m_Permissions;
    static QString              m_NetworkKey;
    static QString              m_NetworkHost;
    static QString              m_ConnectTestUrl;
    static QString              m_ConnectTestSettings;
};
