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

import android.os.Handler;
import android.util.Log;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.LinearLayout;
import android.widget.RelativeLayout;
import android.widget.TextView;

public class MultiSessionState
{
	private static final String LOG_TAG = "MultiSessionState:";
	
	static public final int eMSessionStateUnknown 					= -1;
	static public final int eMSessionStateDisabled 					= 0;
	static public final int eMSessionStateIdle 						= 1;
	static public final int eMSessionStateWaitingOfferResponse 		= 2;
	static public final int eMSessionStateWaitingClickSessionButton = 3;
	static public final int eMSessionStateWaitingUserAcceptReject 	= 4;
	static public final int eMSessionStateInSession 				= 5;
	static public final int eMSessionStateOfferRejected 			= 6;
	
	static public final int eMSessionTypePhone 						= 0;
	static public final int eMSessionTypeVidChat 					= 1;
	static public final int eMSessionTypeTruthOrDare 				= 2;
	
	static public final int eImageTypeNormal 						= 0;
	static public final int eImageTypeDisabled 						= 1;
	static public final int eImageTypeCancel 						= 2;
	static public final int eImageTypeRed 							= 3;
	static public final int eImageTypeYellow 						= 4;
	
	static public final int eMSessionActionNone 					= 0;
	static public final int eMSessionActionChatSessionReq			= 1;
	static public final int eMSessionActionChatSessionAccept		= 2;
	static public final int eMSessionActionChatSessionReject		= 3;
	static public final int eMSessionActionOffer 					= 4;
	static public final int eMSessionActionAccept 					= 5;
	static public final int eMSessionActionReject 					= 6;
	static public final int eMSessionActionHangup 					= 7;
	
	MyApp 							m_MyApp;
	AppletPeerMultiSession  	m_Activity;

	int								m_eMSessionType;
	private int						m_SessionState					= eMSessionStateUnknown;

	VxNetIdent						m_HisIdent 						= null;
	
	private TextView				m_StatusLabel 					= null;
	private LinearLayout 			m_OffersLayout 					= null;
  	private LinearLayout 			m_ResponseLayout 				= null;
	private LinearLayout 			m_HangupLayout 					= null;

	private Button					m_SessionOfferButton 			= null;
	
	private Button					m_AcceptSessionButton 			= null;	
	private TextView				m_AcceptSessionText				= null;
	private Button					m_RejectSessionButton 			= null;
	private TextView				m_RejectSessionText				= null;
	private Button					m_HangupButton 					= null;
	
	private VidCamCtrlLogic			m_VidCamLogic					= null;
	private static boolean			m_TodGameVisible				= false;
	private static boolean			m_VidChatVisible				= false;
	private TodGameLogic			m_TodGameLogic					= null;
	private boolean					m_SessionButtonBlinkEnabled		= false;
	private boolean					m_SessionButtonToggle			= false;
	private boolean					m_IsInChatSession				= false;
	
	
	//========================================================================
	MultiSessionState( MyApp myApp, AppletPeerMultiSession activityToFriendMultiSession, int sessionType )
	{
		m_MyApp 						= myApp;
		m_Activity 						= activityToFriendMultiSession;
		m_eMSessionType 				= sessionType;
	}  
    
 	//========================================================================
 	public void  onOncePerSecond()
 	{	
 		if( m_SessionButtonBlinkEnabled )
 		{
 			m_SessionButtonToggle = !m_SessionButtonToggle;
 			if( m_SessionButtonToggle )
 			{
 				setButtonImage( m_SessionOfferButton, eImageTypeYellow );
 			}
 			else
 			{
 				setButtonImage( m_SessionOfferButton, eImageTypeRed );				
 			}
 		}
 	}

	//========================================================================
   	void enableSessionButtonBlink( boolean enableBlink )
   	{
   		if( enableBlink != m_SessionButtonBlinkEnabled )
   		{
   			m_SessionButtonBlinkEnabled = enableBlink;
   			if( false == m_SessionButtonBlinkEnabled )
   			{
   				setButtonImage( m_SessionOfferButton, eImageTypeNormal );
   			}
   		}
   	}
	
	//========================================================================
	void setGuiWidgets( VxNetIdent				hisIdent,
						TextView				statusLabel,
						LinearLayout 			offersLayout,
						LinearLayout 			responseLayout,
						LinearLayout 			hangupLayout,
						Button					sessionOfferButton,
						Button					acceptSessionButton,
						TextView				acceptSessionText,
						Button					rejectSessionButton,
						TextView				rejectSessionText,
						Button					hangupButton,
						VidCamCtrlLogic			vidCamLogic,
						TodGameLogic			todGameLogic )
	{
		m_HisIdent				= hisIdent;
		m_StatusLabel			= statusLabel;
		m_OffersLayout 			= offersLayout;
	  	m_ResponseLayout 		= responseLayout;
		m_HangupLayout 			= hangupLayout;
		m_SessionOfferButton 	= sessionOfferButton;
		m_AcceptSessionButton 	= acceptSessionButton;	
		m_AcceptSessionText		= acceptSessionText;
		m_RejectSessionButton 	= rejectSessionButton;
		m_RejectSessionText		= rejectSessionText;
		m_HangupButton 			= hangupButton;	
		m_VidCamLogic			= vidCamLogic;
		m_TodGameLogic			= todGameLogic;
 
		if( null != m_SessionOfferButton )
		{
	    	m_SessionOfferButton.setOnClickListener( new OnClickListener() 
	        {
	        	public void onClick(View v) 
			 	{
	    	        m_MyApp.playButtonClick();
	        		onSessionOfferButtonClicked();
			 	}
	        } );
		}
	}
   	
	//========================================================================
   	void onSessionOfferButtonClicked()
   	{
   		if( false == canSend() )
   		{
   			String cantSendReason = describeCantSendReason();
   			NativeRxFrom.toGuiStatusMessage( cantSendReason );
   			return;
   		}
   		
   		if( eMSessionStateDisabled == m_SessionState )
   		{
   			return;
   		}

   		if( eMSessionStateIdle == m_SessionState )
   		{
   			if( false == NativeTxTo.fromGuiMultiSessionAction( eMSessionActionOffer, m_HisIdent.m_u64IdHiPart, m_HisIdent.m_u64IdLoPart, (int)m_eMSessionType ) )
   			{
   				Log.d( LOG_TAG, "MultiSessionState::onSessionOfferButtonClicked user offline\n" ); 
   				return;
   			}
   			
   			setSessionState( eMSessionStateWaitingOfferResponse );	
   		}
   		else if( eMSessionStateWaitingOfferResponse == m_SessionState )
   		{
   			// is being canceled
   			if( false == NativeTxTo.fromGuiMultiSessionAction( eMSessionActionReject, m_HisIdent.m_u64IdHiPart, m_HisIdent.m_u64IdLoPart, (int)m_eMSessionType ) )
   			{
   				Log.d( LOG_TAG, "MultiSessionState::onSessionOfferButtonClicked user offline\n" ); 
   				return;
   			}

   			setSessionState( eMSessionStateIdle );	
   		}
   		else if( eMSessionStateWaitingClickSessionButton == m_SessionState )
   		{
   			// user has been offered and icon is blinking now he clicked so show accept or reject buttons
   			setSessionState( eMSessionStateWaitingUserAcceptReject );	
   		}
   		else
   		{
   			Log.d( LOG_TAG, "MultiSessionState::onSessionOfferButtonClicked state" + m_SessionState );
  		}
  	}
   	
	//========================================================================
   	boolean canSend()
   	{
   		if(  m_HisIdent.isOnline()
   			&& m_IsInChatSession )
   		{
			int accessState = Constants.ePluginAccessLocked; 
			switch( m_eMSessionType )
			{
			case eMSessionTypePhone:
				accessState = m_HisIdent.getMyAccessPermissionFromHim( Constants.ePluginTypeVoicePhone );
				break;
			case eMSessionTypeVidChat:
				accessState = m_HisIdent.getMyAccessPermissionFromHim( Constants.ePluginTypeVideoPhone );
				break;
			case eMSessionTypeTruthOrDare:
				accessState = m_HisIdent.getMyAccessPermissionFromHim( Constants.ePluginTypeTruthOrDare );
				break;
			}

			return ( Constants.ePluginAccessOk == accessState );
		}
   		
   		return false;
   	}

	//========================================================================
   	String describeCantSendReason()
   	{
   		String reasonStr = "";
		int accessState = Constants.ePluginAccessLocked; 
		switch( m_eMSessionType )
		{
		case eMSessionTypePhone:
			reasonStr = GuiHelpers.describePlugin( Constants.ePluginTypeVoicePhone, false );
			accessState = m_HisIdent.getMyAccessPermissionFromHim( Constants.ePluginTypeVoicePhone );
			break;
		case eMSessionTypeVidChat:
			reasonStr = GuiHelpers.describePlugin( Constants.ePluginTypeVideoPhone, false );
			accessState = m_HisIdent.getMyAccessPermissionFromHim( Constants.ePluginTypeVideoPhone );
			break;
		case eMSessionTypeTruthOrDare:
			reasonStr = GuiHelpers.describePlugin( Constants.ePluginTypeTruthOrDare, false );
			accessState = m_HisIdent.getMyAccessPermissionFromHim( Constants.ePluginTypeTruthOrDare );
			break;
		}
  		
		reasonStr += " is disabled because ";
		if( Constants.ePluginAccessOk != accessState )
		{
			reasonStr += "Insufficient Permission";
		}
		else if( false == m_HisIdent.isOnline() )
		{
			reasonStr += m_HisIdent.m_as8OnlineName;	
			reasonStr += "Is Offline";							
		}
		else if( false == m_IsInChatSession )
		{
			reasonStr += m_HisIdent.m_as8OnlineName;	
			reasonStr += "Is Not In Chat Session With You";				
		}
		else
		{
			reasonStr += "Unknown Reason";		
		}
   		
   		return reasonStr;
   	}
   	
	//========================================================================
   	void onAcceptSessionButtonClicked()
   	{
   		setSessionState( eMSessionStateInSession );
   		NativeTxTo.fromGuiMultiSessionAction( eMSessionActionAccept, m_HisIdent.m_u64IdHiPart, m_HisIdent.m_u64IdLoPart, (int)m_eMSessionType );
   	}

	//========================================================================
   	void onRejectSessionButtonClicked()
   	{
   		setSessionState( eMSessionStateIdle );
   		NativeTxTo.fromGuiMultiSessionAction( eMSessionActionReject, m_HisIdent.m_u64IdHiPart, m_HisIdent.m_u64IdLoPart, (int)m_eMSessionType );
   	}

	//========================================================================
   	void onHangupButtonClicked()
   	{
   		setSessionState( eMSessionStateIdle );
   		NativeTxTo.fromGuiMultiSessionAction( eMSessionActionHangup, m_HisIdent.m_u64IdHiPart, m_HisIdent.m_u64IdLoPart, (int)m_eMSessionType );
   	}
	
	//========================================================================
	void onInSession( boolean isInSession )
	{
		m_IsInChatSession = isInSession;
		if( ( false == m_IsInChatSession )
			&& ( eMSessionStateUnknown != m_SessionState )
			&& ( eMSessionStateIdle != m_SessionState )
			&& ( eMSessionStateDisabled != m_SessionState )
			&& ( eMSessionStateInSession != m_SessionState ) )
		{
			// force stop
	   		NativeTxTo.fromGuiMultiSessionAction( eMSessionActionHangup, m_HisIdent.m_u64IdHiPart, m_HisIdent.m_u64IdLoPart, (int)m_eMSessionType );			
	   		setSessionState( eMSessionStateIdle );
		}
		
		checkForSendAccess();
	}

	//========================================================================
   	void setSessionState( int sessionState )
   	{	
   		if( eMSessionStateDisabled == sessionState )
   		{
   			if( eMSessionStateDisabled != m_SessionState )
   			{
  				m_SessionState = sessionState;
   				resetGui();
    			setButtonImage( m_SessionOfferButton, eImageTypeDisabled );
   				//m_SessionOfferButton.setEnabled( false );
   			}
   			
   			return;
   		}
   		
   		if( sessionState == m_SessionState )
   		{
   			return;
   		}

   		if( ( eMSessionStateInSession == m_SessionState )
   			&& ( eMSessionStateInSession != sessionState ) )
   		{
   			if( eMSessionTypeTruthOrDare == m_eMSessionType )
   			{
   				m_TodGameLogic.endGame();
   			}

   			resetGui();
   		}

   		switch( sessionState )
   		{
   		case eMSessionStateIdle:
   			resetGui();
   			m_SessionState = sessionState;
   			setButtonImage( m_SessionOfferButton, eImageTypeNormal );
   			m_SessionOfferButton.setEnabled( true );
  			break;

   		case eMSessionStateWaitingOfferResponse:
   			//m_SessionOfferButton.setEnabled( false );
   			setButtonImage( m_SessionOfferButton, eImageTypeCancel );
   			m_SessionOfferButton.setEnabled( true );
  			m_SessionState = sessionState;
   			switch( m_eMSessionType )
   			{
   			case eMSessionTypePhone:
   				setStatusText( "Waiting Phone Answer" );
   				break;
   			case eMSessionTypeVidChat:
   				setStatusText( "Waiting Video Chat Offer Response"  );
   				break;
   			case eMSessionTypeTruthOrDare:
   				setStatusText( "Waiting Truth Or Dare Offer Response" );
   				break;
   			}
   			
   			break;

   		case eMSessionStateWaitingClickSessionButton :
   			m_SessionState = sessionState;
   			m_SessionOfferButton.setEnabled( true );
   			enableSessionButtonBlink( true );
   			m_MyApp.playSound( ESndDef.eSndDefPhoneRing1 );
   			switch( m_eMSessionType )
   			{
   			case eMSessionTypePhone:
   				setStatusText( "Please Answer Phone" );
   				break;
   			case eMSessionTypeVidChat:
   				setStatusText( "Video Chat Offer Available" );
   				break;
   			case eMSessionTypeTruthOrDare:
   				setStatusText( "Truth Or Dare Offer Available" );
   				break;
   			}
   			
   			break;

   		case eMSessionStateWaitingUserAcceptReject:
   			m_SessionState = sessionState;
  			m_SessionOfferButton.setEnabled( true );
   			enableSessionButtonBlink( false );
   			switch( m_eMSessionType )
   			{
   			case eMSessionTypePhone:
   				m_AcceptSessionText.setText( "Answer Phone" );
   				m_RejectSessionText.setText( "Reject Phone Call" );
   				break;
   			case eMSessionTypeVidChat:
   				m_AcceptSessionText.setText( "Accept Video Chat" );
   				m_RejectSessionText.setText( "Reject Video Chat" );
   				break;
   			case eMSessionTypeTruthOrDare:
   				m_AcceptSessionText.setText( "Play Truth Or Dare" );
   				m_RejectSessionText.setText( "Reject Truth Or Dare" );
   				break;
   			}

   			setButtonImage( m_AcceptSessionButton, eImageTypeNormal );
   			setButtonImage( m_RejectSessionButton, eImageTypeCancel );
  			showTodGame( false );
       		showVidChat( false );
       		showOffersFrame( false );
       		showResponseFrame( true );
       		showHangupSession( false );
       		//m_ResponseLayout.invalidate();
   			break;

   		case eMSessionStateInSession:
   			setButtonImage( m_HangupButton, eImageTypeCancel );
       		showOffersFrame( false );
       		showResponseFrame( false );
       		showHangupSession( true );
   			switch( m_eMSessionType )
   			{
   			case eMSessionTypePhone:
   				setStatusText( "Phone Connected" );
  	      		showVidChat( false );
   	   			showTodGame( false );
 				break;
 				
   			case eMSessionTypeVidChat:
   				setStatusText( "In Video Chat Session" );
   	      		showVidChat( true );
				break;
				
   			case eMSessionTypeTruthOrDare:
 				setStatusText( "In Truth Or Dare Session" );
 	      		showVidChat( true );
   	   			showTodGame( true );
 				m_TodGameLogic.beginGame( eMSessionStateWaitingOfferResponse == m_SessionState  );
 				break;
   			}

   			m_SessionState = sessionState;
   			break;

   		case eMSessionStateOfferRejected:
   			resetGui();
   			m_SessionState = eMSessionStateIdle;
   			setButtonImage( m_SessionOfferButton, eImageTypeNormal );
   			enableSessionButtonBlink( false );
   			m_SessionOfferButton.setEnabled( true );
   			switch( m_eMSessionType )
   			{
   			case eMSessionTypePhone:
   				setStatusText( "Phone Offer Rejected" );
   				break;
   			case eMSessionTypeVidChat:
   				setStatusText( "Video Chat Offer Rejected" );
   				break;
   			case eMSessionTypeTruthOrDare:
   				setStatusText( "Truth Or Dare Offer Rejected" );
   				break;
   			}
   			
   			break;
   			
   		default:
   			break;
   		}
   	}
  	
   	//========================================================================
   	void handleMultiSessionAction( int eMSessionAction )
   	{
   		switch( eMSessionAction )
   		{
   		case eMSessionActionChatSessionReq:
   			if( null != m_HisIdent )
   			{
   				NativeTxTo.fromGuiMultiSessionAction( eMSessionActionChatSessionAccept, m_HisIdent.m_u64IdHiPart, m_HisIdent.m_u64IdLoPart, 0 );
   			}
   			else
   			{
   				Log.e(LOG_TAG, "handleMultiSessionAction NULL m_HisIdent" );
   			}
   			
   			break;
   			
   		case eMSessionActionChatSessionAccept:
   			break;
   			
   		case eMSessionActionChatSessionReject:
   			break;
   			
   		case eMSessionActionOffer:
   			setSessionState( eMSessionStateWaitingClickSessionButton );
   			break;
   			
   		case eMSessionActionAccept:
   			setSessionState( eMSessionStateInSession );
   			break;
   			
   		case eMSessionActionReject:
   			setSessionState( eMSessionStateOfferRejected );
   			break;
   			
   		case eMSessionActionHangup:
   			setSessionState( eMSessionStateIdle );
   			switch( m_eMSessionType )
   			{
   			case eMSessionTypePhone:
   				setStatusText( "User Hung Up Phone" );
   				break;
   			case eMSessionTypeVidChat:
   				setStatusText( "User Ended Video Chat" );
   				break;
   			case eMSessionTypeTruthOrDare:
   				setStatusText( "User Ended Truth Or Dare" );
   				break;
   			}
   			
   			break;
   			
   		default:
   			break; 	
   		}
   	} 		

	//========================================================================
   	boolean isInSession()
   	{
   		return ( eMSessionStateInSession == m_SessionState );
   	}
   	
	//========================================================================
   	void setButtonImage( Button button, int imageType )
   	{
   		switch( m_eMSessionType )
   		{
   		case eMSessionTypePhone:
   	   		switch( imageType )
   	   		{
  	   		case eImageTypeNormal: 			
   	   			button.setBackgroundResource( R.drawable.button_phone_normal );
   	   			break;
   	   		case eImageTypeDisabled: 			
   	   			button.setBackgroundResource( R.drawable.button_phone_disabled );
   	   			break;
   	   		case eImageTypeCancel: 			
   	   			button.setBackgroundResource( R.drawable.button_phone_cancel );
   	   			break;
   	   		case eImageTypeRed: 			
   	   			button.setBackgroundResource( R.drawable.button_phone_red );
   	   			break;
   	   		case eImageTypeYellow: 			
   	   			button.setBackgroundResource( R.drawable.button_phone_yellow );
   	   			break;
   	   		}
   	   		
   	   		break;
   	   		
   		case eMSessionTypeVidChat:
   	   		switch( imageType )
   	   		{
   	   		case eImageTypeNormal: 			
   	   			button.setBackgroundResource( R.drawable.button_vidphone_normal );
   	   			break;
   	   		case eImageTypeDisabled: 			
   	   			button.setBackgroundResource( R.drawable.button_vidphone_disabled );
   	   			break;
   	   		case eImageTypeCancel: 			
   	   			button.setBackgroundResource( R.drawable.button_vidphone_cancel );
   	   			break;
   	   		case eImageTypeRed: 			
   	   			button.setBackgroundResource( R.drawable.button_vidphone_red );
   	   			break;
   	   		case eImageTypeYellow: 			
   	   			button.setBackgroundResource( R.drawable.button_vidphone_yellow );
   	   			break;
   	   		}
   	   		
   	   		break;
   	   		
 		case eMSessionTypeTruthOrDare:
   	   		switch( imageType )
   	   		{
  	   		case eImageTypeNormal: 			
   	   			button.setBackgroundResource( R.drawable.button_truth_or_dare_normal );
   	   			break;
   	   		case eImageTypeDisabled: 			
   	   			button.setBackgroundResource( R.drawable.button_truth_or_dare_disabled );
   	   			break;
   	   		case eImageTypeCancel: 			
   	   			button.setBackgroundResource( R.drawable.button_truth_or_dare_cancel );
   	   			break;
   	   		case eImageTypeRed: 			
   	   			button.setBackgroundResource( R.drawable.button_truth_or_dare_red );
   	   			break;
   	   		case eImageTypeYellow: 			
   	   			button.setBackgroundResource( R.drawable.button_truth_or_dare_yellow );
   	   			break;
   	   		}		
   	   	
   	   		break;
   		}   		
    }
     
	//======================================================================== 
    private void showVidChat( boolean showWidgets )
    {
    	m_VidChatVisible = showWidgets;
    	m_VidCamLogic.showCamAllLayout( m_VidChatVisible | m_TodGameVisible );
     }
    
    //======================================================================== 
	private void showTodGame( boolean showWidgets )
	{
		m_TodGameVisible = showWidgets;
 	  	m_TodGameLogic.showTodGame(showWidgets);
 	   	m_VidCamLogic.showCamAllLayout( m_VidChatVisible | m_TodGameVisible );
	}
  
	//======================================================================== 
    private void showHangupSession( boolean showWidgets )
    {
    	int visibleMode = View.GONE;
    	if( showWidgets )
    	{
    		visibleMode = View.VISIBLE;
    	}
    	
      	m_HangupLayout.setVisibility(visibleMode);
    }
    
 	//======================================================================== 
     private void showOffersFrame( boolean showWidgets )
     {
     	int visibleMode = View.GONE;
     	if( showWidgets )
     	{
     		visibleMode = View.VISIBLE;
     	}

     	m_OffersLayout.setVisibility(visibleMode);
     }
   
 	//======================================================================== 
     private void showResponseFrame( boolean showWidgets )
     {
     	int visibleMode = View.GONE;
     	if( showWidgets )
     	{
     		visibleMode = View.VISIBLE;
     	}

     	m_ResponseLayout.setVisibility(visibleMode);
     }

   	//========================================================================
   	void setStatusText( String statusText )
   	{
   		m_StatusLabel.setText( statusText );
   	}

   	//========================================================================
   	void resetGui()
   	{
	  	enableSessionButtonBlink( false );
	  	setStatusText( "" );
   		showOffersFrame( true );
	  	m_SessionOfferButton.setEnabled( true );
		showResponseFrame( false );
   		showHangupSession( false );
    	showVidChat( false );
  		showTodGame( false );
  		if( canSend() )
  		{
  			setButtonImage( m_SessionOfferButton, eImageTypeNormal );
  			m_SessionOfferButton.setEnabled( true );
  		}
  		else
  		{
  			setButtonImage( m_SessionOfferButton, eImageTypeDisabled );
  			//m_SessionOfferButton.setEnabled( false );
  		}
   	}

   	//========================================================================
	public void checkForSendAccess()
	{
		if( canSend() )
		{
			if( ( eMSessionStateDisabled == m_SessionState ) 
				|| ( eMSessionStateUnknown == m_SessionState ) )
			{
				setSessionState( eMSessionStateIdle );
			}
		}
		else
		{
			setSessionState( eMSessionStateDisabled );
		}	
	}
};
