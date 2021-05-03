#pragma once
//============================================================================
// Copyright (C) 2010 Brett R. Jones
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

#include "PluginSessionMgr.h"
#include "PluginBaseService.h"

#include <CoreLib/VxTimer.h>
#include <CoreLib/VxGUIDList.h>

class RelayClientTestSession;
class RelayServerTestSession;
class RelaySession;
class RelayClientSession;
class RelayServerSession;
class BigListInfo;
class PktRelayServiceReq;
class PktRelayServiceReply;

class PluginBaseRelay : public PluginBaseService
{
public:
	PluginBaseRelay( P2PEngine& engine, PluginMgr& pluginMgr, VxNetIdent * myIdent, EPluginType pluginType );
	virtual ~PluginBaseRelay() = default;

	bool						verboseLog( void )					{ return m_bVerbose; }
	bool						isTest( void )						{ return m_bTest; }	

	virtual bool				fromGuiMakePluginOffer( VxNetIdent *	netIdent, 
                                                        int             pvUserData,
														const char *	pOfferMsg, 
														const char *	pFileName = 0,
                                                        uint8_t *		fileHashId = 0,
														VxGUID			lclSessionId = VxGUID::nullVxGUID() );
	virtual void				fromGuiRelayPermissionCount( int userPermittedCount, int anonymousCount ); 

	EPluginAccess			handlePktRelayServiceReq( BigListInfo * bigListInfo, VxSktBase * sktBase, PktRelayServiceReq * relayServiceReq, PktRelayServiceReply& pktReply );
    bool 						isUserRelayOk( const VxGUID& srcOnlineId, const VxGUID& destOnlineId );

	RelaySession *				startNewRelayService( VxSktBase * sktBase, VxNetIdent * netIdent, VxGUID& rmtInstance );
	RelaySession *				startNewRelayClient( VxSktBase * sktBase, VxNetIdent * netIdent, VxGUID& rmtInstance );

	virtual void				onSessionEnded( PluginSessionBase * poSession, bool pluginIsLocked, 
												EOfferResponse eOfferResponse = eOfferResponseUserOffline );

protected:
	virtual void				onPktRelayServiceReply			( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent );
	virtual void				onPktRelayConnectReq			( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent );
	virtual void				onPktRelayConnectReply			( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent );
	virtual void				onPktRelaySessionReq			( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent );
	virtual void				onPktRelaySessionReply			( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent );
	virtual void				onPktRelayConnect				( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent );
	virtual void				onPktRelayDisconnect			( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent );
	virtual void				onPktRelayTestReq				( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent );
	virtual void				onPktRelayTestReply				( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent );

	virtual	bool				doRelayTest( VxNetIdent * netIdent );
	virtual bool				onTestAll(	RelayClientTestSession * poClientSession, 
											RelayServerTestSession * poServerSession, 
											VxNetIdent * poLclIdent, 
											VxNetIdent * poRmtIdent, 
											VxNetIdent * po3rdIdent );

	virtual void				onTestComplete( void );

	virtual bool				onTestServerClient(	RelayClientTestSession * poClientSession, 
													RelayServerTestSession * poServerSession, 
													VxNetIdent * poLclIdent, 
													VxNetIdent * poRmtIdent );

	virtual bool				onTestRequestRelayService(	RelayClientTestSession * poClientSession, VxNetIdent * poRmtIdent );
	//! test to verify can be used as proxy service
	virtual bool				onTestAcceptRelayServiceRequest( VxNetIdent * netIdent );
	//! test can use current proxy session to connect to another user
	virtual bool				onTestConnectUsingLclRelay( RelaySession * poSession, VxNetIdent * netIdent );
	//! test can use normal connection to connect to remote proxy which is connected to user 
	virtual bool				onTestConnectUsingRmtRelay( VxNetIdent * netIdent );

	virtual EPluginAccess	canAcceptNewSession( VxNetIdent * netIdent );

	RelaySession *				requestRelayService( VxNetIdent * netIdent, RelayClientSession * poSessionIn = NULL, bool bTest = false );
	void						endSession( RelaySession * poSession );
    PluginSessionBase *			findRelay( const VxGUID& onlineId );
	RelayClientSession *		findOrCreateRelayClient( VxSktBase * sktBase, VxNetIdent * netIdent );		
	RelayServerSession *		findOrCreateRelayServer(  VxSktBase * sktBase, VxNetIdent * netIdent );

	//! called when new better connection from user
	virtual void				replaceConnection( VxNetIdent * netIdent, VxSktBase * poOldSkt, VxSktBase * poNewSkt );
	virtual void				onContactWentOffline( VxNetIdent * netIdent, VxSktBase * sktBase );
	void						onConnectionLost( VxSktBase * sktBase );

	//=== constants ===//
	typedef long (PluginBaseRelay::*RELAY_PKT_FUNCTION)( VxSktBase *, VxPktHdr *, VxNetIdent * );  

	//=== vars ====//
	PluginSessionMgr			m_PluginSessionMgr;

	VxTimer						m_TestTimer;
	bool						m_bVerbose;
	bool						m_bTest;
	uint32_t					m_MaxUserRelayAllowCnt;
	uint32_t					m_MaxAnonRelayAllowCnt;
	VxGUIDList					m_FriendGuidList;
	VxGUIDList					m_AnonGuidList;
};



