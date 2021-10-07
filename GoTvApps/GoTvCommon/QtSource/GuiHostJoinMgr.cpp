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

#include "GuiHostJoinMgr.h"
#include "AppCommon.h"

//============================================================================
GuiHostJoinMgr::GuiHostJoinMgr( AppCommon& app )
    : QObject( &app )
    , m_MyApp( app )
{
}

//============================================================================
void GuiHostJoinMgr::onAppCommonCreated( void )
{
    connect( this, SIGNAL( signalInternalUpdateHostJoin(VxNetIdent*,EHostType) ),	                this, SLOT( slotInternalUpdateHostJoin(VxNetIdent*,EHostType) ), Qt::QueuedConnection );
    connect( this, SIGNAL( signalInternalHostJoinRemoved(VxGUID) ),	                                this, SLOT( slotInternalHostJoinRemoved(VxGUID) ), Qt::QueuedConnection );
    connect( this, SIGNAL( signalInternalHostJoinOnlineStatus( VxNetIdent*, EHostType, bool ) ),    this, SLOT( slotInternalHostJoinOnlineStatus( VxNetIdent*, EHostType, bool ) ), Qt::QueuedConnection );
}

//============================================================================
void GuiHostJoinMgr::slotInternalUpdateHostJoin( VxNetIdent* netIdent, EHostType hostType )
{
    updateHostJoin( netIdent, hostType );
}

//============================================================================
void GuiHostJoinMgr::slotInternalUpdateMyIdent( VxNetIdent* netIdent )
{
    updateMyIdent( netIdent );
}

//============================================================================
void GuiHostJoinMgr::slotInternalHostJoinRemoved( VxGUID onlineId )
{
    m_HostJoinListMutex.lock();
    GuiHostJoin* guiHostJoin = findHostJoin( onlineId );
    m_HostJoinListMutex.unlock();
    if( guiHostJoin )
    {
        onHostJoinRemoved( onlineId );
    }

    removeHostJoin( onlineId );
}

//============================================================================
void GuiHostJoinMgr::slotInternalHostJoinOnlineStatus( VxNetIdent* netIdent, EHostType hostType, bool online )
{
    GuiHostJoin* guiHostJoin = updateHostJoin( netIdent, hostType );
    if( guiHostJoin )
    {
        guiHostJoin->setOnlineStatus( online );
    }
}

//============================================================================
GuiHostJoin* GuiHostJoinMgr::findHostJoin( VxGUID& onlineId )
{
    GuiHostJoin* user = nullptr;
    auto iter = m_HostJoinList.find( onlineId );
    if( iter != m_HostJoinList.end() )
    {
        user = iter->second;
    }

    return user;
}

//============================================================================
void GuiHostJoinMgr::removeHostJoin( VxGUID& onlineId )
{
    m_HostJoinListMutex.lock();
    auto iter = m_HostJoinList.find( onlineId );
    if( iter != m_HostJoinList.end() )
    {
        iter->second->deleteLater();
        m_HostJoinList.erase( iter );
    }

    m_HostJoinListMutex.unlock();
}

//============================================================================
bool GuiHostJoinMgr::isHostJoinInSession( VxGUID& onlineId )
{
    m_HostJoinListMutex.lock();
    GuiHostJoin* guiHostJoin = findHostJoin( onlineId );
    m_HostJoinListMutex.unlock();
    return guiHostJoin && guiHostJoin->isInSession();
}

//============================================================================
GuiHostJoin* GuiHostJoinMgr::getHostJoin( VxGUID& onlineId )
{
    m_HostJoinListMutex.lock();
    GuiHostJoin* guiHostJoin = findHostJoin( onlineId );
    m_HostJoinListMutex.unlock();
    return guiHostJoin;
}

//============================================================================
GuiHostJoin* GuiHostJoinMgr::updateHostJoin( VxNetIdent* hisIdent, EHostType hostType )
{
    if( !hisIdent )
    {
        LogMsg( LOG_ERROR, "GuiHostJoinMgr::updateHostJoinOnline invalid param" );
        return nullptr;
    }

    m_HostJoinListMutex.lock();
    GuiHostJoin* guiHostJoin = findHostJoin( hisIdent->getMyOnlineId() );
    m_HostJoinListMutex.unlock();
    if( guiHostJoin && guiHostJoin->getMyOnlineId() == hisIdent->getMyOnlineId() )
    {
        guiHostJoin->addHostType( hostType );
        onHostJoinUpdated( guiHostJoin );
    }
    else
    {
        guiHostJoin = new GuiHostJoin( m_MyApp );
        guiHostJoin->setNetIdent( hisIdent );
        guiHostJoin->addHostType( hostType );
        m_HostJoinListMutex.lock();
        m_HostJoinList[guiHostJoin->getMyOnlineId()] = guiHostJoin;
        m_HostJoinListMutex.unlock();
        onHostJoinAdded( guiHostJoin );
    }

    return guiHostJoin;
}

//============================================================================
void GuiHostJoinMgr::updateMyIdent( VxNetIdent* myIdent )
{
    if( !m_MyIdent )
    {
        GuiHostJoin* guiHostJoin = new GuiHostJoin( m_MyApp );
        guiHostJoin->setNetIdent( myIdent );
        m_HostJoinListMutex.lock();
        m_MyIdent = guiHostJoin;
        m_MyOnlineId = m_MyIdent->getMyOnlineId();
        m_HostJoinListMutex.unlock();
    }

    emit signalMyIdentUpdated( m_MyIdent );
}

//============================================================================
void GuiHostJoinMgr::setHostJoinOffline( VxGUID& onlineId )
{
    m_HostJoinListMutex.lock();
    GuiHostJoin* guiHostJoin = findHostJoin( onlineId );
    m_HostJoinListMutex.unlock();
    if( guiHostJoin )
    {
        guiHostJoin->setOnlineStatus( false );
    }
}

//============================================================================
void GuiHostJoinMgr::onHostJoinAdded( GuiHostJoin* user )
{
    if( isMessengerReady() )
    {
        emit signalHostJoinAdded( user );
    }
}

//============================================================================
void GuiHostJoinMgr::onHostJoinRemoved( VxGUID& onlineId )
{
    if( isMessengerReady() )
    {
        emit signalHostJoinRemoved( onlineId );
    }
}

//============================================================================
void GuiHostJoinMgr::onHostJoinOnlineStatusChange( GuiHostJoin* user, bool isOnline )
{
    if( isMessengerReady() )
    {
        emit signalHostJoinOnlineStatus( user, isOnline );
    }
}

//============================================================================
void GuiHostJoinMgr::onHostJoinUpdated( GuiHostJoin* user )
{
    if( isMessengerReady() )
    {
        emit signalHostJoinUpdated( user );
    }
}

//============================================================================
void GuiHostJoinMgr::onMyIdentUpdated( GuiHostJoin* user )
{
    if( isMessengerReady() )
    {
        emit signalMyIdentUpdated( user );
    }
}