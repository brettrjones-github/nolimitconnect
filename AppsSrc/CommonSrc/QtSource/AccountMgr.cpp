//============================================================================
// Copyright (C) 2019 Brett R. Jones
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


#include "AccountMgr.h"

//============================================================================
AccountMgr::AccountMgr()
{
}

//============================================================================
void AccountMgr::startupAccountMgr( const char* dbPathAndName )
{
    m_AccountDb.dbStartup( DATA_HELPER_DB_VERSION, dbPathAndName );
}

//============================================================================
//! update last login name
bool AccountMgr::updateLastLogin( const char* onlineName )
{
    return m_AccountDb.updateLastLogin( onlineName );
}

//============================================================================
bool AccountMgr::insertAccount( VxNetIdent& userAccount )
{
    return m_AccountDb.insertAccount( userAccount );
}

//============================================================================
bool AccountMgr::updateAccount( VxNetIdent& userAccount )
{
    return m_AccountDb.updateAccount( userAccount );
}

//============================================================================
bool AccountMgr::getAccountByName( const char* name, VxNetIdent& userAccount )
{
    return m_AccountDb.getAccountByName( name, userAccount );
}

//============================================================================
bool AccountMgr::removeAccountByName( const char* name )
{
    return m_AccountDb.removeAccountByName( name );
}

//============================================================================
bool AccountMgr::getAllAccounts( std::vector<VxNetIdent>& accountList )
{
    return m_AccountDb.getAllAccounts( accountList );
}

//============================================================================
bool AccountMgr::getUserProfile( VxNetIdent& userAccount, UserProfile& oProfile )
{
    return m_AccountDb.getUserProfile( userAccount, oProfile );
}

//============================================================================
bool AccountMgr::updateUserProfile( VxNetIdent& userAccount, UserProfile& oProfile )
{
    return m_AccountDb.updateUserProfile( userAccount, oProfile );
}

//============================================================================
bool AccountMgr::updateNetHostSetting( NetHostSetting& anchorSetting )
{
    return m_AccountDb.updateNetHostSetting( anchorSetting );
}

//============================================================================
bool AccountMgr::getNetHostSettingByName( const char* name, NetHostSetting& anchorSetting )
{
    return m_AccountDb.getNetHostSettingByName( name, anchorSetting );
}

//============================================================================
std::string AccountMgr::getLastLogin()
{
    return m_AccountDb.getLastLogin();
}

//============================================================================
bool AccountMgr::getAllNetHostSettings( std::vector<NetHostSetting>& anchorSettingList )
{
    return m_AccountDb.getAllNetHostSettings( anchorSettingList );
}

//============================================================================
bool AccountMgr::removeNetHostSettingByName( const char* name )
{
    return m_AccountDb.removeNetHostSettingByName( name );
}

//============================================================================
bool AccountMgr::updateLastNetHostSettingName( const char* name )
{
    return m_AccountDb.updateLastNetHostSettingName( name );
}

//============================================================================
std::string AccountMgr::getLastNetHostSettingName( void )
{
    return m_AccountDb.getLastNetHostSettingName();
}
