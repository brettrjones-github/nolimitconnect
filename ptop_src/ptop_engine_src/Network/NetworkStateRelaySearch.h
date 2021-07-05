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

#include "ConnectRequest.h"
#include "NetworkStateBase.h"

#include <ptop_src/ptop_engine_src/HostMgr/HostDefs.h>

#include <CoreLib/VxThread.h>
#include <CoreLib/VxMutex.h>
#include <CoreLib/VxSemaphore.h>
#include <PktLib/VxCommon.h>

class P2PConnectList;
class ContactList;
class PossibleRelayList;
class NetworkMgr;
class NetServiceUtils;
class HostList;
class VxSktConnectSimple;
class DirectConnectTester;


class NetworkStateRelaySearch : public NetworkStateBase
{
public:
	NetworkStateRelaySearch( NetworkStateMachine& stateMachine );
	virtual ~NetworkStateRelaySearch();

	virtual void				enterNetworkState( void );
	virtual void				exitNetworkState( void );
	void						runNetworkState( void );
	virtual void				fromGuiUseRelay( VxConnectInfo& connectInfo, bool useRelay ); // so if searching for relay can try immediately 
	virtual void				onPktRelayServiceReply( VxSktBase * sktBase, PktRelayServiceReply * pkt );
	virtual bool				handlePossibleRelayConnect(	VxConnectInfo&		connectInfo, 
															VxSktBase *			sktBase,
															bool				isNewConnection,
															EConnectReason		connectReason );

	void						doRelayAnnounceListAndSearch( void );
	void						setRelayListSearchComplete( bool isComplete )			{ m_SearchThreadComplete = isComplete; }

protected:
	void						getMoreRelaysToPick( void );
	bool						checkHaveRelayService( void );
	void						removeContactInfo( VxConnectInfo& connectInfo );
	void						addToBusyList( ConnectRequest& connectInfo );
	void						removeFromBusyList( VxConnectInfo& connectInfo );
	void						startAnnounceServersListAndSearchThread( void );
	void						moveToAnnounceState( void );

	void						doRelaySearchState( void );
	void						doPreferedRelayList( void );
	void						doCurrentConnectionsList( void );
	void						doPossibleRelayList( void );
	void						attemptRelayServiceInContactList( ContactList& contactList  );
	void						attemptRelayServiceInVectorList( ContactList& contactList  );
	void						doBusyList( void );
	bool						connectRelayService( ConnectRequest& connectRequest );
	bool						checkFromGuiList( void );
	void						getMoreRelaysFromAnnounceServers( void );
	bool						decryptHostList( char * content, int contentDataLen, uint16_t clientPort );
	int							buildAnnounceCmd( std::string& strNetCmdHdr, uint16_t clientPort, HostList& anchorList, EHostAction anchorAction );
	int							getAnnounceData( VxSktConnectSimple * netServConn, char * pRxBuf, int bufLen );
	ERelayStatus				requestRelayService( VxGUID& onlineId, VxSktBase * sktBase );

	//=== vars ===//
	P2PConnectList&				m_ConnectionList;
	NetworkMgr&					m_NetworkMgr;
	NetServiceUtils&			m_NetServiceUtils;

	ContactList&				m_PreferredRelayList;
	ContactList&				m_PossibleRelayList;
	std::vector<ConnectRequest>	m_FromGuiRelayList;
	std::vector<ConnectRequest*> m_BusyRelayList;

	VxThread					m_RelaySearchThread;
	VxMutex						m_RelaySearchMutex;
	VxSemaphore					m_WaitForPktRelayServiceSemaphore;

	VxGUID						m_RelayRequestOnlineId;
	ERelayStatus				m_eRelayRequestStatus;
	bool						m_SearchThreadComplete;
	bool						m_HaveRelayService;
	DirectConnectTester&		m_DirectConnectTester;
};

