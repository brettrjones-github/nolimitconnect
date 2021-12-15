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

#include "VxGuidPairList.h"

#include <CoreLib/VxDebug.h>

//============================================================================
VxGuidPairList::VxGuidPairList()
: m_GuidPairList()
{
}

//============================================================================
VxGuidPairList::VxGuidPairList( const VxGuidPairList& rhs )
    : m_GuidPairList( rhs.m_GuidPairList )
{
}

//============================================================================
VxGuidPairList& VxGuidPairList::operator =( const VxGuidPairList& rhs )
{
    if( this != &rhs )
    {
        m_GuidPairList          = rhs.m_GuidPairList;
    }

    return *this;
}

//============================================================================
void VxGuidPairList::addGuid( const VxGUID& guid1, const VxGUID& guid2 )
{
	uint64_t timeMs = GetTimeStampMs();
	m_GuidPairList.push_back( std::make_pair(guid1, std::make_pair(guid2, timeMs )));
}

//============================================================================
bool VxGuidPairList::addGuidIfDoesntExist( const VxGUID& guid1, const VxGUID& guid2 )
{
	if( doesGuidExist( guid1, guid2 ) )
	{
		return false;
	}
	
	addGuid( guid1, guid2 );
	return true;
}

//============================================================================
bool VxGuidPairList::doesGuidExist( const VxGUID& guid1, const VxGUID& guid2, uint64_t timeoutMs )
{
	uint64_t timeNowMs = timeoutMs ? GetTimeStampMs() : 0;
    for( auto iter = m_GuidPairList.begin(); iter != m_GuidPairList.end(); ++iter )
	{
		if( (*iter).first == guid1 && ( *iter ).second.first == guid2 )
		{
			if( timeoutMs && ( *iter ).second.second + timeoutMs < timeNowMs )
			{
				// timed out
				m_GuidPairList.erase( iter );
				return false;
			}
			else
			{
				return true;
			}
		}
	}

	return false;
}

//============================================================================
bool VxGuidPairList::removeGuid( const VxGUID& guid1, const VxGUID& guid2 )
{
	bool guidExisted = false;
    for( auto iter = m_GuidPairList.begin(); iter != m_GuidPairList.end(); ++iter )
	{
		if( ( *iter ).first == guid1 && ( *iter ).second.first == guid2 )
		{
			guidExisted = true;
			m_GuidPairList.erase( iter );
			break;
		}
	}

	return guidExisted;
}

//============================================================================
void VxGuidPairList::removeExpired( uint64_t& timeNowMs, uint64_t timeoutMs )
{
    for( auto iter = m_GuidPairList.begin(); iter != m_GuidPairList.end(); )
	{
		if( ( *iter ).second.second + timeoutMs < timeNowMs )
		{
			// timed out
			iter = m_GuidPairList.erase( iter );
		}
		else
		{
			++iter;
		}
	}
}

//============================================================================
void VxGuidPairList::clearList( void )
{
	m_GuidPairList.clear();
}
