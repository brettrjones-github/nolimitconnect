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


#include "AppletHostNetworkStatus.h"
#include "AppCommon.h"
#include "AppSettings.h"
#include "MyIcons.h"
#include "GuiHelpers.h"

#include <CoreLib/VxDebug.h>

//============================================================================
AppletHostNetworkStatus::AppletHostNetworkStatus( AppCommon& app, QWidget* parent )
    : AppletBase( OBJNAME_APPLET_HOST_NETWORK_STATUS, app, parent )
    , m_UpdateStatusTimer( new QTimer( this ) )
{
    ui.setupUi( getContentItemsFrame() );
    setAppletType( eAppletHostNetworkStatus );
    setTitleBarText( DescribeApplet( m_EAppletType ) );

    ui.m_HostingRequirementsButton->setVisible( false );
    getNetworkHostPermissionWidget()->setPluginType( ePluginTypeHostNetwork );
    getConnectionTestWidget()->setPluginType( ePluginTypeHostConnectTest );
    getRandomConnectPermissionWidget()->setPluginType( ePluginTypeHostRandomConnect );
    getGroupHostPermissionWidget()->setPluginType( ePluginTypeHostGroup );

    ui.m_OpenPortCheckBox->setEnabled( false );
    ui.m_HostPermissionCheckBox->setEnabled( false );
    ui.m_ConnectionTestPermissionCheckBox->setEnabled( false );

    ui.m_OptionalLabel->setVisible( false );
    ui.m_RandomConnectPermissionWidget->setVisible( false );

    ui.m_AdditionalLabel1->setVisible( false );
    ui.m_AdditionalLabel2->setVisible( false );
    ui.m_AdditionalPermissionWidget->setVisible( false );

    m_MyApp.activityStateChange( this, true );

    connect( ui.m_HostingRequirementsButton, SIGNAL( clicked() ), this, SLOT( slotHostRequirementsButtonClicked() ) );
    connect( m_UpdateStatusTimer, SIGNAL( timeout() ), this, SLOT( slotUpdateStatusTimeout() ) );
    m_UpdateStatusTimer->start( 3000 );
    slotUpdateStatusTimeout();
}

//============================================================================
AppletHostNetworkStatus::~AppletHostNetworkStatus()
{
    m_UpdateStatusTimer->stop();
    m_MyApp.activityStateChange( this, false );
}

//============================================================================
void AppletHostNetworkStatus::slotHostRequirementsButtonClicked()
{
}

//============================================================================
void AppletHostNetworkStatus::slotUpdateStatusTimeout()
{
    bool haveOpenPort = m_MyApp.getEngine().getNetStatusAccum().isRxPortOpen();
    bool networkHostEnabled = m_MyApp.getAppGlobals().getUserIdent()->getPluginPermission( ePluginTypeHostNetwork ) != eFriendStateIgnore;
    bool connectTestEnabled = m_MyApp.getAppGlobals().getUserIdent()->getPluginPermission( ePluginTypeHostConnectTest ) != eFriendStateIgnore;
    ui.m_OpenPortCheckBox->setChecked( haveOpenPort );
    ui.m_HostPermissionCheckBox->setChecked( networkHostEnabled );
    ui.m_ConnectionTestPermissionCheckBox->setChecked( connectTestEnabled );
    if( !haveOpenPort )
    {
        ui.m_HostingStatusText->setText( QObject::tr( "Open Port Required. Check Network Settings" ) );
    }
    else if( !networkHostEnabled )
    {
        ui.m_HostingStatusText->setText( QObject::tr( "Network Hosting Permission is disabled" ) );
    }
    else if( !connectTestEnabled )
    {
        ui.m_HostingStatusText->setText( QObject::tr( "Connection Test Permission is disabled" ) );
    }
    else
    {
        ui.m_HostingStatusText->setText( QObject::tr( "Network Hosting Conditions Are Met" ) );
    }

    int availGroupsCnt = m_MyApp.getFromGuiInterface().fromGuiGetJoinedListCount( ePluginTypeNetworkSearchList );
    ui.m_GroupListCountLabel->setText( QString::number( availGroupsCnt ) );
    std::string url;
    m_MyApp.getFromGuiInterface().fromGuiGetNodeUrl( url );
    ui.m_UrlText->setText( url.c_str() );
}
