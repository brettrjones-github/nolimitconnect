#include "NetTestUtil.h"

#include <CoreLib/VxParse.h>
#include <CoreLib/VxTimer.h>
#include <CoreLib/VxDebug.h>
#include <CoreLib/VxThread.h>

#include <NetLib/VxSktConnectSimple.h>
#include <NetLib/VxSktBase.h>

namespace NetTestUtil
{
    //const int		HTTP_GET_WITH_VER_LEN				= 16;
    const int		MAX_URL_VERSION_LEN_DIGITS = 3;
    const int		MAX_CONTENT_LEN_DIGITS = 13;
    const int		MAX_CMD_VERSION_LEN_DIGITS = 3;
    const int		MAX_ERROR_LEN_DIGITS = 8;
    const int		MAX_NET_CMD_LEN_CHARS = 19;
    const int		MAX_NET_SERVICE_URL_LEN = 16384;
}

//============================================================================
const char * NetTestUtil::getNetworkKey( void )
{ 
    return "TestNetworkKey"; 
}

//============================================================================
void NetTestUtil::buildNetCmd( std::string& retResult, ENetCmdType netCmd, std::string& netServChallengeHash, std::string& strContent, int errCode, int version )
{
    std::string strNetCmd = netCmdEnumToString( ( ENetCmdType )netCmd );
    //http://GET /  1/ = len 16 + 6 /'s
    //http://GET /url version/Crypto Key/total length of data/Net service command/cmd version/error code/content/"
    // total header = 16 + 6 + 32 + 13 + 19 + 3 + 8   = 96

    int totalLen = 16 + 6
        + ( int )netServChallengeHash.length()
        + MAX_CONTENT_LEN_DIGITS
        + MAX_NET_CMD_LEN_CHARS
        + MAX_CMD_VERSION_LEN_DIGITS
        + MAX_ERROR_LEN_DIGITS
        + ( int )strContent.length()
        + 5; // \n\n and terminating 0

    if( strContent.length() )
    {
        StdStringFormat( retResult, "http://GET /  1/%s/%13d/%s/%3d/%8d/%s/\r\n\r\n",
                            netServChallengeHash.c_str(),
                            totalLen,
                            strNetCmd.c_str(),
                            version,
                            errCode,
                            strContent.c_str() );
    }
    else
    {
        StdStringFormat( retResult, "http://GET /  1/%s/%13d/%s/%3d/%8d//\r\n\r\n",
                            netServChallengeHash.c_str(),
                            totalLen,
                            strNetCmd.c_str(),
                            version,
                            errCode );
    }
}

//============================================================================
ENetCmdType NetTestUtil::netCmdStringToEnum( const char * netCmd )
{
    if( 0 == strcmp( NET_CMD_PROFILE, netCmd ) )
    {
        return eNetCmdAboutMePage;
    }
    else if( 0 == strcmp( NET_CMD_STORYBOARD, netCmd ) )
    {
        return eNetCmdStoryboardPage;
    }
    else if( 0 == strcmp( NET_CMD_PING, netCmd ) )
    {
        return eNetCmdPing;
    }
    else if( 0 == strcmp( NET_CMD_PONG, netCmd ) )
    {
        return eNetCmdPong;
    }
    else if( 0 == strcmp( NET_CMD_ANCHOR_REQ, netCmd ) )
    {
        return eNetCmdHostReq;
    }
    else if( 0 == strcmp( NET_CMD_ANCHOR_REPLY, netCmd ) )
    {
        return eNetCmdHostReply;
    }
    else if( 0 == strcmp( NET_CMD_PORT_TEST_REQ, netCmd ) )
    {
        return eNetCmdIsMyPortOpenReq;
    }
    else if( 0 == strcmp( NET_CMD_PORT_TEST_REPLY, netCmd ) )
    {
        return eNetCmdIsMyPortOpenReply;
    }
    else if( 0 == strcmp( NET_CMD_HOST_ID_REQ, netCmd ) )
    {
        return eNetCmdQueryHostOnlineIdReq;
    }
    else if( 0 == strcmp( NET_CMD_HOST_ID_REPLY, netCmd ) )
    {
        return eNetCmdQueryHostOnlineIdReply;
    }
    else
    {
        return eNetCmdUnknown;
    }
}

//============================================================================
const char * NetTestUtil::netCmdEnumToString( ENetCmdType	eNetCmdType )
{
    switch( eNetCmdType )
    {
    case eNetCmdPing:
        return NET_CMD_PING;
    case eNetCmdPong:
        return NET_CMD_PONG;
    case eNetCmdIsMyPortOpenReq:
        return NET_CMD_PORT_TEST_REQ;
    case eNetCmdIsMyPortOpenReply:
        return NET_CMD_PORT_TEST_REPLY;
    case eNetCmdHostReq:
        return NET_CMD_ANCHOR_REQ;
    case eNetCmdHostReply:
        return NET_CMD_ANCHOR_REPLY;
    case eNetCmdAboutMePage:
        return NET_CMD_PROFILE;
    case eNetCmdStoryboardPage:
        return NET_CMD_STORYBOARD;
    case eNetCmdQueryHostOnlineIdReq:
        return NET_CMD_HOST_ID_REQ;
    case eNetCmdQueryHostOnlineIdReply:
        return NET_CMD_HOST_ID_REPLY;
    case eNetCmdUnknown:
    default:
        return NET_CMD_UNKNOWN;
    }
}
//============================================================================
bool NetTestUtil::rxNetServiceCmd( VxSktConnectSimple * netServConn, char * rxBuf, int rxBufLen, NetServiceHdr& netServiceHdr, int rxHdrTimeout, int rxDataTimeout )
{
    vx_assert( rxBufLen > NET_SERVICE_HDR_LEN );

    int iRxed = 0;
    bool bGotCrLfCrLf = false;
    VxTimer rxCmdTimer;
    netServConn->recieveData( rxBuf,					// data buffer to read into
                                NET_SERVICE_HDR_LEN,	// length of data	
                                &iRxed,					// number of bytes actually received
                                rxHdrTimeout,			// timeout attempt to receive
                                false, 					// if true then abort receive when \r\n\r\n is received
                                &bGotCrLfCrLf );

    if( iRxed != NET_SERVICE_HDR_LEN )
    {
        LogMsg( LOG_ERROR, "### ERROR NetTestUtil::rxNetServiceCmd: hdr timeout %3.3f sec rxed data len %d", rxCmdTimer.elapsedSec(), iRxed );
        return false;
    }

    rxBuf[ NET_SERVICE_HDR_LEN ] = 0;
    LogMsg( LOG_DEBUG, "### OK NetTestUtil::rxNetServiceCmd: elapsed %3.3f sec rxed data len %d (%s)", rxCmdTimer.elapsedSec(), iRxed, rxBuf );

    if( ePluginTypeNetServices != parseHttpNetServiceHdr( rxBuf, NET_SERVICE_HDR_LEN, netServiceHdr ) )
    {
        LogMsg( LOG_ERROR, "### ERROR NetTestUtil::rxNetServiceCmd: hdr parse error" );
        return false;
    }

    if( netServiceHdr.m_TotalDataLen <= NET_SERVICE_HDR_LEN )
    {
        LogMsg( LOG_ERROR, "### ERROR NetTestUtil::rxNetServiceCmd: too smal netServiceHdr.m_TotalDataLen %d\n", netServiceHdr.m_TotalDataLen );
        return false;
    }

    if( netServiceHdr.m_TotalDataLen > rxBufLen )
    {
    	LogMsg( LOG_ERROR, "### ERROR NetTestUtil::rxNetServiceCmd: too large netServiceHdr.m_TotalDataLen %d\n", netServiceHdr.m_TotalDataLen );
    	return false;
    }

    int contentLen = netServiceHdr.m_TotalDataLen - NET_SERVICE_HDR_LEN;
    VxTimer rxTimer;
    netServConn->recieveData(   rxBuf,					// data buffer to read into
                                contentLen,				// length of data	
                                &iRxed,					// number of bytes actually received
                                rxDataTimeout,			// timeout attempt to receive
                                false, 					// if true then abort receive when \r\n\r\n is received
                                &bGotCrLfCrLf );
    if( contentLen != iRxed )
    {
        LogMsg( LOG_ERROR, "### ERROR NetActionAnnounce::rxNetServiceCmd: timeout %3.3f sec recieving content\n", rxTimer.elapsedSec() );
        return false;
    }

    netServiceHdr.m_ContentDataLen = contentLen;
    rxBuf[ contentLen ] = 0;
    return true;
}

//============================================================================
bool NetTestUtil::isValidHexString( const char * hexString, int dataLen )
{
    for( int i = 0; i < dataLen; i++ )
    {
        char ch = hexString[ i ];
        if( false == ( ( ( '0' <= ch ) && ( '9' >= ch ) )
                        || ( ( 'A' <= ch ) && ( 'F' >= ch ) )
                        || ( ( 'a' <= ch ) && ( 'f' >= ch ) ) ) )
        {
            return false;
        }
    }

    return true;
}

//============================================================================
int NetTestUtil::getNextPartOfUrl( char * buf, std::string& strValue )
{
    strValue = "";
    int len = 0;
    char * pTemp = strchr( buf, '/' );
    if( pTemp )
    {
        char cSave = *pTemp;
        *pTemp = 0;
        strValue = buf;
        *pTemp = cSave;
        len = strValue.length();
    }
    return len;
}

//============================================================================
bool NetTestUtil::getNetServiceUrlContent( std::string& netServiceUrl, std::string& retFromClientContent )
{
    retFromClientContent = "";
    if( NET_SERVICE_HDR_LEN >= netServiceUrl.length() )
    {
        LogMsg( LOG_ERROR, "NetTestUtil::getNetServiceUrlContent: invalid netService Length\n" );
        return false;
    }

    const char * buf1 = netServiceUrl.c_str();
    retFromClientContent = &buf1[ NET_SERVICE_HDR_LEN ];
    if( 0 == retFromClientContent.length() )
    {
        LogMsg( LOG_ERROR, "NetTestUtil::getNetServiceUrlContent: invalid content Length\n" );
        return false;
    }

    const char * content = retFromClientContent.c_str();
    if( '/' != content[ retFromClientContent.length() - 1 ] )
    {
        LogMsg( LOG_ERROR, "NetTestUtil::getNetServiceUrlContent: no trailing /\n" );
        retFromClientContent = "";
        return false;
    }

    ( ( char * )content )[ retFromClientContent.length() - 1 ] = 0;
    return true;
}

//============================================================================
bool NetTestUtil::sendAndRecievePing( VxTimer& pingTimer, VxSktConnectSimple& toClientConn, std::string& retPong, int receiveTimeout )
{
    std::string strNetCmd;
    std::string strPing = "PING";

    std::string netServChallengeHash;
    uint16_t cryptoKeyPort = toClientConn.getCryptoKeyPort();
    generateNetServiceChallengeHash( netServChallengeHash, cryptoKeyPort );

    NetTestUtil::buildNetCmd( strNetCmd, eNetCmdPing, netServChallengeHash, strPing, 0, 1 );

    LogModule( eLogIsPortOpenTest, LOG_ERROR, "## NetTestUtil::sendAndRecievePing: cypto port %d strNetCmd(%s) thread 0x%x", cryptoKeyPort, strNetCmd.c_str(), VxGetCurrentThreadId() );

    // startSendTime is also the time it took to connect
    double startSendTime = pingTimer.elapsedSec();
    RCODE rc = toClientConn.sendData( strNetCmd.c_str(), ( int )strNetCmd.length(), 8000 );
    if( rc )
    {
        double failSendTime = pingTimer.elapsedSec();
        LogModule( eLogIsPortOpenTest, LOG_ERROR, "## NetTestUtil::sendAndRecievePing: sendData error %d in %3.3f sec thread 0x%x", rc, failSendTime - startSendTime, VxGetCurrentThreadId() );
        toClientConn.closeSkt();
        return false;
    }

    double endSendTime = pingTimer.elapsedSec();
    char rxBuf[ 513 ];
    rxBuf[ 0 ] = 0;
    NetServiceHdr netServiceHdr;
    if( false == NetTestUtil::rxNetServiceCmd( &toClientConn, rxBuf, sizeof( rxBuf ) - 1, netServiceHdr, receiveTimeout, receiveTimeout ) )
    {
        double failResponseTime = pingTimer.elapsedSec();
        LogModule( eLogIsPortOpenTest, LOG_ERROR, "## NetTestUtil::sendAndRecievePing: no response with timeout spec %d and times connect %3.3f sec send %3.3f sec fail respond %3.3f sec thread 0x%x",
                   receiveTimeout, startSendTime, startSendTime - endSendTime, failResponseTime - startSendTime, VxGetCurrentThreadId() );
        toClientConn.closeSkt();
        return false;
    }

    rxBuf[ sizeof( rxBuf ) - 1 ]
    toClientConn.closeSkt();

    double successResponseTime = pingTimer.elapsedSec();

    if( netServiceHdr.m_ContentDataLen <= 0 )
    {
        LogModule( eLogIsPortOpenTest, LOG_ERROR, "## NetTestUtil::sendAndRecievePing: No Content connect %3.3f sec send %3.3f sec fail respond %3.3f sec thread 0x%x",
                   startSendTime, endSendTime - startSendTime, successResponseTime - startSendTime, VxGetCurrentThreadId() );
        return false;
    }

    if( ( 0 == netServiceHdr.m_TotalDataLen )
        || ( 511 <= netServiceHdr.m_TotalDataLen )
        || ( '/' != rxBuf[ netServiceHdr.m_ContentDataLen - 1 ] ) )
    {
        LogModule( eLogIsPortOpenTest, LOG_ERROR, "## NetTestUtil::sendAndRecievePing: invalid response  connect %3.3f sec send %3.3f sec fail respond %3.3f sec thread 0x%x",
                   startSendTime, endSendTime - startSendTime, successResponseTime - startSendTime, VxGetCurrentThreadId() );
        return false;
    }

    LogModule( eLogIsPortOpenTest, LOG_VERBOSE, "## NetTestUtil::sendAndRecievePing: SUCCESS  connect %3.3f sec send %3.3f sec response %3.3f sec thread 0x%x",
               startSendTime, endSendTime - startSendTime, successResponseTime - startSendTime, VxGetCurrentThreadId() );
    rxBuf[ netServiceHdr.m_ContentDataLen - 1 ] = 0;
    retPong = rxBuf;
    return true;
}

//============================================================================
bool NetTestUtil::sendAndRecieveQueryHostOnlineId( VxTimer& pingTimer, VxSktConnectSimple& toClientConn, std::string& retPong, int receiveTimeout )
{
    std::string strNetCmd;

    buildQueryHostIdUrl( VxSktConnectSimple * netServConn, std::string& strNetCmd );

    LogModule( eLogNetService, LOG_ERROR, "## NetTestUtil::sendAndRecieveQueryHostOnlineId: cypto port %d strNetCmd(%s) thread 0x%x", cryptoKeyPort, strNetCmd.c_str(), VxGetCurrentThreadId() );

    // startSendTime is also the time it took to connect
    double startSendTime = pingTimer.elapsedSec();
    RCODE rc = toClientConn.sendData( strNetCmd.c_str(), ( int )strNetCmd.length(), 8000 );
    if( rc )
    {
        double failSendTime = pingTimer.elapsedSec();
        LogModule( eLogNetService, LOG_ERROR, "## NetTestUtil::sendAndRecieveQueryHostOnlineId: sendData error %d in %3.3f sec thread 0x%x", rc, failSendTime - startSendTime, VxGetCurrentThreadId() );
        toClientConn.closeSkt();
        return false;
    }

    double endSendTime = pingTimer.elapsedSec();
    char rxBuf[ 513 ];
    rxBuf[ 0 ] = 0;
    NetServiceHdr netServiceHdr;
    if( false == NetTestUtil::rxNetServiceCmd( &toClientConn, rxBuf, sizeof( rxBuf ) - 1, netServiceHdr, receiveTimeout, receiveTimeout ) )
    {
        double failResponseTime = pingTimer.elapsedSec();
        LogModule( eLogNetService, LOG_ERROR, "## NetTestUtil::sendAndRecieveQueryHostOnlineId: no response with timeout spec %d and times connect %3.3f sec send %3.3f sec fail respond %3.3f sec thread 0x%x",
                   receiveTimeout, startSendTime, startSendTime - endSendTime, failResponseTime - startSendTime, VxGetCurrentThreadId() );
        toClientConn.closeSkt();
        return false;
    }

    rxBuf[ sizeof( rxBuf ) - 1 ]
        toClientConn.closeSkt();

    double successResponseTime = pingTimer.elapsedSec();

    if( netServiceHdr.m_ContentDataLen <= 0 )
    {
        LogModule( eLogNetService, LOG_ERROR, "## NetTestUtil::sendAndRecieveQueryHostOnlineId: No Content connect %3.3f sec send %3.3f sec fail respond %3.3f sec thread 0x%x",
                   startSendTime, endSendTime - startSendTime, successResponseTime - startSendTime, VxGetCurrentThreadId() );
        return false;
    }

    if( ( 0 == netServiceHdr.m_TotalDataLen )
        || ( 511 <= netServiceHdr.m_TotalDataLen )
        || ( '/' != rxBuf[ netServiceHdr.m_ContentDataLen - 1 ] ) )
    {
        LogModule( eLogNetService, LOG_ERROR, "## NetTestUtil::sendAndRecieveQueryHostOnlineId: invalid response  connect %3.3f sec send %3.3f sec fail respond %3.3f sec thread 0x%x",
                   startSendTime, endSendTime - startSendTime, successResponseTime - startSendTime, VxGetCurrentThreadId() );
        return false;
    }

    LogModule( eLogNetService, LOG_VERBOSE, "## NetTestUtil::sendAndRecieveQueryHostOnlineId: SUCCESS  connect %3.3f sec send %3.3f sec response %3.3f sec thread 0x%x",
               startSendTime, endSendTime - startSendTime, successResponseTime - startSendTime, VxGetCurrentThreadId() );
    rxBuf[ netServiceHdr.m_ContentDataLen - 1 ] = 0;
    retPong = rxBuf;
    return true;
}

//============================================================================
void NetTestUtil::generateNetServiceChallengeHash( std::string& strKeyHash, uint16_t clientPort )
{
    std::string strPwd;
    StdStringFormat( strPwd, "xs%ddfj%sd%d75!?jsaf",
                     clientPort,
                     getNetworkKey(),
                     clientPort );
    VxKey key;
    key.setKeyFromPassword( strPwd.c_str(), ( int )strPwd.size() );
    key.exportToAsciiString( strKeyHash );
}

//============================================================================
void NetTestUtil::generateNetServiceChallengeHash( std::string& strKey, VxSktBase * skt )
{
    uint16_t clientPort;
    if( skt->isAcceptSocket() )
    {
        clientPort = skt->m_RmtIp.getPort();
    }
    else
    {
        clientPort = skt->m_LclIp.getPort();
    }

    generateNetServiceChallengeHash( strKey, clientPort );
}

//============================================================================
RCODE NetTestUtil::buildAndSendCmd( VxSktBase * sktBase, ENetCmdType netCmd, std::string& cmdContent, int errCode, int version )
{
    std::string retResult;
    std::string netServChallengeHash;
    generateNetServiceChallengeHash( netServChallengeHash, sktBase );
    buildNetCmd( retResult, netCmd, netServChallengeHash, cmdContent, errCode, version );

    LogMsg( LOG_DEBUG, "buildAndSendCmd %s", retResult.c_str() );
    RCODE rc = sktBase->sendData( retResult.c_str(), retResult.length() );
    if( rc )
    {
        LogMsg( LOG_DEBUG, "buildAndSendCmd %s failed with error %d %s", retResult.c_str(), rc, VxSktBase::describeSktError( rc ) );
    }

    return rc;
}

//============================================================================
bool NetTestUtil::verifyAllDataArrivedOfNetServiceUrl( VxSktBase * sktBase )
{
    //http://GET /Crypto Key/total length of data/ 
    // 12 + 32 + 1 + MAX_CONTENT_LEN_DIGITS + 1

    int iSktDataLen = sktBase->getSktBufDataLen();
    if( iSktDataLen < NET_SERVICE_HDR_LEN )
    {
        sktBase->sktBufAmountRead( 0 );
        return false;
    }

    char *	pSktBuf = ( char * )sktBase->getSktReadBuf();

    int contentLen = getTotalLengthFromNetServiceUrl( pSktBuf, iSktDataLen );
    if( 0 >= contentLen )
    {
        sktBase->sktBufAmountRead( 0 );
        LogMsg( LOG_ERROR, "verifyAllDataArrivedOfNetServiceUrl: not valid\n" );
        sktBase->closeSkt( 633 );
        return false;
    }

    sktBase->sktBufAmountRead( 0 );
    return true;
}

//============================================================================
// returns content length or -1 if invalid url
int NetTestUtil::getTotalLengthFromNetServiceUrl( char * dataBuf, int dataLen )
{
    //                     			32
    //http://GET /url version/Crypto Key/total length of data/Net service command/VxGUID/cmd version/error code/content"
    //               15                46 
    //http://GET /001/                 /
    if( dataLen < NET_SERVICE_HDR_LEN )
    {
        return -1;
    }

    if( 0 != strncmp( dataBuf, "http://GET /", 12 ) )
    {
        LogMsg( LOG_ERROR, "getTotalLengthFromNetServiceUrl: invalid prefix\n" );
        return -1;
    }

    if( '/' != dataBuf[ 15 ] )
    {
        LogMsg( LOG_ERROR, "getTotalLengthFromNetServiceUrl: invalid / location1\n" );
        return -1;
    }

    if( '/' != dataBuf[ 48 ] )
    {
        LogMsg( LOG_ERROR, "getTotalLengthFromNetServiceUrl: invalid / location2\n" );
        return -1;
    }

    if( '/' != dataBuf[ 48 + MAX_CONTENT_LEN_DIGITS + 1 ] )
    {
        LogMsg( LOG_ERROR, "getTotalLengthFromNetServiceUrl: invalid / location3\n" );
        return -1;
    }

    int contentLen = atoi( &dataBuf[ 48 + 1 ] );
    if( ( NET_SERVICE_HDR_LEN > contentLen )
        || ( MAX_NET_SERVICE_URL_LEN < contentLen ) )
    {
        LogMsg( LOG_ERROR, "getTotalLengthFromNetServiceUrl: invalid content length %d\n", contentLen );
        return -1;
    }

    return contentLen;
}

//============================================================================
EPluginType NetTestUtil::parseHttpNetServiceUrl( VxSktBase * sktBase, NetServiceHdr& netServiceHdr )
{
    netServiceHdr.m_NetCmdType = eNetCmdUnknown;

    int iSktDataLen = sktBase->getSktBufDataLen();
    char *	pSktBuf = ( char * )sktBase->getSktReadBuf();
    pSktBuf[ iSktDataLen ] = 0;
    EPluginType pluginType = ( EPluginType )parseHttpNetServiceHdr( pSktBuf, iSktDataLen, netServiceHdr );
    if( ePluginTypeInvalid == pluginType )
    {
        LogMsg( LOG_ERROR, "Invalid Netservice URL\n" );
        sktBase->sktBufAmountRead( 0 );
        sktBase->closeSkt( 633 );
        return ePluginTypeInvalid;
    }

    sktBase->sktBufAmountRead( 0 );

    return pluginType;
}

//============================================================================
EPluginType NetTestUtil::parseHttpNetServiceHdr( char * dataBuf, int dataLen, NetServiceHdr& netServiceHdr )
{
    if( dataLen < NET_SERVICE_HDR_LEN )
    {
        LogMsg( LOG_ERROR, "parseHttpNetServiceHdr: data len < NET_SERVICE_HDR_LEN\n" );
        return ePluginTypeInvalid;
    }

    if( 0 != strncmp( dataBuf, "http://GET /", 12 ) )
    {
        LogMsg( LOG_ERROR, "parseHttpNetServiceHdr: not http prefix\n" );
        return ePluginTypeInvalid;
    }

    dataBuf += 12;
    int dataUsed = 12;

    std::string strValue;
    int partLen = getNextPartOfUrl( dataBuf, strValue );
    dataBuf += partLen + 1;
    dataUsed += partLen + 1;
    if( ( MAX_URL_VERSION_LEN_DIGITS != partLen )
        || ( dataUsed >= dataLen ) )
    {
        LogMsg( LOG_ERROR, "parseHttpNetServiceUrl: Invalid URL Version\n" );
        return ePluginTypeInvalid;
    }

    std::string strKey;
    partLen = getNextPartOfUrl( dataBuf, strKey );
    dataBuf += partLen + 1;
    dataUsed += partLen + 1;
    if( ( 0 == partLen )
        || ( dataUsed >= dataLen ) )
    {
        LogMsg( LOG_ERROR, "parseHttpNetServiceUrl: no data past crypto Key\n" );
        return ePluginTypeInvalid;
    }

    if( 32 != partLen )
    {
        LogMsg( LOG_ERROR, "parseHttpNetServiceUrl: crypto Key wrong length %d\n", partLen );
        return ePluginTypeInvalid;
    }

    // parse crypto key
    const char * pKeyBegin = strKey.c_str();
    if( false == isValidHexString( pKeyBegin, 32 ) )
    {
        LogMsg( LOG_ERROR, "parseHttpNetServiceUrl: crypto Key contains invalid chars %s\n", strKey.c_str() );
        return ePluginTypeInvalid;
    }

    netServiceHdr.m_ChallengeHash = strKey;

    partLen = getNextPartOfUrl( dataBuf, strValue );
    dataBuf += partLen + 1;
    dataUsed += partLen + 1;
    if( ( MAX_CONTENT_LEN_DIGITS != partLen )
        || ( dataUsed >= dataLen ) )
    {
        LogMsg( LOG_ERROR, "parseHttpNetServiceUrl: no data past total data length\n" );
        return ePluginTypeInvalid;
    }

    netServiceHdr.m_TotalDataLen = atoi( strValue.c_str() );
    if( netServiceHdr.m_TotalDataLen < NET_SERVICE_HDR_LEN )
    {
        LogMsg( LOG_ERROR, "parseHttpNetServiceUrl: invalid total data length %d\n", netServiceHdr.m_TotalDataLen );
        return ePluginTypeInvalid;
    }

    netServiceHdr.m_ContentDataLen = netServiceHdr.m_TotalDataLen - NET_SERVICE_HDR_LEN;

    // which net service command
    partLen = getNextPartOfUrl( dataBuf, strValue );
    dataBuf += partLen + 1;
    dataUsed += partLen + 1;
    if( ( 0 == partLen )
        || ( dataUsed >= dataLen ) )
    {
        LogMsg( LOG_ERROR, "parseHttpNetServiceUrl: no data past net command\n" );
        return ePluginTypeInvalid;
    }

    netServiceHdr.m_NetCmdType = netCmdStringToEnum( strValue.c_str() );
    if( eNetCmdUnknown == netServiceHdr.m_NetCmdType )
    {
        LogMsg( LOG_ERROR, "parseHttpNetServiceUrl: not known NET COMMAND\n" );
        return ePluginTypeInvalid;
    }

    partLen = getNextPartOfUrl( dataBuf, strValue );
    dataBuf += partLen + 1;
    dataUsed += partLen + 1;
    if( ( MAX_CMD_VERSION_LEN_DIGITS != partLen )
        || ( dataUsed >= dataLen ) )
    {
        LogMsg( LOG_ERROR, "parseHttpNetServiceUrl: no data past Net command\n" );
        return ePluginTypeInvalid;
    }

    netServiceHdr.m_CmdVersion = atoi( strValue.c_str() );

    partLen = getNextPartOfUrl( dataBuf, strValue );
    dataBuf += partLen + 1;
    dataUsed += partLen + 1;
    if( MAX_ERROR_LEN_DIGITS != partLen )
    {
        LogMsg( LOG_ERROR, "parseHttpNetServiceUrl: invalid error digit len\n" );
        return ePluginTypeInvalid;
    }

    netServiceHdr.m_CmdError = atoi( strValue.c_str() );

    netServiceHdr.m_SktDataUsed = dataUsed;

    EPluginType ePluginType = ePluginTypeNetServices;
    if( eNetCmdAboutMePage == netServiceHdr.m_NetCmdType )
    {
        ePluginType = ePluginTypeWebServer;
    }
    else if( eNetCmdStoryboardPage == netServiceHdr.m_NetCmdType )
    {
        ePluginType = ePluginTypeStoryboard;
    }

    LogMsg( LOG_VERBOSE, "parseHttpNetServiceUrl: cmd %s plugin %d\n", netCmdEnumToString( netServiceHdr.m_NetCmdType ), ePluginType );

    return ePluginType;
}

