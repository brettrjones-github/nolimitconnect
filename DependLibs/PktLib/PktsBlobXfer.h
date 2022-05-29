#pragma once
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

#include "VxCommon.h"
#include "VxPktHdr.h"
#include "PktRequestErrors.h"

#include <CoreLib/IsBigEndianCpu.h>
#include <CoreLib/VxSha1Hash.h>

#define PKT_TYPE_BLOB_MAX_DATA_LEN		    14320	            // maximum length of chunk of Blob data
#define PKT_TYPE_BLOB_MAX_NAME_AND_TAG_LEN	( 4096 * 2 + 48 )	// maximum length of name and tag

#pragma pack(push)
#pragma pack(1)

//============================================================================
// Blob Offer Packets
//============================================================================
enum EBlobXferCmd
{
	eBlobXferCmdBlobSend		    = 0,
	eBlobXferCmdBlobGet			    = 1,
	eBlobXferCmdBlobChecksum	    = 2,
	eBlobXferCmdBlobDirectory	    = 3
};

enum EBlobXferOption
{
	eBlobXferOptionReplaceIfExists 	= 0,
	eBlobXferOptionResumeIfExists	= 1,
	eBlobXferOptionFailIfExists		= 2
};

class BlobInfo;

class PktBlobSendReq : public VxPktHdr
{
public:
	PktBlobSendReq();
	void						setBlobNameAndTag( const char * pBlobName, const char * assetTag = 0 );
	void						setBlobNameLen( uint16_t nameLen )				{ m_BlobNameLen = htons( nameLen ); }
	uint16_t					getBlobNameLen( void )							{ return ntohs( m_BlobNameLen ); }
	void						setBlobTagLen( uint16_t tagLen )				{ m_BlobTagLen = htons( tagLen ); }
	uint16_t					getBlobTagLen( void )							{ return ntohs( m_BlobTagLen ); }

	std::string					getBlobName();
	std::string					getBlobTag();
	void						setBlobHashId( VxSha1Hash& BlobHashId )		    { m_BlobHashId = BlobHashId; }
	VxSha1Hash&					getBlobHashId( void )							{ return m_BlobHashId; }

	void						setLclSessionId( VxGUID& lclId )				{ m_LclSessionId = lclId; }
	VxGUID&						getLclSessionId( void )							{ return m_LclSessionId; }
	void						setRmtSessionId( VxGUID& rmtId )				{ m_RmtSessionId = rmtId; }
	VxGUID&						getRmtSessionId( void )							{ return m_RmtSessionId; }

	void						setUniqueId( VxGUID& uniqueId )					{ m_UniqueId = uniqueId; }
	VxGUID&						getUniqueId( void )								{ return m_UniqueId; }
	void						setCreatorId( VxGUID& creatorId )				{ m_CreatorId = creatorId; }
	VxGUID&						getCreatorId( void )							{ return m_CreatorId; }
	void						setHistoryId( VxGUID& historyId )				{ m_HistoryId = historyId; }
	VxGUID&						getHistoryId( void )							{ return m_HistoryId; }
    void						setThumbId( VxGUID& historyId )				    { m_ThumbId = historyId; }
    VxGUID&						getThumbId( void )							    { return m_ThumbId; }

    void						setCreationTime( int64_t createTime )			{ m_CreationTime = htonU64( createTime ); }
    int64_t					    getCreationTime( void )						    { return ntohU64( m_CreationTime ); }
    void						setModifiedTime( int64_t createTime )			{ m_ModifiedTime = htonU64( createTime ); }
    int64_t					    getModifiedTime( void )						    { return ntohU64( m_ModifiedTime ); }
    void						setAccessedTime( int64_t createTime )			{ m_AccessedTime = htonU64( createTime ); }
    int64_t					    getAccessedTime( void )						    { return ntohU64( m_AccessedTime ); }

	void						setBlobOffset( int64_t offset )				    { m_s64BlobOffs = htonU64( offset ); }
	int64_t						getBlobOffset( void )						    { return ntohU64( m_s64BlobOffs ); }
	void						setBlobLen( int64_t len )					    { m_s64BlobLen = htonU64( len ); }
	int64_t						getBlobLen( void )							    { return ntohU64( m_s64BlobLen ); }

	void						setBlobType( uint16_t BlobType )		        { m_BlobType = htons( BlobType ); }
	uint16_t					getBlobType( void )							    { return ntohs( m_BlobType ); }

    void						setAttributeFlags( uint16_t attribFlages )		{ m_AttributeFlags = htons( attribFlages ); }
    uint16_t					getAttributeFlags( void )					    { return ntohs( m_AttributeFlags ); }

	void						setError( uint32_t error )						{ m_u32Error = htonl( error ); }
	uint32_t					getError( void )								{ return ntohl( m_u32Error ); }

	void						fillPktFromBlob( BlobInfo& assetInfo );
	void						fillBlobFromPkt( BlobInfo& assetInfo );

private:
    uint16_t					m_BlobType{ 0 };
    uint16_t					m_AttributeFlags{ 0 };
	VxGUID						m_UniqueId;
	VxGUID						m_CreatorId;
	VxGUID						m_HistoryId; 
    VxGUID						m_ThumbId; 
	VxGUID						m_LclSessionId;
	VxGUID						m_RmtSessionId;
	VxSha1Hash					m_BlobHashId;
    uint32_t					m_u32Error{ 0 };
	int64_t						m_s64BlobLen{ 0 };
	int64_t						m_s64BlobOffs{ 0 };
	uint64_t					m_CreationTime{ 0 };
    uint64_t					m_ModifiedTime{ 0 };
    uint64_t					m_AccessedTime{ 0 };
    uint32_t					m_BlobAttribue{ 0 };
	uint16_t					m_BlobNameLen{ 0 };
	uint16_t					m_BlobTagLen{ 0 };

	uint32_t					m_u32Res1{ 0 };
	uint32_t					m_u32Res2{ 0 }; 
	uint32_t					m_u32Res3{ 0 };
	uint32_t					m_u32Res4{ 0 };
	char						m_BlobNameAndTag[ PKT_TYPE_BLOB_MAX_NAME_AND_TAG_LEN ];
};

class PktBlobSendReply : public VxPktHdr
{
public:
	PktBlobSendReply();

	void						setUniqueId( VxGUID& uniqueId )					{ m_UniqueId = uniqueId; }
	VxGUID&						getUniqueId( void )								{ return m_UniqueId; }

	void						setLclSessionId( VxGUID& lclId )				{ m_LclSessionId = lclId; }
	VxGUID&						getLclSessionId( void )							{ return m_LclSessionId; }
	void						setRmtSessionId( VxGUID& rmtId )				{ m_RmtSessionId = rmtId; }
	VxGUID&						getRmtSessionId( void )							{ return m_RmtSessionId; }

	void						setBlobOffset( int64_t offset )				    { m_s64BlobOffs = htonU64( offset ); }
	int64_t						getBlobOffset( void )						    { return ntohU64( m_s64BlobOffs ); }

	void						setError( uint32_t error )						{ m_u32Error = htonl( error ); }
	uint32_t					getError( void )								{ return ntohl( m_u32Error ); }

	void						setRequiresFileXfer( bool requiresXfer )		{ m_u8RequiresFileXfer = (uint8_t)requiresXfer; }
	bool						getRequiresFileXfer( void )						{ return m_u8RequiresFileXfer ? true : false; }

private:
	uint8_t						m_u8RequiresFileXfer{ 0 };
	uint8_t						m_u8Res{ 0 };
	uint16_t					m_u16Res{ 0 };
	int64_t						m_s64BlobOffs{ 0 };
	VxGUID						m_UniqueId;
	VxGUID						m_LclSessionId;
	VxGUID						m_RmtSessionId;
	uint32_t					m_u32Error{ 0 };
	uint32_t					m_u32Res1{ 0 };
	uint32_t					m_u32Res2{ 0 };
};

//============================================================================
// Blob chunk packets
//============================================================================
class PktBlobChunkReq : public VxPktHdr
{
public:
	PktBlobChunkReq();
	uint16_t					emptyLength( void );
	void						setChunkLen( uint16_t u16ChunkLen ); // also calculates packet length
	uint16_t					getChunkLen( void );

	void						setLclSessionId( VxGUID& lclId )				{ m_LclSessionId = lclId; }
	VxGUID&						getLclSessionId( void )							{ return m_LclSessionId; }
	void						setRmtSessionId( VxGUID& rmtId )				{ m_RmtSessionId = rmtId; }
	VxGUID&						getRmtSessionId( void )							{ return m_RmtSessionId; }

	void						setDataLen( uint16_t len )						{ m_u16BlobChunkLen = htons( len ); }
	uint16_t					getDataLen( void )								{ return ntohs( m_u16BlobChunkLen ); }

	void						setError( uint32_t error )						{ m_u32Error = htonl( error ); }
	uint32_t					getError( void )								{ return ntohl( m_u32Error ); }

private:
	uint16_t					m_u16Res{ 0 };
	uint16_t					m_u16BlobChunkLen{ 0 };
	VxGUID						m_LclSessionId;
	VxGUID						m_RmtSessionId;
	uint32_t					m_u32Error{ 0 };
	uint32_t					m_u32Res1{ 0 };
public:
	uint8_t						m_au8BlobChunk[ PKT_TYPE_BLOB_MAX_DATA_LEN ];
};

class PktBlobChunkReply : public VxPktHdr
{
public:
	PktBlobChunkReply();

	void						setLclSessionId( VxGUID& lclId )				{ m_LclSessionId = lclId; }
	VxGUID&						getLclSessionId( void )							{ return m_LclSessionId; }
	void						setRmtSessionId( VxGUID& rmtId )				{ m_RmtSessionId = rmtId; }
	VxGUID&						getRmtSessionId( void )							{ return m_RmtSessionId; }

	void						setDataLen( uint16_t len )						{ m_u16BlobChunkLen = htons( len ); }
	uint16_t					getDataLen( void )								{ return ntohs( m_u16BlobChunkLen ); }

	void						setError( uint32_t error )						{ m_u32Error = htonl( error ); }
	uint32_t					getError( void )								{ return ntohl( m_u32Error ); }

private:
	uint16_t					m_u16Res{ 0 };
	uint16_t					m_u16BlobChunkLen{ 0 };
	VxGUID						m_LclSessionId;
	VxGUID						m_RmtSessionId;
	uint32_t					m_u32Error{ 0 };
};

//============================================================================
// PktBlobSendComplete
//============================================================================
class PktBlobSendCompleteReq : public VxPktHdr
{
public:
	PktBlobSendCompleteReq();

	void						setLclSessionId( VxGUID& lclId )			{ m_LclSessionId = lclId; }
	VxGUID&						getLclSessionId( void )						{ return m_LclSessionId; }
	void						setRmtSessionId( VxGUID& rmtId )			{ m_RmtSessionId = rmtId; }
	VxGUID&						getRmtSessionId( void )						{ return m_RmtSessionId; }

	void						setBlobUniqueId( VxGUID& uniqueId  )		{ m_BlobUniqueId = uniqueId; }
	VxGUID&						getBlobHashId( void )						{ return m_BlobUniqueId; }

	void						setError( uint32_t error )					{ m_u32Error = htonl( error ); }
	uint32_t					getError( void )							{ return ntohl( m_u32Error ); }

private:
	VxGUID						m_LclSessionId;
	VxGUID						m_RmtSessionId;
	VxGUID						m_BlobUniqueId;
	uint32_t					m_u32Error{ 0 };
	uint32_t					m_u32Res1{ 0 };
};

class PktBlobSendCompleteReply : public VxPktHdr
{
public:
	PktBlobSendCompleteReply();

	void						setLclSessionId( VxGUID& lclId )			{ m_LclSessionId = lclId; }
	VxGUID&						getLclSessionId( void )						{ return m_LclSessionId; }
	void						setRmtSessionId( VxGUID& rmtId )			{ m_RmtSessionId = rmtId; }
	VxGUID&						getRmtSessionId( void )						{ return m_RmtSessionId; }

	void						setBlobUniqueId( VxGUID& uniqueId  )	    { m_BlobUniqueId = uniqueId; }
	VxGUID&						getAssetUniqueId( void )					    { return m_BlobUniqueId; }

	void						setError( uint32_t error )					{ m_u32Error = htonl( error ); }
	uint32_t					getError( void )							{ return ntohl( m_u32Error ); }

private:
	VxGUID						m_LclSessionId;
	VxGUID						m_RmtSessionId;
	VxGUID						m_BlobUniqueId;
	uint32_t					m_u32Error{ 0 };
	uint32_t					m_u32Res1{ 0 };
};

//============================================================================
// PktBlobXferErr
//============================================================================
class PktBlobXferErr : public VxPktHdr
{
public:
	PktBlobXferErr();

	static const char *			describeError( uint16_t error );

	void						setRxInstance( VxGUID& instanceGuid )			{ m_RxBlobInstance = instanceGuid; }
	VxGUID& 					getRxInstance( void )							{ return m_RxBlobInstance; }
	void						setTxInstance( VxGUID& instanceGuid )			{ m_TxBlobInstance = instanceGuid; }
	VxGUID&						getTxInstance( void )							{ return m_TxBlobInstance; }

	void						setError( uint16_t error )						{ m_u16Err = htons( error ); }
	uint16_t					getError( void )								{ return ntohs( m_u16Err ); }

private:
	uint16_t					m_u16Err{ 0xffff };
	uint16_t					m_u16Res1{ 0 };
	uint32_t					m_u32ResP1{ 0 };
	uint32_t					m_u32ResP2{ 0 };
	uint32_t					m_u32ResP3{ 0 };

	VxGUID						m_RxBlobInstance; 
	VxGUID						m_TxBlobInstance; 
	uint32_t					m_u32Res1{ 0 };
	uint32_t					m_u32Res2{ 0 };
};

#pragma pack(pop) 


