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

#include "HostJoinedLastDb.h"
#include "HostJoinInfo.h"

namespace
{
    std::string 		TABLE_USER_HOST	 				= "tblHostJoinedLast";

    std::string 		CREATE_COLUMNS_JOINED_LAST		= " (onlineId TEXT, pluginType INTEGER, lastJoinMs BIGINT, hostUrl TEXT) ";

    const int			COLUMN_ONLINE_ID			    = 0;
    const int			COLUMN_PLUGIN_TYPE			    = 1;
    const int			COLUMN_LAST_JOIN_MS			    = 2;
    const int			COLUMN_HOST_URL			        = 3;
}

//============================================================================
HostJoinedLastDb::HostJoinedLastDb( P2PEngine& engine, HostJoinMgr& hostListMgr, const char *dbName  )
    : m_Engine( engine )
    , DbBase( dbName )
    , m_HostJoinMgr( hostListMgr )
{
}

//============================================================================
//! create tables in database 
RCODE HostJoinedLastDb::onCreateTables( int iDbVersion )
{
    lockHostJoinedLastDb();
    std::string strCmd = "CREATE TABLE " + TABLE_USER_HOST + CREATE_COLUMNS_JOINED_LAST;
    RCODE rc = sqlExec(strCmd);
    unlockHostJoinedLastDb();
    return rc;
}

//============================================================================
// delete tables in database
RCODE HostJoinedLastDb::onDeleteTables( int iOldVersion ) 
{
    lockHostJoinedLastDb();
    std::string strCmd = "DROP TABLE IF EXISTS " + TABLE_USER_HOST;
    RCODE rc = sqlExec(strCmd);
    unlockHostJoinedLastDb();
    return rc;
}

//============================================================================
void HostJoinedLastDb::purgeAllHJoinedLast( void )
{
    lockHostJoinedLastDb();
    std::string strCmd = "DELETE FROM " + TABLE_USER_HOST;
    RCODE rc = sqlExec( strCmd );
    unlockHostJoinedLastDb();
    if( rc )
    {
        LogMsg( LOG_ERROR, "HostJoinedLastDb::purgeAllHostJoins error %d", rc );
    }
    else
    {
        LogMsg( LOG_INFO, "HostJoinedLastDb::purgeAllHostJoins success" );
    }
}

//============================================================================
void HostJoinedLastDb::removeJoinedLast( EPluginType pluginType )
{
    DbBindList bindList( ( int )pluginType );
    sqlExec( "DELETE FROM tblHostJoins WHERE pluginType=?", bindList );
}

//============================================================================
bool HostJoinedLastDb::setJoinedLast( EPluginType pluginType, VxGUID& onlineId, int64_t lastJoinMs, std::string hostUrl )
{
    m_DbMutex.lock();
    removeJoinedLast( pluginType );

    std::string onlineIdStr = onlineId.toHexString();

    DbBindList bindList( onlineIdStr.c_str() );
    bindList.add( (int)pluginType );
    bindList.add( lastJoinMs ); 
    bindList.add( hostUrl.c_str() );

    RCODE rc = sqlExec( "INSERT INTO tblHostJoinedLast (onlineId, pluginType, lastJoinMs, userUrl) values(?,?,?,?)",
        bindList );
    vx_assert( 0 == rc );
    if( rc )
    {
        LogMsg( LOG_ERROR, "UserJoinedLastDb::addUserJoin error %d", rc );
    }

    m_DbMutex.unlock();
    return ( 0 == rc );
}

//============================================================================
bool HostJoinedLastDb::getJoinedLast( EPluginType pluginType, VxGUID& onlineId, int64_t& lastJoinMs, std::string& hostUrl )
{
    bool result{ false };

    m_DbMutex.lock();
    char queryStatement[512];
    sprintf( queryStatement, "SELECT * FROM tblHostJoinedLast WHERE pluginType=%d", pluginType );

    DbCursor * cursor = startQuery( queryStatement );
    if( cursor )
    {
        if( cursor->getNextRow() )
        {
            onlineId.fromVxGUIDHexString( cursor->getString( COLUMN_ONLINE_ID ) );
            if( pluginType != ( EPluginType )cursor->getS32( COLUMN_PLUGIN_TYPE ) )
            {
                LogMsg( LOG_ERROR, "HostJoinedLastDb::getJoinedLast invalid plugin type" );
            }
            else
            {
                lastJoinMs = cursor->getS64( COLUMN_LAST_JOIN_MS );
                hostUrl = cursor->getString( COLUMN_HOST_URL );
                result = onlineId.isVxGUIDValid() && lastJoinMs && !hostUrl.empty();
                if( !result )
                {
                    LogMsg( LOG_ERROR, "HostJoinedLastDb::getJoinedLast invalid data" );
                }
            }
        }

        cursor->close();
    }

    m_DbMutex.unlock();
    return result;
} 
