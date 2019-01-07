//============================================================================
// Copyright (C) 2009 Brett R. Jones
// Issued to MIT style license by Brett R. Jones in 2017
//
// You may use, copy, modify, merge, publish, distribute, sub-license, and/or sell this software
// provided this Copyright is not modified or removed and is included all copies or substantial portions of the Software
//
// This code is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
//
// brett.jones@engineer.com
// http://www.gotvptop.net
//============================================================================

#include <GoTvCore/GoTvP2P/P2PEngine/P2PEngine.h>
#include <GoTvInterface/IToGui.h>
#include <GoTvCore/GoTvP2P/Network/NetworkStateMachine.h>
#include <GoTvCore/GoTvP2P/NetServices/NetServicesMgr.h>

#include <GoTvCore/GoTvP2P/BigListLib/BigListInfo.h>
#include <GoTvCore/GoTvP2P/BigListLib/BigList.h>

#include <time.h>
#include <stdio.h>
#include <stdarg.h>

//============================================================================
//! return true if user is viewing this kind of friend
bool P2PEngine::shouldNotifyGui( VxNetIdent * netIdent )
{
	EFriendState eFriendship = netIdent->getMyFriendshipToHim();
	switch( m_eFriendView )
	{
	case eFriendViewEverybody:
		//LogMsg(LOG_INFO, "shouldNotifyGui: should show friend %s\n", netIdent->getOnlineName() );
		if( eFriendStateIgnore != eFriendship ) 
		{
			return true;
		}

		break;

	case eFriendViewAdministrators:
		if( eFriendStateAdmin == eFriendship ) 
		{
			return true;
		}

		break;

	case eFriendViewFriendsAndGuests:
		if( ( eFriendStateFriend == eFriendship ) ||
			( eFriendStateGuest == eFriendship ) )
		{
			//LogMsg(LOG_INFO, "shouldNotifyGui: should show friend or guest %s\n", netIdent->getOnlineName() );
			return true;
		}
		break;

	case eFriendViewAnonymous:
		if( eFriendStateAnonymous == eFriendship )
		{
			//LogMsg(LOG_INFO, "shouldNotifyGui: should show anon %s\n", netIdent->getOnlineName() );
			return true;
		}
		break;

	case eFriendViewIgnored:
		if( eFriendStateIgnore == eFriendship )
		{
			//LogMsg(LOG_INFO, "shouldNotifyGui: should show ignored %s\n", netIdent->getOnlineName() );
			return true;
		}
		break;

	case eFriendViewMyProxies:
		if( netIdent->isMyPreferedRelay() )
		{
			//LogMsg(LOG_INFO, "shouldNotifyGui: should show my relays %s\n", netIdent->getOnlineName() );
			return true;
		}
		break;

	case eFriendViewAllProxies:
		if( ( false == netIdent->requiresRelay() ) &&
			( ePluginAccessOk == netIdent->getMyAccessPermissionFromHim( ePluginTypeRelay ) ) )
		{
			//LogMsg(LOG_INFO, "shouldNotifyGui: should show all relays %s\n", netIdent->getOnlineName() );
			return true;
		}
		else
		{
			//LogMsg(LOG_INFO, "shouldNotifyGui: all proxies NO SHOW because requiresRelay %d or cannot access proxy plugin %d\n",
			//	netIdent->requiresRelay(),
			//	netIdent->getMyAccessPermissionFromHim( ePluginTypeRelay ) );
		}
		break;

	default:
		LogMsg(LOG_ERROR, "shouldNotifyGui: UNRECOGNIZED view type\n");
	}

	return false;
}

//============================================================================
int P2PEngine::toGuiSendAdministratorList( int iSentCnt, int iMaxSendCnt )
{
	//LogMsg( LOG_INFO, "toGuiSendAdministratorList: called\n");
	BigListInfo * poInfo = NULL;
	std::vector< BigListInfo * >::iterator iter;
	m_BigListMgr.m_FriendListMutex.lock(111);
	for( iter = m_BigListMgr.m_AdministratorList.begin(); iter != m_BigListMgr.m_AdministratorList.end(); ++iter )
	{
		poInfo = *iter;
		if( shouldNotifyGui( poInfo ) )
		{
			LogMsg( LOG_INFO, "toGuiSendAdministratorList: %s\n", poInfo->getOnlineName());
			m_ToGui.toGuiContactOnline( poInfo );
		}

		iSentCnt++;
		if( iSentCnt >= iMaxSendCnt )
		{
			break;
		}
	}

	m_BigListMgr.m_FriendListMutex.unlock(111);
	return iSentCnt;
}


//============================================================================
int P2PEngine::toGuiSendFriendList( int iSentCnt, int iMaxSendCnt )
{
	//LogMsg( LOG_INFO, "toGuiSendFriendList: called\n");
	BigListInfo * poInfo = NULL;
	std::vector< BigListInfo * >::iterator iter;
	m_BigListMgr.m_FriendListMutex.lock(111);
	for( iter = m_BigListMgr.m_FriendList.begin(); iter != m_BigListMgr.m_FriendList.end(); ++iter )
	{
		poInfo = *iter;
		if( shouldNotifyGui( poInfo ) )
		{
			LogMsg( LOG_INFO, "toGuiSendFriendList: %s\n", poInfo->getOnlineName());
			m_ToGui.toGuiContactOnline( poInfo );
		}

		iSentCnt++;
		if( iSentCnt >= iMaxSendCnt )
		{
			break;
		}
	}
	
	m_BigListMgr.m_FriendListMutex.unlock(111);
	return iSentCnt;
}

//============================================================================
int P2PEngine::toGuiSendGuestList( int iSentCnt, int iMaxSendCnt )
{
	//LogMsg( LOG_INFO, "toGuiSendGuestList: called\n");
	BigListInfo * poInfo = NULL;
	std::vector< BigListInfo * >::iterator iter;
	m_BigListMgr.m_GuestListMutex.lock(112);
	if( iSentCnt < iMaxSendCnt )
	{
		for( iter = m_BigListMgr.m_GuestList.begin(); iter != m_BigListMgr.m_GuestList.end(); ++iter )
		{
			poInfo = *iter;
			if( shouldNotifyGui( poInfo ) )
			{
				LogMsg( LOG_INFO, "toGuiSendGuestList: %s\n", poInfo->getOnlineName());
				m_ToGui.toGuiContactOnline( poInfo );
			}

			iSentCnt++;
			if( iSentCnt >= iMaxSendCnt )
			{
				break;
			}
		}
	}

	m_BigListMgr.m_GuestListMutex.unlock(112);
	return iSentCnt;
}

//============================================================================
int P2PEngine::toGuiSendAnonymousList( int iSentCnt, int iMaxSendCnt )
{
	//LogMsg( LOG_INFO, "toGuiSendAnonymousList: called\n");
	BigListInfo * poInfo = NULL;
	std::vector< BigListInfo * >::iterator iter;

	m_BigListMgr.m_AnonymousListMutex.lock(113);
	for( iter = m_BigListMgr.m_AnonymousList.begin(); iter != m_BigListMgr.m_AnonymousList.end(); ++iter )
	{
		poInfo = *iter;
		if( shouldNotifyGui( poInfo ) )
		{
			LogMsg( LOG_INFO, "toGuiSendAnonymousList: %s\n", poInfo->getOnlineName());
			m_ToGui.toGuiContactOnline( poInfo );
		}

		iSentCnt++;
		if( iSentCnt >= iMaxSendCnt )
		{
			break;
		}
	}

	m_BigListMgr.m_AnonymousListMutex.unlock(113);
	return iSentCnt;
}

//============================================================================
int P2PEngine::toGuiSendIgnoreList( int iSentCnt, int iMaxSendCnt )
{
	//LogMsg( LOG_INFO, "toGuiSendIgnoreList: called\n");
	BigListInfo * poInfo = NULL;
	std::vector< BigListInfo * >::iterator iter;

	m_BigListMgr.m_IgnoreListMutex.lock(114);
	for( iter = m_BigListMgr.m_IgnoreList.begin(); iter != m_BigListMgr.m_IgnoreList.end(); ++iter )
	{
		poInfo = *iter;
		if( shouldNotifyGui( poInfo ) )
		{
			LogMsg( LOG_INFO, "toGuiSendIgnoreList: %s\n", poInfo->getOnlineName());
			m_ToGui.toGuiContactOnline( poInfo );
		}

		iSentCnt++;
		if( iSentCnt >= iMaxSendCnt )
		{
			break;
		}
	}

	m_BigListMgr.m_IgnoreListMutex.unlock(114);
	return iSentCnt;
}

//============================================================================
int P2PEngine::toGuiSendMyProxiesList( int iSentCnt, int iMaxSendCnt )
{
	BigListInfo * poInfo = NULL;
	BigList::BigListIter iter;

	m_BigListMgr.bigListLock(116);
	for( iter = m_BigListMgr.m_BigList.begin(); iter != m_BigListMgr.m_BigList.end(); ++iter )
	{
		poInfo = iter->second;
		if( poInfo->isMyRelay() || poInfo->isMyPreferedRelay() )
		{
			LogMsg( LOG_INFO, "toGuiSendMyProxiesList: %s\n", poInfo->getOnlineName());
			m_ToGui.toGuiContactOnline( poInfo );
		}

		iSentCnt++;
		if( iSentCnt >= iMaxSendCnt )
		{
			break;
		}
	}

	m_BigListMgr.bigListUnlock(116);
	return iSentCnt;
}

//============================================================================
int P2PEngine::toGuiSendAllProxiesList( int iSentCnt, int iMaxSendCnt )
{
	BigListInfo * poInfo = NULL;
	BigList::BigListIter iter;

	m_BigListMgr.bigListLock(116);
	for( iter = m_BigListMgr.m_BigList.begin(); iter != m_BigListMgr.m_BigList.end(); ++iter )
	{
		poInfo = iter->second;
		if( ( false == poInfo->requiresRelay() ) &&
			( ePluginAccessOk == poInfo->getMyAccessPermissionFromHim( ePluginTypeRelay ) ) )
		{
			LogMsg( LOG_INFO, "toGuiSendMyProxiesList: %s\n", poInfo->getOnlineName());
			m_ToGui.toGuiContactOnline( poInfo );
		}

		iSentCnt++;
		if( iSentCnt >= iMaxSendCnt )
		{
			break;
		}
	}

	m_BigListMgr.bigListUnlock(116);
	return iSentCnt;
}

//============================================================================
//! send all friends for view
void P2PEngine::fromGuiSendContactList( EFriendViewType eFriendView, int maxContactsToSend )
{
	//assureUserSpecificDirIsSet( "P2PEngine::fromGuiSendContactList" );
	if( m_eFriendView != eFriendView )
	{
		// if view changed then save to settings
		m_eFriendView = eFriendView;
		getEngineSettings().setWhichContactsToView( eFriendView );
	}

	sendToGuiTheContactList( maxContactsToSend );
	time_t timeNow = time( 0 );
	if( 60 < ( timeNow - m_LastTimeAnnounceFromContactListCalled )  )
	{
		if( m_NetworkStateMachine.isP2POnline() && ( false == m_EngineSettings.getIsThisNodeAnAnchor() ) )
		{
			m_LastTimeAnnounceFromContactListCalled = timeNow;
			m_NetServicesMgr.announceToAnchor( m_NetworkStateMachine.getAnchorIp(), m_NetworkStateMachine.getAnchorPort() );
		}
	}
}

//============================================================================
void P2PEngine::fromGuiRefreshContactList( int maxContactsToSend )
{
	time_t timeNow = time( 0 );
	if( 180 < ( timeNow - m_LastTimeAnnounceFromContactListCalled )  )
	{
		m_LastTimeAnnounceFromContactListCalled = timeNow;
		// force reconnect etc
		fromGuiNetworkSettingsChanged();
	}
	else if( 60 < ( timeNow - m_LastTimeAnnounceFromContactListCalled )  )
	{
		if( m_NetworkStateMachine.isP2POnline() && ( false == m_EngineSettings.getIsThisNodeAnAnchor() ) )
		{
			m_LastTimeAnnounceFromContactListCalled = timeNow;
			m_NetServicesMgr.announceToAnchor( m_NetworkStateMachine.getAnchorIp(), m_NetworkStateMachine.getAnchorPort() );
		}
	}

	sendToGuiTheContactList( maxContactsToSend );
}

//============================================================================
void P2PEngine::sendToGuiTheContactList( int maxContactsToSend )
{
	int iSentContactsCnt = 0;

	//LogMsg( LOG_INFO, "fromGuiSendContactList: announceToWeb view type %d max cnt %d\n",
	//	eFriendView, 
	//	MaxContactsToSend );

	LogMsg( LOG_INFO, "fromGuiSendContactList: view type %d max cnt %d administrators %d friends %d guests %d anon %d ignore %d\n",
			m_eFriendView, 
			maxContactsToSend,
			m_BigListMgr.m_AdministratorList.size(), 
			m_BigListMgr.m_FriendList.size(), 
			m_BigListMgr.m_GuestList.size(),
			m_BigListMgr.m_AnonymousList.size(),
			m_BigListMgr.m_IgnoreList.size() );
	switch( m_eFriendView )
	{
	case eFriendViewEverybody:
		// send friends first then guests
		//LogMsg( LOG_INFO, "fromGuiSendContactList: sending all\n" );
		iSentContactsCnt += toGuiSendAdministratorList( iSentContactsCnt, maxContactsToSend );
		iSentContactsCnt += toGuiSendFriendList( iSentContactsCnt, maxContactsToSend );
		iSentContactsCnt += toGuiSendGuestList( iSentContactsCnt, maxContactsToSend );
		iSentContactsCnt += toGuiSendAnonymousList( iSentContactsCnt, maxContactsToSend );
		iSentContactsCnt += toGuiSendIgnoreList( iSentContactsCnt, maxContactsToSend );
		break;

	case eFriendViewAdministrators:
		// send friends first then guests
		//LogMsg( LOG_INFO, "fromGuiSendContactList: sending administrators\n" );
		iSentContactsCnt += toGuiSendAdministratorList( iSentContactsCnt, maxContactsToSend );
		break;

	case eFriendViewFriendsAndGuests:
		//LogMsg( LOG_INFO, "fromGuiSendContactList: sending friends and guests\n" );
		// send friends first then guests
		iSentContactsCnt += toGuiSendFriendList( iSentContactsCnt, maxContactsToSend );
		iSentContactsCnt += toGuiSendGuestList( iSentContactsCnt, maxContactsToSend );
		break;

	case eFriendViewAnonymous:
		//LogMsg( LOG_INFO, "fromGuiSendContactList: sending anon\n" );
		iSentContactsCnt = toGuiSendAnonymousList( iSentContactsCnt, maxContactsToSend );
		break;

	case eFriendViewIgnored:
		//LogMsg( LOG_INFO, "fromGuiSendContactList: sending ignored\n" );
		iSentContactsCnt = toGuiSendIgnoreList( iSentContactsCnt, maxContactsToSend );
		break;

	case eFriendViewMyProxies:
		//LogMsg( LOG_INFO, "fromGuiSendContactList: sending my proxies\n" );
		iSentContactsCnt = toGuiSendMyProxiesList( iSentContactsCnt, maxContactsToSend );
		break;

	case eFriendViewAllProxies:
		//LogMsg( LOG_INFO, "fromGuiSendContactList: sending all proxies\n" );
		iSentContactsCnt = toGuiSendAllProxiesList( iSentContactsCnt, maxContactsToSend );
		break;

	default:
		LogMsg( LOG_INFO, "fromGuiSendContactList: unknown view type %d\n", (int)m_eFriendView );
	}


	//LogMsg( LOG_INFO, "fromGuiSendContactList: total sent %d\n", iSentContactsCnt);
}

//============================================================================
void P2PEngine::sendToGuiStatusMessage( const char * statusMsg, ... )
{
	char as8Buf[ 1024 ];
	va_list argList;
	va_start( argList, statusMsg );
	vsnprintf( as8Buf, sizeof( as8Buf ), statusMsg, argList );
	as8Buf[sizeof( as8Buf ) - 1] = 0;
	va_end( argList );
	getToGuiInterface().toGuiStatusMessage( as8Buf );
}

//============================================================================
void P2PEngine::toGuiContactNameChange( PktAnnounce * poPktAnn )
{
	if( shouldNotifyGui( poPktAnn ) )
	{
		LogMsg( LOG_INFO, "toGuiContactNameChange:\n");
		m_ToGui.toGuiContactNameChange( (VxNetIdent *)poPktAnn );
	}
}

//============================================================================
//! called when description changes
void P2PEngine::toGuiContactDescChange( PktAnnounce * poPktAnn )
{
	if( shouldNotifyGui( poPktAnn ) )
	{
		LogMsg( LOG_INFO, "toGuiContactDescChange:\n");
		m_ToGui.toGuiContactDescChange( (VxNetIdent *)poPktAnn );
	}
}

//============================================================================
//! called when my friendship to him changes
void P2PEngine::toGuiContactMyFriendshipChange( PktAnnounce * poPktAnn )
{
	if( shouldNotifyGui( poPktAnn ) )
	{
		LogMsg( LOG_INFO, "toGuiContactMyFriendshipChange:\n");
		m_ToGui.toGuiContactMyFriendshipChange( (VxNetIdent *)poPktAnn );
	}
}

//============================================================================
//! called when his friendship to me changes
void P2PEngine::toGuiContactHisFriendshipChange( PktAnnounce * poPktAnn )
{
	if( shouldNotifyGui( poPktAnn ) )
	{
		LogMsg( LOG_INFO, "toGuiContactHisFriendshipChange:\n");
		m_ToGui.toGuiContactHisFriendshipChange( (VxNetIdent *)poPktAnn );
	}
}

//============================================================================
//! called when plugin permission changes
void P2PEngine::toGuiPluginPermissionChange( PktAnnounce * poPktAnn )
{
	if( shouldNotifyGui( poPktAnn ) )
	{
		LogMsg( LOG_INFO, "toGuiPluginPermissionChange:\n");
		m_ToGui.toGuiPluginPermissionChange( (VxNetIdent *)poPktAnn );
	}
}

//============================================================================
//! called when search flags changes
void P2PEngine::toGuiContactSearchFlagsChange( PktAnnounce * poPktAnn )
{
	if( shouldNotifyGui( poPktAnn ) )
	{
		LogMsg( LOG_INFO, "toGuiContactSearchFlagsChange:\n");
		m_ToGui.toGuiContactSearchFlagsChange( (VxNetIdent *)poPktAnn );
	}
}

//============================================================================
//! called when connection info changes
void P2PEngine::toGuiContactConnectionChange( PktAnnounce * poPktAnn )
{
	if( shouldNotifyGui( poPktAnn ) )
	{
		LogMsg( LOG_INFO, "toGuiContactConnectionChange\n");
		m_ToGui.toGuiContactConnectionChange( (VxNetIdent *)poPktAnn );
	}
}

//============================================================================
//! called when any contact info changes ( including any of the above )
void P2PEngine::toGuiContactAnythingChange( PktAnnounce * poPktAnn )
{
	if( shouldNotifyGui( poPktAnn ) )
	{
		LogMsg( LOG_INFO, "toGuiContactAnythingChange\n");
		m_ToGui.toGuiContactAnythingChange( (VxNetIdent *)poPktAnn );
	}
}



