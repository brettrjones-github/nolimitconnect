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

#include <QWidget> // must be declared first or linux Qt 6.2.4 will error in qmetatype.h 2167:23: array subscript value ‘53’ is outside the bounds

#include <GuiInterface/IDefs.h>

#include <PktLib/GroupieId.h>
#include <CoreLib/VxGUID.h>

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
    GuiGroupie( AppCommon& app, GuiUser* guiUser, VxGUID& sessionId, GroupieInfo& groupieInfo );
    GuiGroupie( const GuiGroupie& rhs );
	virtual ~GuiGroupie() = default;

    GuiGroupieListMgr&          getGroupieListMgr( void )                   { return m_GroupieListMgr; }

    void                        setUser( GuiUser* user )                    { m_GuiUser = user; }
    GuiUser*                    getUser( void )                             { return m_GuiUser; }

    void				        setGroupieOnlineId( GroupieId& groupieId )  { m_GroupieId = groupieId; }
    GroupieId&                  getGroupieId( void )                        { return m_GroupieId; }
    void				        setGroupieOnlineId( VxGUID& onlineId )      { m_GroupieId.setGroupieOnlineId( onlineId ); }
    VxGUID&                     getGroupieOnlineId( void )                  { return m_GroupieId.getGroupieOnlineId(); }
    void			            setHostedOnlineId( VxGUID& onlineId )       { m_GroupieId.setHostedOnlineId( onlineId ); }
    VxGUID&                     getHostedOnlineId( void )                   { return m_GroupieId.getHostedOnlineId(); }
    void			            setHostType( EHostType hostType )           { m_GroupieId.setHostType( hostType ); }
    EHostType	                getHostType( void )                         { return m_GroupieId.getHostType(); }

    virtual void                setIsFavorite( bool isFavorite )            { m_IsFavorite = isFavorite; }
    virtual bool                getIsFavorite( void )                       { return m_IsFavorite; }

    virtual void			    setGroupieInfoTimestamp( int64_t timestampMs ) { m_GroupieInfoTimestampMs = timestampMs; }
    virtual int64_t             getGroupieInfoTimestamp( void )             { return m_GroupieInfoTimestampMs; }

    virtual void			    setGroupieUrl( std::string hostUrl )        { m_GroupieUrl = hostUrl; }
    virtual std::string&        getGroupieUrl( void )                       { return m_GroupieUrl; }

    virtual void                setGroupieTitle( std::string hostTitle )    { m_GroupieTitle = hostTitle; }
    virtual std::string&        getGroupieTitle( void )                     { return m_GroupieTitle; }

    virtual void                setGroupieDescription( std::string hostDesc ) { m_GroupieDesc = hostDesc; }
    virtual std::string&        getGroupieDescription( void )               { return m_GroupieDesc; }

protected:
    AppCommon&                  m_MyApp;
    GuiGroupieListMgr&          m_GroupieListMgr;

    GuiUser*                    m_GuiUser{ nullptr };
    GroupieId                   m_GroupieId;
    VxGUID                      m_SessionId;
    
    bool                        m_IsFavorite{ false };
    int64_t                     m_GroupieInfoTimestampMs{ 0 };
    std::string                 m_GroupieUrl{ "" };
    std::string                 m_GroupieTitle{ "" };
    std::string                 m_GroupieDesc{ "" };
};
