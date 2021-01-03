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
// http://www.nolimitconnect.com
//============================================================================

#include "AppletLaunchPage.h"

class AppletSettingsPage : public AppletLaunchPage
{
	Q_OBJECT
public:
	AppletSettingsPage( AppCommon& app, QWidget * parent );
	virtual ~AppletSettingsPage() = default;

protected:
    virtual void				showEvent( QShowEvent * );
    void						resizeEvent( QResizeEvent * );

private:
    void						setupAppletSettingsgPage( void );

    bool						m_IsInitialized{ false };
    QVector<VxWidgetBase*>		m_AppletList;
};

