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
// http://www.nolimitconnect.org
//============================================================================

#include <ptop_src/ptop_engine_src/Plugins/FileInfo.h>
#include <ptop_src/ptop_engine_src/AssetBase/AssetBaseInfo.h>
#include <ptop_src/ptop_engine_src/OfferBase/OfferBaseInfo.h>

#include <PktLib/VxSearchDefs.h>
#include <PktLib/PktBlobEntry.h>

#include <NetLib/VxFileXferInfo.h>

#include <CoreLib/VxFileLists.h>
#include <CoreLib/VxFileIsTypeFunctions.h>
#include <CoreLib/VxFileUtil.h>
#include <CoreLib/VxParse.h>
#include <CoreLib/VxDebug.h>

#include <sys/types.h>
#include <sys/stat.h>

//============================================================================
FileInfo::FileInfo()
{ 
	assureValidAssetId();
}

//============================================================================
FileInfo::FileInfo( const FileInfo& rhs )
    : m_OnlineId( rhs.m_OnlineId )
    , m_FullFileName( rhs.m_FullFileName )
    , m_ShortFileName( rhs.m_ShortFileName )
    , m_s64FileLen( rhs.m_s64FileLen )
    , m_u32Attributes( rhs.m_u32Attributes )
    , m_u8FileType( rhs.m_u8FileType )
    , m_FileHash( rhs.m_FileHash )
    , m_ContainedInDir( rhs.m_ContainedInDir )
    , m_AssetId( rhs.m_AssetId )
    , m_ThumbId( rhs.m_ThumbId )
    , m_FileTime( rhs.m_FileTime )
    , m_XferSessionId( rhs.m_XferSessionId )
    , m_IsInLibrary( rhs.m_IsInLibrary )
    , m_IsSharedFile( rhs.m_IsSharedFile )
{
    assureValidAssetId();
}


//============================================================================
FileInfo::FileInfo( VxGUID& onlineId, const std::string& fileName )
	: m_OnlineId( onlineId )
	, m_FullFileName( fileName )
{ 
	determineShortName();
	assureValidAssetId();
}

//============================================================================
FileInfo::FileInfo( VxGUID& onlineId, const std::string& fileName, uint64_t fileLen, uint8_t fileType )
	: m_OnlineId( onlineId )
	, m_FullFileName( fileName )
	, m_s64FileLen(fileLen) 
	, m_u32Attributes(0) 
	, m_u8FileType(fileType)
	, m_ContainedInDir("")
{ 
	determineShortName();
	assureValidAssetId();
}


//============================================================================
FileInfo::FileInfo( VxGUID& onlineId, const std::string& fileName, uint64_t fileLen, uint8_t fileType, VxGUID& assetId )
	: m_OnlineId( onlineId ) 
	, m_FullFileName( fileName )
	, m_s64FileLen( fileLen )
	, m_u8FileType( fileType )
	, m_AssetId( assetId )
{
	determineShortName();
	assureValidAssetId(); // in case assetId is invalid
}

//============================================================================
FileInfo::FileInfo( VxGUID& onlineId, const std::string& fullFileName, uint64_t fileLen, uint8_t fileType, VxGUID& assetId, VxSha1Hash& sha1Hash )
	: m_OnlineId( onlineId )
	, m_FullFileName( fullFileName )
	, m_s64FileLen( fileLen )
	, m_u8FileType( fileType )
	, m_FileHash( sha1Hash )
    , m_AssetId( assetId )
{
	determineShortName();
	assureValidAssetId(); // in case assetId is invalid
}

//============================================================================
FileInfo::FileInfo( AssetBaseInfo& assetInfo )
: m_OnlineId( assetInfo.getCreatorId() )
, m_FullFileName( assetInfo.getAssetName() )
, m_s64FileLen( assetInfo.getAssetLength() )
, m_u8FileType( (uint8_t)assetInfo.getAssetType() )
, m_AssetId( assetInfo.getAssetUniqueId() )
, m_FileTime( assetInfo.getCreationTime() )
{
	determineShortName();
	assureValidAssetId();
}

//============================================================================
FileInfo::FileInfo( AssetBaseInfo& assetInfo, VxSha1Hash& sha1Hash )
	: FileInfo( assetInfo )
{
	m_FileHash = sha1Hash;
	assureValidAssetId();
}

//============================================================================
FileInfo::FileInfo( OfferBaseInfo& offerInfo )
	: m_OnlineId( offerInfo.getHistoryId() )
	, m_FullFileName( offerInfo.getOfferName() )
	, m_s64FileLen( offerInfo.getOfferLength() )
	, m_u8FileType( (uint8_t)offerInfo.getOfferType() )
	, m_AssetId( offerInfo.getOfferId() )
	, m_ThumbId( offerInfo.getThumbId() )
	, m_FileHash( offerInfo.getOfferHashId() )
	, m_FileTime( offerInfo.getCreationTime() )
{
	assureValidAssetId();
}

//============================================================================
FileInfo::FileInfo( VxFileXferInfo& xferInfo, VxGUID& onlineId )
	: m_OnlineId( onlineId )
	, m_FullFileName( xferInfo.getLclFileName() )
	, m_s64FileLen( xferInfo.getFileLength() )
	, m_u8FileType( VxFileUtil::fileExtensionToFileTypeFlag( xferInfo.getRmtFileName().c_str() ) )
	, m_AssetId( xferInfo.getAssetId() )
	, m_FileHash( xferInfo.getFileHashId() )
{
	assureValidAssetId();
}

//============================================================================
FileInfo& FileInfo::operator=( const FileInfo& rhs ) 
{	
    if( &rhs != this)
    {
        m_OnlineId				= rhs.m_OnlineId;
        m_FullFileName			= rhs.m_FullFileName;
        m_ShortFileName			= rhs.m_ShortFileName;
        m_s64FileLen			= rhs.m_s64FileLen;
        m_u32Attributes			= rhs.m_u32Attributes;
        m_u8FileType			= rhs.m_u8FileType;
        m_FileHash				= rhs.m_FileHash;
        m_ContainedInDir		= rhs.m_ContainedInDir;
        m_AssetId				= rhs.m_AssetId;
        m_ThumbId				= rhs.m_ThumbId;
        m_FileTime				= rhs.m_FileTime;
        m_XferSessionId			= rhs.m_XferSessionId;
        m_IsInLibrary			= rhs.m_IsInLibrary;
        m_IsSharedFile			= rhs.m_IsSharedFile;
    }

	return *this;
}

//============================================================================
VxGUID& FileInfo::initializeNewXferSessionId( void )
{
	m_XferSessionId.initializeWithNewVxGUID();
	return m_XferSessionId;
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
void FileInfo::setFullFileName( std::string fileName )
{
	if( !fileName.empty() )
	{
		m_FullFileName = fileName;
		determineShortName();
	}
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
		std::string filePath;
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

	return result && isValid( false );
}

//============================================================================
bool FileInfo::determineFilePath( void )
{
	if( m_ContainedInDir.empty() )
	{
		if( isDirectory() )
		{
			if( !m_FullFileName.empty() )
			{
				m_ContainedInDir = m_FullFileName;
			}
		}
		else if( !m_FullFileName.empty() )
		{
			std::string fileName;
			RCODE rc = VxFileUtil::seperatePathAndFile( getFullFileName(), m_ContainedInDir, fileName );
			if( 0 != rc || m_ContainedInDir.empty() )
			{
				LogMsg( LOG_ERROR, "FileInfo::determineFilePath Failed to get path from %s", getFullFileName().c_str() );
			}
		}
	}

	return !m_ContainedInDir.empty() && isValid( false );
}

//============================================================================
std::string& FileInfo::getFilePath( void )
{
	determineFilePath();
	return m_ContainedInDir;
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
void FileInfo::assureValidAssetId( void )
{
	generateAssetId();
}

//============================================================================
bool FileInfo::matchTextAndType( std::string& searchStr, uint8_t fileType )
{
	bool result{ false };
	if( searchStr.empty() && !fileType )
	{
		// all
		result = true;
	}
	else if( searchStr.empty() && fileType )
	{
		// by file type only
		result = getFileType() & fileType;
	}
	else if( !searchStr.empty() && !fileType )
	{
		// by search string only
		result = matchText( searchStr );
	}
	else
	{
		// by search string and file type
		result = (getFileType() & fileType) && matchText( searchStr );
	}

	return result;
}

//============================================================================
bool FileInfo::matchText( std::string& searchStr )
{
	return CaseInsensitiveFindSubstr( m_ShortFileName, searchStr ) >= 0;
}

//============================================================================
int FileInfo::calcBlobLen( void )
{
	int blobLen{ 0 };
	blobLen += sizeof( int64_t); // m_s64FileLen
	blobLen += m_ShortFileName.length() + sizeof(uint32_t); // m_ShortFileName
	blobLen += sizeof( uint8_t ); // m_u8FileType
	blobLen += sizeof( int64_t ); // m_FileTime
	blobLen += sizeof( uint64_t ) * 4; // m_AssetId + m_ThumbId
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
	result &= blob.setValue( m_ThumbId );
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
	result &= blob.getValue( m_ThumbId );
	result &= blob.getValue( m_FileHash );
	return result;
}
