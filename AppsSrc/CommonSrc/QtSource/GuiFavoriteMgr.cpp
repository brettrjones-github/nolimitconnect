//============================================================================
// Copyright (C) 2022 Brett R. Jones
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

#include <QWidget> // must be declared first or linux Qt 6.2.4 will error in qmetatype.h 2167:23: array subscript value ‘53’ is outside the bounds

#include "GuiFavoriteMgr.h"

namespace
{
	int GUI_FAVORITE_MGR_DBVERSION = 1;
	const char* FAVORITE_DB_KEY = "IsFavoiriteDb";
}

//============================================================================
GuiFavoriteMgr::GuiFavoriteMgr()
: VxSettings( "GuiFavoriteMgrDb" )
{
}

//============================================================================
RCODE GuiFavoriteMgr::startupFavoriteMgr( const char * dbSettingsFile )
{
	return dbStartup( GUI_FAVORITE_MGR_DBVERSION, dbSettingsFile );
}

//============================================================================
void GuiFavoriteMgr::shutdownFavoriteMgr( void )
{
	dbShutdown();
}

//============================================================================
void GuiFavoriteMgr::setIsFavorite( VxGUID& onlineId, bool favorite )
{
	if( onlineId.isVxGUIDValid() )
	{
		m_FavoriteList[ onlineId ] = favorite;
		std::string keyVal = onlineId.toHexString();
		if( favorite )
		{
			setIniValue( FAVORITE_DB_KEY, keyVal.c_str(), favorite );
		}
		else
		{
			removeBoolIniValueFromDb( FAVORITE_DB_KEY, keyVal.c_str() );
		}
	}
}

//============================================================================
bool GuiFavoriteMgr::getIsFavorite( VxGUID& onlineId )
{
	bool isFavorite{ false };
	if( onlineId.isVxGUIDValid() )
	{
		auto favItem = m_FavoriteList.find( onlineId );
		if( favItem != m_FavoriteList.end() )
		{
			return favItem->second;
		}
		else
		{
			std::string keyVal = onlineId.toHexString();
			getIniValue( FAVORITE_DB_KEY, onlineId.toHexString().c_str(), isFavorite, false );
			m_FavoriteList[ onlineId ] = isFavorite;
		}
	}
	
	return isFavorite;
}

//============================================================================
void GuiFavoriteMgr::toggleIsFavorite( VxGUID& onlineId )
{
	setIsFavorite( onlineId, !getIsFavorite( onlineId ) );
}