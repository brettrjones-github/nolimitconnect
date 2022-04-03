#pragma once
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

#include <GuiInterface/IDefs.h>

class PktBlobEntry;

class MatchParams
{
public:
	MatchParams() = default;
    MatchParams( const MatchParams& rhs );
	virtual ~MatchParams() = default;
	MatchParams&				operator =( const MatchParams& rhs );

    virtual bool                addToBlob( PktBlobEntry& blob );
    virtual bool                extractFromBlob( PktBlobEntry& blob );

    void						setAgeType( enum EAgeType age )                  { m_AgeType = age; }
    EAgeType					getAgeType( void )                          { return m_AgeType; }

    void						setGender( enum EGenderType gender )             { m_GenderType = gender; }
    EGenderType					getGender( void )                           { return m_GenderType; }

    void						setLanguage( enum ELanguageType language )       { m_LanguageType = language; }
    ELanguageType				getLanguage( void )                         { return m_LanguageType; }

    void						setContentRating( enum EContentRating content )  { m_ContentRating = content; }
    EContentRating			    getContentRating( void )                    { return m_ContentRating; }

protected:
	//=== vars ===//
    EAgeType					m_AgeType{ eAgeTypeUnspecified };
    EGenderType					m_GenderType{ eGenderTypeUnspecified };
    ELanguageType				m_LanguageType{ eLangUnspecified };
    EContentRating				m_ContentRating{ eContentRatingUnspecified };
};


