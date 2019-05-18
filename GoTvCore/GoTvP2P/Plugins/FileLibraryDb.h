#pragma once
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
// http://www.gotvptop.net
//============================================================================


#include <CoreLib/DbBase.h>

class LibraryFileInfo;

class FileLibraryDb : public DbBase
{
public:
	FileLibraryDb();
	virtual ~FileLibraryDb();

	void						lockFileLibraryDb( void )			{ m_FileLibraryDbMutex.lock(); }
	void						unlockFileLibraryDb( void )			{ m_FileLibraryDbMutex.unlock(); }

	virtual RCODE				onCreateTables( int iDbVersion );
	virtual RCODE				onDeleteTables( int iOldVersion );

	void 						addFile( const char * fileName, int64_t fileLen, uint8_t fileType, uint8_t * fileHashId );
	void 						addFile( LibraryFileInfo* libFileInfo );
	void						removeFile( const char * fileName );

	void						getAllFiles( std::vector<LibraryFileInfo*>& sharedFileList );
	void						purgeAllFileLibrary( void ); 
	void						updateFileTypes( void );

protected:
	VxMutex						m_FileLibraryDbMutex;
};

