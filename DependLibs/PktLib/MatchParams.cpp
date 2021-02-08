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
    uint8_t ageValue = (uint8_t)m_AgeType;
    bool result = blob.setValue( ageValue );
    uint8_t genderValue = (uint8_t)m_GenderType;
    result &= blob.setValue( genderValue );
    uint16_t languageValue = (uint16_t)m_LanguageType;
    result &= blob.setValue( genderValue );
    uint8_t contentValue = (uint8_t)m_ContentRating;
    result &= blob.setValue( contentValue );
    return result;
}

//============================================================================
bool MatchParams::extractFromBlob( PktBlobEntry& blob )
{
    uint8_t ageValue{ 0 };
    bool result = blob.getValue( ageValue );
    uint8_t genderValue{ 0 };
    result &= blob.getValue( genderValue );
    uint16_t languageValue{ 0 };
    result &= blob.getValue( languageValue );
    uint8_t contentValue{ 0 };
    result &= blob.getValue( contentValue );
    if( result )
    {
        m_AgeType = (EAgeType)ageValue;
        m_GenderType = (EGenderType)genderValue;
        m_LanguageType = (ELanguageType)languageValue;
        m_ContentRating = (EContentRating)contentValue;
    }

    return result;

}