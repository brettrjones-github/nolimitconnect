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
#include <CoreLib/VxFileInfo.h>

#define PKT_TYPE_ASSET_MAX_DATA_LEN		            14320	            // maximum length of chunk of Asset data
#define PKT_TYPE_ASSET_MAX_NAME_AND_TAG_LEN		    ( 4096 * 2 + 48 )	// maximum length of chunk of Asset file name and tag

#define MAX_ASSET_LIST_LEN				            4096	            // maximum length of list of Assets

#define PKT_BASE_MAX_SHARED_FILE_LIST_LEN			12288
#define PKT_BASE_ERR_NO_SHARED_FILES				0x0001
#define PKT_BASE_ERR_FILE_LIST_IDX_OUT_OF_RANGE		0x0002
#define PKT_BASE_FIND_FILE_MATCHNAME_MAX_LEN        128

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

    void						setAssetOffset( int64_t offset )				    { m_s64AssetOffs = htonU64( offset ); }
    int64_t						getAssetOffset( void )							    { return ntohU64( m_s64AssetOffs ); }
    void						setAssetLen( int64_t len )						    { m_s64AssetLen = htonU64( len ); }
    int64_t						getAssetLen( void )								    { return ntohU64( m_s64AssetLen ); } // if 0 then get all

private:
    uint16_t					m_AssetType{ 0 };
    uint16_t					m_u16Res1{ 0 };
    VxGUID						m_UniqueId;
    VxGUID						m_LclSessionId;
    VxGUID						m_RmtSessionId;
    int64_t						m_s64AssetLen{ 0 };
    int64_t						m_s64AssetOffs{ 0 };	// if 0 then get all
    VxSha1Hash					m_FileHashId;

    int64_t						m_s64Res1{ 0 };
    uint32_t					m_u32Res1{ 0 };
    uint32_t					m_u32Res2{ 0 };
};

class PktBaseGetReply : public VxPktHdr
{
public:
    PktBaseGetReply();

    void						setAssetType( uint16_t AssetType )				    { m_AssetType = htons( AssetType ); }
    uint16_t					getAssetType( void )							    { return ntohs( m_AssetType ); }
    void						setUniqueId( VxGUID& uniqueId )					    { m_UniqueId = uniqueId; }
    VxGUID&						getUniqueId( void )								    { return m_UniqueId; }

    void						setAssetNameAndTag( const char * pAssetName, const char * assetTag = 0 );
    void						setAssetNameLen( uint16_t nameLen )				    { m_AssetNameLen = htons( nameLen ); }
    uint16_t					getAssetNameLen( void )							    { return ntohs( m_AssetNameLen ); }
    void						setAssetTagLen( uint16_t tagLen )				    { m_AssetTagLen = htons( tagLen ); }
    uint16_t					getAssetTagLen( void )							    { return ntohs( m_AssetTagLen ); }

    std::string					getAssetName();
    std::string					getAssetTag();
    void						setAssetHashId( VxSha1Hash& AssetHashId )		    { m_AssetHashId = AssetHashId; }
    VxSha1Hash&					getAssetHashId( void )							    { return m_AssetHashId; }

    void						setLclSessionId( VxGUID& lclId )			        { m_LclSessionId = lclId; }
    VxGUID&						getLclSessionId( void )						        { return m_LclSessionId; }
    void						setRmtSessionId( VxGUID& rmtId )			        { m_RmtSessionId = rmtId; }
    VxGUID&						getRmtSessionId( void )						        { return m_RmtSessionId; }

    void						setCreatorId( VxGUID& creatorId )				    { m_CreatorId = creatorId; }
    VxGUID&						getCreatorId( void )							    { return m_CreatorId; }
    void						setHistoryId( VxGUID& historyId )				    { m_HistoryId = historyId; }
    VxGUID&						getHistoryId( void )							    { return m_HistoryId; }

    void						setCreationTime( int64_t createTime )			    { m_CreationTime = htonU64( createTime ); }
    int64_t					    getCreationTime( void )						        { return ntohU64( m_CreationTime ); }
    void						setModifiedTime( int64_t modTime )			        { m_ModifiedTime = htonU64( modTime ); }
    int64_t					    getModifiedTime( void )						        { return ntohU64( m_ModifiedTime ); }

    void						setAssetOffset( int64_t offset )				    { m_s64AssetOffs = htonU64( offset ); }
    int64_t						getAssetOffset( void )							    { return ntohU64( m_s64AssetOffs ); }
    void						setAssetLen( int64_t len )						    { m_s64AssetLen = htonU64( len ); }
    int64_t						getAssetLen( void )								    { return ntohU64( m_s64AssetLen ); }

    void						setError( uint32_t error )						    { m_u32Error = htonl( error ); }
    uint32_t					getError( void )								    { return ntohl( m_u32Error ); }

    bool                        fillPktFromAsset( AssetBaseInfo& assetInfo );
    bool						fillAssetFromPkt( AssetBaseInfo& assetInfo );

private:
    uint16_t					m_AssetType{ 0 };
    uint16_t					m_u16Res1{ 0 };
    VxGUID						m_UniqueId;
    VxGUID						m_CreatorId;
    VxGUID						m_HistoryId; 
    VxGUID						m_LclSessionId;
    VxGUID						m_RmtSessionId;
    VxSha1Hash					m_AssetHashId;
    uint32_t					m_u32Error{ 0 };
    int64_t						m_s64AssetLen{ 0 };
    int64_t						m_s64AssetOffs{ 0 };
    int64_t					    m_CreationTime{ 0 };
    int64_t					    m_ModifiedTime{ 0 };

    uint16_t					m_AssetNameLen{ 0 };
    uint16_t					m_AssetTagLen{ 0 };

    VxSha1Hash					m_FileHashId;

    uint32_t					m_u32Res2{ 0 };
    int64_t	                    m_s64Res3{ 0 };
    int64_t	                    m_s64Res4{ 0 };
    char						m_AssetNameAndTag[ PKT_TYPE_ASSET_MAX_NAME_AND_TAG_LEN ];
};

class PktBaseSendReq : public VxPktHdr
{
public:
	PktBaseSendReq();

    void						setAssetType( uint16_t AssetType )				    { m_AssetType = htons( AssetType ); }
    uint16_t					getAssetType( void )							    { return ntohs( m_AssetType ); }
    void						setUniqueId( VxGUID& uniqueId )					    { m_UniqueId = uniqueId; }
    VxGUID&						getUniqueId( void )								    { return m_UniqueId; }

	void						setAssetNameAndTag( const char * pAssetName, const char * assetTag = 0 );
	void						setAssetNameLen( uint16_t nameLen )				    { m_AssetNameLen = htons( nameLen ); }
	uint16_t					getAssetNameLen( void )							    { return ntohs( m_AssetNameLen ); }
	void						setAssetTagLen( uint16_t tagLen )				    { m_AssetTagLen = htons( tagLen ); }
	uint16_t					getAssetTagLen( void )							    { return ntohs( m_AssetTagLen ); }

	std::string					getAssetName();
	std::string					getAssetTag();
	void						setAssetHashId( VxSha1Hash& AssetHashId )		    { m_AssetHashId = AssetHashId; }
	VxSha1Hash&					getAssetHashId( void )							    { return m_AssetHashId; }

	void						setLclSessionId( VxGUID& lclId )				    { m_LclSessionId = lclId; }
	VxGUID&						getLclSessionId( void )							    { return m_LclSessionId; }
	void						setRmtSessionId( VxGUID& rmtId )				    { m_RmtSessionId = rmtId; }
	VxGUID&						getRmtSessionId( void )							    { return m_RmtSessionId; }

	void						setCreatorId( VxGUID& creatorId )				    { m_CreatorId = creatorId; }
	VxGUID&						getCreatorId( void )							    { return m_CreatorId; }
	void						setHistoryId( VxGUID& historyId )				    { m_HistoryId = historyId; }
	VxGUID&						getHistoryId( void )							    { return m_HistoryId; }

    void						setCreationTime( int64_t createTime )			    { m_CreationTime = htonU64( createTime ); }
    int64_t					    getCreationTime( void )						        { return ntohU64( m_CreationTime ); }
    void						setModifiedTime( uint64_t modTime )			        { m_ModifiedTime = htonU64( modTime ); }
    int64_t					    getModifiedTime( void )						        { return ntohU64( m_ModifiedTime ); }

	void						setAssetOffset( int64_t offset )				    { m_s64AssetOffs = htonU64( offset ); }
	int64_t						getAssetOffset( void )							    { return ntohU64( m_s64AssetOffs ); }
	void						setAssetLen( int64_t len )						    { m_s64AssetLen = htonU64( len ); }
	int64_t						getAssetLen( void )								    { return ntohU64( m_s64AssetLen ); }

	void						setError( uint32_t error )						    { m_u32Error = htonl( error ); }
	uint32_t					getError( void )								    { return ntohl( m_u32Error ); }

	bool						fillPktFromAsset( AssetBaseInfo& assetInfo );
    bool						fillAssetFromPkt( AssetBaseInfo& assetInfo );

private:
	uint16_t					m_AssetType{ 0 };
    uint16_t					m_u16Res1{ 0 };
	VxGUID						m_UniqueId;
	VxGUID						m_CreatorId;
	VxGUID						m_HistoryId; 
	VxGUID						m_LclSessionId;
	VxGUID						m_RmtSessionId;
	VxSha1Hash					m_AssetHashId;
	uint32_t					m_u32Error{ 0 };
	int64_t						m_s64AssetLen{ 0 };
	int64_t						m_s64AssetOffs{ 0 };
    int64_t					    m_CreationTime{ 0 };
    int64_t					    m_ModifiedTime{ 0 };

	uint16_t					m_AssetNameLen{ 0 };
	uint16_t					m_AssetTagLen{ 0 };

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
    uint32_t					m_u32Error{ 0 };
	int64_t						m_s64AssetOffs{ 0 };
	VxGUID						m_UniqueId;
	VxGUID						m_LclSessionId;
	VxGUID						m_RmtSessionId;
    uint64_t					m_s64Res1{ 0 };
    uint64_t					m_s64Res2{ 0 };
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
    uint32_t					m_u32Error{ 0 };
	VxGUID						m_LclSessionId;
	VxGUID						m_RmtSessionId;
	uint64_t					m_s64Res1{ 0 };
    uint64_t					m_s64Res2{ 0 };
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
    uint32_t					m_u32Error{ 0 };
	VxGUID						m_LclSessionId;
	VxGUID						m_RmtSessionId;
    uint64_t					m_s64Res1{ 0 };
    uint64_t					m_s64Res2{ 0 };
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

//============================================================================
// File Find packets
//============================================================================
class PktBaseFindReq : public VxPktHdr
{
public:
    PktBaseFindReq();

    uint16_t					getEmptyLen( void ){ return (uint16_t)(sizeof( PktBaseFindReq )-PKT_BASE_FIND_FILE_MATCHNAME_MAX_LEN); };
    bool						setMatchName( std::string & csMatchName );
    bool						getMatchName( std::string & csRetMatchName );

    void						setLclSessionId( VxGUID& lclId )				{ m_LclSessionId = lclId; }
    VxGUID&						getLclSessionId( void )							{ return m_LclSessionId; }
    void						setRmtSessionId( VxGUID& rmtId )				{ m_RmtSessionId = rmtId; }
    VxGUID&						getRmtSessionId( void )							{ return m_RmtSessionId; }

    void						setFileHashId( VxSha1Hash& fileHashId )			{ m_FileHashId = fileHashId; }
    VxSha1Hash&					getFileHashId( void )							{ return m_FileHashId; }

    void						setFileLen( int64_t len )						{ m_s64FileLen = htonU64( len ); }
    int64_t						getFileLen( void )								{ return ntohU64( m_s64FileLen ); }

    void						setFileFlags( uint16_t flags )					{ m_u16FileFlags = htons( flags ); }
    uint16_t					getFileFlags( void )							{ return ntohs( m_u16FileFlags ); }
    void						setSizeLimitType( uint16_t type )				{ m_u16SizeLimitType = htons( type ); }
    uint16_t					getSizeLimitType( void )						{ return ntohs( m_u16SizeLimitType ); }

private:
    uint16_t					m_u16FileFlags{ 0 };		// types of file to match ( SEE FILE_TYPE_MASK )
    uint16_t					m_u16SizeLimitType{ 0 };	// type of file size limit 0=any size 1=At Least 2=At Most 3=Exactly
    uint32_t					m_u32ResP1{ 0 };			// reserved
    int64_t						m_s64FileLen{ 0 };		    // file size 
    VxGUID						m_LclSessionId;
    VxGUID						m_RmtSessionId;
    VxSha1Hash					m_FileHashId;
    uint32_t					m_u32Res1{ 0 }; 
    uint32_t					m_u32Res2{ 0 }; 
    char						m_MatchName[ PKT_BASE_FIND_FILE_MATCHNAME_MAX_LEN + 16 ];
};

class PktBaseListReq : public VxPktHdr
{
public:
    PktBaseListReq() = default;

    void						setListIndex( uint32_t index )                  { m_u32Index = htonl( index ); }
    uint32_t					getListIndex( void )                            { return ntohl( m_u32Index ); }

private:
    //=== vars ===//
    uint32_t					m_u32Index{ 0 };	
    uint32_t					m_u16Res{ 0 };
};

class PktBaseListReply : public VxPktHdr
{
public:
    PktBaseListReply();

    void						calcPktLen( uint16_t dataLen );

    void						setIsListCompleted( bool isCompleted )			{ m_bListComplete = isCompleted ? 1 : 0; }
    bool						getIsListCompleted( void )						{ return m_bListComplete ? true : false; }

    void						setError( uint32_t u32Error )			        { m_u32Error = htonl( u32Error ); }
    uint32_t					getError( void )						        { return  ntohl( m_u32Error ); }

    void						setListDataLen( uint16_t dataLen )			    { m_u16DataLen =  htons( dataLen ); }
    uint16_t					getListDataLen( void )		                    { return ntohs( m_u16DataLen ); }
    void						setFileCount( uint16_t fileCount )			    { m_u16FileCnt =  htons(fileCount ); }
    uint16_t					getFileCount( void )	                        { return ntohs( m_u16FileCnt ); }
    void						setListIndex( uint32_t index )			        { m_u32ListIndex = htonl( index ); }
    uint32_t					getListIndex( void )                            { return  ntohl( m_u32ListIndex ); }

    void						getFileList( std::vector<VxFileInfo>& retList );
    bool						canAddFile( int fileNameLenIncludingZero );
    void						addFile( VxSha1Hash& fileHashId, uint64_t fileLen, uint8_t fileTypeFlags, const char * fileName );

private:
    //=== vars ===//
    uint8_t						m_bListComplete{ false };		// if 1 then list has been completed
    uint8_t						m_u8Res1{ 0 };
    uint16_t					m_u16DataLen{ 0 };			    // length of data
    uint16_t					m_u16FileCnt{ 0 };		        // number of files in this pkt
    uint16_t					m_u16Res{ 0 };				
    uint32_t					m_u32Error{ 0 };
    uint32_t					m_u32ListIndex{ 0 };
    char						m_as8FileList[ PKT_BASE_MAX_SHARED_FILE_LIST_LEN ];
};

#pragma pack(pop)

