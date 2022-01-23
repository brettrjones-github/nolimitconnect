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

#include <PktLib/GroupieId.h>

#include <string>

class PktHostInviteAnnounceReq;

class HostedInfo
{
public:
	HostedInfo() = default;
	HostedInfo( const HostedInfo& rhs );
    HostedInfo( EHostType hostType, VxGUID& onlineId, std::string& hostUrl );
    virtual ~HostedInfo() = default;

	HostedInfo&				    operator=( const HostedInfo& rhs ); 

    bool                        isHostInviteValid( void );

    virtual void				setHostedId( HostedId& hostedId )                   { m_HostedId = hostedId; }
    HostedId&                   getHostedId( void )                                 { return m_HostedId; }
    virtual void				setOnlineId( VxGUID& onlineId )                     { m_HostedId.setOnlineId( onlineId ); }
    virtual VxGUID&             getOnlineId( void )                                 { return m_HostedId.getOnlineId(); }
    virtual void			    setHostType( EHostType hostType )                   { m_HostedId.setHostType( hostType ); }
    virtual EHostType	        getHostType( void )                                 { return m_HostedId.getHostType(); }

    virtual void                setIsFavorite( bool isFavorite )                    { m_IsFavorite = isFavorite; }
    virtual bool                getIsFavorite( void )                               { return m_IsFavorite; }

    virtual void			    setConnectedTimestamp( int64_t timestampMs )        { m_ConnectedTimestampMs = timestampMs; }
    virtual int64_t             getConnectedTimestamp( void )                       { return m_ConnectedTimestampMs; }
    virtual void			    setJoinedTimestamp( int64_t timestampMs )           { m_JoinedTimestampMs = timestampMs; }
    virtual int64_t             getJoinedTimestamp( void )                          { return m_JoinedTimestampMs; }
    virtual void			    setHostInfoTimestamp( int64_t timestampMs )         { m_HostInfoTimestampMs = timestampMs; }
    virtual int64_t             getHostInfoTimestamp( void )                        { return m_HostInfoTimestampMs; }

    virtual void			    setHostInviteUrl( std::string hostUrl )             { m_HostInviteUrl = hostUrl; }
    virtual std::string&        getHostInviteUrl( void )                            { return m_HostInviteUrl; }

    virtual void                setHostTitle( std::string hostTitle )               { m_HostTitle = hostTitle; }
    virtual std::string&        getHostTitle( void )                                { return m_HostTitle; }

    virtual void                setHostDescription( std::string hostDesc )          { m_HostDesc = hostDesc; }
    virtual std::string&        getHostDescription( void )                          { return m_HostDesc; }

    bool                        shouldSaveToDb( void );
    bool                        isValidForGui( void );

    bool                        fillFromHostInvite( PktHostInviteAnnounceReq* hostAnn );
    int                         getSearchBlobSpaceRequirement( void );
    bool                        fillSearchBlob( PktBlobEntry& blobEntry );
    bool                        extractFromSearchBlob( PktBlobEntry& blobEntry );

protected:
	//=== vars ===//
    HostedId                    m_HostedId;
    int64_t                     m_ConnectedTimestampMs{ 0 };
    int64_t                     m_JoinedTimestampMs{ 0 };
    int64_t                     m_HostInfoTimestampMs{ 0 };
    bool                        m_IsFavorite{ false };
    std::string                 m_HostInviteUrl{ "" };
    std::string                 m_HostTitle{ "" };
    std::string                 m_HostDesc{ "" };
};
