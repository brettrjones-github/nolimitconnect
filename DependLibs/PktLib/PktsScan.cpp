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

#include "PktTypes.h"
#include "PktsScan.h"

#include <string.h>

//============================================================================
PktScanReq::PktScanReq()
    : m_u8SearchType(0)
    , m_u8Res1(0)
    , m_u32Res3(0)
{
	setPktType( PKT_TYPE_SCAN_REQ );
    m_as8SearchExpression[0] = 0;
}

//============================================================================
void PktScanReq::calcPktLen( void )
{
    setPktLength( ROUND_TO_16BYTE_BOUNDRY( sizeof( PktScanReq ) - PKT_ANN_MAX_SEARCH_STR_LEN + strlen(m_as8SearchExpression) + 1 ) );
}

//============================================================================
void			PktScanReq::setSearchType( EScanType eScanType )		{ m_u8SearchType = (uint8_t)eScanType; }
EScanType		PktScanReq::getSearchType( void )						{ return (EScanType)m_u8SearchType; }
//============================================================================
bool			PktScanReq::setSearchExpression( const char* pExp )
{
	m_as8SearchExpression[0] = 0;
	if( pExp )
	{
        size_t len = strlen( pExp );
		if( len < sizeof( m_as8SearchExpression ) )
		{
			strcpy( m_as8SearchExpression, pExp );
			return true;
		}
	}
	return false;
}

//============================================================================
const char* PktScanReq::getSearchExpression( void )
{ 
	return m_as8SearchExpression; 
}

//============================================================================
PktScanReply::PktScanReply()
: m_u8SearchType(0)
, m_u8Res1(0)
, m_u16IdentMatchCount(0)
, m_u16TotalIdentCount(0)
, m_u16Res2(0)
{
    setPktType( PKT_TYPE_SCAN_REPLY );
    calcPktLen();
}

//============================================================================
void PktScanReply::calcPktLen( void )
{
    setPktLength( ROUND_TO_16BYTE_BOUNDRY( sizeof( PktScanReply ) - sizeof(m_aoSearchMatch) + getTotalIdentCount() * sizeof( VxConnectInfo ) ) );
}

//============================================================================
void			PktScanReply::setSearchType( EScanType eScanType )		{ m_u8SearchType = (uint8_t)eScanType; }
EScanType		PktScanReply::getSearchType( void )						{ return (EScanType)m_u8SearchType; }



