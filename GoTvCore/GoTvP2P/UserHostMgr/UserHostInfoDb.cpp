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
    std::string 		TABLE_USER_HOST	 				= "tblUserHosts";

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
    std::string strCmd = "CREATE TABLE " + TABLE_USER_HOST + CREATE_COLUMNS_USER_HOST;
    RCODE rc = sqlExec(strCmd);
    unlockUserHostInfoDb();
    return rc;
}

//============================================================================
// delete tables in database
RCODE UserHostInfoDb::onDeleteTables( int iOldVersion ) 
{
    lockUserHostInfoDb();
    std::string strCmd = "DROP TABLE IF EXISTS " + TABLE_USER_HOST;
    RCODE rc = sqlExec(strCmd);
    unlockUserHostInfoDb();
    return rc;
}

//============================================================================
void UserHostInfoDb::purgeAllUserHosts( void ) 
{
    lockUserHostInfoDb();
    std::string strCmd = "DELETE FROM " + TABLE_USER_HOST;
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
void UserHostInfoDb::removeUserHost( VxGUID& onlineId, EHostType hostType )
{
    std::string onlineIdStr = onlineId.toHexString();
    DbBindList bindList( onlineIdStr.c_str() );
    bindList.add( (int)hostType );
    sqlExec( "DELETE FROM tblUserHosts WHERE onlineId=? AND hostType=?", bindList );
}

//============================================================================
void UserHostInfoDb::addUserHost(   VxGUID&			onlineId, 
                                    VxGUID&			thumbId,
                                    uint64_t		infoModTime,
                                    EHostType       hostType,
                                    uint32_t        hostFlags,
                                    std::string     hostUrl,
                                    uint64_t		lastConnectMs,
                                    uint64_t		lastJoinMs
                                   )
{
    removeUserHost( onlineId, hostType );

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

    RCODE rc = sqlExec( "INSERT INTO tblUserHosts (onlineId, thumbId, infoModMs, hostType, hostFlags, hostUrl, lastConnMs, lastJoinMs) values(?,?,?,?,?,?,?,?)",
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
    addUserHost(	hostInfo->getOnlineId(),
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
void UserHostInfoDb::getAllUserHosts( std::vector<UserHostInfo*>& UserHostUserHostList )
{
    lockUserHostInfoDb();
    DbCursor * cursor = startQuery( "SELECT * FROM tblUserHosts" ); // ORDER BY unique_id DESC  // BRJ don't know why ORDER BY quit working on android.. do in code
    if( NULL != cursor )
    {
        while( cursor->getNextRow() )
        {
            UserHostInfo * hostInfo = new UserHostInfo();
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

            insertUserHostInTimeOrder( hostInfo, UserHostUserHostList );
        }

        cursor->close();
    }

    unlockUserHostInfoDb();
} 

//============================================================================
void UserHostInfoDb::insertUserHostInTimeOrder( UserHostInfo *hostInfo, std::vector<UserHostInfo*>& hostList )
{
    vx_assert( hostInfo && hostInfo->isValid() );
    if( hostInfo->isDefaultHost() )
    {
        hostList.insert( hostList.begin(), hostInfo );
        return;
    }

    std::vector<UserHostInfo*>::iterator iter;
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

