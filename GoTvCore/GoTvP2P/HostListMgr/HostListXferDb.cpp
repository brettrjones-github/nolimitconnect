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
// http://www.nolimitconnect.com
//============================================================================

#include <config_gotvcore.h>
#include "HostListXferDb.h"
#include "HostListInfo.h"

namespace
{
	std::string 		TABLE_ASSET_XFER	 			= "asset_xfer";
	std::string 		CREATE_COLUMNS_ASSET_XFER		= " (unique_id TEXT PRIMARY KEY ) ";
}

//============================================================================
HostListXferDb::HostListXferDb()
: DbBase( "HostListXferDb" )
{
}

//============================================================================
HostListXferDb::~HostListXferDb()
{
}

//============================================================================
//! create tables in database 
RCODE HostListXferDb::onCreateTables( int iDbVersion )
{
	lockHostListXferDb();
    std::string strCmd = "CREATE TABLE " + TABLE_ASSET_XFER + CREATE_COLUMNS_ASSET_XFER;
    RCODE rc = sqlExec(strCmd);
	unlockHostListXferDb();
	return rc;
}

//============================================================================
// delete tables in database
RCODE HostListXferDb::onDeleteTables( int iOldVersion ) 
{
	lockHostListXferDb();
    std::string strCmd = "DROP TABLE IF EXISTS " + TABLE_ASSET_XFER;
    RCODE rc = sqlExec(strCmd);
	unlockHostListXferDb();
	return rc;
}

//============================================================================
void HostListXferDb::purgeAllHostListXfer( void ) 
{
	lockHostListXferDb();
    std::string strCmd = "DELETE FROM " + TABLE_ASSET_XFER;
    RCODE rc = sqlExec( strCmd );
	unlockHostListXferDb();
	if( rc )
	{
		LogMsg( LOG_ERROR, "HostListXferDb::purgeAllHostListXfer error %d\n", rc );
	}
	else
	{
		LogMsg( LOG_INFO, "HostListXferDb::purgeAllHostListXfer success\n" );
	}
}

//============================================================================
void HostListXferDb::removeHostList( VxGUID& assetUniqueId )
{
	std::string strId;
	assetUniqueId.toHexString( strId );
	DbBindList bindList( strId.c_str() );
	sqlExec( "DELETE FROM asset_xfer WHERE unique_id=?", bindList );
}

//============================================================================
void HostListXferDb::addHostList( VxGUID& assetUniqueId )
{
	std::string strId;
	assetUniqueId.toHexString( strId );
	DbBindList bindList( strId.c_str() );
	sqlExec( "DELETE FROM asset_xfer WHERE unique_id=?", bindList );

	RCODE rc  = sqlExec( "INSERT INTO asset_xfer (unique_id) values(?)",
		bindList );
	if( rc )
	{
		LogMsg( LOG_ERROR, "HostListXferDb::addHostList error %d\n", rc );
	}
}

//============================================================================
void HostListXferDb::getAllHostLists( std::vector<VxGUID>& assetList )
{
	assetList.clear();
	std::string strId;
	VxGUID uniqueId;
	lockHostListXferDb();
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

	unlockHostListXferDb();
}


