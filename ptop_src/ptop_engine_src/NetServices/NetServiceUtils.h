#pragma once
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
// http://www.nolimitconnect.org
//============================================================================

#include "NetServiceDefs.h"

#include <ptop_src/ptop_engine_src/Network/NetworkDefs.h>

class VxSktBase;
class VxSktConnectSimple;
class VxGUID;
class P2PEngine;
class NetworkMgr;
class PktAnnounce;
class NetServiceHdr;
class VxKey;

class NetServiceUtils
{
public:
	NetServiceUtils( P2PEngine& engine );
    virtual ~NetServiceUtils() = default;

    NetworkMgr&					getNetworkMgr( void ) { return m_NetworkMgr; }

    std::string                 getNetworkKey( void );

    static ENetCmdType			netCmdStringToEnum( const char* netCmd );
	static const char*			netCmdEnumToString( ENetCmdType	eNetCmdType );

	EPluginType					parseHttpNetServiceUrl( VxSktBase* sktBase, NetServiceHdr& netServiceHdr );
	EPluginType					parseHttpNetServiceHdr( char * dataBuf, int dataLen, NetServiceHdr& netServiceHdr );
	bool						getNetServiceUrlContent( std::string& netServiceUrl, std::string& retFromClientContent );

	int							getIndexOfCrLfCrLf( VxSktBase* sktBase );

	RCODE						buildAndSendCmd( VxSktBase* sktBase, ENetCmdType netCmd, std::string& cmdContent, ENetCmdError errCode = eNetCmdErrorNone, int version = 1 );
    RCODE                       buildAndSendCmd( VxSktConnectSimple * sktBase, ENetCmdType netCmd, std::string& cmdContent, ENetCmdError errCode = eNetCmdErrorNone, int version = 1 );
    
    bool						buildIsMyPortOpenUrl( VxSktConnectSimple * netServConn, std::string& strHttpUrl, uint16_t u16Port );
    bool						buildQueryHostIdUrl( VxSktConnectSimple * netServConn, std::string& strNetCmdHttpUrl );
    bool						buildPingTestUrl( VxSktConnectSimple * netServConn, std::string& strNetCmdHttpUrl );
    bool                        buildHostPingReqUrl( VxSktConnectSimple* netServConn, std::string& strNetCmdHttpUrl );

    bool 						buildNetCmd( VxSktConnectSimple * netServConn, std::string& retResult, ENetCmdType netCmd, std::string& strContent, ENetCmdError errCode = eNetCmdErrorNone, int version = 1 );
    bool 						buildNetCmd( uint16_t cryptoPort, std::string& retResult, ENetCmdType netCmd, std::string& strContent, ENetCmdError errCode = eNetCmdErrorNone, int version = 1 );
    void						buildNetCmd( std::string& retResult, ENetCmdType netCmd, std::string& netServChallengeHash, std::string& strContent, ENetCmdError errCode = eNetCmdErrorNone, int version = 1 );

    // returns total length of data to send
    int							buildNetCmdHeader( std::string& retResult, ENetCmdType netCmd, std::string& netServChallengeHash, int contentLength, ENetCmdError errCode = eNetCmdErrorNone, int version = 1 );

    static void					generateNetServiceChallengeHash(	std::string&			strKey,
                                                                    VxSktBase*				skt,
                                                                    std::string             netKey);
    static void					generateNetServiceChallengeHash(	std::string&			strKey,
                                                                    VxSktConnectSimple *	skt,
                                                                    std::string             netKey);
    static void					generateNetServiceChallengeHash(	std::string&			strKey,		// set this key
                                                                    uint16_t				clientPort,
                                                                    std::string             netKey);
    static void					generateNetServiceCryptoKey(		VxKey&					key,
                                                                    uint16_t				clientPort,
                                                                    std::string             netKey);
	static bool					verifyAllDataArrivedOfNetServiceUrl( VxSktBase* sktBase );
	// returns content length or -1 if invalid url
	static int					getTotalLengthFromNetServiceUrl(  char * dataBuf, int dataLen );

	bool						decryptNetServiceContent( char * content, int contentDataLen, uint16_t clientPort );
	bool						rxNetServiceCmd(	VxSktConnectSimple * netServConn, 
													char *				rxBuf, 
													int					rxBufLen, 
													NetServiceHdr&		netServiceHdr, 
													int					rxHdrTimeout = NETSERVICE_RX_URL_HDR_TIMEOUT, 
													int					rxDataTimeout = NETSERVICE_RX_DATA_TIMEOUT );
	bool						isValidHexString( const char* hexString, int dataLen );

protected:
	int							getNextPartOfUrl( char * buf, std::string& strValue );

	P2PEngine&					m_Engine;
	NetworkMgr&					m_NetworkMgr;
	PktAnnounce&				m_PktAnn;
};


