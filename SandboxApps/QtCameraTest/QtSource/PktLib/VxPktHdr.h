#pragma once
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

#include <GoTvDependLibrariesConfig.h>

#include <CoreLib/VxGUID.h>

//NOTE: all packets must be a multiple of 16 in length ( to optimize encryption )
//NOTE: all packets must have the following packet header

#ifndef MAX_PKT_LEN
    //NOTE: packets greater than 16384 may be blocked by routers/ satellite etc
    #define MAX_PKT_LEN						16384
#endif //MAX_PKT_LEN

#pragma pack(push) 
#pragma pack(1)

// size 8 bytes
class VxPktHdrPrefix
{
public:
    VxPktHdrPrefix();

    //=== return true if valid pkt type and length ===//
    bool						isValidPkt( void );
	bool						isNetServicePkt( void );

    //! return true if data length is large enough to contain this packet
    bool						isPktAllHere(int iDataLen);
	void						setPktLength( uint16_t pktLen );
	uint16_t					getPktLength( void );

	void						setPktType( uint16_t u8PktType );
	uint16_t					getPktType( void );

	void						setPluginNum( uint8_t u8PluginNum );
	uint8_t						getPluginNum( void );

	void						setPktVersionNum( uint8_t u8PktVersionNum );
	uint8_t						getPktVersionNum( void );

	void						setPktSeqNum( uint8_t seqNum )					{ m_u8SeqNum = seqNum; }
	uint8_t						getPktSeqNum( void )							{ return m_u8SeqNum; }

private:
	//=== vars ===//
	uint16_t					m_u16PktLen;			// length of packet ( must be a multiple of 16 )
	uint16_t					m_u16PktType;			// Type of packet ( defined by plugin 1-200..zero is reserved )
	uint8_t						m_u8PluginNum;	

	uint8_t						m_u8SeqNum;				// sequence number for lost packets and replay attack prevention
	uint8_t						m_u8PktHdrVersion;		// pkt header version number
	uint8_t						m_u8PktVersion;			// pkt specific version number
};

//	  8 bytes VxPktHdrPrefix
// + 16 bytes SrcOnlineId
// + 16 bytes DestOnlineId
// = 40 bytes total
class VxPktHdr : public VxPktHdrPrefix
{
public:
    VxGUID						getSrcOnlineId( void );
    void						setSrcOnlineId( VxGUID onlineId )			{ setGuidToNetOrder( onlineId, m_SrcOnlineId ); }
    VxGUID						getDestOnlineId( void );
    void						setDestOnlineId( VxGUID onlineId )			{ setGuidToNetOrder( onlineId, m_DestOnlineId ); }

    //! make a copy of this packet
    VxPktHdr *					makeCopy( void );

    static void                 setGuidToNetOrder( VxGUID& srcGuid, VxGUID& destGuid );
    static VxGUID               getGuidInHostOrder( VxGUID& srcGuid );

private:
	//=== vars ===//
	VxGUID					    m_SrcOnlineId;
	VxGUID					    m_DestOnlineId;
};

#pragma pack(pop)