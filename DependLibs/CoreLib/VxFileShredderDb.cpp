//============================================================================
// Copyright (C) 2014 Brett R. Jones
// Issued to MIT style license by Brett R. Jones in 2017
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

#include "config_corelib.h"

#include "VxFileShredderDb.h"
#include "VxParse.h"

#include <string.h>

namespace
{
	const char* SHRED_FILES_DB_NAME = "ShredFilesDb.db3";
}

//============================================================================
VxFileShredderDb::VxFileShredderDb()
: DbBase( "VxFileShredderDb" )
{
}

//============================================================================
void VxFileShredderDb::initShredderDb( std::string& dataDirectory )
{
	std::string dbFileName = dataDirectory + SHRED_FILES_DB_NAME;
	dbStartup( 1, dbFileName );
}

//============================================================================
RCODE VxFileShredderDb::onCreateTables( int iDbVersion )
{
	RCODE rc = sqlExec( "CREATE TABLE table_files (file_name TEXT)" );
	vx_assert( 0 == rc );
	return rc;
}

//============================================================================
RCODE VxFileShredderDb::onDeleteTables( int iOldVersion )
{
	RCODE rc = sqlExec( (char *)"DROP TABLE table_files" );
	vx_assert( 0 == rc );
	return rc;
}

//============================================================================
void VxFileShredderDb::getShredList( std::vector<std::string>&	shredList )
{
	m_DbMutex.lock();
	DbCursor * cursor =  startQuery( "SELECT * FROM table_files" ); 
	if( NULL != cursor )
	{
		while( cursor->getNextRow() )
		{
			const char* tempStr = cursor->getString( 0 );
			if( tempStr 
				&& strlen( tempStr ) )
			{
				shredList.push_back( tempStr );
			}
		}

		cursor->close();
	}

	m_DbMutex.unlock();
}

//============================================================================
void VxFileShredderDb::addFileToShred( std::string& fileName )
{
	m_DbMutex.lock();
	bool fileExists = false;
	DbCursor * cursor = startQuery(  "SELECT * FROM table_files WHERE file_name=?", fileName.c_str() );
	if( NULL != cursor )
	{
		if( cursor->getNextRow() )
		{
			fileExists = true;
		}

		cursor->close();
	}

	if( fileExists )
	{
		// already in database
		m_DbMutex.unlock();
		return;
	}

	DbBindList bindList( fileName.c_str() );
	sqlExec( "INSERT INTO table_files (file_name) VALUES(?)", bindList );
	m_DbMutex.unlock();
}

//============================================================================
void VxFileShredderDb::removeFileToShred( std::string& fileName )
{
	m_DbMutex.lock();
	DbBindList bindList( fileName.c_str() );
	sqlExec( "DELETE FROM table_files WHERE file_name=?", bindList );
	m_DbMutex.unlock();
}

