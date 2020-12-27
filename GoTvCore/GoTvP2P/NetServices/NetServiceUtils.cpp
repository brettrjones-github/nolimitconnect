//============================================================================
// Copyright (C) 2014 Brett R. Jones
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

#include "NetServiceUtils.h"

#include <GoTvCore/GoTvP2P/P2PEngine/P2PEngine.h>
#include <GoTvCore/GoTvP2P/Network/NetworkMgr.h>
#include <GoTvCore/GoTvP2P/NetServices/NetServiceHdr.h>

#include <PktLib/PktAnnounce.h>
#include <NetLib/VxSktBase.h>
#include <NetLib/VxSktConnectSimple.h>
#include <CoreLib/VxParse.h>

#include <stdlib.h>
#include <string.h>

namespace
{
	//const int		HTTP_GET_WITH_VER_LEN				= 16;
	const int		MAX_URL_VERSION_LEN_DIGITS			= 3;
	const int		MAX_CONTENT_LEN_DIGITS				= 13;
	const int		MAX_CMD_VERSION_LEN_DIGITS			= 3;
	const int		MAX_ERROR_LEN_DIGITS				= 8;
	const int		MAX_NET_CMD_LEN_CHARS				= 19;
	const int		MAX_NET_SERVICE_URL_LEN				= 16384;
}

//============================================================================
NetServiceUtils::NetServiceUtils( P2PEngine& engine )
: m_Engine( engine )
, m_NetworkMgr( engine.getNetworkMgr() )
, m_PktAnn( engine.getMyPktAnnounce() )
{
}

//============================================================================
NetServiceUtils::~NetServiceUtils()
{
}

//============================================================================
bool NetServiceUtils::verifyAllDataArrivedOfNetServiceUrl( VxSktBase * sktBase )
{
	//http://GET /Crypto Key/total length of data/ 
	// 12 + 32 + 1 + MAX_CONTENT_LEN_DIGITS + 1

	int iSktDataLen = sktBase->getSktBufDataLen();
	if( iSktDataLen < NET_SERVICE_HDR_LEN )
	{
		sktBase->sktBufAmountRead( 0 );
		return false;
	}

	char *	pSktBuf = (char *)sktBase->getSktReadBuf();

	int contentLen = getTotalLengthFromNetServiceUrl( pSktBuf, iSktDataLen );
	if( 0 >= contentLen )
	{
		sktBase->sktBufAmountRead( 0 );
		LogMsg( LOG_ERROR, "verifyAllDataArrivedOfNetServiceUrl: not valid\n" );
		VxReportHack( sktBase, "parseHttpNetServiceUrl: not http prefix\n" );
		sktBase->closeSkt( 633 );
		return false;
	}

	sktBase->sktBufAmountRead( 0 );
	return true; 
}

//============================================================================
// returns content length or -1 if invalid url
int  NetServiceUtils::getTotalLengthFromNetServiceUrl(  char * dataBuf, int dataLen )
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
		LogMsg( LOG_ERROR, "getTotalLengthFromNetServiceUrl: invalid prefix\n");
		return -1;
	}

	if( '/' != dataBuf[15] )
	{
		LogMsg( LOG_ERROR, "getTotalLengthFromNetServiceUrl: invalid / location1\n");
		return -1;
	}

	if( '/' != dataBuf[48] )
	{
		LogMsg( LOG_ERROR, "getTotalLengthFromNetServiceUrl: invalid / location2\n");
		return -1;
	}

	if( '/' != dataBuf[ 48 + MAX_CONTENT_LEN_DIGITS + 1 ] )
	{
		LogMsg( LOG_ERROR, "getTotalLengthFromNetServiceUrl: invalid / location3\n");
		return -1;
	}

	int contentLen = atoi( &dataBuf[ 48 + 1 ] );
	if( ( NET_SERVICE_HDR_LEN > contentLen   )
		|| ( MAX_NET_SERVICE_URL_LEN < contentLen ) )
	{
		LogMsg( LOG_ERROR, "getTotalLengthFromNetServiceUrl: invalid content length %d\n", contentLen );
		return -1;
	}

	return contentLen;
}


//============================================================================
void  NetServiceUtils::buildNetCmd( std::string& retResult, ENetCmdType netCmd, std::string& netServChallengeHash, std::string& strContent, int errCode, int version )
{
	std::string strNetCmd = netCmdEnumToString( netCmd );
	//http://GET /  1/ = len 16 + 6 /'s
	//http://GET /url version/Crypto Key/total length of data/Net service command/cmd version/error code/content/"
	// total header = 16 + 6 + 32 + 13 + 19 + 3 + 8   = 96

	int totalLen = (int)(16 + 6
				+ netServChallengeHash.length()
				+ MAX_CONTENT_LEN_DIGITS 
				+ MAX_NET_CMD_LEN_CHARS
				+ MAX_CMD_VERSION_LEN_DIGITS
				+ MAX_ERROR_LEN_DIGITS
				+ strContent.length());

	if( strContent.length() )
	{
		StdStringFormat( retResult, "http://GET /  1/%s/%13d/%s/%3d/%8d/%s/", 
			netServChallengeHash.c_str(), 
			totalLen, 
			strNetCmd.c_str(), 
			version, 
			errCode,
			strContent.c_str() );
	}
	else
	{
		StdStringFormat( retResult, "http://GET /  1/%s/%13d/%s/%3d/%8d//", 
			netServChallengeHash.c_str(), 
			totalLen, 
			strNetCmd.c_str(), 
			version, 
			errCode );
	}

    LogModule( eLogNetService, LOG_INFO, "buildNetCmd %s", retResult.c_str() );
}

//============================================================================
int  NetServiceUtils::buildNetCmdHeader( std::string& retResult, ENetCmdType netCmd, std::string& netServChallengeHash, int contentLength, int errCode, int version )
{
	std::string strNetCmd = netCmdEnumToString( netCmd );
	//http://GET /  1/ = len 16
	// + 5 /s  = 22 for header and /'s

	int totalLen = (int)(16 + 5
		+ netServChallengeHash.length()
		+ MAX_CONTENT_LEN_DIGITS 
		+ MAX_NET_CMD_LEN_CHARS
		+ MAX_CMD_VERSION_LEN_DIGITS
		+ MAX_ERROR_LEN_DIGITS
		+ contentLength);

	StdStringFormat( retResult, "http://GET /  1/%s/%13d/%s/%3d/%8d/", 
		netServChallengeHash.c_str(), 
		totalLen, 
		strNetCmd.c_str(), 
		version, 
		errCode );
	return totalLen;
}

//============================================================================
bool NetServiceUtils::buildIsMyPortOpenUrl( VxSktConnectSimple * netServConn, std::string& strHttpUrl, uint16_t u16Port )
{
    std::string strContent;
    StdStringFormat( strContent, "%d", u16Port );

    return buildNetCmd( netServConn, strHttpUrl, eNetCmdIsMyPortOpenReq, strContent );
}

//============================================================================
bool NetServiceUtils::buildQueryHostIdUrl( VxSktConnectSimple * netServConn, std::string& strNetCmdHttpUrl )
{
    std::string strContent = "QUERYHOSTID";

    return buildNetCmd( netServConn, strNetCmdHttpUrl, eNetCmdQueryHostOnlineIdReq, strContent );
}

//============================================================================
bool NetServiceUtils::buildPingTestUrl( VxSktConnectSimple * netServConn, std::string& strNetCmdHttpUrl )
{
    std::string strContent = "PING";

    return buildNetCmd( netServConn, strNetCmdHttpUrl, eNetCmdPing, strContent );
}

//============================================================================
bool NetServiceUtils::buildNetCmd( VxSktConnectSimple * netServConn, std::string& retResult, ENetCmdType netCmd, std::string& strContent, int errCode, int version )
{
    if( netServConn && netServConn->isConnected() )
    {
        uint16_t cryptoKeyPort = netServConn->getCryptoKeyPort();
        return buildNetCmd( cryptoKeyPort, retResult, netCmd, strContent, errCode, version );
    }

    return false;
}

//============================================================================
bool NetServiceUtils::buildNetCmd( uint16_t cryptoKeyPort, std::string& retResult, ENetCmdType netCmd, std::string& strContent, int errCode, int version )
{
    if( cryptoKeyPort )
    {
        std::string netServChallengeHash;
        generateNetServiceChallengeHash( netServChallengeHash, cryptoKeyPort );
        buildNetCmd( retResult, netCmd, netServChallengeHash, strContent, errCode, version );
        return !retResult.empty();
    }

    return false;
}

//============================================================================
EPluginType NetServiceUtils::parseHttpNetServiceUrl( VxSktBase * sktBase, NetServiceHdr& netServiceHdr )
{
	netServiceHdr.m_NetCmdType = eNetCmdUnknown;

	int iSktDataLen = sktBase->getSktBufDataLen();
	char *	pSktBuf = (char *)sktBase->getSktReadBuf();
	pSktBuf[ iSktDataLen ] = 0;
	EPluginType pluginType = parseHttpNetServiceHdr( pSktBuf, iSktDataLen, netServiceHdr );
	if( ePluginTypeInvalid == pluginType )
	{
		VxReportHack( sktBase, "Invalid Netservice URL\n" );
		sktBase->sktBufAmountRead( 0 );
		sktBase->closeSkt( 633 );
		return ePluginTypeInvalid;
	}

	sktBase->sktBufAmountRead( 0 );

	return pluginType;
}

//============================================================================
bool NetServiceUtils::isValidHexString( const char * hexString, int dataLen )
{
	for( int i = 0; i < dataLen; i++ )
	{
		char ch = hexString[ i ];
		if( false == ( (('0' <= ch) && ('9' >= ch)) 
						|| (('A' <= ch) && ('F' >= ch))
						|| (('a' <= ch) && ('f' >= ch)) ) )
		{
			return false;
		}
	}

	return true;
}

//============================================================================
EPluginType NetServiceUtils::parseHttpNetServiceHdr( char * dataBuf, int dataLen, NetServiceHdr& netServiceHdr )
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
    else if( eNetCmdQueryHostOnlineIdReq == netServiceHdr.m_NetCmdType )
    {
        ePluginType = ePluginTypeHostNetwork;
    }
    else if( eNetCmdIsMyPortOpenReq == netServiceHdr.m_NetCmdType )
    {
        ePluginType = ePluginTypeServiceConnectTest;
    }

    LogMsg( LOG_VERBOSE, "parseHttpNetServiceUrl: cmd %s plugin %d\n", netCmdEnumToString( netServiceHdr.m_NetCmdType ), ePluginType );

	return ePluginType;
}

//============================================================================
int  NetServiceUtils::getNextPartOfUrl( char * buf, std::string& strValue )
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
		len = (int)strValue.length();
	}
	return len;
}

//============================================================================
bool  NetServiceUtils::getNetServiceUrlContent( std::string& netServiceUrl, std::string& retFromClientContent )
{
	retFromClientContent = "";
	if( NET_SERVICE_HDR_LEN >= netServiceUrl.length() )
	{
		LogMsg( LOG_ERROR, "NetServiceUtils::getNetServiceUrlContent: invalid netService Length\n" );
		return false;
	}

	const char * buf1 = netServiceUrl.c_str();
	retFromClientContent = &buf1[ NET_SERVICE_HDR_LEN ];
	if( 0 == retFromClientContent.length() )
	{
		LogMsg( LOG_ERROR, "NetServiceUtils::getNetServiceUrlContent: invalid content Length\n" );
		return false;
	}

	const char * content = retFromClientContent.c_str();
	if( '/' != content[ retFromClientContent.length() - 1 ] )
	{
		LogMsg( LOG_ERROR, "NetServiceUtils::getNetServiceUrlContent: no trailing /\n" );
		retFromClientContent = "";
		return false;
	}

	((char *)content)[ retFromClientContent.length() - 1 ] = 0;
	return true;
}

//============================================================================
ENetCmdType  NetServiceUtils::netCmdStringToEnum( const char * netCmd )
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
const char *  NetServiceUtils::netCmdEnumToString( ENetCmdType	eNetCmdType )
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
	case eNetCmdHostReply	:
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
int  NetServiceUtils::getIndexOfCrLfCrLf( VxSktBase * sktBase )
{
	int indexOfCrLfCrLf = -1;
	int iDataLen = sktBase->getSktBufDataLen();
	if( iDataLen >= 4 )
	{
		char *	pSktBuf = (char *)sktBase->getSktReadBuf();
		pSktBuf[ iDataLen ] = 0;

		char * pEndPtr = strstr( pSktBuf, "\r\n\r\n" );
		if( pEndPtr )
		{
			indexOfCrLfCrLf = (int)( pEndPtr - pSktBuf );
		}

		sktBase->sktBufAmountRead( 0 );
	}

	return indexOfCrLfCrLf;
}

//============================================================================
RCODE NetServiceUtils::buildAndSendCmd( VxSktBase * sktBase, ENetCmdType netCmd, std::string& cmdContent, int errCode, int version )
{
	std::string retResult;
	std::string netServChallengeHash;
	generateNetServiceChallengeHash( netServChallengeHash, sktBase );
	buildNetCmd( retResult, netCmd, netServChallengeHash, cmdContent, errCode, version );

	return sktBase->sendData( retResult.c_str(), (int)retResult.length() );
}

//============================================================================
RCODE NetServiceUtils::buildAndSendCmd( VxSktConnectSimple * sktBase, ENetCmdType netCmd, std::string& cmdContent, int errCode, int version )
{
    std::string retResult;
    std::string netServChallengeHash;
    generateNetServiceChallengeHash( netServChallengeHash, sktBase );
    buildNetCmd( retResult, netCmd, netServChallengeHash, cmdContent, errCode, version );

    return sktBase->sendData( retResult.c_str(), ( int )retResult.length() );
}

//============================================================================
void NetServiceUtils::generateNetServiceChallengeHash(	std::string&			strKey,	
														VxSktBase *				skt )
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
void NetServiceUtils::generateNetServiceChallengeHash(	std::string&			strKey,	
														VxSktConnectSimple *	skt )
{
	uint16_t clientPort = skt->m_LclIp.getPort();
	generateNetServiceChallengeHash( strKey, clientPort );
}

//============================================================================
void NetServiceUtils::generateNetServiceChallengeHash(	std::string&			strKeyHash,	
														uint16_t				clientPort )
{
	std::string strPwd;
	StdStringFormat( strPwd, "xs%ddfj%sd%d75!?jsaf", 
		clientPort,
        m_NetworkMgr.getNetworkKey(),
		clientPort );

	VxKey key;
	key.setKeyFromPassword( strPwd.c_str(), (int)strPwd.size() );
	key.exportToAsciiString( strKeyHash );
    LogModule( eLogNetService, LOG_INFO, "generateNetServiceChallengeHash: clientPort = %d, network %s hash %s", 
               clientPort, m_NetworkMgr.getNetworkKey(), strPwd.c_str() );
}

//============================================================================
void NetServiceUtils::generateNetServiceCryptoKey(	VxKey& key, uint16_t clientPort )
{
	std::string strPwd;
	StdStringFormat( strPwd, "xz&gdf%d%s!?d%d759sdc", 
		             clientPort,
		             m_NetworkMgr.getNetworkKey(), 
		             clientPort );
	key.setKeyFromPassword( strPwd.c_str(), (int)strPwd.size() );
}

//============================================================================
bool NetServiceUtils::decryptNetServiceContent( char * content, int contentDataLen, uint16_t clientPort )
{
	if( ( 0 == contentDataLen )
		|| ( 0x0f & contentDataLen ) )
	{
		LogMsg( LOG_ERROR, "NetActionAnnounce::decryptNetServiceContent: invalid length %d\n", contentDataLen );
		return false;
	}

	VxKey key;
	generateNetServiceCryptoKey( key, clientPort );
	VxSymDecrypt( &key, content, contentDataLen );

	return true;
}

//============================================================================
bool NetServiceUtils::rxNetServiceCmd( VxSktConnectSimple * netServConn, char * rxBuf, int rxBufLen, NetServiceHdr& netServiceHdr, int rxHdrTimeout, int rxDataTimeout )
{
	vx_assert( rxBufLen > NET_SERVICE_HDR_LEN );

	int iRxed = 0;
	bool bGotCrLfCrLf = false;
    VxTimer rxCmdTimer;
	netServConn->recieveData(		rxBuf,					// data buffer to read into
									NET_SERVICE_HDR_LEN,	// length of data	
									&iRxed,					// number of bytes actually received
									rxHdrTimeout,			// timeout attempt to receive
									false, 					// if true then abort receive when \r\n\r\n is received
									&bGotCrLfCrLf );						

	if( iRxed != NET_SERVICE_HDR_LEN )
	{
		LogMsg( LOG_ERROR, "### ERROR NetServiceUtils::rxNetServiceCmd: hdr timeout %3.3f sec rxed data len %d\n", rxCmdTimer.elapsedSec(), iRxed );
		return false;
	}

	rxBuf[ NET_SERVICE_HDR_LEN ] = 0;
	if( ePluginTypeNetServices != parseHttpNetServiceHdr( rxBuf, NET_SERVICE_HDR_LEN, netServiceHdr ) )
	{
		LogMsg( LOG_ERROR, "### ERROR NetServiceUtils::rxNetServiceCmd: hdr parse error\n" );
		return false;
	}

    if( netServiceHdr.m_TotalDataLen <= NET_SERVICE_HDR_LEN )
    {
        LogMsg( LOG_ERROR, "### ERROR NetServiceUtils::rxNetServiceCmd: too smal netServiceHdr.m_TotalDataLen %d\n", netServiceHdr.m_TotalDataLen );
        return false;
    }

	if( netServiceHdr.m_TotalDataLen > rxBufLen )
	{
		LogMsg( LOG_ERROR, "### ERROR NetServiceUtils::rxNetServiceCmd: too large netServiceHdr.m_TotalDataLen %d\n", netServiceHdr.m_TotalDataLen );
		return false;
	}

	int contentLen = netServiceHdr.m_TotalDataLen - NET_SERVICE_HDR_LEN;
    VxTimer rxTimer;
	netServConn->recieveData(		rxBuf,					// data buffer to read into
									contentLen,				// length of data	
									&iRxed,					// number of bytes actually received
									rxDataTimeout,			// timeout attempt to receive
									false, 					// if true then abort receive when \r\n\r\n is received
									&bGotCrLfCrLf );	
	if( contentLen != iRxed )
	{
		LogMsg( LOG_ERROR, "### ERROR NetServiceUtils::rxNetServiceCmd: timeout %3.3f sec recieving content\n", rxTimer.elapsedSec() );
		return false;
	}

	netServiceHdr.m_ContentDataLen = contentLen;
	rxBuf[ contentLen ] = 0;
	return true;
}
