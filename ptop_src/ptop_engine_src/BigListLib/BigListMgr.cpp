//============================================================================
// Copyright (C) 2003 Brett R. Jones 
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

#include <config_appcorelibs.h>
#include <ptop_src/ptop_engine_src/BigListLib/BigListMgr.h>
#include <ptop_src/ptop_engine_src/BigListLib/BigListInfo.h>
#include <ptop_src/ptop_engine_src/P2PEngine/P2PEngine.h>
#include <ptop_src/ptop_engine_src/Network/NetworkMgr.h>
#include <GuiInterface/INlc.h>

#include <PktLib/PktAnnList.h>
#include <CoreLib/VxGlobals.h>
#include <CoreLib/VxTimeDefs.h>
#include <CoreLib/VxParse.h>

#include <memory.h>
#include <string.h>
#include <string>

#define BIGLIST_VERSION 0x100

#ifdef _MSC_VER
# pragma warning(disable: 4355) //'this' : used in base member initializer list
#endif //_MSC_VER

//============================================================================
BigListMgr::BigListMgr( P2PEngine& engine )
: BigListDb( engine, *this )
{	
}

//============================================================================
BigListMgr::~BigListMgr()
{
	bigListMgrShutdown();
}

//============================================================================
//! startup
RCODE BigListMgr::bigListMgrStartup( const char* pDbFileName )
{
	if( m_BigListMgrInitialized )
	{
		bigListMgrShutdown();
	}

    m_BigListMgrInitialized = true;
	return bigListDbStartup( pDbFileName );
}

//============================================================================
//! shutdown
RCODE BigListMgr::bigListMgrShutdown( void )
{
    m_BigListMgrInitialized = false;
	return bigListDbShutdown();
}

//============================================================================
RCODE BigListMgr::updateBigListDatabase( BigListInfo * poInfo, const char* networkName )
{
	RCODE rc = 0;
	if( 0 == poInfo )
	{
		return rc;
	}

	if( m_Engine.shouldInfoBeInDatabase( poInfo ))
	{
		// insert into database
		rc = dbUpdateBigListInfo( poInfo, networkName );
	}
	else if( poInfo->isInDatabase() )
	{
		rc = dbRemoveBigListInfo( poInfo->getMyOnlineId() );
		poInfo->setIsInDatabase( false );
	}

	return rc;
}

//============================================================================
bool BigListMgr::getFriendships( VxGUID& hisOnlineId,
	EFriendState& retMyFriendshipToHim,
	EFriendState& retHisFriendshipToMe )
{
	retMyFriendshipToHim = eFriendStateAnonymous;
	retHisFriendshipToMe = eFriendStateAnonymous;

	BigListAutoLock bigListAutoLock( *this );
	BigListInfo* poInfo = findBigListInfo( hisOnlineId, true );	// id of friend to look for
	if( poInfo )
	{
		retMyFriendshipToHim = poInfo->getMyFriendshipToHim();
		retHisFriendshipToMe = poInfo->getHisFriendshipToMe();
		return true;
	}

	return false;
}

//============================================================================
bool BigListMgr::getOnlineName( VxGUID& hisOnlineId, std::string& onlineName )
{
	BigListAutoLock bigListAutoLock( *this );
	BigListInfo * poInfo = findBigListInfo( hisOnlineId, true );	// id of friend to look for
	if( poInfo )
	{
		onlineName = poInfo->getOnlineName();
		return true;
	}

	onlineName = "";
	return false;
}

//============================================================================
//! add a or update remote friend.. return true 
EPktAnnUpdateType BigListMgr::updatePktAnn(	PktAnnounce *		poPktAnnIn,				// announcement pkt received
											BigListInfo **		ppoRetInfo )				// return pointer to all we know about this contact
{
	BigListAutoLock bigListAutoLock( *this );
	EPktAnnUpdateType eUpdateType = ePktAnnUpdateTypeContactIsSame;

	BigListInfo * poInfo = findBigListInfo( poPktAnnIn->getMyOnlineId(), true );	// id of friend to look for
	if( poInfo )
	{
		LogMsg( LOG_INFO, "Found BigListInfo for %s isMyRelay %d", poInfo->getOnlineName(),  poInfo->isMyRelay() );
		// set flag in pkt announce so gui can read if is my proxy or not
		poPktAnnIn->setIsMyPreferedRelay( poInfo->isMyRelay() );

		if( poInfo->isIgnored() )
		{
			// ignore this person
			poPktAnnIn->setMyFriendshipToHim( eFriendStateIgnore );
			return ePktAnnUpdateTypeIgnored;
		}

		poPktAnnIn->setLastSessionTimeMs( poInfo->getLastSessionTimeMs() );
		// update permission levels to guest if needed
		bool friendshipChanged = false;
		if( poPktAnnIn->isGuest() && poInfo->isAnonymous() )
		{
			poPktAnnIn->setMyFriendshipToHim( eFriendStateGuest );
			poInfo->makeGuest();
			updateVectorList( eFriendStateGuest, poInfo );
			friendshipChanged = true;
			eUpdateType = ePktAnnUpdateTypeContactChanged;
		}
		else
		{
			poPktAnnIn->setMyFriendshipToHim( poInfo->getMyFriendshipToHim() );
		}

		if( poPktAnnIn->getHisFriendshipToMe() != poInfo->getHisFriendshipToMe() )
		{
			poInfo->setHisFriendshipToMe( poPktAnnIn->getHisFriendshipToMe() );
			friendshipChanged = true;
			eUpdateType = ePktAnnUpdateTypeContactChanged;
		}

		if( friendshipChanged )
		{
			GetPtoPEngine().toGuiContactHisFriendshipChange( poInfo );
		}


        poPktAnnIn->getOnlineName()[ MAX_ONLINE_NAME_LEN - 1 ] = 0;
		if( 0 != strcmp( poPktAnnIn->getOnlineName(), poInfo->getOnlineName() ) )
		{
			memcpy( poInfo->getOnlineName(), poPktAnnIn->getOnlineName(), MAX_ONLINE_NAME_LEN );
			GetPtoPEngine().toGuiContactNameChange( poInfo );
			eUpdateType = ePktAnnUpdateTypeContactChanged;
		}

		poPktAnnIn->getOnlineDescription()[ MAX_ONLINE_DESC_LEN - 1 ] = 0;
		if( 0 != strcmp( poPktAnnIn->getOnlineDescription(), poInfo->getOnlineDescription() ) )
		{
			memcpy( poInfo->getOnlineDescription(), poPktAnnIn->getOnlineDescription(), MAX_ONLINE_DESC_LEN );
			GetPtoPEngine().toGuiContactDescChange( poInfo );
			eUpdateType = ePktAnnUpdateTypeContactChanged;
		}

		if( poPktAnnIn->getSearchFlags() != poInfo->getSearchFlags() )
		{
			poInfo->setSearchFlags( poPktAnnIn->getSearchFlags() );
			GetPtoPEngine().toGuiContactSearchFlagsChange( poInfo );
			eUpdateType = ePktAnnUpdateTypeContactChanged;
		}

		if( 0 != memcmp( poPktAnnIn->getPluginPermissions(), poInfo->getPluginPermissions(), PERMISSION_ARRAY_SIZE ) )
		{
			memcpy( poInfo->getPluginPermissions(), poPktAnnIn->getPluginPermissions(), PERMISSION_ARRAY_SIZE );
			GetPtoPEngine().toGuiPluginPermissionChange( poInfo );
			eUpdateType = ePktAnnUpdateTypeContactChanged;
		}

		bool contactInfoChanged = false;
		if( ( poPktAnnIn->m_DirectConnectId != poInfo->m_DirectConnectId ) ||
			( poPktAnnIn->m_u8RelayFlags != poInfo->m_u8RelayFlags ) )
		{
			contactInfoChanged = true;
			eUpdateType = ePktAnnUpdateTypeContactChanged;
		}

		memcpy( poInfo, poPktAnnIn, sizeof( VxNetIdent ) );
		if( contactInfoChanged )
		{
			GetPtoPEngine().toGuiContactConnectionChange( poInfo );
		}

		if( ePktAnnUpdateTypeContactIsSame != eUpdateType )
		{
			GetPtoPEngine().toGuiContactAnythingChange( poInfo );
		}
	}
	else
	{
		if( true == canAddFriend() )
		{
			// new friend
			poPktAnnIn->setMyFriendshipToHim( eFriendStateAnonymous );
			poInfo = new BigListInfo();
			memcpy( poInfo, poPktAnnIn, sizeof( PktAnnounce ) );
			//LogMsg( LOG_INFO, "BigListMgr::updatePktAnn: new contact %s Hi 0x%llX, Lo 0x%llX\n", poInfo->getOnlineName(), poInfo->getMyOnlineId().getVxGUIDHiPart(), poInfo->getMyOnlineId().getVxGUIDLoPart() );
			poInfo->setIsConnected( true );
			bigInsertInfo( poInfo->getMyOnlineId(), poInfo, true );
			updateBigListDatabase( poInfo, GetPtoPEngine().getNetworkMgr().getNetworkKey() );
			//! notify new contact found
			eUpdateType = ePktAnnUpdateTypeNewContact;
		}
		else
		{
			LogMsg( LOG_INFO, "Could not add %s to BigList\n", poPktAnnIn->getOnlineName() );
			eUpdateType = ePktAnnUpdateTypeIgnored;
		}
	}

	if( ppoRetInfo )
	{
		*ppoRetInfo = poInfo;
	}

	return eUpdateType;
}

//============================================================================
bool BigListMgr::canAddFriend( void )
{
	// first limit size.. remove old if possible
	LimitListSize();
	if( MAX_BIGLIST_ITEMS > m_BigList.size() )
	{
		return true;
	}
	LogMsg( LOG_ERROR, "BigListMgr::canAddFriend false" );
	return false;
}

//============================================================================
//! remove from big list.. also from db if bRemoveStorage = true 
RCODE BigListMgr::removeFriend( PktAnnounce * poPktAnn, bool  bRemoveStorage )
{
	BigListAutoLock bigListAutoLock( *this );
	RCODE rc = 0;
	bigRemoveInfo( poPktAnn->getMyOnlineId(), true );
	if( bRemoveStorage )
	{
		rc = dbRemoveBigListInfo( *((VxGUID *)poPktAnn) );
	}

	return rc;
}

//============================================================================
RCODE BigListMgr::FillAnnList(	PktAnnList * poPktAnnList, 
								int iMaxListLen,
								int64_t s64ContactTimeLimitMs,
								bool bIncludeThisNode )
{
	BigListAutoLock bigListAutoLock( *this );

	RCODE rc;
	BigListInfo * poInfo;
	int iemptyLen = poPktAnnList->emptyLen();

	std::map< VxGUID, BigListInfo *, cmp_vxguid >::iterator oMapIter;
	for( oMapIter = m_BigList.begin(); oMapIter != m_BigList.end(); ++oMapIter )
	{
		delete oMapIter->second;
		poInfo = oMapIter->second;
		if( poInfo->getElapsedMsTcpLastContact() <= s64ContactTimeLimitMs )
		{
			//is a node we can add if fits
			if( ((int)sizeof( VxNetIdent ) + poPktAnnList->getPktLength() ) <= (iMaxListLen + iemptyLen) )
			{
			
				if( ( false == bIncludeThisNode ) 
					&& ( GetPtoPEngine().getMyPktAnnounce().getMyOnlineId() == poInfo->getMyOnlineId() ) )
				{
					// don't include ourself
					continue;
				}
				rc = poPktAnnList->addAnn( poInfo );
				if( rc )
				{
					//list is full
					return -1;
				}
			}
			else
			{
				//filled to limit
				return -1;
			}
		}
	}
	return 0;
}

//============================================================================
void BigListMgr::LimitListSize( void )
{
	return; //NOTE: TODO NEED REWRITTEN>> THIS IS CRAP
	int iCnt = (int)m_BigList.size();
	if( MAX_BIGLIST_ITEMS >= iCnt )
	{
		// list is not to big
		return;
	}

	int iToRemoveCnt = (iCnt - MAX_BIGLIST_ITEMS) + 10; // remove a extra 10 so we don't have to do so often
	int iRemovedCnt = 0;
	BigListInfo * poCurInfo;
	//work the list backwards.. this will tend to put
	//the guests etc at the end of the list
	std::map< VxGUID, BigListInfo *, cmp_vxguid >::iterator oMapIter;
	for( oMapIter = m_BigList.end(); oMapIter != m_BigList.begin(); oMapIter-- )
	{
		poCurInfo = oMapIter->second;
		if( poCurInfo->isSafeToDelete() )
		{
			if( GetGmtTimeMs() - poCurInfo->getElapsedMsAnyContact() > WEEK_OF_MIILISECONDS )
			{
				// has not had contact in week and is not friend etc
				delete poCurInfo;
				m_BigList.erase( oMapIter );
				iRemovedCnt++;
				if( iRemovedCnt >= iToRemoveCnt )
				{
					// we have removed enough
					return;
				}
			}
		}
	}
	// couldn't remove enough.. try again with looser criteria
	for( oMapIter = m_BigList.end(); oMapIter != m_BigList.begin(); oMapIter-- )
	{
		poCurInfo = oMapIter->second;
		if( poCurInfo->isSafeToDelete() )
		{
			if( GetGmtTimeMs() - poCurInfo->getElapsedMsAnyContact() > HOUR_OF_MIILISECONDS * 3 )
			{
				// has not had contact in week and is not friend etc
				delete poCurInfo;
				m_BigList.erase( oMapIter );
				iRemovedCnt++;
				if( iRemovedCnt >= iToRemoveCnt )
				{
					// we have removed enough
					return;
				}
			}
		}
	}
}

//============================================================================
bool BigListMgr::queryIdent( VxGUID& onlineId, VxNetIdent& netIdent )
{
	bool foundIdent = false;
	if( onlineId.isVxGUIDValid() )
	{
		BigListAutoLock bigListAutoLock( *this );
		auto iter = m_BigList.find( onlineId );
		if( iter != m_BigList.end() )
		{
			BigListInfo* bigListInfo = iter->second;
			if( bigListInfo )
			{
				netIdent = *bigListInfo;
				foundIdent = true;
			}
		}
	}

	return foundIdent;
}