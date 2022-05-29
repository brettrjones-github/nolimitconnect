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
#pragma once

#include <CoreLib/DbBase.h>
#include "UserProfile.h"

#include <vector>

#define DATA_HELPER_DB_VERSION 1

class VxNetIdent;
class NetHostSetting;
class VxGUID;

//! implements database for gui
class AccountDb : public DbBase
{
public:
	AccountDb();
	virtual ~AccountDb() = default;

	bool						isValid( void )							{ return m_bIsValid; }
	void						setIsValid( bool bValid )				{ m_bIsValid = bValid; }

	//=== overrides ===//
	//! delete tables from database 
	virtual RCODE				onDeleteTables( int oldVersion );
	//! create tables in database 
	virtual RCODE				onCreateTables( int iDbVersion );

	//=== methods ===//
	//! return true if online id exists in table
	bool						onlineUidExistsInTable( VxGUID& onlineId, std::string & strTableName );
	//=========================================================================
	//=== login ===//
	//! update last login name
	bool						updateLastLogin( std::string& strThingName ) { return updateLastLogin( strThingName.c_str() ); } 
	//! update last login name
	bool						updateLastLogin( const char * pThingName );
	//! get last login name.. return "" if doesn't exist
	std::string					getLastLogin(); 
	//=========================================================================
	//=== accounts ===//
	//! insert new account
	bool						insertAccount( VxNetIdent& userAccount );
	//! update existing account
	bool						updateAccount( VxNetIdent& userAccount );
	//! retrieve account by name
	bool						getAccountByName( const char * name, VxNetIdent& oUserAccount ); 
	//! remove account by name
	bool 						removeAccountByName( const char * name ); 
    //! retrieve all accounts
    bool						getAllAccounts( std::vector<VxNetIdent>& accountList );

    //=========================================================================
    //! get profile data for account
	bool						getUserProfile( VxNetIdent& oUserAccount, UserProfile& oProfile ); 
	//! update friend profile
	bool						updateUserProfile( VxNetIdent& oUserAccount, UserProfile& oProfile ); 

	//=========================================================================
	//=== friends ===//
	//! get list of friend with given friendship
	void						getFriendList( uint8_t u8MyFrienship,  std::vector<VxNetIdent>& aoIdent );
	//! update net info about friend
	bool						updateFriend( VxNetIdent& oIdent );

	//=========================================================================
	//=== host settings ===//
	bool						updateNetHostSetting( NetHostSetting& anchorSetting ); 
	bool						getNetHostSettingByName( const char * name, NetHostSetting& anchorSetting ); 
	bool						getAllNetHostSettings( std::vector<NetHostSetting>& anchorSettingList ); 
	bool 						removeNetHostSettingByName( const char * name ); 

	bool						updateLastNetHostSettingName( const char * name ); 
	std::string 				getLastNetHostSettingName( void ); 
    //=========================================================================

protected:
	//=== vars ===//
	bool						m_bIsValid;
};

