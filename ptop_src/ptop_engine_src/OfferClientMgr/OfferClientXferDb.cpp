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

#include <config_appcorelibs.h>
#include "OfferClientXferDb.h"
#include "OfferClientInfo.h"

namespace
{
	std::string 		TABLE_OFFER_XFER	 			= "asset_xfer";
	std::string 		CREATE_COLUMNS_OFFER_XFER		= " (unique_id TEXT PRIMARY KEY ) ";
}

//============================================================================
OfferClientXferDb::OfferClientXferDb()
: DbBase( "OfferClientXferDb" )
{
}

//============================================================================
OfferClientXferDb::~OfferClientXferDb()
{
}

//============================================================================
//! create tables in database 
RCODE OfferClientXferDb::onCreateTables( int iDbVersion )
{
	lockOfferClientXferDb();
    std::string strCmd = "CREATE TABLE " + TABLE_OFFER_XFER + CREATE_COLUMNS_OFFER_XFER;
    RCODE rc = sqlExec(strCmd);
	unlockOfferClientXferDb();
	return rc;
}

//============================================================================
// delete tables in database
RCODE OfferClientXferDb::onDeleteTables( int iOldVersion ) 
{
	lockOfferClientXferDb();
    std::string strCmd = "DROP TABLE IF EXISTS " + TABLE_OFFER_XFER;
    RCODE rc = sqlExec(strCmd);
	unlockOfferClientXferDb();
	return rc;
}

//============================================================================
void OfferClientXferDb::purgeAllOfferClientXfer( void ) 
{
	lockOfferClientXferDb();
    std::string strCmd = "DELETE FROM " + TABLE_OFFER_XFER;
    RCODE rc = sqlExec( strCmd );
	unlockOfferClientXferDb();
	if( rc )
	{
		LogMsg( LOG_ERROR, "OfferClientXferDb::purgeAllOfferClientXfer error %d\n", rc );
	}
	else
	{
		LogMsg( LOG_INFO, "OfferClientXferDb::purgeAllOfferClientXfer success\n" );
	}
}

//============================================================================
void OfferClientXferDb::removeOfferClient( VxGUID& assetOfferId )
{
	std::string strId;
	assetOfferId.toHexString( strId );
	DbBindList bindList( strId.c_str() );
	sqlExec( "DELETE FROM asset_xfer WHERE unique_id=?", bindList );
}

//============================================================================
void OfferClientXferDb::addOfferClient( VxGUID& assetOfferId )
{
	std::string strId;
	assetOfferId.toHexString( strId );
	DbBindList bindList( strId.c_str() );
	sqlExec( "DELETE FROM asset_xfer WHERE unique_id=?", bindList );

	RCODE rc  = sqlExec( "INSERT INTO asset_xfer (unique_id) values(?)",
		bindList );
	if( rc )
	{
		LogMsg( LOG_ERROR, "OfferClientXferDb::addOfferClient error %d\n", rc );
	}
}

//============================================================================
void OfferClientXferDb::getAllOfferClients( std::vector<VxGUID>& assetList )
{
	assetList.clear();
	std::string strId;
	VxGUID uniqueId;
	lockOfferClientXferDb();
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

	unlockOfferClientXferDb();
}


