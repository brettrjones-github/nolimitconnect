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

#include <PktLib/GroupieId.h>
#include <CoreLib/VxGUID.h>

#include <QWidget>

#include <vector>

class AppCommon;
class GuiUser;
class GuiGroupieListMgr;
class GroupieInfo;

class GuiGroupie : public QWidget
{
public:
    GuiGroupie() = delete;
    GuiGroupie( AppCommon& app );
    GuiGroupie( AppCommon& app, GuiUser* guiUser, VxGUID& sessionId );
    GuiGroupie( AppCommon& app, GuiUser* guiUser, VxGUID& sessionId, GroupieInfo& hostedInfo );
    GuiGroupie( const GuiGroupie& rhs );
	virtual ~GuiGroupie() = default;

    GuiGroupieListMgr&          getGroupieListMgr( void )               { return m_GroupieListMgr; }

    GroupieId                   getGroupieId( void )                    { return GroupieId( m_GroupieOnlineId, m_HostOnlineId, m_HostType ); }

    void                        setUser( GuiUser* user )                { m_GuiUser = user; }
    GuiUser*                    getUser( void )                         { return m_GuiUser; }

    virtual void				setGroupieOnlineId( VxGUID& onlineId ) { m_GroupieOnlineId = onlineId; }
    virtual VxGUID&             getGroupieOnlineId( void )             { return m_GroupieOnlineId; }

    virtual void			    setHostOnlineId( VxGUID& onlineId )     { m_HostOnlineId = onlineId; }
    virtual VxGUID&             getHostOnlineId( void )                 { return m_HostOnlineId; }

    virtual void			    setHostType( EHostType hostType )       { m_HostType = hostType; }
    virtual EHostType	        getHostType( void )                     { return m_HostType; }

    virtual void                setIsFavorite( bool isFavorite )        { m_IsFavorite = isFavorite; }
    virtual bool                getIsFavorite( void )                   { return m_IsFavorite; }

    virtual void			    setGroupieInfoTimestamp( int64_t timestampMs ) { m_GroupieInfoTimestampMs = timestampMs; }
    virtual int64_t             getGroupieInfoTimestamp( void )         { return m_GroupieInfoTimestampMs; }

    virtual void			    setGroupieUrl( std::string hostUrl )    { m_GroupieUrl = hostUrl; }
    virtual std::string&        getGroupieUrl( void )                   { return m_GroupieUrl; }

    virtual void                setGroupieTitle( std::string hostTitle ) { m_GroupieTitle = hostTitle; }
    virtual std::string&        getGroupieTitle( void )                 { return m_GroupieTitle; }

    virtual void                setGroupieDescription( std::string hostDesc ) { m_GroupieDesc = hostDesc; }
    virtual std::string&        getGroupieDescription( void )           { return m_GroupieDesc; }

protected:
    AppCommon&                  m_MyApp;
    GuiGroupieListMgr&          m_GroupieListMgr;

    GuiUser*                    m_GuiUser{ nullptr };
    VxGUID                      m_SessionId;

    VxGUID                      m_GroupieOnlineId;
    VxGUID                      m_HostOnlineId;
    EHostType                   m_HostType{ eHostTypeUnknown };

    
    bool                        m_IsFavorite{ false };
    int64_t                     m_GroupieInfoTimestampMs{ 0 };
    std::string                 m_GroupieUrl{ "" };
    std::string                 m_GroupieTitle{ "" };
    std::string                 m_GroupieDesc{ "" };
};
