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

    // search session (unique per search instance)
    void						setSearchSessionGuid( VxGUID& guid )						{ m_SearchSessionGuid = guid; }
    VxGUID&					    getSearchSessionGuid( void )								{ return m_SearchSessionGuid; }

    // search for identity (for person or host)
    void						setSearchIdentGuid( VxGUID& guid )							{ m_SearchIdentGuid = guid; }
    VxGUID&					    getSearchIndentGuid( void )									{ return m_SearchIdentGuid; }

	void						setSearchText( std::string& text )							{ m_SearchText = text; }
    std::string&				getSearchText( void )									    { return m_SearchText; }

    virtual bool                addToBlob( PktBlobEntry& blob ) override;
    virtual bool                extractFromBlob( PktBlobEntry& blob ) override;

protected:
	//=== vars ===//
    VxGUID					    m_SearchSessionGuid;
    VxGUID					    m_SearchIdentGuid;
	std::string					m_SearchText;
};


