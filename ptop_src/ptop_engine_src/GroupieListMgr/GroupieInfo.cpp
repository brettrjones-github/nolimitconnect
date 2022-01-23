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

#include <PktLib/GroupieId.h>
#include <PktLib/PktsGroupie.h>

#include <CoreLib/VxDebug.h>
#include <CoreLib/VxPtopUrl.h>
#include <CoreLib/VxParse.h>

//============================================================================
GroupieInfo::GroupieInfo( VxGUID& groupieOnlineId, VxGUID& hostOnlineId, EHostType hostType, std::string& groupieUrl )
    : m_GroupieId( groupieOnlineId, hostOnlineId, hostType )
    , m_GroupieUrl( groupieUrl )
{
}

//============================================================================
GroupieInfo::GroupieInfo( GroupieId& groupieId, std::string& groupieUrl, std::string& groupieTitle, std::string& groupieDesc, int64_t timeModified )
    : m_GroupieId( groupieId )
    , m_GroupieInfoTimestampMs( timeModified )
    , m_GroupieUrl( groupieUrl )
    , m_GroupieTitle( groupieTitle )
    , m_GroupieDesc( groupieDesc )
{
}

//============================================================================
GroupieInfo::GroupieInfo( const GroupieInfo& rhs )
    : m_GroupieId( rhs.m_GroupieId )
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
        m_GroupieId = rhs.m_GroupieId;
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
            setHostedOnlineId( hostUrl.getOnlineId() );
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
    return m_GroupieId.isValid() && 
        m_GroupieInfoTimestampMs && !m_GroupieUrl.empty() && !m_GroupieTitle.empty() && !m_GroupieDesc.empty();
}

//============================================================================
bool GroupieInfo::isIdMatch( GroupieId& groupieId )
{
    return m_GroupieId == groupieId;
}

//============================================================================
bool GroupieInfo::isSearchTextMatch( std::string& searchText )
{
    if( searchText.empty() )
    {
        return false;
    }

    bool hasMatch = CaseInsensitiveFindSubstr( m_GroupieTitle, searchText ) >= 0;
    if( !hasMatch )
    {
        hasMatch = CaseInsensitiveFindSubstr( m_GroupieDesc, searchText ) >= 0;
    }

    return hasMatch;
}

//============================================================================
bool GroupieInfo::setGroupieUrlAndTitleAndDescription( std::string& groupieUrl, std::string& groupieTitle, std::string& groupieDesc, int64_t& lastModifiedTime )
{
    if( !groupieUrl.empty() && !groupieTitle.empty() && !groupieDesc.empty() && lastModifiedTime )
    {
        m_GroupieInfoTimestampMs = lastModifiedTime;
        m_GroupieUrl = groupieUrl;
        m_GroupieTitle = groupieTitle;
        m_GroupieDesc = groupieDesc;
        return true;
    }

    return false;
}

//============================================================================
bool GroupieInfo::getGroupieUrlAndTitleAndDescription(std::string& groupieUrl, std::string& groupieTitle, std::string& groupieDesc, int64_t& lastModifiedTime )
{
    lastModifiedTime = m_GroupieInfoTimestampMs;
    groupieUrl = m_GroupieUrl;
    groupieTitle = m_GroupieTitle;
    groupieDesc = m_GroupieDesc;

    return !groupieUrl.empty() && !groupieTitle.empty() && !groupieDesc.empty() && lastModifiedTime;
}