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


#include "RelayServiceWidget.h"
#include "AppCommon.h"
#include "AppGlobals.h"
#include "ActivityInformation.h"
#include "GuiHelpers.h"
#include "GuiParams.h"

#include <ptop_src/ptop_engine_src/P2PEngine/P2PEngine.h>

//============================================================================
RelayServiceWidget::RelayServiceWidget( QWidget * parent )
	: QWidget( parent )
    , m_MyApp( GetAppInstance() )
{
	initRelayServiceWidget();
}

//============================================================================
void RelayServiceWidget::initRelayServiceWidget( void )
{
	ui.setupUi( this );
    ui.m_PermissionInfoButton->setIcon( eMyIconInformation );
    fillPermissionComboBox();
    if( m_PluginType != ePluginTypeInvalid )
    {
        updateUi();
    }
}

//============================================================================
void RelayServiceWidget::fillPermissionComboBox( void )
{
    ui.m_PermissionComboBox->addItem( GuiParams::describePermissionLevel( eFriendStateAdmin ) );
    ui.m_PermissionComboBox->addItem( GuiParams::describePermissionLevel( eFriendStateFriend ) );
    ui.m_PermissionComboBox->addItem( GuiParams::describePermissionLevel( eFriendStateGuest ) );
    ui.m_PermissionComboBox->addItem( GuiParams::describePermissionLevel( eFriendStateAnonymous ) );
    ui.m_PermissionComboBox->addItem( GuiParams::describePermissionLevel( eFriendStateIgnore ) );
    ui.m_PermissionComboBox->setCurrentIndex( 4 );
}

//============================================================================
void RelayServiceWidget::updateUi( void )
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
        connect( ui.m_PermissionInfoButton, SIGNAL( clicked() ), this, SLOT( slotShowPermissionInformation() ) );
        connect( ui.m_PermissionButton, SIGNAL( clicked() ), this, SLOT( slotShowPermissionInformation() ) );
        connect( ui.m_PermissionComboBox, SIGNAL( currentIndexChanged( int ) ), this, SLOT( slotHandleSelectionChanged( int ) ) );
        connect( ui.m_PluginInfoButton, SIGNAL( clicked() ), this, SLOT( slotShowPluginInformation() ) );
    }

    updatePermissionIcon();
}

//============================================================================
void RelayServiceWidget::setPluginType( EPluginType pluginType, int subType ) 
{ 
    if( pluginType == ePluginTypeInvalid )
    {
        return;
    }

    m_PluginType = pluginType;
    m_SubPluginType = subType;

    if( !m_OrigPermissionIsSet )
    {
        m_OrigPermissionIsSet = true;
        m_OrigPermissionLevel = m_MyApp.getMyIdentity()->getPluginPermission( m_PluginType );;
        setPermissionLevel( m_OrigPermissionLevel );
    }

    updateUi();  
    updatePermissionIcon(); 
}

//============================================================================
void RelayServiceWidget::updatePermissionIcon( void )
{
    if( m_PluginType == ePluginTypeInvalid )
    {
        return;
    }

    ui.m_PermissionButton->setIcon( m_MyApp.getMyIcons().getFriendshipIcon( GuiHelpers::comboIdxToFriendState( ui.m_PermissionComboBox->currentIndex() ) ) );
    ui.m_PluginInfoButton->setIcon( m_MyApp.getMyIcons().getPluginIcon( m_PluginType ) );
}

//============================================================================
void RelayServiceWidget::slotHandleSelectionChanged( int )
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
void RelayServiceWidget::slotShowPermissionInformation()
{
    ActivityInformation * activityInfo = new ActivityInformation( m_MyApp, this, eInfoTypePermission );
    activityInfo->show();
}

//============================================================================
void RelayServiceWidget::slotShowPluginInformation()
{
    ActivityInformation * activityInfo = new ActivityInformation( m_MyApp, this, m_PluginType );
    activityInfo->show();
}

//============================================================================
void RelayServiceWidget::setPermissionLevel( EFriendState permLevel )
{
    if( m_PluginType == ePluginTypeInvalid )
    {
        return;
    }

    ui.m_PermissionComboBox->setCurrentIndex( GuiHelpers::friendStateToComboIdx( permLevel ) );
}

//============================================================================
EFriendState RelayServiceWidget::getPermissionLevel( void )
{
    if( m_PluginType == ePluginTypeInvalid )
    {
        return eFriendStateIgnore;
    }

    return GuiHelpers::comboIdxToFriendState( ui.m_PermissionComboBox->currentIndex() );
}
