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

#include <ptop_src/ptop_engine_src/IdentListMgrs/ConnectIdListCallbackInterface.h>

#include <PktLib/ConnectId.h>

#include <set>
#include <map>

class AppCommon;
class GuiConnectIdListCallback;

class GuiConnectIdListMgr : public QObject, public ConnectIdListCallbackInterface
{
    Q_OBJECT
public:
    GuiConnectIdListMgr() = delete;
    GuiConnectIdListMgr( AppCommon& app );
    GuiConnectIdListMgr( const GuiConnectIdListMgr& rhs ) = delete;
	virtual ~GuiConnectIdListMgr() = default;
    virtual void                onAppCommonCreated( void );
    virtual void                onMessengerReady( bool ready ) { }
    virtual bool                isMessengerReady( void );
    virtual void                onSystemReady( bool ready ) { }

    void                        wantGuiConnectIdCallbacks( GuiConnectIdListCallback* callback, bool wantCallback );

    virtual void				callbackRelayStatusChange( VxGUID& connectId, bool isRelayed ) override;
    virtual void				callbackOnlineStatusChange( VxGUID& onlineId, bool isOnline ) override;
    // callbackConnectionStatusChange should happen before callbackOnlineStatusChange when user disconnects from host
    virtual void				callbackConnectionStatusChange( ConnectId& connectId, bool isConnected ) override;
    virtual void				callbackConnectionReason( VxGUID& sktConnectId, EConnectReason connectReason, bool enableReason ) override;
    virtual void				callbackConnectionLost( VxGUID& sktConnectId ) override;

    bool                        isOnline( VxGUID& onlineId );
    bool                        isRelayed( VxGUID& onlineId );

signals:
    void				        signalInternalOnlineStatusChange( VxGUID onlineId, bool isOnline );
    void				        signalInternalRelayStatusChange( VxGUID onlineId, bool isRelayed );
    void				        signalInternalConnectionStatusChange( ConnectId connectId, bool isConnected );
    void				        signalInternalConnectionReason( VxGUID sktConnectId, EConnectReason connectReason, bool enableReason );
    void				        signalInternalConnectionLost( VxGUID sktConnectId );

private slots:
    void				        slotInternalOnlineStatusChange( VxGUID onlineId, bool isOnline );
    void				        slotInternalRelayStatusChange( VxGUID onlineId, bool isOnline );
    void				        slotInternalConnectionStatusChange( ConnectId connectId, bool isConnected );
    void				        slotInternalConnectionReason( VxGUID sktConnectId, EConnectReason connectReaso, bool enableReasonn );
    void				        slotInternalConnectionLost( VxGUID sktConnectId );

protected: 
    void                        onRelayStatusChange( VxGUID& onlineId, bool isRelayed );
    void                        onOnlineStatusChange( VxGUID& onlineId, bool isOnline );
    void                        onConnectionStatusChange( ConnectId& connectId, bool isConnected );

    void                        announceRelayStatusChange( VxGUID& onlineId, bool isRelayed );
    void                        announceOnlineStatusChange( VxGUID& onlineId, bool isOnline );
    void                        announceConnectionStatusChange( ConnectId& connectId, bool isConnected );

    AppCommon&                  m_MyApp;
    std::vector<GuiConnectIdListCallback*> m_GuiConnectIdClientList;
    std::set<ConnectId>         m_ConnectIdList;
    std::map<VxGUID, bool>      m_RelayedIdList;
    std::map<VxGUID, bool>      m_OnlineList;
    std::map<VxGUID, std::set<EConnectReason>>      m_ConnectReasonList;
};
