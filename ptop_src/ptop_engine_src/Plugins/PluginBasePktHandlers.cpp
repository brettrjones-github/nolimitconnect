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

#include "PluginBase.h"

#include <ptop_src/ptop_engine_src/ThumbMgr/ThumbMgr.h>

//============================================================================
void PluginBase::onInvalidRxedPacket( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent, const char * msg )
{
    // TODO proper invalid packet handling
    LogMsg( LOG_INFO, "PluginBase::onInvalidRxedPacket plugin %s user %s", DescribePluginType( getPluginType() ), netIdent->getOnlineName() );
}

//============================================================================
void PluginBase::onPktUserConnect( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
	LogMsg( LOG_INFO, "PluginBase::onPktUserConnect" );
}

//============================================================================
void PluginBase::onPktUserDisconnect( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
	LogMsg( LOG_INFO, "PluginBase::onPktUserConnect" );
}

//============================================================================
void PluginBase::onPktPluginOfferReq( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
	LogMsg( LOG_ERROR, "PluginBase::onPktPluginOfferReq" );
}

//============================================================================
//! packet with remote users reply to offer
void PluginBase::onPktPluginOfferReply( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
	LogMsg( LOG_ERROR, "PluginBase::onPktPluginOfferReply" );
}

//============================================================================
void PluginBase::onPktSessionStartReq( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
	LogMsg( LOG_ERROR, "PluginBase::onPktSessionStartReq" );
}
//============================================================================
void PluginBase::onPktSessionStartReply( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
	LogMsg( LOG_ERROR, "PluginBase::onPktSessionStartReply" );
}
//============================================================================
void PluginBase::onPktSessionStopReq( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
	LogMsg( LOG_ERROR, "PluginBase::onPktSessionStopReq" );
}

//============================================================================
void PluginBase::onPktSessionStopReply( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
	LogMsg( LOG_ERROR, "PluginBase::onPktSessionStopReply" );
}

//============================================================================
void PluginBase::onPktMyPicSendReq( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
	LogMsg( LOG_ERROR, "PluginBase::onPktMyPicSendReq" );
}

//============================================================================
void PluginBase::onPktMyPicSendReply( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
	LogMsg( LOG_ERROR, "PluginBase::onPktMyPicSendReply" );
}

//============================================================================
void PluginBase::onPktWebServerPicChunkTx( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
	LogMsg( LOG_ERROR, "PluginBase::onPktWebServerPicChunkTx" );
}

//============================================================================
void PluginBase::onPktWebServerPicChunkAck( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
	LogMsg( LOG_ERROR, "PluginBase::onPktWebServerPicChunkAck" );
}

//============================================================================
void PluginBase::onPktWebServerGetChunkTx( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
	LogMsg( LOG_ERROR, "PluginBase::onPktWebServerGetChunkTx" );
}

//============================================================================
void PluginBase::onPktWebServerGetChunkAck( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
	LogMsg( LOG_ERROR, "PluginBase::onPktWebServerGetChunkAck" );
}

//============================================================================
void PluginBase::onPktWebServerPutChunkTx( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
	LogMsg( LOG_ERROR, "PluginBase::onPktWebServerPutChunkTx" );
}

//============================================================================
void PluginBase::onPktWebServerPutChunkAck( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
	LogMsg( LOG_ERROR, "PluginBase::onPktWebServerPutChunkAck" );
}

//============================================================================
void PluginBase::onPktThumbGetReq( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
    m_ThumbXferMgr.onPktThumbGetReq( sktBase, pktHdr, netIdent );
}

//============================================================================
void PluginBase::onPktThumbGetReply( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
    m_ThumbXferMgr.onPktThumbGetReply( sktBase, pktHdr, netIdent );
}

//============================================================================
void PluginBase::onPktThumbSendReq( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
    m_ThumbXferMgr.onPktThumbSendReq( sktBase, pktHdr, netIdent );
}

//============================================================================
void PluginBase::onPktThumbSendReply( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
    m_ThumbXferMgr.onPktThumbSendReply( sktBase, pktHdr, netIdent );
}

//============================================================================
void PluginBase::onPktThumbChunkReq( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
    m_ThumbXferMgr.onPktThumbChunkReq( sktBase, pktHdr, netIdent );
}

//============================================================================
void PluginBase::onPktThumbChunkReply( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
    m_ThumbXferMgr.onPktThumbChunkReply( sktBase, pktHdr, netIdent );
}

//============================================================================
void PluginBase::onPktThumbGetCompleteReq( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
    m_ThumbXferMgr.onPktThumbGetCompleteReq( sktBase, pktHdr, netIdent );
}

//============================================================================
void PluginBase::onPktThumbGetCompleteReply( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
    m_ThumbXferMgr.onPktThumbGetCompleteReply( sktBase, pktHdr, netIdent );
}

//============================================================================
void PluginBase::onPktThumbSendCompleteReq( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
    m_ThumbXferMgr.onPktThumbSendCompleteReq( sktBase, pktHdr, netIdent );
}

//============================================================================
void PluginBase::onPktThumbSendCompleteReply( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
    m_ThumbXferMgr.onPktThumbSendCompleteReply( sktBase, pktHdr, netIdent );
}

//============================================================================
void PluginBase::onPktThumbXferErr( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
    m_ThumbXferMgr.onPktThumbXferErr( sktBase, pktHdr, netIdent );
}
