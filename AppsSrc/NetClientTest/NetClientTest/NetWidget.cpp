//============================================================================
// Copyright (C) 2018 Brett R. Jones
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

#include "NetWidget.h"
#include "AppLogic.h"
#include "NetTestUtil.h"

#include <CoreLib/VxParse.h>
#include <NetLib/VxSktConnectSimple.h>
#include <NetLib/VxSktUtil.h>
#include <NetLib/InetAddress.h>

#include <QScrollBar>
#include <QClipboard>
#include <QMessageBox>

//============================================================================
NetWidget::NetWidget( QWidget * parent )
    : QWidget( parent )
    , m_AppLogic( GetAppLogic() )
    , m_PingResponseServer( GetAppLogic() )
    , m_ConnectTimer( new QTimer(this) )
    , m_ListenTimer( new QTimer( this ) )
{
    ui.setupUi( this );
    initNetWidget();
}

//============================================================================
void NetWidget::initNetWidget()
{
    ui.m_LogWidget->initLogCallback();
    LogMsg( LOG_DEBUG, "Log Initialized" );
    m_ConnectTimer->setInterval( 6000 );
    m_ListenTimer->setInterval( 6000 );

    connect( getConnectButton(), SIGNAL( clicked() ), this, SLOT( slotConnectButtonClicked() ) );
    connect( getDisconnectButton(), SIGNAL( clicked() ), this, SLOT( slotDisconnectButtonClicked() ) );
    connect( getListenButton(), SIGNAL( clicked() ), this, SLOT( slotListenButtonClicked() ) );
    connect( getStopListenButton(), SIGNAL( clicked() ), this, SLOT( slotStopListenButtonClicked() ) );

    connect( m_ConnectTimer, SIGNAL( timeout() ), this, SLOT( slotConnectTimeout() ) );
    connect( m_ListenTimer, SIGNAL( timeout() ), this, SLOT( slotListenTimeout() ) );

    setConnectionStatus( false );
    setListenStatus( false );
    loadFromSettings();
    fillAdapterComboBox();

    connect( ui.m_OverrideAdapterComboBox, SIGNAL( currentTextChanged( const QString& ) ), this, SLOT( slotAdapterIpSelectionChanged( const QString& ) ) );
}

//============================================================================
void NetWidget::fillAdapterComboBox( void )
{
    std::vector<InetAddress> addrList;
    InetAddress::getAllAddresses( addrList );
    InetAddress::dumpAddresses( addrList );
    addAdapterAddr( ANY_IP_DEF );
    for( auto inetAddr : addrList )
    {
        if( inetAddr.isIPv4() )
        {
            addAdapterAddr( inetAddr.toStdString().c_str() );
        }
    }

    addAdapterAddr( "127.0.0.1" );
}

//============================================================================
void NetWidget::addAdapterAddr( QString ipString )
{
    ui.m_OverrideAdapterComboBox->addItem( ipString );
}

//============================================================================
void NetWidget::slotAdapterIpSelectionChanged( const QString& ipText )
{
    if( ipText == ANY_IP_DEF )
    {
        // leave blank for any ip address
        ui.m_OverrideAdapterIpEdit->setText( "" );
    }
    else
    {
        ui.m_OverrideAdapterIpEdit->setText( ipText );
    }
}

//============================================================================
void NetWidget::loadFromSettings()
{
    AppSettings& appSettings = m_AppLogic.getAppSettings();
    m_ConnectIp = appSettings.value( "ConnectIp", "" ).toString();
    m_ConnectPortText = appSettings.value( "ConnectPort", "45125" ).toString();
    m_ConnectTimeout = appSettings.value( "ConnectTimeout", "12" ).toInt();
    m_ConnectIntervalSeconds = appSettings.value( "ConnectInterval", "0" ).toInt();
    m_AdapterIp = appSettings.value( "AdapterIp", "" ).toString();
    m_ListenPortText = appSettings.value( "ListenPort", "45125" ).toString();

    m_ConnectTimer->setInterval( m_ConnectIntervalSeconds * 1000 );

    ui.m_ConnectIpEdit->setText( m_ConnectIp );
    ui.m_ConnectPortEdit->setText( m_ConnectPortText );
    ui.m_ConnectTimeout->setValue( m_ConnectTimeout );
    ui.m_IntervalSpinBox->setValue( m_ConnectIntervalSeconds );
    ui.m_OverrideAdapterIpEdit->setText( m_AdapterIp );
    ui.m_ListenPortLineEdit->setText( m_ListenPortText );
}

//============================================================================
void NetWidget::saveToSettings()
{
    updateVarsFromGui();
    AppSettings& appSettings = m_AppLogic.getAppSettings();
    appSettings.setValue( "ConnectIp", m_ConnectIp );
    appSettings.setValue( "ConnectPort", m_ConnectPortText );
    appSettings.setValue( "ConnectTimeout", m_ConnectTimeout );
    appSettings.setValue( "ConnectInterval", m_ConnectIntervalSeconds );
    appSettings.setValue( "AdapterIp", m_AdapterIp );
    appSettings.setValue( "ListenPort", m_ListenPortText );
}

//============================================================================
void NetWidget::updateVarsFromGui()
{
    m_ConnectIp = ui.m_ConnectIpEdit->text();
    m_ConnectPortText = ui.m_ConnectPortEdit->text();
    m_ConnectIntervalSeconds = ui.m_IntervalSpinBox->value();
    m_ConnectTimeout = ui.m_ConnectTimeout->value();
    m_AdapterIp = ui.m_OverrideAdapterIpEdit->text();
    m_ListenPortText = ui.m_ListenPortLineEdit->text();
    m_ListenPort = (uint16_t)m_ListenPortText.toInt();

    if( m_AdapterIp == ANY_IP_DEF )
    {
        // any ip or default ip requires ip setting to blank
        m_AdapterIp = "";
    }

    if( m_ConnectIntervalSeconds )
    {
        m_ConnectTimer->setInterval( m_ConnectIntervalSeconds * 1000 );
    }
    else
    {
        m_ConnectTimer->stop();
    }
}

//============================================================================
void NetWidget::setConnectionStatus( bool isConnected )
{
    getConnectButton()->setEnabled( !isConnected );
    getDisconnectButton()->setEnabled( isConnected );
}

//============================================================================
void NetWidget::setListenStatus( bool isListening )
{
    getListenButton()->setEnabled( !isListening );
    getStopListenButton()->setEnabled( isListening );
}

//============================================================================
bool NetWidget::isUrlValid( std::string& url )
{
    bool isValid = false;

    return isValid;
}

//============================================================================
bool NetWidget::isIpValid( std::string& ipAddr )
{
    bool isValid = false;

    return isValid;
}

//============================================================================
bool NetWidget::isPortValid( uint16_t ipPort )
{
    bool isValid = false;

    return isValid;
}

//============================================================================
void NetWidget::onLogEvent( uint32_t u32LogFlags, const char * logMsg )
{
    getLogWidget()->onLogEvent( u32LogFlags,logMsg );
}

//============================================================================
void NetWidget::slotConnectButtonClicked( void )
{
    setPingStatus( "CONNECTING" );
    updateVarsFromGui();
    if( m_ConnectIp.isEmpty() )
    {
        QMessageBox::warning( this, QObject::tr( "Connection URL" ), QObject::tr( "Connection URL is empty." ) );
    }
    else
    {
        m_ConnectPort = ( uint16_t )m_ConnectPortText.toInt();
        if( 0 == m_ConnectPort )
        {
            QMessageBox::warning( this, QObject::tr( "Connection Port" ), QObject::tr( "Connection Port Cannot Be Zero." ) );
        }
        else
        {
            if( m_ConnectTimeout == 0 )
            {
                m_ConnectTimeout = 12;
            }
            
            saveToSettings();
 
            if( m_ConnectIntervalSeconds )
            {
                setConnectionStatus( true );
                m_ConnectTimer->setInterval( m_ConnectIntervalSeconds * 1000 );
                m_ConnectTimer->start();
                LogMsg( LOG_DEBUG, "Will Attempt Connection to %s:%d with timeout %d every %d seconds", m_ConnectIp.toUtf8().constData(), m_ConnectPort, m_ConnectTimeout, m_ConnectIntervalSeconds );
            }
            else
            {
                m_ConnectTimer->stop();
                LogMsg( LOG_DEBUG, "Will Attempt Connection to %s:%d with timeout %d one time", m_ConnectIp.toUtf8().constData(), m_ConnectPort, m_ConnectTimeout );
            }

            slotConnectTimeout();
            LogMsg( LOG_DEBUG, "\n" );
        }
    }
}

//============================================================================
void NetWidget::slotDisconnectButtonClicked( void )
{
    m_ConnectTimer->stop();
    setConnectionStatus( false );
    LogMsg( LOG_DEBUG, "Connect attempts stopped" );
}

//============================================================================
void NetWidget::slotConnectTimeout( void )
{
    updateVarsFromGui();

    VxSktConnectSimple sktSimple;
    VxTimer testTimer;

    setPingStatus( "CONNECTING" );
    if( 0 == m_ConnectPort )
    {
        LogMsg( LOG_DEBUG, "Attempt Connect invalid port %d", m_ConnectPort );
        setPingStatus( "INVALID" );
        return;
    }

    if( 8 > m_ConnectIp.size() )
    {
        setPingStatus( "INVALID" );
        LogMsg( LOG_DEBUG, "Attempt Connect invalid ip address %s", m_ConnectIp.toUtf8().constData() );
        return;
    }

    int connectTimeout = m_ConnectTimeout ? m_ConnectTimeout : 6;

    LogMsg( LOG_DEBUG, "Attempt Connect to %s:%d for %d seconds", m_ConnectIp.toUtf8().constData(), m_ConnectPort, connectTimeout );
    if( INVALID_SOCKET != sktSimple.connectTo( m_ConnectIp.toUtf8().constData(), m_ConnectPort, connectTimeout * 1000 ) )
    {
        sktSimple.dumpConnectionInfo();

        int pingTimeout = m_ConnectTimeout ? m_ConnectTimeout : 6;
        LogMsg( LOG_DEBUG, "Attempt PING %s:%d for %d seconds", m_ConnectIp.toUtf8().constData(), m_ConnectPort, connectTimeout );
        std::string retPong;
        if( NetTestUtil::sendAndRecievePing( testTimer, sktSimple, retPong, pingTimeout * 1000 ) )
        {
            sktSimple.closeSkt();
            LogMsg( LOG_DEBUG, "** SUCCESS rxed (%s) Connect and ping %s:%d in elapsed seconds %1.0f", retPong.c_str(), m_ConnectIp.toUtf8().constData(), m_ConnectPort, testTimer.elapsedSec() );
            setPingStatus( "SUCCESS" );
        }
        else
        {
            setPingStatus( "FAILED CONNECT" );
            sktSimple.dumpConnectionInfo();
            LogMsg( LOG_DEBUG, "Connected to %s but failed ping in elapsed seconds %1.0f", m_ConnectIp.toUtf8().constData(), testTimer.elapsedSec() );
            sktSimple.closeSkt();
            return;
        }
    }
    else
    {
        setPingStatus( "FAILED CONNECT" );
        sktSimple.dumpConnectionInfo();
        LogMsg( LOG_DEBUG, "Failed Connect to %s:%d elapsed seconds %1.0f", m_ConnectIp.toUtf8().constData(), m_ConnectPort, testTimer.elapsedSec() );
    }
}

//============================================================================
void NetWidget::slotListenButtonClicked( void )
{
    //m_PingResponseServer.startListening( (uint16_t)port, )

    updateVarsFromGui();

    LogMsg( LOG_DEBUG, "Listen" );
    if( 0 == m_ListenPort )
    {
        LogMsg( LOG_DEBUG, "Listen invalid port %d", m_ListenPort );
        return;
    }

    if( m_AdapterIp.size() &&  8 > m_AdapterIp.size() )
    {
        LogMsg( LOG_DEBUG, "Attempt Connect invalid ip address %s", m_AdapterIp.toUtf8().constData() );
        return;
    }

    m_PingResponseServer.startListening( m_ListenPort, m_AdapterIp.toUtf8().constData() );

    setListenStatus( true );
    m_ListenTimer->start();
}

//============================================================================
void NetWidget::slotStopListenButtonClicked( void )
{
    m_ListenTimer->stop();
    setListenStatus( false );
    if( m_PingResponseServer.isListening() )
    {
        LogMsg( LOG_DEBUG, "Stop Listening Port %d", m_PingResponseServer.getListenPort() );
        m_PingResponseServer.stopListening();
    }
    else
    {
        LogMsg( LOG_DEBUG, "Listen port %d already stopped", m_ListenPort );
    }
}

//============================================================================
void NetWidget::slotListenTimeout( void )
{
    LogMsg( LOG_DEBUG, "slotListenTimeout");
}
