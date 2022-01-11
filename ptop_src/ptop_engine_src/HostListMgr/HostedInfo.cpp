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

#include "HostedInfo.h"

#include <PktLib/PktsHostInvite.h>
#include <CoreLib/VxDebug.h>
#include <CoreLib/VxPtopUrl.h>

//============================================================================
HostedInfo::HostedInfo( EHostType hostType, VxGUID& onlineId, std::string& hostUrl )
    : m_HostType( hostType )
    , m_OnlineId( onlineId )
    , m_HostInviteUrl( hostUrl )
{
}

//============================================================================
HostedInfo::HostedInfo( const HostedInfo& rhs )
    : m_HostType( rhs.m_HostType )
    , m_OnlineId( rhs.m_OnlineId )
    , m_ConnectedTimestampMs( rhs.m_ConnectedTimestampMs )
    , m_JoinedTimestampMs( rhs.m_JoinedTimestampMs )
    , m_HostInfoTimestampMs( rhs.m_HostInfoTimestampMs )
    , m_IsFavorite( rhs.m_IsFavorite )
    , m_HostInviteUrl( rhs.m_HostInviteUrl )
    , m_HostTitle( rhs.m_HostTitle )
    , m_HostDesc( rhs.m_HostDesc )
{
}

//============================================================================
HostedInfo& HostedInfo::operator=( const HostedInfo& rhs ) 
{	
	if( this != &rhs )
	{
        m_HostType = rhs.m_HostType;
        m_OnlineId = rhs.m_OnlineId;
        m_ConnectedTimestampMs = rhs.m_ConnectedTimestampMs;
        m_JoinedTimestampMs = rhs.m_JoinedTimestampMs;
        m_HostInfoTimestampMs = rhs.m_HostInfoTimestampMs;
        m_IsFavorite = rhs.m_IsFavorite;
        m_HostInviteUrl = rhs.m_HostInviteUrl;
        m_HostTitle = rhs.m_HostTitle;
        m_HostDesc = rhs.m_HostDesc;
    }

	return *this;
}

//============================================================================
bool HostedInfo::shouldSaveToDb( void )
{
    return m_IsFavorite || m_JoinedTimestampMs;
}

//============================================================================
bool HostedInfo::isValidForGui( void )
{
    return !m_HostInviteUrl.empty() && !m_HostTitle.empty() && !m_HostDesc.empty();
}

//============================================================================
int HostedInfo::getSearchBlobSpaceRequirement( void )
{
    // the +3 is for string \0 terminators
    return sizeof( int64_t ) + m_HostInviteUrl.length() + m_HostTitle.length() + m_HostDesc.length() + 3 * 5; // each string requires null terminator and 4 byte length of data in the blob
}

//============================================================================
bool HostedInfo::fillSearchBlob( PktBlobEntry& blobEntry )
{
    bool result{ true };
    if( getSearchBlobSpaceRequirement() <= blobEntry.getRemainingStorageLen() )
    {
        result &= blobEntry.setValue( m_HostInfoTimestampMs );
        result &= blobEntry.setValue( m_HostInviteUrl );
        result &= blobEntry.setValue( m_HostTitle );
        result &= blobEntry.setValue( m_HostDesc );  
    }
    else
    {
        result = false;
    }

    return result;
}

//============================================================================
bool HostedInfo::extractFromSearchBlob( PktBlobEntry& blobEntry )
{
    bool result = blobEntry.getValue( m_HostInfoTimestampMs );
    result &= blobEntry.getValue( m_HostInviteUrl );
    result &= blobEntry.getValue( m_HostTitle );
    result &= blobEntry.getValue( m_HostDesc );
    if( result )
    {
        VxPtopUrl hostUrl( m_HostInviteUrl );
        result = hostUrl.isValid() && m_HostInfoTimestampMs && !m_HostTitle.empty() && !m_HostDesc.empty();
        if( result )
        {
            setOnlineId( hostUrl.getOnlineId() );
            if( eHostTypeUnknown != hostUrl.getHostType() )
            {
                setHostType( hostUrl.getHostType() );
            }
        }
    }

    return result;
}

//============================================================================
bool HostedInfo::fillFromHostInvite( PktHostInviteAnnounceReq* hostAnn )
{
    std::string hostInviteUrl;
    std::string hostTitle;
    std::string hostDesc;
    int64_t hostTimestampMs{ 0 };

    if( hostAnn->getHostInviteInfo( hostInviteUrl, hostTitle, hostDesc, hostTimestampMs ) )
    {
        VxPtopUrl hostUrl( hostInviteUrl );
        if( hostUrl.isValid() && hostTimestampMs && !hostTitle.empty() && !hostDesc.empty() )
        {
            setOnlineId( hostUrl.getOnlineId() );
            setHostType( hostAnn->getHostType() );
            setHostInviteUrl( hostInviteUrl );
            setHostTitle( hostTitle );
            setHostDescription( hostDesc );
            setHostInfoTimestamp( hostTimestampMs );
            return true;
        }
        else
        {
            LogMsg( LOG_ERROR, "HostedInfo::fillFromHostInvite extract failed" );
        }
    }
    else
    {
        LogMsg( LOG_ERROR, "HostedInfo::fillFromHostInvite extract failed");
    }

    return false;
}

//============================================================================
bool HostedInfo::isHostInviteValid( void )
{
    return eHostTypeUnknown != getHostType() && m_OnlineId.isVxGUIDValid() && m_HostInfoTimestampMs && !m_HostInviteUrl.empty() && !m_HostTitle.empty() && !m_HostDesc.empty();
}