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
#include "AppletSettingsHostChatRoom.h"
#include "AppCommon.h"
#include "AppSettings.h"
#include "MyIcons.h"
#include "GuiHelpers.h"

#include <CoreLib/VxDebug.h>

//============================================================================
AppletSettingsHostChatRoom::AppletSettingsHostChatRoom( AppCommon& app, QWidget * parent )
    : AppletSettingsBase( OBJNAME_APPLET_SETTINGS_HOST_CHAT_ROOM, app, parent )
{
    ui.setupUi( getContentItemsFrame() );
    setAppletType( eAppletSettingsHostChatRoom );
    setPluginType( ePluginTypeChatRoomHost );
    setTitleBarText( DescribeApplet( m_EAppletType ) );
    getPluginSettingsWidget()->setupSettingsWidget( eAppletSettingsHostChatRoom, ePluginTypeChatRoomHost );
    getPluginSettingsWidget()->getPermissionWidget()->getPluginRunButton()->setVisible( false );
    getPluginSettingsWidget()->getPermissionWidget()->getPluginSettingsButton()->setVisible( false );
    getConnectionTestWidget()->setPluginType( ePluginTypeConnectTestHost );
    connectServiceWidgets();
    loadPluginSetting();

    m_MyApp.activityStateChange( this, true );
}

//============================================================================
AppletSettingsHostChatRoom::~AppletSettingsHostChatRoom()
{
    m_MyApp.activityStateChange( this, false );
}

//============================================================================
void AppletSettingsHostChatRoom::connectServiceWidgets()
{
    connect( getPluginSettingsWidget()->getApplyButton(), SIGNAL( clicked() ), this, SLOT( slotApplyServiceSettings() ) );
}

//============================================================================
void AppletSettingsHostChatRoom::loadPluginSetting()
{
    if( ePluginTypeInvalid != getPluginType() )
    {
        m_OrigPermissionLevel = m_MyApp.getAppGlobals().getUserIdent()->getPluginPermission( getPluginType() );
        m_OrigConnectTestPermission = m_MyApp.getAppGlobals().getUserIdent()->getPluginPermission( getConnectionTestWidget()->getPluginType() );
        getPluginSettingsWidget()->getPermissionWidget()->setPermissionLevel( m_OrigPermissionLevel );
        getConnectionTestWidget()->setPermissionLevel( m_OrigConnectTestPermission );

        m_PluginSetting.setPluginType( getPluginType() );// must set before get settings so engine will know which plugin setting to get
        m_MyApp.getEngine().getPluginSettingMgr().getPluginSetting( getPluginType(), m_PluginSetting );
        loadUiFromSetting();
    }
}

//============================================================================
void AppletSettingsHostChatRoom::savePluginSetting()
{
    if( ePluginTypeInvalid != getPluginType() )
    {
        saveUiToSetting();
        m_PluginSetting.setPluginType( getPluginType() );
        m_MyApp.getEngine().getPluginSettingMgr().setPluginSetting( m_PluginSetting );
    }
}

//============================================================================
void AppletSettingsHostChatRoom::slotApplyServiceSettings()
{
    saveUiToSetting();
    if( verifyPluginSettings() )
    {
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
}

//============================================================================
bool AppletSettingsHostChatRoom::verifyPluginSettings()
{
    bool settingsOk = true;
    if( m_PluginSetting.getTitle().size() < 3 )
    {
        QMessageBox::warning( this, QObject::tr( "Chat Room Title" ), QObject::tr( "Chat Room Title must be at least 3 charcters long (title is used in user searches)" ), QMessageBox::Ok );
        settingsOk = false;
    }
    
    return settingsOk && ( ePluginTypeInvalid != getPluginType() );
}