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
#include "AppletSettingsBase.h"
#include "AppCommon.h"
#include "AppSettings.h"
#include "MyIcons.h"
#include "GuiHelpers.h"

#include <CoreLib/VxDebug.h>

//============================================================================
AppletSettingsBase::AppletSettingsBase( const char * ObjName, AppCommon& app, QWidget * parent )
    : AppletBase( ObjName, app, parent )
{
}

//============================================================================
AppletSettingsBase::~AppletSettingsBase()
{
}

//============================================================================
void AppletSettingsBase::connectServiceWidgets()
{
    connect( getPluginSettingsWidget()->getApplyButton(), SIGNAL( clicked() ), this, SLOT( slotApplyServiceSettings() ) );
}

//============================================================================
void AppletSettingsBase::loadPluginSetting()
{
    if( ePluginTypeInvalid != getPluginType() )
    {
        m_OrigPermissionLevel = m_MyApp.getAppGlobals().getUserIdent()->getPluginPermission( getPluginType() );
        m_OrigConnectTestPermission = m_MyApp.getAppGlobals().getUserIdent()->getPluginPermission( getConnectionTestWidget()->getPluginType() );
        getPluginSettingsWidget()->getPermissionWidget()->setPermissionLevel( m_OrigPermissionLevel );
        getConnectionTestWidget()->setPermissionLevel( m_OrigConnectTestPermission );

        m_PluginSetting.setPluginType( getPluginType() );// must set before get settings so engine will know which
        m_MyApp.getEngine().getPluginSettingMgr().getPluginSetting( getPluginType(), m_PluginSetting );
        loadUiFromSetting();
    }
}

//============================================================================
void AppletSettingsBase::savePluginSetting()
{
    if( ( ePluginTypeInvalid != getPluginType() ) && ( ePluginTypeInvalid != m_PluginSetting.getPluginType() ) )
    {
        saveUiToSetting();
        m_MyApp.getEngine().getPluginSettingMgr().setPluginSetting( m_PluginSetting );
    }
}

//============================================================================
void AppletSettingsBase::loadUiFromSetting()
{
    if( ePluginTypeInvalid != getPluginType() )
    {
        GuiHelpers::pluginSettingsToWidget( getPluginType(), m_PluginSetting, getPluginSettingsWidget() );
    }
}

//============================================================================
void AppletSettingsBase::saveUiToSetting()
{
    if( ePluginTypeInvalid != getPluginType() )
    {
        GuiHelpers::widgetToPluginSettings( getPluginType(), getPluginSettingsWidget(), m_PluginSetting );
    }
}

//============================================================================
void AppletSettingsBase::slotApplyServiceSettings()
{
    saveUiToSetting();
    m_MyApp.getEngine().getPluginSettingMgr().setPluginSetting( m_PluginSetting );

    EFriendState newPermissionLevel = getPluginSettingsWidget()->getPermissionWidget()->getPermissionLevel();
    EFriendState newConnectionTestPermission = getConnectionTestWidget()->getPermissionLevel();
    if( newPermissionLevel != m_OrigPermissionLevel )
    {
        m_MyApp.getEngine().setPluginPermission( getPluginSettingsWidget()->getPermissionWidget()->getPluginType(), newPermissionLevel );
    }

    if( newConnectionTestPermission != m_OrigConnectTestPermission )
    {
        m_MyApp.getEngine().setPluginPermission( getConnectionTestWidget()->getPluginType(), newConnectionTestPermission );
    }

    savePluginSetting();
    QMessageBox::information( this, QObject::tr( "Service Settings" ), QObject::tr( "Service Settings Applied" ), QMessageBox::Ok );
}
