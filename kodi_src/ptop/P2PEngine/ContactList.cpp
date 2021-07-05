//============================================================================
// Copyright (C) 2014 Brett R. Jones 
// Issued to MIT style license by Brett R. Jones in 2017
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

#include "ContactList.h"

#include <GoTvCore/GoTvP2P/BigListLib/BigListInfo.h>

//============================================================================
ContactList::ContactList()
{
}

//============================================================================
ContactList::~ContactList()
{

}

//============================================================================
void ContactList::addContactInfo( VxConnectInfo& connectInfo )
{
	bool bFound = false;
	std::vector<ConnectRequest>::iterator iter;
	m_ContactListMutex.lock();
	for( iter = m_ContactList.begin(); iter != m_ContactList.end(); ++iter )
	{
		if( (*iter).getMyOnlineId() == connectInfo.getMyOnlineId() )
		{
			(*iter) = connectInfo;
			bFound = true;
			break;
		}
	}

	if( false == bFound )
	{
		m_ContactList.push_back( ConnectRequest(connectInfo) );
	}

	m_ContactListMutex.unlock();
}

//============================================================================
void ContactList::removeContactInfo( VxConnectInfo& connectInfo )
{
	std::vector<ConnectRequest>::iterator iter;
	m_ContactListMutex.lock();
	for( iter = m_ContactList.begin(); iter != m_ContactList.end(); ++iter )
	{
		if( (*iter).getMyOnlineId() == connectInfo.getMyOnlineId() )
		{
			m_ContactList.erase(iter);
			break;
		}
	}

	m_ContactListMutex.unlock();
}

//============================================================================
bool ContactList::contactIsInList( VxGUID& onlineId )
{
	bool isInList = false;
	std::vector<ConnectRequest>::iterator iter;
	m_ContactListMutex.lock();
	for( iter = m_ContactList.begin(); iter != m_ContactList.end(); ++iter )
	{
		if( (*iter).getMyOnlineId() == onlineId )
		{
			isInList = true;
			break;
		}
	}

	m_ContactListMutex.unlock();

	return isInList;
}
