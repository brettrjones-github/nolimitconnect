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
// http://www.nolimitconnect.org
//============================================================================

#include <NlcDependLibrariesConfig.h>
#include <CoreLib/StdMinMaxForWindows.h>

#include "PktsPluginOffer.h"

#include <CoreLib/VxDebug.h>

#include <string.h>
#include <string>

//============================================================================
PktPluginOfferReq::PktPluginOfferReq()
{
	setPktType( PKT_TYPE_PLUGIN_OFFER_REQ );
	setPktLength( sizeof( PktPluginOfferReq ) );

	vx_assert( 0 == (getPktLength() & 0x0f) );
}

//============================================================================
void PktPluginOfferReq::calcPktLen( void )
{
	uint16_t pktLen = (uint16_t)sizeof( PktPluginOfferReq ) - sizeof( PktBlobEntry );
	pktLen += getBlobEntry().getTotalBlobLen();
	setPktLength( ROUND_TO_16BYTE_BOUNDRY( pktLen ) );

	vx_assert( 0 == (getPktLength() & 0x0f) );
}

//============================================================================
PktPluginOfferReply::PktPluginOfferReply()
{
	setPktType(  PKT_TYPE_PLUGIN_OFFER_REPLY );
	setPktLength(  sizeof( PktPluginOfferReply ) );

	vx_assert( 0 == ( getPktLength() & 0x0f ) );
}

//============================================================================
void PktPluginOfferReply::calcPktLen( void )
{
	uint16_t pktLen = (uint16_t)sizeof( PktPluginOfferReply ) - sizeof( PktBlobEntry );
	pktLen += getBlobEntry().getTotalBlobLen();
	setPktLength( ROUND_TO_16BYTE_BOUNDRY( pktLen ) );

	vx_assert( 0 == (getPktLength() & 0x0f) );
}

