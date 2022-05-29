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

#include "GuiThumbList.h"
#include "GuiThumb.h"

//============================================================================
GuiThumbList::GuiThumbList()
: m_ThumbList()
{
}

//============================================================================
GuiThumbList::GuiThumbList( const GuiThumbList& rhs )
    : m_ThumbList( rhs.m_ThumbList )
{
}

//============================================================================
GuiThumbList& GuiThumbList::operator =( const GuiThumbList& rhs )
{
    if( this != &rhs )
    {
        m_ThumbList          = rhs.m_ThumbList;
    }

    return *this;
}

//============================================================================
bool GuiThumbList::addThumbIfDoesntExist( GuiThumb* guiThumb )
{
	if( doesThumbExist( guiThumb ) )
	{
		return false;
	}
	
	addThumb( guiThumb );
	return true;
}

//============================================================================
void GuiThumbList::addThumb( GuiThumb* guiThumb )
{
    m_ThumbList.push_back( guiThumb );
}

//============================================================================
bool GuiThumbList::doesThumbExist( GuiThumb* guiThumb )
{
	for( auto thumb : m_ThumbList )
	{
		if( guiThumb == thumb )
		{
			return true;
		}
	}

	return false;
}

//============================================================================
GuiThumb* GuiThumbList::findThumb( VxGUID& thumbId )
{
    for( auto iter = m_ThumbList.begin(); iter != m_ThumbList.end(); ++iter )
    {
        if( (*iter)->getThumbId() == thumbId )
        {
            return *iter;
        }
    }

    return nullptr;
}

//============================================================================
bool GuiThumbList::removeThumb( GuiThumb* guiThumb )
{
    if( guiThumb )
    {
        return removeThumb( guiThumb->getThumbId() );
    }

    return false;
}

//============================================================================
bool GuiThumbList::removeThumb( VxGUID& thumbId )
{
    bool thumbExisted = false;
    std::vector<GuiThumb*>::iterator iter;
    for( iter = m_ThumbList.begin(); iter != m_ThumbList.end(); ++iter )
    {
        if( (*iter)->getThumbId() == thumbId )
        {
            thumbExisted = true;
            m_ThumbList.erase( iter );
            break;
        }
    }

    return thumbExisted;
}

//============================================================================
void GuiThumbList::clearList( void )
{
	m_ThumbList.clear();
}

//============================================================================
void GuiThumbList::copyTo( GuiThumbList& destThumbList )
{
	std::vector<GuiThumb*>& thumbList = destThumbList.getThumbList();
	std::vector<GuiThumb*>::iterator iter;
	for( iter = m_ThumbList.begin(); iter != m_ThumbList.end(); ++iter )
	{
		thumbList.push_back( *iter );
	}
}
