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

#include <PktLib/VxCommon.h>

#include <set>

class AppCommon;
class VxNetIdent;
class VxSktBase;

class GuiUserBase : public QWidget
{
public:
    static const int NEARBY_TIMEOUT_MS = 10000;

    GuiUserBase() = delete;
    GuiUserBase( AppCommon& app );
    GuiUserBase( AppCommon& app, VxNetIdent* netIdent, VxGUID& sessionId );
    GuiUserBase( const GuiUserBase& rhs );
	virtual ~GuiUserBase() override = default;

    bool                        isValid( void )                         { return isValidNetIdent(); }
    bool                        isValidNetIdent( void )                 { return m_NetIdent.isValidNetIdent(); }

    void                        setNetIdent( VxNetIdent* netIdent );
    VxNetIdent&                 getNetIdent( void )                     { return m_NetIdent; }
   
    void                        setSessionId( VxGUID& sessionId )       { m_SessionId = sessionId; }
    VxGUID&                     getSessionId( void )                    { return m_SessionId; }

    bool                        updateIsOnline( void )                  { bool isDirect = updateIsDirectConnect(); bool isRelay = updateIsRelayed(); return isDirect || isRelay; }
    bool                        isOnline( void )                        { return isDirectConnect() || isRelayed(); }

    bool                        updateIsNearby( void );
    virtual bool                setNearbyStatus( int64_t nearbyTimeOrZeroIfNotd ); // return false if nearbyTime is zero
    bool                        isNearby( void );

    bool                        updateIsDirectConnect( void );
    virtual bool                setDirectConnectStatus( bool isDirectConnect ); // return true if direct connect state changed.. derived classes can override
    bool                        isDirectConnect( void )                 { return m_IsDirectConnect; }

    bool                        updateIsRelayed( void );
    virtual bool                setRelayStatus( bool isRelayed ); // return true if relay state changed.. derived classes can override
    bool                        isRelayed( void )                       { return m_IsRelayed; }

    VxGUID&                     getMyOnlineId( void )                   { return m_OnlineId; }
    std::string                 getOnlineName( void )                   { return std::string( m_NetIdent.getOnlineName() ); }
    std::string                 getOnlineDescription( void )            { return std::string( m_NetIdent.getOnlineDescription() ); }
    std::string                 getMyOnlineUrl( void )                  { return m_NetIdent.getMyOnlineUrl(); }
    VxGUID                      getHostThumbId( EHostType hostType, bool defaultToAvatarThumbId ) { return m_NetIdent.getHostThumbId( hostType, defaultToAvatarThumbId ); } 
    VxGUID                      getAvatarThumbId( void )                { return m_NetIdent.getAvatarThumbGuid(); }

    void                        setMyFriendshipToHim( EFriendState friendState ) { m_NetIdent.setMyFriendshipToHim( friendState ); }
    EFriendState                getMyFriendshipToHim( void )            { return  m_NetIdent.getMyFriendshipToHim(); }
    EFriendState                getHisFriendshipToMe( void )            { return m_NetIdent.getHisFriendshipToMe(); }
    EPluginAccess               getMyAccessPermissionFromHim( EPluginType pluginType, bool inGroup = false );
    EFriendState                getPluginPermission( EPluginType pluginType ) { return m_NetIdent.getPluginPermission( pluginType ); }
    int64_t					    getLastSessionTimeMs( void )            { return m_NetIdent.getLastSessionTimeMs(); }

    QString				        describeMyFriendshipToHim( bool inGroup  );
    QString				        describeHisFriendshipToMe( bool inGroup );

    bool                        isAdmin( void )                         { return m_NetIdent.isAdministrator(); }
    bool                        isFriend( void )                        { return m_NetIdent.isFriend(); }
    bool                        isGuest( void )                         { return m_NetIdent.isGuest(); }
    bool                        isAnonymous( void )                     { return m_NetIdent.isAnonymous(); }
    bool                        isIgnored( void )                       { return m_NetIdent.isIgnored(); }
    bool                        isMyself( void );

    bool                        isHosted( void )                        { return m_HostSet.size(); }
    bool                        isGroupHosted( void )                   { return m_HostSet.find( eHostTypeGroup ) != m_HostSet.end(); }
    bool                        isChatRoomHosted( void )                { return m_HostSet.find( eHostTypeChatRoom ) != m_HostSet.end(); }
    bool                        isRandomConnectHosted( void )           { return m_HostSet.find( eHostTypeRandomConnect ) != m_HostSet.end(); }

    bool                        isInSession( void );

    uint32_t                    getTruthCount( void )                   { return m_NetIdent.getTruthCount(); }
    uint32_t                    getDareCount( void )                    { return m_NetIdent.getDareCount(); }
    void		                setHasTextOffers( bool hasOffers )	    { m_NetIdent.setHasTextOffers( hasOffers ); }
    bool		                getHasTextOffers( void )				{ return m_NetIdent.getHasTextOffers(); }
    bool                        isMyAccessAllowedFromHim( EPluginType pluginType, bool inGroup = false ) { return m_NetIdent.isMyAccessAllowedFromHim( pluginType, inGroup ); }
    
    void                        setLastUpdateTime( uint64_t updateTimeMs ) { m_LastUpdateTime = updateTimeMs; }
    uint64_t                    getLastUpdateTime( void )               { return m_LastUpdateTime; }

    void                        addHostType( EHostType hostType );
    bool                        hasHostType( EHostType hostType )       { return m_HostSet.find( hostType ) != m_HostSet.end(); }
    void                        removeHostType( EHostType hostType )    { m_HostSet.erase( hostType ); }
    int                         hostTypeCount( void )                   { return m_HostSet.size(); }

protected:
    AppCommon&                  m_MyApp;
    VxNetIdent                  m_NetIdent;
    VxGUID                      m_OnlineId;
    VxGUID                      m_SessionId;
    uint64_t                    m_LastUpdateTime{ 0 };
    int64_t                     m_NearbyTimeOrZero{ 0 };
    bool                        m_IsDirectConnect{ false };
    bool                        m_IsRelayed{ false };
    std::set<EHostType>         m_HostSet;
};
