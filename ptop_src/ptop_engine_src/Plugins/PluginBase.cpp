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
// http://www.nolimitconnect.org
//============================================================================

#include "PluginBase.h"
#include "PluginMgr.h"
#include "P2PSession.h"
#include "RxSession.h"
#include "TxSession.h"

#include <ptop_src/ptop_engine_src/P2PEngine/P2PEngine.h>

#include <CoreLib/VxFileUtil.h>

//============================================================================
std::string PluginBase::getThumbXferDbName( EPluginType pluginType )
{
    std::string thumbXferName( "ThumbXferDb" );
    thumbXferName += std::to_string( (int)pluginType );
    return thumbXferName + ".db3";
}

//============================================================================
std::string PluginBase::getThumbXferThreadName( EPluginType pluginType )
{
    std::string thumbXferName( "ThumbXferDb" );
    thumbXferName += std::to_string( (int)pluginType );
    return thumbXferName + ".db3";
}

//============================================================================
PluginBase::PluginBase( P2PEngine& engine, PluginMgr& pluginMgr, VxNetIdent * myIdent, EPluginType pluginType )
: PktPluginHandlerBase()
, m_ePluginType( pluginType )
, m_Engine( engine )
, m_PluginMgr( pluginMgr )
, m_AssetMgr( engine.getAssetMgr() )
, m_ThumbMgr( engine.getThumbMgr() )
, m_ThumbXferMgr( engine, engine.getThumbMgr(), *this, getThumbXferDbName(pluginType).c_str(), getThumbXferThreadName(pluginType).c_str() )
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
    }
}

//============================================================================
bool PluginBase::setPluginSetting( PluginSetting& pluginSetting, int64_t modifiedTimeMs )
{
    m_PluginSetting = pluginSetting;
    onPluginSettingChange( m_PluginSetting, modifiedTimeMs );

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
bool PluginBase::isAccessAllowed( VxNetIdent* netIdent, bool logAccessError, const char* accessReason )
{
	EFriendState curPermission = m_Engine.getMyPktAnnounce().getPluginPermission( m_ePluginType ); 
    if( eFriendStateIgnore != curPermission && eFriendStateIgnore != netIdent->getMyFriendshipToHim() )
    {
        if( netIdent->getMyFriendshipToHim() >= curPermission || netIdent->getMyOnlineId() == m_Engine.getMyOnlineId() )
        {
            return true;
        }
    }

    if( logAccessError )
    {
        if( accessReason )
        {
            LogMsg( LOG_WARN, "%s %s Access Not Allowed to %s %s %s", DescribePluginType( getPluginType() ), accessReason,
                netIdent->getMyOnlineId().describeVxGUID().c_str(), DescribeFriendState( netIdent->getMyFriendshipToHim() ), netIdent->getOnlineName() );

        }
        else
        {
            LogMsg( LOG_WARN, "%s Access Not Allowed to %s %s %s", DescribePluginType( getPluginType() ),
                netIdent->getMyOnlineId().describeVxGUID().c_str(), DescribeFriendState( netIdent->getMyFriendshipToHim() ), netIdent->getOnlineName() );
        }
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
bool PluginBase::txPacket( VxNetIdent * netIdent, VxSktBase * sktBase, VxPktHdr * poPkt, bool bDisconnectAfterSend )
{
	if( nullptr == sktBase )
	{
		LogMsg( LOG_WARN, "PluginBase::txPacket NULL sktBase" );
		return false;
	}

	return m_PluginMgr.pluginApiTxPacket( m_ePluginType, netIdent->getMyOnlineId(), sktBase, poPkt, bDisconnectAfterSend );
}

//============================================================================
bool PluginBase::txPacket( const VxGUID& onlineId, VxSktBase * sktBase, VxPktHdr * poPkt, bool bDisconnectAfterSend, EPluginType overridePlugin )
{
    if( nullptr == sktBase )
    {
        LogMsg( LOG_WARN, "PluginBase::txPacket NULL sktBase" );
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
bool PluginBase::fromGuiPushToTalk( VxNetIdent* netIdent, bool enableTalk )
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
    P2PSession * p2pSession = new P2PSession( sktBase, netIdent, m_ePluginType );
	p2pSession->setPluginType( m_ePluginType );
	return p2pSession;
}

//============================================================================ 
P2PSession * PluginBase::createP2PSession( VxGUID& lclSessionId, VxSktBase * sktBase, VxNetIdent * netIdent )
{
    P2PSession * p2pSession = new P2PSession( lclSessionId, sktBase, netIdent, m_ePluginType );
	p2pSession->setPluginType( m_ePluginType );
	return p2pSession;
}

//============================================================================ 
RxSession *	PluginBase::createRxSession( VxSktBase * sktBase, VxNetIdent * netIdent )
{
    RxSession * rxSession = new RxSession( sktBase, netIdent, m_ePluginType );
	rxSession->setPluginType( m_ePluginType );
	return rxSession;
}

//============================================================================ 
RxSession *	PluginBase::createRxSession( VxGUID& lclSessionId, VxSktBase * sktBase, VxNetIdent * netIdent )
{
    RxSession * rxSession =  new RxSession( lclSessionId, sktBase, netIdent, m_ePluginType );
	rxSession->setPluginType( m_ePluginType );
	return rxSession;
}

//============================================================================ 
TxSession *	PluginBase::createTxSession( VxSktBase * sktBase, VxNetIdent * netIdent )
{
    TxSession * txSession = new TxSession( sktBase, netIdent, m_ePluginType );
	txSession->setPluginType( m_ePluginType );
	return txSession;
}

//============================================================================ 
TxSession *	PluginBase::createTxSession( VxGUID& lclSessionId, VxSktBase * sktBase, VxNetIdent * netIdent )
{
    TxSession * txSession = new TxSession( lclSessionId, sktBase, netIdent, m_ePluginType );
	txSession->setPluginType( m_ePluginType );
	return txSession;
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

//============================================================================ 
EPluginType PluginBase::getDestinationPluginOverride( EHostType hostType )
{
    if( eHostTypeNetwork == hostType )
    {
        // no matter which plugin we send from if the destination host is network host
        // the always set the packet plugin destination to network host
        if( ePluginTypeHostNetwork == m_ePluginType )
        {
            LogMsg( LOG_ERROR, "Tried to send to host %s from plugin %s", DescribeHostType( hostType ), DescribePluginType( m_ePluginType ) );
            vx_assert( false );
            return ePluginTypeInvalid;
        }

        return ePluginTypeHostNetwork;
    }

    if( eHostTypePeerUserDirect == hostType )
    {
        // directly connected peer users can only access peer type plugins (Not Client/Host)
        return ePluginTypeInvalid;
    }

    if( eHostTypeGroup == hostType )
    {
        switch( m_ePluginType )
        {
        case ePluginTypeClientGroup:
            return ePluginTypeHostGroup;
        case ePluginTypeHostGroup:
            return ePluginTypeClientGroup;
        default:
            LogMsg( LOG_ERROR, "Tried to send to host %s from plugin %s", DescribeHostType( hostType ), DescribePluginType( m_ePluginType ) );
            vx_assert( false );
            return ePluginTypeInvalid;
        }
    }

    if( eHostTypeChatRoom == hostType )
    {
        switch( m_ePluginType )
        {
        case ePluginTypeClientChatRoom:
            return ePluginTypeHostChatRoom;
        case ePluginTypeHostChatRoom:
            return ePluginTypeClientChatRoom;
        default:
            LogMsg( LOG_ERROR, "Tried to send to host %s from plugin %s", DescribeHostType( hostType ), DescribePluginType( m_ePluginType ) );
            vx_assert( false );
            return ePluginTypeInvalid;
        }
    }

    if( eHostTypeRandomConnect == hostType )
    {
        switch( m_ePluginType )
        {
        case ePluginTypeClientRandomConnect:
            return ePluginTypeHostRandomConnect;
        case ePluginTypeHostRandomConnect:
            return ePluginTypeClientRandomConnect;
        default:
            LogMsg( LOG_ERROR, "Tried to send to host %s from plugin %s", DescribeHostType( hostType ), DescribePluginType( m_ePluginType ) );
            vx_assert( false );
            return ePluginTypeInvalid;
        }
    }

    if( eHostTypeConnectTest == hostType )
    {
        switch( m_ePluginType )
        {
        case ePluginTypeClientConnectTest:
            return ePluginTypeHostConnectTest;
        case ePluginTypeHostConnectTest:
            return ePluginTypeClientConnectTest;
        default:
            LogMsg( LOG_ERROR, "Tried to send to host %s from plugin %s", DescribeHostType( hostType ), DescribePluginType( m_ePluginType ) );
            vx_assert( false );
            return ePluginTypeInvalid;
        }
    }

    LogMsg( LOG_ERROR, "Tried to send to Unknown Host Type %d from plugin %s", hostType, DescribePluginType( m_ePluginType ) );
    vx_assert( false );
    return ePluginTypeInvalid;
}

//============================================================================
EPluginAccess PluginBase::getPluginAccessState( VxNetIdent* netIdent )
{
    EPluginAccess pluginAccess = ePluginAccessNotSet;

    EFriendState pluginState = m_MyIdent->getPluginPermission( getPluginType() );
    if( eFriendStateIgnore == pluginState )
    {
        // we are not enabled
        pluginAccess = ePluginAccessDisabled;
    }
    else
    {
        if( netIdent->isIgnored() )
        {
            pluginAccess = ePluginAccessIgnored;
        }
        else
        {
            EFriendState friendState = netIdent->getMyFriendshipToHim();
            // everybody gets at least guest permission
            if( m_Engine.getConnectIdListMgr().isHosted( netIdent->getMyOnlineId() ) && friendState == eFriendStateAnonymous )
            {
                friendState = eFriendStateGuest;
            }

            if( friendState < pluginState )
            {
                // not enough permission
                pluginAccess = ePluginAccessLocked;
            }
            else
            {
                pluginAccess = ePluginAccessOk;
            }
        }
    }

    return pluginAccess;
}

//============================================================================
void PluginBase::logCommError( ECommErr commErr, const char* desc, VxSktBase* sktBase, VxNetIdent* netIdent )
{
    LogMsg( LOG_ERROR, "%s %s from %s %s", desc, DescribeCommError( commErr ), netIdent->getOnlineName(), sktBase->describeSktConnection().c_str() );
}

//============================================================================
ECommErr PluginBase::getCommAccessState( VxNetIdent* netIdent )
{
    ECommErr commErr{ eCommErrNone };
    if( !isPluginEnabled() )
    {
        commErr = eCommErrPluginNotEnabled;
    }
    else
    {
        EPluginAccess pluginAccess = getPluginAccessState( netIdent );
        if( ePluginAccessOk != pluginAccess )
        {
            commErr = eCommErrPluginPermission;
        }
    }

    return commErr;
}
