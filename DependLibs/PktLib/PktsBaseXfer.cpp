//============================================================================
// Copyright (C) 2010 Brett R. Jones
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

#include "PktTypes.h"
#include "PktsBaseXfer.h"
#include <ptop_src/ptop_engine_src/AssetMgr/AssetInfo.h>
#include <ptop_src/ptop_engine_src/AssetBase/AssetBaseInfo.h>

#include <CoreLib/VxChop.h>
#include <CoreLib/VxDebug.h>
#include <CoreLib/VxParse.h>

#include <string.h>

namespace
{
    typedef struct
    {
        uint8_t						m_FileHashData[20];
        uint64_t					m_FileLen;
        uint16_t					m_FileNameLen;
        uint8_t						m_FileTypeFlags;
        char						m_FileName[260];	
    }PackedFileListEntry;
}

//============================================================================
//  PktBaseGetReq
//============================================================================

//============================================================================
PktBaseGetReply::PktBaseGetReply()
{ 
    m_AssetNameAndTag[0] = 0;
}

//============================================================================
bool PktBaseGetReply::fillPktFromAsset( AssetBaseInfo& assetInfo )
{
    setAssetType( (uint16_t)assetInfo.getAssetType() );
    setCreatorId( assetInfo.getCreatorId() );
    setHistoryId( assetInfo.getHistoryId() );
    setUniqueId( assetInfo.getAssetUniqueId() );
    setAssetHashId( assetInfo.getAssetHashId() );
    setAssetLen( assetInfo.getAssetLength() );
    setCreationTime( assetInfo.getCreationTime() );
    setModifiedTime( assetInfo.getModifiedTime() );

    setAssetNameAndTag( assetInfo.getAssetName().c_str(), assetInfo.getAssetTag().c_str() );
    return true;
}

//============================================================================
bool PktBaseGetReply::fillAssetFromPkt( AssetBaseInfo& assetInfo )
{
    assetInfo.setAssetType( (EAssetType)getAssetType() );
    assetInfo.setAssetUniqueId( getUniqueId() );
    assetInfo.setCreatorId( getCreatorId() );
    assetInfo.setHistoryId( getHistoryId() );
    assetInfo.setAssetHashId( getAssetHashId() );
    assetInfo.setAssetLength( getAssetLen() );
    assetInfo.setCreationTime( getCreationTime() );
    assetInfo.setModifiedTime( getModifiedTime() );

    std::string assetName = getAssetName();
    assetInfo.setAssetName( assetName );
    if( getAssetTagLen() )
    {
        assetInfo.setAssetTag(  &m_AssetNameAndTag[ getAssetNameLen() ] );
    }

    return true;
}

//============================================================================
std::string  PktBaseGetReply::getAssetName()
{
    return m_AssetNameAndTag;
}

//============================================================================
std::string  PktBaseGetReply::getAssetTag()
{
    std::string strTag = "";
    if( getAssetTagLen() )
    {
        strTag = &m_AssetNameAndTag[ getAssetNameLen() ];
    }

    return strTag;
}

//============================================================================
void  PktBaseGetReply::setAssetNameAndTag( const char * pAssetName, const char * pAssetTag )
{
    int nameLen = (int)strlen( pAssetName );
    if( nameLen > 4095 )
    {
        nameLen = 4095;
        strncpy( m_AssetNameAndTag, pAssetName, nameLen );
        m_AssetNameAndTag[ nameLen ] = 0;
    }
    else
    {
        strcpy( m_AssetNameAndTag, pAssetName );	
    }

    nameLen += 1;
    setAssetNameLen( (uint16_t)nameLen );

    int tagLen = 0;
    if( pAssetTag )
    {
        tagLen = (int)strlen( pAssetTag );
        if( tagLen > 0 )
        {
            if( tagLen > 4095 )
            {
                tagLen = 4095;
                strncpy( &m_AssetNameAndTag[nameLen], pAssetTag, tagLen );
                m_AssetNameAndTag[ nameLen = tagLen ] = 0;
            }
            else
            {
                strcpy( &m_AssetNameAndTag[nameLen], pAssetTag );	
            }

            tagLen += 1;
            setAssetTagLen( (uint16_t)tagLen );
        }
    }

    uint16_t u16PktLen = ( uint16_t )( ( sizeof(  PktBaseGetReply ) - sizeof( m_AssetNameAndTag ) ) + nameLen + tagLen );
    setPktLength( ROUND_TO_16BYTE_BOUNDRY( u16PktLen ) );
    vx_assert( 0 == ( 0x0F & getPktLength() ) );
}

//============================================================================
//  PktBaseSendReq
//============================================================================
 PktBaseSendReq::PktBaseSendReq()
{ 
	m_AssetNameAndTag[0] = 0;
}

//============================================================================
bool PktBaseSendReq::fillPktFromAsset( AssetBaseInfo& assetInfo )
{
	setAssetType( (uint16_t)assetInfo.getAssetType() );
	setCreatorId( assetInfo.getCreatorId() );
	setHistoryId( assetInfo.getHistoryId() );
	setUniqueId( assetInfo.getAssetUniqueId() );
	setAssetHashId( assetInfo.getAssetHashId() );
	setAssetLen( assetInfo.getAssetLength() );
    setCreationTime( assetInfo.getCreationTime() );
    setModifiedTime( assetInfo.getModifiedTime() );

	setAssetNameAndTag( assetInfo.getAssetName().c_str(), assetInfo.getAssetTag().c_str() );
    return true;
}

//============================================================================
bool PktBaseSendReq::fillAssetFromPkt( AssetBaseInfo& assetInfo )
{
	assetInfo.setAssetType( (EAssetType)getAssetType() );
	assetInfo.setAssetUniqueId( getUniqueId() );
	assetInfo.setCreatorId( getCreatorId() );
	assetInfo.setHistoryId( getHistoryId() );
	assetInfo.setAssetHashId( getAssetHashId() );
	assetInfo.setAssetLength( getAssetLen() );
    assetInfo.setCreationTime( getCreationTime() );
    assetInfo.setModifiedTime( getModifiedTime() );

	std::string assetName = getAssetName();
	assetInfo.setAssetName( assetName );
	if( getAssetTagLen() )
	{
		assetInfo.setAssetTag(  &m_AssetNameAndTag[ getAssetNameLen() ] );
	}

    return true;
}

//============================================================================
std::string  PktBaseSendReq::getAssetName()
{
	return m_AssetNameAndTag;
}

//============================================================================
std::string  PktBaseSendReq::getAssetTag()
{
	std::string strTag = "";
	if( getAssetTagLen() )
	{
		strTag = &m_AssetNameAndTag[ getAssetNameLen() ];
	}

	return strTag;
}

//============================================================================
void  PktBaseSendReq::setAssetNameAndTag( const char * pAssetName, const char * pAssetTag )
{
	int nameLen = (int)strlen( pAssetName );
	if( nameLen > 4095 )
	{
		nameLen = 4095;
		strncpy( m_AssetNameAndTag, pAssetName, nameLen );
		m_AssetNameAndTag[ nameLen ] = 0;
	}
	else
	{
		strcpy( m_AssetNameAndTag, pAssetName );	
	}

	nameLen += 1;
	setAssetNameLen( (uint16_t)nameLen );

	int tagLen = 0;
	if( pAssetTag )
	{
		tagLen = (int)strlen( pAssetTag );
		if( tagLen > 0 )
		{
			if( tagLen > 4095 )
			{
				tagLen = 4095;
				strncpy( &m_AssetNameAndTag[nameLen], pAssetTag, tagLen );
				m_AssetNameAndTag[ nameLen = tagLen ] = 0;
			}
			else
			{
				strcpy( &m_AssetNameAndTag[nameLen], pAssetTag );	
			}

			tagLen += 1;
			setAssetTagLen( (uint16_t)tagLen );
		}
	}

	uint16_t u16PktLen = ( uint16_t )( ( sizeof(  PktBaseSendReq ) - sizeof( m_AssetNameAndTag ) ) + nameLen + tagLen );
	setPktLength( ROUND_TO_16BYTE_BOUNDRY( u16PktLen ) );
    vx_assert( 0 == ( 0x0F & getPktLength() ) );
}

//============================================================================
 PktBaseSendReply::PktBaseSendReply()
{ 
}

//============================================================================
 PktBaseChunkReq::PktBaseChunkReq()
{
}

//============================================================================
uint16_t  PktBaseChunkReq::emptyLength( void )
{ 
	return (uint16_t)(sizeof(  PktBaseChunkReq) - PKT_TYPE_ASSET_MAX_DATA_LEN); 
}

//============================================================================
void  PktBaseChunkReq::setChunkLen( uint16_t u16ChunkLen ) 
{ 
	m_u16AssetChunkLen = htons( u16ChunkLen ); 
	setPktLength( ROUND_TO_16BYTE_BOUNDRY( emptyLength() + u16ChunkLen ) );
}

//============================================================================
uint16_t  PktBaseChunkReq::getChunkLen( void ) 
{ 
	return htons( m_u16AssetChunkLen ); 
}

//============================================================================
 PktBaseChunkReply::PktBaseChunkReply()
{
}

//============================================================================
 PktBaseSendCompleteReq::PktBaseSendCompleteReq()
{
}

//============================================================================
 PktBaseSendCompleteReply::PktBaseSendCompleteReply()
{
}

 //============================================================================
 PktBaseGetCompleteReq::PktBaseGetCompleteReq()
 {
 }

 //============================================================================
 PktBaseGetCompleteReply::PktBaseGetCompleteReply()
 {
 }

//============================================================================
 PktBaseXferErr:: PktBaseXferErr()
{
}

//============================================================================
const char *  PktBaseXferErr::describeError( void )
{
	switch( m_u16Err )
	{
	case PKT_REQ_STATUS_OK:
		return "200 Ok";
	case PKT_REQ_STATUS_CREATED:
		return "201 Created";
	case PKT_REQ_STATUS_ACCEPTED:
		return "202 Accepted";
	case PKT_REQ_ERR_NO_CONTENT:
		return "204 No Content";
	case PKT_REQ_ERR_MOVED_PERM:
		return "301 Moved Permanently";
	case PKT_REQ_ERR_MOVED_TEMP:
		return "301 Moved Temporarily";
	case PKT_REQ_ERR_NOT_MODIFIED:
		return "304 Not Modified";
	case PKT_REQ_ERR_BAD_REQUEST:
		return "400 Bad Request";
	case PKT_REQ_ERR_UNAUTHORIZED:
		return "401 Unauthorized";
	case PKT_REQ_ERR_FORBIDDEN:
		return "403 Forbidden";
	case PKT_REQ_ERR_NOT_FOUND:
		return "404 Not Found";
	case PKT_REQ_ERR_INTERNAL_SERVER_ERR:
		return "500 Internal Server Error";
	case PKT_REQ_ERR_NOT_IMPLEMENTED:
		return "501 Not Implemented";
	case PKT_REQ_ERR_BAD_GATEWAY:
		return "502 Bad Gateway";
	case PKT_REQ_ERR_SERVICE_UNAVAIL:
		return "503 Service Unavailable";
		//custom statuses
	case PKT_REQ_ERR_BANDWITH_LIMIT:
		return "600 Refused because of Bandwidth limit";
	case PKT_REQ_ERR_CONNECT_LIMIT:
		return "601 Refused because of Connection limit";
	case PKT_REQ_ERR_SERVICE_DISABLED:
		return "602	Refused because Service was disabled";
	case PKT_REQ_ERR_ALL_THREADS_BUSY:
		return "602	Refused because all threads are busy";
	default:
		return "Unknown Err";
	}
}

//============================================================================
PktBaseFindReq::PktBaseFindReq()
{
    m_MatchName[ 0 ] = 0;
}

//============================================================================
bool PktBaseFindReq::setMatchName( std::string &csName )
{
    if( ( PKT_BASE_FIND_FILE_MATCHNAME_MAX_LEN - 1) > csName.size() )
    {
        strcpy( m_MatchName, (const char *)csName.c_str() );
        setPktLength( ROUND_TO_16BYTE_BOUNDRY( getEmptyLen()  + csName.size() + 1 ) );
        return true;
    }
    else
    {
        m_MatchName[ 0 ] = 0; 
        setPktLength( getEmptyLen() );
        return false;
    }
}

//============================================================================
bool PktBaseFindReq::getMatchName( std::string &csName )
{
    if( getEmptyLen() >= getPktLength() )
    {
        vx_assert( false );
        csName = "";
        return false;
    }
    else
    {
        char as8Buf[ PKT_BASE_FIND_FILE_MATCHNAME_MAX_LEN ];
        VxUnchopStr( (unsigned char *)m_MatchName, as8Buf );
        csName = as8Buf;
        return true;
    }
}

//============================================================================
PktBaseListReply::PktBaseListReply()
{
    memset( m_as8FileList, 0, sizeof( m_as8FileList ) );
    calcPktLen( 0 );
}

//============================================================================
void PktBaseListReply::calcPktLen( uint16_t dataLen )
{
    setListDataLen( dataLen );
    setPktLength( (uint16_t)ROUND_TO_16BYTE_BOUNDRY( sizeof( PktBaseListReply ) - PKT_BASE_MAX_SHARED_FILE_LIST_LEN  + dataLen ) );
}

//============================================================================
void PktBaseListReply::getFileList( std::vector<VxFileInfo>& retFileList )
{
    uint16_t fileCnt = getFileCount();
    uint32_t dataOffset = 0;

    for( uint16_t cnt = 0; cnt < fileCnt; cnt++ )
    {
        PackedFileListEntry * entry = (PackedFileListEntry *)(&m_as8FileList[dataOffset]);

        //char as8Buf[ VX_MAX_PATH ];
        //VxUnchopStr( (unsigned char *)entry->m_FileName, as8Buf );
        VxFileInfo fileInfo( entry->m_FileName );
        fileInfo.setFileHashData( entry->m_FileHashData );
        fileInfo.setFileLength( ntohU64( entry->m_FileLen ) );
        fileInfo.setFileType( entry->m_FileTypeFlags );
        dataOffset += ROUND_TO_4BYTE_BOUNDRY( sizeof( PackedFileListEntry ) - 260 + htons( entry->m_FileNameLen ) );

        retFileList.push_back( fileInfo );
    }
}

//============================================================================
bool PktBaseListReply::canAddFile( int fileNameLenIncludingZero )
{
    uint32_t curLen = getListDataLen();
    uint32_t addLen = sizeof( PackedFileListEntry ) - 260 + fileNameLenIncludingZero + 20;
    return ( curLen + addLen ) < PKT_BASE_MAX_SHARED_FILE_LIST_LEN;
}

//============================================================================
void PktBaseListReply::addFile( VxSha1Hash& fileHashId, uint64_t fileLen, uint8_t fileTypeFlags, const char * fileName )
{
    uint32_t curLen = getListDataLen();
    PackedFileListEntry * entry = (PackedFileListEntry *)(&m_as8FileList[ curLen ]);
    memcpy( entry->m_FileHashData, fileHashId.getHashData(), 20 );
    entry->m_FileLen = htonU64( fileLen );
    entry->m_FileTypeFlags = fileTypeFlags;
    //(uint16_t)VxChopStr( fileName, (unsigned char *)entry->m_FileName );
    uint16_t choppedLen = (uint16_t)(strlen( fileName ) + 1);
    strcpy( entry->m_FileName, fileName );
    entry->m_FileNameLen = htons( choppedLen );
    // if we do not align will sometimes get illegal alignment error
    uint16_t dataLen = (uint16_t)ROUND_TO_4BYTE_BOUNDRY( (curLen + sizeof( PackedFileListEntry ) - 260 + choppedLen ) );
    setListDataLen( dataLen );
    calcPktLen( dataLen );
    setFileCount( getFileCount() + 1 );
}
