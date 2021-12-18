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
// http://www.nolimitconnect.com
//============================================================================


#include "ServiceSettingsWidget.h"
#include "AppletBase.h"
#include "AppCommon.h"
#include "AppGlobals.h"
#include "AppletMgr.h"
#include "GuiHelpers.h"
#include "GuiParams.h"

#include "ActivityInformation.h"
#include "GuiHelpers.h"

#include <ptop_src/ptop_engine_src/P2PEngine/P2PEngine.h>

//============================================================================
ServiceSettingsWidget::ServiceSettingsWidget( QWidget * parent )
	: QWidget( parent )
    , m_MyApp( GetAppInstance() )
{
	initServiceSettingsWidget();
}

//============================================================================
void ServiceSettingsWidget::initServiceSettingsWidget( void )
{
	ui.setupUi( this );

    connect( ui.m_SettingsButton, SIGNAL( clicked() ), this, SLOT( slotServiceSettingsClicked() ) );
    connect( ui.m_ViewServiceButton, SIGNAL( clicked() ), this, SLOT( slotViewServiceClicked() ) );
}

//============================================================================
void ServiceSettingsWidget::updateUi( void )
{
    if( m_PluginType == ePluginTypeInvalid )
    {
        return;
    }

    updateIcons();
}

//============================================================================
void ServiceSettingsWidget::updateIcons( void )
{
    if( m_PluginType == ePluginTypeInvalid )
    {
        return;
    }

    ui.m_SettingsButton->setIcon( m_MyApp.getMyIcons().getPluginSettingsIcon( m_PluginType ) );
    QString settingsText = GuiParams::describePlugin( m_PluginType, false ).c_str();
    settingsText += QObject::tr( "Settings" );
    ui.m_SettingsLabel->setText( settingsText );


    ui.m_ViewServiceButton->setIcon( m_MyApp.getMyIcons().getPluginIcon( m_PluginType ) );
    QString viewText = QObject::tr( "View " ) + GuiParams::describePlugin( m_PluginType, false ).c_str();
    ui.m_ViewServiceLabel->setText( viewText );

    EApplet viewAppletType = GuiHelpers::pluginTypeToViewApplet( m_PluginType );
    if( eAppletUnknown == viewAppletType )
    {
        ui.m_ViewServiceFrame->setVisible( false );
    }
}

//============================================================================
void ServiceSettingsWidget::slotServiceSettingsClicked()
{
    launchApplet( GuiHelpers::pluginTypeToSettingsApplet( m_PluginType ) );
}

//============================================================================
void ServiceSettingsWidget::slotViewServiceClicked()
{
    launchApplet( GuiHelpers::pluginTypeToViewApplet( m_PluginType ) );
}

//============================================================================
void ServiceSettingsWidget::launchApplet( EApplet appletType )
{
    if( eAppletUnknown != appletType )
    {
        AppletBase * parentApplet = GuiHelpers::findParentApplet( this );
        if( parentApplet )
        {
            m_MyApp.getAppletMgr().launchApplet( appletType, parentApplet );
        }
        else
        {
            QString msgText = QObject::tr( "Unable to determine parent" );
            QMessageBox::information( this, msgText, msgText );
        }
    }
    else
    {
        QString msgText = QObject::tr( "Unknown Applet " );
        QMessageBox::information( this, msgText, msgText );
    }
}

