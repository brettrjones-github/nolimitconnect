//============================================================================
// Copyright (C) 2021 Brett R. Jones
// Issued to MIT style license by Brett R. Jones in 2017
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

#include "GuiUserJoinMgr.h"
#include "AppCommon.h"

//============================================================================
GuiUserJoinMgr::GuiUserJoinMgr( AppCommon& app )
    : QObject( &app )
    , m_MyApp( app )
{
}

//============================================================================
void GuiUserJoinMgr::onAppCommonCreated( void )
{
    connect( this, SIGNAL( signalInternalUpdateUserJoin(VxNetIdent*,EHostType) ),	                this, SLOT( slotInternalUpdateUserJoin(VxNetIdent*,EHostType) ), Qt::QueuedConnection );
    connect( this, SIGNAL( signalInternalUserJoinRemoved(VxGUID) ),	                                this, SLOT( slotInternalUserJoinRemoved(VxGUID) ), Qt::QueuedConnection );
    connect( this, SIGNAL( signalInternalUserJoinOnlineStatus( VxNetIdent*, EHostType, bool ) ),    this, SLOT( slotInternalUserJoinOnlineStatus( VxNetIdent*, EHostType, bool ) ), Qt::QueuedConnection );
}

//============================================================================
void GuiUserJoinMgr::slotInternalUpdateUserJoin( VxNetIdent* netIdent, EHostType hostType )
{
    updateUserJoin( netIdent, hostType );
}

//============================================================================
void GuiUserJoinMgr::slotInternalUpdateMyIdent( VxNetIdent* netIdent )
{
    updateMyIdent( netIdent );
}

//============================================================================
void GuiUserJoinMgr::slotInternalUserJoinRemoved( VxGUID onlineId )
{
    m_UserJoinListMutex.lock();
    GuiUserJoin* guiUserJoin = findUserJoin( onlineId );
    m_UserJoinListMutex.unlock();
    if( guiUserJoin )
    {
        onUserJoinRemoved( onlineId );
    }

    removeUserJoin( onlineId );
}

//============================================================================
void GuiUserJoinMgr::slotInternalUserJoinOnlineStatus( VxNetIdent* netIdent, EHostType hostType, bool online )
{
    GuiUserJoin* guiUserJoin = updateUserJoin( netIdent, hostType );
    if( guiUserJoin )
    {
        guiUserJoin->setOnlineStatus( online );
    }
}

//============================================================================
GuiUserJoin* GuiUserJoinMgr::findUserJoin( VxGUID& onlineId )
{
    GuiUserJoin* user = nullptr;
    auto iter = m_UserJoinList.find( onlineId );
    if( iter != m_UserJoinList.end() )
    {
        user = iter->second;
    }

    return user;
}

//============================================================================
void GuiUserJoinMgr::removeUserJoin( VxGUID& onlineId )
{
    m_UserJoinListMutex.lock();
    auto iter = m_UserJoinList.find( onlineId );
    if( iter != m_UserJoinList.end() )
    {
        iter->second->deleteLater();
        m_UserJoinList.erase( iter );
    }

    m_UserJoinListMutex.unlock();
}

//============================================================================
bool GuiUserJoinMgr::isUserJoinInSession( VxGUID& onlineId )
{
    m_UserJoinListMutex.lock();
    GuiUserJoin* guiUserJoin = findUserJoin( onlineId );
    m_UserJoinListMutex.unlock();
    return guiUserJoin && guiUserJoin->isInSession();
}

//============================================================================
GuiUserJoin* GuiUserJoinMgr::getUserJoin( VxGUID& onlineId )
{
    m_UserJoinListMutex.lock();
    GuiUserJoin* guiUserJoin = findUserJoin( onlineId );
    m_UserJoinListMutex.unlock();
    return guiUserJoin;
}

//============================================================================
GuiUserJoin* GuiUserJoinMgr::updateUserJoin( VxNetIdent* hisIdent, EHostType hostType )
{
    if( !hisIdent )
    {
        LogMsg( LOG_ERROR, "GuiUserJoinMgr::updateUserJoinOnline invalid param" );
        return nullptr;
    }

    m_UserJoinListMutex.lock();
    GuiUserJoin* guiUserJoin = findUserJoin( hisIdent->getMyOnlineId() );
    m_UserJoinListMutex.unlock();
    if( guiUserJoin && guiUserJoin->getMyOnlineId() == hisIdent->getMyOnlineId() )
    {
        guiUserJoin->addHostType( hostType );
        onUserJoinUpdated( guiUserJoin );
    }
    else
    {
        guiUserJoin = new GuiUserJoin( m_MyApp );
        guiUserJoin->setNetIdent( hisIdent );
        guiUserJoin->addHostType( hostType );
        m_UserJoinListMutex.lock();
        m_UserJoinList[guiUserJoin->getMyOnlineId()] = guiUserJoin;
        m_UserJoinListMutex.unlock();
        onUserJoinAdded( guiUserJoin );
    }

    return guiUserJoin;
}

//============================================================================
void GuiUserJoinMgr::updateMyIdent( VxNetIdent* myIdent )
{
    if( !m_MyIdent )
    {
        GuiUserJoin* guiUserJoin = new GuiUserJoin( m_MyApp );
        guiUserJoin->setNetIdent( myIdent );
        m_UserJoinListMutex.lock();
        m_MyIdent = guiUserJoin;
        m_MyOnlineId = m_MyIdent->getMyOnlineId();
        m_UserJoinListMutex.unlock();
    }

    emit signalMyIdentUpdated( m_MyIdent );
}

//============================================================================
void GuiUserJoinMgr::setUserJoinOffline( VxGUID& onlineId )
{
    m_UserJoinListMutex.lock();
    GuiUserJoin* guiUserJoin = findUserJoin( onlineId );
    m_UserJoinListMutex.unlock();
    if( guiUserJoin )
    {
        guiUserJoin->setOnlineStatus( false );
    }
}

//============================================================================
void GuiUserJoinMgr::onUserJoinAdded( GuiUserJoin* user )
{
    if( isMessengerReady() )
    {
        emit signalUserJoinAdded( user );
    }
}

//============================================================================
void GuiUserJoinMgr::onUserJoinRemoved( VxGUID& onlineId )
{
    if( isMessengerReady() )
    {
        emit signalUserJoinRemoved( onlineId );
    }
}

//============================================================================
void GuiUserJoinMgr::onUserJoinOnlineStatusChange( GuiUserJoin* user, bool isOnline )
{
    if( isMessengerReady() )
    {
        emit signalUserJoinOnlineStatus( user, isOnline );
    }
}

//============================================================================
void GuiUserJoinMgr::onUserJoinUpdated( GuiUserJoin* user )
{
    if( isMessengerReady() )
    {
        emit signalUserJoinUpdated( user );
    }
}

//============================================================================
void GuiUserJoinMgr::onMyIdentUpdated( GuiUserJoin* user )
{
    if( isMessengerReady() )
    {
        emit signalMyIdentUpdated( user );
    }
}