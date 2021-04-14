//============================================================================
// Copyright (C) 2010 Brett R. Jones
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

#include "PluginBase.h"
#include "PluginMgr.h"
#include "P2PSession.h"
#include "RxSession.h"
#include "TxSession.h"

#include <GoTvCore/GoTvP2P/P2PEngine/P2PEngine.h>

#include <CoreLib/VxFileUtil.h>

//============================================================================
PluginBase::PluginBase( P2PEngine& engine, PluginMgr& pluginMgr, VxNetIdent * myIdent )
: PktPluginHandlerBase()
, m_Engine( engine )
, m_PluginMgr( pluginMgr )
, m_MyIdent( myIdent )
{
}

//============================================================================
IToGui& PluginBase::getToGui()
{ 
    return m_Engine.getToGui(); 
}

//============================================================================
void PluginBase::setPluginType( EPluginType ePluginType )
{
    m_ePluginType = ePluginType;
    m_PluginSetting.setPluginType( ePluginType );
}

//============================================================================
void PluginBase::pluginStartup( void )
{
    if( getPluginType() != ePluginTypeInvalid )
    {
        m_Engine.getPluginSetting( getPluginType(), m_PluginSetting );
        generateSettingPkt( m_PluginSetting );
    }
}

//============================================================================
bool PluginBase::setPluginSetting( PluginSetting& pluginSetting )
{
    m_PluginSetting = pluginSetting;
    generateSettingPkt( pluginSetting );
    onPluginSettingChange( m_PluginSetting );
    return true;
}

//============================================================================
bool PluginBase::generateSettingPkt( PluginSetting& pluginSetting )
{
    PktBlobEntry& blobEntry = m_PktPluginSettingReply.getBlobEntry();
    blobEntry.resetWrite();
    pluginSetting.addToBlob( blobEntry );
    m_PktPluginSettingReply.calcPktLen();
    return true;
}

//============================================================================
EHostType PluginBase::getHostType( void )
{
    EHostType hostType = eHostTypeUnknown;
    switch( getPluginType() )
    {
    case ePluginTypeClientChatRoom:
    case ePluginTypeHostChatRoom:
        hostType = eHostTypeChatRoom;
        break;

    case ePluginTypeClientConnectTest:
    case ePluginTypeHostConnectTest:
        hostType = eHostTypeConnectTest;
        break;

    case ePluginTypeClientGroup:
    case ePluginTypeHostGroup:
        hostType = eHostTypeGroup;
        break;

    case ePluginTypeClientRandomConnect:
    case ePluginTypeHostRandomConnect:
        hostType = eHostTypeRandomConnect;
        break;

    case ePluginTypeClientNetwork:
    case ePluginTypeHostNetwork:
    case ePluginTypeNetworkSearchList:
        hostType = eHostTypeNetwork;
        break;

    default:
        break;
    }

    return hostType;
}

//============================================================================
EAppState PluginBase::getPluginState( void )
{
	if( eFriendStateIgnore == getPluginPermission() )
	{
		return eAppStatePermissionErr;
	}

	return m_ePluginState;
}

//============================================================================
EFriendState PluginBase::getPluginPermission( void )
{ 
	return m_Engine.getMyPktAnnounce().getPluginPermission( m_ePluginType ); 
}

//============================================================================
void PluginBase::setPluginPermission( EFriendState eFriendState )		
{ 
	EFriendState prevState = m_Engine.getMyPktAnnounce().getPluginPermission( m_ePluginType ); 
	if( prevState != eFriendState )
	{
		m_Engine.getMyPktAnnounce().setPluginPermission( m_ePluginType, eFriendState );
		m_Engine.doPktAnnHasChanged( false );
	}
};

//============================================================================
bool PluginBase::isAccessAllowed( VxNetIdent * hisIdent )
{
	EFriendState curPermission = m_Engine.getMyPktAnnounce().getPluginPermission( m_ePluginType ); 
	if( ( eFriendStateIgnore != curPermission )
		&& ( ( eFriendStateAnonymous == curPermission ) || ( hisIdent->getMyFriendshipToHim() >= curPermission ) ) )
	{
		return true;
	}

	return false;
}

//============================================================================
bool PluginBase::isPluginEnabled( void )
{
	return ( eFriendStateIgnore != getPluginPermission() );
}

//============================================================================
void PluginBase::onSessionStart( PluginSessionBase * poSession, bool pluginIsLocked )
{
	m_Engine.onSessionStart( poSession->getPluginType(), poSession->getIdent() );
}

//============================================================================
void PluginBase::onInvalidRxedPacket( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent, const char * msg )
{
    // TODO proper invalid packet handling
    LogMsg( LOG_INFO, "PluginBase::onInvalidRxedPacket plugin %s user %s", DescribePluginType( getPluginType() ), netIdent->getOnlineName() );
}

//============================================================================
void PluginBase::onPktUserConnect( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
	LogMsg( LOG_INFO, "PluginBase::onPktUserConnect\n" );
}

//============================================================================
void PluginBase::onPktUserDisconnect( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
	LogMsg( LOG_INFO, "PluginBase::onPktUserConnect\n" );
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
	LogMsg( LOG_ERROR, "PluginBase::onPktSessionStopReply\n" );
}

//============================================================================
void PluginBase::onPktMyPicSendReq( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
	LogMsg( LOG_ERROR, "PluginBase::onPktMyPicSendReq\n" );
}

//============================================================================
void PluginBase::onPktMyPicSendReply( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
	LogMsg( LOG_ERROR, "PluginBase::onPktMyPicSendReply\n" );
}

//============================================================================
void PluginBase::onPktWebServerPicChunkTx( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
	LogMsg( LOG_ERROR, "PluginBase::onPktWebServerPicChunkTx\n" );
}

//============================================================================
void PluginBase::onPktWebServerPicChunkAck( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
	LogMsg( LOG_ERROR, "PluginBase::onPktWebServerPicChunkAck\n" );
}

//============================================================================
void PluginBase::onPktWebServerGetChunkTx( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
	LogMsg( LOG_ERROR, "PluginBase::onPktWebServerGetChunkTx\n" );
}

//============================================================================
void PluginBase::onPktWebServerGetChunkAck( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
	LogMsg( LOG_ERROR, "PluginBase::onPktWebServerGetChunkAck\n" );
}

//============================================================================
void PluginBase::onPktWebServerPutChunkTx( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
	LogMsg( LOG_ERROR, "PluginBase::onPktWebServerPutChunkTx\n" );
}

//============================================================================
void PluginBase::onPktWebServerPutChunkAck( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
	LogMsg( LOG_ERROR, "PluginBase::onPktWebServerPutChunkAck\n" );
}

//============================================================================
bool PluginBase::txPacket( VxNetIdent * netIdent, VxSktBase * sktBase, VxPktHdr * poPkt, bool bDisconnectAfterSend )
{
	if( NULL == sktBase )
	{
		LogMsg( LOG_WARN, "PluginBase::txPacket NULL sktBase\n" );
		return false;
	}

	return m_PluginMgr.pluginApiTxPacket( m_ePluginType, netIdent->getMyOnlineId(), sktBase, poPkt, bDisconnectAfterSend );
}

//============================================================================
bool PluginBase::txPacket( VxGUID& onlineId, VxSktBase * sktBase, VxPktHdr * poPkt, bool bDisconnectAfterSend, EPluginType overridePlugin )
{
    if( NULL == sktBase )
    {
        LogMsg( LOG_WARN, "PluginBase::txPacket NULL sktBase\n" );
        return false;
    }

    return m_PluginMgr.pluginApiTxPacket( m_ePluginType, onlineId, sktBase, poPkt, bDisconnectAfterSend, overridePlugin );
}

//============================================================================
void PluginBase::fromGuiGetFileShareSettings( FileShareSettings& fileShareSettings )
{
}

//============================================================================
void PluginBase::fromGuiSetFileShareSettings( FileShareSettings& fileShareSettings )
{
}

//============================================================================
//! user wants to send offer to friend
bool PluginBase::fromGuiMakePluginOffer(	VxNetIdent *	netIdent, 
											int				pvUserData, 
											const char *	pOfferMsg, 
											const char *	pFileName,
											uint8_t *			fileHashId,
											VxGUID			lclSessionId )
{
	return false;
}

//============================================================================
//! handle reply to offer
bool PluginBase::fromGuiOfferReply(		VxNetIdent *	netIdent,
										int			pvUserData,				
										EOfferResponse	eOfferResponse,
										VxGUID			lclSessionId )
{
	return false;
}

//============================================================================
int PluginBase::fromGuiPluginControl(		VxNetIdent *	netIdent,
											const char *	pControl, 
											const char *	pAction,
											uint32_t				u32ActionData,
											VxGUID&			fileId,
											uint8_t *			fileHashId )
{
	return false;
}

//============================================================================ 
bool PluginBase::fromGuiInstMsg(	VxNetIdent *	netIdent, 
									const char *	pMsg )
{
	return false;
}

//============================================================================ 
void PluginBase::makeShortFileName( const char * pFullFileName, std::string& strShortFileName )
{
    VxFileUtil::makeShortFileName( pFullFileName, strShortFileName );
}

//============================================================================ 
EPluginAccess PluginBase::canAcceptNewSession( VxNetIdent * netIdent ) 
{ 
    return netIdent->getHisAccessPermissionFromMe( m_ePluginType ); 
}

//============================================================================ 
P2PSession * PluginBase::createP2PSession( VxSktBase * sktBase, VxNetIdent * netIdent )
{
	P2PSession * p2pSession = new P2PSession( sktBase, netIdent );
	p2pSession->setPluginType( m_ePluginType );
	return p2pSession;
}

//============================================================================ 
P2PSession * PluginBase::createP2PSession( VxGUID& lclSessionId, VxSktBase * sktBase, VxNetIdent * netIdent )
{
	P2PSession * p2pSession = new P2PSession( lclSessionId, sktBase, netIdent );
	p2pSession->setPluginType( m_ePluginType );
	return p2pSession;
}

//============================================================================ 
RxSession *	PluginBase::createRxSession( VxSktBase * sktBase, VxNetIdent * netIdent )
{
	RxSession * rxSession = new RxSession( sktBase, netIdent );
	rxSession->setPluginType( m_ePluginType );
	return rxSession;
}

//============================================================================ 
RxSession *	PluginBase::createRxSession( VxGUID& lclSessionId, VxSktBase * sktBase, VxNetIdent * netIdent )
{
	RxSession * rxSession =  new RxSession( lclSessionId, sktBase, netIdent );
	rxSession->setPluginType( m_ePluginType );
	return rxSession;
}

//============================================================================ 
TxSession *	PluginBase::createTxSession( VxSktBase * sktBase, VxNetIdent * netIdent )
{
	TxSession * txSession = new TxSession( sktBase, netIdent );
	txSession->setPluginType( m_ePluginType );
	return txSession;
}

//============================================================================ 
TxSession *	PluginBase::createTxSession( VxGUID& lclSessionId, VxSktBase * sktBase, VxNetIdent * netIdent )
{
	TxSession * txSession = new TxSession( lclSessionId, sktBase, netIdent );
	txSession->setPluginType( m_ePluginType );
	return txSession;
}

//============================================================================ 
void PluginBase::fromGuiAppIdle( void )
{
}

//============================================================================ 
void PluginBase::onAppStartup( void )
{
}

//============================================================================ 
void PluginBase::onAppShutdown( void )
{
}

//============================================================================ 
void PluginBase::fromGuiAppPause( void )
{
	m_AppIsPaused = true;
}

//============================================================================ 
void PluginBase::fromGuiAppResume( void )
{
	m_AppIsPaused = false;
}
