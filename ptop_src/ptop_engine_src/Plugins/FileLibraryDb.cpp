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

#include "FileLibraryDb.h"
#include "LibraryFileInfo.h"

#include <CoreLib/VxFileUtil.h>

namespace
{
	std::string 		TABLE_LIBRARY_FILES	 				= "library_files";
	std::string 		CREATE_COLUMNS_LIBRARY_FILES		= " (file_name TEXT PRIMARY KEY, file_length BIGINT, file_type INTEGER, asset_id TEXT, file_hash BLOB, file_time BIGINT ) ";
}

//============================================================================
FileLibraryDb::FileLibraryDb( std::string fileLibraryDbName )
	: DbBase( fileLibraryDbName.c_str() )
	, m_FileLibraryDbName( fileLibraryDbName )
{
}

//============================================================================
//! create tables in database 
RCODE FileLibraryDb::onCreateTables( int iDbVersion )
{
	lockFileLibraryDb();
    std::string strCmd = "CREATE TABLE " + TABLE_LIBRARY_FILES + CREATE_COLUMNS_LIBRARY_FILES;
    RCODE rc = sqlExec(strCmd);
	unlockFileLibraryDb();
	return rc;
}

//============================================================================
// delete tables in database
RCODE FileLibraryDb::onDeleteTables( int iOldVersion ) 
{
	lockFileLibraryDb();
    std::string strCmd = "DROP TABLE IF EXISTS " + TABLE_LIBRARY_FILES;
    RCODE rc = sqlExec(strCmd);
	unlockFileLibraryDb();
	return rc;
}

//============================================================================
void FileLibraryDb::purgeAllFileLibrary( void ) 
{
	lockFileLibraryDb();
    std::string strCmd = "DELETE FROM " + TABLE_LIBRARY_FILES;
    RCODE rc = sqlExec( strCmd );
	unlockFileLibraryDb();
	if( rc )
	{
		LogMsg( LOG_ERROR, "FileLibraryDb::purgeAllFileLibrary error %d", rc );
	}
	else
	{
		LogMsg( LOG_INFO, "FileLibraryDb::purgeAllFileLibrary success" );
	}
}

//============================================================================
void FileLibraryDb::removeFile( const char * fileName )
{
	lockFileLibraryDb();
	DbBindList bindList( fileName );
	sqlExec( "DELETE FROM library_files WHERE file_name=?", bindList );
	unlockFileLibraryDb();
}

//============================================================================
void FileLibraryDb::addFile( const char * fileName, int64_t fileLen, uint8_t fileType, VxGUID assetId, uint8_t * fileHashId, int64_t fileTime )
{
	removeFile( fileName );

	lockFileLibraryDb();
	DbBindList bindList( fileName );
	bindList.add( fileLen );
	bindList.add( (int)fileType );
	bindList.add( assetId.toHexString().c_str() );
	bindList.add( (void *)fileHashId, 20 );
	bindList.add( fileTime );

	RCODE rc  = sqlExec( "INSERT INTO library_files (file_name,file_length,file_type,asset_id,file_hash,file_time) values(?,?,?,?,?,?)",
		bindList );
	if( rc )
	{
		LogMsg( LOG_ERROR, "FileLibraryDb::addFile error %d", rc );
	}

	unlockFileLibraryDb();
}

//============================================================================
void FileLibraryDb::addFile( LibraryFileInfo* libFileInfo )
{
	addFile(	libFileInfo->getFileName().c_str(),
				libFileInfo->getFileLength(),
				libFileInfo->getFileType(),
				libFileInfo->getAssetId(),
				libFileInfo->getFileHashId().getHashData()
				 );
}

//============================================================================
void FileLibraryDb::getAllFiles( std::vector<LibraryFileInfo*>& sharedFileList )
{
	std::string fileName;
	uint8_t fileType;
	int64_t fileLen;
	std::string destfile;
	std::string assetIdStr;
	VxGUID assetId;
	lockFileLibraryDb();
	std::vector<std::string> deletedFiles; 
	DbCursor * cursor = startQuery( "SELECT * FROM library_files" );
	if( NULL != cursor )
	{
		while( cursor->getNextRow() )
		{
			fileName = cursor->getString( 0 );
			fileLen =  cursor->getS64( 1 );
			fileType = (uint8_t)cursor->getS32( 2 );
			assetIdStr = cursor->getString( 0 );
			assetId.fromVxGUIDHexString( assetIdStr.c_str() );

			uint64_t realFileLen = VxFileUtil::fileExists( fileName.c_str() );
			if( 0 != realFileLen )
			{
				LibraryFileInfo * libFileInfo = new LibraryFileInfo( fileName, realFileLen, fileType, assetId );
				libFileInfo->setFileHashId( (uint8_t *)cursor->getBlob( 3 ) );
				uint64_t fileTime = cursor->getS64( 4 );
				libFileInfo->setFileTime( fileTime );

				sharedFileList.push_back( libFileInfo );
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

	unlockFileLibraryDb();
}


