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

#include "ActivityInformation.h"
#include "AppCommon.h"
#include "AppSettings.h"

#include <CoreLib/VxDebug.h>

#include <QClipboard>

//============================================================================
ActivityInformation::ActivityInformation( AppCommon& app, QWidget * parent, EPluginType pluginType )
: ActivityBase( OBJNAME_ACTIVITY_INFORMATION, app, parent, eAppletActivityDialog )
, m_PluginType( pluginType )
{
    initActivityInformation();
}

//============================================================================
ActivityInformation::ActivityInformation( AppCommon& app, QWidget * parent, EInfoType infoType )
    : ActivityBase( OBJNAME_ACTIVITY_INFORMATION, app, parent, eAppletActivityDialog )
    , m_InfoType( infoType )
{
    initActivityInformation();
}

//============================================================================
void ActivityInformation::initActivityInformation( void )
{
    ui.setupUi( this );
    ui.m_TitleBarWidget->setTitleBarText( QObject::tr( "Information " ) );
    ui.m_TitleBarWidget->setHomeButtonVisibility( false );
    connect( ui.m_ClipboardButton, SIGNAL( clicked() ), this, SLOT( slotCopyToClipboardButtonClicked() ) );
    connect( ui.m_ClipboardIconButton, SIGNAL( clicked() ), this, SLOT( slotCopyToClipboardButtonClicked() ) );

    connectBarWidgets();
}

//============================================================================
void ActivityInformation::showEvent( QShowEvent * ev )
{
    ActivityBase::showEvent( ev );
    updateInformation();
}

//============================================================================
void ActivityInformation::slotCopyToClipboardButtonClicked( void )
{
    QClipboard * clipboard = QApplication::clipboard();
    clipboard->setText( ui.m_InfoText->toPlainText() );
    okMessageBox( QObject::tr( "Clipboard" ), QObject::tr( "Text was copied to clipboard" ) );
}

//============================================================================
void ActivityInformation::updateInformation( void )
{
    ui.m_PictureLabel->setVisible( false );
    ui.m_ServiceInfoButton->setFixedSize( eButtonSizeLarge );
    ui.m_ClipboardIconButton->setFixedSize( eButtonSizeMedium );
    ui.m_ClipboardIconButton->setIcon( eMyIconEditCopy );
    ui.m_WebsiteUrlLabel->setText( "http://www.nolimitconnect.com" );

    if( m_PluginType != ePluginTypeInvalid )
    {
        ui.m_ServiceInfoButton->setIcon( m_MyApp.getMyIcons().getPluginIcon( m_PluginType ) );

        switch( m_PluginType )
        {
        case ePluginTypeHostChatRoom:
        case ePluginTypeHostGroup:
        case ePluginTypeNetworkSearchList:
        case ePluginTypeHostNetwork:
        case ePluginTypeHostRandomConnect:
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
    case eInfoTypeRandomConnectUrl:
        ui.m_ServiceInfoButton->setIcon( eMyIconServiceRandomConnect );
        break;
    case eInfoTypeDefaultGroupHostUrl:
        ui.m_ServiceInfoButton->setIcon( eMyIconSettingsHostGroup );
        break;
    case eInfoTypeDefaultChatRoomHostUrl:
        ui.m_ServiceInfoButton->setIcon( eMyIconSettingsChatRoom );
        break;
    default:
        ui.m_ServiceInfoButton->setIcon( eMyIconInformation );
        break;
    }

    ui.m_InfoText->clear();
    ui.m_InfoText->appendPlainText( getInfoText() );
}

QString ActivityInformation::m_NoInfoAvailable( QObject::tr( "No Information is localy available. please visit http://wwww.nolimitconnect.com for latest infomation and help" ) );

QString ActivityInformation::m_NetworkDesign( QObject::tr(
    "=== NETWORK DESIGN ===\n"
    "NOTE: A VPN with port forwarding feature is suggested\n"
    "1.) An Open Port is required for Hosting and recommended for direct connection between users\n"
    "2.) If you plan to host a separate network then a VPN with a fixed address is recommended\n"
    "Pure VPN is a VPN with Port Forwarding and fixed IP Address as pay for add-in features\n"
    " *USER HOST LISTING SERVICE: Provides List of Group, Chat Room and Random Connect Hosts and thier address for users to connect to and search or join.\n"
    " *CONNECTION TEST SERVICE: Service for devices to discover thier web IP Address and if they have a open port for direct connection or require RELAY SERVICE*.\n"
    "\n"
    "=== TIER 1: NOLIMITCONNECT NETWORK HOST SERVICE* ===\n"
    "NOTE: This service requires a fixed IP Address or a DNS url like http://www.nolimitconnect.net\n"
    "REQUIRED SERVICES* AND SUGGESTED PERMISSION LEVELS*\n"
    " NOLIMITCONNECT NETWORK SERVICE - ANYBODY\n"
    " CONNECTION TEST SERVICE - ANYBODY\n"
    "\n"
    "=== TIER 2: USER HOSTED SERVICES* ===\n"
    "NOTE: Users Require and open port to provide hosting\n"
    " GROUP HOSTING - provided communication services for members of the group\n"
    " CHAT ROOM HOSTING - provides chat room for members\n"
    " RANDOM CONNECT HOSTING - provides a way for anyone to connect to anyone (Recommend Anybody Permission Level)\n"
    "\n"
    "=== TIER 3: PERSON TO PERSON Services* ===\n"
    " Friends that can direct connect with open port will have faster communication and less loading of the host\n"
    " Those that cannot direct connect will use whichever host they are connected to as a relay\n"
    " When using relay a user can only communicate with another member of thier host while member is connected to host\n"
    "\n"
    "Person To Person and Person To Host Members And Friends services\n"
    " *ABOUT PAGE SERVICE - Provide a information page about a host or person\n"
    " *CAM STREAM SERVICE: A host or user can provide streaming video live from their device camera or cam\n"
    " *FILE SHARE PLUGIN: Provides user or host file sharing with other users.\n"
    " *FILE TRANSFER PLUGIN: Provides user file transfer to another person.\n"
    " *PUSH TO TALK PLUGIN: Provides user with voice communiction with one button push to talk.\n"
    " *STORY BOARD PAGE SERVICE - Provide a blog like page a host or person can post to for others to view/read\n"
    " *TRUTH OR DARE PLUGIN: provides truth or dare video chat game between individuals\n"
    " *VIDEO CHAT PLUGIN: Provides user video chat to others using the NoLimitConnect app and internet.\n"
    " *VOICE PHONE PLUGIN: Provides user with voice phone calling to others using the NoLimitConnect app and internet.\n"
    "\n"

    "\n"
) );

QString ActivityInformation::m_PluginDefinitions( QObject::tr(
    "\n"
"DEFINITIONS:\n"
" *ABOUT PAGE SERVICE - Provide a information page about a host or person\n"
" *CAM STREAM SERVICE: A host or user can provide streaming video live from their device camera or cam\n"
" *FILE SHARE PLUGIN: Provides user or host file sharing with other users.\n"
" *FILE TRANSFER PLUGIN: Provides user file transfer to another person.\n"
" *CHAT ROOM HOST SERVICE: Provides group based text chat service.\n"
" *GROUP HOST SERVICE: Provides connection and other services to members who have joined the Group Host.\n"
" *NOLIMITCONNECT NETWORK HOST SERVICE: Provides Connection test and user host listing for search of hosts to join.\n"
" *PERMISSION LEVELS: Friendship level required to be allowed to use a service or connect to a person.\n"
" *PUSH TO TALK PLUGIN: Provides user with voice communiction with one button push to talk.\n"
" *RANDOM CONNECT HOST SERVICE: Provides users who join to communicate with others.\n"
" *SEARCH AND SCAN SERVICES: provide user a list of known persons with the desired search critera or service or stream or files\n"
" *STORY BOARD PAGE SERVICE - Provide a blog like page a host or person can post to for others to view/read\n"
" *TRUTH OR DARE PLUGIN: provides truth or dare video chat game between individuals\n"
" *VIDEO CHAT PLUGIN: Provides user video chat to others using the NoLimitConnect app and internet.\n"
" *VOICE PHONE PLUGIN: Provides user with voice phone calling to others using the NoLimitConnect app and internet.\n"
) );

QString ActivityInformation::m_Permissions( QObject::tr(
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

QString ActivityInformation::m_NetworkKey( QObject::tr(
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

QString ActivityInformation::m_NetworkHost( QObject::tr(
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

QString ActivityInformation::m_ConnectTestUrl( QObject::tr(
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

QString ActivityInformation::m_ConnectTestSettings( QObject::tr(
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

QString ActivityInformation::m_RandomConnectUrl( QObject::tr(
    "=== RANDOM CONNECT SERVICE URL ===\n"
    "Provides Service of listing/connecting 2 Persons using the random connect feature.\n"
    "The person listed could be anyone in the world also using the random connect feature.\n"
    "Random connect works by listing persons who pressed the random connect button.\n"
    "Within 20 seconds of the random connect button press.\n"
) );

QString ActivityInformation::m_DefaultGroupHostUrl( QObject::tr(
    "=== Default GROUP HOST URL ===\n"
    "No Limit Connect will attempt to connect the this group\n"
    "When Log In is completed.\n"
    "If connect fails or no url is provided then you can search for a Group to join\n"
) );

QString ActivityInformation::m_DefaultChatRoomHostUrl( QObject::tr(
    "=== Default CHAT ROOM HOST URL ===\n"
    "No Limit Connect will attempt to connect the this Chat Room\n"
    "When Log In is completed.\n"
    "If connect fails or no url is provided then you can search for a Chat Room to join\n"
) );

QString ActivityInformation::m_NetworkSettingsInvite( QObject::tr(
    "=== Network Settings Invite ===\n"
    "Accepting A Network Settings Invite that changes the Network Host URL\n"
    "Or If user changes the Network Key then\n"
    "The user will no longer be able to connect to No Limit Connect Network.\n"
    "You should only change the Network Host URL and Network Key if you have\n"
    "A private network with a private Network Host server\n"
) );

QString ActivityInformation::m_FriendList( QObject::tr(
    "=== Friends List ===\n"
    "A list showing users set to friend or administrator permission level.\n"
) );

QString ActivityInformation::m_IgnoredList( QObject::tr(
    "=== Ignored List ===\n"
    "A list showing ignored (blocked) users.\n"
    "You can unblock a user by clicking the friendship icon or select Change Friendship from the menue button on right side of list entry.\n"
) );

QString ActivityInformation::m_NearbyList( QObject::tr(
    "=== Nearby Users List ===\n"
    "A list showing users on the same Wireless or LAN network that are also showing the nearby list.\n"
    "An easy way to get connected without being in the same group is to connect his/her device on your home Wireless or LAN network.\n"
) );

QString ActivityInformation::m_UserHostRequrements( QObject::tr(
    "=== Hosting Requirements any of these host services Chat Room, Group, Random Connect ===\n"
    "You will need to port forward the listen port you specified in Network Settings\n"
    "Articles about port forwarding can be found by search engine or at https://www.jguru.com/vpn-port-forwarding \n"
    "The author of No Limit Connect used PureVPN because it has port forwarding and also fixed ip address (additional costs of course)  .\n"
) );

QString ActivityInformation::m_NetworkHostRequrements( QObject::tr(
    "=== Hosting Requirements Network Host ===\n"
    "If you want to host your own network the network host must have port forwarding and also a fixed ip address.\n"
) );

//============================================================================
QString ActivityInformation::getInfoText( void )
{
    if( m_PluginType != ePluginTypeInvalid )
    {
        switch( m_PluginType )
        {
        case ePluginTypeHostChatRoom:
        case ePluginTypeHostGroup:
        case ePluginTypeNetworkSearchList:
        case ePluginTypeHostNetwork:
        case ePluginTypeHostRandomConnect:
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
    case eInfoTypeRandomConnectUrl:
        return m_RandomConnectUrl;
    case eInfoTypeDefaultGroupHostUrl:
        return m_DefaultGroupHostUrl;
    case eInfoTypeDefaultChatRoomHostUrl:
        return m_DefaultChatRoomHostUrl;
    case eInfoTypeNetworkSettingsInvite:
        return m_NetworkSettingsInvite;

    case eInfoTypeFriendsList:
        return m_FriendList;
    case eInfoTypeIgnoredList:
        return m_IgnoredList;
    case eInfoTypeNearbyList:
        return m_NearbyList;

    case eInfoTypeGroupStatus:
    case eInfoTypeHostChatRoom:
    case eInfoTypeHostGroup:
    case eInfoTypeHostRandomConnect:
        return m_UserHostRequrements;
    case eInfoTypeHostNetwork:
        return m_NetworkHostRequrements + m_UserHostRequrements;
    default:
        return m_NoInfoAvailable;
    }
}
