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

#include "HostServerSearchMgr.h"
#include "PluginBase.h"
#include "PluginMgr.h"

#include <ptop_src/ptop_engine_src/P2PEngine/P2PEngine.h>
#include <ptop_src/ptop_engine_src/Plugins/PluginBase.h>
//#include <ptop_src/ptop_engine_src/HostMgr/HostedEntry.h>

#include <NetLib/VxSktBase.h>
#include <PktLib/PktsHostJoin.h>
#include <CoreLib/VxTime.h>
#include <CoreLib/VxParse.h>

#include <memory.h>

#ifdef _MSC_VER
# pragma warning(disable: 4355) //'this' : used in base member initializer list
#endif //_MSC_VER

//============================================================================
void HostSearchEntry::updateLastRxTime( void )
{
    m_LastRxTime = GetTimeStampMs();
}

//============================================================================
bool HostSearchEntry::announceTimeExpired( int64_t timeNow )
{
    return timeNow - m_LastRxTime > MIN_HOST_RX_UPDATE_TIME_MS;
}

//============================================================================
bool HostSearchEntry::updateHostedInfo( PktHostInviteAnnounceReq* hostAnn )
{
    return m_HostedInfo.fillFromHostInvite( hostAnn );
}

//============================================================================
HostSearchEntry::HostSearchEntry( const HostSearchEntry& rhs )
: m_LastRxTime( rhs.m_LastRxTime )
, m_HostedInfo( rhs.m_HostedInfo )
{
}

//============================================================================
HostSearchEntry& HostSearchEntry::operator=( const HostSearchEntry& rhs )
{
    if( this != &rhs )
    {
        m_LastRxTime = rhs.m_LastRxTime;
        m_HostedInfo = rhs.m_HostedInfo;
    }

    return *this;
}

//============================================================================
bool HostSearchEntry::searchHostedMatch( SearchParams& searchParams, std::string& searchStr )
{
    if( searchParams.getSearchListAll() )
    {
        return true;
    }

    if( !searchStr.empty() )
    {
        if( -1 != CaseInsensitiveFindSubstr( m_HostedInfo.getHostTitle(), searchStr ) )
        {
            return true;
        }

        if( -1 != CaseInsensitiveFindSubstr( m_HostedInfo.getHostDescription(), searchStr ) )
        {
            return true;
        }
    }

    return false;
}
