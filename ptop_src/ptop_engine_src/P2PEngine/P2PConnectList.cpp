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
// http://www.nolimitconnect.org
//============================================================================

#include "P2PConnectList.h"
#include "P2PEngine.h"

#include <ptop_src/ptop_engine_src/BigListLib/BigListInfo.h>

#include <NetLib/VxSktBase.h>
#include <PktLib/PktsRelay.h>

#include <CoreLib/VxGlobals.h>
#include <CoreLib/VxGUIDList.h>

#include <memory.h>

#define UDP_NEARBY_TIMEOUT_SECONDS 20
//#define DEBUG_MUTEXES

//============================================================================
bool RcConnectInfo::isConnectionValid( void )
{ 
	return m_SktBase && m_SktBase->isConnected(); 
}

//============================================================================
P2PConnectList::P2PConnectList( P2PEngine& engine )
: m_RelayServiceConnection(0)
, m_Engine( engine )
, m_BigListMgr( engine.getBigListMgr() )
, m_bRequireRelayService(false)
{
}

//============================================================================
void P2PConnectList::fromGuiChangeMyFriendshipToHim(	const VxGUID&	oOnlineId,
														EFriendState	eMyFriendshipToHim,
														EFriendState	eHisFriendshipToMe )
{
	connectListLock();
	RcConnectInfo * poInfo = findConnection( oOnlineId, true );
	if( NULL != poInfo )
	{
		if( poInfo->m_SktBase && poInfo->m_SktBase->isConnected() )
		{
			PktAnnounce pktAnn;
			memcpy( &pktAnn, &m_Engine.getMyPktAnnounce(), sizeof(PktAnnounce) );
			pktAnn.setIsPktAnnReplyRequested( false );
			pktAnn.setIsPktAnnRevConnectRequested( false );
			pktAnn.setIsPktAnnStunRequested( false );

			pktAnn.setMyFriendshipToHim( eMyFriendshipToHim );
			pktAnn.setHisFriendshipToMe( eHisFriendshipToMe );
			
			m_Engine.txSystemPkt( oOnlineId, poInfo->m_SktBase, &pktAnn );
		}
	}

	connectListUnlock();
}

//============================================================================
RcConnectInfo * P2PConnectList::addConnection( VxSktBase* sktBase, BigListInfo * poBigListInfo, bool bNewContact )
{
	if( NULL != poBigListInfo )
	{
		return addConnection( poBigListInfo->getMyOnlineId(), new RcConnectInfo( sktBase, poBigListInfo), bNewContact );
	}

	return NULL;
}

//============================================================================
RcConnectInfo * P2PConnectList::addConnection( const VxGUID& oOnlineId, RcConnectInfo * poInfoIn, bool bNewContact )
{
#ifdef DEBUG_MUTEXES
	LogMsg( LOG_INFO, "P2PConnectList::addConnection: connectListLock()\n" );
#endif // DEBUG_MUTEXES

	poInfoIn->m_BigListInfo->setIsConnected( true );

	connectListLock();
	RcConnectInfo * poInfo = findConnection( oOnlineId, true);
	if( NULL == poInfo )
	{
		// not found so insert
		m_ConnectList.insert(std::make_pair(oOnlineId, poInfoIn));
		poInfo = poInfoIn;
	}
	else
	{
		// already exists


		poInfo->m_BigListInfo = poInfoIn->m_BigListInfo;
		if( poInfo->m_SktBase->isConnected() )
		{
#ifdef DEBUG_CONNECT_LIST
			std::string strOnlineIp = poInfo->m_BigListInfo->getOnlineIpAddress().toStdString();
			LogMsg( LOG_INFO, "P2PConnectList::addConnection: already connected %s %s Hi 0x%llX Lo 0x%llX port %d ip %s my proxy %d\n", 
				m_Engine.knownContactNameFromId( poInfo->m_BigListInfo ),
				poInfo->m_BigListInfo->getOnlineName(),
				poInfo->m_BigListInfo->getMyOnlineId().getVxGUIDHiPart(),
				poInfo->m_BigListInfo->getMyOnlineId().getVxGUIDLoPart(),
				poInfo->m_BigListInfo->getOnlinePort(),
				strOnlineIp.c_str(),
				poInfo->m_BigListInfo->isMyRelay()
				);
#endif // DEBUG_CONNECT_LIST
			VxSktBase *	poNewSkt = poInfoIn->m_SktBase;
			VxSktBase *	poOldSkt = poInfo->m_SktBase;
			if( poNewSkt != poOldSkt )
			{
				poInfo->m_SktBase = poNewSkt;

#ifdef DEBUG_MUTEXES
				LogMsg( LOG_INFO, "P2PConnectList::addConnection: connectListUnlock()\n" );
#endif // DEBUG_MUTEXES

				connectListUnlock();
				m_Engine.replaceConnection( poInfo->m_BigListInfo, poOldSkt, poNewSkt );
				delete poInfoIn;
				return poInfo;
			}
		}

		delete poInfoIn;
	}

#ifdef DEBUG_CONNECT_LIST
    BigListInfo * poBigListInfo = poInfo->m_BigListInfo;
    std::string strOnlineIp = poBigListInfo->getOnlineIpAddress().toStdString();
	LogMsg( LOG_INFO, "P2PConnectList::addConnection: %s %s Hi 0x%llX Lo 0x%llX port %d ip %s my proxy %d\n", 
						m_Engine.knownContactNameFromId( poBigListInfo ),
						poBigListInfo->getOnlineName(),
						poBigListInfo->getMyOnlineId().getVxGUIDHiPart(),
						poBigListInfo->getMyOnlineId().getVxGUIDLoPart(),
						poBigListInfo->getOnlinePort(),
						strOnlineIp.c_str(),
						poBigListInfo->isMyRelay()
						);
#endif // DEBUG_CONNECT_LIST

#ifdef DEBUG_MUTEXES
	LogMsg( LOG_INFO, "P2PConnectList::addConnection: connectListUnlock()\n" );
#endif // DEBUG_MUTEXES
	connectListUnlock();

	m_Engine.onContactConnected( poInfo,  false, bNewContact );
	return poInfo; 
}

//============================================================================
void P2PConnectList::removeConnection( const VxGUID& oOnlineId )
{
	RcConnectInfo * poInfo = NULL;
	std::map<VxGUID, RcConnectInfo *, cmp_vxguid>::iterator oMapIter;

#ifdef DEBUG_MUTEXES
	LogMsg( LOG_INFO, "P2PConnectList::removeConnection: connectListLock()\n" );
#endif // DEBUG_MUTEXES
	connectListLock();
	oMapIter = m_ConnectList.find(oOnlineId);
	if( m_ConnectList.end() != oMapIter )
	{
		poInfo = (*oMapIter).second;

#ifdef DEBUG_CONNECT_LIST
		std::string strId;
		oOnlineId.getOnlineId( strId );
		LogMsg( LOG_INFO, "P2PConnectList::removeConnection: %s id %s name %s\n", 
			m_Engine.knownContactNameFromId( oOnlineId ),
			strId.c_str(), 
			poInfo->m_BigListInfo->getOnlineName() );
#endif // DEBUG_CONNECT_LIST

		m_Engine.onContactDisconnected( poInfo, true );
		m_ConnectList.erase( oMapIter );
		delete poInfo;
	}

#ifdef DEBUG_CONNECT_LIST
	else
	{
		std::string strId;
		oOnlineId.getOnlineId( strId );
		LogMsg( LOG_INFO, "P2PConnectList::removeConnection: CONNECTION NOT FOUND to %s id %s\n", 
			m_Engine.knownContactNameFromId( oOnlineId ),
			strId.c_str() );
	}
#endif // DEBUG_CONNECT_LIST

#ifdef DEBUG_MUTEXES
	LogMsg( LOG_INFO, "P2PConnectList::removeConnection: connectListUnlock()\n" );
#endif // DEBUG_MUTEXES
	connectListUnlock();
}

//============================================================================
RcConnectInfo * P2PConnectList::findConnection( const VxGUID& oOnlineId, bool isLocked )
{
	if( !isLocked )
	{
		connectListLock();
	}

	RcConnectInfo * poInfo = NULL;
	ConnectListIter oMapIter;
	oMapIter = m_ConnectList.find(oOnlineId);
	if( m_ConnectList.end() != oMapIter )
	{
		poInfo = (*oMapIter).second;
	}

	if( !isLocked )
	{
		connectListUnlock();
	}

	return poInfo;
}

//============================================================================
void P2PConnectList::removeSocket( VxSktBase* sktBase, bool isLocked )
{
	if( !sktBase )
	{
		LogMsg( LOG_ERROR, "P2PConnectList::removeSocket null skt" );
		return;
	}

	if( !isLocked )
	{
		connectListLock();
	}

	m_Engine.getNetStatusAccum().onConnectionLost( sktBase->getSocketId() );
	for( auto iter = m_ConnectList.begin(); iter != m_ConnectList.end(); )
	{
		RcConnectInfo* connectInfo = iter->second;
		if( connectInfo->m_SktBase == sktBase )
		{
			m_Engine.onContactDisconnected( connectInfo, true );
			delete connectInfo;
			iter = m_ConnectList.erase( iter );
		}
		else
		{
			++iter;
		}
	}

	if( !isLocked )
	{
		connectListUnlock();
	}
}

//============================================================================
void P2PConnectList::onConnectionLost( VxSktBase* sktBase )
{
	removeSocket( sktBase, false );
}        

//============================================================================
void P2PConnectList::connectListLock( void )
{ 
#ifdef DEBUG_MUTEXES
    LogMsg( LOG_INFO, "P2PConnectList::connectListLock m_ConnectListMutex.Lock" );
#endif // DEBUG_MUTEXES
	m_ConnectListMutex.lock(); 
}

//============================================================================
void P2PConnectList::connectListUnlock( void )							
{ 
#ifdef DEBUG_MUTEXES
    LogMsg( LOG_INFO, "P2PConnectList::connectListUnlock m_ConnectListMutex.Unlock\n" );
#endif // DEBUG_MUTEXES
	m_ConnectListMutex.unlock(); 
}

//============================================================================
void P2PConnectList::broadcastSystemPkt( VxPktHdr * pkt, bool onlyIncludeMyContacts )
{
	VxGUIDList guidList;
	broadcastSystemPkt( pkt, guidList, onlyIncludeMyContacts );
}

//============================================================================
void P2PConnectList::broadcastSystemPkt( VxPktHdr * pkt, VxGUIDList& guidList, bool onlyIncludeMyContacts )
{
	pkt->setSrcOnlineId( m_Engine.getMyOnlineId() );
	std::map<VxGUID, RcConnectInfo *, cmp_vxguid>::iterator mapIter;
	RcConnectInfo * connectInfo;
	BigListInfo	* bigListInfo;

	if( onlyIncludeMyContacts 
		&& ( m_ConnectList.size() < 50 ) 
		&& ( PKT_TYPE_IM_ALIVE_REQ == pkt->getPktType() ) )
	{
		// until there are many connections we want to keep connections alive even if not a contact
		// when we start having too many connections then only send pkt I am alive to contacts
		onlyIncludeMyContacts = false;
	}

	connectListLock();
	for( mapIter = m_ConnectList.begin(); mapIter != m_ConnectList.end(); ++mapIter )
	{
		connectInfo = (*mapIter).second;
		bigListInfo = connectInfo->getBigListInfo();
		VxGUID&		destOnlineId = bigListInfo->getMyOnlineId();
		if( guidList.addGuidIfDoesntExist( destOnlineId ) )
		{
			if( onlyIncludeMyContacts && !bigListInfo->isMyRelay() && !bigListInfo->isMyPreferedRelay() )
			{
				if( bigListInfo->getMyFriendshipToHim() < eFriendStateGuest )
				{
					// anonymous or lower
					continue;
				}
			}

			m_Engine.txSystemPkt( destOnlineId, connectInfo->m_SktBase, pkt );
		}
	}

	connectListUnlock();
}


//============================================================================
bool P2PConnectList::isContactConnected( const VxGUID& onlineId )
{
	bool contactIsConnected = false;
#ifdef DEBUG_MUTEXES
	LogMsg( LOG_INFO, "P2PConnectList::isContactConnected connectListLock\n" );
#endif // DEBUG_MUTEXES
	connectListLock();

	ConnectListIter iter;
	iter = m_ConnectList.find( onlineId );
	if( m_ConnectList.end() != iter )
	{
		contactIsConnected = true;	
	}

#ifdef DEBUG_MUTEXES
	LogMsg( LOG_INFO, "P2PConnectList::isContactConnected connectListUnlock\n" );
#endif // DEBUG_MUTEXES
	connectListUnlock();
	return contactIsConnected;
}

//============================================================================
void P2PConnectList::removeContactInfo( VxConnectInfo& contactInfo )
{
	removeConnection( contactInfo.getMyOnlineId() );

#ifdef DEBUG_MUTEXES
	LogMsg( LOG_INFO, "P2PConnectList::removeContactInfo connectListLock\n" );
#endif // DEBUG_MUTEXES
	connectListLock();

	ConnectListIter iter;
	iter = m_ConnectList.find(contactInfo.getMyOnlineId());
	if( m_ConnectList.end() != iter )
	{
		delete (*iter).second;
		m_ConnectList.erase( iter );
	}

#ifdef DEBUG_MUTEXES
	LogMsg( LOG_INFO, "P2PConnectList::removeContactInfo connectListUnlock\n" );
#endif // DEBUG_MUTEXES
	connectListUnlock();
}
