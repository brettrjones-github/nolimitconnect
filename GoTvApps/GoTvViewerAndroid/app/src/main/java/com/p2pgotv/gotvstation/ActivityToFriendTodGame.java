//============================================================================
// Copyright (C) 2011 Brett R. Jones
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

import android.os.Bundle;
import android.util.Log;
import android.view.WindowManager;
import android.widget.Button;
import android.widget.EditText;
import android.widget.ImageView;
import android.widget.LinearLayout;
import android.widget.RelativeLayout;
import android.widget.TextView;

public class AppletPeerTodGame extends AppletPeerGuiBase
{
	private static final String LOG_TAG = "AppletPeerTodGame:";
	
	class EGameStatus
	{
		static public final int eWaitingForChallengeRx 	= 0;
		static public final int eRxedDareChallenge 		= 1;
		static public final int eRxedDareAccepted 		= 2;
		static public final int eRxedDareRejected 		= 3;
		static public final int eRxedTruthChallenge 	= 4;
		static public final int eRxedTruthAccepted 		= 5;
		static public final int eRxedTruthRejected 		= 6;
	
		static public final int eWaitingForChallengeTx 	= 7;
		static public final int eTxedDareChallenge 		= 8;
		static public final int eTxedDareAccepted 		= 9;
		static public final int eTxedDareRejected 		= 10;
		static public final int eTxedTruthChallenge 	= 11;
		static public final int eTxedTruthAccepted 		= 12;
		static public final int eTxedTruthRejected 		= 13;
		
		static public final int eTxedOffer 				= 14;
		
		static public final int eMaxGameStatus 			= 15; // must be last
	}

	class EGameAction
	{
		static public final int eTodGameActionSendStats 		= 0;
		static public final int eTodGameActionChallengeDare 	= 1;
		static public final int eTodGameActionDareAccepted 		= 2;
		static public final int eTodGameActionDareRejected 		= 3;
		static public final int eTodGameActionChallengeTruth 	= 4;
		static public final int eTodGameActionTruthAccepted 	= 5;
		static public final int eTodGameActionTruthRejected 	= 6;
		static public final int eMaxTodGameAction 				= 7; // must be last
	}

	class EGameButton
	{
		static public final int eGameButtonDare 	= 0;
		static public final int eGameButtonTruth 	= 1;
		static public final int eMaxGameButton 		= 2;
	};
	
	private EditText				m_InstMsgHistoryText 		= null;
	private EditText				m_InstMsgSayText 			= null;
	private Button 					m_InstMsgSayBut 			= null;
	private Button 					m_InstMsgShredBut 			= null;
	private InstMsgLogic			m_InstMsgLogic				= null;
	
	private ImageViewWithSizer 		m_VidCamView				= null;
	private ImageViewWithSizer 		m_VidCamThumbnail			= null;
  	private RelativeLayout 			m_VidCamLayout 				= null;
  	
	private boolean					m_IsInSession				= false;
	private TodGameLogic			m_TodGameLogic				= null;
	
	
	//========================================================================
    @Override
    public void onCreate(Bundle savedInstanceState) 
    {
    	Log.d(LOG_TAG, "onCreate");  
 		m_MyApp 							= (MyApp) this.getApplication();
        m_HisIdent 							= m_MyApp.getCurrentFriend();
		m_ePluginType 						= Constants.ePluginTypeTruthOrDare;

        setContentView(R.layout.activity_to_friend_truth_or_dare);  	
        super.onCreate(savedInstanceState);
	    setPageTitle( R.string.activity_to_friend_tod_game_name );
	    getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);
	    getWindow().setSoftInputMode(WindowManager.LayoutParams.SOFT_INPUT_STATE_HIDDEN); // avoid keyboard pop up on startup
	    m_OfferSessionLogic.setOfferText("Play Truth Or Dare");
	    m_OfferSessionLogic.setUserData( 0 );
	    
		m_InstMsgHistoryText 			= (EditText)this.findViewById(R.id.inst_msg_history_text);
		m_InstMsgSayText 				= (EditText)this.findViewById(R.id.inst_msg_say_text);
		m_InstMsgSayBut 				= (Button)this.findViewById(R.id.inst_msg_say_button);
		m_InstMsgShredBut 				= (Button)this.findViewById(R.id.inst_msg_shred_button);		
		m_InstMsgLogic					= new InstMsgLogic( m_MyApp, m_ePluginType, m_HisIdent, m_InstMsgSayText, m_InstMsgHistoryText, m_InstMsgSayBut, m_InstMsgShredBut );
               
        LinearLayout todGameLayout		= (LinearLayout)this.findViewById(R.id.tod_frame);
        Button dareButton 				= (Button)this.findViewById(R.id.but_dare);
      	Button truthButton 				= (Button)this.findViewById(R.id.but_truth);
 
    	TextView myDaresLabel 			= (TextView)this.findViewById(R.id.textMyDareCnt);
    	TextView myTruthsLabel 			= (TextView)this.findViewById(R.id.textMyTruthCnt);
    	TextView friendDaresLabel 		= (TextView)this.findViewById(R.id.textFriendDareCnt);
        TextView friendTruthsLabel 		= (TextView)this.findViewById(R.id.textFriendTruthCnt);
        TextView todGameStatusLabel 	= (TextView)this.findViewById(R.id.textTodGameStatusLabel);
        
        ImageView todChallengeImage 	= (ImageView)this.findViewById( R.id.todChallengeStatusImage );
        TextView todChallengeText 		= (TextView)this.findViewById( R.id.todChallengeStatusLabel );
       
        m_VidCamLayout					= (RelativeLayout)this.findViewById(R.id.reuse_cam_view_layout);
        m_VidCamView					= getVidCamView();
        m_VidCamThumbnail				= getVidCamThumbnail();
        m_TodGameLogic 					= new TodGameLogic( m_MyApp, 
															m_ePluginType,
															m_HisIdent,
															todGameLayout,
															dareButton,
															truthButton,
															myDaresLabel,
															myTruthsLabel,
															friendDaresLabel,
															friendTruthsLabel,
															todGameStatusLabel,
															todChallengeImage,
															todChallengeText,
															m_VidCamLayout,
															m_VidCamView,
															m_VidCamThumbnail );        	
   	
       
        m_VidCamView.setVideoFeedId( m_HisIdent.getOnlineId() );
        m_VidCamThumbnail.setVideoFeedId( m_MyApp.getMyOnlineId() );
        m_VidCamThumbnail.m_IsThumbnail = true;
    	
        if( m_IsInSession )
        {
        	m_TodGameLogic.beginGame( false == isSessionOffer() );
        }
        else
        {
        	m_TodGameLogic.setGameStatus( EGameStatus.eTxedOffer );
        }
        
        enableAudioControls( true );
		showSpeakerButton( true );
        sendOfferOrResponse();
        checkVolumeLevel();
    }

	//============================================================================
	@Override
	public
	void onInSession( boolean isInSession )
	{
		m_IsInSession = isInSession;
		if( isInSession 
			&& ( null != m_TodGameLogic ) )
		{
			m_TodGameLogic.beginGame( false == isSessionOffer() );
		}
	}
 	
	//========================================================================    
    @Override
    protected void onStop() 
    {
     	VxGUID myId = m_MyApp.getMyOnlineId();
    	NativeTxTo.fromGuiWantMediaInput( myId.m_u64IdHiPart, myId.m_u64IdLoPart, Constants.eMediaInputVideoJpgSmall, false );
    	if( null != m_HisIdent )
    	{
    		NativeTxTo.fromGuiWantMediaInput( m_HisIdent.m_u64IdHiPart, m_HisIdent.m_u64IdLoPart, Constants.eMediaInputVideoJpgSmall, false );
    	}
    	
    	super.onStop();    	
    }   
    
	//============================================================================
	@Override
	public void toGuiSetGameActionVar(int iPlugin, VxGUID sessionId, int s32ActionId, int s32VarValue) 
	{
		if( ( iPlugin == m_ePluginType )
			&& ( null !=  m_TodGameLogic ) )
		{
			final int actionId = s32ActionId;
			final int varValue = s32VarValue;
			try 
			{
				runOnUiThread(new Runnable() 
				{	
					@Override
					public void run() 
					{
						m_TodGameLogic.doGuiSetGameActionVar(actionId, varValue);
					}
				});
			} 
			catch (Exception e) 
			{
				e.printStackTrace();
			}
		}	
	}

	//============================================================================
	@Override
	public void toGuiSetGameValueVar( int iPlugin, VxGUID sessionId, int s32VarId, int s32VarValue ) 
	{
		if( ( iPlugin == m_ePluginType )
			&& ( null !=  m_TodGameLogic ) )
		{
			final int varId = s32VarId;
			final int varValue = s32VarValue;
			try 
			{
				runOnUiThread(new Runnable() 
				{	
					@Override
					public void run() 
					{
						m_TodGameLogic.doGuiSetGameValueVar(varId, varValue);
					}
				});
			} 
			catch (Exception e) 
			{
				e.printStackTrace();
			}
		}	
	}

	//========================================================================
	@Override
	public void toGuiInstMsg( VxNetIdent netIdent, int ePluginType, String strMsg ) 
	{
		if( ( null != m_HisIdent )
			&& ( m_HisIdent.getMyOnlineId().equals( netIdent.getMyOnlineId() ) )
			&& ( ePluginType == m_ePluginType )
			&& ( null != strMsg )
			&& ( 0 != strMsg.length() ) )
		{
			final String instMsgStr = strMsg;				
	        runOnUiThread(new Runnable() 
	    	{
	        	public void run() 
	        	{
	        		m_InstMsgLogic.toGuiInstMsg( instMsgStr );
	        	}
	    	});
		}
	}			
}

