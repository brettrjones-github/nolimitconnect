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

#include "GroupieListDb.h"
#include "GroupieInfo.h"

#include <CoreLib/VxGlobals.h>
#include <CoreLib/VxParse.h>
#include <CoreLib/sqlite3.h>

#include <memory.h>

namespace
{
	const int			COLUMN_IDX_GROUPIE_ONLINE_ID	= 0;
	const int			COLUMN_IDX_HOST_ONLINE_ID		= 1;
	const int			COLUMN_IDX_HOST_TYPE			= 2;
	const int			COLUMN_IDX_HOST_URL				= 3;
	const int			COLUMN_IDX_HOST_TITLE			= 4;
	const int			COLUMN_IDX_HOST_DESC			= 5;
	const int			COLUMN_IDX_IS_FAVORITE			= 6;
	const int			COLUMN_IDX_CONNECT_TIME			= 7;
	const int			COLUMN_IDX_JOIN_TIME			= 8;
	const int			COLUMN_IDX_HOST_INFO_TIME		= 9;
}

//============================================================================
GroupieListDb::GroupieListDb()
: DbBase( "GroupieListDb" )
{
}

//============================================================================
GroupieListDb::~GroupieListDb()
{
}

//============================================================================
RCODE GroupieListDb::groupieListDbStartup( int dbVersion, const char* dbFileName )
{
	dbShutdown();
	return dbStartup( dbVersion, dbFileName );
}

//============================================================================
RCODE GroupieListDb::groupieListDbShutdown( void )
{
	return dbShutdown();
}

//============================================================================
RCODE GroupieListDb::onCreateTables( int iDbVersion )
{
	RCODE rc = sqlExec( "CREATE TABLE tblGroupie (groupieOnlineId TEXT, hostOnlineId TEXT, hostType INTEGER, groupieUrl TEXT, groupieTitle TEXT, groupieDesc TEXT, favorite INTEGER, connectTime BIGINT, joinTime BIGINT, infoTime BIGINT)" );
	vx_assert( 0 == rc );
	return rc;
}

//============================================================================
RCODE GroupieListDb::onDeleteTables( int iOldVersion )
{
	RCODE rc = sqlExec( (char *)"DROP TABLE tblGroupie" );
	vx_assert( 0 == rc );
	return rc;
}

//============================================================================
void GroupieListDb::getAllGroupies( std::vector<GroupieInfo>& groupieList )
{
	lockDb();
	DbCursor* cursor = startQuery( "SELECT * FROM tblGroupie" );
	if( NULL != cursor )
	{
		while( cursor->getNextRow() )
		{
			GroupieInfo hostInfo;

			hostInfo.getGroupieOnlineId().fromVxGUIDHexString( cursor->getString( COLUMN_IDX_GROUPIE_ONLINE_ID ) );
			hostInfo.getHostOnlineId().fromVxGUIDHexString( cursor->getString( COLUMN_IDX_HOST_ONLINE_ID ) );
			hostInfo.setHostType( (EHostType)cursor->getS32( COLUMN_IDX_HOST_TYPE ) );
			hostInfo.setGroupieUrl( cursor->getString( COLUMN_IDX_HOST_URL ) );
			hostInfo.setGroupieTitle( cursor->getString( COLUMN_IDX_HOST_TITLE ) );
			hostInfo.setGroupieDescription( cursor->getString( COLUMN_IDX_HOST_DESC ) );
			hostInfo.setIsFavorite( cursor->getS32( COLUMN_IDX_IS_FAVORITE ) );
			hostInfo.setConnectedTimestamp( ( int64_t )cursor->getS64( COLUMN_IDX_CONNECT_TIME ) );
			hostInfo.setJoinedTimestamp( ( int64_t )cursor->getS64( COLUMN_IDX_JOIN_TIME ) );
			hostInfo.setGroupieInfoTimestamp( ( int64_t )cursor->getS64( COLUMN_IDX_HOST_INFO_TIME ) );

			groupieList.push_back( hostInfo );
		}

		cursor->close();
	}

	unlockDb();
}

//============================================================================
void GroupieListDb::removeGroupieInfo( VxGUID& groupieOnlineId, VxGUID& hostOnlineId, EHostType hostType )
{
	std::string groupieOnlineIdHex = groupieOnlineId.toHexString();
	std::string hostOnlineIdHex = hostOnlineId.toHexString();

	DbBindList bindList( groupieOnlineIdHex.c_str() );
	bindList.add( hostOnlineIdHex.c_str() );
	bindList.add( ( int )hostType );
	m_DbMutex.lock();

	sqlExec( "DELETE FROM tblGroupie WHERE groupieOnlineId=? AND hostOnlineId=? AND hostType=?", bindList );
	m_DbMutex.unlock();
}

//============================================================================
bool GroupieListDb::doesGroupieInfoExist( VxGUID& groupieOnlineId, VxGUID& hostOnlineId, EHostType hostType, std::string& retGroupieOnlineHexStr, std::string& retHostOnlineHexStr )
{
	if( !groupieOnlineId.isVxGUIDValid() || !groupieOnlineId.toHexString( retGroupieOnlineHexStr ) )
	{
		LogMsg( LOG_ERROR, "ERROR: GroupieListDb::doesGroupieInfoExist INVALID GROUPIE ID" );
		return false;
	}

	if( !hostOnlineId.isVxGUIDValid() || !hostOnlineId.toHexString( retHostOnlineHexStr ) )
	{
		LogMsg( LOG_ERROR, "ERROR: GroupieListDb::doesGroupieInfoExist INVALID GROUPIE ID" );
		return false;
	}

	m_DbMutex.lock(); // make thread safe

	bool doesExists = false;
	DbCursor* cursor = startQuery( "SELECT * FROM tblGroupie WHERE groupieOnlineId=? hostOnlineId=? AND hostType=?", retGroupieOnlineHexStr.c_str(), retHostOnlineHexStr.c_str(), ( int )hostType );
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
bool GroupieListDb::updateIsFavorite( VxGUID& groupieOnlineId, VxGUID& hostOnlineId, EHostType hostType, bool isFavorite )
{
	std::string groupieOnlineHexStr;
	std::string hostOnlineHexStr;
	bool result = doesGroupieInfoExist( groupieOnlineId, hostOnlineId, hostType, groupieOnlineHexStr, hostOnlineHexStr );
	if( result )
	{
		DbBindList bindList( (int)isFavorite );
		bindList.add( groupieOnlineHexStr.c_str() );
		bindList.add( hostOnlineHexStr.c_str() );
		bindList.add( ( int )hostType );

		m_DbMutex.lock();
		RCODE rc = sqlExec( "UPDATE tblGroupie SET favorite=? WHERE groupieOnlineId=? hostOnlineId=? AND hostType=?", bindList );
		m_DbMutex.unlock();
		result = 0 == rc;
	}

	return result;
}

//============================================================================
bool GroupieListDb::updateLastConnected( VxGUID& groupieOnlineId, VxGUID& hostOnlineId, EHostType hostType, int64_t lastConnectedTime )
{
	std::string groupieOnlineHexStr;
	std::string hostOnlineHexStr;
	bool result = doesGroupieInfoExist( groupieOnlineId, hostOnlineId, hostType, groupieOnlineHexStr, hostOnlineHexStr );
	if( result )
	{
		DbBindList bindList( lastConnectedTime );
		bindList.add( groupieOnlineHexStr.c_str() );
		bindList.add( hostOnlineHexStr.c_str() );
		bindList.add( ( int )hostType );

		m_DbMutex.lock();
		RCODE rc = sqlExec( "UPDATE tblGroupie SET connectTime=? WHERE groupieOnlineId=? hostOnlineId=? AND hostType=?", bindList );
		m_DbMutex.unlock();
		result = 0 == rc;
	}

	return result;
}

//============================================================================
bool GroupieListDb::updateLastJoined( VxGUID& groupieOnlineId, VxGUID& hostOnlineId, EHostType hostType, int64_t lastJoinedTime )
{
	std::string groupieOnlineHexStr;
	std::string hostOnlineHexStr;
	bool result = doesGroupieInfoExist( groupieOnlineId, hostOnlineId, hostType, groupieOnlineHexStr, hostOnlineHexStr );
	if( result )
	{
		DbBindList bindList( lastJoinedTime );
		bindList.add( groupieOnlineHexStr.c_str() );
		bindList.add( hostOnlineHexStr.c_str() );
		bindList.add( ( int )hostType );

		m_DbMutex.lock();
		RCODE rc = sqlExec( "UPDATE tblGroupie SET joinTime=? WHERE groupieOnlineId=? hostOnlineId=? AND hostType=?", bindList );
		m_DbMutex.unlock();
		result = 0 == rc;
	}

	return result;
}

//============================================================================
bool GroupieListDb::updateGroupieUrl( VxGUID& groupieOnlineId, VxGUID& hostOnlineId, EHostType hostType, std::string& hostUrl )
{
	std::string groupieOnlineHexStr;
	std::string hostOnlineHexStr;
	bool result = doesGroupieInfoExist( groupieOnlineId, hostOnlineId, hostType, groupieOnlineHexStr, hostOnlineHexStr );
	if( result )
	{
		DbBindList bindList( hostUrl.c_str() );
		bindList.add( groupieOnlineHexStr.c_str() );
		bindList.add( hostOnlineHexStr.c_str() );
		bindList.add( ( int )hostType );

		m_DbMutex.lock();
		RCODE rc = sqlExec( "UPDATE tblGroupie SET host_url=? WHERE groupieOnlineId=? hostOnlineId=? AND hostType=?", bindList );
		m_DbMutex.unlock();
		result = 0 == rc;
	}

	return result;
}

//============================================================================
bool GroupieListDb::updateGroupieTitleAndDescription( VxGUID& groupieOnlineId, VxGUID& hostOnlineId, EHostType hostType, std::string& title, std::string& description, int64_t lastDescUpdateTime )
{
	std::string groupieOnlineHexStr;
	std::string hostOnlineHexStr;
	bool result = doesGroupieInfoExist( groupieOnlineId, hostOnlineId, hostType, groupieOnlineHexStr, hostOnlineHexStr );
	if( result )
	{
		DbBindList bindList( title.c_str() );
		bindList.add( description.c_str() );
		bindList.add( lastDescUpdateTime );
		bindList.add( groupieOnlineHexStr.c_str() );
		bindList.add( hostOnlineHexStr.c_str() );
		bindList.add( ( int )hostType );

		m_DbMutex.lock();
		RCODE rc = sqlExec( "UPDATE tblGroupie SET groupieTitle=?, groupieDesc=?, infoTime=? WHERE groupieOnlineId=? hostOnlineId=? AND hostType=?", bindList );
		m_DbMutex.unlock();
		result = 0 == rc;
	}

	return result;
}


//============================================================================
bool GroupieListDb::saveGroupie( GroupieInfo& hostedInfo )
{
	std::string groupieOnlineId;
	if( !hostedInfo.getGroupieOnlineId().isVxGUIDValid() || !hostedInfo.getGroupieOnlineId().toHexString( groupieOnlineId ) )
	{
		LogMsg( LOG_ERROR, "ERROR: GroupieListDb::saveGroupie INVALID ONLINE ID" );
		return false;
	}

	std::string hostOnlineId;
	if( !hostedInfo.getHostOnlineId().isVxGUIDValid() || !hostedInfo.getHostOnlineId().toHexString( hostOnlineId ) )
	{
		LogMsg( LOG_ERROR, "ERROR: GroupieListDb::saveGroupie INVALID ONLINE ID" );
		return false;
	}

	removeGroupieInfo( hostedInfo.getGroupieOnlineId(), hostedInfo.getHostOnlineId(), hostedInfo.getHostType() );

	m_DbMutex.lock(); // make thread safe

	DbBindList bindList( groupieOnlineId.c_str() );
	bindList.add( hostOnlineId.c_str() );
	bindList.add( ( int )hostedInfo.getHostType() );
	bindList.add( hostedInfo.getGroupieUrl().c_str() );
	bindList.add( hostedInfo.getGroupieTitle().c_str() );
	bindList.add( hostedInfo.getGroupieDescription().c_str() );

	bindList.add( ( int )hostedInfo.getIsFavorite() );

	bindList.add( hostedInfo.getConnectedTimestamp() );
	bindList.add( hostedInfo.getJoinedTimestamp() );
	bindList.add( hostedInfo.getGroupieInfoTimestamp() );

	// insert new record
	RCODE rc = sqlExec( "INSERT INTO tblGroupie (groupieOnlineId, hostOnlineId, hostType, groupieUrl, groupieTitle, groupieDesc, favorite, connectTime, joinTime, infoTime) VALUES(?,?,?,?,?,?,?,?,?,?)", bindList );

	if( rc )
	{
		LogMsg( LOG_ERROR, "GroupieListDb::saveGroupie: ERROR %d updating host url", rc );
	}

	m_DbMutex.unlock();
	return 0 == rc;
}
