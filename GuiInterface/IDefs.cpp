#include "IDefs.h"
#include <CoreLib/VxDebug.h>

namespace
{
    const char* ENUM_BAD_PARM = "ENUM BAD PARAM ";

    const char* CommErrEnumStrings[] =
    {
        "No Error ",
        "Comm Error Invalid Packet ",
        "Comm Error User Offline ",
        "Comm Error Search Text To Short ",
        "Comm Error Search Text To Long ",
        "Comm Error Search No Match ",
        "Comm Error Invalid Host Type ",
        "Comm Error Plugin Not Enabled ",
        "Comm Error Plugin Permission Level ",
        "Comm Error Not Found ",
        "Comm Error Invalid Param ",
        "Max Comm Error ",
    };

    const char* ConnectReasonEnumStrings[] =
    { 
        "Connect Reason Unknown ",

        "Connect Reason Group Announce ",
        "Connect Reason Group Join ",
        "Connect Reason Group Leave ",
        "Connect Reason Group UnJoin ",
        "Connect Reason Group Search ",
        "Connect Reason Group User Connect ",

        "Connect Reason Chat RoomAnnounce ",
        "Connect Reason Chat Room Join ",
        "Connect Reason Chat Room Leave ",
        "Connect Reason Chat Room UnJoin ",
        "Connect Reason Chat Room Search ",
        "Connect Reason Chat Room User Connect ",

        "Connect Reason Random Connect Announce ",
        "Connect Reason Random Connect Join ",
        "Connect Reason Random Connect Leave ",
        "Connect Reason Random Connect UnJoin ",
        "Connect Reason Random Connect Search ",  
        "Connect Reason Random Connect User Connect ",

        "Connect Reason Announce Ping ",  
        "Connect Reason Stay Connected ",
        "Connect Reason Plugin ",  
        "Connect Reason Other Search ",

        "Connect Reason Nearby LAN ",
        "Connect Reason Same External Ip ",
        "Connect Reason Reverse Connect Requested ",
        "Connect Reason Pkt Tcp Punch ",
        "Connect Reason Relay Service ",

        "Connect Reason Request Identity ",
        "Connect Reason Network Host List Search",

        "Connect Reason Group User List Search",
        "Connect Reason Chat Room User List Search",
        "Connect Reason Random Connect User List Search",

        "Connect Reason Group Groupie List Search",
        "Connect Reason Chat Room Groupie List Search",
        "Connect Reason Random Connect Groupie List Search",

        "Connect Reason User Relayed Connect",
        "Connect Reason User Direct Connect",
        "Connect Reason Network Host",
        "Connect Reason Connect Test",

        "Connect Reason Push To Talk",

        "Max Connect Reason ",
    };

    const char* ConnectStatusEnumStrings[] =
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

    const char* HackerLevelEnumStrings[] =
    {
        " >? Hacker Level Unknown ",
        " >! Hacker Level Suspicious ",
        " >!! Hacker Level Medium ",
        " >!!! Hacker Level Severe ",

        "Max Hacker Level ",
    };

    const char* HackerReasonEnumStrings[] =
    {
        "Hacker Reason Unknown ",
        "Reason Peer Name ",
        "Reason Host By Name ",
        "Reason No Host Ip Addr ",
        "Reason Host Ip Options ",
        "Reason Net Cmd Length ",
        "Reason Net Cmd List Invalid ",
        "Reason Net Srv Url Invalid",
        "Reason Net Srv Plugin Invalid",
        "Reason Net Srv Query Id Permission ",
        "Reason Http Attack ",
        "Reason Pkt Online Id Me From My Ip ",
        "Reason Pkt Online Id Me From Another Ip ",
        "Reason PktAnn Not First Packet ",
        "Reason PktHdr Invalid ",
        "Reason Pkt Invalid ",

        "Max Hacker Reason ",
    };

    const char* HostAnnounceStatusEnumStrings[] =
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

    const char* HostJoinStatusEnumStrings[] =
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

        "Host Join Fail Permission ",
        "Host Join Fail Connect Dropped ",
        "Host Join Invalid Param ",
        "Host Join Plugin Disabled ",
        "Host Join Done ",

        "Host Join Sending Request ",
        "Host Join Send Request Failed ",
        "Host Join Success ",
        "Host Join Failed ",

        "Host Leave Sending Request ",
        "Host Leave Send Request Failed ",
        "Host Leave Success ",
        "Host Leave Failed ",

        "Host UnJoin Sending Request ",
        "Host UnJoin Send Request Failed ",
        "Host UnJoin Success ",
        "Host UnJoin Failed ",

        "Max Host Join Status ",
    };

    const char* HostSearchStatusEnumStrings[] =
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

    const char* HostTypeEnumStrings[] =
    {
        // must sync with EHostType
        "Host Unknown ",
        "Host Connect Test ",
        "Host Network ",
        "Host Group ",
        "Host Chat Room ",
        "Host Random Connect ",
        "Host Peer User Direct",

        "Max Host Type ",
    };

    const char* InternetStatusEnumStrings[] =
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

    const char* JoinStateEnumStrings[] =
    {
        "Join: No State ",
        "Join: Sending ",
        "Join: Send Failed ",
        "Join: Send Acknowleged ",
        "Join: Requested ",
        "Join: Was Granted ",
        "Join: Is Granted ",
        "Join: Denied ",
        "Join: Leave Host ",
        "Join: Max JoinState ",
    };

    const char* NetAvailStatusEnumStrings[] =
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

    const char* NetCmdTypeEnumStrings[] =
    {
        "Net Cmd: Unknown ",
        "Net Cmd: Ping ",
        "Net Cmd: Pong ",
        "Net Cmd: Is Port Open Req ",
        "Net Cmd: Is Port Open Reply ",
        "Net Cmd: Query Host Id Req ",
        "Net Cmd: Query Host Id Reply ",
        "Net Cmd: Host Ping Req ",
        "Net Cmd: Host Ping Reply ",
    };

    const char* NetCmdErrorEnumStrings[] =
    {
        "Cmd Error: Unknown ",
        "Cmd Error: None ",
        "Cmd Error: Service Disabled ",
        "Cmd Error: Permission Level ",
        "Cmd Error: Failed Resolve IP Address ",
        "Cmd Error: Invalid Content ",
        "Cmd Error: Port Is Closed ",
        "Cmd Error: Connect Failed ",
        "Cmd Error: Transmit Failed ",
        "Cmd Error: Recieve Failed ",
        "Cmd Error: Invalid Parameter ",
        "Cmd Error: Response Timed Out ",
    };

    const char* NetworkStateEnumStrings[] = 
    { 
        "Network State Unknown ",   // 0
        "Network State Lost ",      // 1
        "Network State Available ", // 2 eNetworkStateTypeAvail
        "Network State Testing Connection ",        // 3 eNetworkStateTypeTestConnection
        "Network State Relay Search ",              // 4 eNetworkStateTypeRelaySearch
        "Network State Announce To Network ",       // 5 eNetworkStateTypeAnnounce
        "Network State Online Direct Connect",      // 6 eNetworkStateTypeAnnounce
        "Network State Online Through Relay",       // 7 eNetworkStateTypeOnlineThroughRelay
        "Network State Get Relay List ",            // 8 eNetworkStateTypeGetRelayList
        "Network State No Internet Connection ",    // 9 eNetworkStateTypeNoInternetConnection
        "Network State Failed Resolve Network Host URL ", // 10 eNetworkStateTypeFailedResolveHostNetwork
        "Network State Failed Resolve Group List Host URL ", // 11 eNetworkStateTypeFailedResolveHostGroupList
        "Network State Failed Resolve Group Host URL ", // 12 eNetworkStateTypeFailedResolveHostGroup
        "Max Network State ",
    };

    const char* RelayErrEnumStrings[] =
    {
        "Relay Error None ",                    // 0 eRelayErrNone
        "Relay Error Source Not Joined ",       // 1 eRelayErrSrcNotJoined
        "Relay Error Destination Not Joined ",  // 2 eRelayErrDestNotJoined
        "Relay Error User Not Online ",         // 3 eRelayErrUserNotOnline
        "Max Relay Error ",
    };

    const char* OfferStateEnumStrings[] =
    {
        "No Offer ",
        "Sending Offer ",
        "Offer Sent ",
        "Offer Send Failed ",
        "Offer Recieved By User ",
        "User Is Busy ",
        "Offer Accepted ",
        "Offer Rejected ",
        "Offer Was Canceled ",
        "User Is Offline ",
        "Offer Is In Session ",
        "Offer Session Complete ",
        "Offer Session Failed ",
        "Max Offer State ",
    };

    const char* OfferTypeEnumStrings[] =
    {
        "Offer Type: None ",
        "Offer Type: Join Group ",
        "Offer Type: Join Chat Room ",
        "Offer Type: Join Random Connect ",
        "Offer Type: Photo File ",
        "Offer Type: Audio File ",
        "Offer Type: Video File ",
        "Offer Type: Document File ",
        "Offer Type: Archive File ",
        "Offer Type: Executable File ",
        "Offer Type: Unknown Type File ",
        "Offer Type: Folder Of Files ",
        "Offer Type: Friendship ",
        "Offer Type: Instant Message Session ",
        "Offer Type: Truth Or Dare Game ",
        "Offer Type: Video Chat ",
        "Offer Type: Voice Phone Call ",
        "Max Offer Type ",
    };

    const char* PluginAccessStateEnumStrings[] =
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

    const char* PluginTypeEnumStrings[] =
    {
        "Unknown Plugin ",
        "Admin ",
        "About Me Page Server ",
        "Story Board Server",
        "Shared Web Cam ",
        "Shared Files ",
        "Send A Person File ",    
        "Messenger ", 
        "Play Truth Or Dare ",
        "Video Chat ",
        "Voice Call ",
        "Push To Talk ",
        "Client Group ",
        "Client Chat Room ",
        "Client Random Connect ",
        "Client Peer User ",
        "Client No Limit Network ",
        "Client Connect Test ",
        "Host Group ",
        "Host Chat Room",
        "Host Random Connect ",
        "Host Peer User ",
        "Host No Limit Network ",
        "Host Connect Test ",
        "Network Search ",
        "Max Public Plugin ",
        "Plugin Reserved 27 ",
        "Plugin Reserved 28 ",
        "Plugin Reserved 29 ",
        "Plugin Reserved 30 ",
        "Plugin Reserved 31 ",
        "Plugin Reserved 32 ",
        "Plugin Reserved 33 ",
        "Plugin Reserved 34 ",
        "Plugin Reserved 35 ",
        "Plugin Reserved 36 ",
        "Plugin Reserved 37 ",
        "Plugin Reserved 38 ",
        "Plugin Reserved 39 ",
        "Plugin Reserved 40 ",
        "Plugin Reserved 41 ",
        "Plugin Reserved 42 ",
        "Plugin Reserved 43 ",
        "Plugin Reserved 44 ",
        "Plugin Reserved 45 ",
        "Plugin Reserved 46 ",
        "Plugin Reserved 47 ",
        "Max User Plugin Type ",

        "Plugin Camera Service ",
        "Plugin MJPEG Reader ",
        "Plugin MJPEG Writer ",
        "Plugin Personal Recorder ",
        "Plugin Net Services ",
        "Plugin Search ",
        "Plugin Sound Reader ",
        "Plugin Sound Writer ",
        "Plugin Web Server ",        //!< Web server plugin ( for About Me and Story Board web pages )
        "About Me Page Client ",//!< about me web page plugin client
        "Storyboard Client ",	//!< storyboard web page plugin client
        "FileShare Client ",
        "Web Cam Client ",
        "Plugin Library Server ",
    };

    const char* PortOpenStatusEnumStrings[] =
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

    const char* RunTestStatusEnumStrings[] =
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

    const char* RandomConnectStatusEnumStrings[] =
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

    const char* ScanTypeEnumStrings[] =
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

    const char* SearchTypeEnumStrings[] =
    {
        "Search Type None ",
        "Search For Chat Room Host ",	        //!< Search for Chat Room to Join
        "Search For Group Host ",	            //!< Search for Group to Join
        "Search For Random Connect Host ",	//!< Search for Random Connect Server to Join

        "MaxSearchNone"
    };

    const char* Sha1GenResultEnumStrings[] =
    {
        "Sha1 Generate No Error ",
        "Sha1 Generate Duplicate Request ",	    
        "Sha1 Generate Invalid Param ",
        "Sha1 Generate Failed ",

        "MaxSha1GenResult"
    };

    const char* SktCloseReasonEnumStrings[] =
    {
        "Skt Close Reason Unknown ",
        "Skt Close Not Needed ",	   
        "Skt Close Hack Level Unknown ",
        "Skt Close Hack Level Suspicious ",	 
        "Skt Close Hack Level Medium ",	
        "Skt Close Hack Level Severe ",	 	
        "Skt Close Accept Failed ",
        "Skt Close Connection Lost ",
        "Skt Close Connect Timeout ",
        "Skt Close Skt Destroyed ",
        "Skt Close All ",
        "Skt Close Disconnect After Send ",
        "Skt Close Send Complete ",
        "Skt Close With Error ",
        "Skt Close Crypto Null Data ",
        "Skt Close Crypto Invalid Length ",
        "Skt Close Crypto Invalid Key ",
        "Skt Close Pkt Length Invalid ",
        "Skt Close UDP Create ",
        "Skt Close Upnp Start ",
        "Skt Close Upnp Done ",
        "Skt Close Multicast Listen Done ",
        "Skt Close Web Browser ",
        "Skt Close Im Alive Timeout ",
        "Skt Close Ping Timeout ",
        "Skt Close Invalid Handle ",
        "Skt Close Net Cmd Length Invalid ",
        "Skt Close Net Cmd List Invalid ",
        "Skt Close Net Srv Url Invalid ",
        "Skt Close Net Srv Plugin Invalid ",
        "Skt Close Net Srv Query Id Permission ",
        "Skt Close Net Srv Query Id Sent ",
        "Skt Close Http Url Invalid ",
        "Skt Close Http Plugin Invalid ",
        "Skt Close Http Handle Error ",
        "Skt Close Http Not Accept Skt ",
        "Skt Close Http Close No Error ",
        "Skt Close Http Close With Error ",
        "Skt Close Http No Web Srv ",
        "Skt Close Http Skt Destroy ",
        "Skt Close Connect Reasons Empty ",
        "Skt Close Big Info Find Fail ",
        "Skt Close Connected Info Find Fail ",
        "Skt Close PktAnn Send Fail ",
        "Skt Close To Relay PktAnn Send Fail ",
        "Skt Close Through Relay PktAnn Send Fail ",
        "Skt Close Pkt Online Id Me From My Ip ",
        "Skt Close Pkt Online Id Me From Another Ip ",
        "Skt Close PktAnn Not First Packet ",
        "Skt Close PktAnn Is Invalid ",
        "Skt Close PktHdr Invalid ",
        "Skt Close PktPingReq Send Fail ",
        "Skt Close P2P Not Ready For Accept Skt ",
        "Skt Close User Ignored ",
        "Skt Close Relay Session Not Found ",
        "Skt Close Host Ping Reply Sent ",

        "eMaxSktCloseReason "
  
    };

    const char* SktTypeEnumStrings[] =
    {
        "Unknown Socket ",
        "Connect Socket ",
        "Accept Socket ",
        "UDP Socket ",
        "UDP Broadcast Socket ",
        "Loopback Socket ",

        "eMaxSktType"
    };
}

//============================================================================
const char* DescribeAge( enum EAgeType ageType )
{
    if( ageType < 0 || eMaxAgeType <= ageType )
    {
        return ENUM_BAD_PARM;
    }

    switch( ageType )
    {
    case eAgeTypeUnspecified:
        return "Any";
    case eAgeTypeUnder21:
        return "Under 21 ";
    case eAgeType21OrOlder:
        return "21 Or Older ";

    default:
        return "UNKNOWN Age ";
    }
}

//============================================================================
const char* DescribeCommError( enum ECommErr commErr )
{
    if( commErr < 0 || eMaxCommErr <= commErr )
    {
        return ENUM_BAD_PARM;
    }

    return CommErrEnumStrings[ commErr ];
}

//============================================================================
const char* DescribeAppModule( enum EAppModule eAppModule )
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
    case eAppModulePushToTalk:
        return "eAppModulePushToTalk";
    case eAppModuleMediaReader:
        return "eAppModuleMediaReader";
    case eAppModuleMediaWriter:
        return "eAppModuleMediaWriter";
    case eAppModuleCamServer:
        return "eAppModuleCamServer";
    case eAppModuleCamClient:
        return "eAppModuleCamClient";
    case eAppModuleVideoChat:
        return "eAppModuleVideoChat";
    case eAppModulePersonalNotes:
        return "eAppModulePersonalNotes";
    case eAppModuleChatRoomClient:
        return "eAppModuleChatRoomClient";
    case eAppModuleMessenger:
        return "eAppModuleMessenger";
    case eAppModuleVideoPhone:
        return "eAppModuleVideoPhone";
    case eAppModuleVoicePhone:
        return "eAppModuleVoicePhone";
    case eAppModuleTruthOrDare:
        return "eAppModuleTruthOrDare";
    case eAppModuleSnapshot:
        return "eAppModuleSnapshot";
    case eAppModuleMediaPlayer:
        return "eAppModuleMediaPlayer";
    case eAppModuleTest:
        return "eAppModuleTest";
    case eAppModuleMicrophone:
        return "eAppModuleMicrophone";
    default:
        return "UNKNOWN EAppModule";
    }
}

//============================================================================
const char* DescribeConnectReason( enum EConnectReason eConnectReason )
{
    if( eConnectReason < 0 || eMaxConnectReason <= eConnectReason )
    {
        return ENUM_BAD_PARM;
    }

    return ConnectReasonEnumStrings[ eConnectReason ];
}

//============================================================================
bool IsConnectReasonAnnounce( enum EConnectReason connectReason )
{
    return eConnectReasonGroupAnnounce == connectReason || eConnectReasonChatRoomAnnounce == connectReason || eConnectReasonRandomConnectAnnounce == connectReason;
}

//============================================================================
bool IsConnectReasonJoin( enum EConnectReason connectReason )
{
    return eConnectReasonGroupJoin == connectReason || eConnectReasonChatRoomJoin == connectReason || eConnectReasonRandomConnectJoin == connectReason;
}

//============================================================================
bool IsConnectReasonLeave( enum EConnectReason connectReason )
{
    return eConnectReasonGroupLeave == connectReason || eConnectReasonChatRoomLeave == connectReason || eConnectReasonRandomConnectLeave == connectReason;
}

//============================================================================
bool IsConnectReasonUnJoin( enum EConnectReason connectReason )
{
    return eConnectReasonGroupUnJoin == connectReason || eConnectReasonChatRoomUnJoin == connectReason || eConnectReasonRandomConnectUnJoin == connectReason;
}

//============================================================================
bool IsConnectReasonSearch( enum EConnectReason connectReason )
{
    return eConnectReasonGroupSearch == connectReason || eConnectReasonChatRoomSearch == connectReason || eConnectReasonRandomConnectSearch == connectReason;
}

//============================================================================
const char* DescribeConnectStatus( enum  EConnectStatus eConnectStatus )
{
    if( eConnectStatus < 0 || eMaxConnectStatus <= eConnectStatus )
    {
        return ENUM_BAD_PARM;
    }

    return ConnectStatusEnumStrings[ eConnectStatus ];
}


//============================================================================
//! describe friend state
const char* DescribeFriendState( enum EFriendState eFriendState )
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
const char* DescribeGroupieViewType( enum EGroupieViewType groupieViewType )
{
    switch( groupieViewType )
    {
    case eGroupieViewTypeNone:	
        return "No View Type ";
    case eGroupieViewTypeFriendsOnly:	
        return "Friends Only ";
    case eGroupieViewTypeGroupOnly:	
        return "Hosted Users Only ";
    case eGroupieViewTypeGroupAndFriends:		
        return "Hosted Users And Friends ";
    case eGroupieViewTypeEverybody:
        return "Everybody ";
    case eGroupieViewTypeIgnored:
        return "Ignored ";
    case eGroupieViewTypeNearby:
        return "Nearby ";
    case eGroupieViewTypeOnline:
        return "Online ";
    case eGroupieViewTypeDirectConnect:
        return "Direct Connect ";
    default:
        return "Invalid Group View Type ";
    }
}

//============================================================================
const char* DescribeHackerLevel( enum EHackerLevel hackLevel )
{
    if( hackLevel < 0 || eMaxHackerLevel <= hackLevel )
    {
        return ENUM_BAD_PARM;
    }

    return HackerLevelEnumStrings[ hackLevel ];
}

//============================================================================
const char* DescribeHackerReason( enum EHackerReason hackReason )
{
    if( hackReason < 0 || eMaxHackerReason <= hackReason )
    {
        return ENUM_BAD_PARM;
    }

    return HackerReasonEnumStrings[ hackReason ];
}

//============================================================================
const char* DescribeHostAnnounceStatus( enum EHostAnnounceStatus  hostStatus )
{
    if( hostStatus < 0 || eMaxHostAnnounceStatus <= hostStatus )
    {
        return ENUM_BAD_PARM;
    }

    return HostAnnounceStatusEnumStrings[ hostStatus ];
}

//============================================================================
const char* DescribeHostJoinStatus( enum EHostJoinStatus hostStatus )
{
    if( hostStatus < 0 || eMaxHostJoinStatus <= hostStatus )
    {
        return ENUM_BAD_PARM;
    }

    return HostJoinStatusEnumStrings[ hostStatus ];
}

//============================================================================
const char* DescribeHostSearchStatus( enum EHostSearchStatus hostStatus )
{
    if( hostStatus < 0 || eMaxHostSearchStatus <= hostStatus )
    {
        return ENUM_BAD_PARM;
    }

    return HostSearchStatusEnumStrings[ hostStatus ];
}

//============================================================================
const char* DescribeHostType( enum EHostType eHostType )
{
    if( eHostType < 0 || eMaxHostType <= eHostType )
    {
        return ENUM_BAD_PARM;
    }

    return HostTypeEnumStrings[ eHostType ];
}

//! Internet Status as text
const char* DescribeInternetStatus( enum EInternetStatus internetStatus )
{
    if( internetStatus < 0 || eMaxInternetStatus <= internetStatus )
    {
        return ENUM_BAD_PARM;
    }

    return InternetStatusEnumStrings[ internetStatus ];
}

//============================================================================
const char* DescribeJoinState( enum EJoinState joinState )
{
    if( joinState < 0 || eMaxJoinState <= joinState )
    {
        return ENUM_BAD_PARM;
    }

    return JoinStateEnumStrings[ joinState ];
}

//============================================================================
const char* DescribeListAction( enum EListAction listAction )
{
    switch( listAction )
    {
    case eListActionAnnounced:
        return "eListActionAnnounced";
    case eListActionJoinedHost:
        return "eListActionJoinedHost";
    case eListActionJoinedClient:
        return "eListActionJoinedClient";
    default:
        return "UNKNOWN EListAction";
    }
}

//============================================================================
const char* DescribeNetAction( enum ENetActionType netAction )
{
    switch( netAction )
    {
    case eNetActionIdle:
        return "eNetActionIdle";

    case eNetActionIsPortOpen:
        return "eNetActionIsPortOpen";

    default:
        return "UNKNOWN ENetActionType";
    }
}

//============================================================================
const char* DescribeModuleState( enum EModuleState moduleState )
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
const char* DescribeNetAvailStatus( enum ENetAvailStatus netAvailStatus )
{
    if( netAvailStatus < 0 || eMaxNetAvailStatus <= netAvailStatus )
    {
        return ENUM_BAD_PARM;
    }

    return NetAvailStatusEnumStrings[ netAvailStatus ];
}

const char* DescribeNetworkState( enum ENetworkStateType networkStateType )
{
    if( networkStateType < 0 || eMaxNetworkStateType <= networkStateType )
    {
        return ENUM_BAD_PARM;
    }

    return NetworkStateEnumStrings[ networkStateType ];
}

const char* DescribeRelayError( enum ERelayErr relayError )
{
    if( relayError < 0 || eMaxRelayErr <= relayError )
    {
        return ENUM_BAD_PARM;
    }

    return RelayErrEnumStrings[ relayError ];
}

const char* DescribePortOpenStatus( enum EIsPortOpenStatus ePortOpenStatus )
{
    if( ePortOpenStatus < 0 || eMaxIsPortOpenStatusType <= ePortOpenStatus )
    {
        return ENUM_BAD_PARM;
    }

    return PortOpenStatusEnumStrings[ ePortOpenStatus ];
}

const char* DescribeRunTestStatus( enum ERunTestStatus eTestStatus )
{
    if( eTestStatus < 0 || eMaxRunTestStatusType <= eTestStatus )
    {
        return ENUM_BAD_PARM;
    }

    return RunTestStatusEnumStrings[ eTestStatus ];
}

const char* DescribeRandomConnectStatus( enum ERandomConnectStatus eRandomConnectStatus )
{
    if( eRandomConnectStatus < 0 || eMaxRandomConnectStatusType <= eRandomConnectStatus )
    {
        return ENUM_BAD_PARM;
    }

    return RandomConnectStatusEnumStrings[ eRandomConnectStatus ];
}

//! Net Command type as text
const char* DescribeNetCmdType( enum ENetCmdType netCmdType )
{
    if( netCmdType < 0 || eMaxNetCmdType <= netCmdType )
    {
        return ENUM_BAD_PARM;
    }

    return NetCmdTypeEnumStrings[ netCmdType ]; 
}

//! Net Command Error as text
const char* DescribeNetCmdError( enum ENetCmdError netCmdError )
{
    if(  netCmdError < 0 || eMaxNetCmdError <= netCmdError )
    {
        return ENUM_BAD_PARM;
    }

    return NetCmdErrorEnumStrings[ netCmdError ]; 
}

//============================================================================
const char* DescribeOfferState( enum EOfferState offerState )
{
    if(  offerState < 0 || eMaxOfferState <= offerState )
    {
        return ENUM_BAD_PARM;
    }

    return OfferStateEnumStrings[ offerState ]; 
}

//============================================================================
const char* DescribeOfferType( enum EOfferType offerType )
{
    if(  offerType < 0 || eMaxOfferType <= offerType )
    {
        return ENUM_BAD_PARM;
    }

    return OfferTypeEnumStrings[ offerType ]; 
}

//============================================================================
const char* DescribePluginAccess( enum EPluginAccess pluginAccess )
{
    if(  pluginAccess < 0 || eMaxPluginAccessState <= pluginAccess )
    {
        return ENUM_BAD_PARM;
    }

    return PluginAccessStateEnumStrings[ pluginAccess ]; 
}

//============================================================================
const char* DescribePluginType( enum EPluginType pluginType )
{
    if( pluginType < 0 || eMaxPluginType <= pluginType )
    {
        return ENUM_BAD_PARM;
    }

    return PluginTypeEnumStrings[pluginType];
}

//============================================================================
const char* DescribeScanType( enum EScanType scanType )
{
    if(  scanType < 0 || eMaxScanType <= scanType )
    {
        return ENUM_BAD_PARM;
    }

    return ScanTypeEnumStrings[ scanType ]; 
}

//============================================================================
const char* DescribeSearchType( enum ESearchType searchType )
{
    if(  searchType < 0 || eMaxSearchType <= searchType )
    {
        return ENUM_BAD_PARM;
    }

    return SearchTypeEnumStrings[ searchType ]; 
}

//============================================================================
const char* DescribeSha1GenResult( enum ESha1GenResult sha1GenerateResult )
{
    if( sha1GenerateResult < 0 || eMaxSha1GenResult <= sha1GenerateResult )
    {
        return ENUM_BAD_PARM;
    }

    return Sha1GenResultEnumStrings[sha1GenerateResult];
}

//============================================================================
//! Describe socket close reason
const char* DescribeSktCloseReason( enum ESktCloseReason closeReason )
{
    if(  closeReason < 0 || eMaxSktCloseReason <= closeReason )
    {
        return ENUM_BAD_PARM;
    }

    return SktCloseReasonEnumStrings[ closeReason ]; 
}

//============================================================================
const char* DescribeSktType( enum ESktType sktType )
{
    if(  sktType < 0 || eMaxSktType <= sktType )
    {
        return ENUM_BAD_PARM;
    }

    return SktTypeEnumStrings[ sktType ]; 
}

//============================================================================
const char* DescribeWebPageType( enum EWebPageType webPageType )
{
    switch( webPageType )
    {
    case eWebPageTypeNone:
        return "No Web Page Type ";
    case eWebPageTypeAboutMe:
        return "About Me Web Page ";
    case eWebPageTypeStoryboard:
        return "Storyboard Web Page ";
    default:
        return ENUM_BAD_PARM;
    }
}

//============================================================================
const char* DescribeXferAction( enum EXferAction xferAction )
{
    switch( xferAction )
    {
    case eXferActionNone:
        return "No Xfer Action ";
    case eXferActionDownload:
        return "Xfer Action Start Download ";
    case eXferActionUpload:
        return "Xfer Action Start Upload ";
    case eXferActionCancelXfer:
        return "Xfer Action Cancel Transfer ";
    default:
        return ENUM_BAD_PARM;
    }
}

//============================================================================
// for use in database mainly
const char* getPluginName( enum EPluginType pluginType )
{
    switch( pluginType )
    {
    case ePluginTypeInvalid: return "ePluginTypeInvalid";
    case ePluginTypeAdmin: return "ePluginTypeAdmin";
    case ePluginTypeAboutMePageClient: return "ePluginTypeAboutMePageClient";
    case ePluginTypeAboutMePageServer: return "ePluginTypeAboutMePageServer";
    case ePluginTypeCamServer: return "ePluginTypeCamServer";
    case ePluginTypeFileShareServer: return "ePluginTypeFileShareServer";
    case ePluginTypePersonFileXfer: return "ePluginTypePersonFileXfer";
    case ePluginTypeMessenger: return "ePluginTypeMessenger";

    case ePluginTypeClientChatRoom: return "ePluginTypeClientChatRoom";
    case ePluginTypeClientConnectTest: return "ePluginTypeClientConnectTest";
    case ePluginTypeClientGroup: return "ePluginTypeClientGroup";
    case ePluginTypeClientNetwork: return "ePluginTypeClientNetwork";
    case ePluginTypeClientPeerUser: return "ePluginTypeClientPeerUser";
    case ePluginTypeClientRandomConnect: return "ePluginTypeClientRandomConnect";

    case ePluginTypeHostChatRoom: return "ePluginTypeHostChatRoom";
    case ePluginTypeHostConnectTest: return "ePluginTypeHostConnectTest";
    case ePluginTypeHostGroup: return "ePluginTypeHostGroup";
    case ePluginTypeHostNetwork: return "ePluginTypeHostNetwork";
    case ePluginTypeHostPeerUser: return "ePluginTypeHostPeerUser";
    case ePluginTypeHostRandomConnect: return "ePluginTypeHostRandomConnect";

    case ePluginTypeNetworkSearchList: return "ePluginTypeNetworkSearchList";

    case ePluginTypeStoryboardClient: return "ePluginTypeStoryboardClient";
    case ePluginTypeStoryboardServer: return "ePluginTypeStoryboardServer";
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

    case ePluginTypeFileShareClient: return "ePluginTypeFileShareClient";

    default:
        return "UnknownPlugin";
    }
}

//============================================================================
//! convert Host Type to connect reason
EConnectReason HostTypeToConnectAnnounceReason( enum EHostType hostType )
{
    switch( hostType )
    {
    case eHostTypeGroup:
        return eConnectReasonGroupAnnounce;
    case eHostTypeChatRoom:
        return eConnectReasonChatRoomAnnounce;
    case eHostTypeRandomConnect:
        return eConnectReasonRandomConnectAnnounce;
    default:
        LogMsg( LOG_ERROR, "HostTypeToConnectAnnounceReason unknown host type %d", hostType );
        vx_assert( false );
        return eConnectReasonUnknown;
    }
}

//============================================================================
//! convert Host Type to connect reason
EConnectReason HostTypeToConnectJoinReason( enum EHostType hostType )
{
    switch( hostType )
    {
    case eHostTypeGroup:
        return eConnectReasonGroupJoin;
    case eHostTypeChatRoom:
        return eConnectReasonChatRoomJoin;
    case eHostTypeRandomConnect:
        return eConnectReasonRandomConnectJoin;
    default:
        LogMsg( LOG_ERROR, "HostTypeToConnectJoinReason unknown host type %d", hostType );
        vx_assert( false );
        return eConnectReasonUnknown;
    }
}

//============================================================================
//! convert Host Type to connect reason
EConnectReason HostTypeToConnectLeaveReason( enum EHostType hostType )
{
    switch( hostType )
    {
    case eHostTypeGroup:
        return eConnectReasonGroupLeave;
    case eHostTypeChatRoom:
        return eConnectReasonChatRoomLeave;
    case eHostTypeRandomConnect:
        return eConnectReasonRandomConnectLeave;
    default:
        LogMsg( LOG_ERROR, "HostTypeToConnectJoinReason unknown host type %d", hostType );
        vx_assert( false );
        return eConnectReasonUnknown;
    }
}

//============================================================================
//! convert Host Type to connect reason
EConnectReason HostTypeToConnectUnJoinReason( enum EHostType hostType )
{
    switch( hostType )
    {
    case eHostTypeGroup:
        return eConnectReasonGroupUnJoin;
    case eHostTypeChatRoom:
        return eConnectReasonChatRoomUnJoin;
    case eHostTypeRandomConnect:
        return eConnectReasonRandomConnectUnJoin;
    default:
        LogMsg( LOG_ERROR, "HostTypeToConnectUnJoinReason unknown host type %d", hostType );
        vx_assert( false );
        return eConnectReasonUnknown;
    }
}

//============================================================================
//! convert Host Type to connect reason
EConnectReason HostTypeToConnectSearchReason( enum EHostType hostType )
{
    switch( hostType )
    {
    case eHostTypeGroup:
        return eConnectReasonGroupSearch;
    case eHostTypeChatRoom:
        return eConnectReasonChatRoomSearch;
    case eHostTypeRandomConnect:
        return eConnectReasonRandomConnectSearch;
    default:
        LogMsg( LOG_ERROR, "HostTypeToConnectSearchReason unknown host type %d", hostType );
        vx_assert( false );
        return eConnectReasonUnknown;
    }
}

//============================================================================
//! convert Host Type to service plugin  type
EPluginType HostTypeToHostPlugin( enum EHostType hostType )
{
    switch( hostType )
    {
    case eHostTypeGroup:
        return ePluginTypeHostGroup;

    case eHostTypeChatRoom:
        return ePluginTypeHostChatRoom;

    case eHostTypeRandomConnect:
        return ePluginTypeHostRandomConnect;

    case eHostTypePeerUserDirect:
        return ePluginTypeHostPeerUser;

    case eHostTypeNetwork:
        return ePluginTypeHostNetwork;

    case eHostTypeConnectTest:
        return ePluginTypeHostConnectTest;

    case eHostTypeUnknown:
    default:
        LogMsg( LOG_ERROR, "HostTypeToHostPlugin unknown host type %d", hostType );
        // vx_assert( false );
        return ePluginTypeInvalid;
    }
}

//============================================================================
//! convert Host Type to client plugin type
EPluginType HostTypeToClientPlugin( enum EHostType hostType )
{
    switch( hostType )
    {
    case eHostTypeGroup:
        return ePluginTypeClientGroup;

    case eHostTypeChatRoom:
        return ePluginTypeClientChatRoom;

    case eHostTypeRandomConnect:
        return ePluginTypeClientRandomConnect;

    case eHostTypePeerUserDirect:
        return ePluginTypeClientPeerUser;

    case eHostTypeNetwork:
        return ePluginTypeClientNetwork;

    case eHostTypeConnectTest:
        return ePluginTypeClientConnectTest;

    case eHostTypeUnknown:
    default:
        LogMsg( LOG_ERROR, "HostTypeToClientPlugin unknown host type %d", hostType );
        vx_assert( false );
        return ePluginTypeInvalid;
    }
}

//============================================================================
//! convert Host Type to user connect reason
EConnectReason HostTypeToGroupieConnectReason( enum EHostType hostType )
{
    switch( hostType )
    {
    case eHostTypeGroup:
        return eConnectReasonGroupUserConnect;
    case eHostTypeChatRoom:
        return eConnectReasonChatRoomUserConnect;
    case eHostTypeRandomConnect:
        return eConnectReasonRandomConnectUserConnect;

    case eHostTypePeerUserDirect:
        return eConnectReasonUserDirectConnect;
    case eHostTypeNetwork:
        return eConnectReasonNetworkHost;
    case eHostTypeConnectTest:
        return eConnectReasonConnectTest;
    case eHostTypeUnknown:
    default:
        LogMsg( LOG_ERROR, "HostTypeToHostPlugin unknown host type %d", hostType );
        // vx_assert( false );
        return eConnectReasonUnknown;
    }
}

//============================================================================
//! convert Plugin Type to host type
EHostType PluginTypeToHostType( enum EPluginType pluginType )
{
    switch( pluginType )
    {
    case ePluginTypeHostGroup:
    case ePluginTypeClientGroup:
        return eHostTypeGroup;

    case ePluginTypeHostChatRoom:
    case ePluginTypeClientChatRoom:
        return eHostTypeChatRoom;

    case ePluginTypeHostRandomConnect:
    case ePluginTypeClientRandomConnect:
        return eHostTypeRandomConnect;

    //case ePluginTypeHostPeerUser:
    //case ePluginTypeClientPeerUser:
    //    return eHostTypePeerUserRelayed;

    case ePluginTypeHostNetwork:
    case ePluginTypeClientNetwork:
        return eHostTypeNetwork;

    case ePluginTypeHostConnectTest:
    case ePluginTypeClientConnectTest:
        return eHostTypeConnectTest;

    case ePluginTypeNetServices:
    case ePluginTypeFileShareServer:
    case ePluginTypeNetworkSearchList:
    case ePluginTypePersonFileXfer:
        return eHostTypeUnknown;

    case ePluginTypeInvalid:
    default:
        LogMsg( LOG_ERROR, "PluginTypeToHostType unknown plugin type %d", pluginType );
        // vx_assert( false );
        return eHostTypeUnknown;
    }
}

//============================================================================
// if host plugin return its client plugin else return pluginType param
EPluginType HostPluginToClientPluginType( enum EPluginType pluginType )
{
    switch( pluginType )
    {
    case ePluginTypeHostGroup:
        return ePluginTypeClientGroup;

    case ePluginTypeHostChatRoom:
        return ePluginTypeClientChatRoom;

    case ePluginTypeClientRandomConnect:
        return ePluginTypeClientRandomConnect;

    case ePluginTypeHostPeerUser:
        return ePluginTypeClientPeerUser;

    case ePluginTypeHostNetwork:
        return ePluginTypeClientNetwork;

    case ePluginTypeHostConnectTest:
        return ePluginTypeClientConnectTest;

    case ePluginTypeInvalid:
        LogMsg( LOG_ERROR, "HostPluginToClientPluginType invalid plugin type %d", pluginType );
        return pluginType;
    default:
        LogMsg( LOG_ERROR, "HostPluginToClientPluginType unknown plugin type %d", pluginType );
        return pluginType;
    }
}

//============================================================================
// if client plugin return its host plugin else return pluginType param
EPluginType ClientPluginToHostPluginType( enum EPluginType pluginType )
{
    switch( pluginType )
    {
    case ePluginTypeClientGroup:
        return ePluginTypeHostGroup;

    case ePluginTypeClientChatRoom:
        return ePluginTypeHostChatRoom;

    case ePluginTypeClientRandomConnect:
        return ePluginTypeHostRandomConnect;

    case ePluginTypeClientPeerUser:
        return ePluginTypeHostPeerUser;

    case ePluginTypeClientNetwork:
        return ePluginTypeHostNetwork;

    case ePluginTypeClientConnectTest:
        return ePluginTypeHostConnectTest;

    case ePluginTypeInvalid:
        LogMsg( LOG_ERROR, "ClientPluginToHostPluginType invalid plugin type %d", pluginType );
        return pluginType;
    default:
        LogMsg( LOG_ERROR, "ClientPluginToHostPluginType unknown plugin type %d", pluginType );
        return pluginType;
    }
}

//============================================================================
//! return true if is a client plugin
bool IsClientPluginType( enum EPluginType pluginType )
{
    switch( pluginType )
    {
    case ePluginTypeCamClient:
    case ePluginTypePersonFileXfer:
    case ePluginTypeFileShareClient:
    case ePluginTypeAboutMePageClient:
    case ePluginTypeStoryboardClient:
    case ePluginTypeClientGroup:
    case ePluginTypeClientChatRoom:
    case ePluginTypeClientRandomConnect:
    case ePluginTypeClientPeerUser:
    case ePluginTypeClientNetwork:
    case ePluginTypeClientConnectTest:
        return true;

    case ePluginTypeInvalid:
        LogMsg( LOG_ERROR, "IsClientPluginType unknown plugin type %d", pluginType );
        vx_assert( false );
        return false;
    default:
        return false;
    }
}

//============================================================================
//! return true if is a host plugin
bool IsHostPluginType( enum EPluginType pluginType )
{
    switch( pluginType )
    {
    case ePluginTypeHostGroup:
    case ePluginTypeHostChatRoom:
    case ePluginTypeHostRandomConnect:
    case ePluginTypeHostPeerUser:
    case ePluginTypeHostNetwork:
    case ePluginTypeHostConnectTest:
        return true;

    case ePluginTypeInvalid:
        LogMsg( LOG_ERROR, "IsHostPluginType unknown plugin type %d", pluginType );
        vx_assert( false );
        return false;
    default:
        return false;
    }
}

//============================================================================
//! return true if is a host or client relationship plugin
bool IsHostOrClientPluginType( enum EPluginType pluginType )
{
    return IsClientPluginType( pluginType ) || IsHostPluginType( pluginType );
}

//============================================================================
//! return true if host can be announced to network or is a client of such a host
bool IsAnnounceHostOrClientPluginType( enum EPluginType pluginType )
{
    return  ePluginTypeHostGroup == pluginType ||
        ePluginTypeHostChatRoom == pluginType ||
        ePluginTypeHostRandomConnect == pluginType ||
        ePluginTypeClientGroup == pluginType ||
        ePluginTypeClientChatRoom == pluginType ||
        ePluginTypeClientRandomConnect == pluginType;
}

//============================================================================
//! return true if host can be announced to network or is a client of such a host
bool IsAnnounceHostOrClientHostType( enum EHostType hostType )
{
    return HostShouldAnnounceToNetwork( hostType );
}

//============================================================================
//! return true if plugin should announce to network host
bool PluginShouldAnnounceToNetwork( enum EPluginType pluginType )
{
    return ePluginTypeHostGroup == pluginType ||
        ePluginTypeHostChatRoom == pluginType ||
        ePluginTypeHostRandomConnect == pluginType;
}

//============================================================================
//! return true if host should announce to network host
bool HostShouldAnnounceToNetwork( enum EHostType hostType )
{
    return eHostTypeGroup == hostType ||
        eHostTypeChatRoom == hostType ||
        eHostTypeRandomConnect == hostType;
}

//============================================================================
//! return true if plugin can act as relay for user
bool IsPluginARelayForUser( enum EPluginType pluginType )
{
    return ePluginTypeHostGroup == pluginType ||
        ePluginTypeHostChatRoom == pluginType ||
        ePluginTypeHostRandomConnect == pluginType;
}

//============================================================================
//! return true if host can act as relay for user
bool IsHostARelayForUser( enum EHostType hostType )
{
    return eHostTypeGroup == hostType ||
        eHostTypeChatRoom == hostType ||
        eHostTypeRandomConnect == hostType;
}
