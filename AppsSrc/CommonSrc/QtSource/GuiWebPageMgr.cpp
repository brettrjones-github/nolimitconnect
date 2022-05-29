
//============================================================================
// Copyright (C) 2022 Brett R. Jones
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

#include <QWidget> // must be declared first or linux Qt 6.2.4 will error in qmetatype.h 2167:23: array subscript value ‘53’ is outside the bounds

#include "GuiWebPageMgr.h"
#include "GuiWebPageCallback.h"

#include "AppCommon.h"

#include <ptop_src/ptop_engine_src/WebPageMgr/WebPageMgr.h>

//============================================================================
GuiWebPageMgr::GuiWebPageMgr( AppCommon& app )
    : QObject( &app )
    , m_MyApp( app )
{
}

//============================================================================
void GuiWebPageMgr::onAppCommonCreated( void )
{
    connect( this, SIGNAL( signalIInteralWebDownloadStarted( EWebPageType, VxGUID, QString, int ) ), this, SLOT( slotInternalWebDownloadStarted( EWebPageType, VxGUID, QString, int ) ), Qt::QueuedConnection );
    connect( this, SIGNAL( signalInternalWebDownloadProgress( EWebPageType, VxGUID, int, int ) ), this, SLOT( slotInternalWebDownloadProgress( EWebPageType, VxGUID, int, int ) ), Qt::QueuedConnection );
    connect( this, SIGNAL( signalInternalWebDownloadComplete( EWebPageType, VxGUID, QString ) ), this, SLOT( slotInternalWebDownloadComplete( EWebPageType, VxGUID, QString ) ), Qt::QueuedConnection );
    connect( this, SIGNAL( signalInternalWebDownloadFailed( EWebPageType, VxGUID, QString, EXferError ) ), this, SLOT( slotInternalWebDownloadFailed( EWebPageType, VxGUID, QString, EXferError ) ), Qt::QueuedConnection );

    m_MyApp.getEngine().getWebPageMgr().wantWebPageMgrCallbacks( dynamic_cast< WebPageCallbackInterface* >( this ), true );
}

//============================================================================
void GuiWebPageMgr::onMessengerReady( bool ready )
{
}

//============================================================================
void GuiWebPageMgr::onSystemReady( bool ready )
{
}

//============================================================================
void GuiWebPageMgr::callbackWebDownloadStarted( EWebPageType webPageType, VxGUID& onlineId, std::string& fileName, int fileNum )
{
    QString fileNameOut( fileName.c_str() );
    emit signalIInteralWebDownloadStarted( webPageType, onlineId, fileNameOut, fileNum );
}

//============================================================================
void GuiWebPageMgr::callbackWebDownloadProgress( EWebPageType webPageType, VxGUID& onlineId, int fileNum, int progress )
{
    emit signalInternalWebDownloadProgress( webPageType, onlineId, fileNum, progress );
}

//============================================================================
void GuiWebPageMgr::callbackWebDownloadComplete( EWebPageType webPageType, VxGUID& onlineId, std::string& fileName )
{
    QString fileNameOut( fileName.c_str() );
    emit signalInternalWebDownloadComplete( webPageType, onlineId, fileNameOut );
}

//============================================================================
void GuiWebPageMgr::callbackWebDownloadFailed( EWebPageType webPageType, VxGUID& onlineId, std::string& fileName, enum EXferError xferErr )
{
    QString fileNameOut( fileName.c_str() );
    emit signalInternalWebDownloadFailed( webPageType, onlineId, fileNameOut, xferErr );
}

//============================================================================
void GuiWebPageMgr::slotInternalWebDownloadStarted( EWebPageType webPageType, VxGUID onlineId, QString fileName, int fileNum )
{
    announceWebDownloadStarted( webPageType, onlineId, fileName, fileNum );
}

//============================================================================
void GuiWebPageMgr::slotInternalWebDownloadProgress( EWebPageType webPageType, VxGUID onlineId, int fileNum, int progress )
{
    announceWebDownloadProgress( webPageType, onlineId, fileNum, progress );
}

//============================================================================
void GuiWebPageMgr::slotInternalWebDownloadComplete( EWebPageType webPageType, VxGUID onlineId, QString fileName )
{
    announceWebDownloadComplete( webPageType, onlineId, fileName );
}

//============================================================================
void GuiWebPageMgr::slotInternalWebDownloadFailed( EWebPageType webPageType, VxGUID onlineId, QString fileName, EXferError xferErr )
{
    announceWebDownloadFailed( webPageType, onlineId, fileName, xferErr );
}

//============================================================================
void GuiWebPageMgr::announceWebDownloadStarted( EWebPageType webPageType, VxGUID& onlineId, QString& fileName, int fileNum )
{
    for( auto guiWebPageCallback : m_WebPageClients )
    {
        guiWebPageCallback->callbackWebDownloadStarted( webPageType, onlineId, fileName, fileNum );
    }
}

//============================================================================
void GuiWebPageMgr::announceWebDownloadProgress( EWebPageType webPageType, VxGUID& onlineId, int fileNum, int progress )
{
    for( auto guiWebPageCallback : m_WebPageClients )
    {
        guiWebPageCallback->callbackWebDownloadProgress( webPageType, onlineId, fileNum, progress );
    }
}

//============================================================================
void GuiWebPageMgr::announceWebDownloadComplete( EWebPageType webPageType, VxGUID& onlineId, QString& fileName )
{
    for( auto guiWebPageCallback : m_WebPageClients )
    {
        guiWebPageCallback->callbackWebDownloadComplete( webPageType, onlineId, fileName );
    }
}

//============================================================================
void GuiWebPageMgr::announceWebDownloadFailed( EWebPageType webPageType, VxGUID& onlineId, QString& fileName, enum EXferError xferErr )
{
    for( auto guiWebPageCallback : m_WebPageClients )
    {
        guiWebPageCallback->callbackWebDownloadFailed( webPageType, onlineId, fileName, xferErr );
    }
}

//============================================================================
void GuiWebPageMgr::wantWebPageCallbacks( GuiWebPageCallback* client, bool enable )
{
    for( auto iter = m_WebPageClients.begin(); iter != m_WebPageClients.end(); ++iter )
    {
        if( *iter == client )
        {
            m_WebPageClients.erase( iter );
            break;
        }
    }

    if( enable )
    {
        m_WebPageClients.push_back( client );
    }
}
