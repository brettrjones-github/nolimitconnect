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

#include <vector>

class GuiUserHost;
class VxGUID;

class GuiUserHostList
{
public:
	GuiUserHostList();
	~GuiUserHostList() = default;

    //! copy constructor
    GuiUserHostList( const GuiUserHostList & rhs );
    //! copy operator
    GuiUserHostList& operator =( const GuiUserHostList & rhs );

	int							size( void )						{ return (int)m_UserHostList.size(); }

	void						addUserHost( GuiUserHost* guiUserHost );

    bool                        removeUserHost( GuiUserHost* guiUserHost );
    bool						removeUserHost( VxGUID& thumbId );

    GuiUserHost*					findUserHost( VxGUID& thumbId );

	// returns false if guid already exists
	bool						addUserHostIfDoesntExist( GuiUserHost* guiUserHost );
	// return true if guid is in list
	bool						doesUserHostExist( GuiUserHost* guiUserHost );

	void						clearList( void );

	std::vector<GuiUserHost*>&		getUserHostList( void )					{ return m_UserHostList; }
	void						copyTo( GuiUserHostList& destUserHostList );

protected:
	std::vector<GuiUserHost*>		m_UserHostList;
};
