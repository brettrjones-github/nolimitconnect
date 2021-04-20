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

#include "GuiUserJoinList.h"
#include "GuiUserJoin.h"

//============================================================================
GuiUserJoinList::GuiUserJoinList()
: m_UserJoinList()
{
}

//============================================================================
GuiUserJoinList::GuiUserJoinList( const GuiUserJoinList& rhs )
    : m_UserJoinList( rhs.m_UserJoinList )
{
}

//============================================================================
GuiUserJoinList& GuiUserJoinList::operator =( const GuiUserJoinList& rhs )
{
    if( this != &rhs )
    {
        m_UserJoinList          = rhs.m_UserJoinList;
    }

    return *this;
}

//============================================================================
bool GuiUserJoinList::addUserJoinIfDoesntExist( GuiUserJoin* guiUserJoin )
{
	if( doesUserJoinExist( guiUserJoin ) )
	{
		return false;
	}
	
	addUserJoin( guiUserJoin );
	return true;
}

//============================================================================
void GuiUserJoinList::addUserJoin( GuiUserJoin* guiUserJoin )
{
    m_UserJoinList.push_back( guiUserJoin );
}

//============================================================================
bool GuiUserJoinList::doesUserJoinExist( GuiUserJoin* guiUserJoin )
{
	for( auto thumb : m_UserJoinList )
	{
		if( guiUserJoin == thumb )
		{
			return true;
		}
	}

	return false;
}

//============================================================================
GuiUserJoin* GuiUserJoinList::findUserJoin( VxGUID& thumbId )
{
    for( auto iter = m_UserJoinList.begin(); iter != m_UserJoinList.end(); ++iter )
    {
        if( (*iter)->getMyOnlineId() == thumbId )
        {
            return *iter;
        }
    }

    return nullptr;
}

//============================================================================
bool GuiUserJoinList::removeUserJoin( GuiUserJoin* guiUserJoin )
{
    if( guiUserJoin )
    {
        return removeUserJoin( guiUserJoin->getMyOnlineId() );
    }

    return false;
}

//============================================================================
bool GuiUserJoinList::removeUserJoin( VxGUID& thumbId )
{
    bool thumbExisted = false;
    std::vector<GuiUserJoin*>::iterator iter;
    for( iter = m_UserJoinList.begin(); iter != m_UserJoinList.end(); ++iter )
    {
        if( (*iter)->getMyOnlineId() == thumbId )
        {
            thumbExisted = true;
            m_UserJoinList.erase( iter );
            break;
        }
    }

    return thumbExisted;
}

//============================================================================
void GuiUserJoinList::clearList( void )
{
	m_UserJoinList.clear();
}

//============================================================================
void GuiUserJoinList::copyTo( GuiUserJoinList& destUserJoinList )
{
	std::vector<GuiUserJoin*>& thumbList = destUserJoinList.getUserJoinList();
	std::vector<GuiUserJoin*>::iterator iter;
	for( iter = m_UserJoinList.begin(); iter != m_UserJoinList.end(); ++iter )
	{
		thumbList.push_back( *iter );
	}
}
