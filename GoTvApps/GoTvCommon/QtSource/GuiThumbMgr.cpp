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

#include "GuiThumbMgr.h"
#include "GuiThumb.h"
#include "AppCommon.h"

#include <GoTvCore/GoTvP2P/ThumbMgr/ThumbInfo.h>

//============================================================================
GuiThumbMgr::GuiThumbMgr( AppCommon& app )
    : QObject( &app )
    , m_MyApp( app )
{
}

//============================================================================
void GuiThumbMgr::onAppCommonCreated( void )
{
    connect( this, SIGNAL( signalInternalThumbAdded( ThumbInfo * ) ),	    this, SLOT( slotInternalThumbAdded( ThumbInfo * ) ), Qt::QueuedConnection );
    connect( this, SIGNAL( signalInternalThumbUpdated( ThumbInfo *) ),      this, SLOT( slotInternalThumbUpdated( ThumbInfo * ) ), Qt::QueuedConnection );
    connect( this, SIGNAL( signalInternalThumbRemoved(VxGUID) ),	        this, SLOT( slotInternalThumbRemoved(VxGUID) ), Qt::QueuedConnection );

    m_MyApp.getEngine().getThumbMgr().addThumbMgrClient( this, true );
}

//============================================================================
void GuiThumbMgr::callbackThumbAdded( ThumbInfo *  thumb )
{
    emit signalInternalThumbAdded( thumb );
}

//============================================================================
void GuiThumbMgr::callbackThumbUpdated( ThumbInfo * thumb )
{
    emit signalInternalThumbUpdated( thumb );
}

//============================================================================
void GuiThumbMgr::callbackThumbRemoved( VxGUID& thumbId )
{
    emit signalInternalThumbRemoved( thumbId );
}

//============================================================================
void GuiThumbMgr::slotInternalThumbAdded( ThumbInfo* thumbInfo )
{
    updateThumb( thumbInfo );
}

//============================================================================
void GuiThumbMgr::slotInternalThumbUpdated( ThumbInfo* thumbInfo )
{
    updateThumb( thumbInfo );
}

//============================================================================
void GuiThumbMgr::slotInternalThumbRemoved( VxGUID thumbId )
{
    m_ThumbListMutex.lock();
    GuiThumb* guiThumb = findThumb( thumbId );
    m_ThumbListMutex.unlock();
    if( guiThumb )
    {
        onThumbRemoved( thumbId );
    }

    removeThumb( thumbId );
}

//============================================================================
GuiThumb* GuiThumbMgr::findThumb( VxGUID& thumbId )
{
    return m_ThumbList.findThumb( thumbId );
}

//============================================================================
void GuiThumbMgr::removeThumb( VxGUID& thumbId )
{
    m_ThumbListMutex.lock();
    m_ThumbList.removeThumb( thumbId );
    m_ThumbListMutex.unlock();
}

//============================================================================
GuiThumb* GuiThumbMgr::getThumb( VxGUID& thumbId )
{
    m_ThumbListMutex.lock();
    GuiThumb* guiThumb = findThumb( thumbId );
    m_ThumbListMutex.unlock();
    return guiThumb;
}

//============================================================================
GuiThumb* GuiThumbMgr::updateThumb( ThumbInfo * thumbInfo )
{
    if( !thumbInfo )
    {
        LogMsg( LOG_ERROR, "GuiThumbMgr::updateThumbOnline invalid param" );
        return nullptr;
    }

    m_ThumbListMutex.lock();
    GuiThumb* guiThumb = findThumb( thumbInfo->getCreatorId() );
    m_ThumbListMutex.unlock();
    if( guiThumb )
    {
        onThumbUpdated( guiThumb );
    }
    else
    {
        guiThumb = new GuiThumb( m_MyApp );
        guiThumb->setThumbInfo( thumbInfo );
        m_ThumbListMutex.lock();
        m_ThumbList.addThumbIfDoesntExist( guiThumb );
        m_ThumbListMutex.unlock();
        onThumbAdded( guiThumb );
    }

    return guiThumb;
}

//============================================================================
void GuiThumbMgr::onThumbAdded( GuiThumb* guiThumb )
{
    if( isMessengerReady() )
    {
        emit signalThumbAdded( guiThumb );
    }
}

//============================================================================
void GuiThumbMgr::onThumbUpdated( GuiThumb* guiThumb )
{
    if( isMessengerReady() )
    {
        emit signalThumbUpdated( guiThumb );
    }
}

//============================================================================
void GuiThumbMgr::onThumbRemoved( VxGUID& onlineId )
{
    if( isMessengerReady() )
    {
        emit signalThumbRemoved( onlineId );
    }
}

//============================================================================
bool GuiThumbMgr::requestAvatarImage( GuiUser* user, EHostType hostType, QImage& retAvatarImage, bool requestFromUserIfValid )
{
    bool foundThumb = false;
    if( user && user->getNetIdent() )
    {
        VxNetIdent* netIdent = user->getNetIdent();
        VxGUID thumbId = netIdent->getThumbId( hostType ).isVxGUIDValid() ? netIdent->getThumbId( hostType ) : netIdent->getAvatarGuid();
        if( thumbId.isVxGUIDValid() )
        {
            GuiThumb* thumb = m_ThumbList.findThumb( thumbId );
            if( thumb )
            {
                return thumb->createImage( retAvatarImage );
            }
            else if( requestFromUserIfValid )
            {
                // TODO
            }
        }
    }
    else
    {
        LogMsg( LOG_ERROR, "requestAvatarImage null user" );
    }

    return foundThumb;
}