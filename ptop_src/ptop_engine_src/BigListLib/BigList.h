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

#include <config_appcorelibs.h>

#include <CoreLib/VxGUID.h>
#include <PktLib/VxFriendMatch.h>
#include <CoreLib/VxMutex.h>

#include <map>
#include <vector>

// how many to keep in database
#ifdef TARGET_OS_ANDROID
	#define MAX_FRIENDS_IN_DB		500
	#define MAX_GUESTS_IN_DB		500
	#define MAX_ANONYMOUS_IN_DB		500
	#define MAX_IGNORES_IN_DB		500
#else
	#define MAX_FRIENDS_IN_DB		5000
	#define MAX_GUESTS_IN_DB		5000
	#define MAX_ANONYMOUS_IN_DB		5000
	#define MAX_IGNORES_IN_DB		5000
#endif // TARGET_OS_ANDROID
#define MAX_BIGLIST_ITEMS		(MAX_FRIENDS_IN_DB + MAX_GUESTS_IN_DB + MAX_ANONYMOUS_IN_DB + MAX_IGNORES_IN_DB )

class BigListInfo;
class P2PEngine;

class BigList
{
public:
	BigList() = default;

	//P2PEngine&					getEngine( void )					{ return m_Engine; }

	void						bigListLock( int iInst = 0 );
	void						bigListUnlock( int iInst = 0 );
	
	class BigListAutoLock
	{
	public:
		BigListAutoLock( BigList& bigList ) : m_BigList( bigList ) { m_BigList.bigListLock( 87 ); }
		~BigListAutoLock() { m_BigList.bigListUnlock( 88 ); }
	private:
		BigList&		m_BigList;
	};


	//Note assume caller has already done a mutex lock

	//! find contact
    BigListInfo *				findBigListInfo( const VxGUID& oOnlineId, bool isAlreadyLocked = false );					// id of contact to look for
	//! find contact
	BigListInfo *				findBigListInfo( const char * pUserName, bool isAlreadyLocked = false );				// contact to look for
	//! update which list person is in
	void						updateVectorList( EFriendState oldFriendship, BigListInfo * poInfo );

	//=== vars ===//
#ifdef TARGET_OS_ANDROID
	std::map<VxGUID, BigListInfo *> m_BigList;
	typedef std::map< VxGUID, BigListInfo *, cmp_vxguid >::iterator BigListIter;
#else
	typedef std::map< VxGUID, BigListInfo *, cmp_vxguid >::iterator BigListIter;
	std::map<VxGUID, BigListInfo *, cmp_vxguid> m_BigList;
#endif
	//P2PEngine&					m_Engine;

	std::vector< BigListInfo * >	m_AdministratorList;			// list of administrators
	std::vector< BigListInfo * >	m_FriendList;					// list of friends
	std::vector< BigListInfo * >	m_GuestList;					// list of guests
	std::vector< BigListInfo * >	m_AnonymousList;				// list of anonymous people
	std::vector< BigListInfo * >	m_IgnoreList;					// list of ignored people

	VxMutex						m_BigListMutex;	
	VxMutex						m_AdministratorListMutex;
	VxMutex						m_FriendListMutex;
	VxMutex						m_GuestListMutex;
	VxMutex						m_AnonymousListMutex;
	VxMutex						m_IgnoreListMutex;

protected:
	//! insert contact
	void						bigInsertInfo( VxGUID&, BigListInfo * poInfo, bool isAlreadyLocked = false );	// contact to insert
	//! remove contact
	void						bigRemoveInfo( VxGUID& strOnlineId, bool isAlreadyLocked = false );			// id of contact to remove
	//! remove everybody in list
	void						removeAllInfos( void );
	//! remove from whichever vector list person is in
	void						removeFromVectorList(  EFriendState friendship, BigListInfo *poInfo );
	//! add to whichever vector list person belongs in
	void						addToVectorList( EFriendState friendship, BigListInfo *poInfo );
};
