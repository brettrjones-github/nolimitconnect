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
	const int			COLUMN_IDX_TIMESTAMP			= 3;
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
	RCODE rc = sqlExec( "CREATE TABLE tblHosted (online_id TEXT, host_type INTEGER, host_url TEXT, timestamp BIGINT)" );
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
			hostInfo.setHosted( cursor->getString( COLUMN_IDX_HOST_URL ) );
			hostInfo.setTimestamp( ( int64_t )cursor->getS64( COLUMN_IDX_TIMESTAMP ) );			

			hostedList.push_back( hostInfo );
		}

		cursor->close();
	}

	unlockDb();
}

//============================================================================
bool HostedListDb::saveHosted( HostedInfo& hostedInfo )
{
	RCODE rc = 0;
	std::string onlineId;
	if( !hostedInfo.getOnlineId().toHexString( onlineId ) )
	{
		LogMsg( LOG_ERROR, "ERROR: HostedListDb::saveHosted INVALID ONLINE ID" );
	}

	m_DbMutex.lock(); // make thread safe

	bool bExists = false;
    DbCursor * cursor = startQuery(  "SELECT * FROM tblHosted WHERE online_id=? AND host_type=?", onlineId.c_str(), (int)hostedInfo.getHostType() );
	if( cursor )
    {
		if( cursor->getNextRow() )
		{
			bExists = true;
		}

		cursor->close();
	}

	DbBindList bindList( hostedInfo.getHosted().c_str() );
	bindList.add( hostedInfo.getTimestamp() );
	bindList.add( onlineId.c_str() );
	bindList.add( (int)hostedInfo.getHostType() );

	if( bExists )
	{
		rc = sqlExec( "UPDATE tblHosted SET host_url=?,timestamp=? WHERE online_id=? AND host_type=?", bindList );
	}
	else
	{
		// insert new record
		rc = sqlExec( "INSERT INTO tblHosted (host_url, timestamp, online_id, host_type) VALUES(?,?,?,?)", bindList );
	}

	if( rc )
	{
		LogMsg( LOG_ERROR, "HostedListDb::saveHosted: ERROR %d updating host url", rc );
	}

	m_DbMutex.unlock();
	return 0 == rc;
}

//============================================================================
void HostedListDb::removeClosedPortIdent( VxGUID& onlineId )
{
	std::string onlineIdHex = onlineId.toHexString();
	DbBindList bindList( onlineIdHex.c_str() );
	sqlExec( "DELETE FROM tblHosted WHERE online_id=?", bindList );
}
