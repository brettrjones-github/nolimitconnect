#pragma once
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

#include "VxCommon.h"
#include "VxPktHdr.h"
#include "PktRequestErrors.h"

#include <CoreLib/IsBigEndianCpu.h>
#include <CoreLib/VxSha1Hash.h>

#define PKT_TYPE_ASSET_MAX_DATA_LEN		14320	// maximum length of chunk of Asset data
#define PKT_TYPE_ASSET_MAX_NAME_AND_TAG_LEN		( 4096 * 2 + 48 )	// maximum length of chunk of Asset data

#define MAX_ASSET_LIST_LEN				4096	// maximum length of list of Assets


#pragma pack(push) 
#pragma pack(1)

//============================================================================
// *** NOTE *** no virtual functions allowed in packets due to system dependant jump tables
//============================================================================

//============================================================================
// Asset Offer Packets
//============================================================================
enum EAssetXferCmd
{
	eAssetXferCmdAssetSend		    = 0,
	eAssetXferCmdAssetGet			= 1,
	eAssetXferCmdAssetChecksum	    = 2,
	eAssetXferCmdAssetDirectory	    = 3
};

enum EAssetXferOption
{
	eAssetXferOptionReplaceIfExists 	= 0,
	eAssetXferOptionResumeIfExists	    = 1,
	eAssetXferOptionFailIfExists		= 2
};

class AssetBaseInfo;


class PktBaseGetReq : public VxPktHdr
{
public:
    PktBaseGetReq() {};

    void						setAssetType( uint16_t AssetType )				    { m_AssetType = htons( AssetType ); }
    uint16_t					getAssetType( void )							    { return ntohs( m_AssetType ); }
    void						setUniqueId( VxGUID& uniqueId )					    { m_UniqueId = uniqueId; }
    VxGUID&						getUniqueId( void )								    { return m_UniqueId; }

    void						setFileHashId( VxSha1Hash& fileHashId )			    { m_FileHashId = fileHashId; }
    VxSha1Hash&					getFileHashId( void )							    { return m_FileHashId; }

    void						setLclSessionId( VxGUID& lclId )			        { m_LclSessionId = lclId; }
    VxGUID&						getLclSessionId( void )						        { return m_LclSessionId; }
    void						setRmtSessionId( VxGUID& rmtId )			        { m_RmtSessionId = rmtId; }
    VxGUID&						getRmtSessionId( void )						        { return m_RmtSessionId; }

    void						setStartOffset( int64_t offset )				    { m_s64StartOffs = htonU64( offset ); }
    int64_t						getStartOffset( void )							    { return ntohU64( m_s64StartOffs ); }
    void						setEndOffset( int64_t offset )					    { m_s64EndOffs = htonU64( offset ); }
    int64_t						getEndOffset( void )							    { return ntohU64( m_s64EndOffs ); }

private:
    uint16_t					m_AssetType{ 0 };
    VxGUID						m_UniqueId;
    VxGUID						m_LclSessionId;
    VxGUID						m_RmtSessionId;
    VxSha1Hash					m_FileHashId;
    int64_t						m_s64StartOffs{ 0 };
    int64_t						m_s64EndOffs{ 0 };	//if 0 then get all
    uint32_t					m_u32Res1{ 0 };
    uint32_t					m_u32Res2{ 0 };
};

class PktBaseGetReply : public VxPktHdr
{
public:
    PktBaseGetReply() {};

    void						setAssetType( uint16_t AssetType )				    { m_AssetType = htons( AssetType ); }
    uint16_t					getAssetType( void )							    { return ntohs( m_AssetType ); }
    void						setUniqueId( VxGUID& uniqueId )					    { m_UniqueId = uniqueId; }
    VxGUID&						getUniqueId( void )								    { return m_UniqueId; }

    void						setFileHashId( VxSha1Hash& fileHashId )			    { m_FileHashId = fileHashId; }
    VxSha1Hash&					getFileHashId( void )							    { return m_FileHashId; }

    void						setLclSessionId( VxGUID& lclId )			        { m_LclSessionId = lclId; }
    VxGUID&						getLclSessionId( void )						        { return m_LclSessionId; }
    void						setRmtSessionId( VxGUID& rmtId )			        { m_RmtSessionId = rmtId; }
    VxGUID&						getRmtSessionId( void )						        { return m_RmtSessionId; }

    void						setStartOffset( int64_t offset )					{ m_s64StartOffs = htonU64( offset ); }
    int64_t						getStartOffset( void )							    { return ntohU64( m_s64StartOffs ); }
    void						setEndOffset( int64_t offset )						{ m_s64EndOffs = htonU64( offset ); }
    int64_t						getEndOffset( void )							    { return ntohU64( m_s64EndOffs ); }

    void						setError( uint32_t error )							{ m_u32Error = htonl( error ); }
    uint32_t					getError( void )							        { return ntohl( m_u32Error ); }

private:
    uint16_t					m_AssetType{ 0 };
    VxGUID						m_UniqueId;
    VxGUID						m_LclSessionId;
    VxGUID						m_RmtSessionId;
    VxSha1Hash					m_FileHashId;
    int64_t						m_s64StartOffs{ 0 };
    int64_t						m_s64EndOffs{ 0 };	//if 0 then get all
    uint32_t					m_u32Error{ 0 };
    uint32_t					m_u32Res2{ 0 };
};


class PktBaseSendReq : public VxPktHdr
{
public:
	PktBaseSendReq();

    void						setAssetType( uint16_t AssetType )				{ m_AssetType = htons( AssetType ); }
    uint16_t					getAssetType( void )							{ return ntohs( m_AssetType ); }
    void						setUniqueId( VxGUID& uniqueId )					{ m_UniqueId = uniqueId; }
    VxGUID&						getUniqueId( void )								{ return m_UniqueId; }

	void						setAssetNameAndTag( const char * pAssetName, const char * assetTag = 0 );
	void						setAssetNameLen( uint16_t nameLen )				{ m_AssetNameLen = htons( nameLen ); }
	uint16_t					getAssetNameLen( void )							{ return ntohs( m_AssetNameLen ); }
	void						setAssetTagLen( uint16_t tagLen )				{ m_AssetTagLen = htons( tagLen ); }
	uint16_t					getAssetTagLen( void )							{ return ntohs( m_AssetTagLen ); }

	std::string					getAssetName();
	std::string					getAssetTag();
	void						setAssetHashId( VxSha1Hash& AssetHashId )		{ m_AssetHashId = AssetHashId; }
	VxSha1Hash&					getAssetHashId( void )							{ return m_AssetHashId; }

	void						setLclSessionId( VxGUID& lclId )				{ m_LclSessionId = lclId; }
	VxGUID&						getLclSessionId( void )							{ return m_LclSessionId; }
	void						setRmtSessionId( VxGUID& rmtId )				{ m_RmtSessionId = rmtId; }
	VxGUID&						getRmtSessionId( void )							{ return m_RmtSessionId; }

	void						setCreatorId( VxGUID& creatorId )				{ m_CreatorId = creatorId; }
	VxGUID&						getCreatorId( void )							{ return m_CreatorId; }
	void						setHistoryId( VxGUID& historyId )				{ m_HistoryId = historyId; }
	VxGUID&						getHistoryId( void )							{ return m_HistoryId; }

    void						setCreationTime( uint32_t createTime )			{ m_CreationTime = htonl( createTime ); }
	uint32_t					getCreationTime( void )						    { return ntohl( m_CreationTime ); }

	void						setAssetOffset( int64_t offset )				{ m_s64AssetOffs = htonU64( offset ); }
	int64_t						getAssetOffset( void )							{ return ntohU64( m_s64AssetOffs ); }
	void						setAssetLen( int64_t len )						{ m_s64AssetLen = htonU64( len ); }
	int64_t						getAssetLen( void )								{ return ntohU64( m_s64AssetLen ); }

	void						setError( uint32_t error )						{ m_u32Error = htonl( error ); }
	uint32_t					getError( void )								{ return ntohl( m_u32Error ); }

	void						fillPktFromAsset( AssetBaseInfo& assetInfo );
	void						fillAssetFromPkt( AssetBaseInfo& assetInfo );

private:
	uint16_t					m_AssetType{ 0 };
	VxGUID						m_UniqueId;
	VxGUID						m_CreatorId;
	VxGUID						m_HistoryId; 
	VxGUID						m_LclSessionId;
	VxGUID						m_RmtSessionId;
	VxSha1Hash					m_AssetHashId;
	uint32_t					m_u32Error{ 0 };
	int64_t						m_s64AssetLen{ 0 };
	int64_t						m_s64AssetOffs{ 0 };
	uint32_t					m_CreationTime{ 0 };
	uint16_t					m_AssetNameLen{ 0 };
	uint16_t					m_AssetTagLen{ 0 };

	uint32_t					m_u32Res1{ 0 };
	uint32_t					m_u32Res2{ 0 };
	uint32_t					m_u32Res3{ 0 }; 
	uint32_t					m_u32Res4{ 0 };
	char						m_AssetNameAndTag[ PKT_TYPE_ASSET_MAX_NAME_AND_TAG_LEN ];
};

class PktBaseSendReply : public VxPktHdr
{
public:
	PktBaseSendReply();

	void						setUniqueId( VxGUID& uniqueId )					{ m_UniqueId = uniqueId; }
	VxGUID&						getUniqueId( void )								{ return m_UniqueId; }

	void						setLclSessionId( VxGUID& lclId )				{ m_LclSessionId = lclId; }
	VxGUID&						getLclSessionId( void )							{ return m_LclSessionId; }
	void						setRmtSessionId( VxGUID& rmtId )				{ m_RmtSessionId = rmtId; }
	VxGUID&						getRmtSessionId( void )							{ return m_RmtSessionId; }

	void						setAssetOffset( int64_t offset )				{ m_s64AssetOffs = htonU64( offset ); }
	int64_t						getAssetOffset( void )							{ return ntohU64( m_s64AssetOffs ); }

	void						setError( uint32_t error )						{ m_u32Error = htonl( error ); }
	uint32_t					getError( void )								{ return ntohl( m_u32Error ); }

	void						setRequiresFileXfer( bool requiresXfer )		{ m_u8RequiresFileXfer = (uint8_t)requiresXfer; }
	bool						getRequiresFileXfer( void )						{ return m_u8RequiresFileXfer ? true : false; }

private:
	uint8_t						m_u8RequiresFileXfer{ 0 };
	uint8_t						m_u8Res{ 0 };
	uint16_t					m_u16Res{ 0 };
	int64_t						m_s64AssetOffs{ 0 };
	VxGUID						m_UniqueId;
	VxGUID						m_LclSessionId;
	VxGUID						m_RmtSessionId;
	uint32_t					m_u32Error{ 0 };
	uint32_t					m_u32Res1{ 0 };
	uint32_t					m_u32Res2{ 0 };
};

//============================================================================
// Asset chunk packets
//============================================================================
class PktBaseChunkReq : public VxPktHdr
{
public:
	PktBaseChunkReq();
	uint16_t					emptyLength( void );
	void						setChunkLen( uint16_t u16ChunkLen ); // also calculates packet length
	uint16_t					getChunkLen( void );

	void						setLclSessionId( VxGUID& lclId )				{ m_LclSessionId = lclId; }
	VxGUID&						getLclSessionId( void )							{ return m_LclSessionId; }
	void						setRmtSessionId( VxGUID& rmtId )				{ m_RmtSessionId = rmtId; }
	VxGUID&						getRmtSessionId( void )							{ return m_RmtSessionId; }

	void						setDataLen( uint16_t len )						{ m_u16AssetChunkLen = htons( len ); }
	uint16_t					getDataLen( void )								{ return ntohs( m_u16AssetChunkLen ); }

	void						setError( uint32_t error )						{ m_u32Error = htonl( error ); }
	uint32_t					getError( void )								{ return ntohl( m_u32Error ); }

private:
	uint16_t					m_u16Res{ 0 };
	uint16_t					m_u16AssetChunkLen{ 0 };
	VxGUID						m_LclSessionId;
	VxGUID						m_RmtSessionId;
	uint32_t					m_u32Error{ 0 };
	uint32_t					m_u32Res1{ 0 };
public:
	uint8_t						m_au8AssetChunk[ PKT_TYPE_ASSET_MAX_DATA_LEN ];
};

class PktBaseChunkReply : public VxPktHdr
{
public:
	PktBaseChunkReply();

	void						setLclSessionId( VxGUID& lclId )				{ m_LclSessionId = lclId; }
	VxGUID&						getLclSessionId( void )							{ return m_LclSessionId; }
	void						setRmtSessionId( VxGUID& rmtId )				{ m_RmtSessionId = rmtId; }
	VxGUID&						getRmtSessionId( void )							{ return m_RmtSessionId; }

	void						setDataLen( uint16_t len )						{ m_u16AssetChunkLen = htons( len ); }
	uint16_t					getDataLen( void )								{ return ntohs( m_u16AssetChunkLen ); }

	void						setError( uint32_t error )						{ m_u32Error = htonl( error ); }
	uint32_t					getError( void )								{ return ntohl( m_u32Error ); }

private:
	uint16_t					m_u16Res{ 0 };
	uint16_t					m_u16AssetChunkLen{ 0 };
	VxGUID						m_LclSessionId;
	VxGUID						m_RmtSessionId;
	uint32_t					m_u32Error{ 0 };
};

//============================================================================
// PktBaseSendComplete
//============================================================================
class PktBaseSendCompleteReq : public VxPktHdr
{
public:
	PktBaseSendCompleteReq();

	void						setLclSessionId( VxGUID& lclId )			{ m_LclSessionId = lclId; }
	VxGUID&						getLclSessionId( void )						{ return m_LclSessionId; }
	void						setRmtSessionId( VxGUID& rmtId )			{ m_RmtSessionId = rmtId; }
	VxGUID&						getRmtSessionId( void )						{ return m_RmtSessionId; }

	void						setAssetUniqueId( VxGUID& uniqueId  )		{ m_AssetUniqueId = uniqueId; }
	VxGUID&						getAssetHashId( void )						{ return m_AssetUniqueId; }

	void						setError( uint32_t error )					{ m_u32Error = htonl( error ); }
	uint32_t					getError( void )							{ return ntohl( m_u32Error ); }

private:
	VxGUID						m_LclSessionId;
	VxGUID						m_RmtSessionId;
	VxGUID						m_AssetUniqueId;
	uint32_t					m_u32Error{ 0 };
	uint32_t					m_u32Res1{ 0 };
};

class PktBaseSendCompleteReply : public VxPktHdr
{
public:
	PktBaseSendCompleteReply();

	void						setLclSessionId( VxGUID& lclId )			{ m_LclSessionId = lclId; }
	VxGUID&						getLclSessionId( void )						{ return m_LclSessionId; }
	void						setRmtSessionId( VxGUID& rmtId )			{ m_RmtSessionId = rmtId; }
	VxGUID&						getRmtSessionId( void )						{ return m_RmtSessionId; }

	void						setAssetUniqueId( VxGUID& uniqueId  )		{ m_AssetUniqueId = uniqueId; }
	VxGUID&						getAssetUniqueId( void )					{ return m_AssetUniqueId; }

	void						setError( uint32_t error )					{ m_u32Error = htonl( error ); }
	uint32_t					getError( void )							{ return ntohl( m_u32Error ); }

private:
	VxGUID						m_LclSessionId;
	VxGUID						m_RmtSessionId;
	VxGUID						m_AssetUniqueId;
	uint32_t					m_u32Error{ 0 };
	uint32_t					m_u32Res1{ 0 };
};

class PktBaseGetCompleteReq : public VxPktHdr
{
public:
    PktBaseGetCompleteReq();

    void						setLclSessionId( VxGUID& lclId )			{ m_LclSessionId = lclId; }
    VxGUID&						getLclSessionId( void )						{ return m_LclSessionId; }
    void						setRmtSessionId( VxGUID& rmtId )			{ m_RmtSessionId = rmtId; }
    VxGUID&						getRmtSessionId( void )						{ return m_RmtSessionId; }

    void						setAssetUniqueId( VxGUID& uniqueId  )		{ m_AssetUniqueId = uniqueId; }
    VxGUID&						getAssetUniqueId( void )					{ return m_AssetUniqueId; }

    void						setError( uint32_t error )					{ m_u32Error = htonl( error ); }
    uint32_t					getError( void )							{ return ntohl( m_u32Error ); }

private:
    VxGUID						m_LclSessionId;
    VxGUID						m_RmtSessionId;
    VxGUID						m_AssetUniqueId;
    uint32_t					m_u32Error{ 0 };
};

class PktBaseGetCompleteReply : public VxPktHdr
{
public:
    PktBaseGetCompleteReply();

    void						setLclSessionId( VxGUID& lclId )			{ m_LclSessionId = lclId; }
    VxGUID&						getLclSessionId( void )						{ return m_LclSessionId; }
    void						setRmtSessionId( VxGUID& rmtId )			{ m_RmtSessionId = rmtId; }
    VxGUID&						getRmtSessionId( void )						{ return m_RmtSessionId; }

    void						setAssetUniqueId( VxGUID& uniqueId  )		{ m_AssetUniqueId = uniqueId; }
    VxGUID&						getAssetUniqueId( void )					{ return m_AssetUniqueId; }

    void						setError( uint32_t error )					{ m_u32Error = htonl( error ); }
    uint32_t					getError( void )							{ return ntohl( m_u32Error ); }

private:
    VxGUID						m_LclSessionId;
    VxGUID						m_RmtSessionId;
    VxGUID						m_AssetUniqueId;
    uint32_t					m_u32Error{ 0 };
};

//============================================================================
// PktBaseXferErr
//============================================================================
class PktBaseXferErr : public VxPktHdr
{
public:
	PktBaseXferErr();

	const char *				describeError();

	void						setRxInstance( VxGUID& instanceGuid )			{ m_RxAssetInstance = instanceGuid; }
	VxGUID& 					getRxInstance( void )							{ return m_RxAssetInstance; }
	void						setTxInstance( VxGUID& instanceGuid )			{ m_TxAssetInstance = instanceGuid; }
	VxGUID&						getTxInstance( void )							{ return m_TxAssetInstance; }

	void						setError( uint16_t error )						{ m_u16Err = htons( error ); }
	uint16_t					getError( void )								{ return ntohs( m_u16Err ); }

private:
	uint16_t					m_u16Err{ 0xffff };
	uint16_t					m_u16Res1{ 0 };
	uint32_t					m_u32ResP1{ 0 };
	uint32_t					m_u32ResP2{ 0 };
	uint32_t					m_u32ResP3{ 0 };

	VxGUID						m_RxAssetInstance; 
	VxGUID						m_TxAssetInstance; 
	uint32_t					m_u32Res1{ 0 };
	uint32_t					m_u32Res2{ 0 };
};

#pragma pack(pop)

