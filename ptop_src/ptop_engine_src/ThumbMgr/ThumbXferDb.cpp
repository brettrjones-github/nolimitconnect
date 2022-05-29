//============================================================================
// Copyright (C) 2015 Brett R. Jones
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
#include "ThumbXferDb.h"
#include "ThumbInfo.h"

namespace
{
	std::string 		TABLE_ASSET_XFER	 			= "asset_xfer";
	std::string 		CREATE_COLUMNS_ASSET_XFER		= " (unique_id TEXT PRIMARY KEY ) ";
}

//============================================================================
ThumbXferDb::ThumbXferDb()
: DbBase( "ThumbXferDb" )
{
}

//============================================================================
ThumbXferDb::~ThumbXferDb()
{
}

//============================================================================
//! create tables in database 
RCODE ThumbXferDb::onCreateTables( int iDbVersion )
{
	lockThumbXferDb();
    std::string strCmd = "CREATE TABLE " + TABLE_ASSET_XFER + CREATE_COLUMNS_ASSET_XFER;
    RCODE rc = sqlExec(strCmd);
	unlockThumbXferDb();
	return rc;
}

//============================================================================
// delete tables in database
RCODE ThumbXferDb::onDeleteTables( int iOldVersion ) 
{
	lockThumbXferDb();
    std::string strCmd = "DROP TABLE IF EXISTS " + TABLE_ASSET_XFER;
    RCODE rc = sqlExec(strCmd);
	unlockThumbXferDb();
	return rc;
}

//============================================================================
void ThumbXferDb::purgeAllThumbXfer( void ) 
{
	lockThumbXferDb();
    std::string strCmd = "DELETE FROM " + TABLE_ASSET_XFER;
    RCODE rc = sqlExec( strCmd );
	unlockThumbXferDb();
	if( rc )
	{
		LogMsg( LOG_ERROR, "ThumbXferDb::purgeAllThumbXfer error %d\n", rc );
	}
	else
	{
		LogMsg( LOG_INFO, "ThumbXferDb::purgeAllThumbXfer success\n" );
	}
}

//============================================================================
void ThumbXferDb::removeThumb( VxGUID& assetUniqueId )
{
	std::string strId;
	assetUniqueId.toHexString( strId );
	DbBindList bindList( strId.c_str() );
	sqlExec( "DELETE FROM asset_xfer WHERE unique_id=?", bindList );
}

//============================================================================
void ThumbXferDb::addThumb( VxGUID& assetUniqueId )
{
	std::string strId;
	assetUniqueId.toHexString( strId );
	DbBindList bindList( strId.c_str() );
	sqlExec( "DELETE FROM asset_xfer WHERE unique_id=?", bindList );

	RCODE rc  = sqlExec( "INSERT INTO asset_xfer (unique_id) values(?)",
		bindList );
	if( rc )
	{
		LogMsg( LOG_ERROR, "ThumbXferDb::addThumb error %d\n", rc );
	}
}

//============================================================================
void ThumbXferDb::getAllThumbs( std::vector<VxGUID>& assetList )
{
	assetList.clear();
	std::string strId;
	VxGUID uniqueId;
	lockThumbXferDb();
	DbCursor * cursor = startQuery( "SELECT * FROM asset_xfer" );
	if( NULL != cursor )
	{
		while( cursor->getNextRow() )
		{
			strId = cursor->getString( 0 );
			uniqueId.fromVxGUIDHexString( strId.c_str() );
			assetList.push_back( uniqueId );
		}

		cursor->close();
	}

	unlockThumbXferDb();
}


