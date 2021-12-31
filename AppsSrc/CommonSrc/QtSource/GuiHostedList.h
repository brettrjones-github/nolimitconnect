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

class GuiHosted;
class VxGUID;

class GuiHostedList
{
public:
	GuiHostedList();
	~GuiHostedList() = default;

    //! copy constructor
    GuiHostedList( const GuiHostedList & rhs );
    //! copy operator
    GuiHostedList& operator =( const GuiHostedList & rhs );

	int								size( void )						{ return (int)m_HostedList.size(); }

	void							addHosted( GuiHosted* guiHosted );

    bool							removeHosted( GuiHosted* guiHosted );
    bool							removeHosted( VxGUID& thumbId );

    GuiHosted*						findHosted( VxGUID& thumbId );

	// returns false if guid already exists
	bool							addHostedIfDoesntExist( GuiHosted* guiHosted );
	// return true if guid is in list
	bool							doesHostedExist( GuiHosted* GuiHosted );

	void							clearList( void );

	std::vector<GuiHosted*>&		getHostedList( void )					{ return m_HostedList; }
	void							copyTo( GuiHostedList& destHostedList );

protected:
	std::vector<GuiHosted*>			m_HostedList;
};
