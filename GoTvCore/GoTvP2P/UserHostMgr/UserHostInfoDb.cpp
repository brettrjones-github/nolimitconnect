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

#include "UserHostInfoDb.h"
#include "UserHostInfo.h"

namespace
{
    std::string 		TABLE_ASSETS	 				= "tblUserHosts";

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
UserHostInfoDb::UserHostInfoDb( P2PEngine& engine, UserHostMgr& hostListMgr, const char *dbName  )
    : m_Engine( engine )
    , DbBase( dbName )
    , m_UserHostMgr( hostListMgr )
{
}

//============================================================================
//! create tables in database 
RCODE UserHostInfoDb::onCreateTables( int iDbVersion )
{
    lockUserHostInfoDb();
    std::string strCmd = "CREATE TABLE " + TABLE_ASSETS + CREATE_COLUMNS_ASSETS;
    RCODE rc = sqlExec(strCmd);
    unlockUserHostInfoDb();
    return rc;
}

//============================================================================
// delete tables in database
RCODE UserHostInfoDb::onDeleteTables( int iOldVersion ) 
{
    lockUserHostInfoDb();
    std::string strCmd = "DROP TABLE IF EXISTS " + TABLE_ASSETS;
    RCODE rc = sqlExec(strCmd);
    unlockUserHostInfoDb();
    return rc;
}

//============================================================================
void UserHostInfoDb::purgeAllUserHosts( void ) 
{
    lockUserHostInfoDb();
    std::string strCmd = "DELETE FROM " + TABLE_ASSETS;
    RCODE rc = sqlExec( strCmd );
    unlockUserHostInfoDb();
    if( rc )
    {
        LogMsg( LOG_ERROR, "UserHostInfoDb::purgeAllUserHosts error %d", rc );
    }
    else
    {
        LogMsg( LOG_INFO, "UserHostInfoDb::purgeAllUserHosts success" );
    }
}

//============================================================================
void UserHostInfoDb::removeUserHost( VxGUID& assetId )
{
    std::string assetStr = assetId.toHexString();
    DbBindList bindList( assetStr.c_str() );
    sqlExec( "DELETE FROM tblUserHosts WHERE unique_id=?", bindList );
}

//============================================================================
void UserHostInfoDb::addUserHost(   EHostType       hostType,
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
    removeUserHost( hostOnlineId );

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

    RCODE rc = sqlExec( "INSERT INTO tblUserHosts (hostId,hostType, hostModMs, hostFlags, thumbId, thumbModMs, offerId, offerState, offerModMs, hostUrl) values(?,?,?,?,?,?,?,?,?,?)",
        bindList );
    vx_assert( 0 == rc );
    if( rc )
    {
        LogMsg( LOG_ERROR, "UserHostInfoDb::addUserHost error %d\n", rc );
    }
}

//============================================================================
void UserHostInfoDb::addUserHost( UserHostInfo* hostInfo )
{
    addUserHost(	hostInfo->getHostType(),
        hostInfo->getHostId(),
        hostInfo->getHostModifiedTime(),
        hostInfo->getHostFlags(),
        hostInfo->getThumbId(),
        hostInfo->getThumbModifiedTime(),
        hostInfo->getOfferId(),				
        hostInfo->getOfferState(),
        hostInfo->getOfferModifiedTime(),
        hostInfo->getHostUrl()
    );
}

//============================================================================
void UserHostInfoDb::getAllUserHosts( std::vector<UserHostInfo*>& UserHostUserHostList )
{
    lockUserHostInfoDb();
    DbCursor * cursor = startQuery( "SELECT * FROM tblUserHosts" ); // ORDER BY unique_id DESC  // BRJ don't know why ORDER BY quit working on android.. do in code
    if( NULL != cursor )
    {
        while( cursor->getNextRow() )
        {
            UserHostInfo * hostInfo = new UserHostInfo();

            hostInfo->setHostId( cursor->getString( COLUMN_HOST_ID ) );
            hostInfo->setHostType( (EHostType)cursor->getS32( COLUMN_HOST_TYPE ) );
            hostInfo->setHostModifiedTime( (uint64_t)cursor->getS64( COLUMN_HOST_MOD_MS ) );
            hostInfo->setHostFlags( (uint32_t)cursor->getS32( COLUMN_HOST_FLAGS ) );

            hostInfo->setThumbId( cursor->getString( COLUMN_HOST_THUMB_ID ) );
            hostInfo->setThumbModifiedTime( (uint64_t)cursor->getS64( COLUMN_THUMB_MOD_MS ) );

            hostInfo->setOfferId( cursor->getString( COLUMN_OFFER_ID ) );
            hostInfo->setOfferState( (EOfferState) cursor->getS32( COLUMN_OFFER_STATE ) );
            hostInfo->setOfferModifiedTime(  (int64_t)cursor->getS64( COLUMN_OFFER_MOD_MS ) );

            hostInfo->setHostUrl( cursor->getString( COLUMN_HOST_URL ) );

            vx_assert( hostInfo->isValid() );

            insertUserHostInTimeOrder( hostInfo, UserHostUserHostList );
        }

        cursor->close();
    }

    unlockUserHostInfoDb();
} 

//============================================================================
void UserHostInfoDb::insertUserHostInTimeOrder( UserHostInfo *hostInfo, std::vector<UserHostInfo*>& assetList )
{
    vx_assert( hostInfo->isValid() );

    std::vector<UserHostInfo*>::iterator iter;
    for( iter = assetList.begin(); iter != assetList.end(); ++iter )
    {

        if( ( *iter )->isDefaultHost() )
        {
        }
        else if( (*iter)->getHostModifiedTime() > hostInfo->getHostModifiedTime() )
        {
            assetList.insert( iter, hostInfo );
            return;
        }
    }

    assetList.push_back( hostInfo );
}

