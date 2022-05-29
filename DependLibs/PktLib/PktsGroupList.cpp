//============================================================================
// Copyright (C) 2015 Brett R. Jones 
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

#include "PktsGroupList.h"
#include "PktTypes.h"
#include "../CoreLib/VxDefs.h"

#include <CoreLib/IsBigEndianCpu.h>
#include <CoreLib/VxChop.h>

#include <memory.h>

#ifdef _MSC_VER
# pragma warning( disable: 4200 ) //warning C4200: nonstandard extension used : zero-sized array in struct/union
#endif // _MSC_VER

namespace
{
	typedef struct
	{
		uint8_t						m_GroupHashData[20];
		uint64_t					m_GroupLen;
		uint16_t					m_GroupNameLen;
		uint8_t						m_GroupTypeFlags;
		char						m_GroupName[260];	
	}PackedGroupListEntry;
}

//============================================================================
// Group list request
//============================================================================

//============================================================================
PktGroupListReq::PktGroupListReq()
: m_u16Res(0)
, m_u32Index(0)
{
	setPktType( PKT_TYPE_GROUP_LIST_REQ );
	setPktLength( sizeof( PktGroupListReq ) );
}

//============================================================================
void PktGroupListReq::setListIndex( uint32_t index )
{
	m_u32Index = htonl( index );
}

//============================================================================
uint32_t PktGroupListReq::getListIndex( void )
{
	return ntohl( m_u32Index );
}

//============================================================================
// Group list reply
//============================================================================

//============================================================================
PktGroupListReply::PktGroupListReply()
: m_bListComplete(0)
, m_u8Res1(0)
, m_u16Res(0)
, m_u16DataLen(0)
, m_u16GroupCnt(0)
, m_u32Error(0)
, m_u32ListIndex(0)
{
	memset( m_as8GroupList, 0, sizeof( m_as8GroupList ) );
	setPktType( PKT_TYPE_GROUP_LIST_REPLY );
	calcPktLen( 0 );
}

//============================================================================
void PktGroupListReply::calcPktLen( uint16_t dataLen )
{
	m_u16DataLen = htons( dataLen );
	setPktLength( (uint16_t)ROUND_TO_16BYTE_BOUNDRY( sizeof( PktGroupListReply ) - MAX_SHARED_GROUP_LIST_LEN  + dataLen ) );
}

//============================================================================
void PktGroupListReply::setError( uint32_t u32Error )
{
	m_u32Error = htonl( u32Error );
}

//============================================================================
uint32_t PktGroupListReply::getError( void )
{
	 return ntohl( m_u32Error );
}

//============================================================================
void PktGroupListReply::setGroupCount( uint16_t GroupCount )
{
	m_u16GroupCnt = htons( GroupCount );
}

//============================================================================
uint16_t PktGroupListReply::getGroupCount( void )
{
	return ntohs( m_u16GroupCnt );
}

//============================================================================
void PktGroupListReply::setListDataLen( uint16_t dataLen )
{
	m_u16DataLen = ntohs( dataLen );
}

//============================================================================
uint16_t PktGroupListReply::getListDataLen( void )
{
	return ntohs( m_u16DataLen );
}

//============================================================================
void PktGroupListReply::setListIndex( uint32_t index )
{
	m_u32ListIndex = htonl( index );
}

//============================================================================
uint32_t PktGroupListReply::getListIndex( void )
{
	return ntohl( m_u32ListIndex );
}

//============================================================================
void PktGroupListReply::getGroupList( std::vector<VxGroupInfo>& retGroupList )
{
	uint16_t GroupCnt = getGroupCount();
	uint32_t dataOffset = 0;

	for( uint16_t cnt = 0; cnt < GroupCnt; cnt++ )
	{
		PackedGroupListEntry * entry = (PackedGroupListEntry *)(&m_as8GroupList[dataOffset]);

		//char as8Buf[ VX_MAX_PATH ];
		//VxUnchopStr( (unsigned char *)entry->m_GroupName, as8Buf );
		//VxGroupInfo GroupInfo( entry->m_GroupName );
		//GroupInfo.setGroupHashData( entry->m_GroupHashData );
		//GroupInfo.setGroupLength( ntohU64( entry->m_GroupLen ) );
		//GroupInfo.setGroupType( entry->m_GroupTypeFlags );
		//dataOffset += ROUND_TO_4BYTE_BOUNDRY( sizeof( PackedGroupListEntry ) - 260 + htons( entry->m_GroupNameLen ) );

		//retGroupList.push_back( GroupInfo );
	}
}

//============================================================================
bool PktGroupListReply::canAddGroup( int GroupNameLenIncludingZero )
{
	uint32_t curLen = getListDataLen();
	uint32_t addLen = sizeof( PackedGroupListEntry ) - 260 + GroupNameLenIncludingZero + 20;
	return ( curLen + addLen ) < MAX_SHARED_GROUP_LIST_LEN;
}

//============================================================================
void PktGroupListReply::addGroup( VxGroupInfo& GroupHashId, uint64_t GroupLen, uint8_t GroupTypeFlags, const char * GroupName )
{
	//uint32_t curLen = getListDataLen();
	//PackedGroupListEntry * entry = (PackedGroupListEntry *)(&m_as8GroupList[ curLen ]);
	//memcpy( entry->m_GroupHashData, GroupHashId.getHashData(), 20 );
	//entry->m_GroupLen = htonU64( GroupLen );
	//entry->m_GroupTypeFlags = GroupTypeFlags;
	////(uint16_t)VxChopStr( GroupName, (unsigned char *)entry->m_GroupName );
	//uint16_t choppedLen = (uint16_t)(strlen( GroupName ) + 1);
	//strcpy( entry->m_GroupName, GroupName );
	//entry->m_GroupNameLen = htons( choppedLen );
	//// if we do not align will sometimes get illegal alignment error
	//uint16_t dataLen = (uint16_t)ROUND_TO_4BYTE_BOUNDRY( (curLen + sizeof( PackedGroupListEntry ) - 260 + choppedLen ) );
	//setListDataLen( dataLen );
	//calcPktLen( dataLen );
	//setGroupCount( getGroupCount() + 1 );
}
