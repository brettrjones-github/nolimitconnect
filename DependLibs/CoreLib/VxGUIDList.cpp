//============================================================================
// Copyright (C) 2016 Brett R. Jones
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
// http://www.gotvptop.net
//============================================================================
#include "config_corelib.h"

#include "VxGUIDList.h"

#include <CoreLib/VxParse.h>
#include <CoreLib/VxDebug.h>

#ifdef TARGET_OS_WINDOWS
	#include <Rpc.h>
#else
	#ifdef TARGET_OS_ANDROID
		// android has no uuid.h.. get from java
        //#include <GoTvInterface/AndroidInterface/NativeToJava.h>
	#else
		#include <uuid/uuid.h>
	#endif
#endif

#include <string.h>
#include <stdio.h>

//============================================================================
VxGUIDList::VxGUIDList()
: m_GuidList()
{
}

//============================================================================
VxGUIDList::~VxGUIDList()
{
}

//============================================================================
void VxGUIDList::addGuid( VxGUID& guid )
{
	m_GuidList.push_back( guid );
}

//============================================================================
bool VxGUIDList::addGuidIfDoesntExist( VxGUID& guid )
{
	if( doesGuidExist( guid ) )
	{
		return false;
	}
	
	addGuid( guid );
	return true;
}

//============================================================================
bool VxGUIDList::doesGuidExist( VxGUID& guid )
{
	std::vector<VxGUID>::iterator iter;
	for( iter = m_GuidList.begin(); iter != m_GuidList.end(); ++iter )
	{
		if( (*iter).isEqualTo( guid ) )
		{
			return true;
		}
	}

	return false;
}

//============================================================================
bool VxGUIDList::removeGuid( VxGUID& guid )
{
	bool guidExisted = false;
	std::vector<VxGUID>::iterator iter;
	for( iter = m_GuidList.begin(); iter != m_GuidList.end(); ++iter )
	{
		if( (*iter).isEqualTo( guid ) )
		{
			guidExisted = true;
			m_GuidList.erase( iter );
			break;
		}
	}

	return guidExisted;
}

//============================================================================
void VxGUIDList::clearList( void )
{
	m_GuidList.clear();
}

//============================================================================
void VxGUIDList::copyTo( VxGUIDList& destGuidList )
{
	std::vector<VxGUID>& guidList = destGuidList.getGuidList();
	std::vector<VxGUID>::iterator iter;
	for( iter = m_GuidList.begin(); iter != m_GuidList.end(); ++iter )
	{
		guidList.push_back( *iter );
	}
}
