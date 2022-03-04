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
// http://www.nolimitconnect.com
//============================================================================

#include "FileInfoDb.h"
#include "FileInfo.h"

#include <CoreLib/VxFileUtil.h>

namespace
{
	std::string 		TABLE_LIBRARY_FILES	 				= "library_files";
	std::string 		CREATE_COLUMNS_LIBRARY_FILES		= " (file_name TEXT PRIMARY KEY, file_length BIGINT, file_type INTEGER, asset_id TEXT, file_hash BLOB, file_time BIGINT ) ";
}

//============================================================================
FileInfoDb::FileInfoDb( std::string fileLibraryDbName )
	: DbBase( fileLibraryDbName.c_str() )
	, m_FileInfoDbName( fileLibraryDbName )
{
}

//============================================================================
//! create tables in database 
RCODE FileInfoDb::onCreateTables( int iDbVersion )
{
	lockFileInfoDb();
    std::string strCmd = "CREATE TABLE " + TABLE_LIBRARY_FILES + CREATE_COLUMNS_LIBRARY_FILES;
    RCODE rc = sqlExec(strCmd);
	unlockFileInfoDb();
	return rc;
}

//============================================================================
// delete tables in database
RCODE FileInfoDb::onDeleteTables( int iOldVersion ) 
{
	lockFileInfoDb();
    std::string strCmd = "DROP TABLE IF EXISTS " + TABLE_LIBRARY_FILES;
    RCODE rc = sqlExec(strCmd);
	unlockFileInfoDb();
	return rc;
}

//============================================================================
void FileInfoDb::purgeAllFileLibrary( void ) 
{
	lockFileInfoDb();
    std::string strCmd = "DELETE FROM " + TABLE_LIBRARY_FILES;
    RCODE rc = sqlExec( strCmd );
	unlockFileInfoDb();
	if( rc )
	{
		LogMsg( LOG_ERROR, "FileInfoDb::purgeAllFileLibrary error %d", rc );
	}
	else
	{
		LogMsg( LOG_INFO, "FileInfoDb::purgeAllFileLibrary success" );
	}
}

//============================================================================
void FileInfoDb::removeFile( std::string& fileName )
{
	lockFileInfoDb();
	DbBindList bindList( fileName.c_str() );
	sqlExec( "DELETE FROM library_files WHERE file_name=?", bindList );
	unlockFileInfoDb();
}

//============================================================================
void FileInfoDb::addFile( std::string& fileName, int64_t fileLen, uint8_t fileType, VxGUID& assetId, VxSha1Hash& fileHashId, int64_t fileTime )
{
	removeFile( fileName );

	lockFileInfoDb();
	DbBindList bindList( fileName.c_str() );
	bindList.add( fileLen );
	bindList.add( (int)fileType );
	bindList.add( assetId.toHexString().c_str() );
	bindList.add( (void *)fileHashId.getHashData(), 20 );
	bindList.add( fileTime );

	RCODE rc  = sqlExec( "INSERT INTO library_files (file_name,file_length,file_type,asset_id,file_hash) values(?,?,?,?,?,?)",
		bindList );
	if( rc )
	{
		LogMsg( LOG_ERROR, "FileInfoDb::addFile error %d", rc );
	}

	unlockFileInfoDb();
}

//============================================================================
void FileInfoDb::addFile( FileInfo* libFileInfo )
{
	addFile(	libFileInfo->getFileName(),
				libFileInfo->getFileLength(),
				libFileInfo->getFileType(),
				libFileInfo->getAssetId(),
				libFileInfo->getFileHashId(),
				libFileInfo->getFileTime()
				 );
}

//============================================================================
void FileInfoDb::getAllFiles( std::vector<FileInfo*>& sharedFileList )
{
	std::string fileName;
	uint8_t fileType;
	int64_t fileLen;
	std::string destfile;
	std::string assetIdStr;
	VxGUID assetId;
	lockFileInfoDb();
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
				FileInfo * libFileInfo = new FileInfo( fileName, realFileLen, fileType, assetId );
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
		removeFile( (*iter) );
	}

	unlockFileInfoDb();
}


