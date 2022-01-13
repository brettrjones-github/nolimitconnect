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

#include "GroupieInfo.h"

#include <PktLib/PktsGroupie.h>
#include <CoreLib/VxDebug.h>
#include <CoreLib/VxPtopUrl.h>

//============================================================================
GroupieInfo::GroupieInfo( VxGUID& groupieOnlineId, VxGUID& hostOnlineId, EHostType hostType, std::string& groupieUrl )
    : m_HostType( hostType )
    , m_GroupieOnlineId( groupieOnlineId )
    , m_HostOnlineId( hostOnlineId )
    , m_GroupieUrl( groupieUrl )
{
}

//============================================================================
GroupieInfo::GroupieInfo( const GroupieInfo& rhs )
    : m_HostType( rhs.m_HostType )
    , m_GroupieOnlineId( rhs.m_GroupieOnlineId )
    , m_HostOnlineId( rhs.m_HostOnlineId )
    , m_ConnectedTimestampMs( rhs.m_ConnectedTimestampMs )
    , m_JoinedTimestampMs( rhs.m_JoinedTimestampMs )
    , m_GroupieInfoTimestampMs( rhs.m_GroupieInfoTimestampMs )
    , m_IsFavorite( rhs.m_IsFavorite )
    , m_GroupieUrl( rhs.m_GroupieUrl )
    , m_GroupieTitle( rhs.m_GroupieTitle )
    , m_GroupieDesc( rhs.m_GroupieDesc )
{
}

//============================================================================
GroupieInfo& GroupieInfo::operator=( const GroupieInfo& rhs ) 
{	
	if( this != &rhs )
	{
        m_HostType = rhs.m_HostType;
        m_GroupieOnlineId = rhs.m_GroupieOnlineId;
        m_HostOnlineId = rhs.m_HostOnlineId;
        m_ConnectedTimestampMs = rhs.m_ConnectedTimestampMs;
        m_JoinedTimestampMs = rhs.m_JoinedTimestampMs;
        m_GroupieInfoTimestampMs = rhs.m_GroupieInfoTimestampMs;
        m_IsFavorite = rhs.m_IsFavorite;
        m_GroupieUrl = rhs.m_GroupieUrl;
        m_GroupieTitle = rhs.m_GroupieTitle;
        m_GroupieDesc = rhs.m_GroupieDesc;
    }

	return *this;
}

//============================================================================
bool GroupieInfo::shouldSaveToDb( void )
{
    return m_IsFavorite || m_JoinedTimestampMs;
}

//============================================================================
bool GroupieInfo::isValidForGui( void )
{
    return !m_GroupieUrl.empty() && !m_GroupieTitle.empty() && !m_GroupieDesc.empty();
}

//============================================================================
int GroupieInfo::getSearchBlobSpaceRequirement( void )
{
    // the +3 is for string \0 terminators
    return sizeof( int64_t ) + m_GroupieUrl.length() + m_GroupieTitle.length() + m_GroupieDesc.length() + 3 * 5; // each string requires null terminator and 4 byte length of data in the blob
}

//============================================================================
bool GroupieInfo::fillSearchBlob( PktBlobEntry& blobEntry )
{
    bool result{ true };
    if( getSearchBlobSpaceRequirement() <= blobEntry.getRemainingStorageLen() )
    {
        result &= blobEntry.setValue( m_GroupieInfoTimestampMs );
        result &= blobEntry.setValue( m_GroupieUrl );
        result &= blobEntry.setValue( m_GroupieTitle );
        result &= blobEntry.setValue( m_GroupieDesc );  
    }
    else
    {
        result = false;
    }

    return result;
}

//============================================================================
bool GroupieInfo::extractFromSearchBlob( PktBlobEntry& blobEntry )
{
    bool result = blobEntry.getValue( m_GroupieInfoTimestampMs );
    result &= blobEntry.getValue( m_GroupieUrl );
    result &= blobEntry.getValue( m_GroupieTitle );
    result &= blobEntry.getValue( m_GroupieDesc );
    if( result )
    {
        VxPtopUrl hostUrl( m_GroupieUrl );
        result = hostUrl.isValid() && m_GroupieInfoTimestampMs && !m_GroupieTitle.empty() && !m_GroupieDesc.empty();
        if( result )
        {
            setGroupieOnlineId( hostUrl.getOnlineId() );
            if( eHostTypeUnknown != hostUrl.getHostType() )
            {
                setHostType( hostUrl.getHostType() );
            }
        }
    }

    return result;
}

//============================================================================
bool GroupieInfo::fillFromGroupie( PktGroupieAnnounceReq* hostAnn )
{
    std::string hostInviteUrl;
    std::string hostTitle;
    std::string hostDesc;
    int64_t hostTimestampMs{ 0 };

    if( hostAnn->getGroupieInfo( hostInviteUrl, hostTitle, hostDesc, hostTimestampMs ) )
    {
        VxPtopUrl hostUrl( hostInviteUrl );
        if( hostUrl.isValid() && hostTimestampMs && !hostTitle.empty() && !hostDesc.empty() )
        {
            setGroupieOnlineId( hostUrl.getOnlineId() );
            setHostOnlineId( hostUrl.getOnlineId() );
            setHostType( hostAnn->getHostType() );
            setGroupieUrl( hostInviteUrl );
            setGroupieTitle( hostTitle );
            setGroupieDescription( hostDesc );
            setGroupieInfoTimestamp( hostTimestampMs );
            return true;
        }
        else
        {
            LogMsg( LOG_ERROR, "GroupieInfo::fillFromGroupie extract failed" );
        }
    }
    else
    {
        LogMsg( LOG_ERROR, "GroupieInfo::fillFromGroupie extract failed");
    }

    return false;
}

//============================================================================
bool GroupieInfo::isGroupieValid( void )
{
    return eHostTypeUnknown != getHostType() && m_HostOnlineId.isVxGUIDValid() && m_GroupieOnlineId.isVxGUIDValid() && 
        m_GroupieInfoTimestampMs && !m_GroupieUrl.empty() && !m_GroupieTitle.empty() && !m_GroupieDesc.empty();
}