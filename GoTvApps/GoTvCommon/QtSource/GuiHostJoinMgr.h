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

#include <CoreLib/VxMutex.h>

#include <QObject>

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

    void                        setMessengerReady( bool ready )             { m_MessengerReady = ready; }
    bool                        isMessengerReady( void )                    { return m_MessengerReady; }
    GuiHostJoin *               getMyIdent( void )                          { return m_MyIdent; }  
    VxGUID                      getMyOnlineId( void )                       { return m_MyOnlineId; }  

    bool                        isHostJoinInSession( VxGUID& onlineId );
    void                        setHostJoinOffline( VxGUID& onlineId );

    void                        onHostJoinAdded( GuiHostJoin* user );
    void                        onHostJoinRemoved( VxGUID& onlineId );
    void                        onHostJoinUpdated( GuiHostJoin* user );
    void                        onHostJoinOnlineStatusChange( GuiHostJoin* user, bool isOnline );
    void                        onMyIdentUpdated( GuiHostJoin* user );

    void                        lockHostJoinMgr( void )             { m_HostJoinListMutex.lock(); }
    void                        unlockHostJoinMgr( void )           { m_HostJoinListMutex.unlock(); }
    GuiHostJoin*                    getHostJoin( VxGUID& onlineId );
    std::map<VxGUID, GuiHostJoin*>& getHostJoinList( void )             { return m_HostJoinList; }

    void                        updateMyIdent( VxNetIdent* myIdent );

signals:
    void				        signalMyIdentUpdated( GuiHostJoin* user ); 

    void				        signalHostJoinAdded( GuiHostJoin* user ); 
    void				        signalHostJoinRemoved( VxGUID onlineId ); 
    void                        signalHostJoinUpdated( GuiHostJoin* user );
    void                        signalHostJoinOnlineStatus( GuiHostJoin* user, bool isOnline );

    void                        signalInternalUpdateHostJoin( VxNetIdent* netIdent, EHostType hostType );
    void                        signalInternalUpdateMyIdent( VxNetIdent* netIdent );
    void                        signalInternalHostJoinRemoved( VxGUID onlineId );
    void                        signalInternalHostJoinOnlineStatus( VxNetIdent* netIdent, EHostType hostType, bool online );

private slots:
    void                        slotInternalUpdateHostJoin( VxNetIdent* netIdent, EHostType hostType );
    void                        slotInternalUpdateMyIdent( VxNetIdent* netIdent );
    void                        slotInternalHostJoinRemoved( VxGUID onlineId );
    void                        slotInternalHostJoinOnlineStatus( VxNetIdent* netIdent, EHostType hostType, bool online );

protected:
    void                        removeHostJoin( VxGUID& onlineId );
    GuiHostJoin*                findHostJoin( VxGUID& onlineId );
    GuiHostJoin*                updateHostJoin( VxNetIdent* hisIdent, EHostType hostType = eHostTypeUnknown );
    
    AppCommon&                  m_MyApp;
    VxMutex                     m_HostJoinListMutex;
    // map of online id to GuiHostJoin
    std::map<VxGUID, GuiHostJoin*>  m_HostJoinList;
    bool                        m_MessengerReady{ false };
    GuiHostJoin*                m_MyIdent{ nullptr };
    VxGUID                      m_MyOnlineId;
};
