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

#include "GuiUserHostList.h"
#include "GuiUserHost.h"

//============================================================================
GuiUserHostList::GuiUserHostList()
: m_UserHostList()
{
}

//============================================================================
GuiUserHostList::GuiUserHostList( const GuiUserHostList& rhs )
    : m_UserHostList( rhs.m_UserHostList )
{
}

//============================================================================
GuiUserHostList& GuiUserHostList::operator =( const GuiUserHostList& rhs )
{
    if( this != &rhs )
    {
        m_UserHostList          = rhs.m_UserHostList;
    }

    return *this;
}

//============================================================================
bool GuiUserHostList::addUserHostIfDoesntExist( GuiUserHost* guiUserHost )
{
	if( doesUserHostExist( guiUserHost ) )
	{
		return false;
	}
	
	addUserHost( guiUserHost );
	return true;
}

//============================================================================
void GuiUserHostList::addUserHost( GuiUserHost* guiUserHost )
{
    m_UserHostList.push_back( guiUserHost );
}

//============================================================================
bool GuiUserHostList::doesUserHostExist( GuiUserHost* guiUserHost )
{
	for( auto thumb : m_UserHostList )
	{
		if( guiUserHost == thumb )
		{
			return true;
		}
	}

	return false;
}

//============================================================================
GuiUserHost* GuiUserHostList::findUserHost( VxGUID& thumbId )
{
    for( auto iter = m_UserHostList.begin(); iter != m_UserHostList.end(); ++iter )
    {
        if( (*iter)->getMyOnlineId() == thumbId )
        {
            return *iter;
        }
    }

    return nullptr;
}

//============================================================================
bool GuiUserHostList::removeUserHost( GuiUserHost* guiUserHost )
{
    if( guiUserHost )
    {
        return removeUserHost( guiUserHost->getMyOnlineId() );
    }

    return false;
}

//============================================================================
bool GuiUserHostList::removeUserHost( VxGUID& thumbId )
{
    bool thumbExisted = false;
    std::vector<GuiUserHost*>::iterator iter;
    for( iter = m_UserHostList.begin(); iter != m_UserHostList.end(); ++iter )
    {
        if( (*iter)->getMyOnlineId() == thumbId )
        {
            thumbExisted = true;
            m_UserHostList.erase( iter );
            break;
        }
    }

    return thumbExisted;
}

//============================================================================
void GuiUserHostList::clearList( void )
{
	m_UserHostList.clear();
}

//============================================================================
void GuiUserHostList::copyTo( GuiUserHostList& destUserHostList )
{
	std::vector<GuiUserHost*>& thumbList = destUserHostList.getUserHostList();
	std::vector<GuiUserHost*>::iterator iter;
	for( iter = m_UserHostList.begin(); iter != m_UserHostList.end(); ++iter )
	{
		thumbList.push_back( *iter );
	}
}
