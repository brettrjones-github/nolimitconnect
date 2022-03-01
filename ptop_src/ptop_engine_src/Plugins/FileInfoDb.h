#pragma once
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

#include <CoreLib/DbBase.h>
#include <CoreLib/VxGUID.h>

class FileInfo;

class FileInfoDb : public DbBase
{
public:
	FileInfoDb() = delete;
	FileInfoDb( std::string fileLibraryDbName );
	virtual ~FileInfoDb() = default;

	void						lockFileInfoDb( void )			{ m_FileInfoDbMutex.lock(); }
	void						unlockFileInfoDb( void )			{ m_FileInfoDbMutex.unlock(); }

	virtual RCODE				onCreateTables( int iDbVersion );
	virtual RCODE				onDeleteTables( int iOldVersion );

	void 						addFile( const char * fileName, int64_t fileLen, uint8_t fileType, VxGUID assetId, uint8_t * fileHashId, int64_t fileTime = 0 );
	void 						addFile( FileInfo* libFileInfo );
	void						removeFile( const char * fileName );

	void						getAllFiles( std::vector<FileInfo*>& sharedFileList );
	void						purgeAllFileLibrary( void ); 
	void						updateFileTypes( void );

	std::string&				getFileInfoDbName( void ) { return m_FileInfoDbName; }

protected:
	VxMutex						m_FileInfoDbMutex;
	std::string					m_FileInfoDbName{ "" };
};

