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
#include "VxSearchFlags.h"
#include "VxCommon.h"
#include <GuiInterface/IScan.h>

#define PKT_ANN_MAX_SEARCH_STR_LEN (255 + 16)
#define PKT_ANN_MAX_IDENT_MATCH (40)

#pragma pack(push)
#pragma pack(1)
class PktScanReq : public VxPktHdr, public VxSearchFlags
{
public:
	PktScanReq();

	void						calcPktLen( void );

	void						setSearchType( EScanType eScanType );
	EScanType					getSearchType( void );
	bool						setSearchExpression( const char * pExp );
	const char *				getSearchExpression( void );

private:
	//=== vars ===//
	uint8_t						m_u8SearchType;
	uint8_t						m_u8Res1;
	uint32_t					m_u32Res3;
	char						m_as8SearchExpression[ PKT_ANN_MAX_SEARCH_STR_LEN ];
};

class PktScanReply : public VxPktHdr, public VxSearchFlags
{
public:
	PktScanReply();

	void calcPktLen( void );

	void						setSearchType( EScanType eScanType );
	EScanType					getSearchType( void );
	void						setSizeOfConnectInfo( uint16_t connectInfoSize )	{ m_u16SizeOfConnectInfo = htons( connectInfoSize ); }
	uint16_t					getSizeOfConnectInfo( void )					    { return ntohs( m_u16SizeOfConnectInfo ); }
	void						setIdentMatchCount( uint16_t identCnt )				{ m_u16IdentMatchCount = htons( identCnt ); }
	uint16_t					getIdentMatchCount( void )						    { return ntohs( m_u16IdentMatchCount ); }
	void						setTotalIdentCount( uint16_t identCnt )				{ m_u16TotalIdentCount = htons( identCnt ); }
	uint16_t					getTotalIdentCount( void )						    { return ntohs( m_u16TotalIdentCount ); }

	uint8_t *					getSearchMatch( void )							    { return m_aoSearchMatch; }
private:
	//=== vars ===//
	uint8_t						m_u8SearchType;
	uint8_t						m_u8Res1;
	uint16_t					m_u16IdentMatchCount;
	uint16_t					m_u16TotalIdentCount;
	uint16_t					m_u16SizeOfConnectInfo;
	uint16_t					m_u16Res2;
	uint8_t						m_aoSearchMatch[ PKT_ANN_MAX_IDENT_MATCH * sizeof(VxConnectInfo) + 64 ];
};

#pragma pack(pop)






