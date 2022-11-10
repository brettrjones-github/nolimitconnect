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

#include "PluginBaseFilesClient.h"

#include <ptop_src/ptop_engine_src/P2PEngine/P2PEngine.h>
#include <ptop_src/ptop_engine_src/BigListLib/BigListInfo.h>
#include <ptop_src/ptop_engine_src/ConnectIdListMgr/ConnectIdListMgr.h>

#include <PktLib/PktsFileInfo.h>

//============================================================================
PluginBaseFilesClient::PluginBaseFilesClient( P2PEngine& engine, PluginMgr& pluginMgr, VxNetIdent* myIdent, EPluginType pluginType, std::string fileInfoDbName )
: PluginBaseFiles( engine, pluginMgr, myIdent, pluginType, m_FileInfoClientFilesMgr )
, m_FileInfoClientFilesMgr( engine, *this, fileInfoDbName )
{
}

//============================================================================
bool PluginBaseFilesClient::connectForWebPageDownload( VxGUID& onlineId )
{
    bool result{ false };
    m_HisIdent = nullptr;
    m_Engine.getToGui().toGuiPluginMsg( getPluginType(), onlineId, ePluginMsgConnecting, "" );

    VxNetIdent* netIdent = m_Engine.getBigListMgr().findNetIdent( onlineId );	// id of friend to look for
    if( netIdent )
    {
        VxSktBase* sktBase = m_Engine.getConnectIdListMgr().findBestOnlineConnection( onlineId );
        if( sktBase )
        {
            result = onConnectForWebPageDownload( sktBase, netIdent );
        }
    }
    else
    {
        LogMsg( LOG_ERROR, "PluginBaseFilesClient::connectForWebPageDownload online id not found" );
    }

    if( result )
    {
        m_Engine.getToGui().toGuiPluginMsg( getPluginType(), onlineId, ePluginMsgRetrieveInfo, "" );
    }
    else
    {
        m_Engine.getToGui().toGuiPluginMsg( getPluginType(), onlineId, ePluginMsgConnectFailed, "" );
    }

    return result;
}

//============================================================================
bool PluginBaseFilesClient::onConnectForWebPageDownload( VxSktBase* sktBase, VxNetIdent* netIdent )
{
    m_HisIdent = netIdent;
    m_HisOnlineId = netIdent->getMyOnlineId();
    m_SearchSessionId.initializeWithNewVxGUID();

    PktFileInfoSearchReq pktReq;
    pktReq.setHostOnlineId( m_HisOnlineId );
    pktReq.setSearchSessionId( m_SearchSessionId );
    m_SktConnectionId = sktBase->getSocketId();

    return txPacket( netIdent, sktBase, &pktReq );
}

//============================================================================
bool PluginBaseFilesClient::connectForFileListDownload( VxGUID& onlineId )
{
    bool result{ false };
    m_HisIdent = nullptr;
    m_Engine.getToGui().toGuiPluginMsg( getPluginType(), onlineId, ePluginMsgConnecting, "" );

    VxNetIdent* netIdent = m_Engine.getBigListMgr().findNetIdent( onlineId );	// id of friend to look for
    if( netIdent )
    {
        VxSktBase* sktBase = m_Engine.getConnectIdListMgr().findBestOnlineConnection( onlineId );
        if( sktBase )
        {
            result = onConnectForFileListDownload( sktBase, netIdent );
        }
    }
    else
    {
        LogMsg( LOG_ERROR, "PluginBaseFilesClient::connectForWebPageDownload online id not found" );
    }

    if( result )
    {
        m_Engine.getToGui().toGuiPluginMsg( getPluginType(), onlineId, ePluginMsgRetrieveInfo, "" );
    }
    else
    {
        m_Engine.getToGui().toGuiPluginMsg( getPluginType(), onlineId, ePluginMsgConnectFailed, "" );
    }

    return result;
}

//============================================================================
bool PluginBaseFilesClient::onConnectForFileListDownload( VxSktBase* sktBase, VxNetIdent* netIdent )
{
    m_HisIdent = netIdent;
    m_HisOnlineId = netIdent->getMyOnlineId();
    m_SktConnectionId = sktBase->getSocketId();
    if( !m_SearchSessionId.isVxGUIDValid() )
    {
        m_SearchSessionId.initializeWithNewVxGUID();
    }
    
    PktFileInfoSearchReq pktReq;
    pktReq.setHostOnlineId( m_HisOnlineId );
    pktReq.setSearchSessionId( m_SearchSessionId );
    pktReq.setSearchFileTypes( getSearchFileTypes() );

    return txPacket( netIdent, sktBase, &pktReq );
}

