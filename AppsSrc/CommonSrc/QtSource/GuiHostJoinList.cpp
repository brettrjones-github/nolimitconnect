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

#include <QWidget> // must be declared first or Qt 6.2.4 will error in qmetatype.h 2167:23: array subscript value ‘53’ is outside the bounds

#include "GuiHostJoinList.h"
#include "GuiHostJoin.h"
#include "GuiUser.h"

//============================================================================
GuiHostJoinList::GuiHostJoinList()
: m_UserJoinList()
{
}

//============================================================================
GuiHostJoinList::GuiHostJoinList( const GuiHostJoinList& rhs )
    : m_UserJoinList( rhs.m_UserJoinList )
{
}

//============================================================================
GuiHostJoinList& GuiHostJoinList::operator =( const GuiHostJoinList& rhs )
{
    if( this != &rhs )
    {
        m_UserJoinList          = rhs.m_UserJoinList;
    }

    return *this;
}

//============================================================================
bool GuiHostJoinList::addUserJoinIfDoesntExist( GuiHostJoin* GuiHostJoin )
{
	if( doesUserJoinExist( GuiHostJoin ) )
	{
		return false;
	}
	
	addUserJoin( GuiHostJoin );
	return true;
}

//============================================================================
void GuiHostJoinList::addUserJoin( GuiHostJoin* GuiHostJoin )
{
    m_UserJoinList.push_back( GuiHostJoin );
}

//============================================================================
bool GuiHostJoinList::doesUserJoinExist( GuiHostJoin* GuiHostJoin )
{
	for( auto thumb : m_UserJoinList )
	{
		if( GuiHostJoin == thumb )
		{
			return true;
		}
	}

	return false;
}

//============================================================================
GuiHostJoin* GuiHostJoinList::findUserJoin( VxGUID& thumbId )
{
    for( auto iter = m_UserJoinList.begin(); iter != m_UserJoinList.end(); ++iter )
    {
        if( (*iter)->getUser()->getMyOnlineId() == thumbId )
        {
            return *iter;
        }
    }

    return nullptr;
}

//============================================================================
bool GuiHostJoinList::removeUserJoin( GuiHostJoin* GuiHostJoin )
{
    if( GuiHostJoin )
    {
        return removeUserJoin( GuiHostJoin->getUser()->getMyOnlineId() );
    }

    return false;
}

//============================================================================
bool GuiHostJoinList::removeUserJoin( VxGUID& thumbId )
{
    bool joinExisted = false;
    std::vector<GuiHostJoin*>::iterator iter;
    for( iter = m_UserJoinList.begin(); iter != m_UserJoinList.end(); ++iter )
    {
        if( (*iter)->getUser()->getMyOnlineId() == thumbId )
        {
            joinExisted = true;
            m_UserJoinList.erase( iter );
            break;
        }
    }

    return joinExisted;
}

//============================================================================
void GuiHostJoinList::clearList( void )
{
	m_UserJoinList.clear();
}

//============================================================================
void GuiHostJoinList::copyTo( GuiHostJoinList& destUserJoinList )
{
	std::vector<GuiHostJoin*>& joinList = destUserJoinList.getUserJoinList();
	std::vector<GuiHostJoin*>::iterator iter;
	for( iter = m_UserJoinList.begin(); iter != m_UserJoinList.end(); ++iter )
	{
        joinList.push_back( *iter );
	}
}
