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

#include <PktLib/HostedId.h>
#include <CoreLib/VxGUID.h>

#include <QWidget>

#include <vector>

class AppCommon;
class GuiUser;
class GuiHostedListMgr;
class HostedInfo;

class GuiHosted : public QWidget
{
public:
    GuiHosted() = delete;
    GuiHosted( AppCommon& app );
    GuiHosted( AppCommon& app, GuiUser* guiUser, VxGUID& sessionId );
    GuiHosted( AppCommon& app, GuiUser* guiUser, VxGUID& sessionId, HostedInfo& hostedInfo );
    GuiHosted( const GuiHosted& rhs );
	virtual ~GuiHosted() = default;

    GuiHostedListMgr&           getHostedListMgr( void )        { return m_HostedListMgr; }

    HostedId                    getHostedId( void )             { return HostedId( m_OnlineId, m_HostType ); }

    void                        setUser( GuiUser* user )        { m_GuiUser = user; }
    GuiUser*                    getUser( void )                 { return m_GuiUser; }

    virtual void				setOnlineId( VxGUID& onlineId ) { m_OnlineId = onlineId; }
    virtual VxGUID&             getOnlineId( void )             { return m_OnlineId; }

    virtual void			    setHostType( EHostType hostType ) { m_HostType = hostType; }
    virtual EHostType	        getHostType( void )             { return m_HostType; }

    virtual void                setIsFavorite( bool isFavorite ) { m_IsFavorite = isFavorite; }
    virtual bool                getIsFavorite( void )           { return m_IsFavorite; }

    virtual void			    setConnectedTimestamp( int64_t timestampMs ) { m_ConnectedTimestampMs = timestampMs; }
    virtual int64_t             getConnectedTimestamp( void )   { return m_ConnectedTimestampMs; }
    virtual void			    setJoinedTimestamp( int64_t timestampMs ) { m_JoinedTimestampMs = timestampMs; }
    virtual int64_t             getJoinedTimestamp( void )      { return m_JoinedTimestampMs; }
    virtual void			    setHostInfoTimestamp( int64_t timestampMs ) { m_HostInfoTimestampMs = timestampMs; }
    virtual int64_t             getHostInfoTimestamp( void )    { return m_HostInfoTimestampMs; }

    virtual void			    setHostInviteUrl( std::string hostUrl ) { m_HostInviteUrl = hostUrl; }
    virtual std::string&        getHostInviteUrl( void )        { return m_HostInviteUrl; }

    virtual void                setHostTitle( std::string hostTitle ) { m_HostTitle = hostTitle; }
    virtual std::string&        getHostTitle( void )            { return m_HostTitle; }

    virtual void                setHostDescription( std::string hostDesc ) { m_HostDesc = hostDesc; }
    virtual std::string&        getHostDescription( void )      { return m_HostDesc; }

protected:
    AppCommon&                  m_MyApp;
    GuiHostedListMgr&           m_HostedListMgr;

    GuiUser*                    m_GuiUser{ nullptr };
    VxGUID                      m_SessionId;
    EHostType                   m_HostType{ eHostTypeUnknown };
    VxGUID                      m_OnlineId;
    bool                        m_IsFavorite{ false };
    int64_t                     m_ConnectedTimestampMs{ 0 };
    int64_t                     m_JoinedTimestampMs{ 0 };
    int64_t                     m_HostInfoTimestampMs{ 0 };
    std::string                 m_HostInviteUrl{ "" };
    std::string                 m_HostTitle{ "" };
    std::string                 m_HostDesc{ "" };
};
