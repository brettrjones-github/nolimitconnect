//============================================================================
// Copyright (C) 2015 Brett R. Jones
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

#include "SharedFilesDb.h"
#include "SharedFileInfo.h"

#include <CoreLib/VxFileUtil.h>

namespace
{
	std::string 		TABLE_SHARED_FILES	 				= "shared_files";
	std::string 		CREATE_COLUMNS_SHARED_FILES			= " (file_name TEXT PRIMARY KEY, file_length BIGINT, file_type INTEGER ) ";
}

//============================================================================
SharedFilesDb::SharedFilesDb()
: DbBase( "SharedFilesDb" )
{
}

//============================================================================
SharedFilesDb::~SharedFilesDb()
{
}

//============================================================================
//! create tables in database 
RCODE SharedFilesDb::onCreateTables( int iDbVersion )
{
	lockSharedFilesDb();
    std::string sqlCmd = "CREATE TABLE " + TABLE_SHARED_FILES + CREATE_COLUMNS_SHARED_FILES;
    RCODE rc = sqlExec(sqlCmd);
	unlockSharedFilesDb();
	return rc;
}

//============================================================================
// delete tables in database
RCODE SharedFilesDb::onDeleteTables( int iOldVersion ) 
{
	lockSharedFilesDb();
    std::string sqlCmd = "DROP TABLE IF EXISTS " + TABLE_SHARED_FILES;
    RCODE rc = sqlExec(sqlCmd);
	unlockSharedFilesDb();
	return rc;
}

//============================================================================
void SharedFilesDb::purgeAllSharedFiles( void ) 
{
	lockSharedFilesDb();
    std::string sqlCmd =  "DELETE FROM " + TABLE_SHARED_FILES;
    RCODE rc = sqlExec(sqlCmd);
	unlockSharedFilesDb();
	if( rc )
	{
		LogMsg( LOG_ERROR, "SharedFilesDb::purgeAllSharedFiles error %d\n", rc );
	}
	else
	{
		LogMsg( LOG_INFO, "SharedFilesDb::purgeAllSharedFiles success\n" );
	}
}

//============================================================================
void SharedFilesDb::removeFile( const char * fileName )
{
	lockSharedFilesDb();

	DbBindList bindList( fileName );
	sqlExec( "DELETE FROM shared_files WHERE file_name=?", bindList );
	unlockSharedFilesDb();
}

//============================================================================
void SharedFilesDb::addFile( const char * fileName, int64_t fileLen, uint8_t fileType )
{
	lockSharedFilesDb();

	DbBindList bindList( fileName );
	bindList.add( fileLen );
	bindList.add( (int)fileType );

	RCODE rc  = sqlExec( "INSERT INTO shared_files (file_name,file_length,file_type) values(?,?,?)",
		bindList );
	unlockSharedFilesDb();
	if( rc )
	{
		LogMsg( LOG_ERROR, "SharedFilesDb::addFile error %d\n", rc );
	}
}

//============================================================================
void SharedFilesDb::getAllFiles( std::vector<SharedFileInfo*>& sharedFileList )
{
	std::string fileName;
	uint8_t fileType;
	int64_t fileLen;
	std::string destfile;
	std::string consoleId;
	lockSharedFilesDb();
	std::vector<std::string> deletedFiles; 
	DbCursor * cursor = startQuery( "SELECT * FROM shared_files" );
	if( NULL != cursor )
	{
		while( cursor->getNextRow() )
		{
			fileName = cursor->getString( 0 );
			fileLen =  cursor->getS64( 1 );
			fileType = (uint8_t)cursor->getS32( 2 );
			uint64_t realFileLen = VxFileUtil::fileExists( fileName.c_str() );
			if( 0 != realFileLen )
			{
				sharedFileList.push_back( new SharedFileInfo( fileName, realFileLen, fileType ) );
			}
			else
			{
				deletedFiles.push_back( fileName );
			}
		}

		cursor->close();
	}

	std::vector<std::string>::iterator iter;
	for( iter = deletedFiles.begin(); iter != deletedFiles.end(); ++iter )
	{
		removeFile( (*iter).c_str() );
	}

	unlockSharedFilesDb();
}


