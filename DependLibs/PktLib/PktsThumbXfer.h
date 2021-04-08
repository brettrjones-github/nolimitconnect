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
// http://www.nolimitconnect.com
//============================================================================

#include "VxCommon.h"
#include "VxPktHdr.h"
#include "PktRequestErrors.h"

#include <CoreLib/IsBigEndianCpu.h>
#include <CoreLib/VxSha1Hash.h>

#define PKT_TYPE_THUMB_MAX_DATA_LEN		    14320	            // maximum length of chunk of Thumb data
#define PKT_TYPE_THUMB_MAX_NAME_AND_TAG_LEN	( 4096 * 2 + 48 )	// maximum length of name and tag

#pragma pack(push) 
#pragma pack(1)

//============================================================================
// Thumb Offer Packets
//============================================================================
enum EThumbXferCmd
{
	eThumbXferCmdThumbSend		    = 0,
	eThumbXferCmdThumbGet			    = 1,
	eThumbXferCmdThumbChecksum	    = 2,
	eThumbXferCmdThumbDirectory	    = 3
};

enum EThumbXferOption
{
	eThumbXferOptionReplaceIfExists 	= 0,
	eThumbXferOptionResumeIfExists	= 1,
	eThumbXferOptionFailIfExists		= 2
};

class ThumbInfo;

class PktThumbSendReq : public VxPktHdr
{
public:
	PktThumbSendReq();
	void						setThumbNameAndTag( const char * pThumbName, const char * assetTag = 0 );
	void						setThumbNameLen( uint16_t nameLen )				{ m_ThumbNameLen = htons( nameLen ); }
	uint16_t					getThumbNameLen( void )							{ return ntohs( m_ThumbNameLen ); }
	void						setThumbTagLen( uint16_t tagLen )				{ m_ThumbTagLen = htons( tagLen ); }
	uint16_t					getThumbTagLen( void )							{ return ntohs( m_ThumbTagLen ); }

	std::string					getThumbName();
	std::string					getThumbTag();
	void						setThumbHashId( VxSha1Hash& ThumbHashId )		    { m_ThumbHashId = ThumbHashId; }
	VxSha1Hash&					getThumbHashId( void )							{ return m_ThumbHashId; }

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

	void						setThumbOffset( int64_t offset )				    { m_s64ThumbOffs = htonU64( offset ); }
	int64_t						getThumbOffset( void )						    { return ntohU64( m_s64ThumbOffs ); }
	void						setThumbLen( int64_t len )					    { m_s64ThumbLen = htonU64( len ); }
	int64_t						getThumbLen( void )							    { return ntohU64( m_s64ThumbLen ); }

	void						setThumbType( uint16_t ThumbType )		        { m_ThumbType = htons( ThumbType ); }
	uint16_t					getThumbType( void )							    { return ntohs( m_ThumbType ); }

    void						setAttributeFlags( uint16_t attribFlages )		{ m_AttributeFlags = htons( attribFlages ); }
    uint16_t					getAttributeFlags( void )					    { return ntohs( m_AttributeFlags ); }

	void						setError( uint32_t error )						{ m_u32Error = htonl( error ); }
	uint32_t					getError( void )								{ return ntohl( m_u32Error ); }

	void						fillPktFromThumb( ThumbInfo& assetInfo );
	void						fillThumbFromPkt( ThumbInfo& assetInfo );

private:
    uint16_t					m_ThumbType{ 0 };
    uint16_t					m_AttributeFlags{ 0 };
	VxGUID						m_UniqueId;
	VxGUID						m_CreatorId;
	VxGUID						m_HistoryId; 
    VxGUID						m_ThumbId; 
	VxGUID						m_LclSessionId;
	VxGUID						m_RmtSessionId;
	VxSha1Hash					m_ThumbHashId;
    uint32_t					m_u32Error{ 0 };
	int64_t						m_s64ThumbLen{ 0 };
	int64_t						m_s64ThumbOffs{ 0 };
	uint64_t					m_CreationTime{ 0 };
    uint64_t					m_ModifiedTime{ 0 };
    uint64_t					m_AccessedTime{ 0 };
    uint32_t					m_ThumbAttribue{ 0 };
	uint16_t					m_ThumbNameLen{ 0 };
	uint16_t					m_ThumbTagLen{ 0 };

	uint32_t					m_u32Res1{ 0 };
	uint32_t					m_u32Res2{ 0 }; 
	uint32_t					m_u32Res3{ 0 };
	uint32_t					m_u32Res4{ 0 };
	char						m_ThumbNameAndTag[ PKT_TYPE_THUMB_MAX_NAME_AND_TAG_LEN ];
};

class PktThumbSendReply : public VxPktHdr
{
public:
	PktThumbSendReply();

	void						setUniqueId( VxGUID& uniqueId )					{ m_UniqueId = uniqueId; }
	VxGUID&						getUniqueId( void )								{ return m_UniqueId; }

	void						setLclSessionId( VxGUID& lclId )				{ m_LclSessionId = lclId; }
	VxGUID&						getLclSessionId( void )							{ return m_LclSessionId; }
	void						setRmtSessionId( VxGUID& rmtId )				{ m_RmtSessionId = rmtId; }
	VxGUID&						getRmtSessionId( void )							{ return m_RmtSessionId; }

	void						setThumbOffset( int64_t offset )				    { m_s64ThumbOffs = htonU64( offset ); }
	int64_t						getThumbOffset( void )						    { return ntohU64( m_s64ThumbOffs ); }

	void						setError( uint32_t error )						{ m_u32Error = htonl( error ); }
	uint32_t					getError( void )								{ return ntohl( m_u32Error ); }

	void						setRequiresFileXfer( bool requiresXfer )		{ m_u8RequiresFileXfer = (uint8_t)requiresXfer; }
	bool						getRequiresFileXfer( void )						{ return m_u8RequiresFileXfer ? true : false; }

private:
	uint8_t						m_u8RequiresFileXfer{ 0 };
	uint8_t						m_u8Res{ 0 };
	uint16_t					m_u16Res{ 0 };
	int64_t						m_s64ThumbOffs{ 0 };
	VxGUID						m_UniqueId;
	VxGUID						m_LclSessionId;
	VxGUID						m_RmtSessionId;
	uint32_t					m_u32Error{ 0 };
	uint32_t					m_u32Res1{ 0 };
	uint32_t					m_u32Res2{ 0 };
};

//============================================================================
// Thumb chunk packets
//============================================================================
class PktThumbChunkReq : public VxPktHdr
{
public:
	PktThumbChunkReq();
	uint16_t					emptyLength( void );
	void						setChunkLen( uint16_t u16ChunkLen ); // also calculates packet length
	uint16_t					getChunkLen( void );

	void						setLclSessionId( VxGUID& lclId )				{ m_LclSessionId = lclId; }
	VxGUID&						getLclSessionId( void )							{ return m_LclSessionId; }
	void						setRmtSessionId( VxGUID& rmtId )				{ m_RmtSessionId = rmtId; }
	VxGUID&						getRmtSessionId( void )							{ return m_RmtSessionId; }

	void						setDataLen( uint16_t len )						{ m_u16ThumbChunkLen = htons( len ); }
	uint16_t					getDataLen( void )								{ return ntohs( m_u16ThumbChunkLen ); }

	void						setError( uint32_t error )						{ m_u32Error = htonl( error ); }
	uint32_t					getError( void )								{ return ntohl( m_u32Error ); }

private:
	uint16_t					m_u16Res{ 0 };
	uint16_t					m_u16ThumbChunkLen{ 0 };
	VxGUID						m_LclSessionId;
	VxGUID						m_RmtSessionId;
	uint32_t					m_u32Error{ 0 };
	uint32_t					m_u32Res1{ 0 };
public:
	uint8_t						m_au8ThumbChunk[ PKT_TYPE_THUMB_MAX_DATA_LEN ];
};

class PktThumbChunkReply : public VxPktHdr
{
public:
	PktThumbChunkReply();

	void						setLclSessionId( VxGUID& lclId )				{ m_LclSessionId = lclId; }
	VxGUID&						getLclSessionId( void )							{ return m_LclSessionId; }
	void						setRmtSessionId( VxGUID& rmtId )				{ m_RmtSessionId = rmtId; }
	VxGUID&						getRmtSessionId( void )							{ return m_RmtSessionId; }

	void						setDataLen( uint16_t len )						{ m_u16ThumbChunkLen = htons( len ); }
	uint16_t					getDataLen( void )								{ return ntohs( m_u16ThumbChunkLen ); }

	void						setError( uint32_t error )						{ m_u32Error = htonl( error ); }
	uint32_t					getError( void )								{ return ntohl( m_u32Error ); }

private:
	uint16_t					m_u16Res{ 0 };
	uint16_t					m_u16ThumbChunkLen{ 0 };
	VxGUID						m_LclSessionId;
	VxGUID						m_RmtSessionId;
	uint32_t					m_u32Error{ 0 };
};

//============================================================================
// PktThumbSendComplete
//============================================================================
class PktThumbSendCompleteReq : public VxPktHdr
{
public:
	PktThumbSendCompleteReq();

	void						setLclSessionId( VxGUID& lclId )			{ m_LclSessionId = lclId; }
	VxGUID&						getLclSessionId( void )						{ return m_LclSessionId; }
	void						setRmtSessionId( VxGUID& rmtId )			{ m_RmtSessionId = rmtId; }
	VxGUID&						getRmtSessionId( void )						{ return m_RmtSessionId; }

	void						setThumbUniqueId( VxGUID& uniqueId  )		{ m_ThumbUniqueId = uniqueId; }
	VxGUID&						getThumbHashId( void )						{ return m_ThumbUniqueId; }

	void						setError( uint32_t error )					{ m_u32Error = htonl( error ); }
	uint32_t					getError( void )							{ return ntohl( m_u32Error ); }

private:
	VxGUID						m_LclSessionId;
	VxGUID						m_RmtSessionId;
	VxGUID						m_ThumbUniqueId;
	uint32_t					m_u32Error{ 0 };
	uint32_t					m_u32Res1{ 0 };
};

class PktThumbSendCompleteReply : public VxPktHdr
{
public:
	PktThumbSendCompleteReply();

	void						setLclSessionId( VxGUID& lclId )			{ m_LclSessionId = lclId; }
	VxGUID&						getLclSessionId( void )						{ return m_LclSessionId; }
	void						setRmtSessionId( VxGUID& rmtId )			{ m_RmtSessionId = rmtId; }
	VxGUID&						getRmtSessionId( void )						{ return m_RmtSessionId; }

	void						setThumbUniqueId( VxGUID& uniqueId  )	    { m_ThumbUniqueId = uniqueId; }
	VxGUID&						getAssetUniqueId( void )					    { return m_ThumbUniqueId; }

	void						setError( uint32_t error )					{ m_u32Error = htonl( error ); }
	uint32_t					getError( void )							{ return ntohl( m_u32Error ); }

private:
	VxGUID						m_LclSessionId;
	VxGUID						m_RmtSessionId;
	VxGUID						m_ThumbUniqueId;
	uint32_t					m_u32Error{ 0 };
	uint32_t					m_u32Res1{ 0 };
};

//============================================================================
// PktThumbXferErr
//============================================================================
class PktThumbXferErr : public VxPktHdr
{
public:
	PktThumbXferErr();

	static const char *			describeError( uint16_t error );

	void						setRxInstance( VxGUID& instanceGuid )			{ m_RxThumbInstance = instanceGuid; }
	VxGUID& 					getRxInstance( void )							{ return m_RxThumbInstance; }
	void						setTxInstance( VxGUID& instanceGuid )			{ m_TxThumbInstance = instanceGuid; }
	VxGUID&						getTxInstance( void )							{ return m_TxThumbInstance; }

	void						setError( uint16_t error )						{ m_u16Err = htons( error ); }
	uint16_t					getError( void )								{ return ntohs( m_u16Err ); }

private:
	uint16_t					m_u16Err{ 0xffff };
	uint16_t					m_u16Res1{ 0 };
	uint32_t					m_u32ResP1{ 0 };
	uint32_t					m_u32ResP2{ 0 };
	uint32_t					m_u32ResP3{ 0 };

	VxGUID						m_RxThumbInstance; 
	VxGUID						m_TxThumbInstance; 
	uint32_t					m_u32Res1{ 0 };
	uint32_t					m_u32Res2{ 0 };
};

#pragma pack(pop)

