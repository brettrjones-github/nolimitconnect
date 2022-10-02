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

#include "ConnectInfoDb.h"
#include "ConnectInfo.h"

namespace
{
    std::string 		TABLE_ASSETS	 				= "tblConnects";

    std::string 		CREATE_COLUMNS_ASSETS			= " (hostid TEXT PRIMARY KEY, hostType INTEGER, hostModMs BIGINT, hostFlags INTEGER, thumbId TEXT, thumbModMs BIGINT, offerId TEXT, offerState INTEGER, offerModMs BIGINT, hostUrl TEXT) ";

    const int			COLUMN_HOST_ID			        = 0;
    const int			COLUMN_HOST_TYPE			    = 1;
    const int			COLUMN_HOST_MOD_MS				= 2;
    const int			COLUMN_HOST_FLAGS			    = 3;
    const int			COLUMN_HOST_THUMB_ID			= 4;
    const int			COLUMN_THUMB_MOD_MS				= 5;
    const int			COLUMN_OFFER_ID				    = 6;
    const int			COLUMN_OFFER_STATE			    = 7;
    const int			COLUMN_OFFER_MOD_MS			    = 8;
    const int			COLUMN_HOST_URL			        = 9;
}

//============================================================================
ConnectInfoDb::ConnectInfoDb( P2PEngine& engine, ConnectMgr& hostListMgr, const char*dbName  )
    : m_Engine( engine )
    , DbBase( dbName )
    , m_ConnectMgr( hostListMgr )
{
}

//============================================================================
//! create tables in database 
RCODE ConnectInfoDb::onCreateTables( int iDbVersion )
{
    lockConnectInfoDb();
    std::string strCmd = "CREATE TABLE " + TABLE_ASSETS + CREATE_COLUMNS_ASSETS;
    RCODE rc = sqlExec(strCmd);
    unlockConnectInfoDb();
    return rc;
}

//============================================================================
// delete tables in database
RCODE ConnectInfoDb::onDeleteTables( int iOldVersion ) 
{
    lockConnectInfoDb();
    std::string strCmd = "DROP TABLE IF EXISTS " + TABLE_ASSETS;
    RCODE rc = sqlExec(strCmd);
    unlockConnectInfoDb();
    return rc;
}

//============================================================================
void ConnectInfoDb::purgeAllConnects( void ) 
{
    lockConnectInfoDb();
    std::string strCmd = "DELETE FROM " + TABLE_ASSETS;
    RCODE rc = sqlExec( strCmd );
    unlockConnectInfoDb();
    if( rc )
    {
        LogMsg( LOG_ERROR, "ConnectInfoDb::purgeAllConnects error %d", rc );
    }
    else
    {
        LogMsg( LOG_INFO, "ConnectInfoDb::purgeAllConnects success" );
    }
}

//============================================================================
void ConnectInfoDb::removeConnect( VxGUID& assetId )
{
    std::string assetStr = assetId.toHexString();
    DbBindList bindList( assetStr.c_str() );
    sqlExec( "DELETE FROM tblConnects WHERE unique_id=?", bindList );
}

//============================================================================
void ConnectInfoDb::addConnect(   EHostType       hostType,
                                    VxGUID&			hostOnlineId,  
                                    uint64_t		hostModTime,
                                    uint32_t        hostFlags,
                                    VxGUID&			thumbId,
                                    uint64_t		thumbModTime,
                                    VxGUID&			offerId,
                                    EOfferState		offerState,
                                    uint64_t		offerModTime,
                                    std::string     hostUrl
)
{
    removeConnect( hostOnlineId );

    std::string hostIdStr = hostOnlineId.toHexString();
    std::string thumbIdStr = thumbId.toHexString();
    std::string offerIdStr = offerId.toHexString();

    DbBindList bindList( hostIdStr.c_str() );
    bindList.add( (int)hostType );
    bindList.add( hostModTime );
    bindList.add( (int)hostFlags );
    bindList.add( thumbIdStr.c_str() );
    bindList.add( thumbModTime );
    bindList.add( offerIdStr.c_str() );
    bindList.add( (int)offerState );
    bindList.add( offerModTime );
    bindList.add( hostUrl.c_str() );

    RCODE rc = sqlExec( "INSERT INTO tblConnects (hostId,hostType, hostModMs, hostFlags, thumbId, thumbModMs, offerId, offerState, offerModMs, hostUrl) values(?,?,?,?,?,?,?,?,?,?)",
        bindList );
    vx_assert( 0 == rc );
    if( rc )
    {
        LogMsg( LOG_ERROR, "ConnectInfoDb::addConnect error %d\n", rc );
    }
}

//============================================================================
void ConnectInfoDb::addConnect( ConnectInfo* hostInfo )
{
    addConnect(	hostInfo->getHostType(),
        hostInfo->getOnlineId(),
        hostInfo->getInfoModifiedTime(),
        hostInfo->getHostFlags(),
        hostInfo->getThumbId(),
        hostInfo->getInfoModifiedTime(),
        hostInfo->getOfferId(),				
        hostInfo->getOfferState(),
        hostInfo->getOfferModifiedTime(),
        hostInfo->getHostUrl().getUrl()
    );
}

//============================================================================
void ConnectInfoDb::getAllConnects( std::vector<ConnectInfo*>& ConnectConnectList )
{
    lockConnectInfoDb();
    DbCursor * cursor = startQuery( "SELECT * FROM tblConnects" ); // ORDER BY unique_id DESC  // BRJ don't know why ORDER BY quit working on android.. do in code
    if( NULL != cursor )
    {
        while( cursor->getNextRow() )
        {
            ConnectInfo * hostInfo = new ConnectInfo();

            hostInfo->setOnlineId( cursor->getString( COLUMN_HOST_ID ) );
            hostInfo->setHostType( (EHostType)cursor->getS32( COLUMN_HOST_TYPE ) );
            hostInfo->setInfoModifiedTime( (uint64_t)cursor->getS64( COLUMN_HOST_MOD_MS ) );
            hostInfo->setHostFlags( (uint32_t)cursor->getS32( COLUMN_HOST_FLAGS ) );

            hostInfo->setThumbId( cursor->getString( COLUMN_HOST_THUMB_ID ) );
            hostInfo->setInfoModifiedTime( (uint64_t)cursor->getS64( COLUMN_THUMB_MOD_MS ) );

            hostInfo->setOfferId( cursor->getString( COLUMN_OFFER_ID ) );
            hostInfo->setOfferState( (EOfferState) cursor->getS32( COLUMN_OFFER_STATE ) );
            hostInfo->setOfferModifiedTime(  (int64_t)cursor->getS64( COLUMN_OFFER_MOD_MS ) );

            hostInfo->setHostUrl( cursor->getString( COLUMN_HOST_URL ) );

            vx_assert( hostInfo->isValid() );

            insertConnectInTimeOrder( hostInfo, ConnectConnectList );
        }

        cursor->close();
    }

    unlockConnectInfoDb();
} 

//============================================================================
void ConnectInfoDb::insertConnectInTimeOrder( ConnectInfo *hostInfo, std::vector<ConnectInfo*>& assetList )
{
    vx_assert( hostInfo->isValid() );

    std::vector<ConnectInfo*>::iterator iter;
    for( iter = assetList.begin(); iter != assetList.end(); ++iter )
    {

        if( ( *iter )->isDefaultHost() )
        {
        }
        else if( (*iter)->getInfoModifiedTime() > hostInfo->getInfoModifiedTime() )
        {
            assetList.insert( iter, hostInfo );
            return;
        }
    }

    assetList.push_back( hostInfo );
}

