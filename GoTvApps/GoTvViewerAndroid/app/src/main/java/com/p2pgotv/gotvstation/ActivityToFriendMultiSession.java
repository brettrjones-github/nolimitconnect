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

import java.util.ArrayList;
import java.util.concurrent.Semaphore;
import java.util.HashMap;
import java.util.List;
import java.util.Locale;
import java.util.Map;

import android.app.Activity;
import android.app.AlertDialog;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.media.AudioManager;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.support.v7.app.AppCompatActivity;
import android.util.Log;
import android.view.MotionEvent;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.View.OnTouchListener;
import android.view.WindowManager;
import android.widget.Button;
import android.widget.ImageView;
import android.widget.LinearLayout;
import android.widget.ListView;
import android.widget.RelativeLayout;
import android.widget.TextView;
import android.widget.Toast;
import android.support.v7.app.ActionBar;

public class AppletPeerMultiSession extends AppCompatActivity implements ToGuiUserInputInterface, AssetCallbackInterface, ToGuiActivityInterface, ToGuiMessageInterface, OfferSessionCallbackInterface  
{
	private static final String LOG_TAG 					= "ActivityMultiSession";
	
	static public final int 	eMSessionTypePhone 			= 0;
	static public final int 	eMSessionTypeVidChat 		= 1;
	static public final int 	eMSessionTypeTruthOrDare 	= 2;	
	
	//=== vars ===//
	private int					m_ePluginType				= Constants.ePluginTypeMultiSession;
    public VxNetIdent 			m_HisIdent 					= null;
	public boolean 				m_bIsOffer 					= false;
	public boolean 				m_bIsMyself 				= false;
	
	private ChatMessageInputToolBox m_InputBox;
	private ListView 			m_ListView                  = null;
    private List<ChatMessage>   m_ChatMessageList           = new ArrayList<ChatMessage>();
	private ChatMessageAdapter 	m_ChatAdapter               = null;
    private final Semaphore     m_AddChatMsgListMutex       = new Semaphore(1);
    private MyApp 				m_MyApp 					= null;
	private AppletPeerMultiSessionState m_ActivityState 	= null;
	
	public final  ArrayList<String> m_oMessages 			= new  ArrayList<String>();
	private Button 				m_CloseButton 				= null;

	private TextView			m_StatusMsgLabel			= null;
	private TextView 			m_TotalCamClientsText 		= null;
	
    protected Button            m_MuteMicrophoneBut 		= null;
    protected Button            m_MuteSpeakerBut 			= null;
 
	private LinearLayout 		m_HangupLayout 				= null;
	private LinearLayout 		m_OffersLayout 				= null;
  	private LinearLayout 		m_ResponseLayout 			= null;

	private Button				m_VidChatButton 			= null;
	private Button				m_TruthOrDareButton 		= null;	
	private Button				m_VoicePhoneButton 			= null;
	
	private Button				m_AcceptSessionButton 		= null;	
	private TextView			m_AcceptSessionText			= null;
	private Button				m_RejectSessionButton 		= null;
	private TextView			m_RejectSessionText			= null;
	private Button				m_HangupButton 				= null;
	
	// camera
  	private RelativeLayout 		m_VidCamLayout 				= null;
	private ImageViewWithSizer	m_VidCamView 				= null;
	private ImageViewWithSizer	m_VidCamThumbnail 			= null;
	private VidCamCtrlLogic		m_VidCamLogic				= null;
	public boolean 				m_bIsCamThumbnailVisible 	= false;
	
	private MultiSessionState 	m_PhoneState				= null;
	private MultiSessionState 	m_VidChatState				= null;
	private MultiSessionState 	m_TruthOrDareState			= null;
	private ArrayList<MultiSessionState> m_SessionStates	= new  ArrayList<MultiSessionState>();
	
	private TodGameLogic		m_TodGameLogic				= null;
	private int					m_CurMSessionType			= eMSessionTypePhone;
    
 	protected OfferSessionLogic	m_OfferSessionLogic			= null;
 	private boolean				m_OfferOrResponseIsSent 	= false;
 	private boolean				m_CanSend 					= false;
 	private AssetGuiInfo 		m_GalleryPickAsset			= null;
 	private Handler 			m_MyEventHandler 			= null; 
 	protected boolean			m_IsDestroyed				= false;
 	private int 				m_TickTock500ms 			= 0;
 	private boolean				m_IsInChatSession			= false;
	private OfferBarWidget		m_OfferBarWidget			= null;
 
	//========================================================================
	@Override
	public void onCreate(Bundle savedInstanceState) 
	{
		Log.i( LOG_TAG, "onCreate" );
 		m_MyApp 							= (MyApp) this.getApplication();
		m_ePluginType 						= Constants.ePluginTypeMultiSession;
        m_HisIdent 							= m_MyApp.getCurrentFriend();
        
        setContentView( R.layout.activity_multsession ); 
		super.onCreate(savedInstanceState);		
		ActionBar actionBar = getSupportActionBar();
		if( null != actionBar )
		{
			actionBar.hide();
		}
		
        getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);
       
        m_TotalCamClientsText 				= (TextView)this.findViewById( R.id.total_cam_clients_text );
        m_StatusMsgLabel 					= (TextView)this.findViewById( R.id.status_msg_label );
 
  		m_ActivityState						= m_MyApp.getMultiSessionState();
 		m_ActivityState.setIsPersonalRecorder( false );
 		m_ActivityState.m_Activity 			= this;
 		
		Bundle extras = getIntent().getExtras();
        if( null != extras ) 
        { 
        	m_bIsOffer = extras.getBoolean("IS_OFFER"); 
        } 
        
        if( m_bIsOffer )
        {
        	m_HisIdent = m_MyApp.m_oGuiOfferSession.getHisIdent();
        	m_OfferSessionLogic = new OfferSessionLogic( this, this, m_MyApp, m_ePluginType, m_HisIdent, m_MyApp.m_oGuiOfferSession );
        }
        else
        {
        	m_HisIdent = m_MyApp.getCurrentFriend();
        	m_OfferSessionLogic = new OfferSessionLogic( this, this, m_MyApp, m_ePluginType, m_HisIdent, null );
        }
        
        m_bIsMyself = m_MyApp.isFriendMyself( m_HisIdent );
		m_ActivityState.setFriendIdent( m_HisIdent );
		m_ActivityState.setFriendId( m_HisIdent.getMyOnlineId() );
		
 		TextView titleText 					= (TextView)this.findViewById( R.id.label_title );
 		String strTitle = getString( R.string.activity_to_friend_multisession_title );
 		titleText.setText( strTitle + " - " + m_HisIdent.getOnlineName() );

		initMessageInputToolBox();
		initListView();
		 			
		Log.i( LOG_TAG, "onCreateCalled 2" );
		
    	m_PhoneState			= new MultiSessionState( m_MyApp, this, eMSessionTypePhone );
    	m_VidChatState			= new MultiSessionState( m_MyApp, this, eMSessionTypeVidChat );
    	m_TruthOrDareState		= new MultiSessionState( m_MyApp, this, eMSessionTypeTruthOrDare );
    	m_SessionStates.add( m_PhoneState );
    	m_SessionStates.add( m_VidChatState );
    	m_SessionStates.add( m_TruthOrDareState );
  
    	m_CloseButton = (Button)this.findViewById(R.id.button_close);
        m_CloseButton.setOnClickListener( new OnClickListener() 
        {
        	public void onClick(View v) 
		 	{
  			   	m_MyApp.playButtonClick();
		 		finish();
		 	}
        } );	
        
        setToGuiMessageReadyState( this );
        
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
      	m_VidCamView		 			= (ImageViewWithSizer)this.findViewById(R.id.reuse_cam_view);
    	m_VidCamThumbnail 				= (ImageViewWithSizer)this.findViewById(R.id.reuse_cam_thumbnail);
    	
    	// if we do not disable picture in picture preview then camera will turn on as soon as we enter
    	// multi session and we do not want camera on if not needed because of battery drain
    	m_MyApp.getMySettings().setCamShowPreview( false );
    	m_VidCamView.setVideoFeedId( m_HisIdent.getOnlineId() );
    	m_VidCamThumbnail.setVideoFeedId( m_MyApp.getMyOnlineId() );
    	m_VidCamThumbnail.m_IsThumbnail = true;
    	m_VidCamLogic = new VidCamCtrlLogic( m_MyApp, this, m_HisIdent, m_VidCamView, m_VidCamThumbnail );
    	m_VidCamLogic.initializeVidCamLogic();
  	
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
          
    	m_HangupLayout 					= (LinearLayout)this.findViewById(R.id.hangup_session_frame);
    	m_OffersLayout 					= (LinearLayout)this.findViewById(R.id.offers_frame);
    	m_ResponseLayout 				= (LinearLayout)this.findViewById(R.id.response_frame);
  	
    	m_AcceptSessionButton 			= (Button)this.findViewById(R.id.accept_session_button);  	
    	m_AcceptSessionText 			= (TextView)this.findViewById(R.id.accept_text);
     	m_RejectSessionButton 			= (Button)this.findViewById(R.id.reject_session_button);
       	m_RejectSessionText	 			= (TextView)this.findViewById(R.id.reject_text);
    	
    	m_HangupButton 					= (Button)this.findViewById(R.id.hangup_session_button);
         	
    	m_VoicePhoneButton 				= (Button)this.findViewById(R.id.audio_phone_button);
    	m_PhoneState.setGuiWidgets(	m_HisIdent,
    								m_StatusMsgLabel,
    								m_OffersLayout, 
    								m_ResponseLayout, 
    								m_HangupLayout, 
    								m_VoicePhoneButton, 
    								m_AcceptSessionButton, m_AcceptSessionText,
    								m_RejectSessionButton, m_RejectSessionText,
    								m_HangupButton, 
    								m_VidCamLogic,
    								m_TodGameLogic );
    	 
    	m_VidChatButton 				= (Button)this.findViewById(R.id.video_chat_button);
    	m_VidChatState.setGuiWidgets(	m_HisIdent,
    									m_StatusMsgLabel,
										m_OffersLayout, 
										m_ResponseLayout, 
										m_HangupLayout, 
										m_VidChatButton, 
	    								m_AcceptSessionButton, m_AcceptSessionText,
	    								m_RejectSessionButton, m_RejectSessionText,
										m_HangupButton, 
										m_VidCamLogic,
										m_TodGameLogic );

    	m_TruthOrDareButton 			= (Button)this.findViewById(R.id.truth_or_dare_button);
    	m_TruthOrDareState.setGuiWidgets(	m_HisIdent,
    										m_StatusMsgLabel,
											m_OffersLayout, 
											m_ResponseLayout, 
											m_HangupLayout, 
											m_TruthOrDareButton, 
		    								m_AcceptSessionButton, m_AcceptSessionText,
		    								m_RejectSessionButton, m_RejectSessionText,
											m_HangupButton, 
											m_VidCamLogic,
											m_TodGameLogic ); 
    	
    	m_AcceptSessionButton.setOnClickListener( new OnClickListener() 
        {
        	public void onClick(View v) 
		 	{
  			   	m_MyApp.playButtonClick();
  			   	onAcceptSessionButtonClicked();
		 	}
		} );
  
    	m_RejectSessionButton.setOnClickListener( new OnClickListener() 
        {
        	public void onClick(View v) 
    	 	{
  			   	m_MyApp.playButtonClick();
            	onRejectSessionButtonClicked();
			}
        } );	
    	  
    	m_HangupButton.setOnClickListener( new OnClickListener() 
        {
        	public void onClick(View v) 
    	 	{
  			   	m_MyApp.playButtonClick();
            	onHangupButtonClicked();
    	 	}
        } );	
    	
    	setupCamView();
    	setupSound();
         
	 	m_OfferBarWidget = new OfferBarWidget( 		m_MyApp, 
													this,
													(LinearLayout)findViewById( R.id.offer_bar_layout ),
													(TextView)findViewById( R.id.total_offers_text ),			   									
													(Button)findViewById( R.id.offer_bar_icon ),
													(Button)findViewById( R.id.offer_bar_accept_button ),
													(Button)findViewById( R.id.offer_bar_reject_button ),
													(TextView)findViewById( R.id.offer_bar_toptext ),
													(TextView)findViewById( R.id.offer_bar_missed_call_label ),
													(TextView)findViewById( R.id.offer_bar_missed_call_count ),
													(TextView)findViewById( R.id.offer_bar_bottomtext )
													);
		
    	setCamCaptureActivity( AppletPeerMultiSession.this );
        setActivityReadyState( this );
        
	    NativeTxTo.fromGuiQuerySessionHistory( m_ActivityState.getFriendId().getOnlineIdHiPart(), m_ActivityState.getFriendId().getOnlineIdLoPart() );
	    
        if( m_OfferSessionLogic.sendOfferOrResponse() )
        {
        	m_OfferOrResponseIsSent = true;
        }
        
		m_InputBox.setupInputMode( Constants.eChatInputSelectMode ); // force turn off request for video feed

		m_MyEventHandler = new Handler() 
		{
		    @Override
		    public void handleMessage(Message msg) 
		    {
		    	switch(msg.what)
		    	{
		    	case Constants.MY_EVENT_ONCE_PER_SEC:
		    		//Log.i( LOG_TAG, "Rxed MY_EVENT_ONCE_PER_SEC" );
		    		onOncePerSecond();
		    		if( ( false == m_MyApp.getIsAppShuttingDown() )
		    			&& ( false == m_IsDestroyed ) )
		    		{
		    			m_MyEventHandler.sendEmptyMessageDelayed( Constants.MY_EVENT_ONCE_PER_SEC, 1000 );
		    		}
		    		
		    		break;    		
		    		
		    	case Constants.MY_EVENT_ONCE_PER_500MS:
		    		//Log.i( LOG_TAG, "Rxed MY_EVENT_ONCE_PER_500MS" );
		    		if( ( false == m_MyApp.getIsAppShuttingDown() )
		    			&& ( false == m_IsDestroyed ) )
		    		{
		    			onOncePer500ms();
		    			if( 1 == m_TickTock500ms )
		    			{
		    				m_TickTock500ms = 0;
		    				onOncePerSecond();
		    			}
		    			else
		    			{	
		    				m_TickTock500ms = 1;
		    			}
		    			
		    			m_MyEventHandler.sendEmptyMessageDelayed( Constants.MY_EVENT_ONCE_PER_500MS, 500 );
		    		}
		    		
		    		break;    		
		    	}
		    	
		    	super.handleMessage(msg);
		    }
		};
        
		
		if( m_HisIdent.isOnline() )
		{
			onContactOnline();
		}
		else
		{
			onContactOffline();
		}
		
		if( false == checkForSendAccess( false ) )
		{
			showReasonAccessNotAllowed( m_HisIdent, m_ePluginType );
		}

		setActivityReadyState( this );
	    showSpeakerButton( true );  
	    enableAudioControls( true );
		m_MyEventHandler.sendEmptyMessageDelayed( Constants.MY_EVENT_ONCE_PER_500MS, 500 );
		checkVolumeLevel();
	}
    
	//========================================================================
	public void onOncePer500ms() 
	{
		if( null != m_VidCamLogic )
		{
			m_VidCamLogic.onOncePer500ms();
		}
		
		if( null != m_InputBox )
		{
			m_InputBox.onOncePer500ms();
		}
    };
    
 	//========================================================================
 	protected void onOncePerSecond()
 	{  
  		// Log.d( LOG_TAG,  "ActivityListBase:onOncePerSecond" );
 		for( int i = 0; i < m_SessionStates.size(); i++ )
 		{
 			MultiSessionState curSessionState = m_SessionStates.get(i);
 			if( null != curSessionState )
 			{
 				curSessionState.onOncePerSecond();			
 			}			
 		}
 	}
	
	//========================================================================
	public void checkVolumeLevel()
	{
		//Connect Hardware volume buttons to adjust STREAM_MUSIC used for both sound effects and voice over IP
	    this.setVolumeControlStream(AudioManager.STREAM_MUSIC);
	    final AudioManager audioManager = (AudioManager)getSystemService( Context.AUDIO_SERVICE );
	    int maxVolume 	= audioManager.getStreamMaxVolume( AudioManager.STREAM_MUSIC );
	    int curVolume	= audioManager.getStreamVolume( AudioManager.STREAM_MUSIC ); 
	    if( curVolume < ( maxVolume / 2 ) )
	    {    	
        	Toast.makeText( this, R.string.warning_low_volume, Toast.LENGTH_LONG ).show();
	    }    
	    
	    showSpeakerButton( true );
	}
   
 	//========================================================================
    public void enableVideoControls( boolean enable )
    {
    	if( null != m_VidCamLogic )
    	{
    		m_VidCamLogic.showAllCamControls( enable );
    	}
    }
     
  	//========================================================================
    public void enableCamSourceControls( boolean enable )
    {
     	if( null != m_VidCamLogic )
     	{
     		m_VidCamLogic.enableCamSourceControls( enable );
     	}
    }
     
  	//========================================================================
    public void setCamViewToOfflineImage()
    {
     	if( null != m_VidCamLogic )
     	{
     		m_VidCamLogic.setCamViewImage( R.drawable.ic_cam_black );
     	}
    }
     
   	//========================================================================
    public void enableCamFeedControls( boolean enable )
    {
      	if( null != m_VidCamLogic )
      	{
      		m_VidCamLogic.enableCamFeedControls( enable );
      	}
    }
       
	//============================================================================
	boolean checkForSendAccess( boolean sendOfferIfPossible )
	{
		boolean canSend = false;
		if( m_HisIdent.isOnline() 
			&& m_HisIdent.isMyAccessAllowedFromHim( m_ePluginType ) )
		{
			canSend = true;
		}

		Log.d( LOG_TAG, "checkForSendAccess " + canSend );
		if( canSend != m_CanSend )
		{
			m_CanSend = canSend;
			m_InputBox.setCanSend( m_CanSend );
		}

		if( m_CanSend 
			&& sendOfferIfPossible 
			&& ( false == m_OfferOrResponseIsSent ) )
		{
			String offMsg = "Requested Text Chat Session with ";
			offMsg += m_HisIdent.getOnlineName();
			NativeRxFrom.toGuiStatusMessage( offMsg );
			//m_Engine.fromGuiMultiSessionAction( eMSessionActionChatSessionReq, m_HisIdent->getMyOnlineId(), 0, m_OfferSessionLogic.getOfferSessionId() );
			if( m_OfferSessionLogic.sendOfferOrResponse() )
			{
				m_OfferOrResponseIsSent = true;
			}
		}

		for( int i = 0; i < m_SessionStates.size(); i++ )
		{
			MultiSessionState curSessionState = m_SessionStates.get(i);
			if( null != curSessionState )
			{
				curSessionState.checkForSendAccess();			
			}			
		}

		return canSend;
	}

	//============================================================================
	void showReasonAccessNotAllowed( VxNetIdent netIdent, int ePluginType )
	{
		int accessState = netIdent.getMyAccessPermissionFromHim( ePluginType );
		String accessDesc = Constants.describePluginType( ePluginType );
		accessDesc += " with ";
		accessDesc += netIdent.getOnlineName();
		accessDesc +=  " ";

		if( Constants.ePluginAccessOk != accessState )
		{
			accessDesc +=  Constants.describePluginAccess( accessState );
			NativeRxFrom.toGuiStatusMessage( accessDesc );
			return;
		}

		if( false == netIdent.isOnline() )
		{
			accessDesc +=  " requires user be online ";
			NativeRxFrom.toGuiStatusMessage( accessDesc );
			return;
		}
	}
    
	//========================================================================
	private void setupCamView()
    {
        m_VidCamView = (ImageViewWithSizer)this.findViewById(R.id.reuse_cam_view);
        m_VidCamThumbnail = (ImageViewWithSizer)this.findViewById(R.id.reuse_cam_thumbnail);
        if( null != m_VidCamView )
        {
        	m_VidCamLogic = new VidCamCtrlLogic( m_MyApp, this, m_HisIdent, m_VidCamView, m_VidCamThumbnail );
        	m_VidCamLogic.initializeVidCamLogic();
        }
    }
    
	//========================================================================
    private void setupSound()
    {       
       	setupSpeakerControl();
	    m_MuteMicrophoneBut = (Button)this.findViewById( R.id.button_mic_mute );
	    if( null != m_MuteMicrophoneBut )
	    {
	    	setMicrophoneMuteIcon( NativeTxTo.fromGuiIsMicrophoneMuted() );
	    	m_MuteMicrophoneBut.setOnClickListener(new OnClickListener() 
	    	{
	            public void onClick(View v) 
	            {
	  			   	m_MyApp.playButtonClick();
	  			   	onMuteMicrophoneButClick(v);
	            }
	        });   
	    	
	    	if( NativeTxTo.fromGuiIsPluginInSession( Constants.ePluginTypeCamServer, m_MyApp.getMyOnlineId().m_u64IdHiPart, m_MyApp.getMyOnlineId().m_u64IdLoPart, 
	    					0, 0, 0 ) )
	    	{
	    		m_MuteMicrophoneBut.setVisibility( View.VISIBLE );
	    	}
	    }    	    
	}

	//========================================================================      
    private void onMuteMicrophoneButClick(View v) 
    {
       	boolean isMicrophoneMuted = !NativeTxTo.fromGuiIsMicrophoneMuted();
       	setStatusMessage( isMicrophoneMuted ? "Microphone is muted" :  "Microphone is unmuted" );
               	
    	setMicrophoneMuteIcon( isMicrophoneMuted );
    	NativeTxTo.fromGuiMuteMicrophone( isMicrophoneMuted );
    } 
    
 	//======================================================================== 
     private void setMicrophoneMuteIcon( boolean bIsMicrophoneMuted )
     {
         if( null != m_MuteMicrophoneBut )
         {
         	m_MuteMicrophoneBut.setBackgroundResource( bIsMicrophoneMuted ? R.drawable.button_microphone_cancel_normal : R.drawable.button_microphone_normal );       	
         }
     }

  	//========================================================================      
     public void showSpeakerButton( boolean showButton ) 
     {
      	if( null != m_MuteSpeakerBut )
      	{
      		m_MuteSpeakerBut.setVisibility( showButton ? View.VISIBLE : View.GONE );
      	}
     } 
      
  	//========================================================================
 	private void setupSpeakerControl()
    {
    	m_MuteSpeakerBut = (Button)this.findViewById(R.id.button_speaker_mute);
        if( null != m_MuteSpeakerBut )
        {
         	 m_MuteSpeakerBut.setOnClickListener(new OnClickListener() 
          	{
  	            public void onClick(View v) 
  	            {
  	  			   	m_MyApp.playButtonClick();
  	            	onMuteSpeakerButClick(v);
  	            }
          	});     
         	 
         	setSpeakerIcon( NativeTxTo.fromGuiIsSpeakerMuted() );
        }
    }	
  	
  	//========================================================================      
  	private void onMuteSpeakerButClick(View v) 
  	{
	   	boolean isSpeakerMuted = !NativeTxTo.fromGuiIsSpeakerMuted();
 	   	setSpeakerIcon( isSpeakerMuted );
	   	NativeTxTo.fromGuiMuteSpeaker( isSpeakerMuted );
   	}  
    
 	//========================================================================      
 	private void setSpeakerIcon( boolean isSpeakerMuted )
 	{
 	    if( null != m_MuteSpeakerBut )
 	    {
	    	m_MuteSpeakerBut.setBackgroundResource( isSpeakerMuted ? R.drawable.button_speaker_off_normal : R.drawable.button_speaker_on_normal );       	
 	    }
 	}
		
	//========================================================================
    // called from offer session logic
	public void onInSession( boolean isInSession )
	{
		m_IsInChatSession = isInSession;
		if( m_IsInChatSession )
		{
			sessionStatusMessage( R.string.in_session );
		}
		else
		{
			sessionStatusMessage( R.string.session_fail );
		}
		
		for( int i = 0; i < m_SessionStates.size(); i++ )
		{
			MultiSessionState curSessionState = m_SessionStates.get(i);
			if( null != curSessionState )
			{
				curSessionState.onInSession( m_IsInChatSession );			
			}			
		}
	}
	
	//========================================================================
    public void enableAudioControls( boolean enable )
    {
	   	int visibleState = enable ? View.VISIBLE : View.GONE;   	 
	   	if( null != m_MuteMicrophoneBut )
	   	{
	   		m_MuteMicrophoneBut.setVisibility( visibleState ); 
	   	}    	 
    }
	
	//========================================================================
	void onAcceptSessionButtonClicked()
	{
		MultiSessionState curSessionState = getMSessionState( m_CurMSessionType );
		if( null != curSessionState )
		{
			curSessionState.onAcceptSessionButtonClicked();			
		}		
	}
	
	//========================================================================
	void onRejectSessionButtonClicked()
	{
		MultiSessionState curSessionState = getMSessionState( m_CurMSessionType );
		if( null != curSessionState )
		{
			curSessionState.onRejectSessionButtonClicked();			
		}		
	}
	
	//========================================================================
	void onHangupButtonClicked()
	{
		MultiSessionState curSessionState = getMSessionState( m_CurMSessionType );
		if( null != curSessionState )
		{
			curSessionState.onHangupButtonClicked();			
		}		
	}
	
	//========================================================================
	void showOfflineMsg()
	{
        Toast.makeText(AppletPeerMultiSession.this, m_HisIdent.m_as8OnlineName + " Is Offline.", Toast.LENGTH_SHORT).show();		
	}
	
	//========================================================================
    @Override
    protected void onPause() 
    {
        //setToGuiMessageReadyState( null );
        //setActivityReadyState( null );
    	m_InputBox.setupInputMode( Constants.eChatInputSelectMode ); // force turn off request for video feed and stop vid recodings
     	super.onPause();    	
        m_MyApp.setActivityPause( true );
    }
	 
	//========================================================================
    @Override
    protected void onResume() 
    {
        m_MyApp.setActivityPause( false );
        super.onResume();    
    	//setActivityReadyState( this );
        //setToGuiMessageReadyState( this );
    }

	//========================================================================
    @Override
    protected void onDestroy() 
    {
    	Log.i( LOG_TAG, "AppletPeerMultiSession:onDestroy" );
    	m_VidCamLogic.activityOnDestroy();
    	m_IsDestroyed = true;
		if( null != m_OfferBarWidget )
		{
			m_OfferBarWidget.shudownOfferBarWidget();
		}

    	VxGUID myId = m_MyApp.getMyOnlineId();
    	NativeTxTo.fromGuiWantMediaInput( myId.m_u64IdHiPart, myId.m_u64IdLoPart, Constants.eMediaInputVideoJpgSmall, false );
    	if( null != m_HisIdent )
    	{
    		NativeTxTo.fromGuiWantMediaInput( m_HisIdent.m_u64IdHiPart, m_HisIdent.m_u64IdLoPart, Constants.eMediaInputVideoJpgSmall, false );
    	}
    	
    	setToGuiMessageReadyState( null );
    	m_ChatAdapter.activityOnStop();
        setActivityReadyState( null );
        m_OfferSessionLogic.onStop();
       	NativeRxFrom.handleHaveTextOffer( m_HisIdent, false );
       	super.onDestroy();
     }
     
  	//========================================================================
    protected void setToGuiMessageReadyState( Activity curActivity )
    {
 		try 
 		{
 			if( ( null == curActivity ) || ( null == m_StatusMsgLabel ) )
 			{
 				NativeRxFrom.wantToGuiMessageClient( this, false );
 			}
 			else
 			{
 				NativeRxFrom.wantToGuiMessageClient( this, true );
 			}
 		} 
 		catch (InterruptedException e) 
 		{
 			e.printStackTrace();
 		}
    }
    
  	//========================================================================
     protected void setActivityReadyState( Activity curActivity )
     {
 		try 
 		{
 			if( null == curActivity )
 			{
 				NativeRxFrom.wantAssetCallbackClient( this, false );	
 				NativeRxFrom.wantToGuiActivityClient( this, false );
 			}
 			else
 			{
 				NativeRxFrom.wantAssetCallbackClient( this, true );
 				NativeRxFrom.wantToGuiActivityClient( this, true );
 			}
 		} 
 		catch (InterruptedException e) 
 		{
 			e.printStackTrace();
 		}
    }
     
	//============================================================================
	//! show message box to user
	void ErrMsgBox( boolean bExitWhenClicked, String strMsg )
	{
		final boolean exitWhenClicked = bExitWhenClicked;
		AlertDialog.Builder builder = new AlertDialog.Builder(this); 
		builder.setMessage(strMsg) 
		       .setPositiveButton("Ok", new DialogInterface.OnClickListener() { 
		           public void onClick(DialogInterface dialog, int id) { 
		               // Handle Ok 
		        	   if( exitWhenClicked )
		        	   {
		     			   m_MyApp.playButtonClick();
		        		   finish();
		        	   }
		           } 
		       }) 
		       /*
		       .setNegativeButton("Cancel", new DialogInterface.OnClickListener() { 
		           public void onClick(DialogInterface dialog, int id) { 
		               // Handle Cancel 
		     		   m_MyApp.playButtonClick();
		           } 
		       }) 
		       */
		       .create(); 	
	}
    
 	//========================================================================
    public void toGuiPlayVideoFrame( VxGUID guidId, byte[] pu8Jpg, int jpgDataLen, int motion0to100000 )
    {
    	if( (( null != m_VidChatState ) && m_VidChatState.isInSession() )
    		||	(( null != m_TruthOrDareState ) && m_TruthOrDareState.isInSession() ) )
    	{
    		if( null == pu8Jpg )
    		{
    			Log.e( LOG_TAG, "toGuiPlayVideoFrame NULL pu8Jpg " );
    			return;
    		}
    		
       		if( null == guidId )
    		{
    			Log.e( LOG_TAG, "toGuiPlayVideoFrame NULL guidId " );
    			return;
    		}
    		
      		if( null == m_VidCamView )
    		{
    			Log.e( LOG_TAG, "toGuiPlayVideoFrame NULL m_VidCamView " );
    			return;
    		}
      		
      		if( null == m_VidCamThumbnail )
    		{
    			Log.e( LOG_TAG, "toGuiPlayVideoFrame NULL m_VidCamThumbnail " );
    			return;
    		}
      		
      		m_VidCamLogic.toGuiPlayVideoFrame( guidId, pu8Jpg, jpgDataLen, motion0to100000 );
     	}    	
    }
    
	//========================================================================
	private void initMessageInputToolBox()
	{
		m_InputBox = (ChatMessageInputToolBox) findViewById(R.id.chatMessageInputToolBox);
		m_InputBox.setUserInputCallback( this );
		
		m_InputBox.setOnOperationListener( new ChatOnOperationListener() 
		{			
			@Override
			public void chatSend( String content ) 
			{	
				Log.d( LOG_TAG, "Sending Text " +  content );
				AssetGuiInfo assetInfo = createAndFillNewAssetGuiInfo();
				assetInfo.setChatText( content );
				NativeTxTo.fromGuiAssetAction(Constants.eAssetActionAddAssetAndSend, assetInfo, 0 );
				m_InputBox.setupInputMode( Constants.eChatInputSelectMode );
				m_ListView.setSelection(m_ListView.getBottom());
			}
			
			@Override
			public void chatSelectedFace(String content) 
			{	
				Log.d( LOG_TAG, "Sending Face " +  content );
				AssetGuiInfo assetInfo = createAndFillNewAssetGuiInfo();
				assetInfo.setChatFace( content );				
				NativeTxTo.fromGuiAssetAction( Constants.eAssetActionAddAssetAndSend, assetInfo, 0 );
				m_InputBox.setupInputMode( Constants.eChatInputSelectMode );
				m_ListView.setSelection(m_ListView.getBottom());
			}			
			
			@Override
			public void chatSelectedFuncation(int index) 
			{	
				System.out.println("===============" + index);
				
				switch (index) {
				case 0:
					//do some thing
					break;
				case 1:
					//do some thing
					break;
					
				default:
					break;
				}
				
				Toast.makeText(AppletPeerMultiSession.this, "Do some thing here, index :" + index, 1000).show();				
			}		
		});
		
		ArrayList<String> faceNameList = new ArrayList<String>();
		for( int x = 1; x <= 9; x++ )
		{
			faceNameList.add("face"+x);
		}
		
		ArrayList<String> faceNameList1 = new ArrayList<String>();
		for(int x = 10; x <= 19; x++)
		{
			faceNameList1.add("face"+x);
		}
		
		
		ArrayList<String> faceNameList2 = new ArrayList<String>();
		for(int x = 20; x <= 32; x++)
		{
			faceNameList2.add("face"+x);
		}
		
		Map<Integer, ArrayList<String>> faceData = new HashMap<Integer, ArrayList<String>>();
		faceData.put(R.drawable.face_catagory3, faceNameList1);
		faceData.put(R.drawable.face_catagory2, faceNameList2 );
		faceData.put(R.drawable.face_catagory1, faceNameList );
		m_InputBox.setFaceData(faceData);	
		
		List<ChatOption> functionData = new ArrayList<ChatOption>();
		for(int x = 0; x < 5; x++)
		{
			ChatOption takePhotoOption = new ChatOption(this, "Take", R.drawable.take_photo);
			ChatOption galleryOption = new ChatOption(this, "Gallery", R.drawable.gallery);
			functionData.add(galleryOption);
			functionData.add(takePhotoOption);
		}
		
		m_InputBox.setFunctionData( functionData );
		m_InputBox.setInputState( m_ActivityState );
		m_InputBox.setupInputMode( Constants.eChatInputSelectMode );
	}
	
	//========================================================================
	private AssetGuiInfo createAndFillNewAssetGuiInfo()
	{
		AssetGuiInfo assetInfo = new AssetGuiInfo();
		fillAssetGuiInfo( assetInfo );
		return assetInfo;
	}
	
	//========================================================================
	private void fillAssetGuiInfo( AssetGuiInfo assetInfo )
	{
		assetInfo.m_CreationTime = VxTimeUtil.getCurrentTime();
		assetInfo.setHistoryId( m_ActivityState.getFriendId() );
		assetInfo.setCreatorId( m_ActivityState.getMyId() );
		assetInfo.generateNewUniqueId();
	}
	
	//========================================================================
	private void addChatMessage( ChatMessage message )
	{
		final  ArrayList<ChatMessage> messageList = new  ArrayList<ChatMessage>();
		try 
		{
            m_AddChatMsgListMutex.acquire();
		} 
		catch( InterruptedException e ) 
		{
			e.printStackTrace();
		}	

		messageList.add( message );
        m_AddChatMsgListMutex.release();

		try 
		{
			runOnUiThread(new Runnable() 
			{	
				@Override
				public void run() 
				{
					try 
					{
                        m_AddChatMsgListMutex.acquire();
					} 
					catch( InterruptedException e ) 
					{
						e.printStackTrace();
					}	
					
			    	for( ChatMessage chatMsg:  messageList )
			    	{
			    		m_ChatAdapter.addChatMsg(chatMsg);
			    	}

			    	messageList.clear();
                    m_AddChatMsgListMutex.release();
                    //scrollToBottomOfList();
				}
			});
		} 
		catch (Exception e) 
		{
			e.printStackTrace();
		}
	}
	
	//========================================================================
	private void removeChatMessage( VxGUID messageId )
	{		
		final Semaphore removeChatMsgListMutex = new Semaphore(1); 
		final  ArrayList<VxGUID> messageIdList = new  ArrayList<VxGUID>();
		try 
		{
			removeChatMsgListMutex.acquire();
		} 
		catch( InterruptedException e ) 
		{
			e.printStackTrace();
		}	

		messageIdList.add( messageId );
		removeChatMsgListMutex.release();
		
		new Thread(new Runnable() 
		{			
			@Override
			public void run() 
			{
				try 
				{
					runOnUiThread(new Runnable() 
					{	
						@Override
						public void run() 
						{
							try 
							{
								removeChatMsgListMutex.acquire();
							} 
							catch( InterruptedException e ) 
							{
								e.printStackTrace();
							}	
							
					    	for( VxGUID chatMsgId:  messageIdList )
					    	{
					    		m_ChatAdapter.removeChatMessageById(chatMsgId);
					    	}

					    	removeChatMsgListMutex.release();
							m_ChatAdapter.notifyDataSetChanged();
						}
					});
				} 
				catch (Exception e) 
				{
					e.printStackTrace();
				}
			}
		}).start();
	}
	
	//========================================================================
	private void initListView()
	{
		m_ListView = (ListView) findViewById( R.id.chatMessageListview );	
        m_ChatAdapter = new ChatMessageAdapter( m_MyApp, this, m_ChatMessageList );
        m_ListView.setAdapter( m_ChatAdapter );
        m_ListView.setOnTouchListener(new OnTouchListener()
        {
            @Override
            public boolean onTouch(View v, MotionEvent event)
            {
            	//Log.d( LOG_TAG, "m_ListView onTouch" );
                m_InputBox.hide();
                return false;
            }
        });

        m_ChatAdapter.notifyDataSetChanged();
	}
	
	//========================================================================
	@Override
	public void toGuiAssetAdded( AssetGuiInfo assetInfo ) 
	{
		if( null == m_ActivityState )
			return;
		if( assetInfo.getHistoryId().equals(m_ActivityState.getFriendIdent().getMyOnlineId() ) )
		{
			ChatMessage message = new ChatMessage( m_ActivityState, assetInfo, m_ChatAdapter );
			message.setIsSend( assetInfo.getCreatorId().equals( m_ActivityState.getMyId() ) );
			addChatMessage( message );
		}	
	}

	//========================================================================
	@Override
	public void toGuiSessionHistory( AssetGuiInfo assetInfo ) 
	{
		toGuiAssetAdded( assetInfo ); // action is the same
	}

	//========================================================================
	@Override
	public void toGuiAssetAction( int assetAction, VxGUID assetId, int actionParam ) 
	{
		//Log.i( LOG_TAG, "got toGuiAssetAction assetAction=" + String.valueOf( assetAction ) );
		try 
		{
			final int assetActionFinal 		= assetAction;
			final VxGUID assetIdFinal 		= assetId;
			final int actionParamFinal 		= actionParam;
			
			runOnUiThread(new Runnable() 
			{	
				@Override
				public void run() 
				{
					if( ( Constants.eAssetActionShreadFile == assetActionFinal )
							|| ( Constants.eAssetActionRemoveFromAssetMgr == assetActionFinal )
							|| ( Constants.eAssetActionRemoveFromHistory == assetActionFinal ) )
					{
						removeChatMessage( assetIdFinal );
					}
					else
					{
						m_ChatAdapter.doToGuiAssetAction( assetActionFinal, assetIdFinal, actionParamFinal );
					}
				}
			});
		} 
		catch (Exception e) 
		{
			e.printStackTrace();
		}
	}

	//========================================================================
	@Override
	public void toGuiRxedPluginOffer( GuiOfferSession offerSession ) 
	{
		m_OfferSessionLogic.toGuiRxedPluginOffer( offerSession );
	}

	//========================================================================
	@Override
	public void toGuiRxedOfferReply( GuiOfferSession offerSession ) 
	{
		m_OfferSessionLogic.toGuiRxedOfferReply( offerSession );
	}

	//========================================================================
	@Override
	public void toGuiPluginSessionEnded( GuiOfferSession offerSession ) 
	{
		m_OfferSessionLogic.toGuiPluginSessionEnded( offerSession );
	}

	//============================================================================
	@Override
	public void toGuiContactOnline( VxNetIdent hisIdent, boolean newContact ) 
	{
		m_OfferSessionLogic.toGuiContactOnline( hisIdent, newContact );
	}

 	//========================================================================
	@Override
	public void onContactOnline()
	{
		m_HisIdent.setIsOnline( true );
		m_InputBox.setCanSend( checkForSendAccess( false ) );	
	}

	//============================================================================
	@Override
	public void toGuiContactOffline( VxNetIdent hisIdent ) 
	{
		m_OfferSessionLogic.toGuiContactOffline( hisIdent );
	}

 	//========================================================================
	@Override
	public void onContactOffline()
	{
		m_HisIdent.setIsOnline( false );
		m_InputBox.setCanSend( checkForSendAccess( false ) );	
		NativeRxFrom.toGuiStatusMessage( m_HisIdent.getOnlineName() + " went offline-chat disabled" );
	}

	//============================================================================
	@Override
	public void toGuiMultiSessionAction( int eMSessionActionIn, VxGUID onlineId, int actionParamIn ) 
	{
		if( ( null != m_HisIdent ) 
			&& onlineId.equals( m_HisIdent.getOnlineId() ) )
		{
			try 
			{
				m_CurMSessionType 				= actionParamIn;
				final int eMSessionAction		= eMSessionActionIn;
				final int actionParam 			= actionParamIn;
				runOnUiThread(new Runnable() 
				{	
					@Override
					public void run() 
					{
						if( MultiSessionState.eMSessionActionChatSessionAccept == eMSessionAction )
						{
							onInSession( true );
						}
						else if( MultiSessionState.eMSessionActionChatSessionReject == eMSessionAction )
						{
							onInSession( false );
						}

						MultiSessionState sessionState = getMSessionState( actionParam );
						if( null != sessionState )
						{
							Log.d(LOG_TAG, "handleMultiSessionAction start" );
							sessionState.handleMultiSessionAction( eMSessionAction );
							Log.d(LOG_TAG, "handleMultiSessionAction done" );
						}		
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
	MultiSessionState getMSessionState( int eMSessionType )
	{
		switch( eMSessionType )
		{
		case eMSessionTypePhone:
			return m_PhoneState;
		case eMSessionTypeVidChat:
			return m_VidChatState;
		case eMSessionTypeTruthOrDare:
			return m_TruthOrDareState;
		default:
			return null;
		}
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
	public void toGuiInstMsg(VxNetIdent friendIdent, int ePluginType, String strMsg) 
	{
	}
	
  	//========================================================================
	public void sessionStatusMessage( String userMsg )
	{
		if( null != m_StatusMsgLabel )
		{
			m_StatusMsgLabel.setText( userMsg );
		}		
	}

  	//========================================================================
	public void sessionStatusMessage( int stringResId )
	{
		if( null != m_StatusMsgLabel )
		{
			m_StatusMsgLabel.setText( stringResId );
		}		
	}

	//========================================================================
	void setStatusMessage( String strMsgIn ) 
	{
		if( null != m_StatusMsgLabel )
		{
			final String strMsg = strMsgIn;
			runOnUiThread(new Runnable() {
				public void run() 
				{
					m_StatusMsgLabel.setText(strMsg);
				}
			});
		}
	}

  	//========================================================================
	@Override
	public void toGuiUserMessage( String userMsg )
	{
		if( null != m_StatusMsgLabel )
		{
			m_StatusMsgLabel.setText( userMsg );
		}		
	}

  	//========================================================================
	@Override
	public void toGuiUserMessage( int stringResId )
	{
		if( null != m_StatusMsgLabel )
		{
			m_StatusMsgLabel.setText( stringResId );
		}		
	}
	
  	//========================================================================
	@Override
	public void toGuiStatusMessage( String statusMsg )
	{
		setStatusMessage( statusMsg );
	}

 	//========================================================================
	@Override
	public void onSessionStateChange( int eSessionState )
	{
			
	}

 	//========================================================================
	@Override
	public void onSessionActivityShouldExit()
	{
		finish();	
	}

 	//========================================================================
	@Override
	public void toGuiUserInputButtonClicked()
	{
		m_InputBox.setCanSend( checkForSendAccess( true ) );	
	}

 	//========================================================================
	@Override
	public void toGuiGalleryButtonClicked( AssetGuiInfo assetGuiInfo )
	{
		m_GalleryPickAsset = assetGuiInfo;
    	Intent myIntent = new Intent( AppletPeerMultiSession.this, ActivityPickImage.class );
    	myIntent.putExtra( Constants.PARAM_FILE_NAME, m_GalleryPickAsset.getAssetName() ); 	
    	startActivityForResult( myIntent, Constants.ACTIVITY_PICK_IMAGE );
	}	

	//========================================================================
	@Override
	public void onActivityResult(int requestCode, int resultCode, Intent data) 
	{
		super.onActivityResult(requestCode, resultCode, data);
		Log.d(LOG_TAG, "onActivityResult result code " + resultCode);
			
		switch (requestCode) 
		{
		case Constants.ACTIVITY_PICK_IMAGE:
			if (resultCode == Activity.RESULT_OK) 
		    {
				Log.d( LOG_TAG, "onActivityResult ACTIVITY_PICK_IMAGE" );
				Bundle extras = data.getExtras(); 
		        if( null != extras ) 
		        { 
		        	String fileName 	= extras.getString( Constants.PARAM_FILE_NAME );
		        	long fileLen		= VxFileUtil.getFileLen( fileName );
		        	if( 0 != fileLen )
		        	{
		        		if( null != m_GalleryPickAsset )
		        		{
		        			Log.d(LOG_TAG, "Choose Image " +  fileName + " len " + fileLen );
		        			m_GalleryPickAsset.setAssetName( fileName );
		        			m_GalleryPickAsset.setAssetLength( fileLen );	        			
							NativeTxTo.fromGuiAssetAction( Constants.eAssetActionAddAssetAndSend, m_GalleryPickAsset, 0 );
		        		}
		        		else
		        		{
		        			Log.d(LOG_TAG, "NULL m_GalleryPickAsset in Choose Image " +  fileName);
				        	Toast.makeText( this, R.string.error_bad_activity_param, Toast.LENGTH_LONG ).show();
		        		}
		        	}
			        else
			        {
			        	Log.d(LOG_TAG, "0 File Len in Choose Image " +  fileName);
			        	Toast.makeText( this, R.string.error_file_does_not_exist, Toast.LENGTH_LONG ).show();
			        }
		        }
		        else
		        {
					Log.d(LOG_TAG, "NULL EXTRA in Choose Image" );
		        	Toast.makeText( this, R.string.error_bad_activity_param, Toast.LENGTH_LONG ).show();
		        }
		    }
			else
			{
				Log.d(LOG_TAG, "Choose Image Canceled By User" );
				Toast.makeText( this, R.string.choose_image_canceled_by_user, Toast.LENGTH_SHORT ).show();
			}
			
			break;
			
		case Constants.ACTIVITY_VIEW_PHOTO:
			Log.d( LOG_TAG, "onActivityResult ACTIVITY_VIEW_VIDEO_FILE" );
			break;
			
		case Constants.ACTIVITY_VIEW_VIDEO_FILE:
			Log.d( LOG_TAG, "onActivityResult ACTIVITY_VIEW_VIDEO_FILE" );
			break;
			
		default:
			break;
		}
		
		setCamCaptureActivity( AppletPeerMultiSession.this );
	}
	
	   
	//========================================================================
    protected void setCamCaptureActivity( Activity curActivity )
    {
       	if( null != m_MyApp.getCamCaptureMgr() )
    	{
       		m_MyApp.getCamCaptureMgr().setCamActivity( curActivity, R.id.cam_preview_frame, R.id.no_preview_image );
    	}
    }

  	//========================================================================
	@Override
	public void doToGuiPluginStatus( int ePluginType, int statusType, int statusValue )
	{
		if( ( Constants.ePluginTypeCamServer == ePluginType )
			&& ( 1 == statusType )
			&& ( null != m_TotalCamClientsText ) )
		{
			if( statusValue < 0 )
			{
				m_TotalCamClientsText.setText( "" );
			}
			else
			{
				String valueAsText = String.format( Locale.getDefault(), "%d", statusValue ); 
				m_TotalCamClientsText.setText( valueAsText );
			}		
		}	
	}

  	//========================================================================
	@Override
	public void toGuiPluginLogMessage( int ePluginType, String strLogMsg )
	{		
	}
};


