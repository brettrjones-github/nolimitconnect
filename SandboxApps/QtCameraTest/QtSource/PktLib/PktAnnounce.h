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
#include "VxCommon.h"

#include <CoreLib/VxDebug.h>

#define PKT_ANN_ANNOUNCE_VERSION 1
#define P2P_ENGINE_VERSION 1

#pragma pack(push) 
#pragma pack(1)

class P2PEngineVersion
{
public:
    P2PEngineVersion();

    uint8_t							getP2PEngineVersion( void );
    void						    getP2PEngineVersion( std::string& strRetP2PEngineVersion );

private:
    uint8_t							m_u8P2PEngineVersion{0};
};

class MyOSVersion
{
public:
	MyOSVersion();

	uint8_t							getOSVersion( void );
	void						    getOSVersion( std::string& strRetOSVersion );

private:
    uint8_t							m_u8OSVersion{0};
};

//=== request FLAGS ===//
#define FLAG_PKT_ANN_REQ_REPLY					0x01
#define FLAG_PKT_ANN_REQ_REV_CONNECT			0x02
#define FLAG_PKT_ANN_REQ_STUN					0x04
#define FLAG_PKT_ANN_REQ_TOP_TEN				0x08

//    1 P2PEngineVersion
// +  1 MyOSVersion
// +  1 m_u8TimeToLive
// +  1 m_u8RequestFlags
// + 28 reserved
// = 32 bytes
class PktAnnActionData : public P2PEngineVersion, public MyOSVersion
{
public:
	PktAnnActionData();
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

private:
	//=== vars ===//
    uint8_t						m_u8TimeToLive{0};			//
    uint8_t						m_u8RequestFlags{0};		// request flags
    uint32_t				    m_u32ActionRes1{0};		// Action Flags				.. not currently used
    uint32_t					m_u32ActionRes2{0};		// Action Data				.. not currently used
    uint32_t					m_u32ActionRes3{0};
    uint32_t					m_u32ActionRes4{0};
    uint32_t					m_u32ActionRes5{0};
    uint32_t					m_u32ActionRes6{0};
    uint32_t					m_u32ActionRes7{0};
    uint32_t					m_u32ActionRes8{0};
};

//! this is the part of pkt announce that we would not expect to change often
//! can be used for quick check to see if we need to update info about friend to user
//! this is what we would send in list of pkt announcements
// + 196 bytes VxNetIdent
// +   4 bytes m_u32AppAliveTimeSec
// = 200 bytes total
class PktAnnBase : public VxNetIdent
{
public:
	PktAnnBase();

	void						setAppAliveTimeSec( uint32_t aliveTime )				{ m_u32AppAliveTimeSec = aliveTime; }
	uint32_t					getAppAliveTimeSec( void )						        { return m_u32AppAliveTimeSec; }

	bool						hasFriendDataChanged( PktAnnBase * poOther );

private:
	uint32_t					m_u32AppAliveTimeSec;		// how long application has been alive	
};


//! is what we put in normal pkt announce so we can do actions etc
//! size 
//	 200 bytes PktAnnBase
// +  40 bytes VxPktHdr
// +  32 bytes PktAnnActionData
// = 272 bytes total
class PktAnnounce :  public VxPktHdr, public PktAnnBase,  public PktAnnActionData
{
public:
	PktAnnounce();

	bool						isValidPktAnn( void );

	bool						isOnlineStatusExpired( void );
	bool						isNearbyStatusExpired( void );
	void						updateNearbyPermissions( void );

	PktAnnounce *				makeAnnCopy( void );
	PktAnnounce *				makeAnnReverseCopy( void );
	void						DebugDump( void );
};

#pragma pack(pop)

