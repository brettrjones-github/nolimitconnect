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

#include "GuiHostJoin.h"

#include <ptop_src/ptop_engine_src/HostJoinMgr/HostJoinCallbackInterface.h>
#include <ptop_src/ptop_engine_src/HostJoinMgr/HostJoinInfo.h>

#include <CoreLib/VxMutex.h>

#include <QObject>

// hosted side callbacks with client user states

class AppCommon;

class GuiHostJoinMgr : public QObject, public HostJoinCallbackInterface
{
    Q_OBJECT
public:
    GuiHostJoinMgr() = delete;
    GuiHostJoinMgr( AppCommon& app );
    GuiHostJoinMgr( const GuiHostJoinMgr& rhs ) = delete;
	virtual ~GuiHostJoinMgr() = default;
    void                        onAppCommonCreated( void );
    void                        onMessengerReady( bool ready );
    bool                        isMessengerReady( void );
    virtual void                onSystemReady( bool ready );

    GuiHostJoin *               getMyIdent( void )                          { return m_MyIdent; }  
    VxGUID                      getMyOnlineId( void )                       { return m_MyOnlineId; }  

    bool                        isHostJoinInSession( VxGUID& onlineId );
    void                        setHostJoinOffline( VxGUID& onlineId );

    void                        onHostJoinAdded( GuiHostJoin* user );
    void                        onHostJoinUpdated( GuiHostJoin* user );
    void                        onUserOnlineStatusChange( GuiHostJoin* user, bool isOnline );

    GuiHostJoin*                    getHostJoin( VxGUID& onlineId );
    std::map<VxGUID, GuiHostJoin*>& getHostJoinList( void )             { return m_HostJoinList; }

    void                        joinAccepted( GuiHostJoin* hostJoin, EHostType hostType );
    void                        joinRejected( GuiHostJoin* hostJoin, EHostType hostType );

signals:
    void                        signalHostJoinRequestCount( int requestCnt );

    void                        signalHostJoinRequested( GuiHostJoin* user );
    void				        signalHostJoinUpdated( GuiHostJoin* user );
    void				        signalHostJoinRemoved( VxGUID& onlineId, EHostType hostType );
    void                        signalHostJoinOfferStateChange( VxGUID& userOnlineId, EHostType hostType, EJoinState hostOfferState );
    void                        signalHostJoinOnlineStatus( GuiHostJoin* user, bool isOnline );

    void                        signalInternalHostJoinRequested( HostJoinInfo* hostJoinInfo );
    void                        signalInternalHostJoinUpdated( HostJoinInfo* hostJoinInfo );
    void                        signalInternalHostJoinRemoved( VxGUID hostOnlineId, EPluginType pluginType );
    void                        signalInternalHostJoinOfferState( VxGUID hostOnlineId, EPluginType pluginType, EJoinState joinOfferState );
    void                        signalInternalHostJoinOnlineState( VxGUID hostOnlineId, EPluginType pluginType, EOnlineState onlineState, VxGUID connectionId );

private slots:
    void                        slotInternalHostJoinRequested( HostJoinInfo* hostJoinInfo );
    void                        slotInternalHostJoinUpdated( HostJoinInfo* hostJoinInfo );
    void                        slotInternalHostJoinRemoved( VxGUID hostOnlineId, EPluginType pluginType );
    void                        slotInternalHostJoinOfferState( VxGUID userOnlineId, EPluginType pluginType, EJoinState joinOfferState );
    void                        slotInternalHostJoinOnlineState( VxGUID userOnlineId, EPluginType pluginType, EOnlineState onlineState, VxGUID connectionId );

protected:
    GuiHostJoin*                findHostJoin( VxGUID& onlineId );
    GuiHostJoin*                updateHostJoin( HostJoinInfo* hostJoinInfo );
    void                        updateHostRequestCount( bool forceEmit = false );

    virtual void				callbackHostJoinRequested( HostJoinInfo* userHostInfo ) override;
    virtual void				callbackHostJoinUpdated( HostJoinInfo* userHostInfo ) override;
    virtual void				callbackHostJoinRemoved( VxGUID& userOnlineId, EPluginType pluginType ) override;
    virtual void				callbackHostJoinOfferState( VxGUID& userOnlineId, EPluginType pluginType, EJoinState userHostOfferState ) override;
    virtual void				callbackHostJoinOnlineState( VxGUID& userOnlineId, EPluginType pluginType, EOnlineState onlineState, VxGUID& connectionId ) override;
    
    AppCommon&                  m_MyApp;
    // map of online id to GuiHostJoin
    std::map<VxGUID, GuiHostJoin*>  m_HostJoinList;
    GuiHostJoin*                m_MyIdent{ nullptr };
    VxGUID                      m_MyOnlineId;
    int                         m_HostRequestCount{ 0 };
};
