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

#include <ptop_src/ptop_engine_src/ThumbMgr/ThumbInfo.h>
#include <ptop_src/ptop_engine_src/ThumbMgr/ThumbMgr.h>

#include <CoreLib/VxFileUtil.h>

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
bool GuiThumbMgr::isMessengerReady( void )
{
    return m_MyApp.isMessengerReady();
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
    GuiThumb* guiThumb = findThumb( thumbInfo->getAssetUniqueId() );
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
    return requestAvatarImage( user, HostTypeToClientPlugin( hostType ), retAvatarImage, requestFromUserIfValid );
}

//============================================================================
bool GuiThumbMgr::requestAvatarImage( GuiUser* user, EPluginType pluginType, QImage& retAvatarImage, bool requestFromUserIfValid )
{
    bool foundThumb = false;
    if( user )
    {
        pluginType = HostPluginToClientPluginType( pluginType );
        VxNetIdent& netIdent = user->getNetIdent();
        bool hostImageValid = netIdent.getThumbId( pluginType ).isVxGUIDValid();
        VxGUID thumbId = hostImageValid ? netIdent.getThumbId( pluginType ) : netIdent.getAvatarThumbGuid();
        if( thumbId.isVxGUIDValid() )
        {
            GuiThumb* thumb = m_ThumbList.findThumb( thumbId );
            if( thumb )
            {
                return thumb->createImage( retAvatarImage );
            }
            else if( requestFromUserIfValid )
            {
                m_MyApp.getEngine().getThumbMgr().requestPluginThumb( &user->getNetIdent(), hostImageValid ? pluginType : ePluginTypeClientPeerUser, thumbId );
            }
        }
    }
    else
    {
        LogMsg( LOG_ERROR, "requestAvatarImage null user" );
    }

    return foundThumb;
}

//============================================================================
bool GuiThumbMgr::getAvatarImage( VxGUID& thumbId, QImage& image )
{
    bool result = getThumbImage( thumbId, image );
    if( result && !image.isNull() )
    {
        result = makeCircleImage( image );
    }

    return result;
}

//============================================================================
bool GuiThumbMgr::getThumbImage( VxGUID& thumbId, QImage& image )
{
    bool result = false;
    if( thumbId.isVxGUIDValid() )
    {
        GuiThumb* thumb = m_ThumbList.findThumb( thumbId );
        if( thumb )
        {
            result = thumb->createImage( image );
        }
        else
        {
            // try the raw file name
            std::string thumbFile = m_MyApp.getEngine().getThumbMgr().fromGuiGetThumbFile( thumbId );
            if( !thumbFile.empty() && VxFileUtil::fileExists( thumbFile.c_str() ) )
            {
                result = image.load( thumbFile.c_str() ) && !image.isNull();
            }
        }
    }

    return result;
}

//============================================================================
bool GuiThumbMgr::makeCircleImage( QImage& image )
{
    QPixmap target( image.width(), image.height() );
    target.fill( Qt::transparent );

    QPainter painter( &target );

    // Set clipped region (circle) in the center of the target image
    QRegion clipRegion( QRect( 0, 0, image.width(), image.height() ), QRegion::Ellipse );
    painter.setClipRegion( clipRegion );

    painter.drawImage( 0, 0, image );
    image = target.toImage();
    return !image.isNull();
}