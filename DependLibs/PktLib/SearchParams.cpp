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
// http://www.nolimitconnect.com
//============================================================================

#include "SearchParams.h"
#include "PktBlobEntry.h"

//============================================================================
SearchParams::SearchParams( const SearchParams& rhs )
    : MatchParams(rhs)
    , m_SearchGuid(rhs.m_SearchGuid)
    , m_SearchText(rhs.m_SearchText)
{
}

//============================================================================
SearchParams& SearchParams::operator =( const SearchParams& rhs )
{
	if( this != &rhs )
	{
        *((MatchParams *)this) = (const MatchParams&)rhs;

        m_SearchGuid        = rhs.m_SearchGuid;
        m_SearchText		= rhs.m_SearchText;
	}

	return *this;
}

//============================================================================
bool SearchParams::addToBlob( PktBlobEntry& blob )
{
    bool result = MatchParams::addToBlob( blob );
    result &= blob.setValue( m_SearchGuid );
    result &= blob.setValue( m_SearchText );
    return result;
}

//============================================================================
bool SearchParams::extractFromBlob( PktBlobEntry& blob )
{
    bool result = MatchParams::extractFromBlob( blob );
    result &= blob.getValue( m_SearchGuid );
    result &= blob.getValue( m_SearchText );
    return result;
}
