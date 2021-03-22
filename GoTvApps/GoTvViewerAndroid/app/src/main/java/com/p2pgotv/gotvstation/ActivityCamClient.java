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

import android.os.Bundle;
import android.util.Log;
import android.view.WindowManager;

public class ActivityCamClient extends AppletPeerGuiBase
{
	//=== constants ===//
	public static final String LOG_TAG = "ActivityCamClient:";
	
	//=== vars ===//
	private MyApp 				m_MyApp 					= null;
	private VxGUID 				m_LclSessionId				= new VxGUID();
	private boolean 			m_MicWasMuted				= false;

	//========================================================================
    @Override
    public void onCreate(Bundle savedInstanceState) 
    {
    	Log.d(LOG_TAG, "onCreate");  
    	m_ePluginType 						= Constants.ePluginTypeCamServer;
 		m_MyApp 							= (MyApp) this.getApplication();
    	m_HisIdent 							= m_MyApp.getCurrentFriend();

        setContentView( R.layout.activity_web_cam_client );  	
        super.onCreate(savedInstanceState);
        setPageTitle( R.string.activity_cam_client );
        getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);
        enableVideoControls( true );
        enableCamSourceControls( false );
      
        m_OfferSessionLogic.setOfferText("View Cam");
        m_OfferSessionLogic.setUserData( 0 );
        //sendOfferOrResponse();
    	if( null != m_HisIdent )
    	{
    		m_LclSessionId.initializeWithNewGUID();
    		if( null != m_VidCamView )
    		{
    			m_VidCamView.requestVideoFeed( m_HisIdent.getMyOnlineId() );    			
    		}
    		
 			NativeTxTo.fromGuiStartPluginSession( 	m_ePluginType, 
					m_HisIdent.getMyOnlineId().m_u64IdHiPart,  m_HisIdent.getMyOnlineId().m_u64IdLoPart,
					m_LclSessionId.m_u64IdHiPart, m_LclSessionId.m_u64IdLoPart, 
					0 );
   		
    	}
    	
    	showSpeakerButton( true );
    	m_MicWasMuted = NativeTxTo.fromGuiIsMicrophoneMuted();
    	if( false == m_MyApp.isWebCamServerRunning() )
    	{
    		showMicrophoneButton( false );
    		NativeTxTo.fromGuiMuteMicrophone( true );
    	}
    }

	//========================================================================    
    @Override
    protected void onStop() 
    {
   		if( null != m_VidCamView )
		{
			m_VidCamView.stopVideoFeed();    			
		}
   		
   		NativeTxTo.fromGuiMuteSpeaker( false );
    	if( null != m_HisIdent )
    	{
 
			NativeTxTo.fromGuiStopPluginSession( 	m_ePluginType, 
					m_HisIdent.getMyOnlineId().m_u64IdHiPart,  m_HisIdent.getMyOnlineId().m_u64IdLoPart,
					m_LclSessionId.m_u64IdHiPart, m_LclSessionId.m_u64IdLoPart, 
					0 );
    	}
    	
    	// restore microphone to state before entering this activity
      	if( m_MicWasMuted != NativeTxTo.fromGuiIsMicrophoneMuted() )
      	{
      		NativeTxTo.fromGuiMuteMicrophone( m_MicWasMuted );
      	}
    	
    	super.onStop();    	
    }   
    

	//======================================================================== 
	@Override
	public void toGuiContactOffline( VxNetIdent friendIdent ) 
	{
    	if( ( null != m_HisIdent )
    		&& m_HisIdent.getOnlineId().equals( friendIdent.getMyOnlineId() ) )
    	{
			try 
			{
				runOnUiThread(new Runnable() 
				{	
					@Override
					public void run() 
					{	
			    		if( null != m_VidCamView )
			    		{
			    			m_VidCamView.stopVideoFeed();    			
			    		}
			    		
						m_HisIdent.setIsOnline( false );
						setStatusMessage( getString( R.string.contact_offline ) + " " + m_HisIdent.m_as8OnlineName  );
						setCamViewToOfflineImage();
					}
				});
			} 
			catch (Exception e) 
			{
				e.printStackTrace();
			}
		}
   	}
}

