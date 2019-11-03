#pragma once
//============================================================================
// Copyright (C) 2013 Brett R. Jones 
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

#include "ContactList.h"

#include <CoreLib/VxGUIDList.h>

#include <map>

class BigListInfo;
class VxSktBase;
class P2PEngine;
class BigListMgr;
class VxPktHdr;

class RcConnectInfo
{
public:
	RcConnectInfo()
		: m_SktBase(0)
		, m_BigListInfo(0)
		, m_bIsRelayClient(0)
		, m_bIsRelayServer(0)
	{
	}

	RcConnectInfo( VxSktBase * sktBase, BigListInfo * poBigListInfo, bool bIsRelayClient = false, bool bIsRelayServer = false  )
		: m_SktBase(sktBase)
		, m_BigListInfo(poBigListInfo)
		, m_bIsRelayClient(bIsRelayClient)
		, m_bIsRelayServer(bIsRelayServer)
	{
	}

	bool						isRelayServer( void )							{ return m_bIsRelayServer; }
	void						setIsRelayServer( bool bIsServer )				{ m_bIsRelayServer = bIsServer; }
	bool						isRelayClient( void )							{ return m_bIsRelayClient; }
	void						setIsRelayClient( bool bIsClient )				{ m_bIsRelayClient = bIsClient; }
	BigListInfo	*				getBigListInfo( void )							{ return m_BigListInfo; }
	VxSktBase	*				getSkt( void )									{ return m_SktBase; }

	VxSktBase *					m_SktBase;
	BigListInfo	*				m_BigListInfo;		
	bool						m_bIsRelayClient;
	bool						m_bIsRelayServer;
};

class P2PConnectList
{
public:
	P2PConnectList( P2PEngine& engine );
	virtual ~P2PConnectList();

	ContactList&				getPreferredRelayList( void )						{ return m_PreferedRelayList; }
	ContactList&				getPossibleRelayList( void )						{ return m_PossibleRelayList; }

	void						connectListLock( void );
	void						connectListUnlock( void );

	void						broadcastSystemPkt( VxPktHdr * pkt, bool onlyIncludeMyContacts );
	void						broadcastSystemPkt( VxPktHdr * pkt, VxGUIDList& retIdsSentPktTo, bool onlyIncludeMyContacts = false );

	void						fromGuiChangeMyFriendshipToHim(	VxGUID&		oOnlineId, 
																EFriendState	eMyFriendshipToHim,
																EFriendState	eHisFriendshipToMe );

	void						onConnectionLost( VxSktBase * sktBase );

	bool						isContactConnected( VxGUID&	onlineId );
	bool						isRelayRequired( void )								{ return m_bRequireRelayService; }
	void						setIsRelayRequired( bool bRequireRelayService )		{ m_bRequireRelayService = bRequireRelayService; }
	bool						isMyRelayAvailable( void )							{ return m_RelayServiceConnection?1:0; }

	virtual void				addRelayServerConnection( RcConnectInfo * poInfo );

	RcConnectInfo *				addConnection( VxSktBase * sktBase, BigListInfo * poBigListInfo, bool bNewContact = false );
	RcConnectInfo *				addConnection( VxGUID& oOnlineId, RcConnectInfo * poInfo, bool bNewContact = false );
	RcConnectInfo *				findConnection( VxGUID& oOnlineId );
	void						removeConnection( VxGUID& oOnlineId );

	void						removeContactInfo( VxConnectInfo& contactInfo );
	void						sendMyTop10( VxSktBase * sktBase, VxGUID& idToExclude );
	void						onPotentialRelayServiceAvailable( RcConnectInfo * poConnection, bool connectionListIsLocked );

	void						cancelRelayService( void );
	
	//=== vars ===//
#ifdef TARGET_OS_ANDROID
	std::map<VxGUID, RcConnectInfo *> m_ConnectList;
	typedef std::map<VxGUID, RcConnectInfo *>::iterator ConnectListIter;
	std::map<VxGUID, RcConnectInfo *>&				getConnectedList( void )		{ return m_ConnectList; }
#else
	std::map<VxGUID, RcConnectInfo *, cmp_vxguid> m_ConnectList;
	typedef std::map<VxGUID, RcConnectInfo *, cmp_vxguid>::iterator ConnectListIter;
	std::map<VxGUID, RcConnectInfo *, cmp_vxguid>&	getConnectedList( void )		{ return m_ConnectList; }
#endif

	RcConnectInfo *				m_RelayServiceConnection;
	std::vector<RcConnectInfo *>m_RelayServerConnectedList;

protected:
	//=== vars ===//
	ContactList					m_PreferedRelayList;
	ContactList					m_PossibleRelayList;
	P2PEngine&					m_Engine;
	BigListMgr&					m_BigListMgr;

	bool						m_bRequireRelayService;

	VxMutex						m_ConnectListMutex;
};
