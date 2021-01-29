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
// http://www.nolimitconnect.com
//============================================================================
#include <app_precompiled_hdr.h>
#include "AppletSettingsHostRandomConnect.h"
#include "AppCommon.h"
#include "AppSettings.h"
#include "MyIcons.h"
#include "GuiHelpers.h"

#include <CoreLib/VxDebug.h>
#include <CoreLib/VxTime.h>

//============================================================================
AppletSettingsHostRandomConnect::AppletSettingsHostRandomConnect( AppCommon& app, QWidget * parent )
: AppletSettingsBase( OBJNAME_APPLET_SETTINGS_HOST_RANDOM_CONNECT, app, parent )
{
    ui.setupUi( getContentItemsFrame() );
    setAppletType( eAppletSettingsHostRandomConnect );
    setTitleBarText( DescribeApplet( m_EAppletType ) );
    getPluginSettingsWidget()->setupSettingsWidget( eAppletSettingsHostRandomConnect, ePluginTypeRandomConnectHost );
    getPluginSettingsWidget()->getPermissionWidget()->getPluginRunButton()->setVisible( false );
    getPluginSettingsWidget()->getPermissionWidget()->getPluginSettingsButton()->setVisible( false );
    getGroupListingWidget()->setPluginType( ePluginTypeNetworkSearchList );
    getConnectionTestWidget()->setPluginType( ePluginTypeConnectTestHost );
    connectServiceWidgets();
    loadPluginSetting();

    m_MyApp.activityStateChange( this, true );
}

//============================================================================
AppletSettingsHostRandomConnect::~AppletSettingsHostRandomConnect()
{
    m_MyApp.activityStateChange( this, false );
}

//============================================================================
void AppletSettingsHostRandomConnect::connectServiceWidgets()
{
    connect( getPluginSettingsWidget()->getApplyButton(), SIGNAL( clicked() ), this, SLOT( slotApplyServiceSettings() ) );
}

//============================================================================
void AppletSettingsHostRandomConnect::loadPluginSetting()
{
    if( ePluginTypeInvalid != getPluginType() )
    {
        m_OrigPermissionLevel = m_MyApp.getEngine().getPluginPermission( getPluginType() );
        // m_OrigGroupListPermission = m_MyApp.getEngine().getPluginPermission( getGroupListingWidget()->getPluginType() );
        m_OrigConnectTestPermission = m_MyApp.getEngine().getPluginPermission( getConnectionTestWidget()->getPluginType() );

        getPluginSettingsWidget()->getPermissionWidget()->setPermissionLevel( m_OrigPermissionLevel );
        // getGroupListingWidget()->setPermissionLevel( m_OrigGroupListPermission );
        getConnectionTestWidget()->setPermissionLevel( m_OrigConnectTestPermission );

        m_MyApp.getEngine().getPluginSettingMgr().getPluginSetting( getPluginType(), m_PluginSetting );
        loadUiFromSetting();
    }
}

//============================================================================
void AppletSettingsHostRandomConnect::savePluginSetting()
{
    if( ( ePluginTypeInvalid != getPluginType() ) && ( ePluginTypeInvalid != m_PluginSetting.getPluginType() ) )
    {
        saveUiToSetting();
        m_MyApp.getEngine().getPluginSettingMgr().setPluginSetting( m_PluginSetting );
    }
}

//============================================================================
void AppletSettingsHostRandomConnect::slotApplyServiceSettings()
{
    saveUiToSetting();
    m_PluginSetting.setLastUpdateTimestamp( GetGmtTimeMs() );
    m_MyApp.getEngine().getPluginSettingMgr().setPluginSetting( m_PluginSetting );
    EFriendState newPermissionLevel = getPluginSettingsWidget()->getPermissionWidget()->getPermissionLevel();
    EFriendState newGroupListPermission = getGroupListingWidget()->getPermissionLevel();
    EFriendState newConnectionTestPermission = getConnectionTestWidget()->getPermissionLevel();
    if( newPermissionLevel != m_OrigPermissionLevel )
    {
        m_MyApp.getEngine().setPluginPermission( getPluginSettingsWidget()->getPermissionWidget()->getPluginType(), newPermissionLevel );
    }

    /*
    if( newGroupListPermission != m_OrigGroupListPermission )
    {
        m_MyApp.getEngine().setPluginPermission( getGroupListingWidget()->getPluginType(), newGroupListPermission );
    }*/

    if( newConnectionTestPermission != m_OrigConnectTestPermission )
    {
        m_MyApp.getEngine().setPluginPermission( getConnectionTestWidget()->getPluginType(), newConnectionTestPermission );
    }

    QMessageBox::information( this, QObject::tr( "Service Settings" ), QObject::tr( "Service Settings Applied" ), QMessageBox::Ok );
}
