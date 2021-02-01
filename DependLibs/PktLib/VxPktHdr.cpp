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

#include <memory.h>

//from ascii chart
// h=0x68 t=0x74 p=0x70
// http = 0x68747470
#define PKT_TYPE_NET_SERVICES						0x6874
#define PKT_LENGTH_NET_SERVICES						0x7470

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
	return ( ( PKT_TYPE_NET_SERVICES == getPktType() ) 
			&& ( PKT_LENGTH_NET_SERVICES == getPktLength() ) );
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
VxPktHdr * VxPktHdr::makeCopy( void )
{
    uint8_t * pu8Copy = new uint8_t[ getPktLength() ];
    memcpy( pu8Copy, this, getPktLength() );
    return (VxPktHdr *)pu8Copy;
}

//============================================================================
void  VxPktHdr::setGuidToNetOrder( VxGUID& srcGuid, VxGUID& destGuid )
{
    destGuid = srcGuid;
    destGuid.setToNetOrder();
}

//============================================================================
VxGUID  VxPktHdr::getGuidInHostOrder( VxGUID& srcGuid )
{
    VxGUID guidCopy = srcGuid;
    guidCopy.setToHostOrder();
    return guidCopy;
}
