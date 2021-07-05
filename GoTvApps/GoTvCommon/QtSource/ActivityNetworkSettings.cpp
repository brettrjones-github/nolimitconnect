//============================================================================
// Copyright (C) 2013 Brett R. Jones
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

#include "ActivityNetworkSettings.h"
#include "AppCommon.h"
#include "ActivityNetHostSettings.h"
#include "ActivityIsPortOpenTest.h"
#include "ActivityNetworkHostTest.h"
#include "AppGlobals.h"

#include <ptop_src/ptop_engine_src/P2PEngine/EngineSettings.h>
#include <ptop_src/ptop_engine_src/P2PEngine/EngineSettingsDefaultValues.h>
#include <NetLib/VxGetRandomPort.h>
#include <NetLib/VxSktUtil.h>

#include <QMessageBox>

namespace
{
	QString				DEFAULT_ADAPTER_IP_CHOICE			= "default";
}

//============================================================================
ActivityNetworkSettings::ActivityNetworkSettings( AppCommon& app, QWidget * parent )
: ActivityBase( OBJNAME_ACTIVITY_NETWORK_SETTINGS, app, parent, eAppletMessenger, true )
{
	ui.setupUi(this);
	ui.m_TitleBarWidget->setTitleBarText( QObject::tr("Network Settings") );
    connectBarWidgets();

	connectSignals();
	updateDlgFromSettings();
}

//============================================================================
void ActivityNetworkSettings::connectSignals( void )
{
	connect( ui.m_TitleBarWidget,					SIGNAL(signalBackButtonClicked()), this, SLOT( slotExitButtonClick() ) );
	connect( ui.m_GoToNetHostSettingsButton,		SIGNAL(clicked()), this, SLOT( slotGoToNetHostSettingsButtonClick() ) );

	connect( ui.AutoDetectProxyRadioButton,			SIGNAL(clicked()), this, SLOT( slotAutoDetectProxyClick() ) );
	connect( ui.AssumeNoProxyRadioButton,			SIGNAL(clicked()), this, SLOT( slotNoProxyClick() ) );
	connect( ui.AssumeProxyRadioButton,				SIGNAL(clicked()), this, SLOT( slotYesProxyClick() ) );

	connect( ui.RandomPortButton,					SIGNAL(clicked()), this, SLOT( slotRandomPortButtonClick() ) );
	connect( ui.m_IsPortOpenTesButton,				SIGNAL(clicked()), this, SLOT( slotRunIsPortOpenButtonClick() ) );
    connect( ui.m_IsPortOpenTesButton,              SIGNAL( clicked() ), this, SLOT( slotRunNetworkHostTestButtonClick() ) );
	connect( ui.m_UseUpnpCheckBox,					SIGNAL(clicked()), this, SLOT( slotUseUpnpCheckBoxClick() ) );
	connect( ui.m_ExternIpHelpButton,				SIGNAL(clicked()), this, SLOT( slotExternIpHelpButtonClick() ) );
}

//============================================================================
void ActivityNetworkSettings::updateDlgFromSettings()
{
	uint16_t u16Port = m_Engine.getEngineSettings().getTcpIpPort();
	ui.PortEdit->setText(QString("%1").arg(u16Port));

	ui.AutoDetectProxyRadioButton->setChecked( false );
	ui.AssumeNoProxyRadioButton->setChecked( false );
	ui.AssumeProxyRadioButton->setChecked( false );
    FirewallSettings::EFirewallTestType iDetectProxySetting = m_Engine.getEngineSettings().getFirewallTestSetting();
	switch( iDetectProxySetting )
	{
	case EngineSettings::eFirewallTestAssumeNoFirewall:
		ui.AssumeNoProxyRadioButton->setChecked( true );
		break;

	case EngineSettings::eFirewallTestAssumeFirewalled:
		ui.AssumeProxyRadioButton->setChecked( true );
		break;

	case EngineSettings::eFirewallTestUrlConnectionTest:
	default:
		ui.AutoDetectProxyRadioButton->setChecked( true );
		break;
	}

	ui.m_UseUpnpCheckBox->setChecked( m_Engine.getEngineSettings().getUseUpnpPortForward() );
	ui.m_LclIpListComboBox->clear();
	ui.m_LclIpListComboBox->addItem( DEFAULT_ADAPTER_IP_CHOICE );

	std::string strPreferredIp = m_Engine.getEngineSettings().getPreferredNetworkAdapterIp();
	std::vector<InetAddress> lclIpAddresses;
	VxGetLocalIps(	lclIpAddresses );
	std::vector<InetAddress>::iterator iter;
	std::string ipAddr;
	int preferredAddrIndex = 0;
	int currIdx = 0;
	for( iter = lclIpAddresses.begin(); iter != lclIpAddresses.end(); ++iter )
	{
		InetAddress& inetAddr = (*iter);
		if( inetAddr.isIPv4() 
			&& ( false == inetAddr.isLoopBack() ) )
		{
			currIdx++;
			ipAddr = inetAddr.toStdString();
			if( ipAddr == strPreferredIp )
			{
				preferredAddrIndex = currIdx;
			}
			
			ui.m_LclIpListComboBox->addItem( ipAddr.c_str() );
		}
	}

	ui.m_LclIpListComboBox->setCurrentIndex( preferredAddrIndex );
	std::string externIP;
	m_Engine.getEngineSettings().getExternalIp( externIP );
	ui.m_ExternIpEdit->setText( externIP.c_str() );
}

//============================================================================
void ActivityNetworkSettings::updateSettingsFromDlg()
{
	std::string strPreferredIp = "";
	if( 0 != ui.m_LclIpListComboBox->currentIndex() )
	{
		strPreferredIp = ui.m_LclIpListComboBox->currentText().toUtf8().constData();
	}

	m_Engine.getEngineSettings().setPreferredNetworkAdapterIp( strPreferredIp.c_str() );

	uint16_t u16TcpPort = ui.PortEdit->text().toUShort();
	if( 0 != u16TcpPort )
	{
		m_Engine.getEngineSettings().setTcpIpPort( u16TcpPort );
		m_Engine.getMyPktAnnounce().setMyOnlinePort( u16TcpPort );
		m_MyApp.getAppGlobals().getUserIdent()->m_DirectConnectId.setPort( u16TcpPort );
	}

	std::string externIp = ui.m_ExternIpEdit->text().toUtf8().constData();
	if( externIp.length() )
	{
		m_Engine.getEngineSettings().setExternalIp( externIp );
	}

    FirewallSettings::EFirewallTestType eFirewallTestType = EngineSettings::eFirewallTestUrlConnectionTest;
	if( ui.AssumeNoProxyRadioButton->isChecked() )
	{
		eFirewallTestType= FirewallSettings::eFirewallTestAssumeNoFirewall;
		if( externIp.length() )
		{
			m_Engine.getMyPktAnnounce().setOnlineIpAddress( externIp.c_str() );
		}
	}
	else if( ui.AssumeProxyRadioButton->isChecked() )
	{
		eFirewallTestType = EngineSettings::eFirewallTestAssumeFirewalled;
	}
	//else if( ui.AssumeIsNetworkServiceNodeButton->isChecked() )
	//{
	//	eFirewallTestType = EngineSettings::eFirewallTestIsNetService;
	//}
	m_Engine.getEngineSettings().setFirewallTestSetting( eFirewallTestType );
	m_Engine.getEngineSettings().setUseUpnpPortForward( ui.m_UseUpnpCheckBox->isChecked() );

    m_Engine.engineSettingsUpdate( m_Engine.getEngineSettings() );
}

////============================================================================
//void ActivityNetworkSettings::slotApplyButtonClick()
//{
//	updateSettingsFromDlg();
//	QMessageBox::warning(this, tr("Application"), tr("You must restart application for settings to take effect." ) );
//	accept();
//}

//============================================================================
void ActivityNetworkSettings::slotExitButtonClick()
{
	updateSettingsFromDlg();
	m_Engine.fromGuiNetworkSettingsChanged();
	accept();
}

//============================================================================
void ActivityNetworkSettings::slotGoToNetHostSettingsButtonClick()
{

	ActivityNetHostSettings * dlg = new ActivityNetHostSettings( 
		m_MyApp,
		this			);
	dlg->exec();
}

//============================================================================
void ActivityNetworkSettings::slotAutoDetectProxyClick( void )
{
	SetFirewallTest( EngineSettings::eFirewallTestUrlConnectionTest );
}

//============================================================================
void ActivityNetworkSettings::slotNoProxyClick( void )
{
	SetFirewallTest( EngineSettings::eFirewallTestAssumeNoFirewall );
}

//============================================================================
void ActivityNetworkSettings::slotYesProxyClick( void )
{
	SetFirewallTest( EngineSettings::eFirewallTestAssumeFirewalled );
}

//============================================================================
void ActivityNetworkSettings::SetFirewallTest( FirewallSettings::EFirewallTestType eFirewallType )
{
	ui.AutoDetectProxyRadioButton->setChecked( false );
	ui.AssumeNoProxyRadioButton->setChecked( false );
	ui.AssumeProxyRadioButton->setChecked( false );

	switch( eFirewallType )
	{
	case FirewallSettings::eFirewallTestUrlConnectionTest:
		ui.AutoDetectProxyRadioButton->setChecked( true );
		break;

	case FirewallSettings::eFirewallTestAssumeFirewalled:
		ui.AssumeProxyRadioButton->setChecked( true );
		break;

	case FirewallSettings::eFirewallTestAssumeNoFirewall:
		ui.AssumeNoProxyRadioButton->setChecked( true );
		break;
	}
}

//============================================================================
void ActivityNetworkSettings::slotRandomPortButtonClick( void )
{
	uint16_t u16TcpPort = VxGetRandomTcpPort();
	QString strPort;
	strPort.sprintf("%d", u16TcpPort);
	ui.PortEdit->setText(strPort);
	updateSettingsFromDlg();
}

//============================================================================
void ActivityNetworkSettings::slotRunIsPortOpenButtonClick( void )
{
	uint16_t u16Port = ui.PortEdit->text().toUShort();
	if( 0 != u16Port )
	{
		updateSettingsFromDlg();
		ActivityIsPortOpenTest * dlg = new ActivityIsPortOpenTest( 
			m_MyApp,
			u16Port,
			this			);
		dlg->exec();
	}
	else
	{
		QMessageBox::information(this, tr("Error"), tr("TCP Port cannot be zero." ) );
	}
}

//============================================================================
void ActivityNetworkSettings::slotRunNetworkHostTestButtonClick( void )
{
    updateSettingsFromDlg();
    ActivityNetworkHostTest * dlg = new ActivityNetworkHostTest(
        m_MyApp,
        u16Port,
        this );
    dlg->exec();
}

//============================================================================
void ActivityNetworkSettings::slotUseUpnpCheckBoxClick( void )
{
}

//============================================================================
void ActivityNetworkSettings::slotExternIpHelpButtonClick( void )
{
	QMessageBox::information(this, tr("Information"), tr("You can determine your external IP Address by browsing to http://www.google.com and searching for 'what is my ip'." ) );
}