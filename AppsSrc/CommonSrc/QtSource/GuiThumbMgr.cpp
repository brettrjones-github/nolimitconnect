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
// http://www.nolimitconnect.org
//============================================================================

#include "GuiThumbMgr.h"
#include "GuiHelpers.h"
#include "AppCommon.h"

#include <ptop_src/ptop_engine_src/ThumbMgr/ThumbInfo.h>
#include <ptop_src/ptop_engine_src/ThumbMgr/ThumbMgr.h>

#include <CoreLib/VxFileUtil.h>
#include <algorithm>

//============================================================================
GuiThumbMgr::GuiThumbMgr( AppCommon& app )
    : QObject( &app )
    , m_MyApp( app )
{
}

//============================================================================
void GuiThumbMgr::onAppCommonCreated( void )
{
    connect( this, SIGNAL( signalInternalThumbAdded( ThumbInfo ) ),	    this, SLOT( slotInternalThumbAdded( ThumbInfo ) ), Qt::QueuedConnection );
    connect( this, SIGNAL( signalInternalThumbUpdated( ThumbInfo ) ),      this, SLOT( slotInternalThumbUpdated( ThumbInfo ) ), Qt::QueuedConnection );
    connect( this, SIGNAL( signalInternalThumbRemoved(VxGUID) ),	        this, SLOT( slotInternalThumbRemoved(VxGUID) ), Qt::QueuedConnection );

    m_MyApp.getEngine().getThumbMgr().addThumbMgrClient( this, true );
    std::vector<VxGUID>& emoticonList = m_MyApp.getEngine().getThumbMgr().getEmoticonIdList();
    // LogMsg( LOG_VERBOSE, "GuiThumbMgr::onAppCommonCreated emoticon list size %d", emoticonList.size() );
    int emoticonNum = 0;
    for( auto& guid : emoticonList )
    {
        
        if( guid.isVxGUIDValid() )
        {
            // LogMsg( LOG_VERBOSE, "GuiThumbMgr::onAppCommonCreated emoticon  %d is valid %s", emoticonNum, guid.toOnlineIdString().c_str() );
            m_EmoticonList.push_back( guid );
        }
        else
        {
            LogMsg( LOG_VERBOSE, "GuiThumbMgr::onAppCommonCreated emoticon  %d invalid %s", emoticonNum, guid.toOnlineIdString().c_str() );
        }

        emoticonNum++;
    }
}

//============================================================================
bool GuiThumbMgr::isMessengerReady( void )
{
    return m_MyApp.isMessengerReady();
}

//============================================================================
void GuiThumbMgr::callbackThumbAdded( ThumbInfo * thumb )
{
    if( thumb && thumb->isValidThumbnail() )
    {
        emit signalInternalThumbAdded( *thumb );
    }
    else
    {
        LogMsg( LOG_ERROR, "GuiThumbMgr::callbackThumbAdded invalid thumb" );
    }
}

//============================================================================
void GuiThumbMgr::callbackThumbUpdated( ThumbInfo * thumb )
{
    if( thumb && thumb->isValidThumbnail() )
    {
        emit signalInternalThumbUpdated( *thumb );
    }
    else
    {
        LogMsg( LOG_ERROR, "GuiThumbMgr::callbackThumbUpdated invalid thumb" );
    }
}

//============================================================================
void GuiThumbMgr::callbackThumbRemoved( VxGUID& thumbId )
{
    emit signalInternalThumbRemoved( thumbId );
}

//============================================================================
void GuiThumbMgr::slotInternalThumbAdded( ThumbInfo thumbInfo )
{
    updateThumb( thumbInfo );
}

//============================================================================
void GuiThumbMgr::slotInternalThumbUpdated( ThumbInfo thumbInfo )
{
    updateThumb( thumbInfo );
}

//============================================================================
void GuiThumbMgr::slotInternalThumbRemoved( VxGUID thumbId )
{
    GuiThumb* guiThumb = findThumb( thumbId );
    if( guiThumb )
    {
        onThumbRemoved( thumbId );
    }

    removeThumb( thumbId );
}

//============================================================================
GuiThumb* GuiThumbMgr::findThumb( VxGUID& thumbId )
{
    GuiThumb* guiThumb = m_ThumbList.findThumb( thumbId );
    if( !guiThumb && std::find( m_EmoticonList.begin(), m_EmoticonList.end(), thumbId ) != m_EmoticonList.end() )
    {
        guiThumb = generateEmoticon( thumbId, false );
    }

    return guiThumb;
}

//============================================================================
GuiThumb* GuiThumbMgr::findOrCreateEmoticonThumb( VxGUID& thumbId )
{
    GuiThumb* guiThumb = m_ThumbList.findThumb( thumbId );
    if( !guiThumb && std::find( m_EmoticonList.begin(), m_EmoticonList.end(), thumbId ) != m_EmoticonList.end() )
    {
        guiThumb = generateEmoticon( thumbId, false );
    }

    return guiThumb;
}

//============================================================================
void GuiThumbMgr::removeThumb( VxGUID& thumbId )
{
    m_ThumbList.removeThumb( thumbId );
}

//============================================================================
GuiThumb* GuiThumbMgr::getThumb( VxGUID& thumbId )
{
    GuiThumb* guiThumb = findThumb( thumbId );
    return guiThumb;
}

//============================================================================
GuiThumb* GuiThumbMgr::updateThumb( ThumbInfo& thumbInfo )
{
    if( !thumbInfo.isValidThumbnail() )
    {
        LogMsg( LOG_ERROR, "GuiThumbMgr::updateThumbOnline invalid param" );
        return nullptr;
    }

    GuiThumb* guiThumb = m_ThumbList.findThumb( thumbInfo.getAssetUniqueId() ); 
    if( guiThumb )
    {
        onThumbUpdated( guiThumb );
    }
    else
    {
        guiThumb = new GuiThumb( m_MyApp );
        guiThumb->setThumbInfo( thumbInfo );
        m_ThumbList.addThumbIfDoesntExist( guiThumb );
        m_MyApp.getEngine().getThumbMgr().fromGuiThumbCreated( thumbInfo );
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
                ThumbMgr& thumbMgr = m_MyApp.getEngine().getThumbMgr();
                if( thumbMgr.isEmoticonThumbnail( thumbId ) )
                {
                    GuiThumb* thumb = generateEmoticon( thumbId );
                    if( thumb )
                    {
                        return thumb->createImage( retAvatarImage );
                    }
                }
                else
                {
                    GuiThumb* newThumb{ nullptr };

                    thumbMgr.lockResources();
                    AssetBaseInfo* assetInfo = thumbMgr.findAsset( thumbId );
                    ThumbInfo* thumbInfo = dynamic_cast< ThumbInfo* >( assetInfo );
                    if( thumbInfo && thumbInfo->isValidThumbnail() )
                    {
                        newThumb = new GuiThumb( m_MyApp );
                        newThumb->setThumbInfo( *thumbInfo );
                    }

                    thumbMgr.unlockResources();

                    if( newThumb )
                    {
                        m_ThumbList.addThumbIfDoesntExist( newThumb );
                        onThumbAdded( newThumb );
                        return newThumb->createImage( retAvatarImage );
                    }
                    else
                    {
                        thumbMgr.fromGuiRequestPluginThumb( &user->getNetIdent(), hostImageValid ? pluginType : ePluginTypeHostPeerUser, thumbId );
                    }
                }
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
        result = GuiHelpers::makeCircleImage( image );
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
        if( !thumb )
        {
            if( m_MyApp.getEngine().getThumbMgr().isEmoticonThumbnail( thumbId ) )
            {
                thumb = generateEmoticon( thumbId );
            }
        }
        
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
GuiThumb* GuiThumbMgr::generateEmoticon( VxGUID& thumbId, bool checkIfExists )
{
    if( !thumbId.isVxGUIDValid() )
    {
        QString msgText = QObject::tr( "Invalid emoticon id" );
        QMessageBox::warning( &m_MyApp.getHomePage(), msgText, msgText, QMessageBox::Ok );
        return nullptr;
    }

    GuiThumb* thumb = nullptr;
    if( checkIfExists )
    {
        // see if already have it
        thumb = findThumb( thumbId );
        if( thumb )
        {
            return thumb;
        }
    }

    // see if engine thumb manager has it
    ThumbMgr& thumbMgr = m_MyApp.getEngine().getThumbMgr();
    thumbMgr.lockResources();
    ThumbInfo* existingThumbInfo = dynamic_cast< ThumbInfo* >( thumbMgr.findAsset( thumbId ) );
    if( existingThumbInfo && existingThumbInfo->isValidThumbnail() )
    {
        ThumbInfo copyThumbInfo = *existingThumbInfo;
        thumbMgr.unlockResources();
        thumb = updateThumb( copyThumbInfo );
        if( thumb )
        {
            return thumb;
        }
    }
    else
    {
        thumbMgr.unlockResources();
    }

    // see if file exists and we just need to create the info
    QString fileName;
    if( !GuiHelpers::createThumbFileName( thumbId, fileName ) )
    {
        QString msgText = QObject::tr( "Could create emoticon file name" );
        QMessageBox::warning( &m_MyApp.getHomePage(), QObject::tr( "Error occured creating emoticon file " ), msgText, QMessageBox::Ok );
        return nullptr;
    }

    uint64_t fileLen = VxFileUtil::fileExists( fileName.toUtf8().constData() );
    if( fileLen )
    {
        // file exists so create a thumbnail instance
        ThumbInfo thumbInfo( fileName.toUtf8().constData(), fileLen, thumbId );
        return updateThumb( thumbInfo );
    }

    // create and new emoticon thumbnail
    const int emoteMargin = 20;

    // static member so no need to make copy
    std::vector<VxGUID>& emoticonIdList = thumbMgr.getEmoticonIdList();
    // 0 based emoticon number but first id is always null
    int emoticonNum = 1;
    bool foundId = false;
    for( auto& assetGuid : emoticonIdList )
    {
        if( assetGuid == thumbId )
        {
            foundId = true;
            break;
        }

        emoticonNum++;
    }

    if( !foundId || emoticonNum > (int)emoticonIdList.size() )
    {
        QString msgText = QObject::tr( "Invalid emoticon id" );
        QMessageBox::warning( &m_MyApp.getHomePage(), QObject::tr( "Invalid emoticon id " ), msgText + thumbId.toOnlineIdString().c_str(), QMessageBox::Ok );
    }

    QPixmap image;
    QSize imageSize( GuiParams::getThumbnailSize().width() - emoteMargin * 2, GuiParams::getThumbnailSize().height() - emoteMargin * 2 );
    if( m_MyApp.getThumbMgr().getEmoticonImage( emoticonNum, imageSize, image ) )
    {
        QPixmap finalThumbnail( GuiParams::getThumbnailSize() );
        finalThumbnail.fill( QColor( COLOR_TRANSPARENT ) );
        QPainter painter( &finalThumbnail );
        painter.setRenderHint( QPainter::Antialiasing, true );
        painter.setRenderHint( QPainter::TextAntialiasing, true );
        painter.setRenderHint( QPainter::SmoothPixmapTransform, true );

        painter.drawPixmap( emoteMargin, emoteMargin, image.width(), image.height(), image );

        uint64_t fileLen = GuiHelpers::saveToPngFile( finalThumbnail, fileName );
        if( fileLen )
        {
            ThumbInfo assetInfo( ( const char* )fileName.toUtf8().constData(), fileLen, thumbId );
            assetInfo.setCreatorId( m_MyApp.getEngine().getMyOnlineId() );
            assetInfo.setCreationTime( GetTimeStampMs() );
            assetInfo.setModifiedTime( assetInfo.getCreationTime() );
            thumb = updateThumb( assetInfo );

            if( !thumbMgr.fromGuiThumbCreated( assetInfo ) )
            {
                QString msgText = QObject::tr( "Could not create emoticon asset" );
                QMessageBox::warning( &m_MyApp.getHomePage(), QObject::tr( "Error occured creating emoticon asset " ) + fileName, msgText, QMessageBox::Ok );
            }
        }
        else
        {
            QString msgText = QObject::tr( "Could not create emoticon png file" );
            QMessageBox::warning( &m_MyApp.getHomePage(), QObject::tr( "Error occured creating emoticon file " ) + fileName, msgText, QMessageBox::Ok );
        }
    }
    else
    {
        QString msgText = QObject::tr( "Could not create emoticon image" );
        QMessageBox::warning( &m_MyApp.getHomePage(), QObject::tr( "Error occured creating emoticon image %1" ).arg( emoticonNum ), msgText, QMessageBox::Ok );
    }

    return thumb;
}

//============================================================================
void GuiThumbMgr::generateEmoticonsIfNeeded( AppletBase * applet )
{
    ThumbMgr& thumbMgr = m_MyApp.getEngine().getThumbMgr();
    std::vector<VxGUID>& emoticonIdList = thumbMgr.getEmoticonIdList();
    int emoticonNum = 1;
    for( auto& assetGuid : emoticonIdList )
    {
        if( assetGuid.isVxGUIDValid() )
        {
            GuiThumb *thumb = findOrCreateEmoticonThumb( assetGuid );
            if( !thumb )
            {
                QString msgText = QObject::tr( "Could create emoticon image" );
                QMessageBox::warning( ( QWidget* )applet, QObject::tr( "Error occured creating emoticon image %1" ).arg( emoticonNum ), msgText, QMessageBox::Ok );
            }
        }

        emoticonNum++;
    }
}

//============================================================================
bool GuiThumbMgr::getEmoticonImage( int emoticonNum, QSize imageSize, QPixmap& retImage )
{
    char resBuf[128];
    if( emoticonNum > 0 && emoticonNum <= 40)
    {
        if( emoticonNum > 9 )
        {
            sprintf( resBuf, ":/AppRes/Resources/emoj%d.svg", emoticonNum );
        }
        else
        {
            sprintf( resBuf, ":/AppRes/Resources/emoj0%d.svg", emoticonNum );
        }

        QPixmap faceImage( resBuf );
        retImage = faceImage.scaled( imageSize, Qt::KeepAspectRatio, Qt::SmoothTransformation );
        return !retImage.isNull();
    }
    else
    {
        return false;
    }   
}
