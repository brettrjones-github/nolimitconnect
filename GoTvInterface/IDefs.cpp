#include "IDefs.h"

namespace
{

    const char * HostStatusEnumStrings[] = 
    { 
        "Host Status Unknown",
        "Host LogMsg",

        "Host OK",
        "Host Connect Fail",
        "Host Connection Dropped",
        "Host TestComplete",

        "NetService OK",
        "NetService Connect Fail",
        "NetService Connection Dropped",
        "NetService Test Complete",

        "Max Host Status",
    };

    const char * HostTypeEnumStrings[] =
    {
        "Host Unknown ",
        "Host Chat Room ",
        "Host Connect Test ",
        "Host Group ",
        "Host Network ",
        "Host Random Connect ",

        "Max Host Type ",
    };

    const char * InternetStatusEnumStrings[] =
    {
        "Internet: No Internet ",
        "Internet: Internet Available ",
        "Internet: No Connection Test Host ",
        "Internet: Connection Test Host Available ",
        "Internet: Assume Direct Connect ",
        "Internet: Can Direct Connect ",
        "Internet: Relay ",
        "Internet: Max Internet Status ",
    };

    const char * NetAvailStatusEnumStrings[] =
    {
        "Net Avail: Network Unavailable ",
        "Net Avail: Host Available ",
        "Net Avail: P2P Available ",
        "Net Avail: Online But Requires Relay ",
        "Net Avail: Online With Relay ",
        "Net Avail: Online And Can Direct Connect ",
        "Net Avail: Group Host ",
        "Net Avail: Max Net Avail Status ",
    };

    const char * NetCmdTypeEnumStrings[] =
    {
        "Net Cmd: Unknown ",
        "Net Cmd: Ping ",
        "Net Cmd: Pong ",
        "Net Cmd: Is Port Open Req ",
        "Net Cmd: Is Port Open Reply ",
        "Net Cmd: Host Req ",
        "Net Cmd: Host Reply ",
        "Net Cmd: About Me Page ",
        "Net Cmd: Storyboard Page ",
        "Net Cmd: Query Host Id Req ",
        "Net Cmd: Query Host Id Reply ",
    };

    const char * NetCmdErrorEnumStrings[] =
    {
        "Cmd Error: Unknown ",
        "Cmd Error: None ",
        "Cmd Error: Service Disabled ",
        "Cmd Error: Permission Level ",
    };

    const char * NetworkStateEnumStrings[] = 
    { 
        "State Unknown ",
        "Init ",
        "Test ", // eNetworkStateTypeAvail
        "Testing Connection ",
        "Relay Search ",
        "Announce To Network ",
        "Online Direct Connect",
        "Online Through Relay",
        "Get Relay List ",
        "No Internet Connection ",
        "Failed Resolve Host URL ",
        "Max Network State "
    };

    const char * RelayStatusEnumStrings[] = 
    { 
        "Relay Disconnected ",
        "Relay Connected ",
        "Searching For Relay ",
        "No Relays Listed ",
        "Relay Search ",
        "Relay List Exhausted ",
        "Relay Assume Firewalled ",
        "Relay Assume Can Direct Connect ",
        "Max Relay Status "
    };

    const char * PortOpenStatusEnumStrings[] = 
    { 
        "Port Open: Status Unknown",
        "Port Open: LogMsg",

        "Port Open: OK",
        "Port Open: Closed",
        "Port Open: Connect Fail",
        "Port Open: Connection Dropped",
        "Port Open: Invalid Response",
        "Port Open: Test Complete",

        "Max Port Open Status",
    };

    const char * RunTestStatusEnumStrings[] =
    {
        "Run Test: Status Unknown",
        "Run Test: LogMsg",

        "Run Test: Test Success",
        "Run Test: Test Fail",
        "Run Test: Bad Parameter",
        "Run Test: Already Queued",
        "Run Test: Connect Fail",
        "Run Test: Connection Dropped",
        "Run Test: Invalid Response",
        "Run Test: My Port Is Open",
        "Run Test: My Port Is Closed",
        "Run Test: Test Complete",
        "Run Test: Test Complete But Failed",
        "Run Test: Test Complete With Success",

        "Max Run Test Status",
    };

    const char * RandomConnectStatusEnumStrings[] = 
    { 
        "Phone Shake: Status Unknown ",
        "Phone Shake: Contact Host Failed ",

        "Phone Shake: Found Friend ",
        "Phone Shake: Empty List ",
        "Phone Shake: Send Request Fail ",
        "Phone Shake: Invalid Response ",
        "Phone Shake: Decrypt Error ",
        "Phone Shake: Search Complete ",

        "Max Phone Shake Status ",
    };

    const char * ENUM_BAD_PARM = "ENUM BAD PARAM ";
}

//============================================================================
const char * DescribeAppModule( EAppModule appModule )
{
    switch( appModule )
    {
    case eAppModuleInvalid:
        return "eAppModuleInvalid";
    case eAppModuleAll:
        return "eAppModuleAll";
    case eAppModuleKodi:
        return "eAppModuleKodi";
    case eAppModulePtoP:
        return "eAppModulePtoP";
    case eAppModuleTest:
        return "eAppModuleTest";
    default:
        return "UNKNOWN EAppModule";
    }
}

const char * DescribeHostStatus( EHostTestStatus eHostStatus )
{
    if( eHostStatus < 0 || eMaxHostTestStatusType <= eHostStatus )
    {
        return ENUM_BAD_PARM;
    }

    return HostStatusEnumStrings[ eHostStatus ];
}

const char * DescribeHostType( EHostType eHostType )
{
    if( eHostType < 0 || eMaxHostType <= eHostType )
    {
        return ENUM_BAD_PARM;
    }

    return HostTypeEnumStrings[ eHostType ];
}

//! Internet Status as text
const char * DescribeInternetStatus( EInternetStatus internetStatus )
{
    if( internetStatus < 0 || eMaxInternetStatus <= internetStatus )
    {
        return ENUM_BAD_PARM;
    }

    return InternetStatusEnumStrings[ internetStatus ];
}

//============================================================================
const char * DescribeModuleState( EModuleState moduleState )
{
    switch( moduleState )
    {
    case eModuleStateUnknown:
        return "eModuleStateUnknown";
    case eModuleStateInitialized:
        return "eModuleStateInitialized";
    case eModuleStateDeinitialized:
        return "eModuleStateDeinitialized";
    case eModuleStateInitError:
        return "eModuleStateInitError";
    default:
        return "UNKNOWN EModuleState";
    }
}

//! Net Available Status as text
const char * DescribeNetAvailStatus( ENetAvailStatus netAvailStatus )
{
    if( netAvailStatus < 0 || eMaxNetAvailStatus <= netAvailStatus )
    {
        return ENUM_BAD_PARM;
    }

    return NetAvailStatusEnumStrings[ netAvailStatus ];
}

const char * DescribeNetworkState( ENetworkStateType networkStateType )
{
    if( networkStateType < 0 || eMaxNetworkStateType <= networkStateType )
    {
        return ENUM_BAD_PARM;
    }

    return NetworkStateEnumStrings[ networkStateType ];
}

const char * DescribeRelayStatus( EMyRelayStatus eRelayStatus )
{
    if( eRelayStatus < 0 || eMaxMyRelayStatus <= eRelayStatus )
    {
        return ENUM_BAD_PARM;
    }

    return RelayStatusEnumStrings[ eRelayStatus ];
}

const char * DescribePortOpenStatus( EIsPortOpenStatus ePortOpenStatus )
{
    if( ePortOpenStatus < 0 || eMaxIsPortOpenStatusType <= ePortOpenStatus )
    {
        return ENUM_BAD_PARM;
    }

    return PortOpenStatusEnumStrings[ ePortOpenStatus ];
}

const char * DescribeRunTestStatus( ERunTestStatus eTestStatus )
{
    if( eTestStatus < 0 || eMaxRunTestStatusType <= eTestStatus )
    {
        return ENUM_BAD_PARM;
    }

    return RunTestStatusEnumStrings[ eTestStatus ];
}

const char * DescribeRandomConnectStatus( ERandomConnectStatus eRandomConnectStatus )
{
    if( eRandomConnectStatus < 0 || eMaxRandomConnectStatusType <= eRandomConnectStatus )
    {
        return ENUM_BAD_PARM;
    }

    return RandomConnectStatusEnumStrings[ eRandomConnectStatus ];
}

//! Net Command type as text
const char * DescribeNetCmdType( ENetCmdType netCmdType )
{
    if( netCmdType < 0 || eMaxNetCmdType <= netCmdType )
    {
        return ENUM_BAD_PARM;
    }

    return NetCmdTypeEnumStrings[ netCmdType ]; 
}

//! Net Command Error as text
const char * DescribeNetCmdError( ENetCmdError netCmdError )
{
    if(  netCmdError < 0 || eMaxNetCmdError <= netCmdError )
    {
        return ENUM_BAD_PARM;
    }

    return NetCmdErrorEnumStrings[ netCmdError ]; 
}

//============================================================================
// for use in database mainly
const char * getPluginName( EPluginType pluginType )
{
    switch( pluginType )
    {
    case ePluginTypeInvalid: return "ePluginTypeInvalid";
    case ePluginTypeAdmin: return "ePluginTypeAdmin";
    case ePluginTypeAboutMePage: return "ePluginTypeAboutMePage";
    case ePluginTypeAvatarImage: return "ePluginTypeAvatarImage";
    case ePluginTypeCamServer: return "ePluginTypeCamServer";
    case ePluginTypeChatRoomClient: return "ePluginTypeChatRoomClient";
    case ePluginTypeServiceConnectTest: return "ePluginTypeServiceConnectTest";
    case ePluginTypeClientConnectTest: return "ePluginTypeClientConnectTest";
    case ePluginTypeFileServer: return "ePluginTypeFileServer";
    case ePluginTypeFileXfer: return "ePluginTypeFileXfer";
    case ePluginTypeHostChatRoom: return "ePluginTypeHostChatRoom";
    case ePluginTypeHostGroup: return "ePluginTypeHostGroup";
    case ePluginTypeHostGroupListing: return "ePluginTypeHostGroupListing";
    case ePluginTypeHostNetwork: return "ePluginTypeHostNetwork";
    case ePluginTypeMessenger: return "ePluginTypeMessenger";
    case ePluginTypeRandomConnect: return "ePluginTypeRandomConnect";
    case ePluginTypeRandomConnectRelay: return "ePluginTypeRandomConnectRelay";
    case ePluginTypeRelay: return "ePluginTypeRelay";
    case ePluginTypeStoryboard: return "ePluginTypeStoryboard";
    case ePluginTypeTruthOrDare: return "ePluginTypeTruthOrDare";
    case ePluginTypeVideoPhone: return "ePluginTypeVideoPhone";
    case ePluginTypeVoicePhone: return "ePluginTypeVoicePhone";
    case eMaxUserPluginType: return "eMaxUserPluginType";
    case ePluginTypeCameraService: return "ePluginTypeCameraService";
    case ePluginTypeMJPEGReader: return "ePluginTypeMJPEGReader";
    case ePluginTypeMJPEGWriter: return "ePluginTypeMJPEGWriter";
    case ePluginTypePersonalRecorder: return "ePluginTypePersonalRecorder";
    case ePluginTypeNetServices: return "ePluginTypeNetServices";
    case ePluginTypeSearch: return "ePluginTypeSearch";
    case ePluginTypeSndReader: return "ePluginTypeSndReader";
    case ePluginTypeSndWriter: return "ePluginTypeSndWriter";
    case ePluginTypeWebServer: return "ePluginTypeWebServer";

    default:
        return "UnknownPlugin";
    }
}
