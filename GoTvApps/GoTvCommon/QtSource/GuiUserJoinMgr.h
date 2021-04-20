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

#include "GuiUserJoin.h"

#include <GoTvCore/GoTvP2P/UserJoinMgr/UserJoinCallbackInterface.h>

#include <CoreLib/VxMutex.h>

#include <QObject>

class AppCommon;

class GuiUserJoinMgr : public QObject, public UserJoinCallbackInterface
{
    Q_OBJECT
public:
    GuiUserJoinMgr() = delete;
    GuiUserJoinMgr( AppCommon& app );
    GuiUserJoinMgr( const GuiUserJoinMgr& rhs ) = delete;
	virtual ~GuiUserJoinMgr() = default;
    void                        onAppCommonCreated( void );

    void                        setMessengerReady( bool ready )             { m_MessengerReady = ready; }
    bool                        isMessengerReady( void )                    { return m_MessengerReady; }
    GuiUserJoin *               getMyIdent( void )                          { return m_MyIdent; }  
    VxGUID                      getMyOnlineId( void )                       { return m_MyOnlineId; }  

    bool                        isUserJoinInSession( VxGUID& onlineId );
    void                        setUserJoinOffline( VxGUID& onlineId );

    void                        onUserJoinAdded( GuiUserJoin* user );
    void                        onUserJoinRemoved( VxGUID& onlineId );
    void                        onUserJoinUpdated( GuiUserJoin* user );
    void                        onUserJoinOnlineStatusChange( GuiUserJoin* user, bool isOnline );
    void                        onMyIdentUpdated( GuiUserJoin* user );

    void                        lockUserJoinMgr( void )             { m_UserJoinListMutex.lock(); }
    void                        unlockUserJoinMgr( void )           { m_UserJoinListMutex.unlock(); }
    GuiUserJoin*                    getUserJoin( VxGUID& onlineId );
    std::map<VxGUID, GuiUserJoin*>& getUserJoinList( void )             { return m_UserJoinList; }

    void                        updateMyIdent( VxNetIdent* myIdent );

signals:
    void				        signalMyIdentUpdated( GuiUserJoin* user ); 

    void				        signalUserJoinAdded( GuiUserJoin* user ); 
    void				        signalUserJoinRemoved( VxGUID onlineId ); 
    void                        signalUserJoinUpdated( GuiUserJoin* user );
    void                        signalUserJoinOnlineStatus( GuiUserJoin* user, bool isOnline );

    void                        signalInternalUpdateUserJoin( VxNetIdent* netIdent, EHostType hostType );
    void                        signalInternalUpdateMyIdent( VxNetIdent* netIdent );
    void                        signalInternalUserJoinRemoved( VxGUID onlineId );
    void                        signalInternalUserJoinOnlineStatus( VxNetIdent* netIdent, EHostType hostType, bool online );

private slots:
    void                        slotInternalUpdateUserJoin( VxNetIdent* netIdent, EHostType hostType );
    void                        slotInternalUpdateMyIdent( VxNetIdent* netIdent );
    void                        slotInternalUserJoinRemoved( VxGUID onlineId );
    void                        slotInternalUserJoinOnlineStatus( VxNetIdent* netIdent, EHostType hostType, bool online );

protected:
    void                        removeUserJoin( VxGUID& onlineId );
    GuiUserJoin*                findUserJoin( VxGUID& onlineId );
    GuiUserJoin*                updateUserJoin( VxNetIdent* hisIdent, EHostType hostType = eHostTypeUnknown );
    
    AppCommon&                  m_MyApp;
    VxMutex                     m_UserJoinListMutex;
    // map of online id to GuiUserJoin
    std::map<VxGUID, GuiUserJoin*>  m_UserJoinList;
    bool                        m_MessengerReady{ false };
    GuiUserJoin*                m_MyIdent{ nullptr };
    VxGUID                      m_MyOnlineId;
};
