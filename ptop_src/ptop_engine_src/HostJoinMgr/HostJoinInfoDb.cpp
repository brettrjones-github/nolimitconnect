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

#include "HostJoinInfoDb.h"
#include "HostJoinInfo.h"

#include <ptop_src/ptop_engine_src/P2PEngine/P2PEngine.h>

#include <CoreLib/VxPtopUrl.h>

namespace
{
    std::string 		TABLE_USER_HOST	 				= "tblHostJoins";

    std::string 		CREATE_COLUMNS_USER_HOST		= " (onlineId TEXT, thumbId TEXT, infoModMs BIGINT, pluginType INTEGER, friendState INTEGER, joinState INTEGER, lastConnMs BIGINT, lastJoinMs BIGINT, hostFlags INTEGER, userUrl TEXT) ";

    const int			COLUMN_ONLINE_ID			    = 0;
    const int			COLUMN_HOST_THUMB_ID			= 1;
    const int			COLUMN_INFO_MOD_MS				= 2;
    const int			COLUMN_PLUGIN_TYPE			    = 3;
    const int			COLUMN_FRIEND_STATE			    = 4;
    const int			COLUMN_JOIN_STATE			    = 5;
    const int			COLUMN_LAST_CONN_MS				= 6;
    const int			COLUMN_LAST_JOIN_MS			    = 7;
    const int			COLUMN_HOST_FLAGS			    = 8;
    const int			COLUMN_USER_URL			        = 9;
}

//============================================================================
HostJoinInfoDb::HostJoinInfoDb( P2PEngine& engine, HostJoinMgr& hostListMgr, const char*dbName  )
    : DbBase( dbName )
    , m_Engine( engine )
    , m_HostJoinMgr( hostListMgr )
{
}

//============================================================================
//! create tables in database 
RCODE HostJoinInfoDb::onCreateTables( int iDbVersion )
{
    lockHostJoinInfoDb();
    std::string strCmd = "CREATE TABLE " + TABLE_USER_HOST + CREATE_COLUMNS_USER_HOST;
    RCODE rc = sqlExec(strCmd);
    unlockHostJoinInfoDb();
    return rc;
}

//============================================================================
// delete tables in database
RCODE HostJoinInfoDb::onDeleteTables( int iOldVersion ) 
{
    lockHostJoinInfoDb();
    std::string strCmd = "DROP TABLE IF EXISTS " + TABLE_USER_HOST;
    RCODE rc = sqlExec(strCmd);
    unlockHostJoinInfoDb();
    return rc;
}

//============================================================================
void HostJoinInfoDb::purgeAllHostJoins( void ) 
{
    lockHostJoinInfoDb();
    std::string strCmd = "DELETE FROM " + TABLE_USER_HOST;
    RCODE rc = sqlExec( strCmd );
    unlockHostJoinInfoDb();
    if( rc )
    {
        LogMsg( LOG_ERROR, "HostJoinInfoDb::purgeAllHostJoins error %d", rc );
    }
    else
    {
        LogMsg( LOG_INFO, "HostJoinInfoDb::purgeAllHostJoins success" );
    }
}

//============================================================================
void HostJoinInfoDb::removeHostJoin( GroupieId& groupieId )
{
    std::string onlineIdStr = groupieId.getGroupieOnlineId().toHexString();
    lockHostJoinInfoDb();
    DbBindList bindList( onlineIdStr.c_str() );
    bindList.add( (int)groupieId.getHostType() );
    sqlExec( "DELETE FROM tblHostJoins WHERE onlineId=? AND pluginType=?", bindList );
    unlockHostJoinInfoDb();
}

//============================================================================
bool HostJoinInfoDb::addHostJoin(   GroupieId&      groupieId,
                                    VxGUID&			thumbId,
                                    uint64_t		infoModTime,
                                    EJoinState      joinState,
                                    uint64_t		lastConnectMs,
                                    uint64_t		lastJoinMs,
                                    EFriendState    friendState,
                                    uint32_t        hostFlags,
                                    std::string     hostUrl
                                   )
{
    // always change is granted to was granted so when loaded the app knows have not rejoined yet
    joinState = joinState == eJoinStateJoinIsGranted ? eJoinStateJoinWasGranted : joinState;
    removeHostJoin( groupieId );

    if( !IsAnnounceHostOrClientHostType( groupieId.getHostType() ) )
    {
        LogMsg( LOG_ERROR, "UserJoinInfoDb::addUserJoin invalid host type %s", DescribeHostType( groupieId.getHostType() ) );
        return false;
    }

    std::string onlineIdStr = groupieId.getGroupieOnlineId().toHexString();
    std::string thumbIdStr = thumbId.toHexString();

    lockHostJoinInfoDb();
    DbBindList bindList( onlineIdStr.c_str() );
    bindList.add( thumbIdStr.c_str() );
    bindList.add( infoModTime );
    bindList.add( (int)groupieId.getHostType() );
    bindList.add( (int)friendState );
    bindList.add( (int)joinState );
    bindList.add( lastConnectMs );
    bindList.add( lastJoinMs ); 
    bindList.add( (int)hostFlags );
    bindList.add( hostUrl.c_str() );
    RCODE rc = sqlExec( "INSERT INTO tblHostJoins (onlineId, thumbId, infoModMs, pluginType, friendState, joinState, lastConnMs, lastJoinMs, hostFlags, userUrl) values(?,?,?,?,?,?,?,?,?,?)",
        bindList );
    unlockHostJoinInfoDb();
    vx_assert( 0 == rc );
    if( rc )
    {
        LogMsg( LOG_ERROR, "UserJoinInfoDb::addUserJoin error %d", rc );
    }

    return ( 0 == rc );
}

//============================================================================
bool HostJoinInfoDb::addHostJoin( HostJoinInfo* hostInfo )
{
    return addHostJoin(	hostInfo->getGroupieId(),
                        hostInfo->BaseInfo::getThumbId(),
                        hostInfo->BaseInfo::getInfoModifiedTime(),  
                        hostInfo->BaseJoinInfo::getJoinState(),		
                        hostInfo->BaseJoinInfo::getLastConnectTime(),	
                        hostInfo->BaseJoinInfo::getLastJoinTime(),
                        hostInfo->getFriendState(),
                        hostInfo->getHostFlags(),
                        hostInfo->getUserUrl()
        );
}

//============================================================================
void HostJoinInfoDb::getAllHostJoins( std::map<GroupieId, HostJoinInfo*>& HostJoinHostJoinList )
{
    lockHostJoinInfoDb();
    DbCursor * cursor = startQuery( "SELECT * FROM tblHostJoins" ); // ORDER BY unique_id DESC  //   don't know why ORDER BY quit working on android.. do in code
    if( NULL != cursor )
    {
        while( cursor->getNextRow() )
        {
            HostJoinInfo * hostInfo = new HostJoinInfo();

            hostInfo->setOnlineId( cursor->getString( COLUMN_ONLINE_ID ) );
            hostInfo->setThumbId( cursor->getString( COLUMN_HOST_THUMB_ID ) );
            hostInfo->setInfoModifiedTime( (uint64_t)cursor->getS64( COLUMN_INFO_MOD_MS ) );
            EHostType hostType =  ( EHostType )cursor->getS32( COLUMN_PLUGIN_TYPE );
            hostInfo->setFriendState( (EFriendState)cursor->getS32( COLUMN_FRIEND_STATE ) );
            hostInfo->setJoinState( (EJoinState)cursor->getS32( COLUMN_JOIN_STATE ) );
            hostInfo->setLastConnectTime( (uint64_t)cursor->getS64( COLUMN_LAST_CONN_MS ) );
            hostInfo->setLastJoinTime(  (uint64_t)cursor->getS64( COLUMN_LAST_JOIN_MS ) ); 
            hostInfo->setHostFlags( (uint32_t)cursor->getS32( COLUMN_HOST_FLAGS ) );
            hostInfo->setUserUrl( cursor->getString( COLUMN_USER_URL ) );

            EPluginType pluginType = HostTypeToHostPlugin( hostType );
            if( !IsAnnounceHostOrClientPluginType( pluginType ) )
            {
                LogMsg( LOG_ERROR, "UserJoinInfoDb::getAllHostJoins invalid host type %s", DescribeHostType( hostType ) );
                continue;
            }

            hostInfo->setPluginType( pluginType );

            VxPtopUrl ptopUrl( hostInfo->getUserUrl() );
            if( ptopUrl.isValid() && hostInfo->getOnlineId().isVxGUIDValid() )
            {
                GroupieId groupieId( ptopUrl.getOnlineId(), m_Engine.getMyOnlineId(), hostInfo->getHostType() );
                hostInfo->setGroupieId( groupieId );
                insertHostJoinInTimeOrder( hostInfo, HostJoinHostJoinList );
            }
            else
            {
                LogMsg( LOG_ERROR, "HostJoinInfoDb::getAllHostJoins invalid id or user url" );
            }
        }

        cursor->close();
    }

    unlockHostJoinInfoDb();
} 

//============================================================================
void HostJoinInfoDb::insertHostJoinInTimeOrder( HostJoinInfo *hostInfo, std::map<GroupieId, HostJoinInfo*>& hostList )
{
    hostList[hostInfo->getGroupieId()] = hostInfo;
    /*
    vx_assert( hostInfo && hostInfo->isUrlValid() );

    if( hostInfo->isDefaultHost() )
    {
        hostList.insert( hostList.begin(), hostInfo );
        return;
    }

    std::vector<HostJoinInfo*>::iterator iter;
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
    */
}

