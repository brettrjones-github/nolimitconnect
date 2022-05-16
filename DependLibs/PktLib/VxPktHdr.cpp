//============================================================================
// Copyright (C) 2013 Brett R. Jones
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

#include "VxPktHdr.h"
#include "PktTypes.h"
#include "PktBlobEntry.h"

#include <memory.h>

//from ascii chart
// h=0x68 t=0x74 p=0x70
// http = 0x68747470
    #define PKT_TYPE_NET_SERVICES						0x6874
    #define PKT_LENGTH_NET_SERVICES						0x7470


//============================================================================
VxPktHdrPrefix::VxPktHdrPrefix( const VxPktHdrPrefix& rhs )
    : m_u16PktLen( rhs.m_u16PktLen )
    , m_u16PktType( rhs.m_u16PktType )
    , m_u8PluginNum( rhs.m_u8PluginNum )
    , m_u8SeqNum( rhs.m_u8SeqNum )
    , m_RouteFlags( rhs.m_RouteFlags )
    , m_u8PktVersion( rhs.m_u8PktVersion )
{
}

//============================================================================
VxPktHdrPrefix&  VxPktHdrPrefix::operator = ( const VxPktHdrPrefix& rhs )
{
    if( this != &rhs )
    {
        m_u16PktLen = rhs.m_u16PktLen;
        m_u16PktType = rhs.m_u16PktType;
        m_u8PluginNum = rhs.m_u8PluginNum;
        m_u8SeqNum = rhs.m_u8SeqNum;
        m_RouteFlags = rhs.m_RouteFlags;
        m_u8PktVersion = rhs.m_u8PktVersion;
    }

    return *this;
}

//============================================================================
bool VxPktHdrPrefix::addToBlob( PktBlobEntry& blob )
{
    bool result = blob.setValue( m_u16PktLen );
    result &= blob.setValue( m_u16PktType );
    result &= blob.setValue( m_u8PluginNum );
    result &= blob.setValue( m_u8SeqNum );
    result &= blob.setValue( m_RouteFlags );
    result &= blob.setValue( m_u8PktVersion );
    return result;
}

//============================================================================
bool VxPktHdrPrefix::extractFromBlob( PktBlobEntry& blob )
{
    bool result = blob.getValue( m_u16PktLen );
    result &= blob.getValue( m_u16PktType );
    result &= blob.getValue( m_u8PluginNum );
    result &= blob.getValue( m_u8SeqNum );
    result &= blob.getValue( m_RouteFlags );
    result &= blob.getValue( m_u8PktVersion );
    return result;
}

//============================================================================
//=== return true if valid pkt type and length ===//
bool VxPktHdrPrefix::isValidPkt( void )
{
	uint16_t u16PktLen = getPktLength();
	uint16_t u16PktType = getPktType();
    if( (16 > u16PktLen) 
		|| (u16PktLen > MAX_PKT_LEN ) 
		|| (u16PktLen & 0x0f) 
		|| (1 > u16PktType) 
		|| (250 < u16PktType) )
    {
		if( isNetServicePkt() )
		{
			return true;
		}

		return false;
    }

    return true;
}

//============================================================================
bool VxPktHdrPrefix::isNetServicePkt( void )
{
    return PKT_TYPE_NET_SERVICES == getPktType()
            && PKT_LENGTH_NET_SERVICES == getPktLength();
}

//============================================================================
//! return true if data length is large enough to contain this packet
bool VxPktHdrPrefix::isPktAllHere(int iDataLen)
{
    return (iDataLen >= ntohs(m_u16PktLen))?true:false;
}

//============================================================================
void VxPktHdrPrefix::setPktLength( uint16_t pktLen )
{
    m_u16PktLen = htons( pktLen );
}

//============================================================================
//! return length of packet
uint16_t VxPktHdrPrefix::getPktLength( void ) const
{
    return ntohs( m_u16PktLen );
}

//============================================================================
void VxPktHdrPrefix::setPktType( uint16_t u16PktType )
{
	m_u16PktType = htons( u16PktType );
}

//============================================================================
uint16_t VxPktHdrPrefix::getPktType( void ) const
{
	return ntohs( m_u16PktType );
}

//============================================================================
void VxPktHdrPrefix::setPluginNum( uint8_t u8PluginNum )
{
	m_u8PluginNum = u8PluginNum;
}

//============================================================================
uint8_t	VxPktHdrPrefix::getPluginNum( void )
{
	return m_u8PluginNum;
}

//============================================================================
void VxPktHdrPrefix::setPktVersionNum( uint8_t  u8PktVersionNum )
{
	m_u8PktVersion = u8PktVersionNum;
}

//============================================================================
uint8_t	VxPktHdrPrefix::getPktVersionNum( void )
{
	return m_u8PktVersion;
}

//============================================================================
VxPktHdr::VxPktHdr( const VxPktHdr& rhs )
    : VxPktHdrPrefix( rhs )
    , m_SrcOnlineId( rhs.m_SrcOnlineId )
    , m_DestOnlineId( rhs.m_DestOnlineId )
{
}

//============================================================================
VxPktHdr&  VxPktHdr::operator = ( const VxPktHdr& rhs )
{
    if( this != &rhs )
    {
        m_SrcOnlineId = rhs.m_SrcOnlineId;
        m_DestOnlineId = rhs.m_DestOnlineId;
    }

    return *this;
}

//============================================================================
bool VxPktHdr::addToBlob( PktBlobEntry& blob )
{
    bool result = VxPktHdrPrefix::addToBlob( blob );
    result &= blob.setValue( m_SrcOnlineId );
    result &= blob.setValue( m_DestOnlineId );
    return result;
}

//============================================================================
bool VxPktHdr::extractFromBlob( PktBlobEntry& blob )
{
    bool result = VxPktHdrPrefix::extractFromBlob( blob );
    result &= blob.getValue( m_SrcOnlineId );
    result &= blob.getValue( m_DestOnlineId );
    return result;
}

//============================================================================
VxGUID VxPktHdr::getSrcOnlineId( void )
{
    return getGuidInHostOrder( m_SrcOnlineId );
}

//============================================================================
VxGUID VxPktHdr::getDestOnlineId( void )
{
    return getGuidInHostOrder( m_DestOnlineId );
}

//============================================================================
//! make a copy of this packet
VxPktHdr* VxPktHdr::makeCopy( void )
{
    uint8_t * pu8Copy = new uint8_t[ getPktLength() ];
    memcpy( pu8Copy, this, getPktLength() );
    return (VxPktHdr *)pu8Copy;
}

//============================================================================
void VxPktHdr::setGuidToNetOrder( VxGUID& srcGuid, VxGUID& destGuid )
{
    destGuid = srcGuid;
    destGuid.setToNetOrder();
}

//============================================================================
VxGUID VxPktHdr::getGuidInHostOrder( VxGUID& srcGuid )
{
    VxGUID guidCopy = srcGuid;
    guidCopy.setToHostOrder();
    return guidCopy;
}

//============================================================================
std::string VxPktHdr::describePkt( void )
{
    std::string pktDesc = describePktType( getPktType() );
    pktDesc += " ";
    pktDesc += std::to_string( getPktType());
    pktDesc += " len ";
    pktDesc += std::to_string( getPktLength());

    return pktDesc;
}

//============================================================================
const char* VxPktHdr::describePktType( uint16_t pktType )
{
    switch( pktType )
    {
    case PKT_TYPE_ANNOUNCE:							return "PktAnnounce";					// 1
    case PKT_TYPE_ANN_LIST:							return "PktAnnList";				    // 2 ( 0x02 )
    case PKT_TYPE_HOST_UNJOIN_REQ:					return "PktHostUnJoinReq";				// 3 ( 0x03 )
    case PKT_TYPE_HOST_UNJOIN_REPLY:				return "PktHostUnJoinReply";			// 4 ( 0x04 )
    case PKT_TYPE_SCAN_REQ:							return "PktPktScanReq";                 // 5 ( 0x05 )
    case PKT_TYPE_SCAN_REPLY:						return "PktPktScanReply";			    // 6 ( 0x06 )
    case PKT_TYPE_PLUGIN_OFFER_REQ:					return "PktPluginOfferReq";			    // 7 ( 0x07 )
    case PKT_TYPE_PLUGIN_OFFER_REPLY:				return "PktPluginOfferRepl";		    // 8 ( 0x08 ))
    case PKT_TYPE_CHAT_REQ:							return "PkChatReq";                     // 9 ( 0x09 )
    case PKT_TYPE_CHAT_REPLY:						return "PktChatReply";				    // 10 ( 0x0a )
    case PKT_TYPE_VOICE_REQ:						return "PktVoiceReq";			        // 11 ( 0x0b )
    case PKT_TYPE_VOICE_REPLY:						return "PktVoiceReply";			        // 12 ( 0x0c )
    case PKT_TYPE_VIDEO_FEED_REQ:					return "PktVideoFeedReq";			    // 13 ( 0x0d )
    case PKT_TYPE_VIDEO_FEED_STATUS:				return "PktVideoFeedStatus";		    // 14 ( 0x0e )
    case PKT_TYPE_VIDEO_FEED_PIC:					return "PktVideoFeedPic";               // 15 ( 0x0f )
    case PKT_TYPE_VIDEO_FEED_PIC_CHUNK:				return "PktVideoFeedPicChunk";		    // 16 ( 0x10 )
    case PKT_TYPE_VIDEO_FEED_PIC_ACK:				return "PktVideoFeedPicAck";	        // 17 ( 0x11 )
    case PKT_TYPE_SESSION_START_REQ:				return "PktSessionStartReq";	        // 18 ( 0x12 )
    case PKT_TYPE_SESSION_START_REPLY:				return "PktSessionStartReply";	        // 19 ( 0x13 )
    case PKT_TYPE_SESSION_STOP_REQ:					return "PktSessionStopReq";             // 20 ( 0x14 )
    case PKT_TYPE_SESSION_STOP_REPLY:				return "PktSessionStopReply";		    // 21 ( 0x15 )
    case PKT_TYPE_FILE_GET_REQ:						return "PktFileGetReq";                 // 22 ( 0x16 )
    case PKT_TYPE_FILE_GET_REPLY:					return "PktFileGetReply";			    // 23 ( 0x17 )
    case PKT_TYPE_FILE_SEND_REQ:					return "PktFileSendReq";		        // 24 ( 0x18 )
    case PKT_TYPE_FILE_SEND_REPLY:					return "PktFileSendReply";		        // 25 ( 0x19 )
    case PKT_TYPE_FILE_CHUNK_REQ:					return "PktFileChunkReq";		        // 26 ( 0x1a )
    case PKT_TYPE_FILE_CHUNK_REPLY:					return "PktFileChunkReply";		        // 27 ( 0x1b )
    case PKT_TYPE_FILE_GET_COMPLETE_REQ:			return "PkFileGetCompleteReq";			// 28 ( 0x1c )
    case PKT_TYPE_FILE_GET_COMPLETE_REPLY:			return "PktFileGetCompleteReply";	    // 29 ( 0x1d )
    case PKT_TYPE_FILE_SEND_COMPLETE_REQ:			return "PktFileSendCompleteReq";	    // 30 ( 0x1e )
    case PKT_TYPE_FILE_SEND_COMPLETE_REPLY:			return "PktFileSendCompleteReply";	    // 31 ( 0x1f )
    case PKT_TYPE_FILE_FIND_REQ:					return "PktFindFileReq";                // 32 ( 0x20 )
    case PKT_TYPE_FILE_FIND_REPLY:					return "PktFindFileReply";			    // 33 ( 0x21 )
    case PKT_TYPE_FILE_LIST_REQ:					return "PktFileListReq";			    // 34 ( 0x22 )
    case PKT_TYPE_FILE_LIST_REPLY:					return "PktFileListReply";			    // 35 ( 0x23 )
    case PKT_TYPE_FILE_INFO_REQ:					return "PktFileInfoReq";			    // 36 ( 0x24 )
    case PKT_TYPE_FILE_INFO_REPLY:					return "PktFileInfoReply";			    // 37 ( 0x25 )
    case PKT_TYPE_FILE_SHARE_ERR:					return "PktFileShareErr";			    // 38 ( 0x26 )
    case PKT_TYPE_ASSET_GET_REQ:					return "PktAssetGetReq";	            // 39 ( 0x27 )
    case PKT_TYPE_ASSET_GET_REPLY:					return "PktAssetGetReply";			    // 40 ( 0x28 )
    case PKT_TYPE_ASSET_SEND_REQ:					return "PktAssetSendReq";				// 41 ( 0x29 )
    case PKT_TYPE_ASSET_SEND_REPLY:					return "PktAssetSendReply";				// 42 ( 0x2A )
    case PKT_TYPE_ASSET_CHUNK_REQ:					return "PktAssetChunkReq";				// 43 ( 0x2B )
    case PKT_TYPE_ASSET_CHUNK_REPLY:				return "PktAssetChunkReply";            // 44 ( 0x2C )
    case PKT_TYPE_ASSET_GET_COMPLETE_REQ:			return "PktAssetGetCompleteReq";        // 45 ( 0x2D )
    case PKT_TYPE_ASSET_GET_COMPLETE_REPLY:			return "PktAssetGetCompleteReply";	    // 46 ( 0x2E )
    case PKT_TYPE_ASSET_SEND_COMPLETE_REQ:			return "PktAssetSendCompleteReq";		// 47 ( 0x2F )
    case PKT_TYPE_ASSET_SEND_COMPLETE_REPLY:		return "PktAssetSendCompleteReply";		// 48 ( 0x30 )
    case PKT_TYPE_ASSET_XFER_ERR:					return "PktAssetXferErr";               // 49 ( 0x31 )
/*
    case PKT_TYPE_ASSET_FIND_REQ					return "Pkt_TYPE_ASSET_XFER_ERR	";	            // 50 ( 0x32 )
    case PKT_TYPE_ASSET_FIND_REPLY					return "Pkt_TYPE_ASSET_FIND_REQ	";			    // 51 ( 0x33 )
    case PKT_TYPE_ASSET_LIST_REQ					return "Pkt_TYPE_ASSET_FIND_REPLY	";			    // 52 ( 0x34 )
    case PKT_TYPE_ASSET_LIST_REPLY					return "Pkt_TYPE_ASSET_LIST_REQ	";			    // 53 ( 0x35 )
    case PKT_TYPE_ASSET_INFO_REQ					return "Pkt_TYPE_ASSET_LIST_REPLY	";			    // 54 ( 0x36 )
    case PKT_TYPE_ASSET_INFO_REPLY					return "Pkt_TYPE_ASSET_INFO_REQ	";			    // 55 ( 0x37 )
// 56 ( 0x38 )
*/
    case PKT_TYPE_THUMB_GET_REQ:					return "PktThumbGetReq";                    // 56 ( 0x38 )
    case PKT_TYPE_THUMB_GET_REPLY:					return "PktThumbGetReply";                  // 57 ( 0x39 )
    case PKT_TYPE_THUMB_SEND_REQ:					return "PktThumbSendReq";                   // 58 ( 0x3A )
    case PKT_TYPE_THUMB_SEND_REPLY:				    return "PktThumbSendReply";                 // 59 ( 0x3B )
    case PKT_TYPE_THUMB_CHUNK_REQ:				    return "PktThumbChunkReq";                  // 60 ( 0x3C )
    case PKT_TYPE_THUMB_CHUNK_REPLY:				return "PktThumbChunkReply";		        // 61 ( 0x3D )
    case PKT_TYPE_THUMB_GET_COMPLETE_REQ:			return "PktThumbGetCompleteReq";			// 62 ( 0x3E )
    case PKT_TYPE_THUMB_GET_COMPLETE_REPLY:			return "PktThumbGetCompleteReply";          // 63 ( 0x3F )
    case PKT_TYPE_THUMB_SEND_COMPLETE_REQ:		    return "PktThumbSendCompleteReq";           // 64 ( 0x40 )
    case PKT_TYPE_THUMB_SEND_COMPLETE_REPLY:        return "PktThumbSendCompleteReply";         // 65 ( 0x41 )
    case PKT_TYPE_THUMB_XFER_ERR:					return "PktThumbXferErr";                   // 66 ( 0x42 )
/*
    // 67 ( 0x43 )
    case PKT_TYPE_THUMB_FIND_REQ					return "Pkt_TYPE_THUMB_XFER_ERR	";	            // 67 ( 0x43 )
    case PKT_TYPE_THUMB_FIND_REPLY					return "Pkt_TYPE_THUMB_FIND_REQ	";			    // 68 ( 0x44 )
    case PKT_TYPE_THUMB_LIST_REQ					return "Pkt_TYPE_THUMB_FIND_REPLY	";			    // 69 ( 0x45 )
    case PKT_TYPE_THUMB_LIST_REPLY					return "Pkt_TYPE_THUMB_LIST_REQ	";			    // 70 ( 0x46 )
    case PKT_TYPE_THUMB_INFO_REQ					return "Pkt_TYPE_THUMB_LIST_REPLY	";			    // 71 ( 0x47 )
    case PKT_TYPE_THUMB_INFO_REPLY					return "Pkt_TYPE_THUMB_INFO_REQ	";			 // 72 ( 0x48 )
// 73 ( 0x49 )
*/
    case PKT_TYPE_MSESSION_REQ:						return "PktMultiSessionReq";                // 73 ( 0x49 )
    case PKT_TYPE_MSESSION_REPLY:					return "PktMultiSessionReq";                // 74 ( 0x4a )
// 75 ( 0x4b )
    case PKT_TYPE_TCP_PUNCH:						return "PktTcpPunch";                       // 75 ( 0x4b )
    case PKT_TYPE_PING_REQ:							return "PktPingReq";                        // 76 ( 0x4c )
    case PKT_TYPE_PING_REPLY:						return "PktPingReply";                      // 77 ( 0x4d )
    case PKT_TYPE_IM_ALIVE_REQ:						return "PktImAliveReq";                     // 78 ( 0x4e )
    case PKT_TYPE_IM_ALIVE_REPLY:					return "PktImAliveReply";	                // 79 ( 0x4f )
/*
    // 80 ( 0x50 )
    case PKT_TYPE_GROUP_SEND_REQ					return "Pkt_TYPE_IM_ALIVE_REPLY	";	            // 80 ( 0x50 )
    case PKT_TYPE_GROUP_SEND_REPLY					return "Pkt_TYPE_GROUP_SEND_REQ	";               // 81 ( 0x51 )
    case PKT_TYPE_GROUP_CHUNK_REQ					return "Pkt_TYPE_GROUP_SEND_REPLY	";	            // 82 ( 0x52 )
    case PKT_TYPE_GROUP_CHUNK_REPLY					return "Pkt_TYPE_GROUP_CHUNK_REQ	";	            // 83 ( 0x53 )
// 84 ( 0x54 )
    case PKT_TYPE_GROUP_GET_COMPLETE_REQ				return "Pkt_TYPE_GROUP_CHUNK_REPLY	";		    // 84 ( 0x54 )
    case PKT_TYPE_GROUP_GET_COMPLETE_REPLY			return "Pkt_TYPE_GROUP_GET_COMPLETE_REQ	";	    // 85 ( 0x55 )
    case PKT_TYPE_GROUP_SEND_COMPLETE_REQ			return "Pkt_TYPE_GROUP_GET_COMPLETE_REPLY	";	    // 86 ( 0x56 )
    case PKT_TYPE_GROUP_SEND_COMPLETE_REPLY			return "Pkt_TYPE_GROUP_SEND_COMPLETE_REQ	";	    // 87 ( 0x57 )
// 88 ( 0x58 )
    case PKT_TYPE_GROUP_FIND_REQ						return "Pkt_TYPE_GROUP_SEND_COMPLETE_REPLY ";	    // 88 ( 0x58 )
    case PKT_TYPE_GROUP_FIND_REPLY					return "Pkt_TYPE_GROUP_FIND_REQ	";			    // 89 ( 0x59 )
    case PKT_TYPE_GROUP_LIST_REQ						return "Pkt_TYPE_GROUP_FIND_REPLY	";			    // 90 ( 0x5A )
    case PKT_TYPE_GROUP_LIST_REPLY					return "Pkt_TYPE_GROUP_LIST_REQ	";			    // 91 ( 0x5B )
    case PKT_TYPE_GROUP_INFO_REQ						return "Pkt_TYPE_GROUP_LIST_REPLY	";			    // 92 ( 0x5C )
    case PKT_TYPE_GROUP_INFO_REPLY					return "Pkt_TYPE_GROUP_INFO_REQ	";			    // 93 ( 0x5D )
    case PKT_TYPE_GROUP_SHARE_ERR					return "Pkt_TYPE_GROUP_INFO_REPLY	";			    // 94 ( 0x5E )
// 95 ( 0x5F )
*/
    case PKT_TYPE_TOD_GAME_STATS:					return "PktTodGameStats";               // 95 ( 0x5F )
    case PKT_TYPE_TOD_GAME_ACTION:					return "PktTodGameAction";			    // 96 ( 0x60 )
    case PKT_TYPE_TOD_GAME_VALUE:					return "PktTodGameValue";			    // 97 ( 0x61 )
// 98 ( 0x62 )
    case PKT_TYPE_PLUGIN_SETTING_REQ:				return "PktPluginSettingReq";			// 98 ( 0x62 )
    case PKT_TYPE_PLUGIN_SETTING_REPLY:			    return "PktPluginSettingReply";		    // 99 ( 0x63 )
// 100 ( 0x64 )
    case PKT_TYPE_THUMB_SETTING_REQ:				return "PktThumbSettingReq";		    // 100 ( 0x64 )
    case PKT_TYPE_THUMB_SETTING_REPLY:			    return "PktThumbSettingReply";		    // 101 ( 0x65 )
    case PKT_TYPE_THUMB_SETTING_CHUNK_REQ:			return "PktThumbSettingChunkReq";		// 102 ( 0x66 )
    case PKT_TYPE_THUMB_SETTING_CHUNK_REPLY:		return "PktThumbSettingChunkReply";	    // 103 ( 0x67 )
// 104 ( 0x68 )
    case PKT_TYPE_THUMB_AVATAR_REQ:					return "PktThumbAvatarReq";             // 104 ( 0x68 )
    case PKT_TYPE_THUMB_AVATAR_REPLY:			    return "Pkt_ThumbAvatarReply";			// 105 ( 0x69 )
    case PKT_TYPE_THUMB_AVATAR_CHUNK_REQ:			return "PktThumbAvatarChunkReq";		// 106 ( 0x6A )
    case PKT_TYPE_THUMB_AVATAR_CHUNK_REPLY:			return "PktThumbAvatarChunkReply";	    // 107 ( 0x6B )
// 108 ( 0x6C )
    case PKT_TYPE_THUMB_FILE_REQ:					return "PktThumbFileReq";               // 108 ( 0x6C )
    case PKT_TYPE_THUMB_FILE_REPLY:			        return "PktThumbFileReply";			    // 109 ( 0x6D )
    case PKT_TYPE_THUMB_FILE_CHUNK_REQ:				return "PktThumbFileChunkReq";		    // 110 ( 0x6E )
    case PKT_TYPE_THUMB_FILE_CHUNK_REPLY:			return "PktThumbFileChunkReply";		// 111 ( 0x6F )
// 112 ( 0x70 )
    case PKT_TYPE_HOST_ANNOUNCE:					return "PktHostAnnounce";               // 112 ( 0x70 )
// 113 ( 0x71 )
    case PKT_TYPE_BLOB_SEND_REQ:					return "PkBlobSendReq";                 // 113 ( 0x71 )
    case PKT_TYPE_BLOB_SEND_REPLY:				    return "PktBlobSendReply";			    // 114 ( 0x72 )
    case PKT_TYPE_BLOB_CHUNK_REQ:				    return "PktBlobChunkReq";			    // 115 ( 0x73 )
    case PKT_TYPE_BLOB_CHUNK_REPLY:				    return "PktBlobChunkReply";			    // 116 ( 0x74 )
    case PKT_TYPE_BLOB_SEND_COMPLETE_REQ:		    return "PktBlobSendCompleteReq";		// 117 ( 0x75 )
    case PKT_TYPE_BLOB_SEND_COMPLETE_REPLY:		    return "PktBlobSendCompleteReply ";	    // 118 ( 0x76 )
    case PKT_TYPE_BLOB_XFER_ERR:					return "PktBlobXferErr";                // 119 ( 0x77 )
// 120 ( 0x78 )
    case PKT_TYPE_HOST_JOIN_REQ:		            return "PktHostJoinReq";	            // 120 ( 0x78 )
    case PKT_TYPE_HOST_JOIN_REPLY:					return "PktHostJoinReply";	            // 121 ( 0x79 )
    case PKT_TYPE_HOST_LEAVE_REQ:		            return "PktHostLeaveReq";	            // 122 ( 0x7A )
    case PKT_TYPE_HOST_LEAVE_REPLY:					return "PktHostLeaveReply";	            // 123 ( 0x7B )
// 122 ( 0x7A )
    case PKT_TYPE_HOST_OFFER_REQ:		            return "PktHostOfferReq";	            // 124 ( 0x7C )
    case PKT_TYPE_HOST_OFFER_REPLY:					return "PktHostOfferReply";	            // 125 ( 0x7D )
// 124 ( 0x7C )
    case PKT_TYPE_HOST_SEARCH_REQ:		            return "PktHostSearchReq";	            // 126 ( 0x7E )
    case PKT_TYPE_HOST_SEARCH_REPLY:                return "PktHostSearchReply";	        // 127 ( 0x7F )
// 126 ( 0x7E )
    case PKT_TYPE_FRIEND_OFFER_REQ:		            return "PktFriendOfferReq";	            // 128 ( 0x80 )
    case PKT_TYPE_FRIEND_OFFER_REPLY:               return "PktFriendOfferReply";	        // 129 ( 0x81 )
// 128 ( 0x80 )
    case PKT_TYPE_OFFER_SEND_REQ:					return "PktOfferSendReq";               // 130 ( 0x82 )
    case PKT_TYPE_OFFER_SEND_REPLY:					return "PktOfferSendReply";			    // 132 ( 0x83 )
    case PKT_TYPE_OFFER_CHUNK_REQ:					return "PktOfferChunkReq";			    // 132 ( 0x84 )
    case PKT_TYPE_OFFER_CHUNK_REPLY:				return "PktOfferChunkReply";			// 133 ( 0x85 )
    case PKT_TYPE_OFFER_SEND_COMPLETE_REQ:			return "PktOfferSendCompleteReq";       // 134 ( 0x86 )
    case PKT_TYPE_OFFER_SEND_COMPLETE_REPLY:		return "PktOfferSendCompleteReply";	    // 135 ( 0x87 )
    case PKT_TYPE_OFFER_XFER_ERR:					return "PktOfferXferErr";               // 136 ( 0x88 )
// 135 ( 0x87 )
    case PKT_TYPE_WEB_SERVER_MY_PIC_SEND_REQ:		return "PktMyPicSendReq";				// 137 ( 0x89 )
    case PKT_TYPE_WEB_SERVER_MY_PIC_SEND_REPLY:		return "PktMyPicSendReply";             // 138 ( 0x8A )
    case PKT_TYPE_WEB_SERVER_PIC_CHUNK_TX:			return "PktWebServerPicChunkTx";        // 139 ( 0x8B )
    case PKT_TYPE_WEB_SERVER_PIC_CHUNK_ACK:			return "PktWebServerPicChunkAck";		// 140 ( 0x8C )
    case PKT_TYPE_WEB_SERVER_GET_CHUNK_TX:			return "PktWebServerGetChunkTx";        // 141 ( 0x8D )
    case PKT_TYPE_WEB_SERVER_GET_CHUNK_ACK:			return "PktWebServerGetChunkAck";       // 142 ( 0x8E )
    case PKT_TYPE_WEB_SERVER_PUT_CHUNK_TX:			return "PktWebServerPutChunkTx";        // 143 ( 0x8F )
    case PKT_TYPE_WEB_SERVER_PUT_CHUNK_ACK:			return "PktWebServerPutChunkAck";       // 144 ( 0x90 )
// 143 ( 0x8F )
//    case PKT_TYPE_STORY_DATA_REQ					return "Pkt_TYPE_WEB_SERVER_PUT_CHUNK_ACK	";       // 145 ( 0x91 )
//    case PKT_TYPE_STORY_DATA_REPLY					return "Pkt_TYPE_STORY_DATA_REQ	";               // 146 ( 0x92 )
// 145 ( 0x91 )
    /*
    case PKT_TYPE_RELAY_SERVICE_REQ					return "Pkt_TYPE_STORY_DATA_REPLY	";				// 147 ( 0x93 )
    case PKT_TYPE_RELAY_SERVICE_REPLY				return "Pkt_TYPE_RELAY_SERVICE_REQ	";			// 148 ( 0x94 )
    case PKT_TYPE_RELAY_CONNECT_REQ					return "Pkt_TYPE_RELAY_SERVICE_REPLY	";			// 149 ( 0x95 )
    case PKT_TYPE_RELAY_CONNECT_REPLY				return "Pkt_TYPE_RELAY_CONNECT_REQ	";			// 150 ( 0x96 )
    case PKT_TYPE_RELAY_SESSION_REQ					return "Pkt_TYPE_RELAY_CONNECT_REPLY	";			// 151 ( 0x97 )
    case PKT_TYPE_RELAY_SESSION_REPLY				return "Pkt_TYPE_RELAY_SESSION_REQ	";			// 152 ( 0x98 )
    case PKT_TYPE_RELAY_CONNECT_TO_USER_REQ			return "Pkt_TYPE_RELAY_SESSION_REPLY	";			// 153 ( 0x99 )
    case PKT_TYPE_RELAY_CONNECT_TO_USER_REPLY		return "Pkt_TYPE_RELAY_CONNECT_TO_USER_REQ ";      // 154 ( 0x9A )
    case PKT_TYPE_RELAY_USER_DISCONNECT				return "Pkt_TYPE_RELAY_CONNECT_TO_USER_REPLY ";	// 155 ( 0x9B )
    case PKT_TYPE_RELAY_TEST_REQ					return "Pkt_TYPE_RELAY_USER_DISCONNECT	";		// 156 ( 0x9C )
    case PKT_TYPE_RELAY_TEST_REPLY					return "Pkt_TYPE_RELAY_TEST_REQ	";          // 157 ( 0x9D )
// 156 ( 0x9C )
*/
    case PKT_TYPE_PUSH_TO_TALK_REQ:				    return "PktPushToTalkReq";		        // 158 ( 0x9E )
    case PKT_TYPE_PUSH_TO_TALK_REPLY:				return "PktPushToTalkReply";               // 159 ( 0x9F )
// 158 ( 0x9E )
//    case PKT_TYPE_MEMBERSHIP_REQ				    return "Pkt_TYPE_PUSH_TO_TALK_REPLY ";		        // 160 ( 0xA0 )
//    case PKT_TYPE_MEMBERSHIP_REPLY			    return "Pkt_TYPE_MEMBERSHIP_REQ	";               // 161 ( 0xA1 )
// 160 ( 0xA0 )
    case PKT_TYPE_HOST_INFO_REQ:				    return "PktHostInfoReq";                    // 162 ( 0xA2 )
    case PKT_TYPE_HOST_INFO_REPLY:			        return "PktHostInfoReply";                  // 163 ( 0xA3 )
// 162 ( 0xA2 )
    case PKT_TYPE_HOST_INVITE_ANN_REQ:				return "PktHostInviteAnnReq";		        // 164 ( 0xA4 )
    case PKT_TYPE_HOST_INVITE_ANN_REPLY:			return "PktHostInviteAnnReply";             // 165 ( 0xA5 )
    case PKT_TYPE_HOST_INVITE_SEARCH_REQ:			return "PktHostInviteSearchReq";		    // 166 ( 0xA6 )
    case PKT_TYPE_HOST_INVITE_SEARCH_REPLY:			return "PktHostInviteSearchReply";          // 167 ( 0xA7 )
    case PKT_TYPE_HOST_INVITE_MORE_REQ:				return "PktHostInviteMoreReq";              // 168 ( 0xA8 )
    case PKT_TYPE_HOST_INVITE_MORE_REPLY:			return "PktHostInviteMoreReply";            // 169 ( 0xA9 )
// 168 ( 0xA8 )
    case PKT_TYPE_GROUPIE_INFO_REQ:				    return "PktGroupieInfoReq";                 // 170 ( 0xAA )
    case PKT_TYPE_GROUPIE_INFO_REPLY:			    return "PktGroupieInfoReply";               // 171 ( 0xAB )
// 170 ( 0xAA )
    case PKT_TYPE_GROUPIE_ANN_REQ:				    return "PktGroupieAnnReq";                  // 172 ( 0xAC )
    case PKT_TYPE_GROUPIE_ANN_REPLY:			    return "PktGroupieAnnReply";                // 173 ( 0xAD )
    case PKT_TYPE_GROUPIE_SEARCH_REQ:				return "PktGroupieSearchReq";		        // 174 ( 0xAE )
    case PKT_TYPE_GROUPIE_SEARCH_REPLY:			    return "PktGroupieSearchReply";             // 175 ( 0xAF )
    case PKT_TYPE_GROUPIE_MORE_REQ:				    return "PktGroupieMoreReq";                 // 176 ( 0xB0 )
    case PKT_TYPE_GROUPIE_MORE_REPLY:               return "PktGroupieMoreReply";               // 177 ( 0xB1 )

// 178 ( 0x82 )
    case PKT_TYPE_FILE_INFO_INFO_REQ:				return "PktFileInfoReq";                    // 178 ( 0xB2 )
    case PKT_TYPE_FILE_INFO_INFO_REPLY:			    return "PktFileInfoReply";                  // 179 ( 0xB3 )
// 170 ( 0xB4 )
    case PKT_TYPE_FILE_INFO_ANN_REQ:				return "PktFileInfoAnnReq";                 // 180 ( 0xB4 )
    case PKT_TYPE_FILE_INFO_ANN_REPLY:			    return "PktFileInfoAnnReply";               // 181 ( 0xB5 )
    case PKT_TYPE_FILE_INFO_SEARCH_REQ:				return "PktFileInfoSearchReq";              // 182 ( 0xB6 )
    case PKT_TYPE_FILE_INFO_SEARCH_REPLY:			return "PktFileInfoSearchReply";            // 183 ( 0xB7 )
    case PKT_TYPE_FILE_INFO_MORE_REQ:				return "PktFileInfoMoreReq";                // 184 ( 0xB8 )
    case PKT_TYPE_FILE_INFO_MORE_REPLY:			    return "PktFileInfoMoreReply";
    default:
        return "PktUnknown";
    }
}
