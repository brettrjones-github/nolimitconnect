#pragma once
//============================================================================
// Copyright (C) 2020 Brett R. Jones
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

#include "ui_RelayServiceWidget.h"

class RelayServiceWidget : public QWidget
{
	Q_OBJECT

public:
    RelayServiceWidget( QWidget* parent = 0 );

    void						setPluginType( EPluginType pluginType, int subType = 0 );
    void						setPermissionLevel( EFriendState permLevel );
    EFriendState                getPermissionLevel( void );

protected slots:
    void                        slotHandleSelectionChanged( int );
    void                        slotShowPermissionInformation();
    void                        slotShowPluginInformation();

protected:
	void						initRelayServiceWidget( void );
    void						fillPermissionComboBox( void );

    void						updatePermissionIcon( void );
    void                        updateUi( void );

    Ui::RelayServiceWidgetUi	    ui;
    AppCommon&                  m_MyApp;
    EPluginType                 m_PluginType = ePluginTypeInvalid;
    int                         m_SubPluginType = 0;
    bool                        m_OrigPermissionIsSet = false;
    EFriendState                m_OrigPermissionLevel = eFriendStateIgnore;
    bool                        m_PermissionsConnected = false;
};
