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

#define PKT_TYPE_OFFER_MAX_DATA_LEN		14320	// maximum length of chunk of Offer data
#define PKT_TYPE_OFFER_MAX_NAME_AND_TAG_LEN		( 4096 * 2 + 48 )	// maximum length of chunk of Offer data

#define MAX_OFFER_LIST_LEN				4096	// maximum length of list of Offers


#pragma pack(push) 
#pragma pack(1)

//============================================================================
// Offer Offer Packets
//============================================================================
enum EOfferXferCmd
{
	eOfferXferCmdOfferSend		    = 0,
	eOfferXferCmdOfferGet			= 1,
	eOfferXferCmdOfferChecksum	    = 2,
	eOfferXferCmdOfferDirectory	    = 3
};

enum EOfferXferOption
{
	eOfferXferOptionReplaceIfExists 	= 0,
	eOfferXferOptionResumeIfExists	    = 1,
	eOfferXferOptionFailIfExists		= 2
};

class OfferBaseInfo;

class PktOfferSendReq : public VxPktHdr
{
public:
	PktOfferSendReq();
	void						setOfferNameAndTag( const char * pOfferName, const char * assetTag = 0 );
	void						setOfferNameLen( uint16_t nameLen )				{ m_OfferNameLen = htons( nameLen ); }
	uint16_t					getOfferNameLen( void )							{ return ntohs( m_OfferNameLen ); }
	void						setOfferTagLen( uint16_t tagLen )				{ m_OfferTagLen = htons( tagLen ); }
	uint16_t					getOfferTagLen( void )							{ return ntohs( m_OfferTagLen ); }

	std::string					getOfferName();
	std::string					getOfferTag();
	void						setOfferHashId( VxSha1Hash& OfferHashId )		{ m_OfferHashId = OfferHashId; }
	VxSha1Hash&					getOfferHashId( void )							{ return m_OfferHashId; }

	void						setLclSessionId( VxGUID& lclId )				{ m_LclSessionId = lclId; }
	VxGUID&						getLclSessionId( void )							{ return m_LclSessionId; }
	void						setRmtSessionId( VxGUID& rmtId )				{ m_RmtSessionId = rmtId; }
	VxGUID&						getRmtSessionId( void )							{ return m_RmtSessionId; }

	void						setOfferId( VxGUID& uniqueId )					{ m_OfferId = uniqueId; }
	VxGUID&						getOfferId( void )								{ return m_OfferId; }
	void						setCreatorId( VxGUID& creatorId )				{ m_CreatorId = creatorId; }
	VxGUID&						getCreatorId( void )							{ return m_CreatorId; }
	void						setHistoryId( VxGUID& historyId )				{ m_HistoryId = historyId; }
	VxGUID&						getHistoryId( void )							{ return m_HistoryId; }

    void						setCreationTime( uint32_t createTime )			{ m_CreationTime = htonl( createTime ); }
	uint32_t					getCreationTime( void )						    { return ntohl( m_CreationTime ); }

	void						setOfferOffset( int64_t offset )				{ m_s64OfferOffs = htonU64( offset ); }
	int64_t						getOfferOffset( void )							{ return ntohU64( m_s64OfferOffs ); }
	void						setOfferLen( int64_t len )						{ m_s64OfferLen = htonU64( len ); }
	int64_t						getOfferLen( void )								{ return ntohU64( m_s64OfferLen ); }

	void						setOfferType( uint16_t OfferType )				{ m_OfferType = htons( OfferType ); }
	uint16_t					getOfferType( void )							{ return ntohs( m_OfferType ); }

	void						setError( uint32_t error )						{ m_u32Error = htonl( error ); }
	uint32_t					getError( void )								{ return ntohl( m_u32Error ); }

	void						fillPktFromOffer( OfferBaseInfo& assetInfo );
	void						fillOfferFromPkt( OfferBaseInfo& assetInfo );

private:
	uint16_t					m_OfferType;
	VxGUID						m_OfferId;
	VxGUID						m_CreatorId;
	VxGUID						m_HistoryId; 
	VxGUID						m_LclSessionId;
	VxGUID						m_RmtSessionId;
	VxSha1Hash					m_OfferHashId;
	uint32_t					m_u32Error; 
	int64_t						m_s64OfferLen;
	int64_t						m_s64OfferOffs;
	uint32_t					m_CreationTime;
	uint16_t					m_OfferNameLen;
	uint16_t					m_OfferTagLen;

	uint32_t					m_u32Res1; 
	uint32_t					m_u32Res2; 
	uint32_t					m_u32Res3; 
	uint32_t					m_u32Res4; 
	char						m_OfferNameAndTag[ PKT_TYPE_OFFER_MAX_NAME_AND_TAG_LEN ];
};

class PktOfferSendReply : public VxPktHdr
{
public:
	PktOfferSendReply();

	void						setOfferId( VxGUID& uniqueId )					{ m_OfferId = uniqueId; }
	VxGUID&						getOfferId( void )								{ return m_OfferId; }

	void						setLclSessionId( VxGUID& lclId )				{ m_LclSessionId = lclId; }
	VxGUID&						getLclSessionId( void )							{ return m_LclSessionId; }
	void						setRmtSessionId( VxGUID& rmtId )				{ m_RmtSessionId = rmtId; }
	VxGUID&						getRmtSessionId( void )							{ return m_RmtSessionId; }

	void						setOfferOffset( int64_t offset )				{ m_s64OfferOffs = htonU64( offset ); }
	int64_t						getOfferOffset( void )							{ return ntohU64( m_s64OfferOffs ); }

	void						setError( uint32_t error )						{ m_u32Error = htonl( error ); }
	uint32_t					getError( void )								{ return ntohl( m_u32Error ); }

	void						setRequiresFileXfer( bool requiresXfer )		{ m_u8RequiresFileXfer = (uint8_t)requiresXfer; }
	bool						getRequiresFileXfer( void )						{ return m_u8RequiresFileXfer ? true : false; }

private:
	uint8_t						m_u8RequiresFileXfer;
	uint8_t						m_u8Res;
	uint16_t					m_u16Res; 
	int64_t						m_s64OfferOffs;
	VxGUID						m_OfferId;
	VxGUID						m_LclSessionId;
	VxGUID						m_RmtSessionId;
	uint32_t					m_u32Error; 
	uint32_t					m_u32Res1;
	uint32_t					m_u32Res2; 
};

//============================================================================
// Offer chunk packets
//============================================================================
class PktOfferChunkReq : public VxPktHdr
{
public:
	PktOfferChunkReq();
	uint16_t					emptyLength( void );
	void						setChunkLen( uint16_t u16ChunkLen ); // also calculates packet length
	uint16_t					getChunkLen( void );

	void						setLclSessionId( VxGUID& lclId )				{ m_LclSessionId = lclId; }
	VxGUID&						getLclSessionId( void )							{ return m_LclSessionId; }
	void						setRmtSessionId( VxGUID& rmtId )				{ m_RmtSessionId = rmtId; }
	VxGUID&						getRmtSessionId( void )							{ return m_RmtSessionId; }

	void						setDataLen( uint16_t len )						{ m_u16OfferChunkLen = htons( len ); }
	uint16_t					getDataLen( void )								{ return ntohs( m_u16OfferChunkLen ); }

	void						setError( uint32_t error )						{ m_u32Error = htonl( error ); }
	uint32_t					getError( void )								{ return ntohl( m_u32Error ); }

private:
	uint16_t					m_u16Res;
	uint16_t					m_u16OfferChunkLen;
	VxGUID						m_LclSessionId;
	VxGUID						m_RmtSessionId;
	uint32_t					m_u32Error; 
	uint32_t					m_u32Res1; 
public:
	uint8_t						m_au8OfferChunk[ PKT_TYPE_OFFER_MAX_DATA_LEN ];
};

class PktOfferChunkReply : public VxPktHdr
{
public:
	PktOfferChunkReply();

	void						setLclSessionId( VxGUID& lclId )				{ m_LclSessionId = lclId; }
	VxGUID&						getLclSessionId( void )							{ return m_LclSessionId; }
	void						setRmtSessionId( VxGUID& rmtId )				{ m_RmtSessionId = rmtId; }
	VxGUID&						getRmtSessionId( void )							{ return m_RmtSessionId; }

	void						setDataLen( uint16_t len )						{ m_u16OfferChunkLen = htons( len ); }
	uint16_t					getDataLen( void )								{ return ntohs( m_u16OfferChunkLen ); }

	void						setError( uint32_t error )						{ m_u32Error = htonl( error ); }
	uint32_t					getError( void )								{ return ntohl( m_u32Error ); }

private:
	uint16_t					m_u16Res;	
	uint16_t					m_u16OfferChunkLen;
	VxGUID						m_LclSessionId;
	VxGUID						m_RmtSessionId;
	uint32_t					m_u32Error; 
};

//============================================================================
// PktOfferSendComplete
//============================================================================
class PktOfferSendCompleteReq : public VxPktHdr
{
public:
	PktOfferSendCompleteReq();

	void						setLclSessionId( VxGUID& lclId )			{ m_LclSessionId = lclId; }
	VxGUID&						getLclSessionId( void )						{ return m_LclSessionId; }
	void						setRmtSessionId( VxGUID& rmtId )			{ m_RmtSessionId = rmtId; }
	VxGUID&						getRmtSessionId( void )						{ return m_RmtSessionId; }

	void						setOfferId( VxGUID& uniqueId  )		        { m_OfferId = uniqueId; }
	VxGUID&						getOfferHashId( void )						{ return m_OfferId; }

	void						setError( uint32_t error )					{ m_u32Error = htonl( error ); }
	uint32_t					getError( void )							{ return ntohl( m_u32Error ); }

private:
	VxGUID						m_LclSessionId;
	VxGUID						m_RmtSessionId;
	VxGUID						m_OfferId;
	uint32_t					m_u32Error; 
	uint32_t					m_u32Res1; 
};

class PktOfferSendCompleteReply : public VxPktHdr
{
public:
	PktOfferSendCompleteReply();

	void						setLclSessionId( VxGUID& lclId )			{ m_LclSessionId = lclId; }
	VxGUID&						getLclSessionId( void )						{ return m_LclSessionId; }
	void						setRmtSessionId( VxGUID& rmtId )			{ m_RmtSessionId = rmtId; }
	VxGUID&						getRmtSessionId( void )						{ return m_RmtSessionId; }

	void						setOfferId( VxGUID& uniqueId  )		        { m_OfferId = uniqueId; }
	VxGUID&						getOfferId( void )					        { return m_OfferId; }

	void						setError( uint32_t error )					{ m_u32Error = htonl( error ); }
	uint32_t					getError( void )							{ return ntohl( m_u32Error ); }

private:
	VxGUID						m_LclSessionId;
	VxGUID						m_RmtSessionId;
	VxGUID						m_OfferId;
	uint32_t					m_u32Error; 
	uint32_t					m_u32Res1; 
};

//============================================================================
// PktOfferXferErr
//============================================================================
class PktOfferXferErr : public VxPktHdr
{
public:
	PktOfferXferErr();

	const char *				describeError();

	void						setRxInstance( VxGUID& instanceGuid )			{ m_RxOfferInstance = instanceGuid; }
	VxGUID& 					getRxInstance( void )							{ return m_RxOfferInstance; }
	void						setTxInstance( VxGUID& instanceGuid )			{ m_TxOfferInstance = instanceGuid; }
	VxGUID&						getTxInstance( void )							{ return m_TxOfferInstance; }

	void						setError( uint16_t error )						{ m_u16Err = htons( error ); }
	uint16_t					getError( void )								{ return ntohs( m_u16Err ); }

private:
	uint16_t					m_u16Err;
	uint16_t					m_u16Res1;
	uint32_t					m_u32ResP1;
	uint32_t					m_u32ResP2;
	uint32_t					m_u32ResP3;

	VxGUID						m_RxOfferInstance; 
	VxGUID						m_TxOfferInstance; 
	uint32_t					m_u32Res1; 
	uint32_t					m_u32Res2;
};

#pragma pack(pop)

