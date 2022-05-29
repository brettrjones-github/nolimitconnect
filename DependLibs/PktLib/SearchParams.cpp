//============================================================================
// Copyright (C) 2021 Brett R. Jones 
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

#include "SearchParams.h"
#include "PktBlobEntry.h"

#include <CoreLib/VxTime.h>

//============================================================================
SearchParams::SearchParams( const SearchParams& rhs )
    : MatchParams(rhs)
    , m_HostType( rhs.m_HostType )
    , m_SearchType( rhs.m_SearchType )
    , m_SearchStartTimeGmtMs( rhs.m_SearchStartTimeGmtMs )
    , m_SearchSessionId( rhs.m_SearchSessionId )
    , m_SearchIdentGuid(rhs.m_SearchIdentGuid )
    , m_SearchUrl( rhs.m_SearchUrl )
    , m_SearchText( rhs.m_SearchText )
    , m_SearchListAll( rhs.m_SearchListAll )
{
}

//============================================================================
SearchParams& SearchParams::operator =( const SearchParams& rhs )
{
	if( this != &rhs )
	{
        *((MatchParams *)this) = (const MatchParams&)rhs;

        m_HostType              = rhs.m_HostType;
        m_SearchType            = rhs.m_SearchType;
        m_SearchStartTimeGmtMs  = rhs.m_SearchStartTimeGmtMs;
        m_SearchSessionId       = rhs.m_SearchSessionId;
        m_SearchIdentGuid       = rhs.m_SearchIdentGuid;
        m_SearchUrl		        = rhs.m_SearchUrl;
        m_SearchText		    = rhs.m_SearchText;
        m_SearchListAll         = rhs.m_SearchListAll;
	}

	return *this;
}

//============================================================================
bool SearchParams::addToBlob( PktBlobEntry& blob )
{
    bool result = MatchParams::addToBlob( blob );
    result &= blob.setValue( m_HostType );
    result &= blob.setValue( m_SearchType );
    result &= blob.setValue( m_SearchStartTimeGmtMs );
    result &= blob.setValue( m_SearchSessionId );
    result &= blob.setValue( m_SearchIdentGuid );
    result &= blob.setValue( m_SearchUrl );
    result &= blob.setValue( m_SearchText );
    result &= blob.setValue( m_SearchListAll );
    return result;
}

//============================================================================
bool SearchParams::extractFromBlob( PktBlobEntry& blob )
{
    bool result = MatchParams::extractFromBlob( blob );
    result &= blob.getValue( m_HostType );
    result &= blob.getValue( m_SearchType );
    result &= blob.getValue( m_SearchStartTimeGmtMs );
    result &= blob.getValue( m_SearchSessionId );
    result &= blob.getValue( m_SearchIdentGuid );
    result &= blob.getValue( m_SearchUrl );
    result &= blob.getValue( m_SearchText );
    result &= blob.getValue( m_SearchListAll );
    return result;
}

//============================================================================
void SearchParams::createNewSessionId( void )
{
    VxGUID::generateNewVxGUID( m_SearchSessionId );
}

//============================================================================
void SearchParams::updateSearchStartTime( void )
{
    m_SearchStartTimeGmtMs = GetGmtTimeMs();
}