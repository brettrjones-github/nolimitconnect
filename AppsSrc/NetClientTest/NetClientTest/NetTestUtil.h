#pragma once

#define NET_SERVICE_HDR_LEN 96

#define NET_CMD_UNKNOWN				"CMD_UNKNOWN        "
#define NET_CMD_PING				"CMD_PING           "
#define NET_CMD_PONG				"CMD_PONG           "
#define NET_CMD_PORT_TEST_REQ		"CMD_PORT_TEST_REQ  "
#define NET_CMD_PORT_TEST_REPLY		"CMD_PORT_TEST_REPLY"
#define NET_CMD_ANCHOR_REQ			"CMD_ANCHOR_REQ     "
#define NET_CMD_ANCHOR_REPLY		"CMD_ANCHOR_REPLY   "
#define NET_CMD_PROFILE				"CMD_PROFILE        "
#define NET_CMD_STORYBOARD			"CMD_STORYBOARD     "
#define NET_CMD_HOST_ID_REQ			"CMD_HOST_ID_REQ    "
#define NET_CMD_HOST_ID_REPLY		"CMD_HOST_ID_REPLY  "

#define NET_TEST_RX_URL_HDR_TIMEOUT			    30000
#define NET_TEST_RX_DATA_TIMEOUT				20000

enum ENetCmdType
{
    eNetCmdUnknown = 0,
    eNetCmdPing = 1,
    eNetCmdPong = 2,
    eNetCmdIsMyPortOpenReq = 3,
    eNetCmdIsMyPortOpenReply = 4,
    eNetCmdHostReq = 5,
    eNetCmdHostReply = 6,
    eNetCmdAboutMePage = 7,
    eNetCmdStoryboardPage = 8,
    eNetCmdQueryHostOnlineIdReq = 9,
    eNetCmdQueryHostOnlineIdReply = 10,

    eMaxNetCmdType
};

//! \public Enumerated plugins
enum EPluginType
{
    //! NOTE: don't handle packets for ePluginTypeInvalid or ePluginTypeWebServer
    ePluginTypeInvalid = 0,	//!< unknown or disabled

    ePluginTypeAdmin = 1,	//!< Administration ( intended for updates but not currently used )
    ePluginTypeAboutMePage = 2,	//!< about me page plugin ( using web page server )
    ePluginTypeAvatarImage = 3,	//!< handle users avatar image
    ePluginTypeServiceConnectTest = 4,	//!< Connection Test Service
    ePluginTypeClientConnectTest = 5,	//!< Connection Test Client
    ePluginTypeCamServer = 6,	//!< Web cam broadcast plugin
    ePluginTypeChatRoom = 7,	//!< chat room host plugin
    ePluginTypeFileServer = 8,	//!< Shared files server
    ePluginTypeFileXfer = 9,	//!< Offer/accept send a file person to person
    ePluginTypeHostGroup = 10,	//!< group host
    ePluginTypeHostGroupListing = 11,	//!< group list host
    ePluginTypeHostNetwork = 12,	//!< master network host
    ePluginTypeMessenger = 13,	//!< Text, voice and video message texting with voice phone, video chat and truth or dare game available in session
    ePluginTypeRandomConnect = 14,	//!< Random connect to another person ( Using Phone Shake )
    ePluginTypeRandomConnectRelay = 15,	//!< Random connect to another person relay service
    ePluginTypeRelay = 16,	//!< Relay services plugin
    ePluginTypeStoryboard = 17,	//!< User editable story board web page server
    ePluginTypeTruthOrDare = 18,	//!< Video Chat Truth Or Dare game  
    ePluginTypeVideoPhone = 19,	//!< Video Chat with motion detect and stream recording
    ePluginTypeVoicePhone = 20,	//!< VOIP audio only phone call
    // NOTE: plugin types 20 - 47 not implemented .. reserved for future use
    eMaxImplementedPluginType = 21,
    // plugins 0-47 are part of PktAnnounce
    // plugins after 47 are system plugins and do not go out in announcement pkt
    eMaxUserPluginType = 48, // this marks end of announced permission
    ePluginTypeCameraService,
    ePluginTypeMJPEGReader,
    ePluginTypeMJPEGWriter,
    ePluginTypePersonalRecorder,
    ePluginTypeNetServices,
    ePluginTypeSearch,
    ePluginTypeSndReader,
    ePluginTypeSndWriter,
    ePluginTypeWebServer,	//!< Web server plugin ( for About Me and Story Board web pages )

    eMaxPluginType
};

#include <string>
#include <CoreLib/VxGUID.h>
class VxNetIdent;
class VxSktConnectSimple;
class VxTimer;
class VxSktBase;

class NetServiceHdr
{
public:
    NetServiceHdr()
        : m_CmdVersion( 0 )
        , m_CmdError( 0 )
        , m_TotalDataLen( 0 )
        , m_ContentDataLen( 0 )
        , m_Ident( 0 )
        , m_SktDataUsed( 0 )
    {
    }

    std::string					m_ChallengeHash;
    ENetCmdType					m_NetCmdType;
    int							m_CmdVersion;
    int							m_CmdError;
    int							m_TotalDataLen;
    int							m_ContentDataLen;
    VxGUID					    m_OnlineId;
    VxNetIdent *				m_Ident;
    int							m_SktDataUsed;
};

namespace NetTestUtil
{
    static ENetCmdType			netCmdStringToEnum( const char * netCmd );
    static const char *			netCmdEnumToString( ENetCmdType	eNetCmdType );
    const char *				getNetworkKey( void );

    void                        buildNetCmd( std::string& retResult, ENetCmdType netCmd, std::string& netServChallengeHash, std::string& strContent, int errCode /*=0*/, int version /*=1*/ );
    RCODE                       buildAndSendCmd( VxSktBase * sktBase, ENetCmdType netCmd, std::string& cmdContent, int errCode /*=0*/, int version /*=1*/ );
    bool					    rxNetServiceCmd( VxSktConnectSimple *   netServConn,
                                                 char *				    rxBuf,
                                                 int					rxBufLen,
                                                 NetServiceHdr&		    netServiceHdr,
                                                 int					rxHdrTimeout = NET_TEST_RX_URL_HDR_TIMEOUT,
                                                 int					rxDataTimeout = NET_TEST_RX_DATA_TIMEOUT );
    bool			            isValidHexString( const char * hexString, int dataLen );

    int                         getNextPartOfUrl( char * buf, std::string& strValue );
    bool                        getNetServiceUrlContent( std::string& netServiceUrl, std::string& retFromClientContent );
    bool                        sendAndRecievePing( VxTimer& pingTimer, VxSktConnectSimple& toClientConn, std::string& retPong, int receiveTimeout );
    bool                        sendAndRecieveQueryHostOnlineId( VxTimer& pingTimer, VxSktConnectSimple& toClientConn, std::string& retPong, int receiveTimeout );
    void                        generateNetServiceChallengeHash( std::string& strKeyHash, uint16_t clientPort );

    void                        generateNetServiceChallengeHash( std::string& strKey, VxSktBase * skt );
    bool                        verifyAllDataArrivedOfNetServiceUrl( VxSktBase * sktBase );
    int                         getTotalLengthFromNetServiceUrl( char * dataBuf, int dataLen );
    EPluginType                 parseHttpNetServiceUrl( VxSktBase * sktBase, NetServiceHdr& netServiceHdr );
    EPluginType                 parseHttpNetServiceHdr( char * dataBuf, int dataLen, NetServiceHdr& netServiceHdr );




};
