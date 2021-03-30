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
#include <app_precompiled_hdr.h>
#include "AppletPeerTodGame.h"
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
AppletPeerTodGame::AppletPeerTodGame( AppCommon& app, QWidget* parent )
: AppletPeerBase( OBJNAME_ACTIVITY_TO_FRIEND_TOD_GAME, app, parent )
, m_TodGameLogic( app, app.getEngine(), ePluginTypeTruthOrDare, this )
{
    setPluginType( ePluginTypeTruthOrDare );
    setAppletType( eAppletPeerTodGame );
    ui.setupUi( getContentItemsFrame() );
    setTitleBarText( DescribeApplet( m_EAppletType ) );

    //setHisIdent( netIdent );

	setupAppletPeerTodGame();
	//m_OfferSessionLogic.sendOfferOrResponse();
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
	// m_TodGameLogic.setGameStatus( eTxedOffer );
}

//============================================================================
void AppletPeerTodGame::setupAppletPeerTodGame( void )
{
	//setupBaseWidgets( ui.m_TitleBarWidget, ui.m_FriendIdentWidget, ui.m_PermissionButton, ui.m_PermissionLabel );
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
void AppletPeerTodGame::doToGuiRxedPluginOffer( void * callbackData, GuiOfferSession * offer )
{
	m_OfferSessionLogic.doToGuiRxedPluginOffer( offer );
}

//============================================================================
void AppletPeerTodGame::doToGuiRxedOfferReply( void * callbackData, GuiOfferSession * offerSession )
{
	m_OfferSessionLogic.doToGuiRxedOfferReply( offerSession );

}

//============================================================================
//! called by base class with in session state
void AppletPeerTodGame::onInSession( bool isInSession )
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
void AppletPeerTodGame::onEndSession( void )
{

}

//============================================================================
void AppletPeerTodGame::toGuiInstMsg( void * callbackData, GuiUser * friendIdent, EPluginType ePluginType, QString instMsg )
{
	Q_UNUSED( callbackData );
	if( ( ePluginType == m_ePluginType )
		&& m_HisIdent 
		&& ( m_HisIdent->getMyOnlineId() == friendIdent->getMyOnlineId() ) )
	{
		ui.m_InstMsgWidget->toGuiInstMsg( instMsg );
	}
}; 
