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
	std::string 		TABLE_FILE_INFO	 				= "file_info";
	std::string 		CREATE_COLUMNS_FILE_INFO		= " (asset_id TEXT PRIMARY KEY, online_id TEXT, file_name TEXT, file_length BIGINT, file_type INTEGER, file_time BIGINT, file_hash BLOB ) ";

	const int			COLUMN_FILE_INFO_ASSET_ID		= 0;
	const int			COLUMN_FILE_INFO_ONLINE_ID		= 1;
	const int			COLUMN_FILE_INFO_FILE_NAME		= 2;
	const int			COLUMN_FILE_INFO_FILE_LEN		= 3;
	const int			COLUMN_FILE_INFO_FILE_TYPE		= 4;
	const int			COLUMN_FILE_INFO_FILE_TIME		= 5;
	const int			COLUMN_FILE_INFO_FILE_HASH		= 6;
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
    std::string strCmd = "CREATE TABLE " + TABLE_FILE_INFO + CREATE_COLUMNS_FILE_INFO;
    RCODE rc = sqlExec(strCmd);
	unlockFileInfoDb();
	return rc;
}

//============================================================================
// delete tables in database
RCODE FileInfoDb::onDeleteTables( int iOldVersion ) 
{
	lockFileInfoDb();
    std::string strCmd = "DROP TABLE IF EXISTS " + TABLE_FILE_INFO;
    RCODE rc = sqlExec(strCmd);
	unlockFileInfoDb();
	return rc;
}

//============================================================================
void FileInfoDb::purgeAllFileLibrary( void ) 
{
	lockFileInfoDb();
    std::string strCmd = "DELETE FROM " + TABLE_FILE_INFO;
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
	sqlExec( "DELETE FROM file_info WHERE file_name=?", bindList );
	unlockFileInfoDb();
}

//============================================================================
void FileInfoDb::removeFile( VxGUID& onlineId, VxGUID& assetId )
{
	lockFileInfoDb();
	DbBindList bindList( assetId.toHexString().c_str() );
	sqlExec( "DELETE FROM file_info WHERE asset_id=?", bindList );
	unlockFileInfoDb();
}

//============================================================================
void FileInfoDb::addFile( VxGUID& onlineId, std::string& fileName, int64_t fileLen, uint8_t fileType, VxGUID& assetId, VxSha1Hash& fileHashId, int64_t fileTime )
{
	removeFile( fileName );

	lockFileInfoDb();
	DbBindList bindList( assetId.toHexString().c_str() );
	bindList.add( onlineId.toHexString().c_str() );
	bindList.add( fileName.c_str() );
	bindList.add( fileLen );
	bindList.add( (int)fileType );
	bindList.add( fileTime );
	bindList.add( (void *)fileHashId.getHashData(), 20 );
	
	RCODE rc  = sqlExec( "INSERT INTO file_info (asset_id,online_id,file_name,file_length,file_type,file_time,file_hash) values(?,?,?,?,?,?,?)",
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
	addFile(	libFileInfo->getOnlineId(),
				libFileInfo->getFileName(),
				libFileInfo->getFileLength(),
				libFileInfo->getFileType(),
				libFileInfo->getAssetId(),
				libFileInfo->getFileHashId(),
				libFileInfo->getFileTime()
				 );
}

//============================================================================
void FileInfoDb::getAllFiles( std::map<VxGUID, FileInfo*>& sharedFileList )
{
	std::string fileName;
	uint8_t fileType;
	int64_t fileLen;
	std::string destfile;
	std::string onlineIdStr;
	VxGUID onlineId;
	std::string assetIdStr;
	VxGUID assetId;
	lockFileInfoDb();
	std::vector<std::string> deletedFiles; 
	DbCursor * cursor = startQuery( "SELECT * FROM file_info" );
	if( NULL != cursor )
	{
		while( cursor->getNextRow() )
		{
			onlineIdStr = cursor->getString( COLUMN_FILE_INFO_ONLINE_ID );
			onlineId.fromVxGUIDHexString( onlineIdStr.c_str() );

			assetIdStr = cursor->getString( COLUMN_FILE_INFO_ASSET_ID );
			assetId.fromVxGUIDHexString( assetIdStr.c_str() );

			fileName = cursor->getString( COLUMN_FILE_INFO_FILE_NAME );
			fileLen =  cursor->getS64( COLUMN_FILE_INFO_FILE_LEN );
			fileType = (uint8_t)cursor->getS32( COLUMN_FILE_INFO_FILE_TYPE );

			if( fileLen && onlineId.isVxGUIDValid() && assetId.isVxGUIDValid() )
			{
				FileInfo* libFileInfo = new FileInfo( onlineId, fileName, fileLen, fileType, assetId );
				libFileInfo->setFileHashId( ( uint8_t* )cursor->getBlob( COLUMN_FILE_INFO_FILE_HASH ) );
				uint64_t fileTime = cursor->getS64( COLUMN_FILE_INFO_FILE_TIME );
				libFileInfo->setFileTime( fileTime );

				sharedFileList[assetId] = libFileInfo;
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


