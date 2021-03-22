//============================================================================
// Copyright (C) 2011 Brett R. Jones

package com.p2pgotv.gotvstation;

import android.util.*;
import android.view.*;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.EditText;
import android.os.Bundle;

public class AppletPeerRequestRelay extends AppletPeerGuiBase 
{
	// === constants ===//
	public static final String LOG_TAG = "ActivityRequestRelay:";

	// === vars ===//
	public Button m_oTestBut = null;
	public Button m_oUseAsRelayBut = null;
	public Button m_oDontUseAsRelayBut = null;

	public EditText m_LogText = null;

	//========================================================================
	/** Called when the activity is first created. */
	@Override
	public void onCreate(Bundle savedInstanceState) 
	{
 		m_MyApp 							= (MyApp) this.getApplication();
        m_HisIdent 							= m_MyApp.getCurrentFriend();
		m_ePluginType 						= Constants.ePluginTypeRelay;
         
		setContentView(R.layout.activity_to_friend_request_proxy);
		super.onCreate(savedInstanceState);
	    setPageTitle( R.string.activity_to_friend_request_proxy_name );


		Log.i(LOG_TAG, "onCreate called " );

		this.m_LogText = (EditText) this.findViewById(R.id.proxy_test_log);

		this.m_oTestBut = (Button) this.findViewById(R.id.button_test);
		this.m_oTestBut.setOnClickListener(new OnClickListener() 
		{
			public void onClick(View v) 
			{
	        	m_MyApp.playButtonClick();
				onTestButClick(v);
			}
		});

		this.m_oUseAsRelayBut = (Button) this.findViewById(R.id.button_proxy_yes);
		this.m_oUseAsRelayBut.setOnClickListener(new OnClickListener() 
		{
			public void onClick(View v) 
			{
	        	m_MyApp.playButtonClick();
				onUseAsRelayButClick(v);
			}
		});

		this.m_oDontUseAsRelayBut = (Button) this.findViewById(R.id.button_proxy_no);
		this.m_oDontUseAsRelayBut.setOnClickListener(new OnClickListener() 
		{
			public void onClick(View v) 
			{
	        	m_MyApp.playButtonClick();
				onDontUseAsRelayButClick(v);
			}
		});
		
		Log.i(LOG_TAG, "onCreate end ");
        //sendOfferOrResponse();
	}

	//========================================================================
	public void onTestButClick(View v) 
	{
		VxGUID lclSessionId = getLclSessionId();
		boolean bOffline = NativeTxTo.fromGuiMakePluginOffer( 	m_ePluginType,
																m_HisIdent.m_u64IdHiPart, m_HisIdent.m_u64IdLoPart, 0,
																"ActionTest", "",
																lclSessionId.m_u64IdHiPart,
																lclSessionId.m_u64IdLoPart );
		Log.i( LOG_TAG, "onTestButClick result " + bOffline );
		if( bOffline ) 
		{
			//showOfflineMsg( m_HisIdent );
		}
	}

	//========================================================================
	public void onUseAsRelayButClick(View v) 
	{
		m_HisIdent.setIsMyPreferedRelay(true);
		NativeRxFrom.toGuiStatusMessage("Relay Selected " + m_HisIdent.getOnlineName() );
		NativeTxTo.fromGuiUseRelay(m_HisIdent.m_u64IdHiPart,
				m_HisIdent.m_u64IdLoPart, true);
		finish();
	}

	//========================================================================
	public void onDontUseAsRelayButClick(View v) {
		m_HisIdent.setIsMyPreferedRelay(false);
		NativeTxTo.fromGuiUseRelay(m_HisIdent.m_u64IdHiPart,
				m_HisIdent.m_u64IdLoPart, false);
		finish();
	}

	//========================================================================
	public void appendLogMsg(int iPluginNum, String strMsg) {
		Log.i(LOG_TAG, "appendLogMsg called text obj " + m_LogText
				+ " log msg " + strMsg);
		m_LogText.append(strMsg);
	}

  	//========================================================================
	@Override
	public void toGuiPluginLogMessage( int ePluginType, String strLogMsg )
	{
		if( ePluginType == getPluginType() )
		{
			final String strFinalLogMsg = strLogMsg;
			this.runOnUiThread( new Runnable() 
			{
				public void run() 
				{
					m_LogText.append( strFinalLogMsg );
				}
			});		
		}
	}
}

