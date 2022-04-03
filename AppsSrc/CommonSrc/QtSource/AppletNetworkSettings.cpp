//============================================================================
// Copyright (C) 2010 Brett R. Jones
// Issued to MIT style license by Brett R. Jones in 2017
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

#include "AppletNetworkSettings.h"
#include "ActivityIsPortOpenTest.h"
#include "ActivityNetworkState.h"
#include "ActivityInformation.h"

#include "AppGlobals.h"
#include "AppCommon.h"
#include "AppSettings.h"
#include "MyIcons.h"
#include "AccountMgr.h"

#include <CoreLib/VxDebug.h>
#include <NetLib/NetHostSetting.h>
#include <NetLib/VxGetRandomPort.h>
#include <NetLib/VxSktUtil.h>

namespace
{
    QString				DEFAULT_ADAPTER_IP_CHOICE = "default";
}

//============================================================================
AppletNetworkSettings::AppletNetworkSettings( AppCommon& app, QWidget * parent )
: AppletBase( OBJNAME_APPLET_NETWORK_SETTINGS, app, parent )
, m_UpdateTimer( new QTimer(this) )
{
	setAppletType( eAppletNetworkSettings );
	ui.setupUi( getContentItemsFrame() );
	setTitleBarText( DescribeApplet( m_EAppletType ) );

    // opening a port in network router using upnp rarely works.. just make invisible for now
    ui.m_UseUpnpCheckBox->setVisible( false );
    // using an specific network adapter ip for outgoing connections has some issues with vpn on some systems.. hide for not
    ui.m_LclIpListComboBox->setVisible(false);
    ui.m_LocalAdapterIpLabel->setVisible(false);

    // default hosts are now handled by last connected.. these could be removed
    ui.m_DefaultGroupHostButton->setVisible( false );
    ui.m_GroupHostUrlLabel->setVisible( false );
    ui.m_GroupHostUrlEdit->setVisible( false );
    ui.m_DefaultGroupHostUrlInfoButton->setVisible( false );

    ui.m_DefaultChatRoomHostButton->setVisible( false );
    ui.m_DefaultChatRoomHostUrlLabel->setVisible( false );
    ui.m_DefaultChatRoomHostUrlEdit->setVisible( false );
    ui.m_ChatRoomUrlInfoButton->setVisible( false );

    ui.m_RandomConnectButton->setVisible( false );
    ui.m_RandomConnectUrlLabel->setVisible( false );
    ui.m_RandomConnectUrlEdit->setVisible( false );
    ui.m_RandomConnectUrlInfoButton->setVisible( false );

    //ui.m_DefaultGroupHostUrlInfoButton->setIcon( eMyIconInformation );
    //ui.m_RandomConnectUrlInfoButton->setIcon( eMyIconInformation );
    //ui.m_ChatRoomUrlInfoButton->setIcon( eMyIconInformation );

    //ui.m_DefaultGroupHostButton->setIcon( eMyIconServiceHostGroup );
    //ui.m_RandomConnectButton->setIcon( eMyIconServiceRandomConnect );
    //ui.m_DefaultChatRoomHostButton->setIcon( eMyIconServiceChatRoom );

    // probably could remove this button
    ui.m_ApplySettingsButton->setVisible( false );


    updateDlgFromSettings( true );

    connectSignals();

	m_MyApp.activityStateChange( this, true );
    fillMyNodeUrl( ui.m_NodeUrlLabel );
    m_UpdateTimer->setInterval( 2000 );
    m_UpdateTimer->start();
}

//============================================================================
AppletNetworkSettings::~AppletNetworkSettings()
{
    m_MyApp.activityStateChange( this, false );
}

//============================================================================
void AppletNetworkSettings::connectSignals( void )
{
    connect( ui.m_NetworkHostButton, SIGNAL( clicked() ), this, SLOT( slotShowNetworkHostInformation() ) );
    connect( ui.m_NetworkHostInfoButton, SIGNAL( clicked() ), this, SLOT( slotShowNetworkHostInformation() ) );
    connect( ui.m_NetworkKeyInfoButton, SIGNAL( clicked() ), this, SLOT( slotShowNetworkKeyInformation() ) );
    connect( ui.m_NetworkKeyButton, SIGNAL( clicked() ), this, SLOT( slotShowNetworkKeyInformation() ) );

    connect( ui.AutoDetectProxyRadioButton, SIGNAL( clicked() ), this, SLOT( slotAutoDetectProxyClick() ) );
    connect( ui.AssumeNoProxyRadioButton, SIGNAL( clicked() ), this, SLOT( slotNoProxyClick() ) );
    connect( ui.AssumeProxyRadioButton, SIGNAL( clicked() ), this, SLOT( slotYesProxyClick() ) );

    connect( ui.RandomPortButton, SIGNAL( clicked() ), this, SLOT( slotRandomPortButtonClick() ) );
    connect( ui.m_UseUpnpCheckBox, SIGNAL( clicked() ), this, SLOT( slotUseUpnpCheckBoxClick() ) );
    connect( ui.m_ConnectTestUrlInfoButton, SIGNAL( clicked() ), this, SLOT( slotShowConnectTestUrlInformation() ) );
    connect( ui.m_ConnectIsOpenInfoButton, SIGNAL( clicked() ), this, SLOT( slotShowConnectTestSettingsInformation() ) );
    connect( ui.m_SaveSettingsButton, SIGNAL( clicked() ), this, SLOT( onSaveButtonClick() ) );
    connect( ui.m_DeleteSettingsButton, SIGNAL( clicked() ), this, SLOT( onDeleteButtonClick() ) );
    connect( ui.m_ApplySettingsButton, SIGNAL( clicked() ), this, SLOT( slotApplySettingsButtonClick() ) );
    connect( ui.m_CopyToClipboardButton, SIGNAL( clicked() ), this, SLOT( slotCopyMyUrlToClipboard() ) );
    connect( ui.m_TestIsPortOpenButton, SIGNAL( clicked() ), this, SLOT( slotTestIsMyPortOpenButtonClick() ) );

    connect( ui.m_DefaultChatRoomHostButton, SIGNAL( clicked() ), this, SLOT( slotShowDefaultChatRoomUrlInformation() ) );
    connect( ui.m_ChatRoomUrlInfoButton, SIGNAL( clicked() ), this, SLOT( slotShowDefaultChatRoomUrlInformation() ) );

    connect( ui.m_DefaultGroupHostButton, SIGNAL( clicked() ), this, SLOT( slotShowDefaultGroupHostUrlInformation() ) );
    connect( ui.m_DefaultGroupHostUrlInfoButton, SIGNAL( clicked() ), this, SLOT( slotShowDefaultGroupHostUrlInformation() ) );

    connect( ui.m_RandomConnectButton, SIGNAL( clicked() ), this, SLOT( slotShowDefaultGroupHostUrlInformation() ) );
    connect( ui.m_RandomConnectUrlInfoButton, SIGNAL( clicked() ), this, SLOT( slotShowDefaultGroupHostUrlInformation() ) );

    connect( ui.m_NetworkSettingsNameComboBox, SIGNAL( currentIndexChanged( int ) ), this, SLOT( onComboBoxSelectionChange( int ) ) );
    connect( ui.m_NetworkSettingsNameComboBox, SIGNAL( editTextChanged( const QString& ) ), this, SLOT( onComboBoxTextChanged( const QString& ) ) );

    connect( m_UpdateTimer, SIGNAL( timeout() ), this, SLOT( slotUpdateTimer() ) );
}

//============================================================================
void AppletNetworkSettings::updateDlgFromSettings( bool origSettings )
{
    ui.m_NetworkSettingsNameComboBox->clear();
    ui.AutoDetectProxyRadioButton->setChecked( false );
    ui.AssumeNoProxyRadioButton->setChecked( false );
    ui.AssumeProxyRadioButton->setChecked( false );

    bool validDbSettings = false;
    AccountMgr& dataHelper = m_MyApp.getAccountMgr();
    std::vector<NetHostSetting> netSettingList;
    std::string lastSettingsName = dataHelper.getLastNetHostSettingName();
    int selectedIdx = 0;
    int currentSettingIdx = 0;
    if( ( 0 != lastSettingsName.length() )
        && dataHelper.getAllNetHostSettings( netSettingList )
        && ( 0 != netSettingList.size() ) )
    {
        std::vector<NetHostSetting>::iterator iter;
        for( iter = netSettingList.begin(); iter != netSettingList.end(); ++iter )
        {
            NetHostSetting& netHostSetting = *iter;
            ui.m_NetworkSettingsNameComboBox->addItem( netHostSetting.getNetHostSettingName().c_str() );
            if( netHostSetting.getNetHostSettingName() == lastSettingsName )
            {
                // found last settings used
                selectedIdx = currentSettingIdx;
                validDbSettings = true;
                populateDlgFromNetHostSetting( netHostSetting );
            }

            currentSettingIdx++;
        }
    }

    if( validDbSettings )
    {
        ui.m_NetworkSettingsNameComboBox->setCurrentIndex( selectedIdx );
    }
    else if( netSettingList.size() )
    {
        ui.m_NetworkSettingsNameComboBox->addItem( netSettingList[ 0 ].getNetHostSettingName().c_str() );
        populateDlgFromNetHostSetting( netSettingList[ 0 ] );
        dataHelper.updateLastNetHostSettingName( netSettingList[ 0 ].getNetHostSettingName().c_str() );
    }
    else
    {
        ui.m_NetworkSettingsNameComboBox->addItem( "default" );
        NetHostSetting engineHostSetting;
        fillNetHostSettingFromEngine( engineHostSetting );
        populateDlgFromNetHostSetting( engineHostSetting );        
    }

    if( origSettings )
    {
        m_OriginalNetworkKey = ui.m_NetworkKeyEdit->text();
    }
}

//============================================================================
void AppletNetworkSettings::fillNetHostSettingFromEngine( NetHostSetting& netSetting )
{
    std::string strValue;

    m_Engine.getEngineSettings().getNetworkHostUrl( strValue );
    netSetting.setNetworkHostUrl( strValue.c_str() );

    m_Engine.getEngineSettings().getNetworkKey( strValue );
    netSetting.setNetworkKey( strValue.c_str() );

    m_Engine.getEngineSettings().getConnectTestUrl( strValue );
    netSetting.setConnectTestUrl( strValue.c_str() );

    //m_Engine.getEngineSettings().getGroupHostUrl( strValue );
    //netSetting.setGroupHostUrl( strValue.c_str() );

    //m_Engine.getEngineSettings().getRandomConnectUrl( strValue );
    //netSetting.setRandomConnectUrl( strValue.c_str() );

    //m_Engine.getEngineSettings().getChatRoomHostUrl( strValue );
    //netSetting.setGroupHostUrl( strValue.c_str() );

    std::string externIP;
    m_Engine.getEngineSettings().getUserSpecifiedExternIpAddr( externIP );
    netSetting.setUserSpecifiedExternIpAddr( externIP.c_str() );

    std::string preferredAdapterIP = m_Engine.getEngineSettings().getPreferredNetworkAdapterIp();
    netSetting.setPreferredNetworkAdapterIp( preferredAdapterIP.c_str() );

    uint16_t u16Port = m_Engine.getEngineSettings().getTcpIpPort();
    netSetting.setTcpPort( u16Port );

    EFirewallTestType eFirewallType = m_Engine.getEngineSettings().getFirewallTestSetting();
    int32_t fireWallType = 0;
    switch( eFirewallType )
    {
    case  eFirewallTestAssumeNoFirewall:
        fireWallType = 1;
        break;

    case eFirewallTestAssumeFirewalled:
        fireWallType = 2;
        break;

    case eFirewallTestUrlConnectionTest:
    default:
        fireWallType = 0;
    }

    netSetting.setFirewallTestType( fireWallType );

    bool useUpnp = m_Engine.getEngineSettings().getUseUpnpPortForward();
    netSetting.setUseUpnpPortForward( useUpnp );
}

//============================================================================
void AppletNetworkSettings::slotApplySettingsButtonClick( void )
{
    NetHostSetting netHostSetting;
    populateNetHostSettingsFromDlg( netHostSetting );
    applyEngineSettingsFromHostSetting( netHostSetting );
}

//============================================================================
void AppletNetworkSettings::applyEngineSettingsFromHostSetting( NetHostSetting& netHostSetting )
{
    if( 1 == netHostSetting.getFirewallTestType() && !netHostSetting.getUserSpecifiedExternIpAddr().empty() )
    {
        m_MyApp.getAppGlobals().getUserIdent()->setOnlineIpAddress( netHostSetting.getUserSpecifiedExternIpAddr().c_str() );
    }

    if( m_Engine.getMyNetIdent()->getMyOnlinePort() != netHostSetting.getTcpPort() )
    {
        m_Engine.getMyNetIdent()->setMyOnlinePort( netHostSetting.getTcpPort() );
        m_Engine.setPktAnnLastModTime( GetTimeStampMs() );
    }

    m_Engine.fromGuiApplyNetHostSettings(netHostSetting);
}

//============================================================================
void AppletNetworkSettings::updateSettingsFromDlg()
{
    NetHostSetting netHostSetting;
    populateNetHostSettingsFromDlg( netHostSetting );
    if( netHostSetting != m_OriginalSettings )
    {
        LogMsg( LOG_DEBUG, "AppletNetworkSettings has changed" );

        m_OriginalSettings = netHostSetting;

        m_MyApp.getAccountMgr().updateNetHostSetting( netHostSetting );
        m_MyApp.getAccountMgr().updateLastNetHostSettingName( netHostSetting.getNetHostSettingName().c_str() );
    }
    else
    {
        LogMsg( LOG_DEBUG, "AppletNetworkSettings no change" );
    }
}

//============================================================================
void AppletNetworkSettings::populateNetHostSettingsFromDlg( NetHostSetting& netHostSetting )
{
    // get user name of the setting
    std::string netSettingsName = ui.m_NetworkSettingsNameComboBox->currentText().toUtf8().constData();
    if( 0 == netSettingsName.length() )
    {
        netSettingsName = "default";
    }

    netHostSetting.setNetHostSettingName( netSettingsName.c_str() );

    std::string strValue;
    strValue = ui.m_NetworkHostUrlEdit->text().toUtf8().constData();
    netHostSetting.setNetworkHostUrl( strValue.c_str() );

    strValue = ui.m_NetworkKeyEdit->text().toUtf8().constData();
    netHostSetting.setNetworkKey( strValue.c_str() );

    strValue = ui.m_ConnectTestUrlEdit->text().toUtf8().constData();
    netHostSetting.setConnectTestUrl( strValue.c_str() );

    strValue = ui.m_RandomConnectUrlEdit->text().toUtf8().constData();
    if( strValue.empty() )
    {
        strValue = "";
    }
    netHostSetting.setRandomConnectUrl( strValue.c_str() );

    strValue = ui.m_GroupHostUrlEdit->text().toUtf8().constData();
    if( strValue.empty() )
    {
        strValue = "";
    }
    netHostSetting.setGroupHostUrl( strValue.c_str() );

    strValue = ui.m_DefaultChatRoomHostUrlEdit->text().toUtf8().constData();
    if( strValue.empty() )
    {
        strValue = "";
    }
    netHostSetting.setChatRoomHostUrl( strValue.c_str() );

    std::string strPreferredIp = "";
    if( 0 != ui.m_LclIpListComboBox->currentIndex() )
    {
        strPreferredIp = ui.m_LclIpListComboBox->currentText().toUtf8().constData();
    }

    netHostSetting.setPreferredNetworkAdapterIp( strPreferredIp.c_str() );

    uint16_t u16TcpPort = ui.PortEdit->text().toUShort();
    if( 0 != u16TcpPort )
    {
        netHostSetting.setTcpPort( u16TcpPort );
    }
    else
    {
        netHostSetting.setTcpPort( NET_DEFAULT_NETSERVICE_PORT );
    }

    std::string externIp = ui.m_ExternIpEdit->text().toUtf8().constData();
    if( externIp.length() )
    {
        netHostSetting.setUserSpecifiedExternIpAddr( externIp.c_str() );
    }
    else
    {
        externIp = "";
        netHostSetting.setUserSpecifiedExternIpAddr( externIp.c_str() );
    }

    EFirewallTestType eFirewallTestType = eFirewallTestUrlConnectionTest;
    if( ui.AssumeNoProxyRadioButton->isChecked() )
    {
        eFirewallTestType = eFirewallTestAssumeNoFirewall;
    }
    else if( ui.AssumeProxyRadioButton->isChecked() )
    {
        eFirewallTestType = eFirewallTestAssumeFirewalled;
    }

    switch( eFirewallTestType )
    {
    case eFirewallTestAssumeNoFirewall:
        netHostSetting.setFirewallTestType( 1 );
        break;
 
    case eFirewallTestAssumeFirewalled:
        netHostSetting.setFirewallTestType( 2 );
        break;

    case eFirewallTestUrlConnectionTest:
    default:
        netHostSetting.setFirewallTestType( 0 );
    }
    
    netHostSetting.setUseUpnpPortForward( ui.m_UseUpnpCheckBox->isChecked() );
}

//============================================================================
void AppletNetworkSettings::slotExitButtonClick()
{
    closeApplet();
}

//============================================================================
void AppletNetworkSettings::slotGoToNetHostSettingsButtonClick()
{
    ActivityNetworkState * activityInfo = new ActivityNetworkState( m_MyApp, this );
    activityInfo->show();
}

//============================================================================
void AppletNetworkSettings::slotAutoDetectProxyClick( void )
{
    setFirewallTest( eFirewallTestUrlConnectionTest );
}

//============================================================================
void AppletNetworkSettings::slotNoProxyClick( void )
{
    setFirewallTest( eFirewallTestAssumeNoFirewall );
}

//============================================================================
void AppletNetworkSettings::slotYesProxyClick( void )
{
    setFirewallTest( eFirewallTestAssumeFirewalled );
}

//============================================================================
void AppletNetworkSettings::setFirewallTest( EFirewallTestType eFirewallType )
{
    ui.AutoDetectProxyRadioButton->setChecked( false );
    ui.AssumeNoProxyRadioButton->setChecked( false );
    ui.AssumeProxyRadioButton->setChecked( false );

    switch( eFirewallType )
    {
    case eFirewallTestAssumeFirewalled:
        ui.AssumeProxyRadioButton->setChecked( true );
        break;

    case eFirewallTestAssumeNoFirewall:
        ui.AssumeNoProxyRadioButton->setChecked( true );
        break;

    case eFirewallTestUrlConnectionTest:
    case eMaxFirewallTestType:
    default:
        ui.AutoDetectProxyRadioButton->setChecked( true );
        break;
    }
}

//============================================================================
void AppletNetworkSettings::slotRandomPortButtonClick( void )
{
    uint16_t u16TcpPort = VxGetRandomTcpPort();
    QString strPort(u16TcpPort);
    ui.PortEdit->setText( strPort );
    updateSettingsFromDlg();
}

//============================================================================
void AppletNetworkSettings::slotTestIsMyPortOpenButtonClick( void )
{
    uint16_t u16Port = ui.PortEdit->text().toUShort();
    if( 0 != u16Port )
    {
        updateSettingsFromDlg();
        ActivityIsPortOpenTest * dlg = new ActivityIsPortOpenTest(
            m_MyApp,
            u16Port,
            this );
        dlg->exec();
    }
    else
    {
        QMessageBox::information( this, tr( "Error" ), tr( "TCP Listen Port cannot be zero." ) );
    }
}

//============================================================================
void AppletNetworkSettings::slotUseUpnpCheckBoxClick( void )
{
}

//============================================================================
void AppletNetworkSettings::onComboBoxTextChanged( const QString & text )
{
    //updateSettingsFromDlg();
}

//============================================================================
void AppletNetworkSettings::onComboBoxSelectionChange( int )
{
    QString netSettingName = ui.m_NetworkSettingsNameComboBox->currentText();
    AccountMgr& dataHelper = m_MyApp.getAccountMgr();
    NetHostSetting netSetting;
    if( dataHelper.getNetHostSettingByName( netSettingName.toUtf8(), netSetting ) )
    {
        populateDlgFromNetHostSetting( netSetting );
        updateSettingsFromDlg();
    }
}

//============================================================================
void AppletNetworkSettings::populateDlgFromNetHostSetting( NetHostSetting& netSetting )
{
    ui.AutoDetectProxyRadioButton->setChecked( false );
    ui.AssumeNoProxyRadioButton->setChecked( false );
    ui.AssumeProxyRadioButton->setChecked( false );

    ui.m_NetworkHostUrlEdit->setText( netSetting.getNetworkHostUrl().c_str() );
    ui.m_NetworkKeyEdit->setText( netSetting.getNetworkKey().c_str() );
    m_OriginalNetworkKey = ui.m_NetworkKeyEdit->text();
    ui.m_ConnectTestUrlEdit->setText( netSetting.getConnectTestUrl().c_str() );
    ui.m_RandomConnectUrlEdit->setText( netSetting.getRandomConnectUrl().c_str() );
    ui.m_GroupHostUrlEdit->setText( netSetting.getGroupHostUrl().c_str() );
    ui.m_DefaultChatRoomHostUrlEdit->setText( netSetting.getChatRoomHostUrl().c_str() );
    ui.m_ExternIpEdit->setText( netSetting.getUserSpecifiedExternIpAddr().c_str() );

    int32_t firewallType = netSetting.getFirewallTestType();
    switch( firewallType )
    {
    case 2:
        ui.AssumeProxyRadioButton->setChecked( true );
        break;

    case 1:
        ui.AssumeNoProxyRadioButton->setChecked( true );
        break;

    default:
        ui.AutoDetectProxyRadioButton->setChecked( true );
        break;
    }

    ui.m_UseUpnpCheckBox->setChecked( netSetting.getUseUpnpPortForward() );

    uint16_t u16Port = netSetting.getTcpPort();
    ui.PortEdit->setText( QString( "%1" ).arg( u16Port ) );

    ui.m_LclIpListComboBox->clear();
    ui.m_LclIpListComboBox->addItem( DEFAULT_ADAPTER_IP_CHOICE );

    std::string strPreferredIp = netSetting.getPreferredNetworkAdapterIp();

    std::vector<InetAddress> lclIpAddresses;
    VxGetLocalIps( lclIpAddresses );
    std::vector<InetAddress>::iterator iter;
    std::string ipAddr;
    int currIdx = 0;
    for( iter = lclIpAddresses.begin(); iter != lclIpAddresses.end(); ++iter )
    {
        InetAddress& inetAddr = ( *iter );
        if( inetAddr.isIPv4()
            && ( false == inetAddr.isLoopBack() ) )
        {
            currIdx++;
            ipAddr = inetAddr.toStdString();
            ui.m_LclIpListComboBox->addItem( ipAddr.c_str() );
        }
    }
}

//============================================================================
void AppletNetworkSettings::closeEvent( QCloseEvent * event )
{
    QWidget::closeEvent( event );
}

//============================================================================
void AppletNetworkSettings::onSaveButtonClick( void )
{
    QString keyVal = getNetworkKey();
    if( verifyNetworkKey( keyVal ) )
    {
        std::string netSettingsName;
        netSettingsName = ui.m_NetworkSettingsNameComboBox->currentText().toUtf8().constData();
        if( netSettingsName.empty() )
        {
            QMessageBox::information( this, QObject::tr( "Network Setting" ), QObject::tr( "Network setting name cannot be blank." ) );
            return;
        }

        std::string netHostUrl;
        netHostUrl = ui.m_NetworkHostUrlEdit->text().toUtf8().constData();
        if( netHostUrl.empty() )
        {
            QMessageBox::information( this, QObject::tr( "Network Setting" ), QObject::tr( "Network host URL cannot be blank." ) );
            return;
        }

        std::string keyString = keyVal.toUtf8().constData();
        if( keyString.empty() )
        {
            QMessageBox::information( this, QObject::tr( "Network Setting" ), QObject::tr( "Network key cannot be blank." ) );
            return;
        }

        std::string connectTestUrl;
        connectTestUrl = ui.m_ConnectTestUrlEdit->text().toUtf8().constData();
        if( connectTestUrl.empty() )
        {
            QMessageBox::information( this, QObject::tr( "Network Setting" ), QObject::tr( "Connection Test URL cannot be blank." ) );
            return;
        }

        std::string randomConnectUrl;
        randomConnectUrl = ui.m_RandomConnectUrlEdit->text().toUtf8().constData();
        if( randomConnectUrl.empty() )
        {
            randomConnectUrl = "";
        }

        std::string groupHostUrl;
        groupHostUrl = ui.m_GroupHostUrlEdit->text().toUtf8().constData();
        if( groupHostUrl.empty() )
        {
            groupHostUrl = "";
        }

        std::string chatRoomHostUrl;
        chatRoomHostUrl = ui.m_DefaultChatRoomHostUrlEdit->text().toUtf8().constData();
        if( chatRoomHostUrl.empty() )
        {
            chatRoomHostUrl = "";
        }

        NetHostSetting netSetting;
        if( m_MyApp.getAccountMgr().getNetHostSettingByName( netSettingsName.c_str(), netSetting ) )
        {
            // setting exists.. check if user wants to change setting name
            if( QMessageBox::Yes != QMessageBox::question( this, QObject::tr( "Network Setting" ), 
                                                           QObject::tr( "Are you sure you want to overwrite existing network setting?.\n The network setting name can be changed in Select Network Setting." ),
                                                           QMessageBox::Yes | QMessageBox::No ) )
            {
                return;
            }
        }

        m_MyApp.getEngine().getEngineSettings().setNetworkKey( keyString );
        updateSettingsFromDlg();
        // need to apply settings also or what is used in ptop engine may not be what is shown in dialog which is confusing
        slotApplySettingsButtonClick();
        QMessageBox::information( this, QObject::tr( "Network Setting" ), QObject::tr( "Network setting was saved." ) );
        //QMessageBox::warning( this, QObject::tr( "Network Key" ), QObject::tr( "You may need to restart application to avoid connection problems." ) );
    }
}

//============================================================================
void AppletNetworkSettings::onDeleteButtonClick( void )
{
    updateSettingsFromDlg();
    if( 1 >= ui.m_NetworkSettingsNameComboBox->count() )
    {
        QMessageBox::warning( this, tr( "Network Settings" ), tr( "You cannot delete the last network setting." ) );
        return;
    }

    std::string netSettingsName;
    netSettingsName = ui.m_NetworkSettingsNameComboBox->currentText().toUtf8().constData();
    if( 0 != netSettingsName.length() )
    {
        m_MyApp.getAccountMgr().removeNetHostSettingByName( netSettingsName.c_str() );
        updateDlgFromSettings( false );
    }
}

//============================================================================
void AppletNetworkSettings::slotShowNetworkHostInformation()
{
    ActivityInformation * activityInfo = new ActivityInformation( m_MyApp, this, eInfoTypeNetworkHost );
    activityInfo->show();
}

//============================================================================
void AppletNetworkSettings::slotShowNetworkKeyInformation()
{
    ActivityInformation * activityInfo = new ActivityInformation( m_MyApp, this, eInfoTypeNetworkKey );
    activityInfo->show();
}

//============================================================================
void AppletNetworkSettings::slotShowConnectTestUrlInformation( void )
{
    ActivityInformation * activityInfo = new ActivityInformation( m_MyApp, this, eInfoTypeConnectTestUrl );
    activityInfo->show();
}

//============================================================================
void AppletNetworkSettings::slotShowConnectTestSettingsInformation( void )
{
    ActivityInformation * activityInfo = new ActivityInformation( m_MyApp, this, eInfoTypeConnectTestSettings );
    activityInfo->show();
}

//============================================================================
void AppletNetworkSettings::slotShowRandomConnectUrlInformation( void )
{
    ActivityInformation * activityInfo = new ActivityInformation( m_MyApp, this, eInfoTypeRandomConnectUrl );
    activityInfo->show();
}

//============================================================================
void AppletNetworkSettings::slotShowDefaultGroupHostUrlInformation( void )
{
    ActivityInformation * activityInfo = new ActivityInformation( m_MyApp, this, eInfoTypeDefaultGroupHostUrl );
    activityInfo->show();
}

//============================================================================
void AppletNetworkSettings::slotShowDefaultChatRoomUrlInformation( void )
{
    ActivityInformation * activityInfo = new ActivityInformation( m_MyApp, this, eInfoTypeDefaultChatRoomHostUrl );
    activityInfo->show();
}

//============================================================================
void AppletNetworkSettings::slotUpdateTimer( void )
{
    ui.m_InternetStateLabel->setText( DescribeInternetStatus( m_MyApp.getEngine().fromGuiGetInternetStatus() ) );
    ui.m_NetAvailStateLabel->setText( DescribeNetAvailStatus( m_MyApp.getEngine().fromGuiGetNetAvailStatus() ) );
    fillMyNodeUrl( ui.m_NodeUrlLabel );
}

//============================================================================
bool AppletNetworkSettings::verifyNetworkKey( QString& keyVal )
{
    bool isValid = true;
    if( keyVal.size() < 6 )
    {
        isValid = false;
        QMessageBox::warning( this, QObject::tr( "Network Key" ), QObject::tr( "Network Key must be at least 6 characters ( 8 or more characters recommended )." ) );
    }

    if( keyVal != m_OriginalNetworkKey && !m_OriginalNetworkKey.isEmpty() )
    {
        if( QMessageBox::Yes != QMessageBox::question( this, QObject::tr( "Network Key" ),
            QObject::tr( "Are you sure you want to change the network key?\n All users of your network will need to have the same key or cannot connect." ),
            QMessageBox::Yes | QMessageBox::No ) )
        {
            ui.m_NetworkKeyEdit->setText( m_OriginalNetworkKey );
            return false;
        }
    }

    return isValid;
}

//============================================================================
void AppletNetworkSettings::slotCopyMyUrlToClipboard( void )
{
    QClipboard * clipboard = QApplication::clipboard();
    clipboard->setText( ui.m_NodeUrlLabel->text() );
}
