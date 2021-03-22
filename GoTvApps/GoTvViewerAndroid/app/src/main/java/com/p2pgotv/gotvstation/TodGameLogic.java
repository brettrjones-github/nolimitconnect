//============================================================================
// Copyright (C) 2015 Brett R. Jones
// All Rights Reserved ( After My Death I Brett R. Jones Issue this source code to Public Domain )
//
// You may not modify or redistribute this code for any reason
// without written consent of Brett R. Jones
//
// This code is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
//
// brett1900@usa.com
// http://www.gotvp2p.net
//============================================================================

package com.p2pgotv.gotvstation;

import com.p2pgotv.gotvstation.AppletPeerTodGame.EGameAction;
import com.p2pgotv.gotvstation.AppletPeerTodGame.EGameButton;
import com.p2pgotv.gotvstation.AppletPeerTodGame.EGameStatus;

import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.ImageView;
import android.widget.LinearLayout;
import android.widget.RelativeLayout;
import android.widget.TextView;

class TodGameLogic
{
//	private static final String LOG_TAG 					= "TodGameLogic:";
	private static final String  GAME_SETTINGS_KEY  		= "TODGAME";	
	
	static public final int eWaitingForChallengeRx 			= 0;
	static public final int eRxedDareChallenge 				= 1;
	static public final int eRxedDareAccepted 				= 2;
	static public final int eRxedDareRejected 				= 3;
	static public final int eRxedTruthChallenge 			= 4;
	static public final int eRxedTruthAccepted 				= 5;
	static public final int eRxedTruthRejected 				= 6;

	static public final int eWaitingForChallengeTx 			= 7;
	static public final int eTxedDareChallenge 				= 8;
	static public final int eTxedDareAccepted 				= 9;
	static public final int eTxedDareRejected 				= 10;
	static public final int eTxedTruthChallenge 			= 11;
	static public final int eTxedTruthAccepted 				= 12;
	static public final int eTxedTruthRejected 				= 13;
	static public final int eTxedOffer 						= 14;
	static public final int eMaxGameStatus 					= 15; // must be last

	static public final int eTodGameActionSendStats 		= 0;
	static public final int eTodGameActionChallengeDare 	= 1;
	static public final int eTodGameActionDareAccepted 		= 2;
	static public final int eTodGameActionDareRejected 		= 3;
	static public final int eTodGameActionChallengeTruth 	= 4;
	static public final int eTodGameActionTruthAccepted 	= 5;
	static public final int eTodGameActionTruthRejected 	= 6;
	static public final int eMaxTodGameAction 				= 7; // must be last

	static public final int eGameButtonDare 				= 0;
	static public final int eGameButtonTruth	 			= 1;
	static public final int eMaxGameButton 					= 2;
	
	//=== vars ===//
	private	MyApp				m_MyApp						= null;
	private int 				m_ePluginType				= Constants.ePluginTypeInvalid;
	private VxNetIdent 			m_HisIdent 					= null;
	
	private MySettings 			m_SettingsDb 				= null;
	private VxSettings 			m_MySettings 				= null;
	private	TodPlayerStats		m_oMyPlayerStats			= new TodPlayerStats();
	private	TodPlayerStats		m_HisIdentPlayerStats 		= new TodPlayerStats();
	
	private int 				m_eGameStatus 				= 0;
	
  	private LinearLayout 		m_TodGameLayout 			= null;
	private Button				m_oDareButton 				= null;
	private Button				m_oTruthButton 				= null;
	
	private TextView			m_oMyDaresLabel 			= null;
	private TextView			m_oMyTruthsLabel 			= null;
	private TextView			m_HisIdentDaresLabel 		= null;
	private TextView			m_HisIdentTruthsLabel 		= null;
	private TextView			m_TodStatusLabel 			= null;
	private ImageView			m_TodChallengeImage 		= null;
	private TextView			m_TodChallengeLabel 		= null;


  	private RelativeLayout 		m_VidCamLayout 				= null;
	private ImageViewWithSizer 	m_VidCamView				= null;
	private ImageViewWithSizer 	m_VidCamThumbnail			= null;
	private boolean				m_IsChallenger				= false;

	//============================================================================
	TodGameLogic( 	MyApp 					myApp, 
					int 					pluginType,
					VxNetIdent				hisIdent,
					LinearLayout 			todGameLayout,
					Button					dareButton,
					Button					truthButton,
					TextView				myDaresLabel,
					TextView				myTruthsLabel,
					TextView				friendDaresLabel,
					TextView				friendTruthsLabel,
					TextView				todStatusLabel,
					ImageView				todChallengeImage,
					TextView				todChallengeLabel,
					RelativeLayout 			vidCamLayout,
					ImageViewWithSizer 		vidCamView,
					ImageViewWithSizer 		vidCamThumbnail )
	{
		m_MyApp 							= myApp;
		m_SettingsDb 						= m_MyApp.getMySettings();
        m_MySettings 						= m_SettingsDb.getVxSettings();
        
		m_ePluginType 						= pluginType;
		m_HisIdent 							= hisIdent;
		m_TodGameLayout						= todGameLayout;
		m_oDareButton						= dareButton;
		m_oTruthButton						= truthButton;
		m_oMyDaresLabel						= myDaresLabel;
		m_oMyTruthsLabel					= myTruthsLabel;
		m_HisIdentDaresLabel				= friendDaresLabel;
		m_HisIdentTruthsLabel				= friendTruthsLabel;
		m_TodStatusLabel					= todStatusLabel;
		m_TodChallengeImage					= todChallengeImage;
		m_TodChallengeLabel					= todChallengeLabel;
		m_VidCamLayout						= vidCamLayout;
		m_VidCamView 						= vidCamView;
		m_VidCamThumbnail 					= vidCamThumbnail;
		
        if( null != m_oDareButton )
        {
        	m_oDareButton.setOnClickListener(new OnClickListener() {
	            public void onClick(View v) 
	            {
	    	        m_MyApp.playButtonClick();
	            	onDareButtonClicked();
	            }
	          });     
        }
  
        if( null != m_oTruthButton )
        {
        	m_oTruthButton.setOnClickListener(new OnClickListener() {
	            public void onClick(View v) 
	            {
	    	        m_MyApp.playButtonClick();
	            	onTruthButtonClicked();
	            }
	          });     
        }
        
    	loadMyGameStats();
    	updateMyStats();
	}	
	
	//======================================================================== 
    public void showChallengeStatus( boolean showChallenge )
    {
    	int visibleMode = View.GONE;
    	if( showChallenge )
    	{
    		visibleMode = View.VISIBLE;
    	}
   	
    	m_TodChallengeImage.setVisibility(visibleMode);
    	m_TodChallengeLabel.setVisibility(visibleMode);
   }
    
	//======================================================================== 
    public void setChallengeStatus( int statusIcon, String statusText )
    {
    	m_TodChallengeImage.setBackgroundResource( statusIcon );   	
    	m_TodChallengeLabel.setText(  statusText );
    }
   
	//======================================================================== 
    public void showTodGame( boolean showWidgets )
    {
    	int visibleMode = View.GONE;
    	if( showWidgets )
    	{
    		visibleMode = View.VISIBLE;
    	}
    	
    	m_TodGameLayout.setVisibility(visibleMode);
        m_oMyDaresLabel.setVisibility(visibleMode);
        m_oMyTruthsLabel.setVisibility(visibleMode);
        m_HisIdentDaresLabel.setVisibility(visibleMode);
        m_HisIdentTruthsLabel.setVisibility(visibleMode);
        m_TodStatusLabel.setVisibility(visibleMode);
    	m_oDareButton.setVisibility(visibleMode);
    	m_oTruthButton.setVisibility(visibleMode);	
     }

	//============================================================================
	void loadMyGameStats()
	{
	  	int defaultVal = 0;
	  	//int val;
		//val = m_MySettings.getIniValueInt( GAME_SETTINGS_KEY, "eTodGameVarIdDareAcceptedCnt", defaultVal );
		m_oMyPlayerStats.setVar( TodPlayerStats.eTodGameVarIdDareAcceptedCnt, m_MyApp.getUserAccount().getDareCount() );
		//val = m_MySettings.getIniValueInt( GAME_SETTINGS_KEY, "eTodGameVarIdDareRejectedCnt", m_MyApp.getUserAccount().getDareRejectedCount() );
		m_oMyPlayerStats.setVar( TodPlayerStats.eTodGameVarIdDareRejectedCnt, m_MyApp.getUserAccount().getRejectedDareCount() );
	  	//val = m_MySettings.getIniValueInt( GAME_SETTINGS_KEY, "eTodGameVarIdDareChallengeCnt", m_MyApp.getUserAccount().getDareCount() + m_MyApp.getUserAccount().getDareRejectedCount() );
		m_oMyPlayerStats.setVar( TodPlayerStats.eTodGameVarIdDareChallengeCnt, m_MyApp.getUserAccount().getDareCount() + m_MyApp.getUserAccount().getRejectedDareCount() );

		//val = m_MySettings.getIniValueInt( GAME_SETTINGS_KEY, "eTodGameVarIdTruthAcceptedCnt", defaultVal );
		m_oMyPlayerStats.setVar( TodPlayerStats.eTodGameVarIdTruthAcceptedCnt, m_MyApp.getUserAccount().getTruthCount() );
		//val = m_MySettings.getIniValueInt( GAME_SETTINGS_KEY, "eTodGameVarIdTruthRejectedCnt", defaultVal );
	  	m_oMyPlayerStats.setVar( TodPlayerStats.eTodGameVarIdTruthRejectedCnt, m_MyApp.getUserAccount().getRejectedTruthCount() );
		//val = m_MySettings.getIniValueInt( GAME_SETTINGS_KEY, "eTodGameVarIdTruthChallengeCnt", defaultVal );
		m_oMyPlayerStats.setVar( TodPlayerStats.eTodGameVarIdTruthChallengeCnt, m_MyApp.getUserAccount().getTruthCount() + m_MyApp.getUserAccount().getRejectedTruthCount() );
	}
	
	//============================================================================
	void saveMyGameStats()
	{
		UserAccount myIdent = m_MyApp.getUserAccount();
		
	  	//int val = m_oMyPlayerStats.getVar( TodPlayerStats.eTodGameVarIdDareChallengeCnt);
	  	//m_MySettings.setIniValue( GAME_SETTINGS_KEY, "eTodGameVarIdDareChallengeCnt", val );
		int val = m_oMyPlayerStats.getVar( TodPlayerStats.eTodGameVarIdDareAcceptedCnt);
		myIdent.setDareCount( val );
		
		//m_MySettings.setIniValue( GAME_SETTINGS_KEY, "eTodGameVarIdDareAcceptedCnt", val );
		int rejectedCnt = m_oMyPlayerStats.getVar( TodPlayerStats.eTodGameVarIdDareRejectedCnt);
		//m_MySettings.setIniValue( GAME_SETTINGS_KEY, "eTodGameVarIdDareRejectedCnt", rejectedCnt );
		myIdent.setRejectedDareCount( val );

		//val = m_oMyPlayerStats.getVar( TodPlayerStats.eTodGameVarIdTruthChallengeCnt);
		//m_MySettings.setIniValue( GAME_SETTINGS_KEY, "eTodGameVarIdTruthChallengeCnt", val );
		
		val = m_oMyPlayerStats.getVar( TodPlayerStats.eTodGameVarIdTruthAcceptedCnt);
		myIdent.setTruthCount( val );

		///m_MySettings.setIniValue( GAME_SETTINGS_KEY, "eTodGameVarIdTruthAcceptedCnt", val );
		val = m_oMyPlayerStats.getVar( TodPlayerStats.eTodGameVarIdTruthRejectedCnt);
		myIdent.setRejectedTruthCount( val );
		//m_MySettings.setIniValue( GAME_SETTINGS_KEY, "eTodGameVarIdTruthRejectedCnt", val );
		m_MyApp.saveUserAccountToDatabase( myIdent );
		NativeTxTo.fromGuiUpdateMyIdent( myIdent, true );
	}

  	//============================================================================
  	boolean sendGameStats()
  	{
		boolean bResult = fromGuiSetGameValueVar(	TodPlayerStats.eTodGameVarIdDareChallengeCnt, 
													m_oMyPlayerStats.getVar( TodPlayerStats.eTodGameVarIdDareChallengeCnt ) );
		if( bResult )
		{
			bResult = fromGuiSetGameValueVar(	TodPlayerStats.eTodGameVarIdDareAcceptedCnt, 
												m_oMyPlayerStats.getVar( TodPlayerStats.eTodGameVarIdDareAcceptedCnt ) );
		}
		
		if( bResult )
		{
			bResult = fromGuiSetGameValueVar(	TodPlayerStats.eTodGameVarIdDareRejectedCnt, 
												m_oMyPlayerStats.getVar( TodPlayerStats.eTodGameVarIdDareRejectedCnt ) );
		}
		
		if( bResult )
		{
			bResult = fromGuiSetGameValueVar(	TodPlayerStats.eTodGameVarIdTruthChallengeCnt, 
												m_oMyPlayerStats.getVar( TodPlayerStats.eTodGameVarIdTruthChallengeCnt ) );
		}
		
		if( bResult )
		{
			bResult = fromGuiSetGameValueVar(	TodPlayerStats.eTodGameVarIdTruthAcceptedCnt, 
												m_oMyPlayerStats.getVar( TodPlayerStats.eTodGameVarIdTruthAcceptedCnt ) );
		}
		
		if( bResult )
		{
			bResult = fromGuiSetGameValueVar(	TodPlayerStats.eTodGameVarIdTruthRejectedCnt, 
												m_oMyPlayerStats.getVar( TodPlayerStats.eTodGameVarIdTruthRejectedCnt ) );
		}
		
		if( bResult )
		{
			bResult = fromGuiSetGameActionVar(	EGameAction.eTodGameActionSendStats, 1 );
		}
		
		if( false == bResult )
		{
			handleUserWentOffline();
		}
		
		return bResult;
  	}

  	//============================================================================
  	void enableGameButton( int eButton, boolean bEnable )
  	{
	  	switch( eButton )
	  	{
	  	case EGameButton.eGameButtonDare:
	  		setButtonVisibility( m_oDareButton, bEnable );
	  		break;
	  		
	  	case EGameButton.eGameButtonTruth:
	  		setButtonVisibility( m_oTruthButton, bEnable );
	  		break;
	  	}
  	}
  	
  	//============================================================================
 	void setButtonVisibility( Button button, boolean enable )
 	{
 		if( enable )
 		{
 			button.setVisibility( View.VISIBLE );
 		}
 		else
 		{
 			button.setVisibility( View.GONE );			
 		}
  	}
 	
  	//============================================================================
 	void setGameButtonText( int eButton, String strText )
 	{
	  	switch( eButton )
	  	{
	  	case EGameButton.eGameButtonDare:
	  		m_oDareButton.setText( strText );
	  		break;
	  		
	  	case EGameButton.eGameButtonTruth:
	  		m_oTruthButton.setText( strText );
	  		break;
	  	}
 	}

 	//============================================================================
 	void setGameStatus( int eGameStatus )
 	{
	  	m_eGameStatus = eGameStatus;
	  	switch( m_eGameStatus )
	  	{
	  		// WAITING
	  	case EGameStatus.eTxedOffer:
	  		enableGameButton( EGameButton.eGameButtonDare, false );
	  		enableGameButton( EGameButton.eGameButtonTruth, false );
	  		setStatusMsg( "Waiting for Offer Response" );
			showChallengeStatus( true );
			setChallengeStatus( R.drawable.face18, "Waiting for Offer Response" );
	  		break;

	  	case EGameStatus.eWaitingForChallengeRx:
	  		setGameButtonText( EGameButton.eGameButtonDare, "Waiting for Challenge" );
	  		enableGameButton( EGameButton.eGameButtonDare, false );
	  		setGameButtonText( EGameButton.eGameButtonTruth, "Waiting for Challenge" );
	  		enableGameButton( EGameButton.eGameButtonTruth, false );
	  		setStatusMsg( "Waiting for Challenge Truth Or Dare" );
			showChallengeStatus( true );
			setChallengeStatus( R.drawable.face18, "Wait for Challenge" );
	  		break;
	
	  	case EGameStatus.eWaitingForChallengeTx:
	  		setGameButtonText( EGameButton.eGameButtonDare, "Challenge Dare" );
	  		enableGameButton( EGameButton.eGameButtonDare, true );
	  		setGameButtonText( EGameButton.eGameButtonTruth, "Challenge truth" );
	  		enableGameButton( EGameButton.eGameButtonTruth, true );
	  		setStatusMsg( "Press Challenge Truth Button Or Challenge Dare Button" );
			showChallengeStatus( false );
	  		break;
	
	  		// RXED
	  			// dare
	  	case EGameStatus.eRxedDareChallenge:
	  		m_MyApp.playSound( ESndDef.eSndDefUserBellMessage );
	  		
	  		enableGameButton( EGameButton.eGameButtonDare, false );
	  		enableGameButton( EGameButton.eGameButtonTruth, false );
	  		setStatusMsg( m_HisIdent.getOnlineName() + " Dares You" );
	  		setGameButtonText( EGameButton.eGameButtonDare, "Perform Dare" );
	  		setGameButtonText( EGameButton.eGameButtonTruth, "Perform Dare" );
	  		m_oMyPlayerStats.m_s32DareChallengeCnt++;
	  		updateMyStats();
	  		sendGameStats();
			saveMyGameStats();
			showChallengeStatus( true );
			setChallengeStatus( R.drawable.face15, "Perform Dare" );
	  		break;
	
	  	case EGameStatus.eRxedDareAccepted:
	  		m_MyApp.playSound( ESndDef.eSndDefYes );
	  		
	  		enableGameButton( EGameButton.eGameButtonDare, true );
	  		setGameButtonText( EGameButton.eGameButtonDare, "Challenge Dare" );
	  		enableGameButton( EGameButton.eGameButtonTruth, true );
	  		setGameButtonText( EGameButton.eGameButtonTruth, "Challenge truth" );
	  		setStatusMsg( m_HisIdent.getOnlineName() + " has given you 1 Dare Point" );
	  		m_oMyPlayerStats.m_s32DareAcceptedCnt++;
	  		updateMyStats();
	  		sendGameStats();
			saveMyGameStats();
			showChallengeStatus( false );
	  		break;
	
	  	case EGameStatus.eRxedDareRejected:
	  		m_MyApp.playSound( ESndDef.eSndDefOfferRejected );
	  		
	  		enableGameButton( EGameButton.eGameButtonDare, true );
	  		setGameButtonText( EGameButton.eGameButtonDare, "Challenge Dare" );
	  		enableGameButton( EGameButton.eGameButtonTruth, true );
	  		setGameButtonText( EGameButton.eGameButtonTruth, "Challenge truth" );
	  		setStatusMsg( m_HisIdent.getOnlineName() + " REJECTED your Dare performance" );
	  		m_oMyPlayerStats.m_s32DareRejectedCnt++;
	  		updateMyStats();
	  		sendGameStats();
			saveMyGameStats();
			showChallengeStatus( false );
	  		break;
	  	
	  			// truth
	  	case EGameStatus.eRxedTruthChallenge:
	  		m_MyApp.playSound( ESndDef.eSndDefUserBellMessage );
	  		
	  		enableGameButton( EGameButton.eGameButtonDare, false );
	  		enableGameButton( EGameButton.eGameButtonTruth, false );
	  		setStatusMsg(  m_HisIdent.getOnlineName() + " Says tell truth" );
	  		setGameButtonText( EGameButton.eGameButtonDare, "Tell Truth" );
	  		setGameButtonText( EGameButton.eGameButtonTruth, "Tell Truth" );
	  		m_oMyPlayerStats.m_s32TruthChallengeCnt++;
	  		updateMyStats();
	  		sendGameStats();
			saveMyGameStats();
			showChallengeStatus( true );
			setChallengeStatus( R.drawable.face20, "Tell Truth" );
			break;
	
	  	case EGameStatus.eRxedTruthAccepted:
	  		m_MyApp.playSound( ESndDef.eSndDefYes );
	  		
	  		enableGameButton( EGameButton.eGameButtonDare, true );
	  		setGameButtonText( EGameButton.eGameButtonDare, "Challenge Dare" );
	  		enableGameButton( EGameButton.eGameButtonTruth, true );
	  		setGameButtonText( EGameButton.eGameButtonTruth, "Challenge truth" );
	  		setStatusMsg( m_HisIdent.getOnlineName() + " has given you 1 Truth Point" );
	  		m_oMyPlayerStats.m_s32TruthAcceptedCnt++;
	  		updateMyStats();
	  		sendGameStats();
			saveMyGameStats();
			showChallengeStatus( false );
	  		break;
	
	  	case EGameStatus.eRxedTruthRejected:
	  		m_MyApp.playSound( ESndDef.eSndDefOfferRejected );

	  		enableGameButton( EGameButton.eGameButtonDare, true );
	  		setGameButtonText( EGameButton.eGameButtonDare, "Challenge Dare" );
	  		enableGameButton( EGameButton.eGameButtonTruth, true );
	  		setGameButtonText( EGameButton.eGameButtonTruth, "Challenge Truth" );
	  		setStatusMsg( m_HisIdent.getOnlineName() + " REJECTED your Truth speech");
	  		m_oMyPlayerStats.m_s32TruthRejectedCnt++;
	  		updateMyStats();
	  		sendGameStats();
			saveMyGameStats();
			showChallengeStatus( false );
	  		break;
	
	  		//TXed
	  	case EGameStatus.eTxedDareChallenge:
	  		setStatusMsg( " Daring " + m_HisIdent.getOnlineName() );
	  		enableGameButton( EGameButton.eGameButtonTruth, true );
	  		setGameButtonText( EGameButton.eGameButtonTruth, "Dare Accepted" );
	  		enableGameButton( EGameButton.eGameButtonDare, true );
	  		setGameButtonText( EGameButton.eGameButtonDare, "Dare Rejected" );
	  		showChallengeStatus( false );
	  		break;
	
	  	case EGameStatus.eTxedTruthChallenge:
	  		setStatusMsg( " Challenging " + m_HisIdent.getOnlineName() + " to tell truth" );
	  		enableGameButton( EGameButton.eGameButtonTruth, true );
	  		setGameButtonText( EGameButton.eGameButtonTruth, "User Told Truth" );
	  		enableGameButton( EGameButton.eGameButtonDare, true );
	  		setGameButtonText( EGameButton.eGameButtonDare, "Failed Tell Truth" );
	  		showChallengeStatus( false );
	  		break;
	
	  	default:
	  		break;
	  	}
 	}

 	//============================================================================
 	void setStatusMsg( String strStatus )
 	{
 		m_TodStatusLabel.setText( strStatus );
 	}

 	//============================================================================
 	boolean doGuiSetGameValueVar( int s32VarId, int	s32VarValue )
 	{
 		m_HisIdentPlayerStats.setVar( s32VarId, s32VarValue );
 		updateFriendStats();
 		return true;
 	}

 	//============================================================================
 	boolean doGuiSetGameActionVar( int s32VarId, int s32VarValue )
 	{
	  	switch(s32VarId)
	  	{
	  	case EGameAction.eTodGameActionChallengeDare:
	  		setGameStatus( EGameStatus.eRxedDareChallenge );
	  		break;
	  		
	  	case EGameAction.eTodGameActionDareAccepted:
	  		setGameStatus( EGameStatus.eRxedDareAccepted );
	  		break;
	  		
	  	case EGameAction.eTodGameActionDareRejected:
	  		setGameStatus( EGameStatus.eRxedDareRejected );
	  		break;
	
	  	case EGameAction.eTodGameActionChallengeTruth:
	  		setGameStatus( EGameStatus.eRxedTruthChallenge );
	  		break;
	  		
	  	case EGameAction.eTodGameActionTruthAccepted:
	  		setGameStatus( EGameStatus.eRxedTruthAccepted );
	  		break;
	  		
	  	case EGameAction.eTodGameActionTruthRejected:
	  		setGameStatus( EGameStatus.eRxedTruthRejected );
	  		break;	
	  	}
	  	
	  	return false;
 	}

 	//============================================================================
 	void updateMyStats()
 	{
	  	m_oMyDaresLabel.setText(
	  			" My Dares Accepted " +
	  			m_oMyPlayerStats.getVar(TodPlayerStats.eTodGameVarIdDareAcceptedCnt) + 
	  			" Rejected " +
	  			m_oMyPlayerStats.getVar(TodPlayerStats.eTodGameVarIdDareRejectedCnt) );
	
	  	m_oMyTruthsLabel.setText(
	  			" My Truths Accepted " +
	  			m_oMyPlayerStats.getVar(TodPlayerStats.eTodGameVarIdTruthAcceptedCnt) + 
	  			" Rejected " +
	  			m_oMyPlayerStats.getVar(TodPlayerStats.eTodGameVarIdTruthRejectedCnt) );
	}

 	//============================================================================
 	void updateFriendStats()
 	{
	  	m_HisIdentDaresLabel.setText(
	  			" " +
	  			m_HisIdent.getOnlineName() + 
	  			" Dares Accepted " +
	  			m_oMyPlayerStats.getVar(TodPlayerStats.eTodGameVarIdDareAcceptedCnt) + 
	  			" Rejected " +
	  			m_oMyPlayerStats.getVar(TodPlayerStats.eTodGameVarIdDareRejectedCnt) );

	  	m_HisIdentTruthsLabel.setText(
	  			" " +
	  			m_HisIdent.getOnlineName() + 
	  			" Truths Accepted " +
	  			m_oMyPlayerStats.getVar(TodPlayerStats.eTodGameVarIdTruthAcceptedCnt) + 
	  			" Rejected " +
	  			m_oMyPlayerStats.getVar(TodPlayerStats.eTodGameVarIdTruthRejectedCnt) );
 	}

 	//============================================================================
 	void onTruthButtonClicked()
 	{
	  	switch( m_eGameStatus )
	  	{
	  	case EGameStatus.eWaitingForChallengeRx:
	  		break;
	  		
	  	case EGameStatus.eRxedDareChallenge:
	  		break;
	  		
	  	case EGameStatus.eRxedDareAccepted:
	  	case EGameStatus.eRxedDareRejected:
	  	case EGameStatus.eRxedTruthChallenge:
	  	case EGameStatus.eRxedTruthAccepted:
	  	case EGameStatus.eRxedTruthRejected:
	  	case EGameStatus.eWaitingForChallengeTx:
	  		fromGuiSetGameActionVar( EGameAction.eTodGameActionChallengeTruth, 1);
	  		setGameStatus( EGameStatus.eTxedTruthChallenge );
	  		break;
	  		
	  	case EGameStatus.eTxedDareChallenge:
	  		fromGuiSetGameActionVar( EGameAction.eTodGameActionDareAccepted, 1);
	  		setGameStatus( EGameStatus.eWaitingForChallengeRx );
	  		break;
	  		
	  	case EGameStatus.eTxedDareAccepted:
	  		break;
	  		
	  	case EGameStatus.eTxedDareRejected:
	  		break;
	  		
	  	case EGameStatus.eTxedTruthChallenge:
	  		fromGuiSetGameActionVar( EGameAction.eTodGameActionTruthAccepted, 1);
	  		setGameStatus( EGameStatus.eWaitingForChallengeRx );
	  		break;
	  		
	  	case EGameStatus.eTxedTruthAccepted:
	  		break;
	  		
	  	case EGameStatus.eTxedTruthRejected:
	  		break;
	  	};
 	}

 	//============================================================================
 	void onDareButtonClicked()
 	{
 		switch( m_eGameStatus )
 		{
	  	case EGameStatus.eWaitingForChallengeRx:
	  		break;
	  		
	  	case EGameStatus.eRxedDareChallenge:
	  	case EGameStatus.eRxedDareAccepted:
	  	case EGameStatus.eRxedDareRejected:
	  	case EGameStatus.eRxedTruthChallenge:
	  	case EGameStatus.eRxedTruthAccepted:
	  	case EGameStatus.eRxedTruthRejected:
	  	case EGameStatus.eWaitingForChallengeTx:
	  		fromGuiSetGameActionVar( EGameAction.eTodGameActionChallengeDare, 1);
	  		setGameStatus( EGameStatus.eTxedDareChallenge );
	  		break;
	  		
	  	case EGameStatus.eTxedDareChallenge:
	  		fromGuiSetGameActionVar( EGameAction.eTodGameActionDareRejected, 1);
	  		setGameStatus( EGameStatus.eWaitingForChallengeRx );
	  		break;
	  		
	  	case EGameStatus.eTxedDareAccepted:
	  		break;
	  		
	  	case EGameStatus.eTxedDareRejected:
	  		break;
	  		
	  	case EGameStatus.eTxedTruthChallenge:
	  		fromGuiSetGameActionVar( EGameAction.eTodGameActionTruthRejected, 1);
	  		setGameStatus( EGameStatus.eWaitingForChallengeRx );
	  		break;
	  		
	  	case EGameStatus.eTxedTruthAccepted:
	  		break;
	  		
	  	case EGameStatus.eTxedTruthRejected:
	  		break;
	  	};
 	}

    
	//======================================================================== 
    boolean fromGuiSetGameValueVar( int iVarId, int iVarValue )
    {
    	return NativeTxTo.fromGuiSetGameValueVar(	m_ePluginType, 
				m_HisIdent.m_u64IdHiPart,
				m_HisIdent.m_u64IdLoPart,  														
				iVarId, 
				iVarValue );
    }
    
	//======================================================================== 
    boolean fromGuiSetGameActionVar( int iVarActionId, int iVarActionValue )
    {
    	return NativeTxTo.fromGuiSetGameActionVar(	m_ePluginType, 
				m_HisIdent.m_u64IdHiPart,
				m_HisIdent.m_u64IdLoPart,  														
				iVarActionId, 
				iVarActionValue );
    }
	
	//========================================================================
	void handleUserWentOffline()
	{
		endGame();
		//ErrMsgBox( true, m_HisIdent.getOnlineName() + " is offline" );	
	}
	
	//============================================================================
	boolean beginGame( boolean isChallenger )
	{
		m_IsChallenger = isChallenger;
		setGameStatus( m_IsChallenger ? eWaitingForChallengeRx : eWaitingForChallengeTx );
		boolean result = sendGameStats();
		if( result )
		{
	
		}
	
		return result;
	}
	
	//============================================================================
	void endGame()
	{
		saveMyGameStats();
		setGameStatus( m_IsChallenger ? eWaitingForChallengeRx : eWaitingForChallengeTx );
	}


}