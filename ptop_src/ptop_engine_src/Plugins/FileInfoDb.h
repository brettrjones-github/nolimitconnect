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

#include <map>

class FileInfo;
class VxSha1Hash;

class FileInfoDb : public DbBase
{
public:
	FileInfoDb() = delete;
	FileInfoDb( std::string fileLibraryDbName );
	virtual ~FileInfoDb() = default;

	void						lockFileInfoDb( void )				{ m_FileInfoDbMutex.lock(); }
	void						unlockFileInfoDb( void )			{ m_FileInfoDbMutex.unlock(); }

	virtual RCODE				onCreateTables( int iDbVersion );
	virtual RCODE				onDeleteTables( int iOldVersion );

	void 						addFile( VxGUID& onlineId, std::string& fileName, int64_t fileLen, uint8_t fileType, VxGUID& assetId, VxSha1Hash& fileHashId, int64_t fileTime = 0 );
	void 						addFile( FileInfo& libFileInfo );
	void						removeFile( std::string& fileName );
	void						removeFile( VxGUID& onlineId, VxGUID& assetId );

	void						getAllFiles( std::map<VxGUID, FileInfo>& sharedFileList );
	void						purgeAllFileLibrary( void ); 

	std::string&				getFileInfoDbName( void ) { return m_FileInfoDbName; }

protected:
	VxMutex						m_FileInfoDbMutex;
	std::string					m_FileInfoDbName{ "" };
};

