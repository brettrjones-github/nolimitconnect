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

import android.view.*;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.EditText;

import android.os.Bundle;

public class AppletPeerVoicePhone extends AppletPeerGuiBase 
{
	//=== constants ===//
	public static final String LOG_TAG = "ActivityVoicePhone:";
	
	//=== vars ===//
	private EditText				m_InstMsgHistoryText 		= null;
	private EditText				m_InstMsgSayText 			= null;
	private Button 					m_InstMsgSayBut 			= null;
	private Button 					m_InstMsgShredBut 			= null;
	private InstMsgLogic			m_InstMsgLogic				= null;
	
	private Button 					m_HangupPhoneBut 			= null;
	
    //======================================================================== 
    @Override
    public void onCreate(Bundle savedInstanceState) 
    {
 		m_MyApp 							= (MyApp) this.getApplication();
        m_HisIdent 							= m_MyApp.getCurrentFriend();
		m_ePluginType 						= Constants.ePluginTypeVoicePhone;
        setContentView(R.layout.activity_to_friend_voice_phone);
        super.onCreate(savedInstanceState);
	    setPageTitle( R.string.activity_to_friend_voice_phone_name );
	    getWindow().setSoftInputMode(WindowManager.LayoutParams.SOFT_INPUT_STATE_HIDDEN); // avoid keyboard pop up on startup
        m_OfferSessionLogic.setOfferText("Voice Phone");
        m_OfferSessionLogic.setUserData( 0 );

		m_InstMsgHistoryText 			= (EditText)this.findViewById(R.id.inst_msg_history_text);
		m_InstMsgSayText 				= (EditText)this.findViewById(R.id.inst_msg_say_text);
		m_InstMsgSayBut 				= (Button)this.findViewById(R.id.inst_msg_say_button);		
		m_InstMsgShredBut 				= (Button)this.findViewById(R.id.inst_msg_shred_button);		
		m_InstMsgLogic					= new InstMsgLogic( m_MyApp, m_ePluginType, m_HisIdent, m_InstMsgSayText, m_InstMsgHistoryText, m_InstMsgSayBut, m_InstMsgShredBut  );
 
		m_HangupPhoneBut				= (Button)this.findViewById(R.id.voice_phone_hangup_button);	
		if( null != m_HangupPhoneBut )
		{
			m_HangupPhoneBut.setOnClickListener(new OnClickListener() 
	    	{
	            public void onClick(View v) 
	            {
		        	m_MyApp.playButtonClick();
	            	onBackPressed();
	            }
	        });     		
		}
		
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

    
}
