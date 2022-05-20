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

#include "UserOnlineMgr.h"
#include "UserOnlineCallbackInterface.h"

#include <ptop_src/ptop_engine_src/P2PEngine/P2PEngine.h>
#include <ptop_src/ptop_engine_src/User/User.h>

#include <CoreLib/VxTime.h>

#include <time.h>

//============================================================================
UserOnlineMgr::UserOnlineMgr( P2PEngine& engine, const char * dbName, const char * dbStateName )
: m_Engine( engine )
{
    m_Engine.getConnectIdListMgr().wantConnectIdListCallback( this, true );
}

//============================================================================
void UserOnlineMgr::fromGuiUserLoggedOn( void )
{
    // dont call HostBaseMgr::fromGuiUserLoggedOn because we never generate sha hash for thumbnails
    if( !m_Initialized )
    {
        m_Initialized = true;
    }
}

//============================================================================
void UserOnlineMgr::callbackOnlineStatusChange( VxGUID& onlineId, bool isOnline )
{
    lockResources();
    User* user = findUser( onlineId );
    if( user && user->getNetIdent() )
    {
        announceUserOnlineState( user, isOnline );
    }

    unlockResources();
}

//============================================================================
void UserOnlineMgr::callbackConnectionStatusChange( ConnectId& connectId, bool isConnected )
{
    lockResources();
    User* user = findUser( connectId.getGroupieOnlineId() );
    if( user && user->getNetIdent() )
    {
        announceUserOnlineState( user, isConnected );
    }

    unlockResources();
}

//============================================================================
void UserOnlineMgr::addUserOnlineMgrClient( UserOnlineCallbackInterface * client, bool enable )
{
    lockClientList();
    if( enable )
    {
        m_UserOnlineClients.push_back( client );
    }
    else
    {
        std::vector<UserOnlineCallbackInterface *>::iterator iter;
        for( iter = m_UserOnlineClients.begin(); iter != m_UserOnlineClients.end(); ++iter )
        {
            if( *iter == client )
            {
                m_UserOnlineClients.erase( iter );
                break;
            }
        }
    }

    unlockClientList();
}

//============================================================================
void UserOnlineMgr::announceUserOnlineAdded( User * userInfo, BaseSessionInfo& sessionInfo )
{
    User * userHostInfo = userInfo;
    if( userHostInfo )
    {
	    LogMsg( LOG_INFO, "UserOnlineMgr::announceUserOnlineAdded start" );
	
	    lockClientList();
	    std::vector<UserOnlineCallbackInterface *>::iterator iter;
	    for( iter = m_UserOnlineClients.begin();	iter != m_UserOnlineClients.end(); ++iter )
	    {
		    UserOnlineCallbackInterface * client = *iter;
		    client->callbackUserOnlineAdded( userHostInfo );
	    }

	    unlockClientList();
	    LogMsg( LOG_INFO, "UserOnlineMgr::announceUserOnlineAdded done" );
    }
    else
    {
        LogMsg( LOG_ERROR, "UserOnlineMgr::announceUserOnlineAdded dynamic_cast failed" );
    }
}

//============================================================================
void UserOnlineMgr::announceUserOnlineUpdated( User * userInfo, BaseSessionInfo& sessionInfo )
{
    User * userHostInfo = userInfo;
    if( userHostInfo )
    {
        lockClientList();
        std::vector<UserOnlineCallbackInterface *>::iterator iter;
        for( iter = m_UserOnlineClients.begin();	iter != m_UserOnlineClients.end(); ++iter )
        {
            UserOnlineCallbackInterface * client = *iter;
            client->callbackUserOnlineUpdated( userHostInfo );
        }

        unlockClientList();
    }
    else
    {
        LogMsg( LOG_ERROR, "UserOnlineMgr::announceUserOnlineRemoved dynamic_cast failed" );
    }
}

//============================================================================
void UserOnlineMgr::announceUserOnlineRemoved( VxGUID& hostOnlineId, EHostType hostType )
{
	lockClientList();
	std::vector<UserOnlineCallbackInterface *>::iterator iter;
	for( iter = m_UserOnlineClients.begin();	iter != m_UserOnlineClients.end(); ++iter )
	{
		UserOnlineCallbackInterface * client = *iter;
		client->callbackUserOnlineRemoved( hostOnlineId );
	}

	unlockClientList();
}

//============================================================================
void UserOnlineMgr::announceUserOnlineState( User* user, bool isOnline )
{
    lockClientList();
    std::vector<UserOnlineCallbackInterface *>::iterator iter;
    for( iter = m_UserOnlineClients.begin(); iter != m_UserOnlineClients.end(); ++iter )
    {
        UserOnlineCallbackInterface * client = *iter;
        client->callbackUserOnlineState( user, isOnline );
    }

    unlockClientList();
}

//============================================================================
void UserOnlineMgr::announceUserSessionState( User* user, bool isInSession )
{
    /*
    lockClientList();
    std::vector<UserOnlineCallbackInterface*>::iterator iter;
    for( iter = m_UserOnlineClients.begin(); iter != m_UserOnlineClients.end(); ++iter )
    {
        UserOnlineCallbackInterface* client = *iter;
        client->callbackUserSessionState( user, isInSession );
    }

    unlockClientList();
    */
}

//============================================================================
void UserOnlineMgr::onUserOnline( VxSktBase * sktBase, VxNetIdent * netIdent, BaseSessionInfo& sessionInfo )
{
    updateUserSession( sktBase, netIdent, sessionInfo );
}

//============================================================================
bool UserOnlineMgr::onUserOnline( GroupieId& groupieId, VxSktBase* sktBase, VxNetIdent* netIdent )
{
    bool wasAdded = false;
    lockResources();
    User* user = findUser( netIdent->getMyOnlineId() );
    if( !user )
    {
        wasAdded = true;
        user = new User( m_Engine, netIdent );
        m_UserOnlineList.push_back( user );
    }

    unlockResources();
    if( wasAdded && !sktBase->isTempConnection() )
    {
        m_Engine.getToGui().toGuiContactAdded( netIdent );
    }

    callbackOnlineStatusChange( groupieId.getGroupieOnlineId(), true );
    return wasAdded;
}

//============================================================================
void UserOnlineMgr::onHostJoinRequestedByUser( VxSktBase* sktBase, VxNetIdent* netIdent, BaseSessionInfo& sessionInfo )
{
    updateUserSession( sktBase, netIdent, sessionInfo ); 
}

//============================================================================
void UserOnlineMgr::onHostJoinedByUser( VxSktBase * sktBase, VxNetIdent * netIdent, BaseSessionInfo& sessionInfo )
{
    updateUserSession( sktBase, netIdent, sessionInfo );
}

//============================================================================
void UserOnlineMgr::onHostLeftByUser( VxSktBase* sktBase, VxNetIdent* netIdent, BaseSessionInfo& sessionInfo )
{
    updateUserSession( sktBase, netIdent, sessionInfo, true );
}

//============================================================================
void UserOnlineMgr::onUserJoinedHost( VxSktBase * sktBase, VxNetIdent * netIdent, BaseSessionInfo& sessionInfo )
{
    updateUserSession( sktBase, netIdent, sessionInfo );
}

//============================================================================
void UserOnlineMgr::onUserJoinedHost( GroupieId& groupieId, VxSktBase* sktBase, VxNetIdent* netIdent, BaseSessionInfo& sessionInfo )
{
    updateUserSession( groupieId, sktBase, netIdent, sessionInfo, false );
}

//============================================================================
void UserOnlineMgr::onUserLeftHost( GroupieId& groupieId, VxSktBase* sktBase, VxNetIdent* netIdent, BaseSessionInfo& sessionInfo )
{
    updateUserSession( groupieId, sktBase, netIdent, sessionInfo, true );
}

//============================================================================
void UserOnlineMgr::onUserUnJoinedHost( GroupieId& groupieId, VxSktBase* sktBase, VxNetIdent* netIdent, BaseSessionInfo& sessionInfo )
{
    updateUserSession( groupieId, sktBase, netIdent, sessionInfo, true );
}

//============================================================================
void UserOnlineMgr::updateUserSession( VxSktBase* sktBase, VxNetIdent* netIdent, BaseSessionInfo& sessionInfo, bool leftHost )
{
    if( IsHostPluginType( sessionInfo.getPluginType() ) )
    {
        GroupieId groupieId( m_Engine.getMyOnlineId(), sessionInfo.getOnlineId(), PluginTypeToHostType( sessionInfo.getPluginType() ) );
        updateUserSession( groupieId, sktBase, netIdent, sessionInfo, leftHost );
    }
    else
    {
        GroupieId groupieId(sessionInfo.getOnlineId(), m_Engine.getMyOnlineId(), PluginTypeToHostType( sessionInfo.getPluginType() ) );
        updateUserSession( groupieId, sktBase, netIdent, sessionInfo, leftHost );
    }
}

//============================================================================
void UserOnlineMgr::updateUserSession( GroupieId& groupieId, VxSktBase * sktBase, VxNetIdent * netIdent, BaseSessionInfo& sessionInfo, bool leftHost )
{
    bool wasAdded = false;
    bool wasInSession = false;
    bool isinSession = false;
    lockResources();
    User* user = findUser( groupieId.getGroupieOnlineId() );
    if( !user )
    {
        user = new User( m_Engine, netIdent, sessionInfo );
        wasAdded = true;
    }
    else
    {
        wasInSession = user->isInSession();
        if( leftHost )
        {
            user->removeSession( sessionInfo );
        }
        else
        {
            user->addSession( sessionInfo );
        }
        
        isinSession = user->isInSession();
    }

    unlockResources();

    if( !leftHost )
    {
        m_Engine.getThumbMgr().queryThumbIfNeeded( sktBase, netIdent, sessionInfo.getPluginType() );
    }

    if( wasAdded )
    {
        announceUserOnlineAdded( user, sessionInfo );
    }
    else
    {
        announceUserOnlineUpdated( user, sessionInfo );
    }

    if( wasInSession != isinSession )
    {
        announceUserSessionState( user, isinSession );
    }
}

//============================================================================
User* UserOnlineMgr::findUser( VxGUID& onlineId )
{
    for( auto user : m_UserOnlineList )
    {
        if( user->getMyOnlineId() == onlineId )
        {
            return user;
        }
    }

    return nullptr;
}

//============================================================================
void UserOnlineMgr::onConnectionLost( VxSktBase* sktBase, VxGUID& connectionId, VxGUID& peerOnlineId )
{
    // TODO BRJ handle disconnect
}

//============================================================================
void UserOnlineMgr::onUserOffline( VxGUID& onlineId )
{
    lockResources();
    User* user = findUser( onlineId );
    if( !user )
    {
        unlockResources();

        lockClientList();
        std::vector<UserOnlineCallbackInterface *>::iterator iter;
        for( iter = m_UserOnlineClients.begin(); iter != m_UserOnlineClients.end(); ++iter )
        {
            UserOnlineCallbackInterface * client = *iter;
            client->callbackUserOffline( onlineId );
        }

        unlockClientList();
    }
    else if( user->isInSession() )
    {
        lockClientList();
        std::vector<UserOnlineCallbackInterface *>::iterator iter;
        for( iter = m_UserOnlineClients.begin(); iter != m_UserOnlineClients.end(); ++iter )
        {
            UserOnlineCallbackInterface * client = *iter;
            client->callbackUserOnlineState( user, false );
        }

        unlockClientList();
        unlockResources();

    }
}
