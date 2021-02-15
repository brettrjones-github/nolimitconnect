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
// http://www.nolimitconnect.com
//============================================================================

#include <config_gotvcore.h>
#include "BlobInfo.h"

#include <PktLib/VxSearchDefs.h>

#include <CoreLib/VxFileLists.h>
#include <CoreLib/VxFileIsTypeFunctions.h>
#include <CoreLib/VxFileUtil.h>
#include <CoreLib/VxDebug.h>
#include <CoreLib/VxGlobals.h>

#include <sys/types.h>
#include <sys/stat.h>

//============================================================================
BlobInfo::BlobInfo()
: m_BlobName( "" )
, m_BlobTag( "" )
, m_s64BlobLen( 0 )
, m_u16BlobType( VXFILE_TYPE_UNKNOWN )
, m_LocationFlags( 0 )
, m_BlobSendState( eBlobSendStateNone )
, m_CreationTime( 0 )
, m_PlayPosition0to100000( 0 )
{ 
}

//============================================================================
BlobInfo::BlobInfo( const BlobInfo& rhs )
{
	*this = rhs;
}

//============================================================================
BlobInfo::BlobInfo( const std::string& fileName )
: m_BlobName( fileName )
, m_BlobTag( "" )
, m_s64BlobLen(0)
, m_u16BlobType(VXFILE_TYPE_UNKNOWN)
, m_LocationFlags( 0 )
, m_BlobSendState( eBlobSendStateNone )
, m_CreationTime( 0 )
, m_PlayPosition0to100000( 0 )
{ 
}

//============================================================================
BlobInfo::BlobInfo( const char * fileName, uint64_t fileLen, uint16_t assetType )
: m_BlobName( fileName )
, m_BlobTag( "" )
, m_s64BlobLen( fileLen )
, m_u16BlobType( assetType )
, m_LocationFlags( 0 )
, m_BlobSendState( eBlobSendStateNone )
, m_CreationTime( GetTimeStampMs() )
, m_PlayPosition0to100000( 0 )
{
}

//============================================================================
BlobInfo& BlobInfo::operator=( const BlobInfo& rhs ) 
{	
	if( this != &rhs )
	{
		m_BlobName					= rhs.m_BlobName;
		m_UniqueId					= rhs.m_UniqueId;
		m_CreatorId					= rhs.m_CreatorId;
		m_HistoryId					= rhs.m_HistoryId; 
		m_BlobHash					= rhs.m_BlobHash;
		m_s64BlobLen				= rhs.m_s64BlobLen;
		m_u16BlobType				= rhs.m_u16BlobType;
		m_LocationFlags				= rhs.m_LocationFlags;
		m_CreationTime				= rhs.m_CreationTime;
		m_BlobTag					= rhs.m_BlobTag;
		m_BlobSendState			    = rhs.m_BlobSendState;
        m_PlayPosition0to100000     = rhs.m_PlayPosition0to100000;
	}

	return *this;
}

//============================================================================
bool BlobInfo::isValid( void )
{
	return ( VXFILE_TYPE_UNKNOWN != m_u16BlobType );
}

//============================================================================
bool BlobInfo::isMine( void )
{
	return isValid() && ( m_CreatorId == VxGetMyOnlineId() );
}

//============================================================================
// generates unique id, assigns it to asset and returns reference to it
VxGUID& BlobInfo::generateNewUniqueId( void )
{
	VxGUID::generateNewVxGUID( m_UniqueId );
	return m_UniqueId;
}

//============================================================================
bool BlobInfo::getIsFileBlob( void )
{
	return (0 != (m_u16BlobType & ( eBlobTypePhoto | eBlobTypeAudio | eBlobTypeVideo | eBlobTypeDocument | eBlobTypeArchives | eBlobTypeExe | eBlobTypeOtherFiles	)) );
}

//============================================================================
bool BlobInfo::hasFileName( void )
{
	return ( 0 != m_BlobName.length() ) && getIsFileBlob();
}

//============================================================================
bool BlobInfo::needsHashGenerated( void )
{
	if( ( false == m_BlobHash.isHashValid() )
		&& hasFileName() )
	{
		return true;
	}

	return false;
}

//============================================================================
bool BlobInfo::isDirectory( void )
{
	return ( VXFILE_TYPE_DIRECTORY & m_u16BlobType ) ? true : false;
}

//============================================================================
void BlobInfo::setBlobName( const char * assetName )
{
	if (assetName)
		m_BlobName = assetName;
	else
		m_BlobName = "";
}

//============================================================================
void BlobInfo::setBlobTag( const char * assetTag )
{
	if( assetTag )
		m_BlobTag = assetTag;
	else
		m_BlobTag = "";
}

//============================================================================
std::string BlobInfo::getRemoteBlobName( void )
{
	std::string rmtBlobName("");
	std::string 	assetPath;
	RCODE rc = VxFileUtil::seperatePathAndFile(	getBlobName(),					
		assetPath,		
		rmtBlobName );	
	if( 0 != rc )
	{
		LogMsg( LOG_ERROR, "BlobInfo::getRemoteBlobName failed error %d asset %s\n", rc, getBlobName().c_str() );
	}

	return rmtBlobName;
}

//============================================================================
const char * BlobInfo::getDefaultFileExtension( EBlobType assetType )
{
	const char * extension = ".txt";
	switch( assetType )
	{
	case eBlobTypePhoto:
		extension = ".jpg";
		break;

	case eBlobTypeAudio:
		extension = ".opus";
		break;

	case eBlobTypeVideo:
		extension = ".avi";
		break;

	default:
		break;
	}

	return extension;
}

//============================================================================
const char * BlobInfo::getSubDirectoryName( EBlobType assetType )
{
	const char * subDir = "asset/";
	switch( assetType )
	{
	case eBlobTypeDocument:
		subDir = "documents/";
		break;

	case eBlobTypeArchives:
		subDir = "archives/";
		break;

	case eBlobTypeOtherFiles:
		subDir = "other_files/";
		break;

	case eBlobTypePhoto:
		subDir = "photos/";
		break;

	case eBlobTypeAudio:
		subDir = "audio/";
		break;

	case eBlobTypeVideo:
		subDir = "video/";
		break;

	default:
		break;
	}

	return subDir;
}

//============================================================================
void BlobInfo::updateBlobInfo( VxThread * callingThread )
{
	return;
}
