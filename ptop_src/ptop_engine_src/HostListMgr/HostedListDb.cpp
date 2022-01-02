//============================================================================
// Copyright (C) 2019 Brett R. Jones
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

#include "HostedListDb.h"
#include "HostedInfo.h"

#include <CoreLib/VxGlobals.h>
#include <CoreLib/VxParse.h>
#include <CoreLib/sqlite3.h>

#include <memory.h>

namespace
{
	const int			COLUMN_IDX_ONLINE_ID			= 0;
	const int			COLUMN_IDX_HOST_TYPE			= 1;
	const int			COLUMN_IDX_HOST_URL				= 2;
	const int			COLUMN_IDX_HOST_TITLE			= 3;
	const int			COLUMN_IDX_HOST_DESC			= 4;
	const int			COLUMN_IDX_IS_FAVORITE			= 5;
	const int			COLUMN_IDX_CONNECT_TIME			= 6;
	const int			COLUMN_IDX_JOIN_TIME			= 7;
	const int			COLUMN_IDX_HOST_INFO_TIME		= 8;
}

//============================================================================
HostedListDb::HostedListDb()
: DbBase( "HostedListDb" )
{
}

//============================================================================
HostedListDb::~HostedListDb()
{
}

//============================================================================
RCODE HostedListDb::hostedListDbStartup( int dbVersion, const char* dbFileName )
{
	dbShutdown();
	return dbStartup( dbVersion, dbFileName );
}

//============================================================================
RCODE HostedListDb::hostedListDbShutdown( void )
{
	return dbShutdown();
}

//============================================================================
RCODE HostedListDb::onCreateTables( int iDbVersion )
{
	RCODE rc = sqlExec( "CREATE TABLE tblHosted (online_id TEXT, host_type INTEGER, host_url TEXT, host_title TEXT, host_desc TEXT, favorite INTEGER, connect_time BIGINT, join_time BIGINT, info_time BIGINT)" );
	vx_assert( 0 == rc );
	return rc;
}

//============================================================================
RCODE HostedListDb::onDeleteTables( int iOldVersion )
{
	RCODE rc = sqlExec( (char *)"DROP TABLE tblHosted" );
	vx_assert( 0 == rc );
	return rc;
}

//============================================================================
void HostedListDb::getAllHosteds( std::vector<HostedInfo>& hostedList )
{
	lockDb();
	DbCursor* cursor = startQuery( "SELECT * FROM tblHosted" );
	if( NULL != cursor )
	{
		while( cursor->getNextRow() )
		{
			HostedInfo hostInfo;

			hostInfo.getOnlineId().fromVxGUIDHexString( cursor->getString( COLUMN_IDX_ONLINE_ID ) );
			hostInfo.setHostType( (EHostType)cursor->getS32( COLUMN_IDX_HOST_TYPE ) );
			hostInfo.setHostInviteUrl( cursor->getString( COLUMN_IDX_HOST_URL ) );
			hostInfo.setHostTitle( cursor->getString( COLUMN_IDX_HOST_TITLE ) );
			hostInfo.setHostDescription( cursor->getString( COLUMN_IDX_HOST_DESC ) );
			hostInfo.setIsFavorite( cursor->getS32( COLUMN_IDX_IS_FAVORITE ) );
			hostInfo.setConnectedTimestamp( ( int64_t )cursor->getS64( COLUMN_IDX_CONNECT_TIME ) );
			hostInfo.setJoinedTimestamp( ( int64_t )cursor->getS64( COLUMN_IDX_JOIN_TIME ) );
			hostInfo.setHostInfoTimestamp( ( int64_t )cursor->getS64( COLUMN_IDX_HOST_INFO_TIME ) );

			hostedList.push_back( hostInfo );
		}

		cursor->close();
	}

	unlockDb();
}

//============================================================================
void HostedListDb::removeClosedPortIdent( VxGUID& onlineId )
{
	m_DbMutex.lock();
	std::string onlineIdHex = onlineId.toHexString();
	DbBindList bindList( onlineIdHex.c_str() );
	sqlExec( "DELETE FROM tblHosted WHERE online_id=?", bindList );
	m_DbMutex.unlock();
}

//============================================================================
void HostedListDb::removeHostedInfo( EHostType hostType, VxGUID& onlineId )
{
	m_DbMutex.lock();
	std::string onlineIdHex = onlineId.toHexString();
	DbBindList bindList( onlineIdHex.c_str() );
	bindList.add( ( int )hostType );
	sqlExec( "DELETE FROM tblHosted WHERE online_id=? AND host_type=?", bindList );
	m_DbMutex.unlock();
}

//============================================================================
bool HostedListDb::doesHostInfoExist( EHostType hostType, VxGUID& onlineId, std::string& retOnlineHexStr )
{
	if( !onlineId.isVxGUIDValid() || !onlineId.toHexString( retOnlineHexStr ) )
	{
		LogMsg( LOG_ERROR, "ERROR: HostedListDb::doesHostInfoExist INVALID ONLINE ID" );
		return false;
	}

	m_DbMutex.lock(); // make thread safe

	bool doesExists = false;
	DbCursor* cursor = startQuery( "SELECT * FROM tblHosted WHERE online_id=? AND host_type=?", retOnlineHexStr.c_str(), ( int )hostType );
	if( cursor )
	{
		if( cursor->getNextRow() )
		{
			doesExists = true;
		}

		cursor->close();
	}

	m_DbMutex.unlock();
	return doesExists;
}

//============================================================================
bool HostedListDb::updateIsFavorite( EHostType hostType, VxGUID& onlineId, bool isFavorite )
{
	std::string onlineHexStr;
	bool result = doesHostInfoExist( hostType, onlineId, onlineHexStr );
	if( result )
	{
		DbBindList bindList( (int)isFavorite );
		bindList.add( onlineHexStr.c_str() );
		bindList.add( ( int )hostType );

		m_DbMutex.lock();
		RCODE rc = sqlExec( "UPDATE tblHosted SET favorite=? WHERE online_id=? AND host_type=?", bindList );
		m_DbMutex.unlock();
		result = 0 == rc;
	}

	return result;
}

//============================================================================
bool HostedListDb::updateLastConnected( EHostType hostType, VxGUID& onlineId, int64_t lastConnectedTime )
{
	std::string onlineHexStr;
	bool result = doesHostInfoExist( hostType, onlineId, onlineHexStr );
	if( result )
	{
		DbBindList bindList( lastConnectedTime );
		bindList.add( onlineHexStr.c_str() );
		bindList.add( ( int )hostType );

		m_DbMutex.lock();
		RCODE rc = sqlExec( "UPDATE tblHosted SET connect_time=? WHERE online_id=? AND host_type=?", bindList );
		m_DbMutex.unlock();
		result = 0 == rc;
	}

	return result;
}

//============================================================================
bool HostedListDb::updateLastJoined( EHostType hostType, VxGUID& onlineId, int64_t lastJoinedTime )
{
	std::string onlineHexStr;
	bool result = doesHostInfoExist( hostType, onlineId, onlineHexStr );
	if( result )
	{
		DbBindList bindList( lastJoinedTime );
		bindList.add( onlineHexStr.c_str() );
		bindList.add( ( int )hostType );

		m_DbMutex.lock();
		RCODE rc = sqlExec( "UPDATE tblHosted SET join_time=? WHERE online_id=? AND host_type=?", bindList );
		m_DbMutex.unlock();
		result = 0 == rc;
	}

	return result;
}

//============================================================================
bool HostedListDb::updateHostUrl( EHostType hostType, VxGUID& onlineId, std::string& hostUrl )
{
	std::string onlineHexStr;
	bool result = doesHostInfoExist( hostType, onlineId, onlineHexStr );
	if( result )
	{
		DbBindList bindList( hostUrl.c_str() );
		bindList.add( onlineHexStr.c_str() );
		bindList.add( ( int )hostType );

		m_DbMutex.lock();
		RCODE rc = sqlExec( "UPDATE tblHosted SET host_url=? WHERE online_id=? AND host_type=?", bindList );
		m_DbMutex.unlock();
		result = 0 == rc;
	}

	return result;
}

//============================================================================
bool HostedListDb::updateHostTitleAndDescription( EHostType hostType, VxGUID& onlineId, std::string& title, std::string& description, int64_t lastDescUpdateTime )
{
	std::string onlineHexStr;
	bool result = doesHostInfoExist( hostType, onlineId, onlineHexStr );
	if( result )
	{
		DbBindList bindList( title.c_str() );
		bindList.add( description.c_str() );
		bindList.add( lastDescUpdateTime );
		bindList.add( onlineHexStr.c_str() );
		bindList.add( ( int )hostType );

		m_DbMutex.lock();
		RCODE rc = sqlExec( "UPDATE tblHosted SET host_title=?, host_desc=?, info_time=? WHERE online_id=? AND host_type=?", bindList );
		m_DbMutex.unlock();
		result = 0 == rc;
	}

	return result;
}


//============================================================================
bool HostedListDb::saveHosted( HostedInfo& hostedInfo )
{
	std::string onlineId;
	if( !hostedInfo.getOnlineId().isVxGUIDValid() || !hostedInfo.getOnlineId().toHexString( onlineId ) )
	{
		LogMsg( LOG_ERROR, "ERROR: HostedListDb::saveHosted INVALID ONLINE ID" );
		return false;
	}

	removeHostedInfo( hostedInfo.getHostType(), hostedInfo.getOnlineId() );

	m_DbMutex.lock(); // make thread safe

	DbBindList bindList( onlineId.c_str() );
	bindList.add( ( int )hostedInfo.getHostType() );
	bindList.add( hostedInfo.getHostInviteUrl().c_str() );
	bindList.add( hostedInfo.getHostTitle().c_str() );
	bindList.add( hostedInfo.getHostDescription().c_str() );

	bindList.add( ( int )hostedInfo.getIsFavorite() );

	bindList.add( hostedInfo.getConnectedTimestamp() );
	bindList.add( hostedInfo.getJoinedTimestamp() );
	bindList.add( hostedInfo.getHostInfoTimestamp() );

	// insert new record
	RCODE rc = sqlExec( "INSERT INTO tblHosted (online_id, host_type, host_url, host_title, host_desc, favorite, connect_time, join_time, info_time) VALUES(?,?,?,?,?,?,?,?,?)", bindList );

	if( rc )
	{
		LogMsg( LOG_ERROR, "HostedListDb::saveHosted: ERROR %d updating host url", rc );
	}

	m_DbMutex.unlock();
	return 0 == rc;
}
