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

#include <PktLib/VxCommon.h>
#include <CoreLib/VxGUID.h>

#include <set>

#include <QWidget>

class AppCommon;
class GuiUserMgr;
class VxNetIdent;
class VxSktBase;

class GuiUserJoin : public QWidget
{
public:
    GuiUserJoin() = delete;
    GuiUserJoin( AppCommon& app );
    GuiUserJoin( AppCommon& app, VxNetIdent* netIdent, VxGUID& sessionId, bool online = false );
    GuiUserJoin( const GuiUserJoin& rhs );
	virtual ~GuiUserJoin() = default;

    void                        setNetIdent( VxNetIdent* netIdent );
    VxNetIdent*                 getNetIdent( void )                     { return m_NetIdent; }
    bool                        isIdentValid( void )                    { return m_NetIdent ? m_NetIdent->isIdentValid() : false; }
    void                        setSessionId( VxGUID& sessionId )       { m_SessionId = sessionId; }
    VxGUID&                     getSessionId( void )                    { return m_SessionId; }

    void                        setOnlineStatus( bool isOnline );
    bool                        isOnline( void )                        { return m_IsOnline; }
    VxGUID&                     getMyOnlineId( void )                   { return m_OnlineId; }
    const char *                getOnlineName( void )                   { return m_NetIdent ? m_NetIdent->getOnlineName() : ""; }
    const char *                getOnlineDescription( void )            { return m_NetIdent ? m_NetIdent->getOnlineDescription() : ""; }

    void                        setMyFriendshipToHim( EFriendState friendState ) { if( m_NetIdent ) m_NetIdent->setMyFriendshipToHim( friendState ); }
    EFriendState                getMyFriendshipToHim( void )            { return m_NetIdent ? m_NetIdent->getMyFriendshipToHim() : eFriendStateIgnore; }
    EFriendState                getHisFriendshipToMe( void )            { return m_NetIdent ? m_NetIdent->getHisFriendshipToMe() : eFriendStateIgnore; }
    EPluginAccess               getMyAccessPermissionFromHim( EPluginType pluginType ) { return m_NetIdent ? m_NetIdent->getMyAccessPermissionFromHim( pluginType ) : ePluginAccessIgnored; }
    EFriendState                getPluginPermission( EPluginType pluginType ) { return m_NetIdent ? m_NetIdent->getPluginPermission( pluginType ) : eFriendStateIgnore; }
    int64_t					    getLastSessionTimeMs( void )            { return m_NetIdent ? m_NetIdent->getLastSessionTimeMs() : 0; }

    QString				        describeMyFriendshipToHim( void );
    QString				        describeHisFriendshipToMe( void );

    bool                        isAdmin( void )                         { return m_NetIdent ? m_NetIdent->isAdministrator() : false; }
    bool                        isFriend( void )                        { return m_NetIdent ? m_NetIdent->isFriend() : false; }
    bool                        isGuest( void )                         { return m_NetIdent ? m_NetIdent->isGuest() : false; }
    bool                        isAnonymous( void )                     { return m_NetIdent ? m_NetIdent->isAnonymous() : false; }
    bool                        isIgnored( void )                       { return m_NetIdent ? m_NetIdent->isIgnored() : true; }
    bool                        isMyself( void );

    bool                        isGroupHosted( void )                   { return m_HostSet.find( eHostTypeGroup ) != m_HostSet.end(); }
    bool                        isChatRoomHosted( void )                { return m_HostSet.find( eHostTypeChatRoom ) != m_HostSet.end(); }
    bool                        isNetworkHosted( void )                 { return m_HostSet.find( eHostTypeNetwork ) != m_HostSet.end(); }
    bool                        isRandomConnectHosted( void )           { return m_HostSet.find( eHostTypeRandomConnect ) != m_HostSet.end(); }
    bool                        isPeerHosted( void )                    { return m_HostSet.find( eHostTypePeerUser ) != m_HostSet.end(); }

    bool                        isInSession( void );
    bool                        isNearby( void )                        { return m_NetIdent ? m_NetIdent->isNearby() : false; }
    bool                        requiresRelay( void )                   { return m_NetIdent ? m_NetIdent->requiresRelay() : true; }
    bool                        isMyPreferedRelay( void )               { return m_NetIdent ? m_NetIdent->isMyPreferedRelay() : false; }


    uint32_t                    getTruthCount( void )                   { return m_NetIdent ? m_NetIdent->getTruthCount() : 0; }
    uint32_t                    getDareCount( void )                    { return m_NetIdent ? m_NetIdent->getDareCount() : 0; }
    void		                setHasTextOffers( bool hasOffers )	    { if( m_NetIdent ) m_NetIdent->setHasTextOffers( hasOffers ); }
    bool		                getHasTextOffers( void )				{ return m_NetIdent ? m_NetIdent->getHasTextOffers() : false; }
    bool                        isMyAccessAllowedFromHim( EPluginType pluginType ) { return m_NetIdent ? m_NetIdent->isMyAccessAllowedFromHim( pluginType ) : false; }
    
    void                        addHostType( EHostType hostType );

protected:
    AppCommon&                  m_MyApp;
    GuiUserMgr&                 m_GuiUserMgr;
    VxNetIdent*                 m_NetIdent;
    VxGUID                      m_OnlineId;
    VxGUID                      m_SessionId;
    bool                        m_IsOnline{ false };
    std::set<EHostType>         m_HostSet;
};
