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


#include "PermissionListItem.h"
#include "AppCommon.h"
#include "AppGlobals.h"
#include "ActivityInformation.h"
#include "AppletBase.h"
#include "GuiHelpers.h"
#include "GuiParams.h"

#include <ptop_src/ptop_engine_src/P2PEngine/P2PEngine.h>
#include <ptop_src/ptop_engine_src/PluginSettings/PluginSetting.h>

//============================================================================
PermissionListItem::PermissionListItem( QListWidget * parent, AppletBase *parentApplet )
: QWidget( parent )
, m_MyApp( GetAppInstance() )
, m_ParentApplet( parentApplet )
, m_SizeHint( parent ? parent->geometry().width() : 200, GuiParams::getButtonSize(eButtonSizeMedium).height() + 8 )
{
    m_ParentApplet = GuiHelpers::findParentApplet( parent );
    ui.setupUi( this );
    setSizeHint( m_SizeHint );
    ui.m_PluginInfoButton->setIcon( eMyIconInformation );
    fillPermissionComboBox();
}

//============================================================================
void PermissionListItem::initPermissionListItem( EPluginType pluginType, int subType )
{
    if( pluginType != ePluginTypeInvalid )
    {
        m_PluginType = pluginType;
        m_SubPluginType = subType;

        if( !m_OrigPermissionIsSet )
        {
            m_OrigPermissionIsSet = true;
            m_OrigPermissionLevel = m_MyApp.getMyIdentity()->getPluginPermission( m_PluginType );;
            setPermissionLevel( m_OrigPermissionLevel );
        }

        fillPluginInfo();
        updateUi();
    }
}

//============================================================================
void PermissionListItem::fillPermissionComboBox( void )
{
    GuiHelpers::fillPermissionComboBox( ui.m_PermissionComboBox );
    ui.m_PermissionComboBox->setCurrentIndex( 4 );
}

//============================================================================
void PermissionListItem::fillPluginInfo()
{
    if( m_PluginType != ePluginTypeInvalid )
    {
        ui.m_PluginRunButton->setIcon( m_MyApp.getMyIcons().getPluginIcon( m_PluginType ) );
        ui.m_PluginSettingsButton->setIcon( m_MyApp.getMyIcons().getPluginSettingsIcon( m_PluginType ) );

        QString pluginName = GuiParams::describePluginType( m_PluginType );
        ui.m_ServiceNameLabel->setText( pluginName );

        PluginSetting pluginSetting;
        m_MyApp.getEngine().getPluginSetting( m_PluginType, pluginSetting );
        QString pluginDesc = pluginSetting.getDescription().c_str();
        ui.m_ServiceDescLabel->setText( pluginName );
    }
}

//============================================================================
void PermissionListItem::updateUi( void )
{
    if( m_PluginType == ePluginTypeInvalid )
    {
        return;
    }

    EFriendState pluginPermission = m_MyApp.getMyIdentity()->getPluginPermission( m_PluginType );
    if( !m_OrigPermissionIsSet )
    {
        m_OrigPermissionIsSet = true;
        m_OrigPermissionLevel = pluginPermission;
    }

    ui.m_PermissionComboBox->setCurrentIndex( GuiHelpers::friendStateToComboIdx( pluginPermission ) );
    if( !m_PermissionsConnected )
    {
        m_PermissionsConnected = true;
        connect( ui.m_PermissionButton, SIGNAL( clicked() ), this, SLOT( slotShowPermissionInformation() ) );
        connect( ui.m_PermissionComboBox, SIGNAL( currentIndexChanged( int ) ), this, SLOT( slotHandleSelectionChanged( int ) ) );
        connect( ui.m_PluginRunButton, SIGNAL( clicked() ), this, SLOT( slotRunPlugin() ) );
        connect( ui.m_PluginSettingsButton, SIGNAL( clicked() ), this, SLOT( slotSetupPlugin() ) );
        connect( ui.m_PluginInfoButton, SIGNAL( clicked() ), this, SLOT( slotShowPluginInformation() ) );
    }

    updatePermissionIcon();
}

//============================================================================
void PermissionListItem::updatePermissionIcon( void )
{
    if( m_PluginType == ePluginTypeInvalid )
    {
        return;
    }

    ui.m_PermissionButton->setIcon( m_MyApp.getMyIcons().getFriendshipIcon( GuiHelpers::comboIdxToFriendState( ui.m_PermissionComboBox->currentIndex() ) ) );
}

//============================================================================
void PermissionListItem::slotHandleSelectionChanged( int )
{
    if( m_PluginType == ePluginTypeInvalid )
    {
        return;
    }

    EFriendState pluginPermission = GuiHelpers::comboIdxToFriendState( ui.m_PermissionComboBox->currentIndex() );
    m_MyApp.getAppGlobals().updatePluginPermission( m_PluginType, pluginPermission );
    updatePermissionIcon();
}

//============================================================================
void PermissionListItem::setPermissionLevel( EFriendState permLevel )
{
    if( m_PluginType == ePluginTypeInvalid )
    {
        return;
    }

    ui.m_PermissionComboBox->setCurrentIndex( GuiHelpers::friendStateToComboIdx( permLevel ) );
}

//============================================================================
EFriendState PermissionListItem::getPermissionLevel( void )
{
    if( m_PluginType == ePluginTypeInvalid )
    {
        return eFriendStateIgnore;
    }

    return GuiHelpers::comboIdxToFriendState( ui.m_PermissionComboBox->currentIndex() );
}

//============================================================================
void PermissionListItem::slotShowPermissionInformation()
{
    ActivityInformation * activityInfo = new ActivityInformation( m_MyApp, this, eInfoTypePermission );
    activityInfo->show();
}

//============================================================================
void PermissionListItem::slotShowPluginInformation()
{
    ActivityInformation * activityInfo = new ActivityInformation( m_MyApp, this, m_PluginType );
    activityInfo->show();
}

//============================================================================
void PermissionListItem::slotRunPlugin()
{
    m_MyApp.launchApplet( GuiHelpers::pluginTypeToUserApplet( m_PluginType ), m_ParentApplet->getParentPageFrame() );
}

//============================================================================
void PermissionListItem::slotSetupPlugin()
{
    m_MyApp.launchApplet( GuiHelpers::pluginTypeToSettingsApplet( m_PluginType ), m_ParentApplet->getParentPageFrame() );
}
