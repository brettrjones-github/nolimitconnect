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

#include "PktsFileList.h"
#include "PktTypes.h"
#include "../CoreLib/VxDefs.h"

#include <CoreLib/IsBigEndianCpu.h>
#include <CoreLib/VxChop.h>

#include <memory.h>

#ifdef _MSC_VER
# pragma warning( disable: 4200 ) //warning C4200: nonstandard extension used : zero-sized array in struct/union
#endif // _MSC_VER

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
// file list request
//============================================================================

//============================================================================
PktFileListReq::PktFileListReq()
: m_u16Res(0)
, m_u32Index(0)
{
	setPktType( PKT_TYPE_FILE_LIST_REQ );
	setPktLength( sizeof( PktFileListReq ) );
}

//============================================================================
void PktFileListReq::setListIndex( uint32_t index )
{
	m_u32Index = htonl( index );
}

//============================================================================
uint32_t PktFileListReq::getListIndex( void )
{
	return ntohl( m_u32Index );
}

//============================================================================
// file list reply
//============================================================================

//============================================================================
PktFileListReply::PktFileListReply()
: m_bListComplete(0)
, m_u8Res1(0)
, m_u16Res(0)
, m_u16DataLen(0)
, m_u16FileCnt(0)
, m_u32Error(0)
, m_u32ListIndex(0)
{
	memset( m_as8FileList, 0, sizeof( m_as8FileList ) );
	setPktType( PKT_TYPE_FILE_LIST_REPLY );
	calcPktLen( 0 );
}

//============================================================================
void PktFileListReply::calcPktLen( uint16_t dataLen )
{
	m_u16DataLen = htons( dataLen );
	setPktLength( (uint16_t)ROUND_TO_16BYTE_BOUNDRY( sizeof( PktFileListReply ) - MAX_SHARED_FILE_LIST_LEN  + dataLen ) );
}

//============================================================================
void PktFileListReply::setError( uint32_t u32Error )
{
	m_u32Error = htonl( u32Error );
}

//============================================================================
uint32_t PktFileListReply::getError( void )
{
	 return ntohl( m_u32Error );
}

//============================================================================
void PktFileListReply::setFileCount( uint16_t fileCount )
{
	m_u16FileCnt = htons( fileCount );
}

//============================================================================
uint16_t PktFileListReply::getFileCount( void )
{
	return ntohs( m_u16FileCnt );
}

//============================================================================
void PktFileListReply::setListDataLen( uint16_t dataLen )
{
	m_u16DataLen = ntohs( dataLen );
}

//============================================================================
uint16_t PktFileListReply::getListDataLen( void )
{
	return ntohs( m_u16DataLen );
}

//============================================================================
void PktFileListReply::setListIndex( uint32_t index )
{
	m_u32ListIndex = htonl( index );
}

//============================================================================
uint32_t PktFileListReply::getListIndex( void )
{
	return ntohl( m_u32ListIndex );
}

//============================================================================
void PktFileListReply::getFileList( std::vector<VxFileInfo>& retFileList )
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
bool PktFileListReply::canAddFile( int fileNameLenIncludingZero )
{
	uint32_t curLen = getListDataLen();
	uint32_t addLen = sizeof( PackedFileListEntry ) - 260 + fileNameLenIncludingZero + 20;
	return ( curLen + addLen ) < MAX_SHARED_FILE_LIST_LEN;
}

//============================================================================
void PktFileListReply::addFile( VxSha1Hash& fileHashId, uint64_t fileLen, uint8_t fileTypeFlags, const char * fileName )
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
