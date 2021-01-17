//============================================================================
// Copyright (C) 2020 Brett R. Jones
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

#include "HostBaseMgr.h"
#include "PluginBase.h"
#include "PluginMgr.h"

#include <GoTvCore/GoTvP2P/P2PEngine/P2PEngine.h>
#include <GoTvCore/GoTvP2P/Plugins/PluginBase.h>

#include <NetLib/VxSktBase.h>
#include <PktLib/PktsHostJoin.h>

#include <memory.h>

#ifdef _MSC_VER
# pragma warning(disable: 4355) //'this' : used in base member initializer list
#endif //_MSC_VER

//============================================================================
HostBaseMgr::HostBaseMgr( P2PEngine& engine, PluginMgr& pluginMgr, VxNetIdent * myIdent, PluginBase& pluginBase )
: m_Engine( engine )
, m_PluginMgr( pluginMgr )
, m_MyIdent( myIdent )
, m_Plugin( pluginBase )
, m_ConnectionMgr(engine.getConnectionMgr())
{
}

//============================================================================
void HostBaseMgr::fromGuiJoinHost( EHostType hostType, const char * ptopUrl )
{
    if( !ptopUrl && hostType == eHostTypeUnknown )
    {
        m_Engine.getToGui().toGuiHostJoinStatus( hostType, eHostJoinInvalidUrl );
        return;
    }

    EConnectReason connectReason = eConnectReasonChatRoomJoin;
    std::string url = ptopUrl ? ptopUrl : m_ConnectionMgr.getDefaultHostUrl( hostType );
    if( !url.empty() )
    {
        VxGUID hostGuid;
        EHostJoinStatus joinStatus = m_ConnectionMgr.lookupOrQueryId( url.c_str(), hostGuid, this, connectReason);
        if( eHostJoinQueryIdSuccess == joinStatus )
        {
            // no need for id query.. just request connection
            onUrlActionQueryIdSuccess( url, hostGuid, connectReason );
        }
        else if( eHostJoinQueryIdInProgress == joinStatus )
        {
            // manager is attempting to query id
            m_Engine.getToGui().toGuiHostJoinStatus( hostType, eHostJoinQueryIdInProgress );
        }
        else
        {
            m_Engine.getToGui().toGuiHostJoinStatus( hostType, eHostJoinInvalidUrl );
        }
    }
    else
    {
        m_Engine.getToGui().toGuiHostJoinStatus( hostType, eHostJoinInvalidUrl );
    }
}

//============================================================================
void HostBaseMgr::onUrlActionQueryIdSuccess( std::string& url, VxGUID& onlineId, EConnectReason connectReason )
{
    if( eConnectReasonChatRoomJoin == connectReason )
    {
        m_Engine.getToGui().toGuiHostJoinStatus( eHostTypeChatRoom, eHostJoinConnecting );
        VxSktBase* sktBase = nullptr;
        EConnectStatus connectStatus = m_ConnectionMgr.requestConnection( url, onlineId, this, sktBase, eConnectReasonChatRoomJoin );
        if( eConnectStatusReady == connectStatus )
        {
            if( sktBase )
            {
                m_Engine.getToGui().toGuiHostJoinStatus( eHostTypeChatRoom, eHostJoinConnectSuccess );
                onContactConnected( sktBase, onlineId, eConnectReasonChatRoomJoin );
            }
            else
            {
                LogMsg( LOG_ERROR, "requestConnection sucess but has null socket" );
            }
        }
    }
}

//============================================================================
void HostBaseMgr::onUrlActionQueryIdFail( std::string& url, ERunTestStatus testStatus, EConnectReason connectReason )
{
    if( eConnectReasonChatRoomJoin == connectReason )
    {
        m_Engine.getToGui().toGuiHostJoinStatus( eHostTypeChatRoom, eHostJoinQueryIdFailed, DescribeRunTestStatus( testStatus ) );
    }
}

//============================================================================
bool HostBaseMgr::onContactConnected( VxSktBase* sktBase, VxGUID& onlineId, EConnectReason connectReason )
{
    if( eConnectReasonChatRoomJoin == connectReason )
    {
        m_Engine.getToGui().toGuiHostJoinStatus( eHostTypeChatRoom, eHostJoinSendingJoinRequest );
        sendJoinRequest( sktBase, onlineId, connectReason );
    }

    return false;
}

//============================================================================
void HostBaseMgr::onConnectRequestFail( VxGUID& onlineId, EConnectStatus connectStatus, EConnectReason connectReason )
{
    if( eConnectReasonChatRoomJoin == connectReason )
    {
        m_Engine.getToGui().toGuiHostJoinStatus( eHostTypeChatRoom, eHostJoinConnectFailed, DescribeConnectStatus( connectStatus ) );
        m_ConnectionMgr.doneWithConnection( onlineId, this, connectReason);
    }
}

//============================================================================
void HostBaseMgr::onContactDisconnected( VxSktBase* sktBase, VxGUID& onlineId, EConnectReason connectReason )
{

}

//============================================================================
void HostBaseMgr::sendJoinRequest( VxSktBase* sktBase, VxGUID& onlineId, EConnectReason connectReason )
{
    vx_assert( nullptr != sktBase );
    PktHostJoinReq pktJoin;
    pktJoin.setHostType( eHostTypeChatRoom );
    if( m_Plugin.txPacket( onlineId, sktBase, &pktJoin ) )
    {
        m_Engine.getToGui().toGuiHostJoinStatus( eHostTypeChatRoom, eHostJoinSendingJoinRequest );
    }
    else
    {
        m_Engine.getToGui().toGuiHostJoinStatus( eHostTypeChatRoom, eHostJoinSendJoinRequestFailed );
        m_ConnectionMgr.doneWithConnection( onlineId, this, connectReason);
    }
}
