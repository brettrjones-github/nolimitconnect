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

#include "GuiUserHost.h"

#include <GoTvCore/GoTvP2P/UserHostMgr/UserHostCallbackInterface.h>

#include <CoreLib/VxMutex.h>

#include <QObject>

class AppCommon;

class GuiUserHostMgr : public QObject, public UserHostCallbackInterface
{
    Q_OBJECT
public:
    GuiUserHostMgr() = delete;
    GuiUserHostMgr( AppCommon& app );
    GuiUserHostMgr( const GuiUserHostMgr& rhs ) = delete;
	virtual ~GuiUserHostMgr() = default;
    void                        onAppCommonCreated( void );

    void                        setMessengerReady( bool ready )             { m_MessengerReady = ready; }
    bool                        isMessengerReady( void )                    { return m_MessengerReady; }
    GuiUserHost *               getMyIdent( void )                          { return m_MyIdent; }  
    VxGUID                      getMyOnlineId( void )                       { return m_MyOnlineId; }  

    bool                        isUserHostInSession( VxGUID& onlineId );
    void                        setUserHostOffline( VxGUID& onlineId );

    void                        onUserHostAdded( GuiUserHost* user );
    void                        onUserHostRemoved( VxGUID& onlineId );
    void                        onUserHostUpdated( GuiUserHost* user );
    void                        onUserHostOnlineStatusChange( GuiUserHost* user, bool isOnline );
    void                        onMyIdentUpdated( GuiUserHost* user );

    void                        lockUserHostMgr( void )             { m_UserHostListMutex.lock(); }
    void                        unlockUserHostMgr( void )           { m_UserHostListMutex.unlock(); }
    GuiUserHost*                    getUserHost( VxGUID& onlineId );
    std::map<VxGUID, GuiUserHost*>& getUserHostList( void )             { return m_UserHostList; }

    void                        updateMyIdent( VxNetIdent* myIdent );

signals:
    void				        signalMyIdentUpdated( GuiUserHost* user ); 

    void				        signalUserHostAdded( GuiUserHost* user ); 
    void				        signalUserHostRemoved( VxGUID onlineId ); 
    void                        signalUserHostUpdated( GuiUserHost* user );
    void                        signalUserHostOnlineStatus( GuiUserHost* user, bool isOnline );

    void                        signalInternalUpdateUserHost( VxNetIdent* netIdent, EHostType hostType );
    void                        signalInternalUpdateMyIdent( VxNetIdent* netIdent );
    void                        signalInternalUserHostRemoved( VxGUID onlineId );
    void                        signalInternalUserHostOnlineStatus( VxNetIdent* netIdent, EHostType hostType, bool online );

private slots:
    void                        slotInternalUpdateUserHost( VxNetIdent* netIdent, EHostType hostType );
    void                        slotInternalUpdateMyIdent( VxNetIdent* netIdent );
    void                        slotInternalUserHostRemoved( VxGUID onlineId );
    void                        slotInternalUserHostOnlineStatus( VxNetIdent* netIdent, EHostType hostType, bool online );

protected:
    void                        removeUserHost( VxGUID& onlineId );
    GuiUserHost*                findUserHost( VxGUID& onlineId );
    GuiUserHost*                updateUserHost( VxNetIdent* hisIdent, EHostType hostType = eHostTypeUnknown );
    
    AppCommon&                  m_MyApp;
    VxMutex                     m_UserHostListMutex;
    // map of online id to GuiUserHost
    std::map<VxGUID, GuiUserHost*>  m_UserHostList;
    bool                        m_MessengerReady{ false };
    GuiUserHost*                m_MyIdent{ nullptr };
    VxGUID                      m_MyOnlineId;
};
