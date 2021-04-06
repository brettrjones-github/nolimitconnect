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
#include "AssetBaseInfo.h"

#include <PktLib/VxSearchDefs.h>

#include <CoreLib/VxFileLists.h>
#include <CoreLib/VxFileIsTypeFunctions.h>
#include <CoreLib/VxFileUtil.h>
#include <CoreLib/VxDebug.h>
#include <CoreLib/VxGlobals.h>

#include <sys/types.h>
#include <sys/stat.h>

//============================================================================
AssetBaseInfo::AssetBaseInfo()
: m_AssetName( "" )
, m_AssetTag( "" )
, m_s64AssetLen( 0 )
, m_u16AssetType( VXFILE_TYPE_UNKNOWN )
, m_LocationFlags( 0 )
, m_AssetSendState( eAssetSendStateNone )
, m_CreationTime( 0 )
, m_PlayPosition0to100000( 0 )
{ 
}

//============================================================================
AssetBaseInfo::AssetBaseInfo( const AssetBaseInfo& rhs )
{
	*this = rhs;
}

//============================================================================
AssetBaseInfo::AssetBaseInfo( const std::string& fileName )
: m_AssetName( fileName )
, m_AssetTag( "" )
, m_s64AssetLen(0)
, m_u16AssetType(VXFILE_TYPE_UNKNOWN)
, m_LocationFlags( 0 )
, m_AssetSendState( eAssetSendStateNone )
, m_CreationTime( 0 )
, m_PlayPosition0to100000( 0 )
{ 
}

//============================================================================
AssetBaseInfo::AssetBaseInfo( const char * fileName, uint64_t fileLen, uint16_t assetType )
: m_AssetName( fileName )
, m_AssetTag( "" )
, m_s64AssetLen( fileLen )
, m_u16AssetType( assetType )
, m_LocationFlags( 0 )
, m_AssetSendState( eAssetSendStateNone )
, m_CreationTime( GetTimeStampMs() )
, m_ModifiedTime( m_CreationTime )
, m_PlayPosition0to100000( 0 )
{
}

//============================================================================
AssetBaseInfo& AssetBaseInfo::operator=( const AssetBaseInfo& rhs ) 
{	
	if( this != &rhs )
	{
		m_AssetName					= rhs.m_AssetName;
		m_UniqueId					= rhs.m_UniqueId;
		m_CreatorId					= rhs.m_CreatorId;
		m_HistoryId					= rhs.m_HistoryId; 
		m_AssetHash					= rhs.m_AssetHash;
		m_s64AssetLen				= rhs.m_s64AssetLen;
		m_u16AssetType				= rhs.m_u16AssetType;
		m_LocationFlags				= rhs.m_LocationFlags;
		m_CreationTime				= rhs.m_CreationTime;
        m_ModifiedTime				= rhs.m_ModifiedTime;
		m_AssetTag					= rhs.m_AssetTag;
		m_AssetSendState			= rhs.m_AssetSendState;
        m_PlayPosition0to100000     = rhs.m_PlayPosition0to100000;
	}

	return *this;
}

//============================================================================
bool AssetBaseInfo::isValid( void )
{
	return ( VXFILE_TYPE_UNKNOWN != m_u16AssetType );
}

//============================================================================
bool AssetBaseInfo::isMine( void )
{
	return isValid() && ( m_CreatorId == VxGetMyOnlineId() );
}

//============================================================================
// generates unique id, assigns it to asset and returns reference to it
VxGUID& AssetBaseInfo::generateNewUniqueId( void )
{
	VxGUID::generateNewVxGUID( m_UniqueId );
	return m_UniqueId;
}

//============================================================================
bool AssetBaseInfo::getIsFileAsset( void )
{
	return (0 != (m_u16AssetType & ( eAssetTypePhoto | eAssetTypeAudio | eAssetTypeVideo | eAssetTypeDocument | eAssetTypeArchives | eAssetTypeExe | eAssetTypeOtherFiles	)) );
}

//============================================================================
bool AssetBaseInfo::hasFileName( void )
{
	return ( 0 != m_AssetName.length() ) && getIsFileAsset();
}

//============================================================================
bool AssetBaseInfo::needsHashGenerated( void )
{
	if( ( false == m_AssetHash.isHashValid() )
		&& hasFileName() )
	{
		return true;
	}

	return false;
}

//============================================================================
bool AssetBaseInfo::isDirectory( void )
{
	return ( VXFILE_TYPE_DIRECTORY & m_u16AssetType ) ? true : false;
}

//============================================================================
void AssetBaseInfo::setAssetName( const char * assetName )
{
	if (assetName)
		m_AssetName = assetName;
	else
		m_AssetName = "";
}

//============================================================================
void AssetBaseInfo::setAssetTag( const char * assetTag )
{
	if( assetTag )
		m_AssetTag = assetTag;
	else
		m_AssetTag = "";
}

//============================================================================
std::string AssetBaseInfo::getRemoteAssetName( void )
{
	std::string rmtAssetName("");
	std::string 	assetPath;
	RCODE rc = VxFileUtil::seperatePathAndFile(	getAssetName(),					
		assetPath,		
		rmtAssetName );	
	if( 0 != rc )
	{
		LogMsg( LOG_ERROR, "AssetBaseInfo::getRemoteAssetName failed error %d asset %s\n", rc, getAssetName().c_str() );
	}

	return rmtAssetName;
}

//============================================================================
const char * AssetBaseInfo::getDefaultFileExtension( EAssetType assetType )
{
	const char * extension = ".txt";
	switch( assetType )
	{
	case eAssetTypePhoto:
		extension = ".jpg";
		break;

	case eAssetTypeAudio:
		extension = ".opus";
		break;

	case eAssetTypeVideo:
		extension = ".avi";
		break;

	default:
		break;
	}

	return extension;
}

//============================================================================
const char * AssetBaseInfo::getSubDirectoryName( EAssetType assetType )
{
	const char * subDir = "asset/";
	switch( assetType )
	{
	case eAssetTypeDocument:
		subDir = "documents/";
		break;

	case eAssetTypeArchives:
		subDir = "archives/";
		break;

	case eAssetTypeOtherFiles:
		subDir = "other_files/";
		break;

	case eAssetTypePhoto:
		subDir = "photos/";
		break;

	case eAssetTypeAudio:
		subDir = "audio/";
		break;

	case eAssetTypeVideo:
		subDir = "video/";
		break;

	default:
		break;
	}

	return subDir;
}

//============================================================================
void AssetBaseInfo::updateAssetInfo( VxThread * callingThread )
{
	return;
}
