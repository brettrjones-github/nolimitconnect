#pragma once
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

#include <GuiInterface/IDefs.h>

#include <CoreLib/VxGUID.h>

#include <string>

class PktGroupieAnnounceReq;

class GroupieInfo
{
public:
	GroupieInfo() = default;
	GroupieInfo( const GroupieInfo& rhs );
    GroupieInfo( VxGUID& groupieOnlineId, VxGUID& hostOnlineId, EHostType hostType, std::string& groupieUrl );
    virtual ~GroupieInfo() = default;

	GroupieInfo&				operator=( const GroupieInfo& rhs ); 

    bool                        isGroupieValid( void );

    virtual void				setGroupieOnlineId( VxGUID& onlineId )              { m_GroupieOnlineId = onlineId; }
    virtual VxGUID&             getGroupieOnlineId( void )                          { return m_GroupieOnlineId; }

    virtual void				setHostOnlineId( VxGUID& onlineId )                 { m_HostOnlineId = onlineId; }
    virtual VxGUID&             getHostOnlineId( void )                             { return m_HostOnlineId; }

    virtual void			    setHostType( EHostType hostType )                   { m_HostType = hostType; }
    virtual EHostType	        getHostType( void )                                 { return m_HostType; }

    virtual void                setIsFavorite( bool isFavorite )                    { m_IsFavorite = isFavorite; }
    virtual bool                getIsFavorite( void )                               { return m_IsFavorite; }

    virtual void			    setConnectedTimestamp( int64_t timestampMs )        { m_ConnectedTimestampMs = timestampMs; }
    virtual int64_t             getConnectedTimestamp( void )                       { return m_ConnectedTimestampMs; }
    virtual void			    setJoinedTimestamp( int64_t timestampMs )           { m_JoinedTimestampMs = timestampMs; }
    virtual int64_t             getJoinedTimestamp( void )                          { return m_JoinedTimestampMs; }
    virtual void			    setGroupieInfoTimestamp( int64_t timestampMs )      { m_GroupieInfoTimestampMs = timestampMs; }
    virtual int64_t             getGroupieInfoTimestamp( void )                     { return m_GroupieInfoTimestampMs; }

    virtual void			    setGroupieUrl( std::string hostUrl )                { m_GroupieUrl = hostUrl; }
    virtual std::string&        getGroupieUrl( void )                               { return m_GroupieUrl; }

    virtual void                setGroupieTitle( std::string hostTitle )            { m_GroupieTitle = hostTitle; }
    virtual std::string&        getGroupieTitle( void )                             { return m_GroupieTitle; }

    virtual void                setGroupieDescription( std::string hostDesc )       { m_GroupieDesc = hostDesc; }
    virtual std::string&        getGroupieDescription( void )                        { return m_GroupieDesc; }

    bool                        shouldSaveToDb( void );
    bool                        isValidForGui( void );

    bool                        fillFromGroupie( PktGroupieAnnounceReq* hostAnn );
    int                         getSearchBlobSpaceRequirement( void );
    bool                        fillSearchBlob( PktBlobEntry& blobEntry );
    bool                        extractFromSearchBlob( PktBlobEntry& blobEntry );

protected:
	//=== vars ===//
    EHostType                   m_HostType{ eHostTypeUnknown };
    VxGUID                      m_GroupieOnlineId;
    VxGUID                      m_HostOnlineId;
    int64_t                     m_ConnectedTimestampMs{ 0 };
    int64_t                     m_JoinedTimestampMs{ 0 };
    int64_t                     m_GroupieInfoTimestampMs{ 0 };
    bool                        m_IsFavorite{ false };
    std::string                 m_GroupieUrl{ "" };
    std::string                 m_GroupieTitle{ "" };
    std::string                 m_GroupieDesc{ "" };
};
