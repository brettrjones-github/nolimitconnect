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
// http://www.nolimitconnect.org
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
    case PKT_TYPE_PLUGIN_OFFER_REPLY:				return "PktPluginOfferRepl";		    // 8 ( 0x08 )
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

    case PKT_TYPE_THUMB_GET_REQ:					return "PktThumbGetReq";                
    case PKT_TYPE_THUMB_GET_REPLY:					return "PktThumbGetReply";              
    case PKT_TYPE_THUMB_SEND_REQ:					return "PktThumbSendReq";               
    case PKT_TYPE_THUMB_SEND_REPLY:				    return "PktThumbSendReply";             
    case PKT_TYPE_THUMB_CHUNK_REQ:				    return "PktThumbChunkReq";              
    case PKT_TYPE_THUMB_CHUNK_REPLY:				return "PktThumbChunkReply";		    
    case PKT_TYPE_THUMB_GET_COMPLETE_REQ:			return "PktThumbGetCompleteReq";		
    case PKT_TYPE_THUMB_GET_COMPLETE_REPLY:			return "PktThumbGetCompleteReply";      
    case PKT_TYPE_THUMB_SEND_COMPLETE_REQ:		    return "PktThumbSendCompleteReq";       
    case PKT_TYPE_THUMB_SEND_COMPLETE_REPLY:        return "PktThumbSendCompleteReply";     
    case PKT_TYPE_THUMB_XFER_ERR:					return "PktThumbXferErr";               

    case PKT_TYPE_MSESSION_REQ:						return "PktMultiSessionReq";            
    case PKT_TYPE_MSESSION_REPLY:					return "PktMultiSessionReq";            

    case PKT_TYPE_TCP_PUNCH:						return "PktTcpPunch";                   
    case PKT_TYPE_PING_REQ:							return "PktPingReq";                    
    case PKT_TYPE_PING_REPLY:						return "PktPingReply";                  
    case PKT_TYPE_IM_ALIVE_REQ:						return "PktImAliveReq";                 
    case PKT_TYPE_IM_ALIVE_REPLY:					return "PktImAliveReply";	            

    case PKT_TYPE_TOD_GAME_STATS:					return "PktTodGameStats";               
    case PKT_TYPE_TOD_GAME_ACTION:					return "PktTodGameAction";			    
    case PKT_TYPE_TOD_GAME_VALUE:					return "PktTodGameValue";			    

    case PKT_TYPE_PLUGIN_SETTING_REQ:				return "PktPluginSettingReq";			
    case PKT_TYPE_PLUGIN_SETTING_REPLY:			    return "PktPluginSettingReply";		    

    case PKT_TYPE_THUMB_SETTING_REQ:				return "PktThumbSettingReq";		    
    case PKT_TYPE_THUMB_SETTING_REPLY:			    return "PktThumbSettingReply";		    
    case PKT_TYPE_THUMB_SETTING_CHUNK_REQ:			return "PktThumbSettingChunkReq";		
    case PKT_TYPE_THUMB_SETTING_CHUNK_REPLY:		return "PktThumbSettingChunkReply";	    

    case PKT_TYPE_THUMB_AVATAR_REQ:					return "PktThumbAvatarReq";             
    case PKT_TYPE_THUMB_AVATAR_REPLY:			    return "Pkt_ThumbAvatarReply";			
    case PKT_TYPE_THUMB_AVATAR_CHUNK_REQ:			return "PktThumbAvatarChunkReq";		
    case PKT_TYPE_THUMB_AVATAR_CHUNK_REPLY:			return "PktThumbAvatarChunkReply";	    

    case PKT_TYPE_THUMB_FILE_REQ:					return "PktThumbFileReq";               
    case PKT_TYPE_THUMB_FILE_REPLY:			        return "PktThumbFileReply";			    
    case PKT_TYPE_THUMB_FILE_CHUNK_REQ:				return "PktThumbFileChunkReq";		    
    case PKT_TYPE_THUMB_FILE_CHUNK_REPLY:			return "PktThumbFileChunkReply";		

    case PKT_TYPE_HOST_ANNOUNCE:					return "PktHostAnnounce";               

    case PKT_TYPE_BLOB_SEND_REQ:					return "PkBlobSendReq";                 
    case PKT_TYPE_BLOB_SEND_REPLY:				    return "PktBlobSendReply";			    
    case PKT_TYPE_BLOB_CHUNK_REQ:				    return "PktBlobChunkReq";			    
    case PKT_TYPE_BLOB_CHUNK_REPLY:				    return "PktBlobChunkReply";			    
    case PKT_TYPE_BLOB_SEND_COMPLETE_REQ:		    return "PktBlobSendCompleteReq";		
    case PKT_TYPE_BLOB_SEND_COMPLETE_REPLY:		    return "PktBlobSendCompleteReply ";	    
    case PKT_TYPE_BLOB_XFER_ERR:					return "PktBlobXferErr";                

    case PKT_TYPE_HOST_JOIN_REQ:		            return "PktHostJoinReq";	            
    case PKT_TYPE_HOST_JOIN_REPLY:					return "PktHostJoinReply";	            
    case PKT_TYPE_HOST_LEAVE_REQ:		            return "PktHostLeaveReq";	            
    case PKT_TYPE_HOST_LEAVE_REPLY:					return "PktHostLeaveReply";	            

    case PKT_TYPE_HOST_OFFER_REQ:		            return "PktHostOfferReq";	            
    case PKT_TYPE_HOST_OFFER_REPLY:					return "PktHostOfferReply";	            

    case PKT_TYPE_HOST_SEARCH_REQ:		            return "PktHostSearchReq";	            
    case PKT_TYPE_HOST_SEARCH_REPLY:                return "PktHostSearchReply";	        

    case PKT_TYPE_FRIEND_OFFER_REQ:		            return "PktFriendOfferReq";	            
    case PKT_TYPE_FRIEND_OFFER_REPLY:               return "PktFriendOfferReply";	        

    case PKT_TYPE_OFFER_SEND_REQ:					return "PktOfferSendReq";               
    case PKT_TYPE_OFFER_SEND_REPLY:					return "PktOfferSendReply";			    
    case PKT_TYPE_OFFER_CHUNK_REQ:					return "PktOfferChunkReq";			    
    case PKT_TYPE_OFFER_CHUNK_REPLY:				return "PktOfferChunkReply";			
    case PKT_TYPE_OFFER_SEND_COMPLETE_REQ:			return "PktOfferSendCompleteReq";       
    case PKT_TYPE_OFFER_SEND_COMPLETE_REPLY:		return "PktOfferSendCompleteReply";	    
    case PKT_TYPE_OFFER_XFER_ERR:					return "PktOfferXferErr";               

    case PKT_TYPE_WEB_SERVER_MY_PIC_SEND_REQ:		return "PktMyPicSendReq";				
    case PKT_TYPE_WEB_SERVER_MY_PIC_SEND_REPLY:		return "PktMyPicSendReply";             
    case PKT_TYPE_WEB_SERVER_PIC_CHUNK_TX:			return "PktWebServerPicChunkTx";        
    case PKT_TYPE_WEB_SERVER_PIC_CHUNK_ACK:			return "PktWebServerPicChunkAck";		
    case PKT_TYPE_WEB_SERVER_GET_CHUNK_TX:			return "PktWebServerGetChunkTx";        
    case PKT_TYPE_WEB_SERVER_GET_CHUNK_ACK:			return "PktWebServerGetChunkAck";       
    case PKT_TYPE_WEB_SERVER_PUT_CHUNK_TX:			return "PktWebServerPutChunkTx";        
    case PKT_TYPE_WEB_SERVER_PUT_CHUNK_ACK:			return "PktWebServerPutChunkAck";       

    case PKT_TYPE_PUSH_TO_TALK_REQ:				    return "PktPushToTalkReq";		        
    case PKT_TYPE_PUSH_TO_TALK_REPLY:				return "PktPushToTalkReply";            

    case PKT_TYPE_HOST_INFO_REQ:				    return "PktHostInfoReq";                
    case PKT_TYPE_HOST_INFO_REPLY:			        return "PktHostInfoReply";              

    case PKT_TYPE_HOST_INVITE_ANN_REQ:				return "PktHostInviteAnnReq";		    
    case PKT_TYPE_HOST_INVITE_ANN_REPLY:			return "PktHostInviteAnnReply";         
    case PKT_TYPE_HOST_INVITE_SEARCH_REQ:			return "PktHostInviteSearchReq";		
    case PKT_TYPE_HOST_INVITE_SEARCH_REPLY:			return "PktHostInviteSearchReply";      
    case PKT_TYPE_HOST_INVITE_MORE_REQ:				return "PktHostInviteMoreReq";          
    case PKT_TYPE_HOST_INVITE_MORE_REPLY:			return "PktHostInviteMoreReply";        

    case PKT_TYPE_GROUPIE_INFO_REQ:				    return "PktGroupieInfoReq";             
    case PKT_TYPE_GROUPIE_INFO_REPLY:			    return "PktGroupieInfoReply";           

    case PKT_TYPE_GROUPIE_ANN_REQ:				    return "PktGroupieAnnReq";              
    case PKT_TYPE_GROUPIE_ANN_REPLY:			    return "PktGroupieAnnReply";            
    case PKT_TYPE_GROUPIE_SEARCH_REQ:				return "PktGroupieSearchReq";		    
    case PKT_TYPE_GROUPIE_SEARCH_REPLY:			    return "PktGroupieSearchReply";         
    case PKT_TYPE_GROUPIE_MORE_REQ:				    return "PktGroupieMoreReq";             
    case PKT_TYPE_GROUPIE_MORE_REPLY:               return "PktGroupieMoreReply";           

    case PKT_TYPE_FILE_INFO_INFO_REQ:				return "PktFileInfoReq";                
    case PKT_TYPE_FILE_INFO_INFO_REPLY:			    return "PktFileInfoReply";              

    case PKT_TYPE_FILE_INFO_ANN_REQ:				return "PktFileInfoAnnReq";             
    case PKT_TYPE_FILE_INFO_ANN_REPLY:			    return "PktFileInfoAnnReply";           
    case PKT_TYPE_FILE_INFO_SEARCH_REQ:				return "PktFileInfoSearchReq";          
    case PKT_TYPE_FILE_INFO_SEARCH_REPLY:			return "PktFileInfoSearchReply";        
    case PKT_TYPE_FILE_INFO_MORE_REQ:				return "PktFileInfoMoreReq";            
    case PKT_TYPE_FILE_INFO_MORE_REPLY:			    return "PktFileInfoMoreReply";
    default:
        return "PktUnknown";
    }
}
