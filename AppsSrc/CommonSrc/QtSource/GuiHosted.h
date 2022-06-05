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
// http://www.nolimitconnect.org
//============================================================================

#include <QWidget> // must be declared first or linux Qt 6.2.4 will error in qmetatype.h 2167:23: array subscript value ‘53’ is outside the bounds

#include <GuiInterface/IDefs.h>

#include <PktLib/HostedId.h>
#include <CoreLib/VxGUID.h>

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

    void                        setUser( GuiUser* user )        { m_GuiUser = user; }
    GuiUser*                    getUser( void )                 { return m_GuiUser; }

    void                        setHostedId( HostedId& hostedId ) { m_HostedId = hostedId; }
    HostedId&                   getHostedId( void )             { return m_HostedId; }

    virtual void				setOnlineId( VxGUID& onlineId ) { m_HostedId.setOnlineId( onlineId ); }
    virtual VxGUID&             getOnlineId( void )             { return m_HostedId.getOnlineId(); }

    virtual void			    setHostType( EHostType hostType ) { m_HostedId.setHostType( hostType ); }
    virtual EHostType	        getHostType( void )             { return m_HostedId.getHostType(); }

    virtual void				setThumbId( VxGUID& thumbId )   { m_ThumbId = thumbId; }
    virtual VxGUID&             getThumbId( void )              { return m_ThumbId; }

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

    virtual void                setIsIgnored( bool isIgnored )  { m_IsIgnored = isIgnored; }
    virtual bool                getIsIgnored( void )            { return m_IsIgnored; }

protected:
    AppCommon&                  m_MyApp;
    GuiHostedListMgr&           m_HostedListMgr;

    GuiUser*                    m_GuiUser{ nullptr };
    HostedId                    m_HostedId;
    VxGUID                      m_SessionId;
    bool                        m_IsFavorite{ false };
    bool                        m_IsIgnored{ false };
    int64_t                     m_ConnectedTimestampMs{ 0 };
    int64_t                     m_JoinedTimestampMs{ 0 };
    int64_t                     m_HostInfoTimestampMs{ 0 };
    std::string                 m_HostInviteUrl{ "" };
    std::string                 m_HostTitle{ "" };
    std::string                 m_HostDesc{ "" };
    VxGUID                      m_ThumbId;
};
