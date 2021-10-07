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

class GuiHostJoin;
class VxGUID;

class GuiHostJoinList
{
public:
	GuiHostJoinList();
	~GuiHostJoinList() = default;

    //! copy constructor
    GuiHostJoinList( const GuiHostJoinList & rhs );
    //! copy operator
    GuiHostJoinList& operator =( const GuiHostJoinList & rhs );

	int								size( void )						{ return (int)m_UserJoinList.size(); }

	void							addUserJoin( GuiHostJoin* GuiHostJoin );

    bool							removeUserJoin( GuiHostJoin* GuiHostJoin );
    bool							removeUserJoin( VxGUID& thumbId );

    GuiHostJoin*					findUserJoin( VxGUID& thumbId );

	// returns false if guid already exists
	bool							addUserJoinIfDoesntExist( GuiHostJoin* GuiHostJoin );
	// return true if guid is in list
	bool							doesUserJoinExist( GuiHostJoin* GuiHostJoin );

	void							clearList( void );

	std::vector<GuiHostJoin*>&		getUserJoinList( void )					{ return m_UserJoinList; }
	void							copyTo( GuiHostJoinList& destUserJoinList );

protected:
	std::vector<GuiHostJoin*>		m_UserJoinList;
};
