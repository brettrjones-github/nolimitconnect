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

#include "GuiHostedList.h"
#include "GuiHosted.h"
#include "GuiUser.h"

//============================================================================
GuiHostedList::GuiHostedList()
: m_HostedList()
{
}

//============================================================================
GuiHostedList::GuiHostedList( const GuiHostedList& rhs )
    : m_HostedList( rhs.m_HostedList )
{
}

//============================================================================
GuiHostedList& GuiHostedList::operator =( const GuiHostedList& rhs )
{
    if( this != &rhs )
    {
        m_HostedList          = rhs.m_HostedList;
    }

    return *this;
}

//============================================================================
bool GuiHostedList::addHostedIfDoesntExist( GuiHosted* guiHosted )
{
	if( doesHostedExist( guiHosted ) )
	{
		return false;
	}
	
	addHosted( guiHosted );
	return true;
}

//============================================================================
void GuiHostedList::addHosted( GuiHosted* guiHosted )
{
    m_HostedList.push_back( guiHosted );
}

//============================================================================
bool GuiHostedList::doesHostedExist( GuiHosted* guiHosted )
{
	for( auto thumb : m_HostedList )
	{
		if( guiHosted == thumb )
		{
			return true;
		}
	}

	return false;
}

//============================================================================
GuiHosted* GuiHostedList::findHosted( VxGUID& thumbId )
{
    for( auto iter = m_HostedList.begin(); iter != m_HostedList.end(); ++iter )
    {
        if( (*iter)->getUser()->getMyOnlineId() == thumbId )
        {
            return *iter;
        }
    }

    return nullptr;
}

//============================================================================
bool GuiHostedList::removeHosted( GuiHosted* guiHosted )
{
    if( guiHosted )
    {
        return removeHosted( guiHosted->getUser()->getMyOnlineId() );
    }

    return false;
}

//============================================================================
bool GuiHostedList::removeHosted( VxGUID& thumbId )
{
    bool joinExisted = false;
    std::vector<GuiHosted*>::iterator iter;
    for( iter = m_HostedList.begin(); iter != m_HostedList.end(); ++iter )
    {
        if( (*iter)->getUser()->getMyOnlineId() == thumbId )
        {
            joinExisted = true;
            m_HostedList.erase( iter );
            break;
        }
    }

    return joinExisted;
}

//============================================================================
void GuiHostedList::clearList( void )
{
	m_HostedList.clear();
}

//============================================================================
void GuiHostedList::copyTo( GuiHostedList& destHostedList )
{
	std::vector<GuiHosted*>& joinList = destHostedList.getHostedList();
	std::vector<GuiHosted*>::iterator iter;
	for( iter = m_HostedList.begin(); iter != m_HostedList.end(); ++iter )
	{
        joinList.push_back( *iter );
	}
}
