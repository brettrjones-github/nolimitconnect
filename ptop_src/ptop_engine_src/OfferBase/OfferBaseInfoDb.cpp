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
// http://www.nolimitconnect.org
//============================================================================

#include <config_appcorelibs.h>

#include "OfferBaseInfoDb.h"
#include "OfferBaseXferMgr.h"
#include "OfferBaseInfo.h"

namespace
{
	std::string 		TABLE_ASSETS	 				= "tblOffers";

	std::string 		CREATE_COLUMNS_ASSETS			= " (unique_id TEXT PRIMARY KEY, creatorId TEXT, historyId TEXT, thumbId TEXT, assetName TEXT, length BIGINT, type INTEGER, hashId BLOB, locFlags INTEGER, attribFlags INTEGER, creationTime BIGINT, modifiedTime BIGINT, accessedTime BIGINT, assetTag TEXT, sendState INTEGER, isTemp INTEGER) ";

	const int			COLUMN_OFFER_UNIQUE_ID			= 0;
	const int			COLUMN_OFFER_CREATOR_ID			= 1;
	const int			COLUMN_OFFER_HISTORY_ID			= 2;
    const int			COLUMN_OFFER_THUMB_ID			= 3;
	const int			COLUMN_OFFER_NAME				= 4;
	const int			COLUMN_OFFER_LEN				= 5;
	const int			COLUMN_OFFER_TYPE				= 6;
	const int			COLUMN_OFFER_HASH_ID			= 6;
	const int			COLUMN_LOCATION_FLAGS			= 8;
    const int			COLUMN_ATTRIBUTE_FLAGS			= 9;
    const int			COLUMN_CREATION_TIME			= 10;
    const int			COLUMN_MODIFIED_TIME			= 11;
    const int			COLUMN_ACCESSED_TIME			= 12;
	const int			COLUMN_OFFER_TAG				= 13;
	const int			COLUMN_OFFER_SEND_STATE			= 14;
    const int			COLUMN_IS_TEMPORARY			    = 15;
}

//============================================================================
OfferBaseInfoDb::OfferBaseInfoDb( OfferBaseMgr& hostListMgr, const char*dbName  )
: DbBase( dbName )
, m_OfferMgr( hostListMgr )
{
}

//============================================================================
//! create tables in database 
RCODE OfferBaseInfoDb::onCreateTables( int iDbVersion )
{
	lockOfferInfoDb();
	std::string strCmd = "CREATE TABLE " + TABLE_ASSETS + CREATE_COLUMNS_ASSETS;
	RCODE rc = sqlExec(strCmd);
	unlockOfferInfoDb();
	return rc;
}

//============================================================================
// delete tables in database
RCODE OfferBaseInfoDb::onDeleteTables( int iOldVersion ) 
{
	lockOfferInfoDb();
	std::string strCmd = "DROP TABLE IF EXISTS " + TABLE_ASSETS;
	RCODE rc = sqlExec(strCmd);
	unlockOfferInfoDb();
	return rc;
}

//============================================================================
void OfferBaseInfoDb::purgeAllOffers( void ) 
{
	lockOfferInfoDb();
	std::string strCmd = "DELETE FROM " + TABLE_ASSETS;
	RCODE rc = sqlExec( strCmd );
	unlockOfferInfoDb();
	if( rc )
	{
		LogMsg( LOG_ERROR, "OfferBaseInfoDb::purgeAllOffers error %d", rc );
	}
	else
	{
		LogMsg( LOG_INFO, "OfferBaseInfoDb::purgeAllOffers success" );
	}
}

//============================================================================
void OfferBaseInfoDb::removeOffer( const char* assetName )
{
	DbBindList bindList( assetName );
	sqlExec( "DELETE FROM tblOffers WHERE assetName=?", bindList );
}

//============================================================================
void OfferBaseInfoDb::removeOffer( VxGUID& assetId )
{
	std::string assetStr = assetId.toHexString();
	DbBindList bindList( assetStr.c_str() );
	sqlExec( "DELETE FROM tblOffers WHERE unique_id=?", bindList );
}

//============================================================================
void OfferBaseInfoDb::removeOffer( OfferBaseInfo* assetInfo )
{
	// the bind string is not copied so must be in memory until sql is executed
	//DbBindList bindList( assetInfo->getOfferId().toHexString().c_str() );
	std::string hexId = assetInfo->getOfferId().toHexString();
	DbBindList bindList( hexId.c_str() );
	sqlExec( "DELETE FROM tblOffers WHERE unique_id=?", bindList );
}

//============================================================================
void OfferBaseInfoDb::addOffer( VxGUID&			assetId,
                                VxGUID&			creatorId,
                                VxGUID&			historyId,
                                VxGUID&			thumbId,
                                const char*	assetName,
                                int64_t			assetLen,
                                uint32_t		assetType,
                                VxSha1Hash&		hashId,
                                uint32_t		locationFlags,
                                uint32_t		attibuteFlags,
                                int             isTemp,
                                int64_t			creationTimeStamp,
                                int64_t			modifiedTimeStamp,
                                int64_t			accessedTimeStamp,
                                const char*	assetTag,
                                EOfferSendState sendState )
{
    removeOffer( assetId );

    std::string assetIdStr = assetId.toHexString();
    std::string creatorIdStr = creatorId.toHexString();
    std::string historyIdStr = historyId.toHexString();
    std::string thumbIdStr = thumbId.toHexString();

    DbBindList bindList( assetIdStr.c_str() );
    bindList.add( creatorIdStr.c_str() );
    bindList.add( historyIdStr.c_str() );
    bindList.add( thumbIdStr.c_str() );
    bindList.add( assetName );
    bindList.add( assetLen );
    bindList.add( (int)assetType );
    bindList.add( (void *)hashId.getHashData(), 20 );
    bindList.add( locationFlags );
    bindList.add( attibuteFlags );
    bindList.add( (uint64_t)creationTimeStamp );
    bindList.add( (uint64_t)modifiedTimeStamp );
    bindList.add( (uint64_t)accessedTimeStamp );
    bindList.add( assetTag );
    bindList.add( (int)sendState );

    RCODE rc = sqlExec( "INSERT INTO tblOffers (unique_id,creatorId,historyId,thumbId,assetName,length,type,hashId,locFlags,attribFlags,creationTime,modifiedTime,accessedTime,assetTag,sendState,isTemp) values(?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?)",
        bindList );
    if( rc )
    {
        LogMsg( LOG_ERROR, "OfferBaseInfoDb::addOffer error %d\n", rc );
    }
}

//============================================================================
void OfferBaseInfoDb::updateOfferSendState( VxGUID& assetId, EOfferSendState sendState )
{
	std::string assetIdStr		= assetId.toHexString();
	DbBindList bindList(  (int)sendState );
	bindList.add( assetIdStr.c_str() );
	RCODE rc  = sqlExec( "UPDATE tblOffers SET sendState=? WHERE unique_id=?",
		bindList );
	if( rc )
	{
		LogMsg( LOG_ERROR, "OfferBaseInfoDb::addOffer error %d\n", rc );
	}
}

//============================================================================
void OfferBaseInfoDb::addOffer( OfferBaseInfo* assetInfo )
{
	addOffer(	assetInfo->getOfferId(),
				assetInfo->getCreatorId(),
				assetInfo->getHistoryId(),
                assetInfo->getThumbId(),
				assetInfo->getOfferName().c_str(),
				assetInfo->getOfferLength(),
				(uint32_t)assetInfo->getOfferType(),				
				assetInfo->getOfferHashId(),
				assetInfo->getLocationFlags(),
                (uint32_t)assetInfo->getAttributeFlags(),
                assetInfo->isTemporary(),
                assetInfo->getCreationTime(),
                assetInfo->getModifiedTime(),
                assetInfo->getAccessedTime(),
				assetInfo->getOfferTag().length() ? assetInfo->getOfferTag().c_str() : "",
				assetInfo->getOfferSendState()
                );
}

//============================================================================
void OfferBaseInfoDb::getAllOffers( std::vector<OfferBaseInfo*>& OfferList )
{
	std::string assetName;
	uint16_t assetType;
	uint64_t assetLen;
	std::string destasset;
	std::string consoleId;
	lockOfferInfoDb();
	DbCursor * cursor = startQuery( "SELECT * FROM tblOffers" ); // ORDER BY unique_id DESC  // don't know why ORDER BY quit working on android.. do in code
	if( NULL != cursor )
	{
		while( cursor->getNextRow() )
		{
			assetName = cursor->getString( COLUMN_OFFER_NAME );
			assetLen =  (uint64_t)cursor->getS64( COLUMN_OFFER_LEN );
			assetType = (uint16_t)cursor->getS32( COLUMN_OFFER_TYPE );

			OfferBaseInfo* assetInfo = createOfferInfo( assetName, assetLen, assetType );
			assetInfo->setOfferId( cursor->getString( COLUMN_OFFER_UNIQUE_ID ) );
			assetInfo->setCreatorId( cursor->getString( COLUMN_OFFER_CREATOR_ID ) );
			assetInfo->setHistoryId( cursor->getString( COLUMN_OFFER_HISTORY_ID ) );
            assetInfo->setThumbId( cursor->getString( COLUMN_OFFER_THUMB_ID ) );
			assetInfo->setOfferHashId( (uint8_t *)cursor->getBlob( COLUMN_OFFER_HASH_ID ) );
			assetInfo->setLocationFlags( cursor->getS32( COLUMN_LOCATION_FLAGS ) );
            assetInfo->setAttributeFlags( cursor->getS32( COLUMN_ATTRIBUTE_FLAGS ) );
            assetInfo->setCreationTime(  (int64_t)cursor->getS64( COLUMN_CREATION_TIME ) );
            assetInfo->setModifiedTime(  (int64_t)cursor->getS64( COLUMN_MODIFIED_TIME ) );
            assetInfo->setAccessedTime(  (int64_t)cursor->getS64( COLUMN_ACCESSED_TIME ) );
			assetInfo->setOfferTag( cursor->getString( COLUMN_OFFER_TAG ) );		
			assetInfo->setOfferSendState( ( EOfferSendState )cursor->getS32( COLUMN_OFFER_SEND_STATE ) );

            vx_assert( assetInfo->isValid() );
			
			insertOfferInTimeOrder( assetInfo, OfferList );
		}

		cursor->close();
	}

	unlockOfferInfoDb();
} 

//============================================================================
void OfferBaseInfoDb::insertOfferInTimeOrder( OfferBaseInfo*assetInfo, std::vector<OfferBaseInfo*>& assetList )
{
    vx_assert( assetInfo->isValid() );

	std::vector<OfferBaseInfo*>::iterator iter;
	for( iter = assetList.begin(); iter != assetList.end(); ++iter )
	{
		if( (*iter)->getCreationTime() > assetInfo->getCreationTime() )
		{
			assetList.insert( iter, assetInfo );
			return;
		}
	}

	assetList.push_back( assetInfo );
}

