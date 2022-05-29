#pragma once
//============================================================================
// Copyright (C) 2003 Brett R. Jones 
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

#define MAX_PKT_LEN						16384	// some routers wont route pkts greater than this
#define MAX_PKT_ERR_MSG_LEN				1023	// maximum length of pkt error message
#define MAX_ARG_LEN						255		// maximum length of command argument

#define BASE_PKT_TYPE								0 // packet type 0 is not allowed


#define PKT_TYPE_ANNOUNCE							( BASE_PKT_TYPE	+ 1 )					// 1
#define PKT_TYPE_ANN_LIST							( PKT_TYPE_ANNOUNCE	+ 1 )				// 2 ( 0x02 )

#define PKT_TYPE_LOG_REQ							( PKT_TYPE_ANN_LIST	+ 1 )				// 3 ( 0x03 )
#define PKT_TYPE_LOG_REPLY							( PKT_TYPE_LOG_REQ	+ 1 )				// 4 ( 0x04 )

#define PKT_TYPE_SEARCH_REQ							( PKT_TYPE_LOG_REPLY	+ 1 )			// 5 ( 0x05 )
#define PKT_TYPE_SEARCH_REPLY						( PKT_TYPE_SEARCH_REQ	+ 1 )			// 6 ( 0x06 )

#define PKT_TYPE_PLUGIN_OFFER_REQ					( PKT_TYPE_SEARCH_REPLY	+ 1 )			// 7 ( 0x07 )
#define PKT_TYPE_PLUGIN_OFFER_REPLY					( PKT_TYPE_PLUGIN_OFFER_REQ	+ 1 )		// 8 ( 0x08 )

#define PKT_TYPE_CHAT_REQ							( PKT_TYPE_PLUGIN_OFFER_REPLY	+ 1 )	// 9 ( 0x09 )
#define PKT_TYPE_CHAT_REPLY							( PKT_TYPE_CHAT_REQ	+ 1 )				// 10 ( 0x0a )

#define PKT_TYPE_VOICE_REQ							( PKT_TYPE_CHAT_REPLY	+ 1 )			// 11 ( 0x0b )
#define PKT_TYPE_VOICE_REPLY						( PKT_TYPE_VOICE_REQ	+ 1 )			// 12 ( 0x0c )

#define PKT_TYPE_VIDEO_FEED_REQ						( PKT_TYPE_VOICE_REPLY	+ 1 )			// 13 ( 0x0d )
#define PKT_TYPE_VIDEO_FEED_STATUS					( PKT_TYPE_VIDEO_FEED_REQ	+ 1 )		// 14 ( 0x0e )
#define PKT_TYPE_VIDEO_FEED_PIC						( PKT_TYPE_VIDEO_FEED_STATUS	+ 1 )	// 15 ( 0x0f )
#define PKT_TYPE_VIDEO_FEED_PIC_CHUNK				( PKT_TYPE_VIDEO_FEED_PIC	+ 1 )		// 16 ( 0x10 )
#define PKT_TYPE_VIDEO_FEED_PIC_ACK					( PKT_TYPE_VIDEO_FEED_PIC_CHUNK	+ 1 )	// 17 ( 0x11 )

#define PKT_TYPE_SESSION_START_REQ					( PKT_TYPE_VIDEO_FEED_PIC_ACK	+ 1 )	// 18 ( 0x12 )
#define PKT_TYPE_SESSION_START_REPLY				( PKT_TYPE_SESSION_START_REQ	+ 1 )	// 19 ( 0x13 )
#define PKT_TYPE_SESSION_STOP_REQ					( PKT_TYPE_SESSION_START_REPLY	+ 1 )	// 20 ( 0x14 )
#define PKT_TYPE_SESSION_STOP_REPLY					( PKT_TYPE_SESSION_STOP_REQ	+ 1 )		// 21 ( 0x15 )

#define PKT_TYPE_FILE_GET_REQ						( PKT_TYPE_SESSION_STOP_REPLY	+ 1 )	// 22 ( 0x16 )
#define PKT_TYPE_FILE_GET_REPLY						( PKT_TYPE_FILE_GET_REQ	+ 1 )			// 23 ( 0x17 )
// HTTP = 0x48545450
#define PKT_TYPE_FILE_SEND_REQ						( PKT_TYPE_FILE_GET_REPLY	+ 1 )		// 24 ( 0x18 )
#define PKT_TYPE_FILE_SEND_REPLY					( PKT_TYPE_FILE_SEND_REQ	+ 1 )		// 25 ( 0x19 )
#define PKT_TYPE_FILE_CHUNK_REQ						( PKT_TYPE_FILE_SEND_REPLY	+ 1 )		// 26 ( 0x1a )
#define PKT_TYPE_FILE_CHUNK_REPLY					( PKT_TYPE_FILE_CHUNK_REQ	+ 1 )		// 27 ( 0x1b )

#define PKT_TYPE_FILE_GET_COMPLETE_REQ				( PKT_TYPE_FILE_CHUNK_REPLY	+ 1 )			// 28 ( 0x1c )
#define PKT_TYPE_FILE_GET_COMPLETE_REPLY			( PKT_TYPE_FILE_GET_COMPLETE_REQ	+ 1 )	// 29 ( 0x1d )
#define PKT_TYPE_FILE_SEND_COMPLETE_REQ				( PKT_TYPE_FILE_GET_COMPLETE_REPLY	+ 1 )	// 30 ( 0x1e )
#define PKT_TYPE_FILE_SEND_COMPLETE_REPLY			( PKT_TYPE_FILE_SEND_COMPLETE_REQ	+ 1 )	// 31 ( 0x1f )

#define PKT_TYPE_FILE_FIND_REQ						( PKT_TYPE_FILE_SEND_COMPLETE_REPLY	+ 1 )	// 32 ( 0x20 )
#define PKT_TYPE_FILE_FIND_REPLY					( PKT_TYPE_FILE_FIND_REQ	+ 1 )			// 33 ( 0x21 )
#define PKT_TYPE_FILE_LIST_REQ						( PKT_TYPE_FILE_FIND_REPLY	+ 1 )			// 34 ( 0x22 )
#define PKT_TYPE_FILE_LIST_REPLY					( PKT_TYPE_FILE_LIST_REQ	+ 1 )			// 35 ( 0x23 )
#define PKT_TYPE_FILE_INFO_REQ						( PKT_TYPE_FILE_LIST_REPLY	+ 1 )			// 36 ( 0x24 )
#define PKT_TYPE_FILE_INFO_REPLY					( PKT_TYPE_FILE_INFO_REQ	+ 1 )			// 37 ( 0x25 )
#define PKT_TYPE_FILE_SHARE_ERR						( PKT_TYPE_FILE_INFO_REPLY	+ 1 )			// 38 ( 0x26 )

#define PKT_TYPE_ASSET_SEND_REQ						( PKT_TYPE_FILE_SHARE_ERR	+ 1 )				// 39 ( 0x27 )
#define PKT_TYPE_ASSET_SEND_REPLY					( PKT_TYPE_ASSET_SEND_REQ	+ 1 )				// 40 ( 0x28 )
#define PKT_TYPE_ASSET_CHUNK_REQ					( PKT_TYPE_ASSET_SEND_REPLY	+ 1 )				// 41 ( 0x29 )
#define PKT_TYPE_ASSET_CHUNK_REPLY					( PKT_TYPE_ASSET_CHUNK_REQ	+ 1 )				// 42 ( 0x2A )
#define PKT_TYPE_ASSET_SEND_COMPLETE_REQ			( PKT_TYPE_ASSET_CHUNK_REPLY	+ 1 )			// 43 ( 0x2B )
#define PKT_TYPE_ASSET_SEND_COMPLETE_REPLY			( PKT_TYPE_ASSET_SEND_COMPLETE_REQ + 1 )		// 44 ( 0x2C )
#define PKT_TYPE_ASSET_XFER_ERR						( PKT_TYPE_ASSET_SEND_COMPLETE_REPLY + 1 )		// 45 ( 0x2D )

#define PKT_TYPE_RELAY_SERVICE_REQ					( PKT_TYPE_ASSET_XFER_ERR	+ 1 )				// 46 ( 0x2E )
#define PKT_TYPE_RELAY_SERVICE_REPLY				( PKT_TYPE_RELAY_SERVICE_REQ	+ 1 )			// 47 ( 0x2f )
#define PKT_TYPE_RELAY_CONNECT_REQ					( PKT_TYPE_RELAY_SERVICE_REPLY	+ 1 )			// 48 ( 0x30 )
#define PKT_TYPE_RELAY_CONNECT_REPLY				( PKT_TYPE_RELAY_CONNECT_REQ	+ 1 )			// 49 ( 0x31 )
#define PKT_TYPE_RELAY_SESSION_REQ					( PKT_TYPE_RELAY_CONNECT_REPLY	+ 1 )			// 50 ( 0x32 )
#define PKT_TYPE_RELAY_SESSION_REPLY				( PKT_TYPE_RELAY_SESSION_REQ	+ 1 )			// 51 ( 0x33 )
#define PKT_TYPE_RELAY_CONNECT_TO_USER_REQ			( PKT_TYPE_RELAY_SESSION_REPLY	+ 1 )			// 52 ( 0x34 )
#define PKT_TYPE_RELAY_CONNECT_TO_USER_REPLY		( PKT_TYPE_RELAY_CONNECT_TO_USER_REQ	+ 1 )	// 53 ( 0x35 )
#define PKT_TYPE_RELAY_USER_DISCONNECT				( PKT_TYPE_RELAY_CONNECT_TO_USER_REPLY	+ 1 )	// 54 ( 0x36 )
#define PKT_TYPE_RELAY_TEST_REQ						( PKT_TYPE_RELAY_USER_DISCONNECT	+ 1 )		// 55 ( 0x37 )		
#define PKT_TYPE_RELAY_TEST_REPLY					( PKT_TYPE_RELAY_TEST_REQ	+ 1 )				// 56 ( 0x38 )

#define PKT_TYPE_WEB_SERVER_MY_PIC_SEND_REQ			( PKT_TYPE_RELAY_TEST_REPLY	+ 1 )				// 57 ( 0x39 )
#define PKT_TYPE_WEB_SERVER_MY_PIC_SEND_REPLY		( PKT_TYPE_WEB_SERVER_MY_PIC_SEND_REQ	+ 1 )	// 58 ( 0x3A )
#define PKT_TYPE_WEB_SERVER_PIC_CHUNK_TX			( PKT_TYPE_WEB_SERVER_MY_PIC_SEND_REPLY	+ 1 )	// 59 ( 0x3B )
#define PKT_TYPE_WEB_SERVER_PIC_CHUNK_ACK			( PKT_TYPE_WEB_SERVER_PIC_CHUNK_TX	+ 1 )		// 60 ( 0x3c )
#define PKT_TYPE_WEB_SERVER_GET_CHUNK_TX			( PKT_TYPE_WEB_SERVER_PIC_CHUNK_ACK	+ 1 )       // 61 ( 0x3d )
#define PKT_TYPE_WEB_SERVER_GET_CHUNK_ACK			( PKT_TYPE_WEB_SERVER_GET_CHUNK_TX	+ 1 )       // 62 ( 0x3e )
#define PKT_TYPE_WEB_SERVER_PUT_CHUNK_TX			( PKT_TYPE_WEB_SERVER_GET_CHUNK_ACK	+ 1 )       // 63 ( 0x3f )
#define PKT_TYPE_WEB_SERVER_PUT_CHUNK_ACK			( PKT_TYPE_WEB_SERVER_PUT_CHUNK_TX	+ 1 )       // 64 ( 0x40 )

#define PKT_TYPE_STORY_DATA_REQ						( PKT_TYPE_WEB_SERVER_PUT_CHUNK_ACK	+ 1 )       // 65 ( 0x41 )
#define PKT_TYPE_STORY_DATA_REPLY					( PKT_TYPE_STORY_DATA_REQ	+ 1 )               // 66 ( 0x42 )

#define PKT_TYPE_MSESSION_REQ						( PKT_TYPE_STORY_DATA_REPLY	+ 1 )               // 67 ( 0x43 )
#define PKT_TYPE_MSESSION_REPLY						( PKT_TYPE_MSESSION_REQ	+ 1 )                   // 68 ( 0x44 )

#define PKT_TYPE_TCP_PUNCH							( PKT_TYPE_MSESSION_REPLY	+ 1 )	        // 69 ( 0x45 )
#define PKT_TYPE_PING_REQ							( PKT_TYPE_TCP_PUNCH	+ 1 )		        // 70 ( 0x46 )
#define PKT_TYPE_PING_REPLY							( PKT_TYPE_PING_REQ	+ 1 )           		// 71 ( 0x47 )
#define PKT_TYPE_IM_ALIVE_REQ						( PKT_TYPE_PING_REPLY	+ 1 )	            // 72 ( 0x48 )
#define PKT_TYPE_IM_ALIVE_REPLY						( PKT_TYPE_IM_ALIVE_REQ	+ 1 )		        // 73 ( 0x49 )

#define PKT_TYPE_GROUP_SEND_REQ						( PKT_TYPE_IM_ALIVE_REQ	+ 1 )		        // 74 ( 0x4a )
#define PKT_TYPE_GROUP_SEND_REPLY					( PKT_TYPE_GROUP_SEND_REQ	+ 1 )		    // 75 ( 0x4b )
#define PKT_TYPE_GROUP_CHUNK_REQ					( PKT_TYPE_GROUP_SEND_REPLY	+ 1 )		    // 76 ( 0x4c )
#define PKT_TYPE_GROUP_CHUNK_REPLY					( PKT_TYPE_GROUP_CHUNK_REQ	+ 1 )		    // 77 ( 0x4d )

#define PKT_TYPE_GROUP_GET_COMPLETE_REQ				( PKT_TYPE_GROUP_CHUNK_REPLY	+ 1 )		// 78 ( 0x4e )
#define PKT_TYPE_GROUP_GET_COMPLETE_REPLY			( PKT_TYPE_GROUP_GET_COMPLETE_REQ	+ 1 )	// 79 ( 0x4f )
#define PKT_TYPE_GROUP_SEND_COMPLETE_REQ			( PKT_TYPE_GROUP_GET_COMPLETE_REPLY	+ 1 )	// 80 ( 0x50 )
#define PKT_TYPE_GROUP_SEND_COMPLETE_REPLY			( PKT_TYPE_GROUP_SEND_COMPLETE_REQ	+ 1 )	// 81 ( 0x51 )

#define PKT_TYPE_GROUP_FIND_REQ						( PKT_TYPE_GROUP_SEND_COMPLETE_REPLY + 1 )	// 82 ( 0x52 )
#define PKT_TYPE_GROUP_FIND_REPLY					( PKT_TYPE_GROUP_FIND_REQ	+ 1 )			// 83 ( 0x53 )
#define PKT_TYPE_GROUP_LIST_REQ						( PKT_TYPE_GROUP_FIND_REPLY	+ 1 )			// 84 ( 0x54 )
#define PKT_TYPE_GROUP_LIST_REPLY					( PKT_TYPE_GROUP_LIST_REQ	+ 1 )			// 85 ( 0x55 )
#define PKT_TYPE_GROUP_INFO_REQ						( PKT_TYPE_GROUP_LIST_REPLY	+ 1 )			// 86 ( 0x56 )
#define PKT_TYPE_GROUP_INFO_REPLY					( PKT_TYPE_GROUP_INFO_REQ	+ 1 )			// 87 ( 0x57 )
#define PKT_TYPE_GROUP_SHARE_ERR					( PKT_TYPE_GROUP_INFO_REPLY	+ 1 )			// 88 ( 0x58 )

#define PKT_TYPE_TOD_GAME_STATS						( PKT_TYPE_GROUP_SHARE_ERR	+ 1 )           // 89 ( 0x59 )
#define PKT_TYPE_TOD_GAME_ACTION					( PKT_TYPE_TOD_GAME_STATS	+ 1 )			// 90 ( 0x5A )
#define PKT_TYPE_TOD_GAME_VALUE						( PKT_TYPE_TOD_GAME_ACTION	+ 1 )			// 91 ( 0x5B )

#define PKT_TYPE_PLUGIN_SETTING_REQ					( PKT_TYPE_TOD_GAME_VALUE	+ 1 )			// 92 ( 0x5C )
#define PKT_TYPE_PLUGIN_SETTING_REPLY			    ( PKT_TYPE_PLUGIN_SETTING_REQ	+ 1 )		// 93 ( 0x5D )

#define PKT_TYPE_THUMB_SETTING_REQ					( PKT_TYPE_PLUGIN_SETTING_REPLY	+ 1 )		// 94 ( 0x5E )
#define PKT_TYPE_THUMB_SETTING_REPLY			    ( PKT_TYPE_THUMB_SETTING_REQ	+ 1 )		// 95 ( 0x5F )
#define PKT_TYPE_THUMB_SETTING_CHUNK_REQ			( PKT_TYPE_THUMB_SETTING_REPLY	+ 1 )		// 96 ( 0x60 )
#define PKT_TYPE_THUMB_SETTING_CHUNK_REPLY			( PKT_TYPE_THUMB_SETTING_CHUNK_REQ	+ 1 )	// 97 ( 0x61 )

#define PKT_TYPE_THUMB_AVATAR_REQ					( PKT_TYPE_THUMB_SETTING_CHUNK_REPLY + 1 )	// 98 ( 0x62 )
#define PKT_TYPE_THUMB_AVATAR_REPLY			        ( PKT_TYPE_THUMB_AVATAR_REQ	+ 1 )			// 99 ( 0x63 )
#define PKT_TYPE_THUMB_AVATAR_CHUNK_REQ				( PKT_TYPE_THUMB_AVATAR_REPLY	+ 1 )		// 100 ( 0x64 )
#define PKT_TYPE_THUMB_AVATAR_CHUNK_REPLY			( PKT_TYPE_THUMB_AVATAR_CHUNK_REQ	+ 1 )	// 101 ( 0x65 )

#define PKT_TYPE_THUMB_FILE_REQ					    ( PKT_TYPE_THUMB_AVATAR_CHUNK_REPLY	+ 1 )	// 102 ( 0x66 )
#define PKT_TYPE_THUMB_FILE_REPLY			        ( PKT_TYPE_THUMB_FILE_REQ	+ 1 )			// 103 ( 0x67 )
#define PKT_TYPE_THUMB_FILE_CHUNK_REQ				( PKT_TYPE_THUMB_FILE_REPLY	+ 1 )		    // 104 ( 0x68 )
#define PKT_TYPE_THUMB_FILE_CHUNK_REPLY			    ( PKT_TYPE_THUMB_FILE_CHUNK_REQ	+ 1 )		// 105 ( 0x69 )

#define MAX_PKT_TYPE_CNT							(PKT_TYPE_THUMB_FILE_CHUNK_REPLY + 1)

#define ROUND_TO_16BYTE_BOUNDRY( a ) (( a + 15 ) & ~15 )	//round up to even 16 byte boundary
#define ROUND_TO_4BYTE_BOUNDRY( a ) (( a + 3 ) & ~3 )	    //round up to even 4 byte boundary

