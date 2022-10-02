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
// bjones.engineer@gmail.com
// http://www.nolimitconnect.org
//============================================================================

#include <ptop_src/ptop_engine_src/P2PEngine/P2PEngine.h>
#include <GuiInterface/IToGui.h>
#include <ptop_src/ptop_engine_src/Network/NetworkStateMachine.h>
#include <ptop_src/ptop_engine_src/NetServices/NetServicesMgr.h>

#include <ptop_src/ptop_engine_src/BigListLib/BigListInfo.h>
#include <ptop_src/ptop_engine_src/BigListLib/BigList.h>

#include <time.h>
#include <stdio.h>
#include <stdarg.h>

//============================================================================
//! return true if user is viewing this kind of friend
bool P2PEngine::shouldNotifyGui( VxNetIdent* netIdent )
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
			LogMsg( LOG_INFO, "toGuiSendAdministratorList: %s", poInfo->getOnlineName());
			IToGui::getToGui().toGuiContactOnline( poInfo->getVxNetIdent() );
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
			IToGui::getToGui().toGuiContactOnline( poInfo->getVxNetIdent() );
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
				LogMsg( LOG_INFO, "toGuiSendGuestList: %s", poInfo->getOnlineName());
				IToGui::getToGui().toGuiContactOnline( poInfo->getVxNetIdent() );
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
			LogMsg( LOG_INFO, "toGuiSendAnonymousList: %s", poInfo->getOnlineName());
			IToGui::getToGui().toGuiContactOnline( poInfo->getVxNetIdent() );
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
			LogMsg( LOG_INFO, "toGuiSendIgnoreList: %s", poInfo->getOnlineName());
			IToGui::getToGui().toGuiContactOnline( poInfo->getVxNetIdent() );
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

	//int64_t timeNow = GetTimeStampMs();
	//if( 60000 < ( timeNow - m_LastTimeAnnounceFromContactListCalled )  )
	//{
	//	if( m_NetworkStateMachine.isP2POnline() && ( false == getHasHostService( eHostServiceNetworkHost ) ) )
	//	{
	//		m_LastTimeAnnounceFromContactListCalled = timeNow;
	//		m_NetServicesMgr.announceToHost( m_NetworkStateMachine.getHostIp(), m_NetworkStateMachine.getHostPort() );
	//	}
	//}
}

//============================================================================
void P2PEngine::fromGuiRefreshContactList( int maxContactsToSend )
{
 //   int64_t timeNow = GetTimeStampMs();
	//if( 180000 < ( timeNow - m_LastTimeAnnounceFromContactListCalled )  )
	//{
	//	m_LastTimeAnnounceFromContactListCalled = timeNow;
	//	// force reconnect etc
	//	fromGuiNetworkSettingsChanged();
	//}
	//else if( 60000 < ( timeNow - m_LastTimeAnnounceFromContactListCalled )  )
	//{
	//	if( m_NetworkStateMachine.isP2POnline() && ( false == getHasHostService( eHostServiceNetworkHost ) ) )
	//	{
	//		m_LastTimeAnnounceFromContactListCalled = timeNow;
	//		m_NetServicesMgr.announceToHost( m_NetworkStateMachine.getHostIp(), m_NetworkStateMachine.getHostPort() );
	//	}
	//}

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

	default:
		LogMsg( LOG_INFO, "fromGuiSendContactList: unknown view type %d\n", (int)m_eFriendView );
	}


	//LogMsg( LOG_INFO, "fromGuiSendContactList: total sent %d\n", iSentContactsCnt);
}

//============================================================================
void P2PEngine::sendToGuiStatusMessage( const char* statusMsg, ... )
{
	char as8Buf[ 1024 ];
	va_list argList;
	va_start( argList, statusMsg );
	vsnprintf( as8Buf, sizeof( as8Buf ), statusMsg, argList );
	as8Buf[sizeof( as8Buf ) - 1] = 0;
	va_end( argList );
	getToGui().toGuiStatusMessage( as8Buf );
}

//============================================================================
void P2PEngine::toGuiContactNameChange( PktAnnounce * poPktAnn )
{
	if( shouldNotifyGui( poPktAnn ) )
	{
		LogMsg( LOG_INFO, "toGuiContactNameChange:\n");
		IToGui::getToGui().toGuiContactNameChange( (VxNetIdent*)poPktAnn );
	}
}

//============================================================================
//! called when description changes
void P2PEngine::toGuiContactDescChange( PktAnnounce * poPktAnn )
{
	if( shouldNotifyGui( poPktAnn ) )
	{
		LogMsg( LOG_INFO, "toGuiContactDescChange:\n");
		IToGui::getToGui().toGuiContactDescChange( (VxNetIdent*)poPktAnn );
	}
}

//============================================================================
//! called when my friendship to him changes
void P2PEngine::toGuiContactMyFriendshipChange( PktAnnounce * poPktAnn )
{
	if( shouldNotifyGui( poPktAnn ) )
	{
		LogMsg( LOG_INFO, "toGuiContactMyFriendshipChange:\n");
		IToGui::getToGui().toGuiContactMyFriendshipChange( (VxNetIdent*)poPktAnn );
	}
}

//============================================================================
//! called when his friendship to me changes
void P2PEngine::toGuiContactHisFriendshipChange( PktAnnounce * poPktAnn )
{
	if( shouldNotifyGui( poPktAnn ) )
	{
		LogMsg( LOG_INFO, "toGuiContactHisFriendshipChange:\n");
		IToGui::getToGui().toGuiContactHisFriendshipChange( (VxNetIdent*)poPktAnn );
	}
}

//============================================================================
//! called when plugin permission changes
void P2PEngine::toGuiPluginPermissionChange( PktAnnounce * poPktAnn )
{
	if( shouldNotifyGui( poPktAnn ) )
	{
		LogMsg( LOG_INFO, "toGuiPluginPermissionChange:\n");
		IToGui::getToGui().toGuiPluginPermissionChange( (VxNetIdent*)poPktAnn );
	}
}

//============================================================================
//! called when search flags changes
void P2PEngine::toGuiContactSearchFlagsChange( PktAnnounce * poPktAnn )
{
	if( shouldNotifyGui( poPktAnn ) )
	{
		LogMsg( LOG_INFO, "toGuiContactSearchFlagsChange:\n");
		IToGui::getToGui().toGuiContactSearchFlagsChange( (VxNetIdent*)poPktAnn );
	}
}

//============================================================================
//! called when connection info changes
void P2PEngine::toGuiContactConnectionChange( PktAnnounce * poPktAnn )
{
	if( shouldNotifyGui( poPktAnn ) )
	{
		LogMsg( LOG_INFO, "toGuiContactConnectionChange\n");
		IToGui::getToGui().toGuiContactConnectionChange( (VxNetIdent*)poPktAnn );
	}
}

//============================================================================
//! called when any contact info changes ( including any of the above )
void P2PEngine::toGuiContactAnythingChange( PktAnnounce * poPktAnn )
{
	if( shouldNotifyGui( poPktAnn ) )
	{
		LogMsg( LOG_INFO, "toGuiContactAnythingChange\n");
		IToGui::getToGui().toGuiContactAnythingChange( (VxNetIdent*)poPktAnn );
	}
}



