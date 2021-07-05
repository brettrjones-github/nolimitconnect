//============================================================================
// Copyright (C) 2021 Brett R. Jones
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
#include "OfferHostXferDb.h"
#include "OfferHostInfo.h"

namespace
{
	std::string 		TABLE_OFFER_XFER	 			= "asset_xfer";
	std::string 		CREATE_COLUMNS_OFFER_XFER		= " (unique_id TEXT PRIMARY KEY ) ";
}

//============================================================================
OfferHostXferDb::OfferHostXferDb( const char * stateDbName )
: DbBase( stateDbName )
{
}

//============================================================================
//! create tables in database 
RCODE OfferHostXferDb::onCreateTables( int iDbVersion )
{
	lockOfferHostXferDb();
    std::string strCmd = "CREATE TABLE " + TABLE_OFFER_XFER + CREATE_COLUMNS_OFFER_XFER;
    RCODE rc = sqlExec(strCmd);
	unlockOfferHostXferDb();
	return rc;
}

//============================================================================
// delete tables in database
RCODE OfferHostXferDb::onDeleteTables( int iOldVersion ) 
{
	lockOfferHostXferDb();
    std::string strCmd = "DROP TABLE IF EXISTS " + TABLE_OFFER_XFER;
    RCODE rc = sqlExec(strCmd);
	unlockOfferHostXferDb();
	return rc;
}

//============================================================================
void OfferHostXferDb::purgeAllOfferHostXfer( void ) 
{
	lockOfferHostXferDb();
    std::string strCmd = "DELETE FROM " + TABLE_OFFER_XFER;
    RCODE rc = sqlExec( strCmd );
	unlockOfferHostXferDb();
	if( rc )
	{
		LogMsg( LOG_ERROR, "OfferHostXferDb::purgeAllOfferHostXfer error %d\n", rc );
	}
	else
	{
		LogMsg( LOG_INFO, "OfferHostXferDb::purgeAllOfferHostXfer success\n" );
	}
}

//============================================================================
void OfferHostXferDb::removeOfferHost( VxGUID& assetOfferId )
{
	std::string strId;
	assetOfferId.toHexString( strId );
	DbBindList bindList( strId.c_str() );
	sqlExec( "DELETE FROM asset_xfer WHERE unique_id=?", bindList );
}

//============================================================================
void OfferHostXferDb::addOfferHost( VxGUID& assetOfferId )
{
	std::string strId;
	assetOfferId.toHexString( strId );
	DbBindList bindList( strId.c_str() );
	sqlExec( "DELETE FROM asset_xfer WHERE unique_id=?", bindList );

	RCODE rc  = sqlExec( "INSERT INTO asset_xfer (unique_id) values(?)",
		bindList );
	if( rc )
	{
		LogMsg( LOG_ERROR, "OfferHostXferDb::addOfferHost error %d\n", rc );
	}
}

//============================================================================
void OfferHostXferDb::getAllOfferHosts( std::vector<VxGUID>& assetList )
{
	assetList.clear();
	std::string strId;
	VxGUID uniqueId;
	lockOfferHostXferDb();
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

	unlockOfferHostXferDb();
}


