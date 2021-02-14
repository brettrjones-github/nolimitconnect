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
// http://www.nolimitconnect.com
//============================================================================

#include "VxPktHdr.h"
#include "VxCommon.h"

#include <CoreLib/VxDebug.h>

#define PKT_ANN_ANNOUNCE_VERSION 1
#define P2P_ENGINE_VERSION 1

#pragma pack(push) 
#pragma pack(1)

//=== request FLAGS ===//
#define FLAG_PKT_ANN_REQ_REPLY					0x01
#define FLAG_PKT_ANN_REQ_REV_CONNECT			0x02
#define FLAG_PKT_ANN_REQ_STUN					0x04
#define FLAG_PKT_ANN_REQ_TOP_TEN				0x08

// +  1 m_u8TimeToLive
// +  1 m_u8RequestFlags
// +  2 m_u8Res16
// + 28 reserved (7 * 4) 
// = 32 bytes
class PktAnnActionData
{
public:
    PktAnnActionData() = default; // do not set = default
    PktAnnActionData( const PktAnnActionData& rhs );
    PktAnnActionData&		    operator = ( const PktAnnActionData& rhs );
    bool                        addToBlob( PktBlobEntry& blob );
    bool                        extractFromBlob( PktBlobEntry& blob );

	void						setTTL( uint8_t timeToLive )					{ m_u8TimeToLive = timeToLive; }
	uint8_t						getTTL( void )							        { return m_u8TimeToLive; }
	void						setIsTopTenRequested( bool enable );
	bool						getIsTopTenRequested( void );

	void						setIsPktAnnReplyRequested( bool bReqReply );
	bool						getIsPktAnnReplyRequested( void );
	void						setIsPktAnnRevConnectRequested( bool bReqConnect );
	bool						getIsPktAnnRevConnectRequested( void );
	void						setIsPktAnnStunRequested( bool bReqStun );
	bool						getIsPktAnnStunRequested( void );

    void						setAppAliveTimeSec( uint32_t aliveTime )				{ m_u32AppAliveTimeSec = aliveTime; }
    uint32_t					getAppAliveTimeSec( void )						        { return m_u32AppAliveTimeSec; }

private:
	//=== vars ===//
    uint8_t						m_u8TimeToLive{0};		
    uint8_t						m_u8RequestFlags{0};	// request flags
    uint16_t				    m_u8Res16{0};
    uint32_t				    m_u32AppAliveTimeSec{0};
    uint32_t				    m_u32ActionRes1{0};		// Action Flags				.. not currently used
    uint32_t					m_u32ActionRes2{0};		// Action Data				.. not currently used
    uint32_t					m_u32ActionRes3{0};
    uint32_t					m_u32ActionRes4{0};
    uint32_t					m_u32ActionRes5{0};
    uint32_t					m_u32ActionRes6{0};
};

//     8 bytes reserved
// +  40 bytes VxPktHdr
// + 576 bytes VxNetIdent
// = 624 bytes
class PktAnnBase : public VxPktHdr, public VxNetIdent
{
public:
    PktAnnBase() = default;
    PktAnnBase( const PktAnnBase& rhs );
    PktAnnBase&		            operator = ( const PktAnnBase& rhs );
    bool                        addToBlob( PktBlobEntry& blob );
    bool                        extractFromBlob( PktBlobEntry& blob );

    uint32_t				    m_AnnRes1{0};
    uint32_t				    m_AnnRes2{0};	
};

//! is what we put in normal pkt announce so we can do actions etc
//! size 
//   624 bytes PktAnnBase
// +  32 bytes PktAnnActionData
// = 656 bytes total
class PktAnnounce :  public PktAnnBase,  public PktAnnActionData
{
public:
	PktAnnounce();
    PktAnnounce( const PktAnnounce& rhs );
    PktAnnounce&				operator = ( const PktAnnounce& rhs );
    bool                        addToBlob( PktBlobEntry& blob );
    bool                        extractFromBlob( PktBlobEntry& blob );

	bool						isValidPktAnn( void );

	bool						isOnlineStatusExpired( void );
	bool						isNearbyStatusExpired( void );
	void						updateNearbyPermissions( void );

	PktAnnounce *				makeAnnCopy( void );
	PktAnnounce *				makeAnnReverseCopy( void );
	void						DebugDump( void );
};

#pragma pack(pop)

