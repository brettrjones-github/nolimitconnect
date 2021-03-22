//============================================================================
// Copyright (C) 2011 Brett R. Jones

package com.p2pgotv.gotvstation;

import android.os.Bundle;
import android.util.Log;
import android.view.WindowManager;
import android.widget.Button;
import android.widget.EditText;

public class AppletPeerVideoPhone extends AppletPeerGuiBase
{
	private static final String LOG_TAG = "AppletPeerVideoPhone:";
	
	//=== vars ===//
	private EditText				m_InstMsgHistoryText 		= null;
	private EditText				m_InstMsgSayText 			= null;
	private Button 					m_InstMsgSayBut 			= null;
	private Button 					m_InstMsgShredBut 			= null;
	private InstMsgLogic			m_InstMsgLogic				= null;
	private ImageViewWithSizer 		m_VidCamView				= null;
	private ImageViewWithSizer 		m_VidCamThumbnail			= null;
	
	//========================================================================
    @Override
    public void onCreate(Bundle savedInstanceState) 
    {
    	Log.d(LOG_TAG, "onCreate");  
 		m_MyApp 							= (MyApp) this.getApplication();
        m_HisIdent 							= m_MyApp.getCurrentFriend();
		m_ePluginType 						= Constants.ePluginTypeVideoPhone;
        
        setContentView(R.layout.activity_to_friend_video_phone);  
        super.onCreate(savedInstanceState);
	    setPageTitle( R.string.activity_to_friend_video_phone_name );
	    getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);
	    getWindow().setSoftInputMode(WindowManager.LayoutParams.SOFT_INPUT_STATE_HIDDEN); // avoid keyboard pop up on startup
	    m_OfferSessionLogic.setOfferText("Video Phone");
	    m_OfferSessionLogic.setUserData( 0 );
  
		m_InstMsgHistoryText 			= (EditText)this.findViewById(R.id.inst_msg_history_text);
		m_InstMsgSayText 				= (EditText)this.findViewById(R.id.inst_msg_say_text);
		m_InstMsgSayBut 				= (Button)this.findViewById(R.id.inst_msg_say_button);
		m_InstMsgShredBut 				= (Button)this.findViewById(R.id.inst_msg_shred_button);		
		m_InstMsgLogic					= new InstMsgLogic( m_MyApp, m_ePluginType, m_HisIdent, m_InstMsgSayText, m_InstMsgHistoryText, m_InstMsgSayBut, m_InstMsgShredBut );
        
        m_VidCamView					= getVidCamView();
        m_VidCamThumbnail				= getVidCamThumbnail();
        m_VidCamView.setVideoFeedId( m_HisIdent.getOnlineId() );
        m_VidCamThumbnail.setVideoFeedId( m_MyApp.getMyOnlineId() );
        m_VidCamThumbnail.m_IsThumbnail = true;
          
        enableAudioControls( true );
		showSpeakerButton( true );
        sendOfferOrResponse();
        checkVolumeLevel();
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

}

