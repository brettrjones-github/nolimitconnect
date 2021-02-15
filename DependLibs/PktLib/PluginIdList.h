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

#include "PluginId.h"
#include <vector>

class PluginIdList
{
public:
	PluginIdList();
	~PluginIdList() = default;

	int							size( void )						{ return (int)m_PluginIdList.size(); }

	void						addPluginId( const PluginId& guid );
	// returns false if id already exists
	bool						addPluginIdIfDoesntExist( const PluginId& guid );
	// return true if id is in list
	bool						doesPluginIdExist( const PluginId& guid );
	// returns false if id did not exists
	bool						removePluginId( PluginId& guid );
	void						clearList( void );

	std::vector<PluginId>&		getPluginIdList( void )					{ return m_PluginIdList; }
	void						copyTo( PluginIdList& destPluginIdList );

protected:
	std::vector<PluginId>		m_PluginIdList;

private:
	PluginIdList( const PluginIdList &rhs ); // disable copy constructor
};
