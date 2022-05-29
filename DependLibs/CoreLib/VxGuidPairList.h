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
// http://www.nolimitconnect.org
//============================================================================

#include "VxGUID.h"
#include <vector>
#include <utility>

class VxGuidPairList
{
public:
	VxGuidPairList();
	~VxGuidPairList() = default;

    //! copy constructor
    VxGuidPairList( const VxGuidPairList & rhs );
    //! copy operator
    VxGuidPairList& operator =( const VxGuidPairList & rhs );

	int							size( void )							{ return (int)m_GuidPairList.size(); }

	void						addGuid( const VxGUID& guid1, const VxGUID& guid2 );
	// returns false if guid already exists
	bool						addGuidIfDoesntExist( const VxGUID& guid1, const VxGUID& guid2 );
	// return true if guid is in list
	bool						doesGuidExist( const VxGUID& guid1, const VxGUID& guid2, uint64_t timeoutMs = 0 );
	// returns false if guid did not exists
	bool						removeGuid( const VxGUID& guid1, const VxGUID& guid2 );
	void						removeExpired( uint64_t& timeNowMs, uint64_t timeoutMs );
	void						clearList( void );

	std::vector<std::pair<VxGUID, std::pair<VxGUID, uint64_t>>>&	getGuidList( void )						{ return m_GuidPairList; }

protected:
	std::vector<std::pair<VxGUID, std::pair<VxGUID, uint64_t>>>	m_GuidPairList;
};
