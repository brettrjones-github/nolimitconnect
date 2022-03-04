#pragma once
//============================================================================
// Copyright (C) 2012 Brett R. Jones 
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
// http://www.nolimitconnect.com
//============================================================================

#include <CoreLib/VxDefs.h>
#include <CoreLib/VxSha1Hash.h>
#include <CoreLib/VxGUID.h>

#include <string>

class VxThread;

class FileInfo 
{
public:
	FileInfo();
	FileInfo( const std::string& fullFileName );
	FileInfo( const std::string& fullFileName, uint64_t fileLen, uint8_t fileType );
	FileInfo( const std::string& fullFileName, uint64_t fileLen, uint8_t fileType, VxGUID& assetId );
	FileInfo( const std::string& fullFileName, uint64_t fileLen, uint8_t fileType, VxGUID& assetId, VxSha1Hash& sha1Hash );

	FileInfo&					operator=( const FileInfo& rhs );

	bool						isValid( bool includeHashValid = true );

	bool						isDirectory( void );
	void						setIsDirty( bool isDirty )				{ m_IsDirty = isDirty; }
	bool						getIsDirty( void )						{ return m_IsDirty; }
	std::string&				getLocalFileName( void )				{ return m_FullFileName; }
	std::string					getRemoteFileName( void );

	void						setFileName( const char * fileName )	{ m_FullFileName = fileName; }
	void						setFileName( std::string& fileName )	{ m_FullFileName = fileName; }
	std::string&				getFileName( void )						{ return m_FullFileName; }

	void						setFileType( uint8_t fileType )			{ m_u8FileType = fileType; }
	uint8_t						getFileType( void )						{ return m_u8FileType; }
	void						setFileLength( int64_t fileLength )		{ m_s64FileLen = fileLength; }
	int64_t						getFileLength( void )					{ return m_s64FileLen; }

	void						setFileTime( int64_t fileTime )			{ m_FileTime = fileTime; }
	int64_t						getFileTime( void )						{ return m_FileTime; }


	void						setFileHashId( VxSha1Hash& id )			{ m_FileHash = id; }
	void						setFileHashId( uint8_t * id )			{ m_FileHash.setHashData( id ); }
	VxSha1Hash&					getFileHashId( void )					{ return m_FileHash; }

	void						setAssetId( VxGUID& assetId )			{ m_AssetId = assetId; }
	VxGUID&						getAssetId( void )						{ return m_AssetId; }

private:
	void						determineSharedDir( void );
	void						generateAssetId( void );

public:
	//=== vars ===//
	std::string					m_FullFileName{ ""};
	int64_t						m_s64FileLen{ 0 };
	uint32_t					m_u32Attributes{ 0 };
	uint8_t						m_u8FileType{ 0 };
	VxSha1Hash					m_FileHash;
	std::string					m_ContainedInDir{ "" };
	bool						m_IsDirty{ true };
	VxGUID						m_AssetId;
	int64_t						m_FileTime{ 0 };
};
