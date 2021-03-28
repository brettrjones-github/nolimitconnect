#include "IDefs.h"

namespace
{
    const char * CommErrEnumStrings[] =
    {
        "No Error ",
        "Comm Error Invalid Packet ",
        "Comm Error User Offline ",
        "Comm Error Search Text To Short ",
        "Comm Error Search No Match ",
        "Comm Error Invalid Host Type ",
        "Comm Error Plugin Not Enabled ",
        "Comm Error Plugin Permission Level ",
        "Max Comm Error ",
    };

    const char * ConnectReasonEnumStrings[] = 
    { 
        "Connect Reason Unknown ",
        "Connect Reason Chat RoomAnnounce ",
        "Connect Reason Chat Room Join ",
        "Connect Reason Chat Room Search ",
        "Connect Reason Group Announce ",
        "Connect Reason Group Join ",
        "Connect Reason Group Search ",

        "Connect Reason Random Connect Announce ",
        "Connect Reason Random Connect Join ",
        "Connect Reason Random Connect Search ",  
        "Connect Reason Relay Announce ",
        "Connect Reason Relay Join ",
        "Connect Reason Relay Search ",

        "Connect Reason Announce Ping ",  
        "Connect Reason Stay Connected ",
        "Connect Reason Plugin ",  
        "Connect Reason Other Search ",

        "Max Connect Reason ",
    };

    const char * ConnectStatusEnumStrings[] = 
    { 
        "Connect Status Unknown ",

        "Connect Status Ready ",
        "Connect Status Bad Param ",
        "Connect Status Bad Address ",
        "Connect Status Permission Denied ",
        "Connect Status Connecting ",
        "Connect Status Connect Failed ",
        "Connect Status Send PktAnn Failed ",
        "Connect Status Handshake ",
        "Connect Status Connect Success ",
        "Connect Status Dropped ",
        "Connect Status Rx Ann Timeout ",

        "Max Connect Status ",
    };

    const char * HackerLevelEnumStrings[] =
    {
        "Hacker Level Unknown ",
        "Hacker Level Suspicious ",
        "Hacker Level Medium ",
        "Hacker Level Severe ",

        "Max Hacker Level ",
    };

    const char * HostAnnounceStatusEnumStrings[] = 
    { 
        "Host Announce Status Unknown ",
        "Host Announce Invalid Url ",
        "Host Announce Querying Id In Progress ",
        "Host Announce Querying Id Success ",
        "Host Announce Query Id Failed ",
        "Host Announce Connecting ",
        "Host Announce HandShaking ",
        "Host Announce HandShake Timeout ",
        "Host Announce Connect Success ",
        "Host Announce Connect Failed ",
        "Host Announce Sending Request ",
        "Host Announce Send Request Failed ",
        "Host Announce Success ",
        "Host Announce Failed ",
        "Host Announce Fail Permission ",  
        "Host Announce Fail Connect Dropped ",
        "Host Announce Invalid Param ",
        "Host Announce Plugin Disabled ",
        "Host Announce Done ",

        "Max Host Announce Status ",
    };

    const char * HostJoinStatusEnumStrings[] = 
    { 
        "Host Join Status Unknown ",
        "Host Join Invalid Url ",
        "Host Join Querying Id In Progress ",
        "Host Join Querying Id Success ",
        "Host Join Query Id Failed ",
        "Host Join Connecting ",
        "Host Join HandShaking ",
        "Host Join HandShake Timeout ",
        "Host Join Connect Success ",
        "Host Join Connect Failed ",
        "Host Join Sending Request ",
        "Host Join Send Request Failed ",
        "Host Join Success ",
        "Host Join Failed ",
        "Host Join Fail Permission ",  
        "Host Join Fail Connect Dropped ",
        "Host Join Invalid Param ",
        "Host Join Plugin Disabled ",
        "Host Join Done ",

        "Max Host Join Status ",
    };

    const char * HostSearchStatusEnumStrings[] = 
    { 
        "Host Search Status Unknown ",
        "Host Search Invalid Url ",
        "Host Search Querying Id In Progress ",
        "Host Search Querying Id Success ",
        "Host Search Query Id Failed ",
        "Host Search Connecting ",
        "Host Search HandShaking ",
        "Host Search HandShake Timeout ",
        "Host Search Connect Success ",
        "Host Search Connect Failed ",
        "Host Search Sending Request ",
        "Host Search Send Request Failed ",
        "Host Search Success ",
        "Host Search Failed ",
        "Host Search Fail Permission ",  
        "Host Search Fail Connect Dropped ",
        "Host Search Invalid Param ",
        "Host Search Plugin Disabled ",
        "Host Search No Matches ",
        "Host Search Completed ",
        "Host Search Done ",

        "Max Host Search Status ",
    };

    const char * HostTypeEnumStrings[] =
    {
        "Host Unknown ",
        "Host Chat Room ",
        "Host Connect Test ",
        "Host Group ",
        "Host Network ",
        "Host Random Connect ",
        "Host Peer User ",

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

    const char * PluginAccessStateEnumStrings[] = 
    {
        "Access: Unknown ",
        "Access: Granted ",
        "Access: Insufficient Permission ",
        "Access: Service Disabled ",
        "Access: Ignored ",
        "Access: Inactive ",
        "Access: Busy ",
        "Access: Requires Direct Connect ",
        "Access: Requires Online ",
        "Max Plugin Access ",
    };

    const char * PluginTypeEnumStrings[] = 
    {
        "Unknown Plugin ",
        "Admin ",
        "About Me Page ",
        "Avatar Image ",
        "Shared Web Cam ",
        "Shared Files ",
        "Send A File ",    
        "Messenger ",
        "Story Board ", 
        "Play Truth Or Dare ",
        "Video Chat ",
        "Voice Call ",
        "Chat Room Client ",
        "Chat Room Host ",
        "Connect Test Client ",
        "Connect Test Host ",
        "Group Client ",
        "Group Host ",
        "Random Connect Client ",
        "Random Connect Host ",
        "Network Client ",
        "Network Host ",
        "Network Search ",
        "Relay ",
        "Max Public Plugin ",
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

    const char * ScanTypeEnumStrings[] = 
    { 
        "Scan None ",
        "Chat Room Host Join Search ",
        "Group Host Join Search ",
        "Random Connect Join ",
        "Person Name ",
        "Mood Message ",
        "Profile Picture ",
        "Cam Server ",
        "File Search ",
        "Scan Story Boards ",

        "Max Scan Type ",
    };

    const char * SearchTypeEnumStrings[] =
    {
        "Search Type None ",
        "Search For Chat Room Host ",	        //!< Search for Chat Room to Join
        "Search For Group Host ",	            //!< Search for Group to Join
        "Search For Random Connect Host ",	//!< Search for Random Connect Server to Join

        "MaxSearchNone"
    };

    const char * SktTypeEnumStrings[] =
    {
        "Unknown Socket ",
        "Connect Socket ",
        "Accept Socket ",
        "UDP Socket ",
        "UDP Broadcast Socket ",

        "eMaxSktType"
    };

    const char * ENUM_BAD_PARM = "ENUM BAD PARAM ";
}

//============================================================================
const char * DescribeAge( EAgeType ageType )
{
    if( ageType < 0 || eMaxAgeType <= ageType )
    {
        return ENUM_BAD_PARM;
    }

    switch( ageType )
    {
    case eAppModuleInvalid:
        return "Any";
    case eAppModuleAll:
        return "Under 21 ";
    case eAppModuleKodi:
        return "21 Or Older ";

    default:
        return "UNKNOWN Age ";
    }
}

//============================================================================
const char * DescribeCommError( ECommErr commErr )
{
    if( commErr < 0 || eMaxCommErr <= commErr )
    {
        return ENUM_BAD_PARM;
    }

    return CommErrEnumStrings[ commErr ];
}

//============================================================================
const char * DescribeAppModule( EAppModule eAppModule )
{
    if( eAppModule < 0 || eMaxAppModule <= eAppModule )
    {
        return ENUM_BAD_PARM;
    }

    switch( eAppModule )
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

//============================================================================
const char * DescribeConnectReason( EConnectReason eConnectReason )
{
    if( eConnectReason < 0 || eMaxConnectReason <= eConnectReason )
    {
        return ENUM_BAD_PARM;
    }

    return ConnectReasonEnumStrings[ eConnectReason ];
}

//============================================================================
const char * DescribeConnectStatus( EConnectStatus eConnectStatus )
{
    if( eConnectStatus < 0 || eMaxConnectStatus <= eConnectStatus )
    {
        return ENUM_BAD_PARM;
    }

    return ConnectStatusEnumStrings[ eConnectStatus ];
}


//============================================================================
//! describe friend state
const char * DescribeFriendState( EFriendState eFriendState )
{
    switch( eFriendState )
    {
    case eFriendStateAnonymous:	// anonymous user
        return "Anonymous ";
    case eFriendStateGuest:		// guest user
        return "Guest ";
    case eFriendStateFriend:	// friend user
        return "Friend ";
    case eFriendStateAdmin:		// administrator
        return "Administrator ";
    default:
        return "Ignore ";
    }
}

//============================================================================
const char * DescribeHackerLevel( EHackerLevel hackLevel )
{
    if( hackLevel < 0 || eMaxHackerLevel <= hackLevel )
    {
        return ENUM_BAD_PARM;
    }

    return HackerLevelEnumStrings[ hackLevel ];
}

//============================================================================
const char * DescribeHostAnnounceStatus( EHostAnnounceStatus  hostStatus )
{
    if( hostStatus < 0 || eMaxHostAnnounceStatus <= hostStatus )
    {
        return ENUM_BAD_PARM;
    }

    return HostAnnounceStatusEnumStrings[ hostStatus ];
}

//============================================================================
const char * DescribeHostJoinStatus( EHostJoinStatus hostStatus )
{
    if( hostStatus < 0 || eMaxHostJoinStatus <= hostStatus )
    {
        return ENUM_BAD_PARM;
    }

    return HostJoinStatusEnumStrings[ hostStatus ];
}

//============================================================================
const char * DescribeHostSearchStatus( EHostSearchStatus hostStatus )
{
    if( hostStatus < 0 || eMaxHostSearchStatus <= hostStatus )
    {
        return ENUM_BAD_PARM;
    }

    return HostSearchStatusEnumStrings[ hostStatus ];
}

//============================================================================
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
const char * DescribePluginAccess( EPluginAccess pluginAccess )
{
    if(  pluginAccess < 0 || eMaxPluginAccessState <= pluginAccess )
    {
        return ENUM_BAD_PARM;
    }

    return PluginAccessStateEnumStrings[ pluginAccess ]; 
}

//============================================================================
const char * DescribePluginType( EPluginType plugType )
{
    if(  plugType < 0 || eMaxImplementedPluginType <= plugType )
    {
        return ENUM_BAD_PARM;
    }

    return PluginTypeEnumStrings[ plugType ]; 
}

//============================================================================
const char * DescribeScanType( EScanType scanType )
{
    if(  scanType < 0 || eMaxScanType <= scanType )
    {
        return ENUM_BAD_PARM;
    }

    return ScanTypeEnumStrings[ scanType ]; 
}

//============================================================================
const char * DescribeSearchType( ESearchType searchType )
{
    if(  searchType < 0 || eMaxSearchType <= searchType )
    {
        return ENUM_BAD_PARM;
    }

    return SearchTypeEnumStrings[ searchType ]; 
}

//============================================================================
//! Describe skt type
const char * DescribeSktType( ESktType sktType )
{
    if(  sktType < 0 || eMaxSktType <= sktType )
    {
        return ENUM_BAD_PARM;
    }

    return SktTypeEnumStrings[ sktType ]; 
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
    case ePluginTypeChatRoomHost: return "ePluginTypeChatRoomHost";
    case ePluginTypeConnectTestHost: return "ePluginTypeConnectTestHost";
    case ePluginTypeConnectTestClient: return "ePluginTypeConnectTestClient";
    case ePluginTypeFileServer: return "ePluginTypeFileServer";
    case ePluginTypeFileXfer: return "ePluginTypeFileXfer";
    case ePluginTypeGroupHost: return "ePluginTypeGroupHost";
    case ePluginTypeNetworkSearchList: return "ePluginTypeNetworkSearchList";
    case ePluginTypeNetworkHost: return "ePluginTypeNetworkHost";
    case ePluginTypeMessenger: return "ePluginTypeMessenger";
    case ePluginTypeRandomConnectClient: return "ePluginTypeRandomConnectClient";
    case ePluginTypeRandomConnectHost: return "ePluginTypeRandomConnectHost";
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
