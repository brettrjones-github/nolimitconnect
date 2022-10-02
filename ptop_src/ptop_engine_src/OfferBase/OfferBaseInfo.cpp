//============================================================================
// Copyright (C) 2021 Brett R. Jones
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
#include "OfferBaseInfo.h"

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
OfferBaseInfo::OfferBaseInfo()
{ 
}

//============================================================================
OfferBaseInfo::OfferBaseInfo( const OfferBaseInfo& rhs )
{
	*this = rhs;
}

//============================================================================
OfferBaseInfo::OfferBaseInfo( const std::string& fileName )
: m_OfferName( fileName )
{ 
}

//============================================================================
OfferBaseInfo::OfferBaseInfo( const char* fileName, uint64_t fileLen, uint16_t assetType )
: m_OfferName( fileName )
, m_s64OfferLen( fileLen )
, m_u16OfferType( assetType )
, m_CreationTime( GetTimeStampMs() )
, m_ModifiedTime( m_CreationTime )
, m_AccessedTime( m_CreationTime )
{
}

//============================================================================
OfferBaseInfo& OfferBaseInfo::operator=( const OfferBaseInfo& rhs ) 
{	
	if( this != &rhs )
	{
		m_OfferName					= rhs.m_OfferName;
		m_OfferId					= rhs.m_OfferId;
		m_CreatorId					= rhs.m_CreatorId;
		m_HistoryId					= rhs.m_HistoryId; 
		m_OfferHash					= rhs.m_OfferHash;
		m_s64OfferLen				= rhs.m_s64OfferLen;
		m_u16OfferType				= rhs.m_u16OfferType;
        m_AttributeFlags			= rhs.m_AttributeFlags;
        m_LocationFlags				= rhs.m_LocationFlags;
		m_CreationTime				= rhs.m_CreationTime;
        m_ModifiedTime				= rhs.m_ModifiedTime;
		m_OfferTag					= rhs.m_OfferTag;
		m_OfferSendState			= rhs.m_OfferSendState;
        m_PlayPosition0to100000     = rhs.m_PlayPosition0to100000;
	}

	return *this;
}

//============================================================================
bool OfferBaseInfo::isValid( void )
{
    vx_assert( ( VXFILE_TYPE_UNKNOWN != m_u16OfferType ) && m_OfferId.isVxGUIDValid() && m_CreatorId.isVxGUIDValid() && 0 != m_CreationTime && 0 != m_ModifiedTime );
	return ( VXFILE_TYPE_UNKNOWN != m_u16OfferType ) && m_OfferId.isVxGUIDValid() && m_CreatorId.isVxGUIDValid() && 0 != m_CreationTime && 0 != m_ModifiedTime;
}

//============================================================================
bool OfferBaseInfo::isMine( void )
{
	return isValid() && ( m_CreatorId == GetPtoPEngine().getMyOnlineId() );
}

//============================================================================
// generates unique id, assigns it to asset and returns reference to it
VxGUID& OfferBaseInfo::generateNewOfferId( void )
{
	VxGUID::generateNewVxGUID( m_OfferId );
	return m_OfferId;
}

//============================================================================
bool OfferBaseInfo::isFileOffer( void )
{
	return (0 != (m_u16OfferType & ( eOfferTypePhotoFile | eOfferTypeAudioFile | eOfferTypeVideoFile | eOfferTypeDocumentFile | eOfferTypeArchiveFile | eOfferTypeExecutableFile | eOfferTypeOtherFile	)) );
}

//============================================================================
bool OfferBaseInfo::hasFileName( void )
{
	return ( 0 != m_OfferName.length() ) && isFileOffer();
}

//============================================================================
bool OfferBaseInfo::needsHashGenerated( void )
{
	if( ( false == m_OfferHash.isHashValid() )
		&& hasFileName() )
	{
		return true;
	}

	return false;
}

//============================================================================
bool OfferBaseInfo::isDirectory( void )
{
	return ( VXFILE_TYPE_DIRECTORY & m_u16OfferType ) ? true : false;
}

//============================================================================
void OfferBaseInfo::setOfferName( const char* assetName )
{
	if (assetName)
		m_OfferName = assetName;
	else
		m_OfferName = "";
}

//============================================================================
void OfferBaseInfo::setOfferTag( const char* assetTag )
{
	if( assetTag )
		m_OfferTag = assetTag;
	else
		m_OfferTag = "";
}

//============================================================================
std::string OfferBaseInfo::getRemoteOfferName( void )
{
	std::string rmtOfferName("");
	std::string 	assetPath;
	RCODE rc = VxFileUtil::seperatePathAndFile(	getOfferName(),					
		assetPath,		
		rmtOfferName );	
	if( 0 != rc )
	{
		LogMsg( LOG_ERROR, "OfferBaseInfo::getRemoteOfferName failed error %d asset %s\n", rc, getOfferName().c_str() );
	}

	return rmtOfferName;
}

//============================================================================
const char* OfferBaseInfo::getDefaultFileExtension( EOfferType assetType )
{
	const char* extension = ".txt";
	switch( assetType )
	{
	case eOfferTypePhotoFile:
		extension = ".jpg";
		break;

	case eOfferTypeAudioFile:
		extension = ".opus";
		break;

	case eOfferTypeVideoFile:
		extension = ".avi";
		break;

	default:
		break;
	}

	return extension;
}

//============================================================================
const char* OfferBaseInfo::getSubDirectoryName( EOfferType assetType )
{
	const char* subDir = "asset/";
	switch( assetType )
	{
	case eOfferTypeDocumentFile:
		subDir = "documents/";
		break;

	case eOfferTypeArchiveFile:
		subDir = "archives/";
		break;

	case eOfferTypeOtherFile:
		subDir = "other_files/";
		break;

	case eOfferTypePhotoFile:
		subDir = "photos/";
		break;

	case eOfferTypeAudioFile:
		subDir = "audio/";
		break;

	case eOfferTypeVideoFile:
		subDir = "video/";
		break;

	default:
		break;
	}

	return subDir;
}

//============================================================================
void OfferBaseInfo::updateOfferInfo( VxThread* callingThread )
{
	return;
}
