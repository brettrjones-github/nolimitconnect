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
#include "AppletSettingsHostNetwork.h"
#include "AppCommon.h"
#include "AppSettings.h"
#include "MyIcons.h"
#include "GuiHelpers.h"

#include <CoreLib/VxDebug.h>
#include <CoreLib/VxTime.h>

//============================================================================
AppletSettingsHostNetwork::AppletSettingsHostNetwork( AppCommon& app, QWidget * parent )
: AppletBase( OBJNAME_APPLET_SETTINGS_HOST_NETWORK, app, parent )
{
    ui.setupUi( getContentItemsFrame() );
    setAppletType( eAppletSettingsHostNetwork );
    setTitleBarText( DescribeApplet( m_EAppletType ) );
    getPluginSettingsWidget()->setupSettingsWidget( eAppletSettingsHostNetwork, ePluginTypeNetworkHost );
    getPluginSettingsWidget()->getPermissionWidget()->getPluginRunButton()->setVisible( false );
    getPluginSettingsWidget()->getPermissionWidget()->getPluginSettingsButton()->setVisible( false );
    getGroupListingWidget()->setPluginType( ePluginTypeNetworkSearchList );
    getConnectionTestWidget()->setPluginType( ePluginTypeConnectTestHost );
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
        m_OrigGroupListPermission = m_MyApp.getEngine().getPluginPermission( getGroupListingWidget()->getPluginType() );
        m_OrigConnectTestPermission = m_MyApp.getEngine().getPluginPermission( getConnectionTestWidget()->getPluginType() );

        getPluginSettingsWidget()->getPermissionWidget()->setPermissionLevel( m_OrigPermissionLevel );
        getGroupListingWidget()->setPermissionLevel( m_OrigGroupListPermission );
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
        m_MyApp.getEngine().getPluginSettingMgr().setPluginSetting( m_PluginSetting );
    }
}

//============================================================================
void AppletSettingsHostNetwork::loadUiFromSetting()
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
void AppletSettingsHostNetwork::saveUiToSetting()
{
    if( ePluginTypeInvalid != getPluginType() )
    {
        m_PluginSetting.setContentRating( ( EContentRating )getPluginSettingsWidget()->getContentRatingComboBox()->currentIndex() );
        m_PluginSetting.setLanguage( ( ELanguageType )getPluginSettingsWidget()->getLanguageComboBox()->currentIndex() );
        m_PluginSetting.setPluginUrl( getPluginSettingsWidget()->getServiceUrlEdit()->text().toUtf8().constData() );
        m_PluginSetting.setTitle( getPluginSettingsWidget()->getServiceNameEdit()->text().toUtf8().constData() );
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
    }
}

//============================================================================
void AppletSettingsHostNetwork::slotApplyServiceSettings()
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

    if( newGroupListPermission != m_OrigGroupListPermission )
    {
        m_MyApp.getEngine().setPluginPermission( getGroupListingWidget()->getPluginType(), newGroupListPermission );
    }

    if( newConnectionTestPermission != m_OrigConnectTestPermission )
    {
        m_MyApp.getEngine().setPluginPermission( getConnectionTestWidget()->getPluginType(), newConnectionTestPermission );
    }

    QMessageBox::information( this, QObject::tr( "Service Settings" ), QObject::tr( "Service Settings Applied" ), QMessageBox::Ok );
}
