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
#include <PktLib/PktBlobEntry.h>

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
	determineShortName();
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
	determineShortName();
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
	determineShortName();
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
	determineShortName();
}

//============================================================================
FileInfo& FileInfo::operator=( const FileInfo& rhs ) 
{	
	m_OnlineId				= rhs.m_OnlineId;
	m_FullFileName			= rhs.m_FullFileName;
	m_ShortFileName			= rhs.m_ShortFileName;
	m_s64FileLen			= rhs.m_s64FileLen;
	m_u32Attributes			= rhs.m_u32Attributes;
	m_u8FileType			= rhs.m_u8FileType;
	m_FileHash				= rhs.m_FileHash;
	m_ContainedInDir		= rhs.m_ContainedInDir;
	m_IsDirty				= rhs.m_IsDirty;
	m_AssetId				= rhs.m_AssetId;
	m_ThumbId				= rhs.m_ThumbId;
	m_FileTime				= rhs.m_FileTime;
	return *this;
}

//============================================================================
bool FileInfo::isValid( bool includeHashValid )
{
	bool valid = !m_FullFileName.empty() && !m_ShortFileName.empty() && m_s64FileLen && m_u8FileType && m_AssetId.isVxGUIDValid();
	if( includeHashValid )
	{
		valid &= m_FileHash.isHashValid();
	}

	return valid;
}

//============================================================================
bool FileInfo::determineShortName( std::string containingDir )
{
	bool result{ false };
	m_ContainedInDir = containingDir;
	std::string shortFileName{ "" };
	if( !m_ContainedInDir.empty()
		&& ( getFullFileName().length() > m_ContainedInDir.length() ) )
	{
		const char* lclFileName = getFullFileName().c_str();
		shortFileName = &lclFileName[m_ContainedInDir.length()];
		result = true;
	}
	else
	{
		std::string 	filePath;
		RCODE rc = VxFileUtil::seperatePathAndFile( getFullFileName(),
			filePath,
			shortFileName );
		if( 0 == rc )
		{
			m_ShortFileName = shortFileName;
			result = true;
		}
		else
		{
			shortFileName = "";
			if( !getFullFileName().empty() )
			{
				LogMsg( LOG_ERROR, "FileInfo::determineShortName failed error %d file %s", rc, getFullFileName().c_str() );
			}
			else
			{
				LogMsg( LOG_ERROR, "FileInfo::determineShortName failed error %d empty file name", rc );
			}			
		}
	}

	return result;
}

//============================================================================
bool FileInfo::determineFullFileName( std::string containingDir )
{
	bool result{ false };
	m_ContainedInDir = containingDir;
	if( !m_ContainedInDir.empty() && !getShortFileName().empty() )
	{
		m_FullFileName = m_ContainedInDir + getShortFileName();
		result = true;
	}

	return result && isValid();
}

//============================================================================
bool FileInfo::isDirectory( void )
{
	return ( VXFILE_TYPE_DIRECTORY & m_u8FileType ) ? true : false;
}

//============================================================================
void FileInfo::generateAssetId( void )
{
	if( !m_AssetId.isVxGUIDValid() )
	{
		m_AssetId.initializeWithNewVxGUID();
	}
}

//============================================================================
bool FileInfo::matchText( std::string& searchStr )
{
	bool result{ false };



	return result;
}

//============================================================================
int FileInfo::calcBlobLen( void )
{
	int blobLen{ 0 };
	blobLen += sizeof( int64_t); // m_s64FileLen
	blobLen += m_ShortFileName.length() + sizeof(uint32_t); // m_ShortFileName
	blobLen += sizeof( uint8_t ); // m_u8FileType
	blobLen += sizeof( int64_t ); // m_FileTime
	blobLen += sizeof( uint64_t ) * 2; // m_AssetId
	blobLen += sizeof( uint32_t ) + sizeof( VxSha1Hash ); // m_FileHash
	return blobLen;
}

//============================================================================
bool FileInfo::addToBlob( PktBlobEntry& blob )
{
	bool result = blob.setValue( m_s64FileLen ); 
	result &= blob.setValue( m_ShortFileName );
	result &= blob.setValue( m_u8FileType );
	result &= blob.setValue( m_FileTime );
	result &= blob.setValue( m_AssetId );
	result &= blob.setValue( m_FileHash );
	return result;
}

//============================================================================
bool FileInfo::extractFromBlob( PktBlobEntry& blob )
{
	bool result = blob.getValue( m_s64FileLen );
	result &= blob.getValue( m_ShortFileName );
	result &= blob.getValue( m_u8FileType );
	result &= blob.getValue( m_FileTime );
	result &= blob.getValue( m_AssetId );
	result &= blob.getValue( m_FileHash );
	return result;
}