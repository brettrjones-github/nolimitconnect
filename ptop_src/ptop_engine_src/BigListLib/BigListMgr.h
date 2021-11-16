#pragma once
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
// http://www.nolimitconnect.com
//============================================================================

#include <config_gotvcore.h>
#include "BigListDb.h"

class IToGui;
class P2PEngine;
class PktAnnList;
class PktAnnounce;
class VxNetIdent;

enum EPktAnnUpdateType
{
	ePktAnnUpdateTypeIgnored,
	ePktAnnUpdateTypeNewContact,
	ePktAnnUpdateTypeContactIsSame,
	ePktAnnUpdateTypeContactChanged,
};

class P2PEngine;

class BigListMgr : public BigListDb
{
public:
	BigListMgr() = delete;
	BigListMgr( P2PEngine& engine );
	virtual ~BigListMgr() override;

	RCODE						bigListMgrStartup( const char * pDbFileName );
	RCODE						bigListMgrShutdown( void );

	RCODE						updateBigListDatabase( BigListInfo * poInfo, const char * networkName );

	//=== add/remove functions ===//
	//! add a or update remote friend
	EPktAnnUpdateType			updatePktAnn(	PktAnnounce *	poPktAnn, 
												BigListInfo **	ppoRetInfo = 0 );	// return pointer to all we know about this contact

	bool						getFriendships( 	VxGUID&		hisOnlineId,
													EFriendState&	retMyFriendshipToHim,
													EFriendState&	retHisFriendshipToMe );
	//! return true if can add friend to list
	bool						canAddFriend( void );
	//! remove from big list.. also from db if bRemoveStorage = true 
	RCODE						removeFriend( PktAnnounce * poPktAnn, bool  bRemoveStorage = true );

	//helpers
	RCODE						FillAnnList(	PktAnnList * poPktAnnList, 
												int iMaxListLen,
												int64_t s64ContactTimeLimitMs,
												bool bIncludeThisNode = false );

	void						LimitListSize( void );

	bool						queryIdent( VxGUID& onlineId, VxNetIdent& netIdent );

protected:

	//=== vars ===//
    bool                        m_BigListMgrInitialized = false;
};



