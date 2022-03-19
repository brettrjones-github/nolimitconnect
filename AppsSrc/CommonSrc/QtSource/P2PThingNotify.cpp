//============================================================================
// Copyright (C) 2013 Brett R. Jones 
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


#include "GuiOfferSession.h"
#include "AppCommon.h"
#include "AppGlobals.h"
#include "GuiOfferClientMgr.h"
#include "GuiOfferHostMgr.h"

#include "ToGuiActivityInterface.h"
#include "ActivityNetworkState.h"

#include <CoreLib/VxGlobals.h>

/*
//============================================================================
void AppCommon::slotNotifyButtonClick( void )
{
	if( VxIsAppShuttingDown() )
	{
		return;
	}

	ActivityNetworkState oDlg( *this, this );
	oDlg.exec();
}*/
/*
//============================================================================
void AppCommon::onMenuNotifySelected( int iMenuId, PopupMenu * senderPopupMenu, ActivityBase * contentFrame )
{
	Q_UNUSED( iMenuId );
	Q_UNUSED( senderPopupMenu );
    Q_UNUSED( contentFrame );
}*/

//============================================================================
//! received offer from friend
void AppCommon::toGuiRxedPluginOffer(			VxNetIdent *	netIdent,			// identity of friend
												EPluginType		ePluginType,		// plugin type
												const char *	pOfferMsg,			// offer message
												int			    pvUserData,			// plugin defined data
												const char *	pFileName, 			// filename if any
												uint8_t *		fileHashId,
												VxGUID&			lclSessionId,
												VxGUID&			rmtSessionId )
{
	if( VxIsAppShuttingDown() )
	{
		return;
	}

	// use signal so widgets will be built in gui thread
	// we must put into new session so offer msg and file name is still available when queued signal is received
	GuiOfferSession * poOffer = new GuiOfferSession(this);
	poOffer->setPluginType( ePluginType );
	poOffer->setHisIdent( m_UserMgr.getUser( netIdent->getMyOnlineId() ) );
	poOffer->setUserData( pvUserData );
	poOffer->setOfferMsg( pOfferMsg );
	poOffer->setIsRemoteInitiated( true );
	poOffer->setFileName( pFileName );
	poOffer->setFileHashId( fileHashId );
	poOffer->setLclSessionId( lclSessionId );
	poOffer->setRmtSessionId( rmtSessionId );
	poOffer->assuredValidLclSessionId();

	switch( ePluginType )
	{
	case ePluginTypePersonFileXfer:
		poOffer->setRequiresReply( true );
		break;
	default:
		break;
	}

	getOfferClientMgr().toGuiRxedPluginOffer( poOffer );
}

//============================================================================
void AppCommon::onToGuiRxedPluginOffer( GuiOfferSession * offerSession )
{
	toGuiActivityClientsLock();
	std::vector<ToGuiActivityClient>::iterator iter;
	for( iter = m_ToGuiActivityClientList.begin(); iter != m_ToGuiActivityClientList.end(); ++iter )
	{
		ToGuiActivityClient& client = *iter;
		client.m_Callback->doToGuiRxedPluginOffer( client.m_UserData, offerSession );
	}

	toGuiActivityClientsUnlock();
}

//============================================================================
//! response to offer
void AppCommon::toGuiRxedOfferReply(	VxNetIdent *	netIdent, 
										EPluginType		ePluginType, 
										int			    pvUserData, 
										EOfferResponse	eOfferResponse,
										const char *	pFileName,
										uint8_t *		fileHashData,
										VxGUID&			lclSessionId,
										VxGUID&			rmtSessionId )
{
	if( VxIsAppShuttingDown() )
	{
		return;
	}

	GuiOfferSession * poOffer = new GuiOfferSession(this);
	poOffer->setPluginType( ePluginType );
	poOffer->setHisIdent( m_UserMgr.getUser( netIdent->getMyOnlineId() ) );
	poOffer->setUserData( pvUserData );
	poOffer->setOfferResponse(eOfferResponse);
	poOffer->setIsRemoteInitiated( false );
	poOffer->setFileName( pFileName );
	poOffer->setFileHashId( fileHashData );
	poOffer->setRmtSessionId( rmtSessionId );
	poOffer->setLclSessionId( lclSessionId );
	poOffer->assuredValidLclSessionId();

	getOfferClientMgr().toGuiRxedOfferReply( poOffer );
}

//============================================================================
void AppCommon::onToGuiRxedOfferReply( GuiOfferSession * offerSession )
{
	toGuiActivityClientsLock();
	std::vector<ToGuiActivityClient>::iterator iter;
	for( iter = m_ToGuiActivityClientList.begin(); iter != m_ToGuiActivityClientList.end(); ++iter )
	{
		ToGuiActivityClient& client = *iter;
		client.m_Callback->doToGuiRxedOfferReply( client.m_UserData, offerSession );
	}

	toGuiActivityClientsUnlock();
}

//============================================================================
void AppCommon::toGuiPluginSessionEnded(	VxNetIdent *	netIdent, 
											EPluginType		ePluginType, 
											int				pvUserData, 
											EOfferResponse	eOfferResponse,
											VxGUID&			lclSessionId )
{
	if( VxIsAppShuttingDown() )
	{
		return;
	}

	GuiOfferSession * poOffer = new GuiOfferSession(this);
	poOffer->setPluginType( ePluginType );
	poOffer->setHisIdent( m_UserMgr.getUser( netIdent->getMyOnlineId() ) );
	poOffer->setUserData( pvUserData );
	poOffer->setOfferResponse(eOfferResponse);
	poOffer->setIsRemoteInitiated( false );
	poOffer->setLclSessionId( lclSessionId );
	poOffer->assuredValidLclSessionId();

	getOfferClientMgr().toGuiPluginSessionEnded( poOffer );
}

//============================================================================
void AppCommon::toGuiPluginStatus(	EPluginType		ePluginType,
										int				statusType,
										int				statusValue )
{
	emit signalToGuiPluginStatus( ePluginType, statusType, statusValue );
}

//============================================================================
void AppCommon::toGuiInstMsg(	VxNetIdent *	netIdent, 
								EPluginType		ePluginType,
								const char *	pMsg )
{
	if( VxIsAppShuttingDown() )
	{
		return;
	}

	emit signalToGuiInstMsg( m_UserMgr.getUser( netIdent->getMyOnlineId() ), ePluginType, pMsg );
}

//============================================================================
void AppCommon::slotToGuiInstMsg( GuiUser * netIdent, EPluginType ePluginType, QString pMsg )
{
	LogMsg( LOG_INFO, "slotToGuiInstMsg: toGuiActivityClientsLock\n" );
	toGuiActivityClientsLock();
	std::vector<ToGuiActivityClient>::iterator iter;
	for( iter = m_ToGuiActivityClientList.begin(); iter != m_ToGuiActivityClientList.end(); ++iter )
	{
		ToGuiActivityClient& client = *iter;
		client.m_Callback->toGuiInstMsg( client.m_UserData, netIdent, ePluginType, pMsg );
	}

	toGuiActivityClientsUnlock();
}
