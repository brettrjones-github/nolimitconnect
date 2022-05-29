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
// http://www.nolimitconnect.org
//============================================================================

#include "HostUrlListDb.h"
#include "HostUrlInfo.h"

#include <CoreLib/VxGlobals.h>
#include <CoreLib/VxParse.h>
#include <CoreLib/sqlite3.h>

#include <memory.h>

namespace
{
	const int			COLUMN_IDX_ONLINE_ID			= 0;
	const int			COLUMN_IDX_HOST_TYPE			= 1;
	const int			COLUMN_IDX_HOST_URL				= 2;
	const int			COLUMN_IDX_TIMESTAMP			= 3;
}

//============================================================================
HostUrlListDb::HostUrlListDb()
: DbBase( "HostUrlListDb" )
{
}

//============================================================================
HostUrlListDb::~HostUrlListDb()
{
}

//============================================================================
RCODE HostUrlListDb::hostUrlListDbStartup( int dbVersion, const char* dbFileName )
{
	dbShutdown();
	return dbStartup( dbVersion, dbFileName );
}

//============================================================================
RCODE HostUrlListDb::hostUrlListDbShutdown( void )
{
	return dbShutdown();
}

//============================================================================
RCODE HostUrlListDb::onCreateTables( int iDbVersion )
{
	RCODE rc = sqlExec( "CREATE TABLE tblHostUrl (online_id TEXT, host_type INTEGER, host_url TEXT, timestamp BIGINT)" );
	vx_assert( 0 == rc );
	return rc;
}

//============================================================================
RCODE HostUrlListDb::onDeleteTables( int iOldVersion )
{
	RCODE rc = sqlExec( (char *)"DROP TABLE tblHostUrl" );
	vx_assert( 0 == rc );
	return rc;
}

//============================================================================
void HostUrlListDb::getAllHostUrls( std::vector<HostUrlInfo>& hostUrlList )
{
	lockDb();
	DbCursor* cursor = startQuery( "SELECT * FROM tblHostUrl" );
	if( NULL != cursor )
	{
		while( cursor->getNextRow() )
		{
			HostUrlInfo hostInfo;

			hostInfo.getOnlineId().fromVxGUIDHexString( cursor->getString( COLUMN_IDX_ONLINE_ID ) );
			hostInfo.setHostType( (EHostType)cursor->getS32( COLUMN_IDX_HOST_TYPE ) );
			hostInfo.setHostUrl( cursor->getString( COLUMN_IDX_HOST_URL ) );
			hostInfo.setTimestamp( ( int64_t )cursor->getS64( COLUMN_IDX_TIMESTAMP ) );			

			hostUrlList.push_back( hostInfo );
		}

		cursor->close();
	}

	unlockDb();
}

//============================================================================
bool HostUrlListDb::saveHostUrl( HostUrlInfo& hostUrlInfo )
{
	RCODE rc = 0;
	std::string onlineId;
	if( !hostUrlInfo.getOnlineId().toHexString( onlineId ) )
	{
		LogMsg( LOG_ERROR, "ERROR: HostUrlListDb::saveHostUrl INVALID ONLINE ID" );
	}

	m_DbMutex.lock(); // make thread safe

	bool bExists = false;
    DbCursor * cursor = startQuery(  "SELECT * FROM tblHostUrl WHERE online_id=? AND host_type=?", onlineId.c_str(), (int)hostUrlInfo.getHostType() );
	if( cursor )
    {
		if( cursor->getNextRow() )
		{
			bExists = true;
		}

		cursor->close();
	}

	DbBindList bindList( hostUrlInfo.getHostUrl().c_str() );
	bindList.add( hostUrlInfo.getTimestamp() );
	bindList.add( onlineId.c_str() );
	bindList.add( (int)hostUrlInfo.getHostType() );

	if( bExists )
	{
		rc = sqlExec( "UPDATE tblHostUrl SET host_url=?,timestamp=? WHERE online_id=? AND host_type=?", bindList );
	}
	else
	{
		// insert new record
		rc = sqlExec( "INSERT INTO tblHostUrl (host_url, timestamp, online_id, host_type) VALUES(?,?,?,?)", bindList );
	}

	if( rc )
	{
		LogMsg( LOG_ERROR, "HostUrlListDb::saveHostUrl: ERROR %d updating host url", rc );
	}

	m_DbMutex.unlock();
	return 0 == rc;
}

//============================================================================
void HostUrlListDb::removeClosedPortIdent( VxGUID& onlineId )
{
	m_DbMutex.lock();
	std::string onlineIdHex = onlineId.toHexString();
	DbBindList bindList( onlineIdHex.c_str() );
	sqlExec( "DELETE FROM tblHostUrl WHERE online_id=?", bindList );
	m_DbMutex.unlock();
}
