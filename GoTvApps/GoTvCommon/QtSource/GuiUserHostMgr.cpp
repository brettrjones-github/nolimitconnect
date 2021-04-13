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

#include "GuiUserHostMgr.h"
#include "AppCommon.h"

//============================================================================
GuiUserHostMgr::GuiUserHostMgr( AppCommon& app )
    : QObject( &app )
    , m_MyApp( app )
{
}

//============================================================================
void GuiUserHostMgr::onAppCommonCreated( void )
{
    connect( this, SIGNAL( signalInternalUpdateUserHost(VxNetIdent*,EHostType) ),	                this, SLOT( slotInternalUpdateUserHost(VxNetIdent*,EHostType) ), Qt::QueuedConnection );
    connect( this, SIGNAL( signalInternalUserHostRemoved(VxGUID) ),	                                this, SLOT( slotInternalUserHostRemoved(VxGUID) ), Qt::QueuedConnection );
    connect( this, SIGNAL( signalInternalUserHostOnlineStatus( VxNetIdent*, EHostType, bool ) ),    this, SLOT( slotInternalUserHostOnlineStatus( VxNetIdent*, EHostType, bool ) ), Qt::QueuedConnection );
}

//============================================================================
void GuiUserHostMgr::slotInternalUpdateUserHost( VxNetIdent* netIdent, EHostType hostType )
{
    updateUserHost( netIdent, hostType );
}

//============================================================================
void GuiUserHostMgr::slotInternalUpdateMyIdent( VxNetIdent* netIdent )
{
    updateMyIdent( netIdent );
}

//============================================================================
void GuiUserHostMgr::slotInternalUserHostRemoved( VxGUID onlineId )
{
    m_UserHostListMutex.lock();
    GuiUserHost* guiUserHost = findUserHost( onlineId );
    m_UserHostListMutex.unlock();
    if( guiUserHost )
    {
        onUserHostRemoved( onlineId );
    }

    removeUserHost( onlineId );
}

//============================================================================
void GuiUserHostMgr::slotInternalUserHostOnlineStatus( VxNetIdent* netIdent, EHostType hostType, bool online )
{
    GuiUserHost* guiUserHost = updateUserHost( netIdent, hostType );
    if( guiUserHost )
    {
        guiUserHost->setOnlineStatus( online );
    }
}

//============================================================================
GuiUserHost* GuiUserHostMgr::findUserHost( VxGUID& onlineId )
{
    GuiUserHost* user = nullptr;
    auto iter = m_UserHostList.find( onlineId );
    if( iter != m_UserHostList.end() )
    {
        user = iter->second;
    }

    return user;
}

//============================================================================
void GuiUserHostMgr::removeUserHost( VxGUID& onlineId )
{
    m_UserHostListMutex.lock();
    auto iter = m_UserHostList.find( onlineId );
    if( iter != m_UserHostList.end() )
    {
        iter->second->deleteLater();
        m_UserHostList.erase( iter );
    }

    m_UserHostListMutex.unlock();
}

//============================================================================
bool GuiUserHostMgr::isUserHostInSession( VxGUID& onlineId )
{
    m_UserHostListMutex.lock();
    GuiUserHost* guiUserHost = findUserHost( onlineId );
    m_UserHostListMutex.unlock();
    return guiUserHost && guiUserHost->isInSession();
}

//============================================================================
GuiUserHost* GuiUserHostMgr::getUserHost( VxGUID& onlineId )
{
    m_UserHostListMutex.lock();
    GuiUserHost* guiUserHost = findUserHost( onlineId );
    m_UserHostListMutex.unlock();
    return guiUserHost;
}

//============================================================================
GuiUserHost* GuiUserHostMgr::updateUserHost( VxNetIdent* hisIdent, EHostType hostType )
{
    if( !hisIdent )
    {
        LogMsg( LOG_ERROR, "GuiUserHostMgr::updateUserHostOnline invalid param" );
        return nullptr;
    }

    m_UserHostListMutex.lock();
    GuiUserHost* guiUserHost = findUserHost( hisIdent->getMyOnlineId() );
    m_UserHostListMutex.unlock();
    if( guiUserHost && guiUserHost->getMyOnlineId() == hisIdent->getMyOnlineId() )
    {
        guiUserHost->addHostType( hostType );
        onUserHostUpdated( guiUserHost );
    }
    else
    {
        guiUserHost = new GuiUserHost( m_MyApp );
        guiUserHost->setNetIdent( hisIdent );
        guiUserHost->addHostType( hostType );
        m_UserHostListMutex.lock();
        m_UserHostList[guiUserHost->getMyOnlineId()] = guiUserHost;
        m_UserHostListMutex.unlock();
        onUserHostAdded( guiUserHost );
    }

    return guiUserHost;
}

//============================================================================
void GuiUserHostMgr::updateMyIdent( VxNetIdent* myIdent )
{
    if( !m_MyIdent )
    {
        GuiUserHost* guiUserHost = new GuiUserHost( m_MyApp );
        guiUserHost->setNetIdent( myIdent );
        m_UserHostListMutex.lock();
        m_MyIdent = guiUserHost;
        m_MyOnlineId = m_MyIdent->getMyOnlineId();
        m_UserHostListMutex.unlock();
    }

    emit signalMyIdentUpdated( m_MyIdent );
}

//============================================================================
void GuiUserHostMgr::setUserHostOffline( VxGUID& onlineId )
{
    m_UserHostListMutex.lock();
    GuiUserHost* guiUserHost = findUserHost( onlineId );
    m_UserHostListMutex.unlock();
    if( guiUserHost )
    {
        guiUserHost->setOnlineStatus( false );
    }
}

//============================================================================
void GuiUserHostMgr::onUserHostAdded( GuiUserHost* user )
{
    if( isMessengerReady() )
    {
        emit signalUserHostAdded( user );
    }
}

//============================================================================
void GuiUserHostMgr::onUserHostRemoved( VxGUID& onlineId )
{
    if( isMessengerReady() )
    {
        emit signalUserHostRemoved( onlineId );
    }
}

//============================================================================
void GuiUserHostMgr::onUserHostOnlineStatusChange( GuiUserHost* user, bool isOnline )
{
    if( isMessengerReady() )
    {
        emit signalUserHostOnlineStatus( user, isOnline );
    }
}

//============================================================================
void GuiUserHostMgr::onUserHostUpdated( GuiUserHost* user )
{
    if( isMessengerReady() )
    {
        emit signalUserHostUpdated( user );
    }
}

//============================================================================
void GuiUserHostMgr::onMyIdentUpdated( GuiUserHost* user )
{
    if( isMessengerReady() )
    {
        emit signalMyIdentUpdated( user );
    }
}