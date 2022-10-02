//============================================================================
// Copyright (C) 2016 Brett R. Jones
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


#include "TodGameLogic.h"
#include "AppCommon.h"
#include "MyIcons.h"
#include "AppGlobals.h"

#include <ptop_src/ptop_engine_src/P2PEngine/P2PEngine.h>
#include <ptop_src/ptop_engine_src/P2PEngine/EngineSettings.h>

namespace
{
	#define GAME_SETTINGS_KEY "TODGAME"
}

//============================================================================
TodGameLogic::TodGameLogic( AppCommon& myApp, P2PEngine& engine, EPluginType pluginType, QWidget* parent )
: QWidget( parent )
, m_MyApp( myApp )
, m_Engine( engine )
, m_ePluginType( pluginType )
, m_MySettings( myApp.getEngine().getEngineSettings() )
, m_HisIdent( 0 )
, m_TodGameWidget( 0 )
, m_eGameStatus( eWaitingForChallengeRx )
, m_IsChallenger( false )
{
}

//============================================================================
void TodGameLogic::setGuiWidgets(	GuiUser*	hisIdent,
									TodGameWidget *	todGameWidget )
{
	m_HisIdent			= hisIdent;
	m_TodGameWidget		= todGameWidget;
	loadMyGameStats();
	updateMyStats();

	connect( m_TodGameWidget->ui.m_TruthButton,	SIGNAL(clicked()), this, SLOT(slotTruthButtonClicked()) );
	connect( m_TodGameWidget->ui.m_DareButton,	SIGNAL(clicked()), this, SLOT(slotDareButtonClicked()) );
}

//============================================================================
void TodGameLogic::setVisible( bool visible )
{
	if( 0 != m_TodGameWidget )
	{
		m_TodGameWidget->setVisible( visible );
	}

	QWidget::setVisible( visible );
}

//============================================================================
void TodGameLogic::showChallengeStatus( bool showChallenge )
{
	m_TodGameWidget->ui.m_ChallengeImageLabel->setVisible( showChallenge );
	m_TodGameWidget->ui.m_ChallengeTextLabel->setVisible( showChallenge );
}

//============================================================================
void TodGameLogic::setChallengeStatus( QString statusIcon, QString statusText )
{
	m_TodGameWidget->ui.m_ChallengeImageLabel->setResourceImage( statusIcon );
	m_TodGameWidget->ui.m_ChallengeTextLabel->setText( statusText );
}

//============================================================================
void TodGameLogic::loadMyGameStats( void )
{
	VxNetIdent* myIdent = m_MyApp.getMyIdentity();
	//int32_t val;
	//m_MySettings.getIniValue( GAME_SETTINGS_KEY, "eTodGameVarIdDareChallengeCnt", val );
	m_TodGameStats.m_MyPlayerStats.setVar( eTodGameVarIdDareChallengeCnt, myIdent->getDareCount() + myIdent->getRejectedDareCount() );
	//m_MySettings.getIniValue( GAME_SETTINGS_KEY, "eTodGameVarIdDareAcceptedCnt", val );
	m_TodGameStats.m_MyPlayerStats.setVar( eTodGameVarIdDareAcceptedCnt, myIdent->getDareCount() );
	//m_MySettings.getIniValue( GAME_SETTINGS_KEY, "eTodGameVarIdDareRejectedCnt", val );
	m_TodGameStats.m_MyPlayerStats.setVar( eTodGameVarIdDareRejectedCnt, myIdent->getRejectedDareCount() );
	//m_MySettings.getIniValue( GAME_SETTINGS_KEY, "eTodGameVarIdTruthChallengeCnt", val );
	m_TodGameStats.m_MyPlayerStats.setVar( eTodGameVarIdTruthChallengeCnt, myIdent->getTruthCount() + myIdent->getRejectedTruthCount() );
	//m_MySettings.getIniValue( GAME_SETTINGS_KEY, "eTodGameVarIdTruthAcceptedCnt", val );
	m_TodGameStats.m_MyPlayerStats.setVar( eTodGameVarIdTruthAcceptedCnt, myIdent->getTruthCount() );
	//m_MySettings.getIniValue( GAME_SETTINGS_KEY, "eTodGameVarIdTruthRejectedCnt", val );
	m_TodGameStats.m_MyPlayerStats.setVar( eTodGameVarIdTruthRejectedCnt, myIdent->getRejectedTruthCount() );
}

//============================================================================
void TodGameLogic::saveMyGameStats( void )
{
	VxNetIdent* myIdent = m_MyApp.getAppGlobals().getUserIdent();

	//int32_t val = m_TodGameStats.m_MyPlayerStats.getVar( eTodGameVarIdDareChallengeCnt);
	//m_MySettings.setIniValue( GAME_SETTINGS_KEY, "eTodGameVarIdDareChallengeCnt", val );
	int32_t val = m_TodGameStats.m_MyPlayerStats.getVar( eTodGameVarIdDareAcceptedCnt);
	//m_MySettings.setIniValue( GAME_SETTINGS_KEY, "eTodGameVarIdDareAcceptedCnt", val );
	myIdent->setDareCount( val );
	int32_t rejectedCnt = m_TodGameStats.m_MyPlayerStats.getVar( eTodGameVarIdDareRejectedCnt);
	//m_MySettings.setIniValue( GAME_SETTINGS_KEY, "eTodGameVarIdDareRejectedCnt", rejectedCnt );
	myIdent->setRejectedDareCount( val );

	//val = m_TodGameStats.m_MyPlayerStats.getVar( eTodGameVarIdTruthChallengeCnt);
	//m_MySettings.setIniValue( GAME_SETTINGS_KEY, "eTodGameVarIdTruthChallengeCnt", val );
	val = m_TodGameStats.m_MyPlayerStats.getVar( eTodGameVarIdTruthAcceptedCnt);
	//m_MySettings.setIniValue( GAME_SETTINGS_KEY, "eTodGameVarIdTruthAcceptedCnt", val );
	myIdent->setTruthCount( val );
	rejectedCnt = m_TodGameStats.m_MyPlayerStats.getVar( eTodGameVarIdTruthRejectedCnt);
	//m_MySettings.setIniValue( GAME_SETTINGS_KEY, "eTodGameVarIdTruthRejectedCnt", val );
	myIdent->setRejectedTruthCount( rejectedCnt );

    m_MyApp.updateMyIdent( myIdent, true );
}

//============================================================================
bool TodGameLogic::sendGameStats( void )
{
	IFromGui& poFromGui = m_Engine.getFromGuiInterface();
	bool bResult = poFromGui.fromGuiSetGameValueVar(	m_ePluginType, 
														m_HisIdent->getMyOnlineId(), 
														eTodGameVarIdDareChallengeCnt, 
														m_TodGameStats.m_MyPlayerStats.getVar( eTodGameVarIdDareChallengeCnt ));
	if( bResult )
	{
		bResult = poFromGui.fromGuiSetGameValueVar(	m_ePluginType, 
													m_HisIdent->getMyOnlineId(), 
													eTodGameVarIdDareAcceptedCnt, 
													m_TodGameStats.m_MyPlayerStats.getVar( eTodGameVarIdDareAcceptedCnt ));
	}
	if( bResult )
	{
		bResult = poFromGui.fromGuiSetGameValueVar(	m_ePluginType, 
													m_HisIdent->getMyOnlineId(), 
													eTodGameVarIdDareRejectedCnt, 
													m_TodGameStats.m_MyPlayerStats.getVar( eTodGameVarIdDareRejectedCnt ));
	}
	if( bResult )
	{
		bResult = poFromGui.fromGuiSetGameValueVar(	m_ePluginType, 
													m_HisIdent->getMyOnlineId(), 
													eTodGameVarIdTruthChallengeCnt, 
													m_TodGameStats.m_MyPlayerStats.getVar( eTodGameVarIdTruthChallengeCnt ));
	}
	if( bResult )
	{
		bResult = poFromGui.fromGuiSetGameValueVar(	m_ePluginType, 
													m_HisIdent->getMyOnlineId(), 
													eTodGameVarIdTruthAcceptedCnt, 
													m_TodGameStats.m_MyPlayerStats.getVar( eTodGameVarIdTruthAcceptedCnt ));
	}
	if( bResult )
	{
		bResult = poFromGui.fromGuiSetGameValueVar(	m_ePluginType, 
													m_HisIdent->getMyOnlineId(), 
													eTodGameVarIdTruthRejectedCnt, 
													m_TodGameStats.m_MyPlayerStats.getVar( eTodGameVarIdTruthRejectedCnt ));
	}
	if( bResult )
	{
		bResult = poFromGui.fromGuiSetGameActionVar(	m_ePluginType, 
														m_HisIdent->getMyOnlineId(), 
														eTodGameActionSendStats, 
														1 );
	}

	if( false == bResult )
	{
		handleUserWentOffline();
	}

	return bResult;
}

//============================================================================
void TodGameLogic::enableGameButton( EGameButton eButton, bool bEnable )
{
	switch( eButton )
	{
	case eGameButtonDare:
		m_TodGameWidget->ui.m_DareButton->setVisible( bEnable );
		break;
	case eGameButtonTruth:
		m_TodGameWidget->ui.m_TruthButton->setVisible( bEnable );
		break;
	default:
		break;
	}
}
//============================================================================
void TodGameLogic::setGameButtonText( EGameButton eButton, QString strText )
{
	switch( eButton )
	{
	case eGameButtonDare:
		m_TodGameWidget->ui.m_DareButton->setText( strText );
		break;
	case eGameButtonTruth:
		m_TodGameWidget->ui.m_TruthButton->setText( strText );
		break;
	default:
		break;
	}
}

//============================================================================
void TodGameLogic::setGameStatus( EGameStatus eGameStatus )
{
	m_eGameStatus = eGameStatus;
	switch( m_eGameStatus )
	{
	case eTxedOffer:
		enableGameButton( eGameButtonDare, false );
		enableGameButton( eGameButtonTruth, false );
		setTodStatusMsg( "Waiting for Offer Response" );
		showChallengeStatus( true );
		setChallengeStatus( ":/AppRes/Resources/face18.png", QObject::tr( "Waiting for Offer Response" ) );
		break;

	case eOfferRejected:
		enableGameButton( eGameButtonDare, false );
		enableGameButton( eGameButtonTruth, false );
		setTodStatusMsg( "Offer Rejected" );
		showChallengeStatus( true );
		setChallengeStatus( ":/AppRes/Resources/face18.png", QObject::tr( "Offer Was Rejected" ) );
		break;

		// WAITING
	case eWaitingForChallengeRx:
		setGameButtonText( eGameButtonDare, QObject::tr("Waiting for Challenge") );
		enableGameButton( eGameButtonDare, false );
		setGameButtonText( eGameButtonTruth, QObject::tr("Waiting for Challenge") );
		enableGameButton( eGameButtonTruth, false );
		setTodStatusMsg( QObject::tr("Waiting for Challenge Truth Or Dare") );
		showChallengeStatus( true );
		setChallengeStatus( ":/AppRes/Resources/face18.png", QObject::tr("Wait for Challenge") );
		break;

	case eWaitingForChallengeTx:
		setGameButtonText( eGameButtonDare, QObject::tr("Send Challenge Dare") );
		enableGameButton( eGameButtonDare, true );
		setGameButtonText( eGameButtonTruth, QObject::tr("Send Challenge Truth") );
		enableGameButton( eGameButtonTruth, true );
		setTodStatusMsg( QObject::tr("Press Challenge Truth Button Or Challenge Dare Button") );
		showChallengeStatus( false );
		break;

		// RXED
		// dare
	case eRxedDareChallenge:
		m_MyApp.playSound( eSndDefUserBellMessage );
		enableGameButton( eGameButtonDare, false );
		enableGameButton( eGameButtonTruth, false );
		setTodStatusMsg( m_HisIdent->getOnlineName().c_str() + QObject::tr(" Dares You"));
		setGameButtonText( eGameButtonDare, QObject::tr("Perform Dare") );
		setGameButtonText( eGameButtonTruth, QObject::tr("Perform Dare") );
		m_TodGameStats.m_MyPlayerStats.m_s32DareChallengeCnt++;
		updateMyStats();
		sendGameStats();
		saveMyGameStats();
		showChallengeStatus( true );
		setChallengeStatus( ":/AppRes/Resources/face15.png", QObject::tr("Perform Dare") );
		break;

	case eRxedDareAccepted:
		m_MyApp.playSound( eSndDefYes );
		enableGameButton( eGameButtonDare, true );
		setGameButtonText( eGameButtonDare, QObject::tr("Send Challenge Dare") );
		enableGameButton( eGameButtonTruth, true );
		setGameButtonText( eGameButtonTruth, QObject::tr("Send Challenge Truth") );
		setTodStatusMsg( m_HisIdent->getOnlineName().c_str() + QObject::tr(" has given you 1 Dare Point"));
		m_TodGameStats.m_MyPlayerStats.m_s32DareAcceptedCnt++;
		updateMyStats();
		sendGameStats();
		saveMyGameStats();
		showChallengeStatus( false );
		break;

	case eRxedDareRejected:
		m_MyApp.playSound( eSndDefOfferRejected );
		enableGameButton( eGameButtonDare, true );
		setGameButtonText( eGameButtonDare, QObject::tr("Challenge Dare") );
		enableGameButton( eGameButtonTruth, true );
		setGameButtonText( eGameButtonTruth, QObject::tr("Challenge Truth") );
		setTodStatusMsg( m_HisIdent->getOnlineName().c_str() + QObject::tr( " REJECTED your Dare performance" ) );
		m_TodGameStats.m_MyPlayerStats.m_s32DareRejectedCnt++;
		updateMyStats();
		sendGameStats();
		saveMyGameStats();
		showChallengeStatus( false );
		break;

		// truth
	case eRxedTruthChallenge:
		m_MyApp.playSound( eSndDefUserBellMessage );
		enableGameButton( eGameButtonDare, false );
		enableGameButton( eGameButtonTruth, false );
		setTodStatusMsg(  m_HisIdent->getOnlineName().c_str() + QObject::tr( " Says tell truth" ) );
		setGameButtonText( eGameButtonDare, QObject::tr("Tell Truth") );
		setGameButtonText( eGameButtonTruth, QObject::tr("Tell Truth") );
		m_TodGameStats.m_MyPlayerStats.m_s32TruthChallengeCnt++;
		updateMyStats();
		sendGameStats();
		saveMyGameStats();
		showChallengeStatus( true );
		setChallengeStatus( ":/AppRes/Resources/face20.png", QObject::tr("Tell Truth") );
		break;

	case eRxedTruthAccepted:
		m_MyApp.playSound( eSndDefYes );
		enableGameButton( eGameButtonDare, true );
		setGameButtonText( eGameButtonDare, QObject::tr("Send Challenge Dare") );
		enableGameButton( eGameButtonTruth, true );
		setGameButtonText( eGameButtonTruth, QObject::tr("Send Challenge Truth") );
		setTodStatusMsg( m_HisIdent->getOnlineName().c_str() + QObject::tr(" has given you 1 Truth Point" ));
		m_TodGameStats.m_MyPlayerStats.m_s32TruthAcceptedCnt++;
		updateMyStats();
		sendGameStats();
		saveMyGameStats();
		showChallengeStatus( false );
		break;

	case eRxedTruthRejected:
		m_MyApp.playSound( eSndDefOfferRejected );
		enableGameButton( eGameButtonDare, true );
		setGameButtonText( eGameButtonDare, tr("Challenge Dare") );
		enableGameButton( eGameButtonTruth, true );
		setGameButtonText( eGameButtonTruth, tr("Challenge Truth") );
		setTodStatusMsg( m_HisIdent->getOnlineName().c_str() + QObject::tr(" REJECTED your Truth speech") );
		m_TodGameStats.m_MyPlayerStats.m_s32TruthRejectedCnt++;
		updateMyStats();
		sendGameStats();
		saveMyGameStats();
		showChallengeStatus( false );
		break;

		//TXed
	case eTxedDareChallenge:
		setTodStatusMsg( QString(" Daring ") + m_HisIdent->getOnlineName().c_str() );
		enableGameButton( eGameButtonTruth, true );
		setGameButtonText( eGameButtonTruth, QObject::tr("Dare Performed")  );
		enableGameButton( eGameButtonDare, true );
		setGameButtonText( eGameButtonDare, QObject::tr("Dare NOT Performed")  );
		showChallengeStatus( false );
		break;

	case eTxedTruthChallenge:
		setTodStatusMsg( QString(" Challenging ") + m_HisIdent->getOnlineName().c_str() + QObject::tr(" to tell truth") );
		enableGameButton( eGameButtonTruth, true );
		setGameButtonText( eGameButtonTruth, QObject::tr("User Told Truth") );
		enableGameButton( eGameButtonDare, true );
		setGameButtonText( eGameButtonDare, QObject::tr( "Failed Tell Truth" ) );
		showChallengeStatus( false );
		break;

	default:
		break;
	}
}

//============================================================================
void TodGameLogic::setTodStatusMsg( QString strStatus )
{
	m_TodGameWidget->ui.m_TodStatusLabel->setText( strStatus );
}

//============================================================================
void TodGameLogic::setGameValueVar( long s32VarId, long s32VarValue )
{
	m_TodGameStats.m_FriendPlayerStats.setVar( (ETodGameVarId)s32VarId, s32VarValue );
	updateFriendStats();
}

//============================================================================
void TodGameLogic::setGameActionVar( long s32VarId, long s32VarValue )
{
	switch(s32VarId)
	{
	case eTodGameActionChallengeDare:
		setGameStatus( eRxedDareChallenge );
		break;

	case eTodGameActionDareAccepted:
		setGameStatus( eRxedDareAccepted );
		break;

	case eTodGameActionDareRejected:
		setGameStatus( eRxedDareRejected );
		break;

	case eTodGameActionChallengeTruth:
		setGameStatus( eRxedTruthChallenge );
		break;

	case eTodGameActionTruthAccepted:
		setGameStatus( eRxedTruthAccepted );
		break;

	case eTodGameActionTruthRejected:
		setGameStatus( eRxedTruthRejected );
		break;
	}
}

//============================================================================
void TodGameLogic::updateMyStats( void )
{
	m_TodGameWidget->ui.MyDaresLabel->setText(QString( tr(" My Dares Accepted %1 Rejected %2 Challenges %3") ).
		arg(m_TodGameStats.m_MyPlayerStats.getVar(eTodGameVarIdDareAcceptedCnt)).
		arg(m_TodGameStats.m_MyPlayerStats.getVar(eTodGameVarIdDareRejectedCnt)).
		arg(m_TodGameStats.m_MyPlayerStats.getVar(eTodGameVarIdDareChallengeCnt)) );

	m_TodGameWidget->ui.MyTruthsLabel->setText(QString( tr(" My Truths Accepted %1 Rejected %2 Challenges %3") ).
		arg(m_TodGameStats.m_MyPlayerStats.getVar(eTodGameVarIdTruthAcceptedCnt)).
		arg(m_TodGameStats.m_MyPlayerStats.getVar(eTodGameVarIdTruthRejectedCnt)).
		arg(m_TodGameStats.m_MyPlayerStats.getVar(eTodGameVarIdTruthChallengeCnt)) );
}

//============================================================================
void TodGameLogic::updateFriendStats( void )
{
	m_TodGameWidget->ui.FriendDaresLabel->setText(QString( tr(" %1 Dares Accepted %2 Rejected %3 Challenges %4") ).
		arg(m_HisIdent->getOnlineName().c_str() ).
		arg(m_TodGameStats.m_FriendPlayerStats.getVar(eTodGameVarIdDareChallengeCnt)).
		arg(m_TodGameStats.m_FriendPlayerStats.getVar(eTodGameVarIdDareAcceptedCnt)).
		arg(m_TodGameStats.m_FriendPlayerStats.getVar(eTodGameVarIdDareRejectedCnt)) );

	m_TodGameWidget->ui.FriendTruthsLabel->setText(QString( tr(" %1 Truths Accepted %2 Rejected %3 Challenges %4") ).
		arg(m_HisIdent->getOnlineName().c_str() ).
		arg(m_TodGameStats.m_FriendPlayerStats.getVar(eTodGameVarIdTruthAcceptedCnt)).
		arg(m_TodGameStats.m_FriendPlayerStats.getVar(eTodGameVarIdTruthRejectedCnt)).
		arg(m_TodGameStats.m_FriendPlayerStats.getVar(eTodGameVarIdTruthChallengeCnt)) );
}

//============================================================================
void TodGameLogic::slotTruthButtonClicked( void )
{
	switch( m_eGameStatus )
	{
	case eWaitingForChallengeRx:
		break;
	case eRxedDareChallenge:
		break;
	case eRxedDareAccepted:
	case eRxedDareRejected:
	case eRxedTruthChallenge:
	case eRxedTruthAccepted:
	case eRxedTruthRejected:
	case eWaitingForChallengeTx:
		fromGuiSetGameActionVar( eTodGameActionChallengeTruth, 1);
		setGameStatus( eTxedTruthChallenge );
		break;

	case eTxedDareChallenge:
		fromGuiSetGameActionVar( eTodGameActionDareAccepted, 1);
		setGameStatus( eWaitingForChallengeRx );
		break;

	case eTxedDareAccepted:
		break;

	case eTxedDareRejected:
		break;

	case eTxedTruthChallenge:
		fromGuiSetGameActionVar( eTodGameActionTruthAccepted, 1);
		setGameStatus( eWaitingForChallengeRx );
		break;

	case eTxedTruthAccepted:
		break;

	case eTxedTruthRejected:
	default:
		break;
	};
}

//============================================================================
void TodGameLogic::slotDareButtonClicked( void )
{
	switch( m_eGameStatus )
	{
	case eWaitingForChallengeRx:
		break;

	case eRxedDareChallenge:
	case eRxedDareAccepted:
	case eRxedDareRejected:
	case eRxedTruthChallenge:
	case eRxedTruthAccepted:
	case eRxedTruthRejected:
	case eWaitingForChallengeTx:
		fromGuiSetGameActionVar( eTodGameActionChallengeDare, 1);
		setGameStatus( eTxedDareChallenge );
		break;

	case eTxedDareChallenge:
		fromGuiSetGameActionVar( eTodGameActionDareRejected, 1);
		setGameStatus( eWaitingForChallengeRx );
		break;

	case eTxedDareAccepted:
		break;

	case eTxedDareRejected:
		break;

	case eTxedTruthChallenge:
		fromGuiSetGameActionVar( eTodGameActionTruthRejected, 1);
		setGameStatus( eWaitingForChallengeRx );
		break;

	case eTxedTruthAccepted:
		break;

	case eTxedTruthRejected:
	default:
		break;
	};
}

//============================================================================
void TodGameLogic::slotToGuiSetGameValueVar( long s32VarId, long s32VarValue )
{
	m_TodGameStats.m_FriendPlayerStats.setVar( (ETodGameVarId)s32VarId, s32VarValue );
	updateFriendStats();
}

//============================================================================
void TodGameLogic::slotToGuiSetGameActionVar( long s32VarId, long s32VarValue )
{
	switch(s32VarId)
	{
	case eTodGameActionChallengeDare:
		setGameStatus( eRxedDareChallenge );
		break;

	case eTodGameActionDareAccepted:
		setGameStatus( eRxedDareAccepted );
		break;

	case eTodGameActionDareRejected:
		setGameStatus( eRxedDareRejected );
		break;

	case eTodGameActionChallengeTruth:
		setGameStatus( eRxedTruthChallenge );
		break;

	case eTodGameActionTruthAccepted:
		setGameStatus( eRxedTruthAccepted );
		break;

	case eTodGameActionTruthRejected:
		setGameStatus( eRxedTruthRejected );
		break;

	default:
		break;
	}
}

//============================================================================
bool TodGameLogic::fromGuiSetGameValueVar(	int32_t s32VarId, int32_t s32VarValue )
{
	bool bResult = m_Engine.fromGuiSetGameValueVar( 
		m_ePluginType, 
		m_HisIdent->getMyOnlineId(),
		s32VarId,
		s32VarValue );
	if( false == bResult )
	{
		handleUserWentOffline();
	}

	return bResult;
}

//============================================================================
bool TodGameLogic::fromGuiSetGameActionVar(	int32_t s32VarId, int32_t s32VarValue )
{
	bool bResult = m_Engine.fromGuiSetGameActionVar( 
		m_ePluginType, 
		m_HisIdent->getMyOnlineId(),
		s32VarId,
		s32VarValue );
	if( false == bResult )
	{
		handleUserWentOffline();
	}

	return bResult;
}

//============================================================================
void TodGameLogic::handleUserWentOffline( void )
{
	emit signalUserWentOffline();
}

//============================================================================
bool TodGameLogic::beginGame( bool isChallenger )
{
	m_IsChallenger = isChallenger;
	setGameStatus( m_IsChallenger ? eWaitingForChallengeRx : eWaitingForChallengeTx );
	bool result = sendGameStats();
	if( result )
	{

	}

	return result;
}

//============================================================================
void TodGameLogic::endGame( void )
{
	saveMyGameStats();
	setGameStatus( m_IsChallenger ? eWaitingForChallengeRx : eWaitingForChallengeTx );
}
