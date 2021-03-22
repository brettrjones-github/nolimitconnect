package com.p2pgotv.gotvstation;

import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;

public class ActivityCamServer extends AppletPeerGuiBase 
{
	// === constants ===//
	public static final String LOG_TAG = "ActivityCamServer:";

	// === vars ===//
	public Button 						m_oRunInBackgroundBut = null;
	public Button 						m_oStopServerBut = null;
	public ImageViewWithSizer 			m_CamView = null;
	
	//======================================================================== 
	@Override
	public void onCreate(Bundle savedInstanceState) 
	{
		Log.d(LOG_TAG, "4onCreate");
		m_ePluginType 		= Constants.ePluginTypeCamServer;
		m_IsServerPlugin 	= true;
		setContentView(R.layout.activity_web_cam_server);
		super.onCreate(savedInstanceState);
		setPageTitle( R.string.activity_cam_server );
		disableScreenTurnOff();
		
        if( null != m_MuteMicrophoneBut )
        {
        	m_MuteMicrophoneBut.setVisibility( View.VISIBLE );  	
        }
	
		m_CamView = (ImageViewWithSizer)this.findViewById( R.id.reuse_cam_view );
		if( null != m_CamView )
		{		
			m_CamView.setVideoFeedId( m_HisIdent.getMyOnlineId() );
			m_CamView.requestVideoFeed();
		}

		Log.i( LOG_TAG, "user account "
				+ m_MyApp.getUserAccount().describeMyId() );
		m_MyApp.getUserAccount().setHasSharedWebCam(true);
		NativeTxTo.fromGuiStartPluginSession( 	m_ePluginType, m_HisIdent.m_u64IdHiPart, m_HisIdent.m_u64IdLoPart, 0, 0, 0 );
		
		this.m_oRunInBackgroundBut = (Button)this.findViewById( R.id.button_run_cam_server_in_background );
	 	this.m_oRunInBackgroundBut.setOnClickListener( new OnClickListener() 
	 	{
	 		public void onClick(View v) 
	 		{
 			    m_MyApp.playButtonClick();
	 			onRunInBackgroundButClick(v);
	 		}
	 	});	

	 	this.m_oStopServerBut = (Button)this.findViewById(R.id.button_stop_cam_server);
	 	this.m_oStopServerBut.setOnClickListener(new OnClickListener() 
	 	{
	 		public void onClick(View v) 
	 		{
 			    m_MyApp.playButtonClick();
	 			onStopServerButClick(v);
	 		}
	 	});	
	 	
        NativeTxTo.fromGuiMuteMicrophone( false );
        // hide the preview
		ImageViewWithSizer camPreview = (ImageViewWithSizer)this.findViewById(R.id.reuse_cam_thumbnail);
		if( null != camPreview )
		{
			camPreview.setVisibility( View.GONE );
		}
	}
	
	//========================================================================      
    private void onRunInBackgroundButClick(View v) 
    {
		m_MyApp.getUserAccount().setHasSharedWebCam(true);
		finish();
	}
    
	//========================================================================      
    private void onStopServerButClick(View v) 
    {
		NativeTxTo.fromGuiStopPluginSession( m_ePluginType, m_HisIdent.m_u64IdHiPart, m_HisIdent.m_u64IdLoPart, 0, 0, 0 );
		m_MyApp.getUserAccount().setHasSharedWebCam(false);
		finish();
   }  
}
