//============================================================================
// Copyright (C) 2010-2013 Brett R. Jones 
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

#include <AppDependLibrariesConfig.h>
#include <CoreLib/StdMinMaxForWindows.h>

#include "PktsPluginOffer.h"

#include <CoreLib/VxDebug.h>

#include <string.h>
#include <string>

//============================================================================
PktPluginOfferReq::PktPluginOfferReq()
: m_u16Msg1Len(0)
, m_u16FileNameLen(0)
, m_u8Version(1)
, m_u8Reserved(0)
, m_u16Res(0)
, m_u32Res1( 0 )
, m_u32Res2( 0 )
, m_u32Res3( 0 )
{
	setPktType( PKT_TYPE_PLUGIN_OFFER_REQ );
	setPktLength( sizeof( PktPluginOfferReq ) - sizeof( m_as8Args ) + 16 );
	m_as8Args[ 0 ] = 0;
	m_as8Args[ 1 ] = 0;
	uint16_t len1 = htons( 1 );
	m_u16Msg1Len = len1;
	m_u16FileNameLen = len1;

}

//============================================================================
void PktPluginOfferReq::setMessages( const char * pMsg1, const char * fileName )
{
	if( pMsg1 && strlen( pMsg1 ) )
	{
        int msgLen = std::min( (int)strlen( pMsg1 ), PKT_PLUGIN_OFFER_MAX_MSG_LEN );
		m_u16Msg1Len = htons( (uint16_t)( msgLen + 1 ) );
		strncpy( m_as8Args, pMsg1, msgLen );
		m_as8Args[ msgLen ] = 0;
	}
	else
	{
		m_u16Msg1Len = htons( 1 );
		m_as8Args[0] = 0;
	}

	if( fileName && strlen( fileName ) )
	{
		m_u16FileNameLen = htons( (uint16_t)( strlen( fileName ) + 1 ) );
		strcpy( &m_as8Args[  getOfferMsgLen() ], fileName );
	}
	else
	{
		m_u16FileNameLen = htons( 1 );
		m_as8Args[ getOfferMsgLen() ] = 0;
	}

	setPktLength( ROUND_TO_16BYTE_BOUNDRY( ((sizeof( PktPluginOfferReq ) - (PKT_PLUGIN_OFFER_MAX_MSG_LEN * 2)) + getOfferMsgLen() + getFileNameLen()) ) );
}

//============================================================================
const char * PktPluginOfferReq::getOfferMsg()
{
	return m_as8Args;
}

//============================================================================
const char * PktPluginOfferReq::getFileName()
{
	if( getFileNameLen() )
	{
		return &m_as8Args[ getOfferMsgLen() ];
	}

	return NULL;
}

//============================================================================
PktPluginOfferReply::PktPluginOfferReply()
: m_u8Version(1)
, m_u8Reserved(0)
, m_u8Response(0)
, m_u8Res2(0)
, m_u32Error(0)
, m_u64Arg2(0)
, m_u64Arg3(0)
, m_u32Res1( 0 )
, m_u32Res2( 0 )
, m_u32Res3( 0 )
{
	setPktType(  PKT_TYPE_PLUGIN_OFFER_REPLY );
	setPktLength(  sizeof( PktPluginOfferReply ) - sizeof( m_as8Arg1 ) + 16 );
	m_as8Arg1[0] = 0;
	m_as8Arg1[1] = 0;
	vx_assert( 0 == ( getPktLength() & 0x0f ) );
}

//============================================================================
void PktPluginOfferReply::calcPktLen( void )
{
	//calculate length of packet from length of text string
	setPktLength( (unsigned short)ROUND_TO_16BYTE_BOUNDRY( ((sizeof( PktPluginOfferReply ) - (PKT_PLUGIN_OFFER_MAX_MSG_LEN +1))  + strlen( m_as8Arg1 ) + 1 ) ) );
}






