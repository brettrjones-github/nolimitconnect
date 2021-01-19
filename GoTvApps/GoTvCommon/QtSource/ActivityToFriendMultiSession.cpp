//============================================================================
// Copyright (C) 2015 Brett R. Jones
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
#include <app_precompiled_hdr.h>
#include "ActivityToFriendMultiSession.h"
#include "ActivityMessageBox.h"
#include "GuiOfferSession.h"
#include "AppGlobals.h"
#include "MyIcons.h"
#include "AppCommon.h"

#include <GoTvCore/GoTvP2P/P2PEngine/P2PEngine.h>
#include <GoTvCore/GoTvP2P/P2PEngine/EngineSettings.h>

#include <CoreLib/VxGlobals.h>

//============================================================================
ActivityToFriendMultiSession::ActivityToFriendMultiSession(	AppCommon&			app,
															VxNetIdent *			netIdent, 
															QWidget *				parent )
: ActivityToFriendBase( OBJNAME_ACTIVITY_TO_FRIEND_MULTISESSION, app, ePluginTypeMessenger, netIdent, parent, eAppletMessenger, true )
, m_TodGameWidget( 0 )
, m_TodGameLogic( app, app.getEngine(), ePluginTypeMessenger, this )
, m_IsInitialized( false )
, m_OfferOrResponseIsSent( false )
, m_CanSend( false )
{
	setupMultiSessionActivity( netIdent );
	m_OfferSessionLogic.sendOfferOrResponse();
	m_OfferOrResponseIsSent = true;
	m_IsInitialized = true;
}

//============================================================================
ActivityToFriendMultiSession::ActivityToFriendMultiSession(	AppCommon&			app,
															GuiOfferSession *		poOffer, 
															QWidget *				parent )
: ActivityToFriendBase( OBJNAME_ACTIVITY_TO_FRIEND_MULTISESSION, app, ePluginTypeMessenger, poOffer, parent, eAppletMessenger, true )
, m_TodGameWidget( 0 )
, m_TodGameLogic( app, app.getEngine(), ePluginTypeMessenger, this )
, m_IsInitialized( false )
, m_OfferOrResponseIsSent( false )
, m_CanSend( false )
{
	setupMultiSessionActivity( poOffer->getHisIdent() );
	m_OfferSessionLogic.sendOfferOrResponse();
	m_OfferOrResponseIsSent = true;
	m_IsInitialized = true;
}

//============================================================================
void ActivityToFriendMultiSession::setupMultiSessionActivity( VxNetIdent * hisIdent )
{
	m_HisIdent = hisIdent;
	ui.setupUi( this );
    QString titleText = QObject::tr("Text Chat - ");
    titleText += hisIdent->getOnlineName();
    ui.m_TitleBarWidget->setTitleBarText( titleText );
    connectBarWidgets();

	m_OffersFrame			= ui.m_OffersFrame;
	m_ResponseFrame			= ui.m_ResponseFrame;
	m_HangupSessionFrame	= ui.m_HangupSessionFrame;
	m_VidChatWidget			= ui.m_VidWidget;
	m_TodGameWidget			= ui.m_TodGameWidget;

	m_VidChatWidget->setVideoFeedId( m_HisIdent->getMyOnlineId() );
	m_TodGameWidget->getVidWidget()->setVideoFeedId( m_HisIdent->getMyOnlineId() );

	m_ResponseFrame->setVisible( false );
	m_HangupSessionFrame->setVisible( false );
	m_VidChatWidget->setVisible( false );
	m_VidChatWidget->setRecordFilePath( VxGetDownloadsDirectory().c_str() );
	m_VidChatWidget->setRecordFriendName( m_HisIdent->getOnlineName() );

	connect( ui.m_TitleBarWidget,	SIGNAL(signalBackButtonClicked()),	this,	SLOT(accept()) );

	slotRepositionToParent();
	ui.m_SessionWidget->setIsPersonalRecorder( false );
	ui.m_SessionWidget->setIdents( &m_Engine.getMyPktAnnounce(), hisIdent );
	ui.m_SessionWidget->setEntryMode( eAssetTypeUnknown );
	ui.m_OptionsButton->setVisible( false );
	ui.m_SendAttachmentButton->setVisible( false );

	m_TodGameLogic.setGuiWidgets( m_HisIdent, m_TodGameWidget );
	m_TodGameLogic.setVisible( false );
	connect( this, SIGNAL(signalToGuiSetGameValueVar(long,long)),				&m_TodGameLogic,	SLOT(slotToGuiSetGameValueVar(long,long)) );
	connect( this, SIGNAL(signalToGuiSetGameActionVar(long,long)),				&m_TodGameLogic,	SLOT(slotToGuiSetGameActionVar(long,long)) );
	connect( this, SIGNAL(signalToGuiContactOnline(VxNetIdent *,bool)),				this,			SLOT(slotToGuiContactOnlineMultisession(VxNetIdent *,bool)) );
	connect( this, SIGNAL(signalToGuiContactOffline(VxNetIdent *)),				this,				SLOT(slotToGuiContactOfflineMultisession(VxNetIdent *)) );
	connect( this, SIGNAL(signalToGuiPluginSessionEnded(GuiOfferSession *)),	this,				SLOT(slotToGuiPluginSessionEnded(GuiOfferSession *)) );
	connect( this, SIGNAL(signalToGuiMultiSessionAction(VxGuidQt,EMSessionAction,int)),	this,		SLOT(slotToGuiMultiSessionAction(VxGuidQt,EMSessionAction,int)) );

	connect( ui.m_VoipButton,		SIGNAL(clicked()),						this,	SLOT(slotUserInputButtonClicked()) );
	connect( ui.m_VideoChatButton,	SIGNAL(clicked()),						this,	SLOT(slotUserInputButtonClicked()) );
	connect( ui.m_TrueOrDareButton, SIGNAL(clicked()),						this,	SLOT(slotUserInputButtonClicked()) );
	connect( ui.m_SessionWidget,	SIGNAL(signalUserInputButtonClicked()),	this,	SLOT(slotUserInputButtonClicked()) );

	MultiSessionState * sessionState = new MultiSessionState( m_MyApp, *this, eMSessionTypePhone );
	sessionState->setGuiWidgets(	hisIdent,
									m_OffersFrame,
									m_ResponseFrame,
									m_HangupSessionFrame,
									ui.m_VoipButton,
									ui.m_AcceptOfferButton,
									ui.m_AcceptLabel,
									ui.m_RejectOfferButton,
									ui.m_RejectLabel,
									ui.m_HangupSessionButton,
									m_VidChatWidget,
									&m_TodGameLogic );
	m_MSessionsList.push_back( sessionState );

	sessionState = new MultiSessionState( m_MyApp, *this, eMSessionTypeVidChat );
	sessionState->setGuiWidgets(	hisIdent,
									m_OffersFrame,
									m_ResponseFrame,
									m_HangupSessionFrame,
									ui.m_VideoChatButton,
									ui.m_AcceptOfferButton,
									ui.m_AcceptLabel,
									ui.m_RejectOfferButton,
									ui.m_RejectLabel,
									ui.m_HangupSessionButton,
									m_VidChatWidget,
									&m_TodGameLogic );
	m_MSessionsList.push_back( sessionState );

	sessionState = new MultiSessionState( m_MyApp, *this, eMSessionTypeTruthOrDare );
	sessionState->setGuiWidgets(	hisIdent,
									m_OffersFrame,
									m_ResponseFrame,
									m_HangupSessionFrame,
									ui.m_TrueOrDareButton,
									ui.m_AcceptOfferButton,
									ui.m_AcceptLabel,
									ui.m_RejectOfferButton,
									ui.m_RejectLabel,
									ui.m_HangupSessionButton,
									m_VidChatWidget,
									&m_TodGameLogic );
	m_MSessionsList.push_back( sessionState );
	ui.m_SessionWidget->setCanSend( false );

	if( false == checkForSendAccess( false ) )
	{
		showReasonAccessNotAllowed();
	}

	m_MyApp.toGuiAssetAction( eAssetActionRxViewingMsg, m_HisIdent->getMyOnlineId(), 1 );
}

//============================================================================
void ActivityToFriendMultiSession::onActivityFinish( void )
{
	ui.m_SessionWidget->onActivityStop();
	m_OfferSessionLogic.onStop();
	this->deleteLater();
}

//============================================================================
void ActivityToFriendMultiSession::doToGuiRxedPluginOffer( void * callbackData, GuiOfferSession * offer )
{
	m_OfferSessionLogic.doToGuiRxedPluginOffer( offer );
}

//============================================================================
void ActivityToFriendMultiSession::doToGuiRxedOfferReply( void * callbackData, GuiOfferSession * offerSession )
{
	m_OfferSessionLogic.doToGuiRxedOfferReply( offerSession );
}

//============================================================================
void ActivityToFriendMultiSession::showReasonAccessNotAllowed( void )
{
	EPluginAccess accessState = m_HisIdent->getMyAccessPermissionFromHim( m_ePluginType );
	QString accessDesc = DescribePluginType( m_ePluginType );
	accessDesc += QObject::tr( " with " );
	accessDesc += m_HisIdent->getOnlineName();
	accessDesc +=  " ";

	if( ePluginAccessOk != accessState )
	{
		accessDesc +=  DescribePluginAccess( accessState );
		setStatusMsg( accessDesc );
		return;
	}

	if( false == m_HisIdent->isOnline() )
	{
		accessDesc +=   QObject::tr( " requires user be online " );
		setStatusMsg( accessDesc );
		return;
	}
}

//============================================================================
void ActivityToFriendMultiSession::slotToGuiContactOnlineMultisession( VxNetIdent * hisIdent, bool newContact )
{
	if( hisIdent->getMyOnlineId() == m_HisIdent->getMyOnlineId() )
	{
		QString statMsg = m_HisIdent->getOnlineName();
		statMsg += QObject::tr( " is online" );
		setStatusMsg( statMsg );

		checkForSendAccess( false );
	}
}

//============================================================================
void ActivityToFriendMultiSession::slotToGuiContactOfflineMultisession( VxNetIdent * hisIdent )
{
	if( hisIdent->getMyOnlineId() == m_HisIdent->getMyOnlineId() )
	{
		QString statMsg = m_HisIdent->getOnlineName();
		statMsg += QObject::tr( " went offline-chat is disabled" );
		setStatusMsg( statMsg );

		checkForSendAccess( false );
	}
}

//============================================================================
void ActivityToFriendMultiSession::slotUserInputButtonClicked( void )
{
	checkForSendAccess( true );
}

//============================================================================
bool ActivityToFriendMultiSession::checkForSendAccess( bool sendOfferIfPossible )
{
	bool canSend = false;
	if( m_HisIdent->isOnline() 
		&& m_HisIdent->isMyAccessAllowedFromHim( m_ePluginType ) )
	{
		canSend = true;
	}

	if( canSend != m_CanSend )
	{
		m_CanSend = canSend;
		ui.m_SessionWidget->setCanSend( m_CanSend );
	}

	if( m_CanSend && sendOfferIfPossible && ( false == m_OfferOrResponseIsSent ) )
	{
		QString offMsg = QObject::tr( "Requested Text Chat Session with " );
		offMsg += m_HisIdent->getOnlineName();
		m_MyApp.toGuiStatusMessage( offMsg.toUtf8().constData() );
		//m_Engine.fromGuiMultiSessionAction( eMSessionActionChatSessionReq, m_HisIdent->getMyOnlineId(), 0, m_OfferSessionLogic.getOfferSessionId() );
		if( m_OfferSessionLogic.sendOfferOrResponse() )
		{
			m_OfferOrResponseIsSent = true;
		}
	}

	QVector<MultiSessionState*>::iterator iter;
	for( iter = m_MSessionsList.begin();  iter != m_MSessionsList.end(); ++iter )
	{
		(*iter)->checkForSendAccess();
	}

	return canSend;
}


//============================================================================
void ActivityToFriendMultiSession::showEvent( QShowEvent * ev )
{
	Q_UNUSED( ev );
	ActivityToFriendBase::showEvent( ev );
	m_MyApp.wantToGuiActivityCallbacks( this, this, true );
	m_MyApp.toGuiAssetAction( eAssetActionRxViewingMsg, m_HisIdent->getMyOnlineId(), 1 );
}

//============================================================================
void ActivityToFriendMultiSession::hideEvent( QHideEvent * ev )
{
	Q_UNUSED( ev );
	m_MyApp.toGuiAssetAction( eAssetActionRxViewingMsg, m_HisIdent->getMyOnlineId(), 0 );
	m_MyApp.wantToGuiActivityCallbacks( this, this, false );

	// purposely bypass  ActivityToFriendBase::hideEvent and call ActivityBase::hideEvent instead
	// this is so ??
	m_MyApp.setPluginVisible( m_ePluginType, false );
	ActivityBase::hideEvent( ev );
}

//============================================================================
MultiSessionState *	 ActivityToFriendMultiSession::getMSessionState( EMSessionType sessionType )
{
	if( (sessionType >= 0 ) 
		&& ( sessionType < m_MSessionsList.size() ) )
	{
		return m_MSessionsList[ sessionType ];
	}

	return 0;
}

//============================================================================
void ActivityToFriendMultiSession::onSessionStateChange( ESessionState eSessionState )
{

}

//============================================================================
// called from session logic
void ActivityToFriendMultiSession::onInSession( bool isInSession )
{
	QVector<MultiSessionState*>::iterator iter;
	for( iter = m_MSessionsList.begin();  iter != m_MSessionsList.end(); ++iter )
	{
		(*iter)->onInSession( isInSession );
	}
}

//============================================================================
void ActivityToFriendMultiSession::toGuiMultiSessionAction( void * callbackData, EMSessionAction mSessionAction, VxGUID& onlineId, int pos0to100000 )
{
	Q_UNUSED( callbackData );
	if( m_HisIdent && m_HisIdent->getMyOnlineId() == onlineId )
	{
		VxGuidQt guidId( onlineId );
		emit signalToGuiMultiSessionAction( onlineId, mSessionAction, pos0to100000 );
	}
}

//============================================================================
void ActivityToFriendMultiSession::toGuiClientPlayVideoFrame(	void *			userData, 
																VxGUID&			onlineId, 
																uint8_t *			pu8Jpg, 
																uint32_t				u32JpgDataLen,
																int				motion0To100000 )
{
	if( m_IsInitialized )
	{
		ui.m_SessionWidget->playVideoFrame( onlineId, pu8Jpg, u32JpgDataLen, motion0To100000 );
		m_TodGameWidget->ui.TruthOrDareWidget->playVideoFrame( onlineId, pu8Jpg, u32JpgDataLen, motion0To100000 );
		if( m_VidChatWidget && m_VidChatWidget->isVisible() )
		{
			m_VidChatWidget->playVideoFrame( onlineId, pu8Jpg, u32JpgDataLen, motion0To100000 );
		}
	}
}

//============================================================================
void ActivityToFriendMultiSession::slotToGuiMultiSessionAction( VxGuidQt idPro, EMSessionAction mSessionAction, int pos0to100000 )
{
	if( !m_IsInitialized || ( 0 == m_HisIdent ) )
	{
		return;
	}

	if(		( idPro.getVxGUIDHiPart() == m_HisIdent->getMyOnlineId().getVxGUIDHiPart() )
		&&	( idPro.getVxGUIDLoPart() == m_HisIdent->getMyOnlineId().getVxGUIDLoPart() ) )
	{
		if( eMSessionActionChatSessionAccept == mSessionAction )
		{
			onInSession( true );
		}
		else if( eMSessionActionChatSessionReject == mSessionAction )
		{
			onInSession( false );
		}

		MultiSessionState * sessionState = getMSessionState( (EMSessionType)pos0to100000 );
		if( sessionState )
		{
			sessionState->handleMultiSessionAction( mSessionAction );
		}
	}
}

//============================================================================
void ActivityToFriendMultiSession::slotToGuiPluginSessionEnded( GuiOfferSession * offer )
{
	//handleSessionEnded( offerResponse, this );
}; 

//============================================================================
void ActivityToFriendMultiSession::setStatusMsg( QString strStatus )
{
	m_MyApp.toGuiStatusMessage( strStatus.toUtf8().constData() );
}

//============================================================================
void ActivityToFriendMultiSession::toGuiSetGameValueVar(	void *		userData, 
															EPluginType ePluginType, 
															VxGUID&		onlineId, 
															int32_t			s32VarId, 
															int32_t			s32VarValue )
{
	if( ( ePluginType == m_ePluginType )
		&& ( onlineId == m_HisIdent->getMyOnlineId() ) )
	{
		emit signalToGuiSetGameValueVar( s32VarId, s32VarValue );
	}
}

//============================================================================
void ActivityToFriendMultiSession::toGuiSetGameActionVar(	void *		userData, 
															EPluginType ePluginType, 
															VxGUID&		onlineId, 
															int32_t			s32VarId, 
															int32_t			s32VarValue )
{
	if( ( ePluginType == m_ePluginType )
		&& ( onlineId == m_HisIdent->getMyOnlineId() ) )
	{
		emit signalToGuiSetGameActionVar( s32VarId, s32VarValue );
	}
}
