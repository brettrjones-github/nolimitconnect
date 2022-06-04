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

#include "IgnoredHostsDb.h"
#include "IgnoredHostInfo.h"
#include "IgnoreListMgr.h"

#include <CoreLib/VxPtopUrl.h>

namespace
{
    std::string 		TABLE_IGNORE_HOST	 			= "tblIgnoreHost";

    std::string 		CREATE_COLUMNS_HOST_IGNORE		= " (onlineId TEXT, thumbId TEXT, hostUrl TEXT, hostTitle TEXT, hostDescription TEXT, timestampdMs BIGINT) ";

    const int			COLUMN_ONLINE_ID			    = 0;
    const int			COLUMN_HOST_THUMB_ID			= 1;
    const int			COLUMN_HOST_URL                 = 2;
    const int			COLUMN_HOST_TITLE               = 3;
    const int			COLUMN_HOST_DESC                = 4;
    const int			COLUMN_TIMESTAMP_MS				= 5;
}

//============================================================================
IgnoredHostsDb::IgnoredHostsDb( P2PEngine& engine, IgnoreListMgr& hostListMgr, const char *dbName  )
    : DbBase( dbName )
    , m_Engine( engine )
    , m_IgnoreListMgr( hostListMgr )
{
}

//============================================================================
//! create tables in database 
RCODE IgnoredHostsDb::onCreateTables( int iDbVersion )
{
    lockDb();
    std::string strCmd = "CREATE TABLE " + TABLE_IGNORE_HOST + CREATE_COLUMNS_HOST_IGNORE;
    RCODE rc = sqlExec(strCmd);
    unlockDb();
    return rc;
}

//============================================================================
// delete tables in database
RCODE IgnoredHostsDb::onDeleteTables( int iOldVersion ) 
{
    lockDb();
    std::string strCmd = "DROP TABLE IF EXISTS " + TABLE_IGNORE_HOST;
    RCODE rc = sqlExec(strCmd);
    unlockDb();
    return rc;
}

//============================================================================
void IgnoredHostsDb::removeFromDatabase( VxGUID& onlineId )
{
    std::string onlineIdStr = onlineId.toHexString();
    DbBindList bindList( onlineIdStr.c_str() );
    sqlExec( "DELETE FROM tblUserJoin WHERE onlineId=?", bindList );
}

//============================================================================
bool IgnoredHostsDb::saveToDatabase( IgnoredHostInfo& hostInfo )
{
    removeFromDatabase( hostInfo.getOnlineId() );

    std::string onlineIdStr = hostInfo.getOnlineId().toHexString();
    std::string thumbIdStr = hostInfo.getThumbId().toHexString();

    DbBindList bindList( onlineIdStr.c_str() );
    bindList.add( thumbIdStr.c_str() );
    bindList.add( hostInfo.getHostUrl().c_str() );
    bindList.add( hostInfo.getHostTitle().c_str() );
    bindList.add( hostInfo.getHostDescription().c_str() );
    bindList.add( hostInfo.getTimestampMs() );
    
   
    RCODE rc = sqlExec( "INSERT INTO tblIgnoreHost (onlineId, thumbId, hostUrl, hostTitle, hostDescription, timestampdMs) values(?,?,?,?,?,?)",
        bindList );
    vx_assert( 0 == rc );
    if( rc )
    {
        LogMsg( LOG_ERROR, "IgnoredHostsDb::saveToDatabase error %d", rc );
    }

    return ( 0 == rc );
}

//============================================================================
bool IgnoredHostsDb::restoreFromDatabase( std::map<VxGUID, IgnoredHostInfo>& ignoredHostList )
{
    lockDb();
    DbCursor * cursor = startQuery( "SELECT * FROM tblIgnoreHost" ); 
    if( NULL != cursor )
    {
        while( cursor->getNextRow() )
        {
            VxGUID onlineId;
            onlineId.fromVxGUIDHexString( cursor->getString( COLUMN_ONLINE_ID ) );
            VxGUID thumbId;
            thumbId.fromVxGUIDHexString( cursor->getString( COLUMN_HOST_THUMB_ID ) );

            IgnoredHostInfo ignoredHostInfo( onlineId, thumbId,
                cursor->getString( COLUMN_HOST_URL ),
                cursor->getString( COLUMN_HOST_TITLE ),
                cursor->getString( COLUMN_HOST_DESC ),
                cursor->getS64( COLUMN_TIMESTAMP_MS ) );

            if( onlineId.isVxGUIDValid() )
            {
                ignoredHostList[ onlineId ] = ignoredHostInfo;
            }
            else
            {
                LogMsg( LOG_ERROR, "IgnoredHostsDb::getAllUserJoins invalid id or host url" );
            }         
        }

        cursor->close();
    }

    unlockDb();
    return ignoredHostList.size();
} 
