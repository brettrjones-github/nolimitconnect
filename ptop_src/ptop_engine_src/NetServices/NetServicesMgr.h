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
// http://www.nolimitconnect.com
//============================================================================

#include "NetServiceDefs.h"
#include "NetServiceUtils.h"
#include "NetServiceHost.h"

#include "NetActionIdle.h"

#include <NetLib/VxSktConnectSimple.h>
#include <CoreLib/VxThread.h>
#include <CoreLib/VxMutex.h>
#include <CoreLib/VxSemaphore.h>
#include <CoreLib/AppErr.h>
#include <CoreLib/VxTimer.h>

#ifdef TARGET_OS_WINDOWS
# include <ptop_src/ptop_engine_src/Network/Firewall.h>
#endif // TARGET_OS_WINDOWS

class VxSktBase;
class VxGUID;
class P2PEngine;
class EngineSettings;
class PktAnnounce;
class NetworkMgr;
class HostList;

typedef void (*MY_PORT_OPEN_CALLBACK_FUNCTION )( void *, ENetCmdError, std::string& );
typedef void( *QUERY_HOST_ID_CALLBACK_FUNCTION )( void *, ENetCmdError, VxGUID& );

class NetServicesMgr
{
public:
	NetServicesMgr( P2PEngine& engine );
	virtual ~NetServicesMgr();
	NetServicesMgr() = delete; // don't allow default constructor
	NetServicesMgr( const NetServicesMgr& ) = delete; // don't allow copy constructor

	static const int			MINIMUM_CONTENT_PARTS		= 9;
	static const int			CONTENT_PART_IDX_VERSION	= 5;
	static const int			CONTENT_PART_IDX_ERROR		= 6;
	static const int			CONTENT_PART_IDX_PAYLOAD	= 7;

	P2PEngine&					getEngine( void )			{ return m_Engine; }
    EngineSettings&				getEngineSettings( void )	{ return m_EngineSettings; }
	NetServiceUtils&			getNetUtils( void )			{ return m_NetServiceUtils; }
	NetServiceHost&			    getNetServiceHost( void )	{ return m_NetServiceHost; }

    std::string                 getNetworkKey( void );
	VxGUID&						getMyOnlineId( void );

	void						netServicesStartup( void );
	void						netServicesShutdown( void );

	void						addNetActionIsMyPortOpenToQueue( void );
	void						announceToHost( std::string& anchorIp, uint16_t u16HostPort, EHostAction eHostAction = eHostActionAnnounce );
	void						performRandomConnect( void );

	void						netActionResultIsMyPortOpen( ENetCmdError eCmdErr, std::string& myExternalIp );
    void                        netActionResultQueryHostId( ENetCmdError eCmdErr, VxGUID& hostId );
	void						netActionResultAnnounce( ENetCmdError eCmdErr, HostList * anchorList, EHostAction eHostAction = eHostActionAnnounce );
	void						netActionResultRandomConnect( ENetCmdError eCmdErr, HostList * anchorList );

	void						setMyPortOpenResultCallback( MY_PORT_OPEN_CALLBACK_FUNCTION pfuncPortOpenCallbackHandler, void * userData );
    void						setQueryHostOnlineIdResultCallback( QUERY_HOST_ID_CALLBACK_FUNCTION pfuncQueryHostIdCallbackHandler, void * userData );

	RCODE						handleNetCmdPing( VxSktBase * sktBase, NetServiceHdr& netServiceHdr );
	RCODE						handleNetCmdPong( VxSktBase * sktBase, NetServiceHdr& netServiceHdr );
	RCODE						handleNetCmdIsMyPortOpenReq( VxSktBase * sktBase, NetServiceHdr& netServiceHdr );
	RCODE						handleNetCmdIsMyPortOpenReply( VxSktBase * sktBase, NetServiceHdr& netServiceHdr );
	RCODE						handleNetCmdHostReq( VxSktBase * sktBase, NetServiceHdr& netServiceHdr );
	RCODE						handleNetCmdHostReply( VxSktBase * sktBase, NetServiceHdr& netServiceHdr );
    RCODE						handleNetCmdQueryHostIdReq( VxSktBase * sktBase, NetServiceHdr& netServiceHdr );
    RCODE						handleNetCmdQueryHostIdReply( VxSktBase * sktBase, NetServiceHdr& netServiceHdr );

	void						runNetActions( void );

	VxSktConnectSimple *		actionReqConnectToNetService( void );
	bool						actionReqConnectToNetService( VxSktConnectSimple& sktSimple );
	VxSktConnectSimple *		actionReqConnectToHost( void );
	bool						actionReqConnectToHost( VxSktConnectSimple& sktSimple );

	bool						doNetCmdPing( const char * ipAddress, uint16_t u16Port, std::string& retPong );

	ENetCmdError				doIsMyPortOpen( std::string& retMyExternalIp, bool testLoopbackFirst = false );
	bool						testLoobackPing( std::string localIP, uint16_t tcpListenPort );
	ENetCmdError				sendAndRecieveIsMyPortOpen( VxTimer&				portTestTimer,
															VxSktConnectSimple *	sktSimple, 
															int						tcpListenPort,
															std::string&			retMyExternalIp,
															bool					sendMsgToUser );
	ENetCmdError                sendAndRecieveQueryHostId( VxTimer&				portTestTimer,
                                                           VxSktConnectSimple *	netServConn,
                                                           VxGUID&			    retHostId,
                                                           bool					sendMsgToUser );
	bool						sendAndRecievePing( VxTimer& pingTimer, VxSktConnectSimple& toClientConn, std::string& retPong, int receiveTimeout = 3000 );

protected:
	void						addNetActionCommand( NetActionBase * netActionBase );
	bool						isActionQued( ENetActionType eNetActionType );
	RCODE						sendPong( VxSktBase * sktBase, NetServiceHdr& netServiceHdr );

	//=== vars ===//
#ifdef TARGET_OS_WINDOWS
	Firewall					m_WindowsFirewall;
#endif // TARGET_OS_WINDOWS

	P2PEngine&					m_Engine;
	PktAnnounce&				m_PktAnn;
	EngineSettings&				m_EngineSettings;
	NetworkMgr&					m_NetworkMgr;
	NetServiceUtils				m_NetServiceUtils;
	NetServiceHost			    m_NetServiceHost;

	std::vector<NetActionBase*>	m_NetActionList;
	NetActionIdle				m_NetActionIdle;
	NetActionBase *				m_CurNetAction;

	VxThread					m_NetActionThread;
	VxMutex						m_NetActionMutex;
	VxSemaphore					m_NetActionSemaphore;

	VxSktConnectSimple			m_SktToNetServices;
    MY_PORT_OPEN_CALLBACK_FUNCTION	m_pfuncPortOpenCallbackHandler{nullptr};
    QUERY_HOST_ID_CALLBACK_FUNCTION	m_pfuncQueryHostIdCallbackHandler{ nullptr };

    void *						m_PortOpenCallbackUserData{nullptr};
    void *						m_QueryHostIdCallbackUserData{ nullptr };

	VxSktConnectSimple			m_SktToHost;
};



