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

#include "FileInfo.h"

#include <PktLib/VxSearchDefs.h>

#include <CoreLib/VxFileLists.h>
#include <CoreLib/VxFileIsTypeFunctions.h>
#include <CoreLib/VxFileUtil.h>
#include <CoreLib/VxDebug.h>

#include <sys/types.h>
#include <sys/stat.h>

//============================================================================
FileInfo::FileInfo()
{ 
	generateAssetId();
}

//============================================================================
FileInfo::FileInfo( VxGUID& onlineId, const std::string& fileName )
	: m_OnlineId( onlineId )
	, m_FullFileName( fileName )
{ 
	determineSharedDir();
	generateAssetId();
}

//============================================================================
FileInfo::FileInfo( VxGUID& onlineId, const std::string& fileName, uint64_t fileLen, uint8_t fileType )
	: m_OnlineId( onlineId )
	, m_FullFileName( fileName )
	, m_s64FileLen(fileLen) 
	, m_u32Attributes(0) 
	, m_u8FileType(fileType)
	, m_ContainedInDir("")
	, m_IsDirty( true )
{ 
	determineSharedDir();
	generateAssetId();
}


//============================================================================
FileInfo::FileInfo( VxGUID& onlineId, const std::string& fileName, uint64_t fileLen, uint8_t fileType, VxGUID& assetId )
	: m_OnlineId( onlineId ) 
	, m_FullFileName( fileName )
	, m_s64FileLen( fileLen )
	, m_u8FileType( fileType )
	, m_IsDirty( true )
	, m_AssetId( assetId )
{
	determineSharedDir();
}

//============================================================================
FileInfo::FileInfo( VxGUID& onlineId, const std::string& fullFileName, uint64_t fileLen, uint8_t fileType, VxGUID& assetId, VxSha1Hash& sha1Hash )
	: m_OnlineId( onlineId )
	, m_FullFileName( fullFileName )
	, m_s64FileLen( fileLen )
	, m_u8FileType( fileType )
	, m_IsDirty( true )
	, m_AssetId( assetId )
	, m_FileHash( sha1Hash )
{
}

//============================================================================
FileInfo& FileInfo::operator=( const FileInfo& rhs ) 
{	
	m_OnlineId				= rhs.m_OnlineId;
	m_FullFileName			= rhs.m_FullFileName;
	m_s64FileLen			= rhs.m_s64FileLen;
	m_u32Attributes			= rhs.m_u32Attributes;
	m_u8FileType			= rhs.m_u8FileType;
	m_FileHash				= rhs.m_FileHash;
	m_ContainedInDir		= rhs.m_ContainedInDir;
	m_IsDirty				= rhs.m_IsDirty;
	m_AssetId				= rhs.m_AssetId;
	m_FileTime				= rhs.m_FileTime;
	return *this;
}

//============================================================================
bool FileInfo::isValid( bool includeHashValid )
{
	bool valid = !m_FullFileName.empty() && m_s64FileLen && m_u8FileType && m_AssetId.isVxGUIDValid();
	if( includeHashValid )
	{
		valid &= m_FileHash.isHashValid();
	}

	return valid;
}

//============================================================================
void FileInfo::determineSharedDir( void )
{
	if( 0 == m_ContainedInDir.length() )
	{
		std::string 	strJustFileName;
		RCODE rc = VxFileUtil::seperatePathAndFile(	getLocalFileName(),					
			m_ContainedInDir,		
			strJustFileName );	
		if( 0 != rc )
		{
			LogMsg( LOG_ERROR, "FileInfo::determineSharedDir failed error %d file %s\n", rc, getLocalFileName().c_str() );
		}
	}
}

//============================================================================
bool FileInfo::isDirectory( void )
{
	return ( VXFILE_TYPE_DIRECTORY & m_u8FileType ) ? true : false;
}

//============================================================================
std::string FileInfo::getRemoteFileName( void )
{
	std::string rmtFileName("");
	if( m_ContainedInDir.length() 
		&& ( getLocalFileName().length() > m_ContainedInDir.length() ) )
	{
		const char * lclFileName = getLocalFileName().c_str();
		rmtFileName = &lclFileName[ m_ContainedInDir.length() ];
	}
	else
	{
		std::string 	filePath;
		RCODE rc = VxFileUtil::seperatePathAndFile(	getLocalFileName(),					
													filePath,		
													rmtFileName );	
		if( 0 != rc )
		{
			LogMsg( LOG_ERROR, "FileInfo::getRemoteFileName failed error %d file %s", rc, getLocalFileName().c_str() );
		}
	}

	return rmtFileName;
}

//============================================================================
void FileInfo::generateAssetId( void )
{
	if( !m_AssetId.isVxGUIDValid() )
	{
		m_AssetId.initializeWithNewVxGUID();
	}
}