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
// http://www.nolimitconnect.org
//============================================================================

#include "VxCommon.h"
#include "VxPktHdr.h"
#include "PktRequestErrors.h"

#include <CoreLib/IsBigEndianCpu.h>
#include <CoreLib/VxSha1Hash.h>

#define PKT_TYPE_FILE_MAX_DATA_LEN		14320	// maximum length of chunk of file data
#define MAX_FILE_LIST_LEN				4096	// maximum length of list of files

#pragma pack(push)
#pragma pack(1)

class PktFileGetReq : public VxPktHdr
{
public:
    PktFileGetReq();

    void						setFileName( std::string &csFileName );
    void						getFileName( std::string &csRetFileName );
	void						setAssetId( VxGUID& fileAssetId )				{ m_FileAssetId = fileAssetId; }
	VxGUID&						getAssetId( void )								{ return m_FileAssetId; }
	void						setFileHashId( VxSha1Hash& fileHashId )			{ m_FileHashId = fileHashId; }
	VxSha1Hash&					getFileHashId( void )							{ return m_FileHashId; }

	void						setLclSessionId( VxGUID& lclId )			    { m_LclSessionId = lclId; }
	VxGUID&						getLclSessionId( void )						    { return m_LclSessionId; }
	void						setRmtSessionId( VxGUID& rmtId )			    { m_RmtSessionId = rmtId; }
	VxGUID&						getRmtSessionId( void )						    { return m_RmtSessionId; }

	void						setStartOffset( int64_t offset )				{ m_s64StartOffs = htonU64( offset ); }
	int64_t						getStartOffset( void )							{ return ntohU64( m_s64StartOffs ); }
	void						setEndOffset( int64_t offset )					{ m_s64EndOffs = htonU64( offset ); }
	int64_t						getEndOffset( void )							{ return ntohU64( m_s64EndOffs ); }

private:
	VxGUID						m_LclSessionId;
	VxGUID						m_RmtSessionId;
	VxGUID						m_FileAssetId;
	VxSha1Hash					m_FileHashId;
	int64_t						m_s64StartOffs{ 0 };
	int64_t						m_s64EndOffs{ 0 };	//if 0 then get all
	uint32_t					m_u32Res1{ 0 };
	uint32_t					m_u32Res2{ 0 };
	char						m_FileName[ VX_MAX_PATH + 16 ];
};

class PktFileGetReply : public VxPktHdr
{
public:
	PktFileGetReply();

	void						setFileName( std::string &csFileName );
	void						getFileName( std::string &csRetFileName );
	void						setAssetId( VxGUID& fileAssetId )					{ m_FileAssetId = fileAssetId; }
	VxGUID&						getAssetId( void )									{ return m_FileAssetId; }
	void						setFileHashId( VxSha1Hash& fileHashId )			    { m_FileHashId = fileHashId; }
	VxSha1Hash&					getFileHashId( void )							    { return m_FileHashId; }
    uint16_t					getemptyLen( void );

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
	VxGUID						m_LclSessionId;
	VxGUID						m_RmtSessionId;
	VxGUID						m_FileAssetId;
	VxSha1Hash					m_FileHashId;
	int64_t						m_s64StartOffs{ 0 };
	int64_t						m_s64EndOffs{ 0 };	//if 0 then get all
	uint32_t					m_u32Error{ 0 };
	uint32_t					m_u32Res2{ 0 };
	char						m_FileName[ VX_MAX_PATH + 16 ];
};

//============================================================================
// File Offer Packets
//============================================================================
enum EFileXferCmd
{
	eFileXferCmdFileSend		= 0,
	eFileXferCmdFileGet			= 1,
	eFileXferCmdFileChecksum	= 2,
	eFileXferCmdFileDirectory	= 3
};

enum EFileXferOption
{
	eFileXferOptionReplaceIfExists	= 0,
	eFileXferOptionResumeIfExists	= 1,
	eFileXferOptionFailIfExists		= 2
};

class PktFileSendReq : public VxPktHdr
{
public:
	PktFileSendReq();
	void						setFileName( const char* pFileName );
	const char*					getFileName()									{ return m_FileName; }
	void						setAssetId( VxGUID& assetId )					{ m_FileAssetId = assetId; }
	VxGUID&						getAssetId( void )								{ return m_FileAssetId; }
	void						setFileHashId( VxSha1Hash& fileHashId )			{ m_FileHashId = fileHashId; }
	VxSha1Hash&					getFileHashId( void )							{ return m_FileHashId; }
	void						setFileXferCmd( EFileXferCmd eCmd )				{ m_u8XferCmd = (uint8_t)eCmd; }
	EFileXferCmd				getFileXferCmd()								{ return (EFileXferCmd)m_u8XferCmd; }
	void						setFileXferOption( EFileXferOption eOption )	{ m_u8XferOption = (uint8_t)eOption; }
	EFileXferOption				getFileXferOption()								{ return (EFileXferOption)m_u8XferOption; }

	void						setLclSessionId( VxGUID& lclId )				{ m_LclSessionId = lclId; }
	VxGUID&						getLclSessionId( void )							{ return m_LclSessionId; }
	void						setRmtSessionId( VxGUID& rmtId )				{ m_RmtSessionId = rmtId; }
	VxGUID&						getRmtSessionId( void )							{ return m_RmtSessionId; }

	void						setFileOffset( int64_t offset )					{ m_s64FileOffs = htonU64( offset ); }
	int64_t						getFileOffset( void )							{ return ntohU64( m_s64FileOffs ); }
	void						setFileLen( int64_t len )						{ m_s64FileLen = htonU64( len ); }
	int64_t						getFileLen( void )								{ return ntohU64( m_s64FileLen ); }

	void						setFileState( uint16_t state )					{ m_u16State = htons( state ); }
	uint16_t					getFileState( void )							{ return ntohs( m_u16State ); }
	void						setFileType( uint8_t fileType )					{ m_u8FileType = fileType; }
	uint8_t						getFileType( void )								{ return m_u8FileType; }

	void						setError( uint32_t error )						{ m_u32Error = htonl( error ); }
	uint32_t					getError( void )								{ return ntohl( m_u32Error ); }

private:
	uint16_t					m_u16State;
	uint8_t						m_u8XferCmd;
	uint8_t						m_u8XferOption;
	uint8_t						m_u8FileType;
	uint8_t						m_u8Res1;
	uint16_t					m_u16Res1;
	int64_t						m_s64FileOffs;
	int64_t						m_s64FileLen;
	VxGUID						m_LclSessionId;
	VxGUID						m_RmtSessionId;
	VxGUID						m_FileAssetId;
	VxSha1Hash					m_FileHashId;
	uint32_t					m_u32Error; 
	uint32_t					m_u32Res2; 
	char						m_FileName[ VX_MAX_PATH + 16 ];
};

class PktFileSendReply : public VxPktHdr
{
public:
	PktFileSendReply();

	void						setFileName( const char* pFileName );
	const char*				getFileName()									{ return m_FileName; }
	void						setAssetId( VxGUID& assetId )					{ m_FileAssetId = assetId; }
	VxGUID&						getAssetId( void )								{ return m_FileAssetId; }
	void						setFileHashId( VxSha1Hash& fileHashId )			{ m_FileHashId = fileHashId; }
	VxSha1Hash&					getFileHashId( void )							{ return m_FileHashId; }
	void						setFileXferCmd( EFileXferCmd eCmd )				{ m_u8XferCmd = (uint8_t)eCmd; }
	EFileXferCmd				getFileXferCmd()								{ return (EFileXferCmd)m_u8XferCmd; }
	void						setFileXferOption( EFileXferOption eOption )	{ m_u8XferOption = (uint8_t)eOption; }
	EFileXferOption				getFileXferOption()								{ return (EFileXferOption)m_u8XferOption; }

	void						setLclSessionId( VxGUID& lclId )				{ m_LclSessionId = lclId; }
	VxGUID&						getLclSessionId( void )							{ return m_LclSessionId; }
	void						setRmtSessionId( VxGUID& rmtId )				{ m_RmtSessionId = rmtId; }
	VxGUID&						getRmtSessionId( void )							{ return m_RmtSessionId; }

	void						setFileOffset( int64_t offset )					{ m_s64FileOffs = htonU64( offset ); }
	int64_t						xgetFileOffset( void )							{ return ntohU64( m_s64FileOffs ); }
	void						setFileLen( int64_t len )						{ m_s64FileLen = htonU64( len ); }
	int64_t						getFileLen( void )								{ return ntohU64( m_s64FileLen ); }

	void						setFileState( uint16_t state )					{ m_u16State = htons( state ); }
	uint16_t					getFileState( void )							{ return ntohs( m_u16State ); }

	void						setError( uint32_t error )						{ m_u32Error = htonl( error ); }
	uint32_t					getError( void )								{ return ntohl( m_u32Error ); }

private:
	uint16_t					m_u16State;
	uint8_t						m_u8XferCmd;
	uint8_t						m_u8XferOption;
	uint32_t					m_u32Res;
	int64_t						m_s64FileOffs;
	int64_t						m_s64FileLen;
	VxGUID						m_LclSessionId;
	VxGUID						m_RmtSessionId;
	VxGUID						m_FileAssetId;
	VxSha1Hash					m_FileHashId;
	uint32_t					m_u32Error; 
	uint32_t					m_u32Res2; 
	char						m_FileName[ VX_MAX_PATH + 16 ];
};

//============================================================================
// File Find packets
//============================================================================
#define PKT_SHARE_FIND_FILE_MATCHNAME_MAX_LEN 128
class PktFileFindReq : public VxPktHdr
{
public:
	PktFileFindReq();
	uint16_t					getEmptyLen( void ){ return (uint16_t)(sizeof( PktFileFindReq )-PKT_SHARE_FIND_FILE_MATCHNAME_MAX_LEN); };
	void						SetMatchName( std::string & csMatchName );
	void						GetMatchName( std::string & csRetMatchName );

	void						setLclSessionId( VxGUID& lclId )				{ m_LclSessionId = lclId; }
	VxGUID&						getLclSessionId( void )							{ return m_LclSessionId; }
	void						setRmtSessionId( VxGUID& rmtId )				{ m_RmtSessionId = rmtId; }
	VxGUID&						getRmtSessionId( void )							{ return m_RmtSessionId; }

	void						setAssetId( VxGUID& assetId )					{ m_FileAssetId = assetId; }
	VxGUID&						getAssetId( void )								{ return m_FileAssetId; }

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
	uint16_t					m_u16SizeLimitType{ 0 };;	// type of file size limit 0=any size 1=At Least 2=At Most 3=Exactly
	uint32_t					m_u32ResP1{ 0 };;			// reserved
	int64_t						m_s64FileLen{ 0 };;		// file size 
	VxGUID						m_LclSessionId;
	VxGUID						m_RmtSessionId;
	VxGUID						m_FileAssetId;
	VxSha1Hash					m_FileHashId;
	uint32_t					m_u32Res1{ 0 };; 
	uint32_t					m_u32Res2{ 0 };; 
	char						m_MatchName[ PKT_SHARE_FIND_FILE_MATCHNAME_MAX_LEN + 16 ];
};

//============================================================================
// File chunk packets
//============================================================================
class PktFileChunkReq : public VxPktHdr
{
public:
	PktFileChunkReq();
	uint16_t					emptyLength( void );
	void						setChunkLen( uint16_t u16ChunkLen );
	uint16_t					getChunkLen( void );

	void						setLclSessionId( VxGUID& lclId )				{ m_LclSessionId = lclId; }
	VxGUID&						getLclSessionId( void )							{ return m_LclSessionId; }
	void						setRmtSessionId( VxGUID& rmtId )				{ m_RmtSessionId = rmtId; }
	VxGUID&						getRmtSessionId( void )							{ return m_RmtSessionId; }

	void						setAssetId( VxGUID& assetId )					{ m_AssetId = assetId; }
	VxGUID&						getAssetId( void )								{ return m_AssetId; }

	void						setDataLen( uint16_t len )						{ m_u16FileChunkLen = htons( len ); }
	uint16_t					getDataLen( void )								{ return ntohs( m_u16FileChunkLen ); }

	void						setError( uint32_t error )						{ m_u32Error = htonl( error ); }
	uint32_t					getError( void )								{ return ntohl( m_u32Error ); }

private:
	uint16_t					m_u16Res;
	uint16_t					m_u16FileChunkLen;
	VxGUID						m_LclSessionId;
	VxGUID						m_RmtSessionId;
	VxGUID						m_AssetId;
	uint32_t					m_u32Error; 
	uint32_t					m_u32Res1; 
public:
	uint8_t						m_au8FileChunk[ PKT_TYPE_FILE_MAX_DATA_LEN ];
};

class PktFileChunkReply : public VxPktHdr
{
public:
	PktFileChunkReply();

	void						setLclSessionId( VxGUID& lclId )				{ m_LclSessionId = lclId; }
	VxGUID&						getLclSessionId( void )							{ return m_LclSessionId; }
	void						setRmtSessionId( VxGUID& rmtId )				{ m_RmtSessionId = rmtId; }
	VxGUID&						getRmtSessionId( void )							{ return m_RmtSessionId; }

	void						setAssetId( VxGUID& assetId )					{ m_AssetId = assetId; }
	VxGUID&						getAssetId( void )								{ return m_AssetId; }

	void						setDataLen( uint16_t len )						{ m_u16FileChunkLen = htons( len ); }
	uint16_t					getDataLen( void )								{ return ntohs( m_u16FileChunkLen ); }

	void						setError( uint32_t error )						{ m_u32Error = htonl( error ); }
	uint32_t					getError( void )								{ return ntohl( m_u32Error ); }

private:
	uint16_t					m_u16Res;	
	uint16_t					m_u16FileChunkLen;
	VxGUID						m_LclSessionId;
	VxGUID						m_RmtSessionId;
	VxGUID						m_AssetId;
	uint32_t					m_u32Error; 
};

//============================================================================
// PktFileSendComplete
//============================================================================
class PktFileSendCompleteReq : public VxPktHdr
{
public:
	PktFileSendCompleteReq();

	void						setLclSessionId( VxGUID& lclId )			{ m_LclSessionId = lclId; }
	VxGUID&						getLclSessionId( void )						{ return m_LclSessionId; }
	void						setRmtSessionId( VxGUID& rmtId )			{ m_RmtSessionId = rmtId; }
	VxGUID&						getRmtSessionId( void )						{ return m_RmtSessionId; }

	void						setAssetId( VxGUID& assetId )				{ m_AssetId = assetId; }
	VxGUID&						getAssetId( void )							{ return m_AssetId; }

	void						setFileHashId( VxSha1Hash& fileHashId )		{ m_FileHashId = fileHashId; }
	VxSha1Hash&					getFileHashId( void )						{ return m_FileHashId; }

	void						setError( uint32_t error )					{ m_u32Error = htonl( error ); }
	uint32_t					getError( void )							{ return ntohl( m_u32Error ); }

private:
	VxGUID						m_LclSessionId;
	VxGUID						m_RmtSessionId;
	VxGUID						m_AssetId;
	VxSha1Hash					m_FileHashId;
	uint32_t					m_u32Error; 
};

class PktFileSendCompleteReply : public VxPktHdr
{
public:
	PktFileSendCompleteReply();

	void						setLclSessionId( VxGUID& lclId )			{ m_LclSessionId = lclId; }
	VxGUID&						getLclSessionId( void )						{ return m_LclSessionId; }
	void						setRmtSessionId( VxGUID& rmtId )			{ m_RmtSessionId = rmtId; }
	VxGUID&						getRmtSessionId( void )						{ return m_RmtSessionId; }

	void						setAssetId( VxGUID& assetId )				{ m_AssetId = assetId; }
	VxGUID&						getAssetId( void )							{ return m_AssetId; }

	void						setFileHashId( VxSha1Hash& fileHashId )		{ m_FileHashId = fileHashId; }
	VxSha1Hash&					getFileHashId( void )						{ return m_FileHashId; }

	void						setError( uint32_t error )					{ m_u32Error = htonl( error ); }
	uint32_t					getError( void )							{ return ntohl( m_u32Error ); }

private:
	VxGUID						m_LclSessionId;
	VxGUID						m_RmtSessionId;
	VxGUID						m_AssetId;
	VxSha1Hash					m_FileHashId;
	uint32_t					m_u32Error; 
};

class PktFileGetCompleteReq : public VxPktHdr
{
public:
	PktFileGetCompleteReq();

	void						setLclSessionId( VxGUID& lclId )			{ m_LclSessionId = lclId; }
	VxGUID&						getLclSessionId( void )						{ return m_LclSessionId; }
	void						setRmtSessionId( VxGUID& rmtId )			{ m_RmtSessionId = rmtId; }
	VxGUID&						getRmtSessionId( void )						{ return m_RmtSessionId; }

	void						setAssetId( VxGUID& assetId )				{ m_AssetId = assetId; }
	VxGUID&						getAssetId( void )							{ return m_AssetId; }

	void						setFileHashId( VxSha1Hash& fileHashId )		{ m_FileHashId = fileHashId; }
	VxSha1Hash&					getFileHashId( void )						{ return m_FileHashId; }

	void						setError( uint32_t error )					{ m_u32Error = htonl( error ); }
	uint32_t					getError( void )							{ return ntohl( m_u32Error ); }

private:
	VxGUID						m_LclSessionId;
	VxGUID						m_RmtSessionId;
	VxGUID						m_AssetId;
	VxSha1Hash					m_FileHashId;
	uint32_t					m_u32Error; 
};

class PktFileGetCompleteReply : public VxPktHdr
{
public:
	PktFileGetCompleteReply();

	void						setLclSessionId( VxGUID& lclId )			{ m_LclSessionId = lclId; }
	VxGUID&						getLclSessionId( void )						{ return m_LclSessionId; }
	void						setRmtSessionId( VxGUID& rmtId )			{ m_RmtSessionId = rmtId; }
	VxGUID&						getRmtSessionId( void )						{ return m_RmtSessionId; }

	void						setFileHashId( VxSha1Hash& fileHashId )		{ m_FileHashId = fileHashId; }
	VxSha1Hash&					getFileHashId( void )						{ return m_FileHashId; }

	void						setAssetId( VxGUID& assetId )				{ m_AssetId = assetId; }
	VxGUID&						getAssetId( void )							{ return m_AssetId; }

	void						setError( uint32_t error )					{ m_u32Error = htonl( error ); }
	uint32_t					getError( void )							{ return ntohl( m_u32Error ); }

private:
	VxGUID						m_LclSessionId;
	VxGUID						m_RmtSessionId;
	VxGUID						m_AssetId;
	VxSha1Hash					m_FileHashId;
	uint32_t					m_u32Error; 
};

//============================================================================
// PktShareErr
//============================================================================


class PktFileShareErr : public VxPktHdr
{
public:
	PktFileShareErr();

	const char*					describeError();

	void						setRxInstance( VxGUID& instanceGuid )			{ m_RxFileInstance = instanceGuid; }
	VxGUID& 					getRxInstance( void )							{ return m_RxFileInstance; }
	void						setTxInstance( VxGUID& instanceGuid )			{ m_TxFileInstance = instanceGuid; }
	VxGUID&						getTxInstance( void )							{ return m_TxFileInstance; }

	void						setAssetId( VxGUID& assetId )					{ m_AssetId = assetId; }
	VxGUID&						getAssetId( void )								{ return m_AssetId; }

	void						setError( uint16_t error )						{ m_u16Err = htons( error ); }
	uint16_t					getError( void )								{ return ntohs( m_u16Err ); }

private:
	uint16_t					m_u16Err;
	uint16_t					m_u16Res1;
	uint32_t					m_u32ResP1;
	uint32_t					m_u32ResP2;
	uint32_t					m_u32ResP3;

	VxGUID						m_RxFileInstance; 
	VxGUID						m_TxFileInstance; 
	VxGUID						m_AssetId;
	uint32_t					m_u32Res1; 
	uint32_t					m_u32Res2;
};

#pragma pack(pop) 


