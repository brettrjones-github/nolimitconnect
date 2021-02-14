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

#include "MatchParams.h"
#include "PktBlobEntry.h"

MatchParams::MatchParams( const MatchParams& rhs )
    : m_AgeType(rhs.m_AgeType)
    , m_GenderType(rhs.m_GenderType)
    , m_LanguageType(rhs.m_LanguageType)
    , m_ContentRating(rhs.m_ContentRating)
{
}

//============================================================================
MatchParams& MatchParams::operator =( const MatchParams& rhs )
{
	if( this != &rhs )
	{
        m_AgeType               = rhs.m_AgeType;
        m_GenderType            = rhs.m_GenderType;
        m_LanguageType          = rhs.m_LanguageType;
        m_ContentRating         = rhs.m_ContentRating;
	}

	return *this;
}

//============================================================================
bool MatchParams::addToBlob( PktBlobEntry& blob )
{
    bool result = blob.setValue( m_AgeType );
    result &= blob.setValue( m_GenderType );
    result &= blob.setValue( m_LanguageType );
    result &= blob.setValue( m_ContentRating );
    return result;
}

//============================================================================
bool MatchParams::extractFromBlob( PktBlobEntry& blob )
{
    bool result = blob.getValue( m_AgeType );
    result &= blob.getValue( m_GenderType );
    result &= blob.getValue( m_LanguageType );
    result &= blob.getValue( m_ContentRating );
    return result;

}