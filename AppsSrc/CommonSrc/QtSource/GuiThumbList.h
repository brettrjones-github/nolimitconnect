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

#include <vector>

class GuiThumb;
class VxGUID;

class GuiThumbList
{
public:
	GuiThumbList();
	~GuiThumbList() = default;

    //! copy constructor
    GuiThumbList( const GuiThumbList & rhs );
    //! copy operator
    GuiThumbList& operator =( const GuiThumbList & rhs );

	int							size( void )						{ return (int)m_ThumbList.size(); }

	void						addThumb( GuiThumb* guiThumb );

    bool                        removeThumb( GuiThumb* guiThumb );
    bool						removeThumb( VxGUID& thumbId );

    GuiThumb*					findThumb( VxGUID& thumbId );

	// returns false if guid already exists
	bool						addThumbIfDoesntExist( GuiThumb* guiThumb );
	// return true if guid is in list
	bool						doesThumbExist( GuiThumb* guiThumb );

	void						clearList( void );

	std::vector<GuiThumb*>&		getThumbList( void )					{ return m_ThumbList; }
	void						copyTo( GuiThumbList& destThumbList );

protected:
	std::vector<GuiThumb*>		m_ThumbList;
};
