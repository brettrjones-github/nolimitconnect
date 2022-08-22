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

#include "ActivityInformation.h"
#include "AppletSettingsHostNetwork.h"
#include "AppCommon.h"
#include "AppSettings.h"
#include "MyIcons.h"
#include "GuiHelpers.h"

#include <CoreLib/VxDebug.h>
#include <CoreLib/VxTime.h>

//============================================================================
AppletSettingsHostNetwork::AppletSettingsHostNetwork( AppCommon& app, QWidget * parent )
: AppletSettingsHostBase( OBJNAME_APPLET_SETTINGS_HOST_NETWORK, app, parent )
{
    ui.setupUi( getContentItemsFrame() );
    setAppletType( eAppletSettingsHostNetwork );
    setPluginType( ePluginTypeHostNetwork );
    setTitleBarText( DescribeApplet( m_EAppletType ) );

    getPluginSettingsWidget()->setupSettingsWidget( eAppletSettingsHostNetwork, ePluginTypeHostNetwork );
    getPluginSettingsWidget()->getPermissionWidget()->getPluginRunButton()->setVisible( false );
    getPluginSettingsWidget()->getPermissionWidget()->getPluginSettingsButton()->setVisible( false );

    connect( ui.m_HostingRequirementsButton, SIGNAL( clicked() ), this, SLOT( slotHostRequirementsButtonClicked() ) );
    getConnectionTestWidget()->setPluginType( ePluginTypeHostConnectTest );
    connectServiceWidgets();
    loadPluginSetting();

    m_MyApp.activityStateChange( this, true );
}

//============================================================================
AppletSettingsHostNetwork::~AppletSettingsHostNetwork()
{
    m_MyApp.activityStateChange( this, false );
}

//============================================================================
void AppletSettingsHostNetwork::connectServiceWidgets()
{
    connect( getPluginSettingsWidget()->getApplyButton(), SIGNAL( clicked() ), this, SLOT( slotApplyServiceSettings() ) );
}

//============================================================================
void AppletSettingsHostNetwork::loadPluginSetting()
{
    if( ePluginTypeInvalid != getPluginType() )
    {
        m_OrigPermissionLevel = m_MyApp.getEngine().getPluginPermission( getPluginType() );
        //m_OrigGroupListPermission = m_MyApp.getEngine().getPluginPermission( getGroupListingWidget()->getPluginType() );
        m_OrigConnectTestPermission = m_MyApp.getEngine().getPluginPermission( getConnectionTestWidget()->getPluginType() );

        getPluginSettingsWidget()->getPermissionWidget()->setPermissionLevel( m_OrigPermissionLevel );
        //getGroupListingWidget()->setPermissionLevel( m_OrigGroupListPermission );
        getConnectionTestWidget()->setPermissionLevel( m_OrigConnectTestPermission );

        m_MyApp.getEngine().getPluginSettingMgr().getPluginSetting( getPluginType(), m_PluginSetting );
        loadUiFromSetting();
    }
}

//============================================================================
void AppletSettingsHostNetwork::savePluginSetting()
{
    if( ( ePluginTypeInvalid != getPluginType() ) && ( ePluginTypeInvalid != m_PluginSetting.getPluginType() ) )
    {
        saveUiToSetting();
        m_MyApp.getEngine().getPluginSettingMgr().setPluginSetting( m_PluginSetting, m_MyApp.elapsedMilliseconds() );
    }
}

//============================================================================
void AppletSettingsHostNetwork::slotApplyServiceSettings()
{
    saveUiToSetting();
    m_PluginSetting.setLastUpdateTimestamp( m_MyApp.elapsedMilliseconds() );
    m_MyApp.getEngine().getPluginSettingMgr().setPluginSetting( m_PluginSetting );
    EFriendState newPermissionLevel = getPluginSettingsWidget()->getPermissionWidget()->getPermissionLevel();
    // EFriendState newGroupListPermission = getGroupListingWidget()->getPermissionLevel();
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

//============================================================================
void AppletSettingsHostNetwork::slotHostRequirementsButtonClicked()
{
    ActivityInformation* activityInfo = new ActivityInformation( m_MyApp, this, eInfoTypeHostNetwork );
    if( activityInfo )
    {
        activityInfo->show();
    }
}