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

#include "ActivityToFriendTodGame.h"
#include "AppCommon.h"

#include "GuiOfferSession.h"
#include "AppGlobals.h"

#include <GoTvCore/GoTvP2P/P2PEngine/P2PEngine.h>
#include <GoTvCore/GoTvP2P/P2PEngine/EngineSettings.h>

#include <CoreLib/VxGlobals.h>

namespace
{
	#define GAME_SETTINGS_KEY "TODGAME"
}

//============================================================================
ActivityToFriendTodGame::ActivityToFriendTodGame(	AppCommon&			app,
													VxNetIdent *			netIdent, 
													QWidget *				parent )
: ActivityToFriendBase( OBJNAME_ACTIVITY_TO_FRIEND_TOD_GAME, app, ePluginTypeTruthOrDare, netIdent, parent, eAppletMessenger, true )
, m_TodGameLogic( app, app.getEngine(), ePluginTypeTruthOrDare, this )
{
	setupActivityToFriendTodGame();
	m_OfferSessionLogic.sendOfferOrResponse();
	//bool bSentMsg = m_FromGui.fromGuiMakePluginOffer(	m_ePluginType, 
	//													m_HisIdent->getMyOnlineId(), 
	//													this,
	//													describePluginOffer( m_ePluginType ).toUtf8().constData(),
	//													0,
	//													0,
	//													m_LclSessionId );
	//if( false == bSentMsg )
	//{
	//	handleUserWentOffline();
	//}
	//else
	//{
	//	setStatusText( tr( "Waiting For Offer Reply" ) );
	//}
	m_TodGameLogic.setGameStatus( eTxedOffer );
}

//============================================================================
ActivityToFriendTodGame::ActivityToFriendTodGame(	AppCommon&			    app,
													GuiOfferSession *		poOffer, 
													QWidget *				parent )
: ActivityToFriendBase( OBJNAME_ACTIVITY_TO_FRIEND_TOD_GAME, app, ePluginTypeTruthOrDare, poOffer, parent, eAppletMessenger, true )
, m_TodGameLogic( app, app.getEngine(), ePluginTypeTruthOrDare, this )
{
	setupActivityToFriendTodGame();
	m_TodGameLogic.beginGame( false );
	m_OfferSessionLogic.sendOfferOrResponse();
}

//============================================================================
void ActivityToFriendTodGame::setupActivityToFriendTodGame( void )
{
	ui.setupUi( this );
	setupBaseWidgets( ui.m_TitleBarWidget, ui.m_FriendIdentWidget, ui.m_PermissionButton, ui.m_PermissionLabel );
	ui.m_TitleBarWidget->setTitleBarText( tr("Truth Or Dare Game") );
	ui.m_TitleBarWidget->enableAudioControls( true );
    connectBarWidgets();
	m_TodGameLogic.setGuiWidgets( m_HisIdent, ui.m_TodGameWidget );
	ui.m_InstMsgWidget->setInstMsgWidgets( m_ePluginType, m_HisIdent );

	ui.m_TodGameWidget->getVidWidget()->setVideoFeedId( m_HisIdent->getMyOnlineId() );
	ui.m_TodGameWidget->getVidWidget()->setRecordFilePath( VxGetDownloadsDirectory().c_str() );
	ui.m_TodGameWidget->getVidWidget()->setRecordFriendName( m_HisIdent->getOnlineName() );

	setVidCamWidget( ui.m_TodGameWidget->getVidWidget() );
	connect( this, SIGNAL(signalToGuiSetGameValueVar(long,long)),	&m_TodGameLogic,	SLOT(slotToGuiSetGameValueVar(long,long)) );
	connect( this, SIGNAL(signalToGuiSetGameActionVar(long,long)),	&m_TodGameLogic,	SLOT(slotToGuiSetGameActionVar(long,long)) );
}

//============================================================================
void ActivityToFriendTodGame::doToGuiRxedPluginOffer( void * callbackData, GuiOfferSession * offer )
{
	m_OfferSessionLogic.doToGuiRxedPluginOffer( offer );
}

//============================================================================
void ActivityToFriendTodGame::doToGuiRxedOfferReply( void * callbackData, GuiOfferSession * offerSession )
{
	m_OfferSessionLogic.doToGuiRxedOfferReply( offerSession );

}

//============================================================================
//! called by base class with in session state
void ActivityToFriendTodGame::onInSession( bool isInSession )
{
	if( isInSession )
	{
		//setStatusText( tr( "In Truth Or Dare Session" ) );
		m_TodGameLogic.beginGame( ! m_OfferSessionLogic.isRmtInitiated() );
		//m_Engine.fromGuiStartPluginSession( VxNetIdent * netIdent, void * pvUserData )
	}
	//else
	//{
	//	setStatusText( tr( "Truth Or Dare Session Ended" ) );
	//}
}

//============================================================================
//! called after session end or dialog exit
void ActivityToFriendTodGame::onEndSession( void )
{

}

//============================================================================
void ActivityToFriendTodGame::toGuiInstMsg( void * callbackData, VxNetIdent * friendIdent, EPluginType ePluginType, QString instMsg )
{
	Q_UNUSED( callbackData );
	if( ( ePluginType == m_ePluginType )
		&& m_HisIdent 
		&& ( m_HisIdent->getMyOnlineId() == friendIdent->getMyOnlineId() ) )
	{
		ui.m_InstMsgWidget->toGuiInstMsg( instMsg );
	}
}; 
