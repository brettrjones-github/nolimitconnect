//============================================================================
// Copyright (C) 2019 Brett R. Jones
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

#include "AppletServiceBaseSettings.h"
#include "AppCommon.h"
#include "AppSettings.h"
#include "MyIcons.h"
#include "GuiHelpers.h"
#include "GuiParams.h"

#include <CoreLib/VxDebug.h>

//============================================================================
AppletServiceBaseSettings::AppletServiceBaseSettings( const char * objName, AppCommon& app, QWidget * parent )
    : AppletBase( objName, app, parent )
{
}

//============================================================================
void AppletServiceBaseSettings::setupServiceBaseApplet( EApplet applet, EPluginType pluginType )
{
    setAppletType( applet );
    setPluginType( pluginType );
    ui.setupUi( getContentItemsFrame() );
    setTitleBarText( DescribeApplet( m_EAppletType ) );

    getServiceUrlLabel()->setVisible( false );
    getServiceUrlEdit()->setVisible( false );
    getInformationWidget()->setVisible( false );

    if( ePluginTypeInvalid != getPluginType() )
    {
        getPermissionWidget()->setPluginType( getPluginType() );
        getServiceTitle()->setText( GuiParams::describePlugin( getPluginType(), false ).c_str() );
        loadPluginSetting();
    }

    connectServiceWidgets();

    ui.m_DescriptionEdit->setFixedHeight( GuiParams::getButtonSize( eButtonSizeMedium ).height() );
}

//============================================================================
void AppletServiceBaseSettings::connectServiceWidgets()
{
    connect( getApplyButton(), SIGNAL( clicked() ), this, SLOT( slotApplyButtonClicked() ) );
}

//============================================================================
void AppletServiceBaseSettings::loadPluginSetting()
{
    if( ePluginTypeInvalid != getPluginType() )
    {
        m_OrigPermissionLevel = m_MyApp.getAppGlobals().getUserIdent()->getPluginPermission( getPluginType() );
        getPermissionWidget()->setPermissionLevel( m_OrigPermissionLevel );
        m_MyApp.getEngine().getPluginSettingMgr().getPluginSetting( getPluginType(), m_PluginSetting );
        loadUiFromSetting();
    }
}

//============================================================================
void AppletServiceBaseSettings::savePluginSetting()
{
    if( ePluginTypeInvalid != getPluginType() )
    {
        saveUiToSetting();
        m_MyApp.getEngine().getPluginSettingMgr().setPluginSetting( m_PluginSetting );
    }
}

//============================================================================
void AppletServiceBaseSettings::loadUiFromSetting()
{
    if( ePluginTypeInvalid != getPluginType() )
    {

    }
}

//============================================================================
void AppletServiceBaseSettings::saveUiToSetting()
{
    if( ePluginTypeInvalid != getPluginType() )
    {
    }
}

//============================================================================
void AppletServiceBaseSettings::slotApplyButtonClicked()
{
    saveUiToSetting();
    m_MyApp.getEngine().getPluginSettingMgr().setPluginSetting( m_PluginSetting );
}