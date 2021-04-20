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

class GuiUserJoin;
class VxGUID;

class GuiUserJoinList
{
public:
	GuiUserJoinList();
	~GuiUserJoinList() = default;

    //! copy constructor
    GuiUserJoinList( const GuiUserJoinList & rhs );
    //! copy operator
    GuiUserJoinList& operator =( const GuiUserJoinList & rhs );

	int							size( void )						{ return (int)m_UserJoinList.size(); }

	void						addUserJoin( GuiUserJoin* guiUserJoin );

    bool                        removeUserJoin( GuiUserJoin* guiUserJoin );
    bool						removeUserJoin( VxGUID& thumbId );

    GuiUserJoin*					findUserJoin( VxGUID& thumbId );

	// returns false if guid already exists
	bool						addUserJoinIfDoesntExist( GuiUserJoin* guiUserJoin );
	// return true if guid is in list
	bool						doesUserJoinExist( GuiUserJoin* guiUserJoin );

	void						clearList( void );

	std::vector<GuiUserJoin*>&		getUserJoinList( void )					{ return m_UserJoinList; }
	void						copyTo( GuiUserJoinList& destUserJoinList );

protected:
	std::vector<GuiUserJoin*>		m_UserJoinList;
};
