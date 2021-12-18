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

#include "AppletHelpNetSignalBars.h"
#include "AppCommon.h"
#include "AppSettings.h"

#include <CoreLib/VxDebug.h>

#include <QClipboard>

//============================================================================
AppletHelpNetSignalBars::AppletHelpNetSignalBars( AppCommon& app, QWidget * parent )
    : AppletBase( OBJNAME_APPLET_HELP_NET_SIGNAL_BARS, app, parent )
{
    ui.setupUi( getContentItemsFrame() );
    setAppletType( eAppletHelpNetSignalBars );
    setTitleBarText( DescribeApplet( m_EAppletType ) );
    connect( this, SIGNAL( signalBackButtonClicked() ), this, SLOT( close() ) );

    m_MyApp.activityStateChange( this, true );
    initAppletHelpNetSignalBars();
}

//============================================================================
AppletHelpNetSignalBars::~AppletHelpNetSignalBars()
{
    m_MyApp.activityStateChange( this, false );
}

//============================================================================
void AppletHelpNetSignalBars::initAppletHelpNetSignalBars( void )
{
    ui.setupUi( this );

    ui.m_Pic_1->setResourceImage( ":/AppRes/Resources/bars_1_internet.png", true );
    ui.m_Pic_2->setResourceImage( ":/AppRes/Resources/bars_2_test_avail.png", true );
    ui.m_Pic_3->setResourceImage( ":/AppRes/Resources/bars_3_net_avail.png", true );
    ui.m_Pic_4->setResourceImage( ":/AppRes/Resources/bars_4_relay_search.png", true );
    ui.m_Pic_5->setResourceImage( ":/AppRes/Resources/bars_5_onlie_with_relay.png", true );
    ui.m_Pic_6->setResourceImage( ":/AppRes/Resources/bars_6_direct.png", true );
    ui.m_Pic_7->setResourceImage( ":/AppRes/Resources/bars_7_hosted_direct.png", true );
    ui.m_Pic_8->setResourceImage( ":/AppRes/Resources/bars_7_hosted_with_relay.png", true );

    connectBarWidgets();
}

/*
//============================================================================
void AppletHelpNetSignalBars::showEvent( QShowEvent * ev )
{
    ActivityBase::showEvent( ev );
    updateInformation();
}

//============================================================================
void AppletHelpNetSignalBars::slotCopyToClipboardButtonClicked( void )
{
    QClipboard * clipboard = QApplication::clipboard();
    clipboard->setText( ui.m_InfoText->toPlainText() );
}

//============================================================================
void AppletHelpNetSignalBars::updateInformation( void )
{
    ui.m_PictureLabel->setVisible( false );

    if( m_PluginType != ePluginTypeInvalid )
    {
        ui.m_ServiceInfoButton->setIcon( m_MyApp.getMyIcons().getPluginIcon( m_PluginType ) );

        switch( m_PluginType )
        {
        case ePluginTypeHostGroup:
        case ePluginTypeGroupBlobing:
        case ePluginTypeHostNetwork:
            ui.m_PictureLabel->setResourceImage( ":/AppRes/Resources/NetworkDesign.png", true );
            ui.m_PictureLabel->setVisible( true );
            break;
        default:
            break;
        }
    }

    switch( m_InfoType )
    {
    case eInfoTypePermission:
        ui.m_ServiceInfoButton->setIcon( eMyIconPermissions );
        break;
    case eInfoTypeNetworkKey:
        ui.m_ServiceInfoButton->setIcon( eMyIconNetworkKey );
        break;
    case eInfoTypeConnectTestUrl:
        ui.m_ServiceInfoButton->setIcon( eMyIconServiceConnectionTest );
        break;
    case eInfoTypeConnectTestSettings:
        ui.m_ServiceInfoButton->setIcon( eMyIconSettingsConnectionTest );
        break;

    case eInfoTypeNetworkHost:
        ui.m_PictureLabel->setResourceImage( ":/AppRes/Resources/NetworkDesign.png", true );
        ui.m_PictureLabel->setVisible( true );
        ui.m_ServiceInfoButton->setIcon( eMyIconServiceHostNetwork );
        break;
    default:
        break;
    }

    ui.m_InfoText->clear();
    ui.m_InfoText->appendPlainText( getInfoText() );
}

QString AppletHelpNetSignalBars::m_NoInfoAvailable( QObject::tr( "No Information is local available. please visit http://wwww.nolimitconnect.com for latest infomation and help" ) );

QString AppletHelpNetSignalBars::m_NetworkDesign( QObject::tr(
    "=== NETWORK DESIGN ===\n"
    "NOTE1: For anyone hosting services I suggest using good anti-virus/anti-malware and a service like PeerBlock to avoid bandwidth useage by those who make money spying on others.\n"
    "NOTE2: For anyone not hosting a NoLimitConnect Hosting Service a VPN is also suggested ( A VPN is not recommended when hosting a NoLimitConnect Network because of the requirement of a fixed ip address )\n"
    "\n"
    "=== TIER 1: NOLIMITCONNECT NETWORK SERVICE* ===\n"
    "NOTE3: This service requires a fixed IP Address or a DNS url like http://www.nolimitconnect.net.\n"
    "REQUIRED SERVICES* AND SUGGESTED PERMISSION LEVELS*\n"
    " CONNECTION TEST SERVICE - ANYBODY\n"
    " NOLIMITCONNECT NETWORK SERVICE - ANYBODY\n"
    "\n"
    "SUGGESTED OPTIONAL SERVICES* AND SUGGESTED PERMISSION LEVELS*\n"
    " RANDOM CONNECT SERVICE - ANYBODY\n"
    " ABOUT PAGE SERVICE - GUEST\n"
    " SEARCH AND SCAN SERVICES - GUEST\n"
    " STORY BOARD PAGE SERVICE - GUEST\n"
    "\n"
    "OTHER OPTIONAL SERVICES* AND SUGGESTED PERMISSION LEVELS*\n"
    " RELAY SERVICE - FRIEND\n"
    "\n"
    "\n"
    "=== TIER 2: GROUP HOST LISTING HOST SERVICE* ===\n"
    "REQUIRED SERVICES* AND SUGGESTED PERMISSION LEVELS*\n"
    " GROUP HOST LISTING SERVICE - ANYBODY\n"
    "\n"
    "SUGGESTED OPTIONAL SERVICES* AND SUGGESTED PERMISSION LEVELS*\n"
    " CONNECTION TEST SERVICE - ANYBODY\n"
    " RANDOM CONNECT SERVICE - ANYBODY\n"
    " PROFILE PAGE SERVICE - GUEST\n"
    " SEARCH AND SCAN SERVICES - GUEST\n"
    " STORY BOARD PAGE SERVICE - GUEST\n"
    "\n"
    "OTHER OPTIONAL SERVICES* AND SUGGESTED PERMISSION LEVELS*\n"
    " RELAY SERVICE - FRIEND\n"
    "\n"
    "=== TIER 3: GROUP HOST SERVICE* ===\n"
    " MEMEBERSHIP LEVEL means the permission level required to join the group - can be anonymous but suggest at least guest level\n"
    " Members of a group will be automatically given guest permission level between users. Higher level permissions can be granted by individual users to another user.\n"
    "REQUIRED SERVICES* AND SUGGESTED PERMISSION LEVELS*\n"
    " GROUP HOST SERVICE - MEMEBERSHIP LEVEL\n"
    " RELAY SERVICE - MEMEBERSHIP LEVEL\n"
    "\n"
    "SUGGESTED OPTIONAL SERVICES* AND SUGGESTED PERMISSION LEVELS*\n"
    " CONNECTION TEST SERVICE - ANYBODY\n"
    " GROUP CHAT SERVICE - MEMEBERSHIP LEVEL\n"
    " RANDOM CONNECT SERVICE - ANYBODY\n"
    " ABOUT PAGE SERVICE - MEMEBERSHIP LEVEL\n"
    " SEARCH AND SCAN SERVICES - MEMEBERSHIP LEVEL\n"
    " STORY BOARD PAGE SERVICE - MEMEBERSHIP LEVEL\n"
    "\n"
    "OTHER OPTIONAL SERVICES* AND SUGGESTED PERMISSION LEVELS*\n"
    "\n"
    "\n"
    "=== TIER 4: USERS ===\n"
    " USERS should set their permission levels of services and/or streams to be as public or private as desired by user.\n"
    "\n"
) );

QString AppletHelpNetSignalBars::m_PluginDefinitions( QObject::tr(
    "\n"
"DEFINITIONS:\n"
" *ABOUT PAGE SERVICE - Provide a information page about a host or person\n"
" *CAM STREAM SERVICE: A host or user can provide streaming video live from their device camera or cam\n"
" *CONNECTION TEST SERVICE: Service for devices to discover thier web IP Address and if they can have a open port for direct connection or require RELAY SERVICE*.\n"
" *FILE SHARE PLUGIN: Provides user or host file sharing with other users.\n"
" *FILE TRANSFER PLUGIN: Provides user file transfer to another person.\n"
" *NOLIMITCONNECT NETWORK SERVICE: Provides Connection test and list of group host listing hosts for search for groups to join.\n"
" *GROUP CHAT PLUGIN: Provides group based text chat service.\n"
" *GROUP HOST LISTING SERVICE: Provides List of Group Hosts and thier address for users to connect to and search or join.\n"
" *GROUP HOST SERVICE: Provides connection and other services to members who have joined the Group Host.\n"
" *PERMISSION LEVELS: Friendship level required to be allowed to use a service or connect to a person.\n"
" *PHONE CALL PLUGIN: Provides user with voice phone calling to others using the NoLimitConnect app and internet.\n"
" *RANDOM CONNECT CONNECT SERVICE: Provides list of current persons using phone shake mode to discover and connect to others.\n"
" *RELAY SERVICE: Provides data transfer services for persons without a open port to accept incomming connections.\n"
" *SEARCH AND SCAN SERVICES: provide user a list of known persons with the desired search critera or service or stream or files\n"
" *STORY BOARD PAGE SERVICE - Provide a blog like page a host or person can post to for others to view/read\n"
" *TRUTH OR DARE PLUGIN: provides truth or dare video chat game between individuals\n"
" *VIDEO CHAT PLUGIN: Provides user video chat to others using the NoLimitConnect app and internet.\n"
) );

QString AppletHelpNetSignalBars::m_Permissions( QObject::tr(
    "=== PERMISSION LEVELS ===\n"
    " Permission Levels are used for setting either what level of permission is required to access a plugin or"
    " the permission level granted to another person to control what that person has access to."
    "\n"
    "\n"
    " NOTE 1: If you join a group then other members of that group are granted Guest Permission Level automatically and"
    " you can grant individual users a higher permission level if you want to."
    "\n"
    "\n"
    " NOTE 2: If you put your device in RANDOM CONNECT CONNECT mode then others that are in RANDOM CONNECT CONNECT mode"
    " are granted Guest Permission Level automatically and you can grant individual users a higher permission level if you want to."
    "\n"
    "\n"
    "=== ADMINISTRATOR PERMISSION ===\n"
    " This is the highest level of permission.\n"
    " A person granted this permission level can access plugins or services set to permission level Admistrator, Friend, Guest or Anybody.\n"
    " A example of usage would be if you wanted to set up a web cam stream service as a nanny or security cam that only you can access."
    " You would set the web cam service to permission level admistrator and only grant your self administrator permission level."
    "\n"
    "\n"
    "=== FRIEND PERMISSION ===\n"
    " A person granted this permission level can access plugins or services set to permission level Friend, Guest or Anybody.\n"
    " A example of usage would be if you wanted other members of a group you joined to be able to message you and view your about page"
    " but not be able to access your shared files."
    " You would set the file share service to permission level to friend ( or higher ) and set messenger and about page to guest level permission."
    "\n"
    "\n"
    "=== GUEST PERMISSION ===\n"
    " A person granted this permission level can access plugins or services set to permission level Guest or Anybody.\n"
    " A example of usage would be granting a person guest permission even if not a member of your group."
    "\n"
    "\n"
    "=== ANYBODY PERMISSION ===\n"
    " This permission grants anybody that can connect to you access to plugin or service set to this level."
    " A example of usage would be setting STORY BOARD service to anybody so everyone can see your story."
    "\n"
    "\n"
    "=== NOBODY (DISABLED or IGNORED) PERMISSION ===\n"
    " This permission level when applied to a person means that this person will be ignored and cannot access"
    " any of your services or plugins.\n"
    " This permission level when applied to a plugin or service means that the plugin or sevice will be completely"
    " disabled and nobody can access that plugin or service.\n"
    "\n"
) );

QString AppletHelpNetSignalBars::m_NetworkKey( QObject::tr(
    "=== NETWORK KEY ===\n"
    "The network key is a text string used for person to person network encryption.\n"
    "The network key should only be changed if connecting to or hosting a private network seperate from NoLimitConnect.\n"
    "If the network key is changed then connecting to the NoLimitConnect network will no longer be possible.\n"
    "A private network can be hosted/connected to without changing the network key, however, changing the "
    "network key will give you the best privacy NoLimitConnect has to offer.\n"
    "\n"
    "NoLimitConnect has weak encryption and should NOT be considered secure."
    "If you truly need anonymity and a secure network you should consider products with security as"
    " the primary goal such as Tor.\n"
    "\n"
    "Use of a VPN is recommended to improve your privacy."
) );

QString AppletHelpNetSignalBars::m_NetworkHost( QObject::tr(
    "=== NETWORK HOST ===\n"
    "The network host provides group host listing and connection test services for a PtoP Network.\n"
    "The network host URL should only be changed if connecting to or hosting a private network seperate from NoLimitConnect.\n"
    "The network host URL can be one of two formats.\n"
    "\n"
    "Format 1 using host web name and port\n"
    " Example 1 ptop://www.nolimitconnect.net:45124\n"
    "\n"
    "Format 2 using host external IP Adrress and port\n"
    " Example 2 ptop://111.122.133.144:45124\n"
    "\n"
    "NOTE 1: The network host IP port is normally 45124 but can be any open port.\n"
    "NOTE 2: The network host address must eitehr be a fixed/permenent ip or be a web url name\n"
    " that can be resolved to a IP using DNS ( Domain Name Service ).\n"
) );

QString AppletHelpNetSignalBars::m_ConnectTestUrl( QObject::tr(
    "=== CONNECTION TEST URL ===\n"
    "The connection test service provides services to test if your device's port is open.\n"
    "If your port is open then others can connect directly to your device.\n"
    "A open port provides highest possible speed of connect and data transfer.\n"
    "A blocked port requires the device to use a relay service which greatly limits speed and number of connections possible.\n"
    "\n"
    "Format 1 using host web name and port\n"
    " Example 1 ptop://www.nolimitconnect.net:45124\n"
    "\n"
    "Format 2 using host external IP Adrress and port\n"
    " Example 2 ptop://111.122.133.144:45124\n"
    "\n"
) );

QString AppletHelpNetSignalBars::m_ConnectTestSettings( QObject::tr(
    "=== Enable UPNP check box ===\n"
    "If enabled then UPNP protocol will be used to attempt to open a port to your devcice\n"
    "Because of the various implementations of router firmware this only sometimes is succesfull in opening your port  \n"
    "Consult your router specific instructions for how to forward a port to your device or visit\n"
    "https://www.wikihow.com/Open-Ports\n\n"
    "=== CONNECTION TEST SETTINGS ===\n\n"
    "Option 1 - Use connection test service.\n"
    "This is the most reliable and recommended method of determining your external IP Address and\n"
    "determining if your device's port is open.\n"
    "\n"
    "Option 2 - Specify your external IP Address and assume your port is open.\n"
    "This setting is only recommended if you have a fixed IP Address because\n"
    "your IP Address may change at the whim of your Internet Service Provider.\n"
    "You can determine your IP Address by doing a google search for \"what is my ip address\"\n"
    "\n"
    "Option 3 - Always assume your device's port is blocked.\n"
    "This option is only recommended for users that always use mobile data service and do not use a VPN.\n"
    "It will somewhat speed up searches for relay services by not attempting to open port or testing your connection\n"
    "\n"
    "\n"
) );
//============================================================================
QString AppletHelpNetSignalBars::getInfoText( void )
{
    if( m_PluginType != ePluginTypeInvalid )
    {
        switch( m_PluginType )
        {
        case ePluginTypeHostGroup:
        case ePluginTypeGroupBlobing:
        case ePluginTypeHostNetwork:
            return m_NetworkDesign + m_PluginDefinitions;
            break;
        default:
            return m_NoInfoAvailable;
            break;
        }
    }

    switch( m_InfoType )
    {
    case eInfoTypePermission:
        return m_Permissions;
    case eInfoTypeNetworkKey:
        return m_NetworkKey;
    case eInfoTypeNetworkHost:
        return m_NetworkHost;
    case eInfoTypeConnectTestUrl:
        return m_ConnectTestUrl;
    case eInfoTypeConnectTestSettings:
        return m_ConnectTestSettings;

    default:
        return m_NoInfoAvailable;
    }
}
*/