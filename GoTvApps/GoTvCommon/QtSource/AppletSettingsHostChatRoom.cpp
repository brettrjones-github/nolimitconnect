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
    : AppletBase( OBJNAME_APPLET_SETTINGS_HOST_CHAT_ROOM, app, parent )
{
    ui.setupUi( getContentItemsFrame() );
    setAppletType( eAppletSettingsHostChatRoom );
    setPluginType( ePluginTypeChatRoomHost );
    setTitleBarText( DescribeApplet( m_EAppletType ) );
    getPluginSettingsWidget()->setupSettingsWidget( eAppletSettingsHostChatRoom, ePluginTypeChatRoomHost );
    getPluginSettingsWidget()->getPermissionWidget()->getPluginRunButton()->setVisible( false );
    getPluginSettingsWidget()->getPermissionWidget()->getPluginSettingsButton()->setVisible( false );
    getRelayWidget()->setPluginType( ePluginTypeRelay );
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
        m_OrigRelayPermission = m_MyApp.getAppGlobals().getUserIdent()->getPluginPermission( getRelayWidget()->getPluginType());
        m_OrigConnectTestPermission = m_MyApp.getAppGlobals().getUserIdent()->getPluginPermission( getConnectionTestWidget()->getPluginType() );
        getPluginSettingsWidget()->getPermissionWidget()->setPermissionLevel( m_OrigPermissionLevel );
        getRelayWidget()->setPermissionLevel( m_OrigRelayPermission );
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
void AppletSettingsHostChatRoom::loadUiFromSetting()
{
    if( ePluginTypeInvalid != getPluginType() )
    {
        getPluginSettingsWidget()->getContentRatingComboBox()->setCurrentIndex( GuiHelpers::contentRatingToIndex( m_PluginSetting.getContentRating() ) );
        getPluginSettingsWidget()->getLanguageComboBox()->setCurrentIndex( GuiHelpers::languageToIndex( m_PluginSetting.getLanguage() ) );
        getPluginSettingsWidget()->getServiceUrlEdit()->setText( m_PluginSetting.getPluginUrl().c_str() );
        getPluginSettingsWidget()->getServiceNameEdit()->setText( m_PluginSetting.getTitle().c_str() );
        getPluginSettingsWidget()->getServiceDescriptionEdit()->appendPlainText( m_PluginSetting.getDescription().c_str() );
        getPluginSettingsWidget()->getThumbnailChooseWidget()->loadThumbnail( m_PluginSetting.getThumnailId() );
    }
}

//============================================================================
void AppletSettingsHostChatRoom::saveUiToSetting()
{
    if( ePluginTypeInvalid != getPluginType() )
    {
        m_PluginSetting.setPluginType( getPluginType() );
        m_PluginSetting.setContentRating( ( EContentRating )getPluginSettingsWidget()->getContentRatingComboBox()->currentIndex() );
        m_PluginSetting.setLanguage( ( ELanguageType )getPluginSettingsWidget()->getLanguageComboBox()->currentIndex() );
        m_PluginSetting.setPluginUrl( getPluginSettingsWidget()->getServiceUrlEdit()->text().toUtf8().constData() );
        std::string serviceName = getPluginSettingsWidget()->getServiceNameEdit()->text().toUtf8().constData();
        m_PluginSetting.setTitle( serviceName.c_str() );
        VxGUID thumbId = getPluginSettingsWidget()->getThumbnailChooseWidget()->updateAndGetThumbnailId();
        m_PluginSetting.setThumnailId( getPluginSettingsWidget()->getThumbnailChooseWidget()->updateAndGetThumbnailId() );

        QString description = getPluginSettingsWidget()->getServiceDescriptionEdit()->toPlainText().trimmed();
        if( !description.isEmpty() )
        {
            m_PluginSetting.setDescription( description.toUtf8().constData() );
        }
        else
        {
            m_PluginSetting.setDescription( "" );
        }

        LogMsg( LOG_DEBUG, "chat settings title %s thumb id %s", serviceName.c_str(), thumbId.toGUIDStandardFormatedString().c_str() );
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
        EFriendState newRelayPermission = getRelayWidget()->getPermissionLevel();
        EFriendState newConnectionTestPermission = getConnectionTestWidget()->getPermissionLevel();
        if( newPermissionLevel != m_OrigPermissionLevel )
        {
            m_MyApp.getEngine().setPluginPermission( getPluginSettingsWidget()->getPermissionWidget()->getPluginType(), newPermissionLevel );
        }

        if( newRelayPermission != m_OrigRelayPermission )
        {
            m_MyApp.getEngine().setPluginPermission( getRelayWidget()->getPluginType(), newRelayPermission );
        }

        if( newConnectionTestPermission != m_OrigConnectTestPermission )
        {
            m_MyApp.getEngine().setPluginPermission( getConnectionTestWidget()->getPluginType(), newConnectionTestPermission );
        }

        m_MyApp.getEngine().setPluginPermission( ePluginTypeRelay, getRelayWidget()->getPermissionLevel() );
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