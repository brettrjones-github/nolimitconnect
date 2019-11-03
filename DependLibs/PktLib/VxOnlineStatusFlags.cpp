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

#include "VxOnlineStatusFlags.h"


#define ONLINE_STATUS_CONNECTED				0x01
#define ONLINE_STATUS_NEARBY				0x02
#define ONLINE_STATUS_FROM_SEARCH_PKT		0x04 // is from search list.. never connected to
#define ONLINE_STATUS_HAS_TEXT_OFFERS		0x08 

VxOnlineStatusFlags::VxOnlineStatusFlags()
: m_u8OnlineStatusFlags(0)
{
}


void		VxOnlineStatusFlags::setIsOnline( bool bIsOnline )				{ if( bIsOnline )(m_u8OnlineStatusFlags |= ONLINE_STATUS_CONNECTED); else m_u8OnlineStatusFlags &= (~ONLINE_STATUS_CONNECTED); }
bool		VxOnlineStatusFlags::isOnline( void )								{ return (m_u8OnlineStatusFlags & ONLINE_STATUS_CONNECTED)?1:0; }
void		VxOnlineStatusFlags::setIsNearby( bool bIsNearby )				{ if( bIsNearby )(m_u8OnlineStatusFlags |= ONLINE_STATUS_NEARBY); else m_u8OnlineStatusFlags &= (~ONLINE_STATUS_NEARBY); }
bool		VxOnlineStatusFlags::isNearby( void )							{ return (m_u8OnlineStatusFlags & ONLINE_STATUS_NEARBY)?1:0; }
void		VxOnlineStatusFlags::setHasTextOffers( bool hasOffers )			{ if( hasOffers )(m_u8OnlineStatusFlags |= ONLINE_STATUS_HAS_TEXT_OFFERS); else m_u8OnlineStatusFlags &= (~ONLINE_STATUS_HAS_TEXT_OFFERS); }
bool		VxOnlineStatusFlags::getHasTextOffers( void )					{ return (m_u8OnlineStatusFlags & ONLINE_STATUS_HAS_TEXT_OFFERS)?1:0; }


void		VxOnlineStatusFlags::setIsFromSearchPkt( bool bIsFromSearch )		{ if( bIsFromSearch )(m_u8OnlineStatusFlags |= ONLINE_STATUS_FROM_SEARCH_PKT); else m_u8OnlineStatusFlags &= (~ONLINE_STATUS_NEARBY); }
bool		VxOnlineStatusFlags::isFromSearchPkt( void )						{ return (m_u8OnlineStatusFlags & ONLINE_STATUS_FROM_SEARCH_PKT)?1:0; }
