#pragma once
//============================================================================
// Copyright (C) 2014 Brett R. Jones
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

#include "VxGUID.h"
#include <vector>

class VxGUIDList
{
public:
	VxGUIDList();
	~VxGUIDList() = default;

	int							size( void )						{ return (int)m_GuidList.size(); }

	void						addGuid( const VxGUID& guid );
	// returns false if guid already exists
	bool						addGuidIfDoesntExist( const VxGUID& guid );
	// return true if guid is in list
	bool						doesGuidExist( const VxGUID& guid );
	// returns false if guid did not exists
	bool						removeGuid( VxGUID& guid );
	void						clearList( void );

	std::vector<VxGUID>&		getGuidList( void )					{ return m_GuidList; }
	void						copyTo( VxGUIDList& destGuidList );

protected:
	std::vector<VxGUID>			m_GuidList;

private:
	VxGUIDList( const VxGUIDList &rhs ); // disable copy constructor
};
