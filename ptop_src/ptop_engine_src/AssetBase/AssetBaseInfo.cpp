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
// http://www.nolimitconnect.org
//============================================================================

#include <config_appcorelibs.h>
#include "AssetBaseInfo.h"

#include <ptop_src/ptop_engine_src/P2PEngine/P2PEngine.h>

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
    : BaseInfo()
{ 
}

//============================================================================
AssetBaseInfo::AssetBaseInfo( EAssetType assetType )
    : BaseInfo()
    , m_u16AssetType( (uint16_t) assetType )
{ 
}

//============================================================================
AssetBaseInfo::AssetBaseInfo( const AssetBaseInfo& rhs )
: BaseInfo( rhs )
, m_AssetName( rhs.m_AssetName )
, m_AssetTag( rhs.m_AssetTag )
, m_UniqueId( rhs.m_UniqueId )
, m_HistoryId( rhs.m_HistoryId )
, m_AssetHash( rhs.m_AssetHash )
, m_s64AssetLen( rhs.m_s64AssetLen )
, m_u16AssetType( rhs.m_u16AssetType )
, m_AttributeFlags( rhs.m_AttributeFlags )
, m_LocationFlags( rhs.m_LocationFlags )
, m_CreationTime( rhs.m_CreationTime )
, m_AccessedTime( rhs.m_AccessedTime ) 
, m_ExpiresTime( rhs.m_ExpiresTime )
, m_AssetSendState( rhs.m_AssetSendState )
, m_PlayPosition0to100000( rhs.m_PlayPosition0to100000 )
{   
	generateNewUniqueId( true );
}

//============================================================================
AssetBaseInfo::AssetBaseInfo( EAssetType assetType, VxGUID& creatorId, int64_t modifiedTime )
: BaseInfo( creatorId, modifiedTime )
, m_u16AssetType( (uint16_t) assetType )
, m_CreationTime( modifiedTime ? modifiedTime : GetTimeStampMs() )
, m_AccessedTime( m_CreationTime )
{
	generateNewUniqueId( true );
}

//============================================================================
AssetBaseInfo::AssetBaseInfo(EAssetType assetType,  VxGUID& creatorId, VxGUID& assetId, int64_t modifiedTime )
: BaseInfo( creatorId, modifiedTime )
, m_UniqueId( assetId )
, m_u16AssetType( (uint16_t) assetType )
, m_CreationTime( modifiedTime ? modifiedTime : GetTimeStampMs() )
, m_AccessedTime( m_CreationTime )
{
	generateNewUniqueId( true );
}

//============================================================================
AssetBaseInfo::AssetBaseInfo( EAssetType assetType, const std::string& fileName )
: BaseInfo()
, m_AssetName( fileName )
, m_u16AssetType( (uint16_t) assetType )
, m_CreationTime( GetTimeStampMs() )
, m_AccessedTime( m_CreationTime )
{ 
    BaseInfo::setInfoModifiedTime( m_CreationTime );
	generateNewUniqueId( true );
}

//============================================================================
AssetBaseInfo::AssetBaseInfo( EAssetType assetType, const std::string& fileName, VxGUID& assetId )
	: BaseInfo()
	, m_AssetName( fileName )
    , m_UniqueId( assetId )
    , m_u16AssetType( (uint16_t)assetType )
	, m_CreationTime( GetTimeStampMs() )
	, m_AccessedTime( m_CreationTime )
{
	BaseInfo::setInfoModifiedTime( m_CreationTime );
	generateNewUniqueId( true );
}

//============================================================================
AssetBaseInfo::AssetBaseInfo( EAssetType assetType, const char* fileName, uint64_t fileLen )
: BaseInfo()
, m_AssetName( fileName )
, m_s64AssetLen( fileLen )
, m_u16AssetType( (uint16_t) assetType )
, m_CreationTime( GetTimeStampMs() )
, m_AccessedTime( m_CreationTime )
{
    BaseInfo::setInfoModifiedTime( m_CreationTime );
}

//============================================================================
AssetBaseInfo::AssetBaseInfo( EAssetType assetType, const char* fileName, uint64_t fileLen, VxGUID& assetId )
	: BaseInfo()
	, m_AssetName( fileName )
    , m_UniqueId( assetId )
	, m_s64AssetLen( fileLen )
    , m_u16AssetType( (uint16_t)assetType )
	, m_CreationTime( GetTimeStampMs() )
	, m_AccessedTime( m_CreationTime )

{
	BaseInfo::setInfoModifiedTime( m_CreationTime );
	generateNewUniqueId( true );
}

//============================================================================
AssetBaseInfo::AssetBaseInfo( EAssetType assetType, VxGUID& creatorId, VxGUID& assetId )
: BaseInfo( creatorId )
, m_UniqueId( assetId )
, m_u16AssetType( (uint16_t) assetType )
, m_CreationTime( GetTimeStampMs() )
, m_AccessedTime( m_CreationTime )
{
	generateNewUniqueId( true );
}

//============================================================================
bool AssetBaseInfo::validateAssetExist( void )
{
	bool exists = true;
	if( ( isFileAsset() || isThumbAsset() )  && !VxFileUtil::fileExists( m_AssetName.c_str() ) )
	{
		exists = false;
	}

	return exists;
}

//============================================================================
AssetBaseInfo& AssetBaseInfo::operator=( const AssetBaseInfo& rhs ) 
{	
	if( this != &rhs )
	{
        BaseInfo::operator=( rhs );
		m_AssetName					= rhs.m_AssetName;
		m_UniqueId					= rhs.m_UniqueId;
		m_HistoryId					= rhs.m_HistoryId; 
		m_AssetHash					= rhs.m_AssetHash;
		m_s64AssetLen				= rhs.m_s64AssetLen;
		m_u16AssetType				= rhs.m_u16AssetType;
        m_AttributeFlags			= rhs.m_AttributeFlags;
        m_LocationFlags				= rhs.m_LocationFlags;
		m_CreationTime				= rhs.m_CreationTime;
        m_AccessedTime              = rhs.m_AccessedTime;
        m_ExpiresTime               = rhs.m_ExpiresTime;
		m_AssetTag					= rhs.m_AssetTag;
		m_AssetSendState			= rhs.m_AssetSendState;
        m_PlayPosition0to100000     = rhs.m_PlayPosition0to100000;
	}

	return *this;
}

//============================================================================
bool AssetBaseInfo::isValid( void )
{
    vx_assert( ( VXFILE_TYPE_UNKNOWN != m_u16AssetType ) && m_UniqueId.isVxGUIDValid() && getCreatorId().isVxGUIDValid() && 0 != m_CreationTime && 0 != m_InfoModifiedTime );
	return ( VXFILE_TYPE_UNKNOWN != m_u16AssetType ) && m_UniqueId.isVxGUIDValid() && getCreatorId().isVxGUIDValid() && 0 != m_CreationTime && 0 != m_InfoModifiedTime;
}

//============================================================================
bool AssetBaseInfo::isValidFile( void )
{
	bool valid = !m_AssetName.empty() && m_s64AssetLen == VxFileUtil::fileExists( m_AssetName.c_str() );
	if( !valid )
	{
		LogMsg( LOG_ERROR, "AssetBaseInfo::isValidFile fail %lld %s ", m_s64AssetLen, m_AssetName.empty() ? "NO FILE NAME" : m_AssetName.c_str() );
		vx_assert( false );
	}

	return valid;
}

//============================================================================
bool AssetBaseInfo::isValidThumbnail( void )
{
    // dont use isValid.. thumbs may not have a creator id if is an emoticon
    bool valid = getAssetType() == eAssetTypeThumbnail && isValidFile() && m_InfoModifiedTime && m_UniqueId.isVxGUIDValid();
	if( !valid )
	{
		LogMsg( LOG_ERROR, "AssetBaseInfo::isValidThumbnail invalid " );
		vx_assert( false );
	}

	return valid;
}

//============================================================================
bool AssetBaseInfo::isMine( void )
{
	return isValid() && ( getCreatorId() == GetPtoPEngine().getMyOnlineId() );
}

//============================================================================
// generates unique id, assigns it to asset and returns reference to it
VxGUID& AssetBaseInfo::generateNewUniqueId( bool ifNotValid )
{
	if( !ifNotValid || ( ifNotValid && !m_UniqueId.isVxGUIDValid() ) )
	{
		VxGUID::generateNewVxGUID( m_UniqueId );
	}

	return m_UniqueId;
}

//============================================================================
bool AssetBaseInfo::isFileAsset( void )
{
	return (0 != ( m_u16AssetType & ( eAssetTypePhoto | eAssetTypeAudio | eAssetTypeVideo | eAssetTypeDocument | eAssetTypeArchives | eAssetTypeExe | eAssetTypeOtherFiles ) ) );
}

//============================================================================
bool AssetBaseInfo::hasFileName( void )
{
	return isFileAsset() || isThumbAsset();
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
void AssetBaseInfo::setAssetName( const char* assetName )
{
	if (assetName)
		m_AssetName = assetName;
	else
		m_AssetName = "";
}

//============================================================================
void AssetBaseInfo::setAssetTag( const char* assetTag )
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
const char* AssetBaseInfo::getDefaultFileExtension( EAssetType assetType )
{
	const char* extension = ".txt";
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
const char* AssetBaseInfo::getSubDirectoryName( EAssetType assetType )
{
	const char* subDir = "asset/";
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
void AssetBaseInfo::updateAssetInfo( VxThread* callingThread )
{
	return;
}

//============================================================================
void AssetBaseInfo::print( void ) const
{
	LogMsg( LOG_VERBOSE, "*Begin AssetBaseInfo" );
	LogMsg( LOG_VERBOSE, "m_AssetName=(%s)", m_AssetName.c_str() );
	LogMsg( LOG_VERBOSE, "m_UniqueId=(%s)", m_UniqueId.toOnlineIdString().c_str() );
	LogMsg( LOG_VERBOSE, "m_HistoryId=(%s)", m_HistoryId.toOnlineIdString().c_str() );
	LogMsg( LOG_VERBOSE, "m_AssetHash=(%s)", m_AssetHash.toString().c_str() );
	LogMsg( LOG_VERBOSE, "m_s64AssetLen=(%lld)", m_s64AssetLen );
	LogMsg( LOG_VERBOSE, "m_u16AssetType=(%d)", m_u16AssetType );
	LogMsg( LOG_VERBOSE, "m_AttributeFlags=(0x%4.4X)", m_AttributeFlags );
	LogMsg( LOG_VERBOSE, "m_AttributeFlags=(0x%8.8X)", m_LocationFlags );
	LogMsg( LOG_VERBOSE, "m_CreationTime=(%lld)", m_CreationTime );
	LogMsg( LOG_VERBOSE, "m_AccessedTime=(%lld)", m_AccessedTime );
	LogMsg( LOG_VERBOSE, "m_ExpiresTime=(%lld)", m_ExpiresTime );
	LogMsg( LOG_VERBOSE, "m_AssetTag=(%s)", m_AssetTag.c_str() );
	LogMsg( LOG_VERBOSE, "m_AssetSendState=(%d)", m_AssetSendState );
	LogMsg( LOG_VERBOSE, "m_PlayPosition0to100000=(%d)", m_PlayPosition0to100000 );
	LogMsg( LOG_VERBOSE, "*End AssetBaseInfo" );
}

//============================================================================
void AssetBaseInfo::updateAssetLength( int64_t assetLength )
{
	// TODO regenerate hash id
	m_s64AssetLen = assetLength;
}