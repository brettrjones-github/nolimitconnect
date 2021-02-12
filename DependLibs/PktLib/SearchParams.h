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

#include "MatchParams.h"
#include <CoreLib/VxGUID.h>

class SearchParams : public MatchParams
{
public:
	SearchParams() = default;
    SearchParams( const SearchParams& rhs );
	virtual ~SearchParams() = default;

	SearchParams&				operator =( const SearchParams& rhs );

    void						setHostType( EHostType hostType )						    { m_HostType = hostType; }
    EHostType					getHostType( void )								            { return m_HostType; }

    void						setSearchType( ESearchType searchType )						{ m_SearchType = searchType; }
    ESearchType					getSearchType( void )								        { return m_SearchType; }

    // search session (unique per search instance)
    void						setSearchSessionId( VxGUID& guid )						    { m_SearchSessionId = guid; }
    VxGUID&					    getSearchSessionId( void )								    { return m_SearchSessionId; }

    // search for identity (for person or host)
    void						setSearchIdentGuid( VxGUID& guid )							{ m_SearchIdentGuid = guid; }
    VxGUID&					    getSearchIndentGuid( void )									{ return m_SearchIdentGuid; }

    void						setSearchUrl( std::string& url )							{ m_SearchUrl = url; }
    std::string&				getSearchUrl( void )									    { return m_SearchUrl; }

	void						setSearchText( std::string& text )							{ m_SearchText = text; }
    std::string&				getSearchText( void )									    { return m_SearchText; }

    virtual bool                addToBlob( PktBlobEntry& blob ) override;
    virtual bool                extractFromBlob( PktBlobEntry& blob ) override;

protected:
	//=== vars ===//
    EHostType                   m_HostType{ eHostTypeUnknown };
    ESearchType                 m_SearchType{ eSearchNone };
    VxGUID					    m_SearchSessionId;
    VxGUID					    m_SearchIdentGuid;
    std::string					m_SearchUrl;
	std::string					m_SearchText;
};


