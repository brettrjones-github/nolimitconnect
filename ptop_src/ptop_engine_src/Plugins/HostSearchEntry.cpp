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

#include "HostServerSearchMgr.h"
#include "PluginBase.h"
#include "PluginMgr.h"

#include <ptop_src/ptop_engine_src/P2PEngine/P2PEngine.h>
#include <ptop_src/ptop_engine_src/Plugins/PluginBase.h>
#include <ptop_src/ptop_engine_src/HostMgr/HostedEntry.h>

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
HostSearchEntry::HostSearchEntry( const HostSearchEntry& rhs )
: m_LastRxTime( rhs.m_LastRxTime )
, m_PluginSetting( rhs.m_PluginSetting )
, m_Url( rhs.m_Url )
, m_SearchStrings( rhs.m_SearchStrings )
{
    memcpy( &m_Ident, &rhs.m_Ident, sizeof( VxNetIdent ) );
    m_PktHostAnn.setPktLength( 0 );
    if( rhs.m_PktHostAnn.getPktLength() )
    {
        memcpy( &m_PktHostAnn, &rhs.m_PktHostAnn, rhs.m_PktHostAnn.getPktLength() );
    }
}

//============================================================================
HostSearchEntry& HostSearchEntry::operator=( const HostSearchEntry& rhs )
{
    if( this != &rhs )
    {
        m_LastRxTime = rhs.m_LastRxTime;
        memcpy( &m_Ident, &rhs.m_Ident, sizeof( VxNetIdent ) );
        m_PktHostAnn.setPktLength( 0 );
        if( rhs.m_PktHostAnn.getPktLength() )
        {
            memcpy( &m_PktHostAnn, &rhs.m_PktHostAnn, rhs.m_PktHostAnn.getPktLength() );
        }

        m_PluginSetting = rhs.m_PluginSetting;
        m_Url = rhs.m_Url;
        m_SearchStrings = rhs.m_SearchStrings;
    }

    return *this;
}

//============================================================================
bool HostSearchEntry::searchMatch( SearchParams& searchParams, std::string& searchStr )
{
    if( !searchStr.empty() )
    {
        for( auto& str : m_SearchStrings )
        {
            if( !str.empty() )
            {
                if( -1 != CaseInsensitiveFindSubstr( str, searchStr ) )
                {
                    return true;
                }
            }
        }
    }

    return false;
}

//============================================================================
void HostSearchEntry::toHostedEntry( HostedEntry& entry )
{
    entry.setConnectIdent( (const VxConnectIdent &)m_Ident );
    BinaryBlob blob;
    fillSearchReplyBlob( blob );
    entry.setPluginSettingsBlob( blob );
}

//============================================================================
void HostSearchEntry::fillSearchReplyBlob( BinaryBlob& blob )
{
    m_PluginSetting.toBinary( blob );
}

//============================================================================
bool HostSearchEntry::addToBlob( PktBlobEntry& entryBlob )
{
    bool result = m_Ident.addToBlob( entryBlob );
    result &= m_PluginSetting.addToBlob( entryBlob );
    return result;
}
