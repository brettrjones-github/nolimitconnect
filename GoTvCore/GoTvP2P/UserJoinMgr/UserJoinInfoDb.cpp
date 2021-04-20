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

#include "UserJoinInfoDb.h"
#include "UserJoinInfo.h"

namespace
{
    std::string 		TABLE_USER_HOST	 				= "tblUserJoins";

    std::string 		CREATE_COLUMNS_USER_HOST		= " (onlineId TEXT, thumbId TEXT, infoModMs BIGINT, hostType INTEGER, hostFlags INTEGER, hostUrl TEXT, lastConnMs BIGINT, lastJoinMs BIGINT) ";

    const int			COLUMN_ONLINE_ID			    = 0;
    const int			COLUMN_HOST_THUMB_ID			= 1;
    const int			COLUMN_INFO_MOD_MS				= 2;
    const int			COLUMN_HOST_TYPE			    = 3;
    const int			COLUMN_HOST_FLAGS			    = 4;
    const int			COLUMN_HOST_URL			        = 5;
    const int			COLUMN_LAST_CONN_MS				= 6;
    const int			COLUMN_LAST_JOIN_MS			    = 7;
}

//============================================================================
UserJoinInfoDb::UserJoinInfoDb( P2PEngine& engine, UserJoinMgr& hostListMgr, const char *dbName  )
    : m_Engine( engine )
    , DbBase( dbName )
    , m_UserJoinMgr( hostListMgr )
{
}

//============================================================================
//! create tables in database 
RCODE UserJoinInfoDb::onCreateTables( int iDbVersion )
{
    lockUserJoinInfoDb();
    std::string strCmd = "CREATE TABLE " + TABLE_USER_HOST + CREATE_COLUMNS_USER_HOST;
    RCODE rc = sqlExec(strCmd);
    unlockUserJoinInfoDb();
    return rc;
}

//============================================================================
// delete tables in database
RCODE UserJoinInfoDb::onDeleteTables( int iOldVersion ) 
{
    lockUserJoinInfoDb();
    std::string strCmd = "DROP TABLE IF EXISTS " + TABLE_USER_HOST;
    RCODE rc = sqlExec(strCmd);
    unlockUserJoinInfoDb();
    return rc;
}

//============================================================================
void UserJoinInfoDb::purgeAllUserJoins( void ) 
{
    lockUserJoinInfoDb();
    std::string strCmd = "DELETE FROM " + TABLE_USER_HOST;
    RCODE rc = sqlExec( strCmd );
    unlockUserJoinInfoDb();
    if( rc )
    {
        LogMsg( LOG_ERROR, "UserJoinInfoDb::purgeAllUserJoins error %d", rc );
    }
    else
    {
        LogMsg( LOG_INFO, "UserJoinInfoDb::purgeAllUserJoins success" );
    }
}

//============================================================================
void UserJoinInfoDb::removeUserJoin( VxGUID& onlineId, EHostType hostType )
{
    std::string onlineIdStr = onlineId.toHexString();
    DbBindList bindList( onlineIdStr.c_str() );
    bindList.add( (int)hostType );
    sqlExec( "DELETE FROM tblUserJoins WHERE onlineId=? AND hostType=?", bindList );
}

//============================================================================
void UserJoinInfoDb::addUserJoin(   VxGUID&			onlineId, 
                                    VxGUID&			thumbId,
                                    uint64_t		infoModTime,
                                    EHostType       hostType,
                                    uint32_t        hostFlags,
                                    std::string     hostUrl,
                                    uint64_t		lastConnectMs,
                                    uint64_t		lastJoinMs
                                   )
{
    removeUserJoin( onlineId, hostType );

    std::string onlineIdStr = onlineId.toHexString();
    std::string thumbIdStr = thumbId.toHexString();

    DbBindList bindList( onlineIdStr.c_str() );
    bindList.add( thumbIdStr.c_str() );
    bindList.add( infoModTime );
    bindList.add( (int)hostType );
    bindList.add( (int)hostFlags );
    bindList.add( hostUrl.c_str() );
    bindList.add( lastConnectMs );
    bindList.add( lastJoinMs );    

    RCODE rc = sqlExec( "INSERT INTO tblUserJoins (onlineId, thumbId, infoModMs, hostType, hostFlags, hostUrl, lastConnMs, lastJoinMs) values(?,?,?,?,?,?,?,?)",
        bindList );
    vx_assert( 0 == rc );
    if( rc )
    {
        LogMsg( LOG_ERROR, "UserJoinInfoDb::addUserJoin error %d\n", rc );
    }
}

//============================================================================
void UserJoinInfoDb::addUserJoin( UserJoinInfo* hostInfo )
{
    addUserJoin(	hostInfo->getOnlineId(),
                    hostInfo->getThumbId(),
                    hostInfo->getInfoModifiedTime(),                    
                    hostInfo->getHostType(),
                    hostInfo->getHostFlags(),			
                    hostInfo->getHostUrl(),
                    hostInfo->getLastConnectTime(),	
                    hostInfo->getLastJoinTime()	
                    );
}

//============================================================================
void UserJoinInfoDb::getAllUserJoins( std::vector<UserJoinInfo*>& UserJoinUserJoinList )
{
    lockUserJoinInfoDb();
    DbCursor * cursor = startQuery( "SELECT * FROM tblUserJoins" ); // ORDER BY unique_id DESC  // BRJ don't know why ORDER BY quit working on android.. do in code
    if( NULL != cursor )
    {
        while( cursor->getNextRow() )
        {
            UserJoinInfo * hostInfo = new UserJoinInfo();
            const int			COLUMN_ONLINE_ID			    = 0;
            const int			COLUMN_HOST_THUMB_ID			= 1;
            const int			COLUMN_INFO_MOD_MS				= 2;
            const int			COLUMN_HOST_TYPE			    = 3;
            const int			COLUMN_HOST_FLAGS			    = 4;
            const int			COLUMN_HOST_URL			        = 5;
            const int			COLUMN_LAST_CONN_MS				= 6;
            const int			COLUMN_LAST_JOIN_MS			    = 7;

            hostInfo->setOnlineId( cursor->getString( COLUMN_ONLINE_ID ) );
            hostInfo->setThumbId( cursor->getString( COLUMN_HOST_THUMB_ID ) );
            hostInfo->setInfoModifiedTime( (uint64_t)cursor->getS64( COLUMN_INFO_MOD_MS ) );
            hostInfo->setHostType( (EHostType)cursor->getS32( COLUMN_HOST_TYPE ) );
            hostInfo->setHostFlags( (uint32_t)cursor->getS32( COLUMN_HOST_FLAGS ) );
            hostInfo->setHostUrl( cursor->getString( COLUMN_HOST_URL ) );
            
            hostInfo->setLastConnectTime( (uint64_t)cursor->getS64( COLUMN_LAST_CONN_MS ) );
            hostInfo->setLastJoinTime(  (uint64_t)cursor->getS64( COLUMN_LAST_JOIN_MS ) ); 

            vx_assert( hostInfo->isValid() );

            insertUserJoinInTimeOrder( hostInfo, UserJoinUserJoinList );
        }

        cursor->close();
    }

    unlockUserJoinInfoDb();
} 

//============================================================================
void UserJoinInfoDb::insertUserJoinInTimeOrder( UserJoinInfo *hostInfo, std::vector<UserJoinInfo*>& hostList )
{
    vx_assert( hostInfo && hostInfo->isValid() );
    if( hostInfo->isDefaultHost() )
    {
        hostList.insert( hostList.begin(), hostInfo );
        return;
    }

    std::vector<UserJoinInfo*>::iterator iter;
    for( iter = hostList.begin(); iter != hostList.end(); ++iter )
    {
        if( ( *iter )->isDefaultHost() )
        {
            // skip defaults so they are at front of list
            continue;
        }
        else if( (*iter)->getLastJoinTime() > hostInfo->getLastJoinTime() )
        {
            hostList.insert( iter, hostInfo );
            return;
        }
    }

    hostList.push_back( hostInfo );
}

