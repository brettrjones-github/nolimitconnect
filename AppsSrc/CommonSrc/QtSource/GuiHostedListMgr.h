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

#include "GuiHosted.h"

#include <ptop_src/ptop_engine_src/HostListMgr/HostedListCallbackInterface.h>
#include <ptop_src/ptop_engine_src/HostListMgr/HostedInfo.h>

#include <CoreLib/VxMutex.h>

#include <QObject>

class AppCommon;
class VxNetIdent;
class GuiHostedListCallback;

class GuiHostedListMgr : public QObject, public HostedListCallbackInterface
{
    Q_OBJECT
public:
    GuiHostedListMgr() = delete;
    GuiHostedListMgr( AppCommon& app );
    GuiHostedListMgr( const GuiHostedListMgr& rhs ) = delete;
	virtual ~GuiHostedListMgr() = default;
    void                        onAppCommonCreated( void );
    void                        onMessengerReady( bool ready ) { }
    bool                        isMessengerReady( void );
    virtual void                onSystemReady( bool ready ) { }

    void                        toGuiHostSearchResult( EHostType hostType, VxGUID& sessionId, HostedInfo& hostedInfo );
    void                        toGuiHostSearchComplete( EHostType hostType, VxGUID& sessionId );

    bool                        isHostedInSession( VxGUID& onlineId );
    void                        setHostedOffline( VxGUID& onlineId );

    void                        onHostedAdded( GuiHosted* guiHosted );
    void                        onHostedRemoved( VxGUID& onlineId, EHostType hostType );
    void                        onHostedUpdated( GuiHosted* guiHosted );
    void                        onUserOnlineStatusChange( GuiHosted* user, bool isOnline );
    void                        onMyIdentUpdated( GuiHosted* guiHosted );

    GuiHosted*                  getHosted( VxGUID& onlineId, EHostType hostType )   { return findHosted( onlineId, hostType ); }
    GuiHosted*                  getHosted( HostedId& hostTypeId )                   { return findHosted( hostTypeId ); }
    std::map<HostedId, GuiHosted*>& getHostedList( void )                           { return m_HostedList; }
    GuiHosted*                  updateHosted( VxNetIdent* hisIdent, EHostType hostType );

    EJoinState                  getHostJoinState( GroupieId& groupieId );

    void                        wantHostedListCallbacks( GuiHostedListCallback* client, bool enable );

    void                        setJoinOnStartup( std::string& hostUrl, bool enable );
    std::string&                getJoinOnStartup( void )                            { return m_FavoriteHostGroup; }
    bool                        isJoinOnStartup( std::string& hostUrl );

signals:
    void				        signalMyIdentUpdated( GuiHosted* guiHosted );

    void				        signalHostedRequested( GuiHosted* guiHosted ); 
    void                        signalHostedUpdated( GuiHosted* guiHosted );
    void				        signalHostedRemoved( VxGUID onlineId, EHostType hostType );
    void                        signalHostedOfferStateChange( VxGUID& userOnlineId, EHostType hostType, EJoinState hostOfferState );
    void                        signalHostedOnlineStatus( GuiHosted* guiHosted, bool isOnline );

    void                        signalInternalHostedUpdated( HostedInfo* hostedInfo );
    void                        signalInternalHostedRemoved( VxGUID hostOnlineId, EHostType hostType );
    void                        signalInternalHostSearchResult( HostedInfo* hostedInfo, VxGUID sessionId );
    void                        signalInternalHostSearchComplete( EHostType hostType, VxGUID sessionId );

private slots:
    void                        slotInternalHostedUpdated( HostedInfo* hostedInfo );
    void                        slotInternalHostedRemoved( VxGUID hostOnlineId, EHostType hostType );
    void                        slotInternalHostSearchResult( HostedInfo* hostedInfo, VxGUID sessionId );
    void                        slotInternalHostSearchComplete( EHostType hostType, VxGUID sessionId );

    void                        slotNetAvailableStatus( ENetAvailStatus eNetAvailStatus );

protected:
    void                        removeHosted( VxGUID& onlineId, EHostType hostType );
    GuiHosted*                  findHosted( VxGUID& onlineId, EHostType hostType );
    GuiHosted*                  findHosted( HostedId& hostTypeId );
    GuiHosted*                  updateHostedInfo( HostedInfo& hostedInfo );
    void                        updateHostSearchResult( HostedInfo& hostedInfo, VxGUID& sessionId );
    
    virtual void				callbackHostedInfoListUpdated( HostedInfo* hostedInfo ) override;
    virtual void				callbackHostedInfoListRemoved( VxGUID& hostOnlineId, EHostType hostType ) override;
    virtual void				callbackHostedInfoListSearchResult( HostedInfo* hostedInfo, VxGUID& hostOnlineId ) override;
    virtual void				callbackHostedInfoListSearchComplete( EHostType hostType, VxGUID& sessionId ) override;

    void                        announceHostedListUpdated( HostedId& hostedId, GuiHosted* guiHosted );
    void                        announceHostedListRemoved( HostedId& hostedId );
    void                        announceHostedListSearchResult( HostedId& hostedId, GuiHosted* guiHosted, VxGUID& sessionId );
    void                        announceHostedListSearchComplete( EHostType hostType, VxGUID& sessionId );

    void                        checkAutoJoinGroupHost( void );
    
    AppCommon&                  m_MyApp;
    std::string                 m_FavoriteHostGroup{ "" };
    bool                        m_AttemptedJoinHostGroup{ false };
    // map of online id to GuiHosted
    std::map<HostedId, GuiHosted*>  m_HostedList;

    std::vector<GuiHostedListCallback*>  m_HostedListClients;
};
