//============================================================================
// Copyright (C) 2009 Brett R. Jones
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

import java.lang.reflect.Method;
import java.util.ArrayList;
import java.util.List;

import android.Manifest;
import android.annotation.SuppressLint;
import android.app.Activity;
import android.app.ActivityManager;
import android.app.AlertDialog;
import android.content.*;
import android.content.pm.PackageManager;
import android.media.AudioManager;
import android.os.Build;
import android.os.Bundle;
import android.os.SystemClock;
import android.util.*;
import android.view.*;
import android.view.View.OnClickListener;
import android.widget.AdapterView;
import android.widget.AdapterView.OnItemSelectedListener;
import android.widget.ArrayAdapter;
import android.widget.ImageButton;
import android.widget.ImageView;
import android.widget.Spinner;
import android.widget.TextView;
import android.widget.Toast;

import com.p2pgotv.gotvstation.NativeTxTo;
import com.p2pgotv.gotvstation.ActivityEditStoryboard;
import com.p2pgotv.gotvstation.ActivityEditProfile;

public class MyP2PWeb extends ActivityFriendListBase implements OnItemSelectedListener, ToGuiNetworkStateInterface, ToGuiAssetMessageInterface
{
	private static final String LOG_TAG = "MyP2PWeb:";
	// === vars ===//
	ImageButton					m_oSearchBut 					= null;
	ImageButton					m_FileMenuBut 					= null;
	ImageButton					m_oServersBut 					= null;
	ImageButton 				m_oOptionsBut 					= null;
	ImageButton 				m_NetworkStateBut 				= null; 
	ImageButton 				m_RefreshContactsBut 			= null; 
	
	Spinner 					m_SelectContactsSpinner 		= null; 

	boolean						m_bGreenOffer 					= false;

	private MyP2PWebState		m_MyP2PWebState					= null; 
	String[] 					m_ContactViewTypeText 			=  {"List Everybody Except Ignored", "List Administrators", "List Friends And Guests", 
																	"List Anonymous Contacts", "List People I Ignore", 
																	"List My Prefered Relays", "List All Possible Relays"};
	
	int[] 						m_ContactViewTypeIcon 			=  {R.drawable.ic_guest, R.drawable.button_administrator_normal, R.drawable.button_people_normal, 
																	R.drawable.button_person_unknown_normal, R.drawable.ic_ignore, 
																	R.drawable.button_proxy_normal, R.drawable.button_proxy_normal };
	
	boolean						m_HaveWriteDiskPermission 		= false;
	boolean						m_HaveReadDiskPermission 		= false;	
	boolean						m_HaveCameraPermission 			= false;
	boolean						m_HaveMicrophonePermission 		= false;
	//boolean						m_HaveSensorPermission 			= false;
	boolean						m_HaveWifiPermission 			= false;
	boolean						m_HaveNetworkPermission 		= false;
	boolean						m_HaveInternetPermission 		= false;
	int							m_SavedEntryVolume 				= 0;
	
	//========================================================================
	@Override
	public void onCreate(Bundle savedInstanceState) 
	{
		m_NeedFinishCreation 			= true;
		Log.d( Constants.LIFETIME, "&&&& MyP2PWeb onCreate" );
        setContentView(R.layout.main);
        m_MyApp 				= (MyApp) this.getApplication();
		m_MyApp.m_oMyP2PWeb 	= this;
		m_IsMainContactList 	= true;
		super.onCreate(savedInstanceState);
		setPageTitle( R.string.activity_main_title );
		if( NativeTxTo.fromGuiGetIsAppCommercial() )
		{
	        if( null != m_TitleText )
	        {
	        	m_TitleText.setText( "Test App Main Menu" );	
	        }
		}
		
	    // Connect Hardware volume buttons to adjust STREAM_MUSIC used for both sound effects and voice over IP
        this.setVolumeControlStream( AudioManager.STREAM_MUSIC );
	    final AudioManager audioManager = (AudioManager)getSystemService( Context.AUDIO_SERVICE );
	    int maxVolume 	= audioManager.getStreamMaxVolume( AudioManager.STREAM_MUSIC );
	    int curVolume	= audioManager.getStreamVolume( AudioManager.STREAM_MUSIC ); 
	    m_SavedEntryVolume = curVolume;
	    if( curVolume < ( maxVolume / 2 ) )
	    {    	
	    	setMediaVolume( (int) ((float)maxVolume * 0.80f) );
	    }    

		m_iWhoToViewInList = m_MyApp.getMySettings().getWhichContactsToView();
		
		this.m_FileMenuBut = (ImageButton) this.findViewById(R.id.file_menu_but);
		this.m_FileMenuBut.setOnClickListener(new OnClickListener() 
		{
			public void onClick(View v) 
			{
  			   	m_MyApp.playButtonClick();
				onFileMenuButClick(v);
			}
		});

		this.m_oSearchBut = (ImageButton) this.findViewById(R.id.but_search);
		this.m_oSearchBut.setOnClickListener(new OnClickListener() 
		{
			public void onClick(View v) 
			{
  			   	m_MyApp.playButtonClick();
				OnSearchButClick(v);
			}
		});
		
		m_oSearchBut.setEnabled( false );

		this.m_oServersBut = (ImageButton) this.findViewById(R.id.but_servers);
		this.m_oServersBut.setOnClickListener(new OnClickListener() 
		{
			public void onClick(View v) 
			{
  			   	m_MyApp.playButtonClick();
				OnServersButClick(v);
			}
		});

		this.m_oOptionsBut = (ImageButton) this.findViewById(R.id.but_setup);
		this.m_oOptionsBut.setOnClickListener(new OnClickListener() 
		{
			public void onClick(View v) 
			{
  			   	m_MyApp.playButtonClick();
				OnOptionsButClick(v);
			}
		});

		this.m_NetworkStateBut = (ImageButton) this.findViewById(R.id.but_network_state);
		this.m_NetworkStateBut.setOnClickListener(new OnClickListener() 
		{
			public void onClick(View v) 
			{
  			   	m_MyApp.playButtonClick();
  			   	onNetworkStateButClick(v);
			}
		});
		
		this.m_RefreshContactsBut = (ImageButton) this.findViewById(R.id.refresh_contacts_but);
		this.m_RefreshContactsBut.setOnClickListener(new OnClickListener() 
		{
			public void onClick(View v) 
			{
  			   	m_MyApp.playButtonClick();
				onRefreshContactsButClick(v);
			}
		});

		m_SelectContactsSpinner  = (Spinner)findViewById(R.id.contact_list_select_spinner);
			
		//Log.i(Constants.LIFETIME, "MainActivity checkExternalMedia");
		// m_oDataHelper.deleteTables(m_oDataHelper.getMyWriteableDb());
		// m_oDataHelper.createTables(m_oDataHelper.getMyWriteableDb());
		
		if( checkStupidVistaStylePermissions() )
		{
			Log.d( Constants.LIFETIME, "&&&& MyP2PWeb onCreate finishCreateMyP2PWeb" );
			//finishCreateMyP2PWeb();
		}
		
     	Log.d( Constants.LIFETIME, "&&&& MyP2PWeb onCreate starting once per second" );
    	startOncePerSecondEvent();
    	Log.d( Constants.LIFETIME, "&&&& MyP2PWeb onCreate done" );
    }
	
	//========================================================================
	// called to finish creation if can write to disk and access camera
	private void finishCreateMyP2PWeb() 
	{
		if( false == m_NeedFinishCreation )
		{
			Log.d( Constants.LIFETIME, "&&&& MyP2PWeb finishCreateMyP2PWeb false == m_NeedFinishCreation" );
			return;
		}
		
		if( null == m_MyApp.m_MyP2PWebState )
		{
			Log.d( Constants.LIFETIME, "&&&& MyP2PWeb finishCreateMyP2PWeb m_MyApp.m_MyP2PWebState init" );
			m_MyApp.myAppInitialize( this, getResources());
	    	if( null == m_MyApp.getCamCaptureMgr() )
	    	{
	 			Toast.makeText( this, R.string.please_wait_before_restarting, Toast.LENGTH_SHORT ).show();
				m_MyApp.setIsAppShuttingDown( true );
				String thisPackageName = "com.p2pgotv.gotvstation";
				ActivityManager am = (ActivityManager) this.getSystemService(Context.ACTIVITY_SERVICE);
				// give few seconds before kill
				SystemClock.sleep(1000);
				finish();
				SystemClock.sleep(1000);
				Log.d(Constants.LIFETIME, "&&&&& MyP2PWeb finishCreateMyP2PWeb killing package");
				
				am.killBackgroundProcesses( thisPackageName );
				System.exit(0);
			
				return;
	    	}

			m_MyApp.m_MyP2PWebState = new MyP2PWebState();
			m_MyP2PWebState = m_MyApp.getMyP2PWebState();
			m_MyP2PWebState.doStartupTasks( m_MyApp, this );
		}
			
		Log.d( Constants.LIFETIME, "&&&& MyP2PWeb finishCreateMyP2PWeb continue create" );
		m_SelectContactsSpinner.setEnabled( false );	
		m_SelectContactsSpinner.setAdapter( new SelectContactsCustomAdapter( this, R.layout.custom_spinner_row, m_ContactViewTypeText ) );
		m_SelectContactsSpinner.setSelection( m_iWhoToViewInList );
		m_SelectContactsSpinner.setOnItemSelectedListener( this );	
		m_SelectContactsSpinner.setEnabled( true );	
		if( m_MyP2PWebState.getIsNativeReadyForCalls() )
		{
			m_SelectContactsSpinner.setEnabled( false );
		}
		
		if( false == m_MyP2PWebState.getIsMainInitialized() )
		{
			Log.i( Constants.LIFETIME, "&&&& MyP2PWeb finishCreateMyP2PWeb applicationMainCreate");
			m_MyApp.applicationMainCreate( getBaseContext() );		
		}

		setCamCaptureActivity( this );	
		setStatusMessage( R.string.network_state_init );
    	NativeRxFrom.wantToGuiNetworkState( this, true );
    	NativeRxFrom.wantToGuiAssetMessages( this, true );
    	doGuiNetworkStateChange( m_MyApp.getNetworkState() );  
 		setupSoundAndOfferBar(); // delayed setup of sound so calls to native delayed until setup
		setTitleBarHelpUrl( "http://www.gotvp2p.net/network_help.html" );
		m_NeedFinishCreation = false;
		Log.d( Constants.LIFETIME, "&&&& MyP2PWeb finishCreateMyP2PWeb done" );
	}
	
	//========================================================================
	protected void setMediaVolume( int newVolume )
	{
		if( newVolume > 0 )
		{
		    final AudioManager audioManager = (AudioManager)getSystemService( Context.AUDIO_SERVICE );   
		    audioManager.setStreamVolume( AudioManager.STREAM_MUSIC, newVolume, 0);
		}
	}

	//========================================================================
	@Override
	protected void onOncePerSecond()
	{  
		if( true == m_NeedFinishCreation )
		{
			if( m_MyApp.m_IsMyAppInitialized  && haveAllNeededAppPermissions() )
			{
				Log.i( Constants.LIFETIME, "&& MyP2PWeb onOncePerSecond calling finishCreateMyP2PWeb" );
				finishCreateMyP2PWeb();
				
				if( null == m_MyApp.getCamCaptureMgr() )
				{
					Log.i( Constants.LIFETIME, "&& MyP2PWeb onOncePerSecond null == m_MyApp.getCamCaptureMgr()" );
					// better to just shutdown than have app crash because not full initialized
					//Toast.makeText( this, R.string.please_wait_before_restarting, Toast.LENGTH_LONG ).show();
					//m_MyApp.setIsAppShuttingDown( true );
					//finish();	
					
				}
				
				return;
			}
			else
			{
				if( false == m_MyApp.m_IsMyAppInitialized )
				{					
					Log.i( Constants.LIFETIME, "&& MyP2PWeb onOncePerSecond cannot finishCreateMyP2PWeb until MyApp initialized" );
				}
				
				if( false == haveAllNeededAppPermissions() )
				{					
					Log.i( Constants.LIFETIME, "&& MyP2PWeb onOncePerSecond cannot finishCreateMyP2PWeb until permissions granted" );
				}
			}
			
			return;
		}

		//Log.d( LOG_TAG,  "MyP2PWeb:onOncePerSecond" );
		if( ( false == m_MyApp.getIsAppShuttingDown() )
			&& ( null != m_MyP2PWebState ) )
		{
			NativeTxTo.fromGuiKickWatchdog();
			m_MyP2PWebState.onOncePerSecond();
		}
	}
	
	//========================================================================
	private void abortCreateMyP2PWebDueToPermissions()
	{
		AlertDialog.Builder dlgAlert = new AlertDialog.Builder(this); 
	    dlgAlert.setTitle("My P2P Web Will Now Exit");  
	    dlgAlert.setMessage( "My P2P Web Cannot Run Correctly Without The Required User Permissions So Will Now Exit" );  
	    dlgAlert.setPositiveButton( "Ok", new DialogInterface.OnClickListener() 
	    { 
	        public void onClick(DialogInterface dialog, int whichButton) 
	        { 
			   	m_MyApp.playButtonClick();
			   	NativeRxFrom.toGuiShutdownHardware();
			   	finish();
	        } 
	    }); 
	    
		dlgAlert.create().show(); 
	}
	
	//========================================================================
	private boolean checkStupidVistaStylePermissions()
	{
		boolean haveWritePermission = checkSystemWritePermission();	
		return haveWritePermission;
	}

	//========================================================================
	@SuppressLint("NewApi") private boolean checkSystemWritePermission() 
	{
    	m_HaveWriteDiskPermission 		= true;
    	m_HaveReadDiskPermission 		= true;
    	m_HaveCameraPermission 			= true;
    	m_HaveMicrophonePermission 		= true;
    	//m_HaveSensorPermission 			= true;	
    	m_HaveWifiPermission 			= true;	
    	m_HaveNetworkPermission			= true;
    	m_HaveInternetPermission 		= true;	

	    if( Build.VERSION.SDK_INT >= Build.VERSION_CODES.M ) 
	    {
	    	int haveWriteDiskPermission 	= checkSelfPermission( Manifest.permission.WRITE_EXTERNAL_STORAGE );
	    	int haveReadDiskPermission 		= checkSelfPermission( Manifest.permission.READ_EXTERNAL_STORAGE );
	    	int haveCameraPermission 		= checkSelfPermission( Manifest.permission.CAMERA );
	    	int haveMicrophonePermission 	= checkSelfPermission( Manifest.permission.RECORD_AUDIO );
	    	//int haveSensorPermission 		= checkSelfPermission( Manifest.permission.SEND_SMS );
	    	int haveWifiPermission 			= checkSelfPermission( Manifest.permission.ACCESS_WIFI_STATE );
	    	int haveNetworkPermission 		= checkSelfPermission( Manifest.permission.ACCESS_NETWORK_STATE );
	    	int haveInternetPermission 		= checkSelfPermission( Manifest.permission.INTERNET );

	    	List<String> permissions = new ArrayList<String>();
	    	if( haveWriteDiskPermission != PackageManager.PERMISSION_GRANTED ) 
	    	{
	    		m_HaveWriteDiskPermission 		= false;
	    	    permissions.add( Manifest.permission.WRITE_EXTERNAL_STORAGE );
	    	}

	    	if( haveReadDiskPermission != PackageManager.PERMISSION_GRANTED ) 
	    	{
	    		m_HaveReadDiskPermission 		= false;
	    	    permissions.add( Manifest.permission.READ_EXTERNAL_STORAGE );
	    	}

	    	if( haveCameraPermission != PackageManager.PERMISSION_GRANTED ) 
	    	{
	    		m_HaveCameraPermission 			= false;
	    	    permissions.add( Manifest.permission.CAMERA );
	    	}
	    	 
	    	if( haveMicrophonePermission != PackageManager.PERMISSION_GRANTED ) 
	    	{
	    		m_HaveMicrophonePermission 		= false;
	    	    permissions.add( Manifest.permission.RECORD_AUDIO );
	    	}
	    	 
	    	//if( haveSensorPermission != PackageManager.PERMISSION_GRANTED ) 
	    	//{
	    	//    m_HaveSensorPermission 			= false;	
	    	//    permissions.add( Manifest.permission.SEND_SMS );
	    	//}
	    	 
	    	if( haveWifiPermission != PackageManager.PERMISSION_GRANTED ) 
	    	{
	    		m_HaveWifiPermission 			= false;	
	    	    permissions.add( Manifest.permission.ACCESS_WIFI_STATE );
	    	}
	    	 
	    	if( haveNetworkPermission != PackageManager.PERMISSION_GRANTED ) 
	    	{
	    		m_HaveNetworkPermission			= false;
	    	    permissions.add( Manifest.permission.ACCESS_NETWORK_STATE );
	    	}
	    	 
	    	if( haveInternetPermission != PackageManager.PERMISSION_GRANTED ) 
	    	{
	    		m_HaveInternetPermission 		= false;	
	    	    permissions.add( Manifest.permission.INTERNET );
	    	}
	    	
	    	if( !permissions.isEmpty() ) 
	    	{
	    	    requestPermissions( permissions.toArray( new String[permissions.size()] ), Constants.ACTIVITY_ANDROID_USER_PERMISSIONS );
	    	}
	    }
	    
	    return haveAllNeededAppPermissions();
	}

	//========================================================================
	private boolean haveAllNeededAppPermissions() 
	{
		return ( m_HaveWriteDiskPermission
				&& m_HaveReadDiskPermission
				&& m_HaveCameraPermission
				&& m_HaveMicrophonePermission
				//&& m_HaveSensorPermission
				&& m_HaveNetworkPermission
				&& m_HaveWifiPermission
				&& m_HaveInternetPermission );
	}
	
	//========================================================================
	@Override
	public void onRequestPermissionsResult(int requestCode, String[] permissions, int[] grantResults) 
	{
	    switch ( requestCode ) 
	    {
	        case Constants.ACTIVITY_ANDROID_USER_PERMISSIONS: 
	        {
	        	boolean somePermissionsDenied = false;
	            for( int i = 0; i < permissions.length; i++ ) 
	            {
	                if( grantResults[i] == PackageManager.PERMISSION_GRANTED ) 
	                {
	                    Log.d( "Permissions", "Permission Granted: " + permissions[i] );
	                } 
	                else if( grantResults[i] == PackageManager.PERMISSION_DENIED ) 
	                {
	                    Log.d( "Permissions", "Permission Denied: " + permissions[i] );
	                    somePermissionsDenied = true;
	                }
	            }
	            
	            if( somePermissionsDenied )
	            {
	            	abortCreateMyP2PWebDueToPermissions();
	            }
	            else
	            {
					m_HaveWriteDiskPermission 		= true;
					m_HaveReadDiskPermission 		= true;
					m_HaveCameraPermission 			= true;
					m_HaveMicrophonePermission 		= true;
					m_HaveNetworkPermission 		= true;
					m_HaveWifiPermission 			= true;
					m_HaveInternetPermission		= true;
					//m_HaveSensorPermission 			= true;	 
					Log.d( Constants.LIFETIME, "onRequestPermissionsResult success once per sec should call finishCreateMyP2PWeb" );
	            }
	        }
	        
	        break;
	        
	        default: 
	        {
	            super.onRequestPermissionsResult(requestCode, permissions, grantResults);
	        }
	    }
	}

	//========================================================================
	@Override
    public void onBackButtonPressed() // back button on title bar
    {
		doConfirmExitPrompt();
    }
	  
	//========================================================================
	@Override
	public void onBackPressed() // back button on android phone
	{
		doConfirmExitPrompt();
	}
	
	//========================================================================	
	private void doConfirmExitPrompt()
	{
		AlertDialog.Builder dlgAlert = new AlertDialog.Builder(this); 
	    dlgAlert.setTitle("Exit My P2P Web App ?");  
	    dlgAlert.setMessage( "Are You Sure You Want To Quit?" ); 
 
	    dlgAlert.setPositiveButton( "Yes", new DialogInterface.OnClickListener() 
	    { 
	        public void onClick(DialogInterface dialog, int whichButton) 
	        { 
	        	onExitConfirmed();
	        } 
	    }); 
	    
	    dlgAlert.setNegativeButton("No",new DialogInterface.OnClickListener() 
	    { 
	        public void onClick(DialogInterface dialog, int whichButton) 
	        { 
			   	m_MyApp.playButtonClick();
	        } 
	    }); 
	    
	    dlgAlert.setCancelable(true); 
	    dlgAlert.create().show(); 
	}	
	
	//========================================================================	
	public void onExitConfirmed()
	{
	   	m_MyApp.playButtonClick();
      	setToGuiMessageReadyState( null );  
		finish();		
	}
	
	//========================================================================	
	@Override
	public void onItemSelected(AdapterView<?> parent, View view, int position, long id)
	{
	    // On selecting a spinner item
	    String item = parent.getItemAtPosition(position).toString();
	    if( m_SelectContactsSpinner.isEnabled() && ( 0 <= position  ) )
	    {	      
	    	if( position != m_iWhoToViewInList )
	    	{
	    		// Showing selected spinner item
	    		Toast.makeText(parent.getContext(), "Selected: " + item, Toast.LENGTH_LONG).show();
	    		m_iWhoToViewInList = position;
	    		m_MyApp.getMySettings().setWhichContactsToView( m_iWhoToViewInList );
	    		RefreshContacts();	    		   		  
	    	}
	    }
	}
	
	//========================================================================	
	public void onNothingSelected(AdapterView<?> arg0) 
	{
	}

	//========================================================================	
    public class SelectContactsCustomAdapter extends ArrayAdapter<String>
    {
    	//========================================================================
		public SelectContactsCustomAdapter(Context context, int textViewResourceId, String[] objects) 
		{
			super(context, textViewResourceId, objects);
		}

		//========================================================================
		@Override
		public View getDropDownView(int position, View convertView, ViewGroup parent) 
		{
			return getCustomView(position, convertView, parent);
		}

		//========================================================================
		@Override
		public View getView(int position, View convertView, ViewGroup parent) 
		{
			return getCustomView(position, convertView, parent);
		}

		//========================================================================
		public View getCustomView(int position, View convertView, ViewGroup parent) 
		{
			//return super.getView(position, convertView, parent);

			LayoutInflater inflater=getLayoutInflater();
			View row=inflater.inflate(R.layout.custom_spinner_row, parent, false);
			TextView label=(TextView)row.findViewById( R.id.custom_spinner_text_view );
			label.setText(m_ContactViewTypeText[position]);
			
			ImageView icon = (ImageView)row.findViewById(R.id.custom_spinner_icon);
			icon.setImageResource( m_ContactViewTypeIcon[position] );
			
			return row;
		}	
    }
	
	//========================================================================
    @Override
	public void onDestroy() 
    {
		Log.d(Constants.LIFETIME, "&&&&& MyP2PWeb onDestroy");
		m_MyApp.setIsAppShuttingDown( true );
    	if( null != m_MyApp.getCamCaptureMgr() )
    	{
			setMediaVolume( m_SavedEntryVolume );
			setCamCaptureActivity( null );
		   	NativeRxFrom.wantToGuiNetworkState( this, false );
			NativeRxFrom.wantToGuiAssetMessages( this, false );
		   	//NativeRxFrom.toGuiShutdownHardware(); // causes crash
			m_MyApp.applicationMainDestroy();
			if( null != m_MyApp.m_MyP2PWebState )
			{
				m_MyApp.m_MyP2PWebState.doMainStateShutdown();
				m_MyApp.m_MyP2PWebState = null;
			}
    	}
		
		String thisPackageName = "com.p2pgotv.gotvstation";
		ActivityManager am = (ActivityManager) this.getSystemService(Context.ACTIVITY_SERVICE);
		super.onDestroy();
		// give few seconds before kill
		SystemClock.sleep(1000);
		Log.d(Constants.LIFETIME, "&&&&& MyP2PWeb onDestroyDone");
		am.killBackgroundProcesses( thisPackageName );
    }
	
	//========================================================================
    @Override
	public void onPause() 
    {
		//Log.d(Constants.LIFETIME, "&&&& MyP2PWeb onPause");
    	if( null != m_MyApp.getCamCaptureMgr() )
    	{
    		NativeRxFrom.wantToGuiNetworkState( this, false );
    		NativeRxFrom.wantToGuiAssetMessages( this, false );
    	}
     	
 		super.onPause();
    }
	
	//========================================================================
    @Override
	public void onResume() 
    {
		super.onResume();
    	if( null != m_MyApp.getCamCaptureMgr() )
    	{
    		//Log.d(Constants.LIFETIME, "&&&& MyP2PWeb onResume");
    		NativeRxFrom.wantToGuiNetworkState( this, true );
    		NativeRxFrom.wantToGuiAssetMessages( this, true );
    		doGuiNetworkStateChange( m_MyApp.getNetworkState() );
    	}
    }
	
	//========================================================================
    @Override
    protected void onStop() 
    {
 		super.onStop();
		Log.d(Constants.LIFETIME, "&&&&&&&&& MyP2PWeb onStop");
    }

	//========================================================================
	@Override
	public void onActivityResult(int requestCode, int resultCode, Intent data) 
	{
		super.onActivityResult(requestCode, resultCode, data);
		Log.d(LOG_TAG, "onActivityResult result code " + resultCode);
			
		switch (requestCode) 
		{
		case Constants.ACTIVITY_TO_FRIEND_CHANGE_FRIENDSHIP:
			Log.d( LOG_TAG, "onActivityResult ACTIVITY_TO_FRIEND_CHANGE_FRIENDSHIP" );
			if( resultCode == Activity.RESULT_OK ) 
			{
				RefreshContacts();
			}
			break;

		case Constants.ACTIVITY_EDIT_PROFILE:
			Log.d(LOG_TAG, "onActivityResult ACTIVITY_EDIT_PROFILE");
			break;
			
		case Constants.ACTIVITY_CREATE_PROFILE:
			Log.d(LOG_TAG, "ACTIVITY_CREATE_PROFILE result" + resultCode);
			if(RESULT_CANCELED == resultCode)
			{
				finish();
			}
			else if( m_MyP2PWebState.LoginOrCreateProfile() ) 
			{

			}
			else
			{
				finish();
			}
			break;
		}
		
		setCamCaptureActivity( this );
	}
	
	//========================================================================
	public void ShowRelayStatus(UserAccount oAccount) 
	{
		if (false == oAccount.requiresRelay()) 
		{
			setStatusMessage("Can Direct Connect (No Relay Required)");
		} 
		else if (false == oAccount.isRelayValid()) 
		{
			setStatusMessage("User must select a Relay");
			m_iWhoToViewInList = Constants.eFriendViewAllProxies;
		} 
		else 
		{
			setStatusMessage("Searching for a Relay");
		}
	}

	//========================================================================
	public void onRefreshContactsButClick(View v) 
	{
		m_MyApp.playButtonClick() ;
		if( false == m_MyP2PWebState.getIsNativeReadyForCalls() )
		{
			Log.e( LOG_TAG, "Refresh Button clicked before native is ready" );
			return;
		}
			
		NativeTxTo.fromGuiSendContactList( m_iWhoToViewInList );
		switch( m_iWhoToViewInList )
		{
		case Constants.eFriendViewEverybody:
			setStatusMessage( "Refresh List Everybody Except Ignored" );
			break;
			
		case Constants.eFriendViewFriendsAndGuests:
			setStatusMessage( "Refresh List Friends And Guests" );
			break;
			
		case Constants.eFriendViewAnonymous:
			setStatusMessage( "Refresh List Anonymous Contacts" );
			break;
			
		case Constants.eFriendViewAdministrators:
			setStatusMessage( "Refresh List Administrators" );
			break;			
			
		case Constants.eFriendViewIgnored:
			setStatusMessage( "Refresh List People I Ignore" );
			break;			
			
		case Constants.eFriendViewMyProxies:
			setStatusMessage( "Refresh List My Prefered Relays" );
			break;			
			
		case Constants.eFriendViewAllProxies:
			setStatusMessage(  "Refresh List All Possible Relays" );
			break;	
			
		default:
			setStatusMessage(  "Unknown Refresh List View" );
			break;	
		}
	}
	
	//========================================================================
	public void onFileMenuButClick(View v) 
	{
		m_MyApp.playButtonClick() ;
		m_MyApp.ShowPopupMenu(Constants.MENU_FILES);
	}	

	//========================================================================
	public void OnSearchButClick(View v) 
	{
		m_MyApp.playButtonClick() ;
		m_MyApp.ShowPopupMenu(Constants.MENU_SEARCH);
	}

	//========================================================================
	public void OnServersButClick(View v) 
	{
		m_MyApp.playButtonClick() ;
		m_MyApp.ShowPopupMenu(Constants.MENU_SERVERS);
	}

	//========================================================================
	public void OnOptionsButClick(View v) 
	{
		m_MyApp.playButtonClick() ;
		m_MyApp.ShowPopupMenu(Constants.MENU_OPTIONS);
	}

	//========================================================================
	public void onNetworkStateButClick(View v) 
	{
		m_MyApp.playButtonClick() ;
		Intent myIntent = new Intent(MyP2PWeb.this, ActivityNetworkStatus.class);
		MyP2PWeb.this.startActivityForResult( myIntent, Constants.ACTIVITY_OFFERS );
	}

	//========================================================================
	public void onMenuSelected( PopupMenuItem oMenuItem ) 
	{
		if( Constants.MENU_PEOPLE == oMenuItem.m_iMenuId) 
		{
			int iWhoToView = Constants.eFriendViewEverybody;
			if( Constants.MENU_PEOPLE_VIEW_EVERYBODY == oMenuItem.m_iSubMenuId) 
			{
				iWhoToView = Constants.eFriendViewEverybody;
				m_MyApp.getMySettings().setWhichContactsToView(iWhoToView);
			}
			else if( Constants.MENU_PEOPLE_VIEW_GUESTS_AND_FRIENDS == oMenuItem.m_iSubMenuId) 
			{
				iWhoToView = Constants.eFriendViewFriendsAndGuests;
				m_MyApp.getMySettings().setWhichContactsToView(iWhoToView);
			} 
			else if (Constants.MENU_PEOPLE_VIEW_ANONYMOUS == oMenuItem.m_iSubMenuId) 
			{
				iWhoToView = Constants.eFriendViewAnonymous;
				m_MyApp.getMySettings().setWhichContactsToView(iWhoToView);
			} 
			else if (Constants.MENU_PEOPLE_VIEW_ADMINISTRATORS == oMenuItem.m_iSubMenuId) 
			{
					iWhoToView = Constants.eFriendViewAdministrators;
				m_MyApp.getMySettings().setWhichContactsToView(iWhoToView);
			} 
			else if (Constants.MENU_PEOPLE_VIEW_IGNORED == oMenuItem.m_iSubMenuId) 
			{
				iWhoToView = Constants.eFriendViewIgnored;
				m_MyApp.getMySettings().setWhichContactsToView(iWhoToView);
			} 
			else if (Constants.MENU_PEOPLE_VIEW_MY_PROXIES == oMenuItem.m_iSubMenuId) 
			{
				iWhoToView = Constants.eFriendViewMyProxies;
				m_MyApp.getMySettings().setWhichContactsToView(iWhoToView);
			} 
			else if (Constants.MENU_PEOPLE_VIEW_ALL_PROXIES == oMenuItem.m_iSubMenuId) 
			{
				iWhoToView = Constants.eFriendViewAllProxies;
				m_MyApp.getMySettings().setWhichContactsToView(iWhoToView);
			} 
			else if (Constants.MENU_PEOPLE_VIEW_REFRESH == oMenuItem.m_iSubMenuId) 
			{
				iWhoToView = m_MyApp.getMySettings().getWhichContactsToView();
			}
			
			m_iWhoToViewInList = iWhoToView;
			// save setting for every view type except refresh
			m_MyApp.getMySettings().setWhichContactsToView( m_iWhoToViewInList );
			RefreshContacts();
		} 
		else if( Constants.MENU_SEARCH == oMenuItem.m_iMenuId ) 
		{
			if( Constants.MENU_SEARCH_BY_PHONE_SHAKE == oMenuItem.m_iSubMenuId ) 
			{
				Intent myIntent = new Intent( MyP2PWeb.this, ActivityPhoneShake.class );
				myIntent.putExtra("PeopleSearchType", Constants.eScanTypePhoneShake );
				MyP2PWeb.this.startActivityForResult( myIntent, Constants.ACTIVITY_PHONE_SHAKE );
			}
			else if( Constants.MENU_SEARCH_BY_NAME == oMenuItem.m_iSubMenuId ) 
			{
				Intent myIntent = new Intent( MyP2PWeb.this, ActivityScanPeopleSearch.class );
				myIntent.putExtra("PeopleSearchType", Constants.eScanTypePeopleSearch );
				MyP2PWeb.this.startActivityForResult( myIntent, Constants.ACTIVITY_SCAN_PEOPLE_BY_NAME );
			}
			else if( Constants.MENU_SEARCH_BY_MOOD_MSG == oMenuItem.m_iSubMenuId ) 
			{
				Intent myIntent = new Intent( MyP2PWeb.this, ActivityScanPeopleSearch.class );
				myIntent.putExtra("PeopleSearchType", Constants.eScanTypeMoodMsgSearch );
				MyP2PWeb.this.startActivityForResult( myIntent, Constants.ACTIVITY_SCAN_PEOPLE_BY_MOOD_MSG );
			}
			else if( Constants.MENU_SEARCH_SCAN_PROFILES == oMenuItem.m_iSubMenuId ) 
			{
				Intent myIntent = new Intent( MyP2PWeb.this, ActivityScanProfiles.class );
				MyP2PWeb.this.startActivityForResult( myIntent, Constants.ACTIVITY_SCAN_PROFILES );
			}
			else if( Constants.MENU_SEARCH_SCAN_WEBCAMS == oMenuItem.m_iSubMenuId ) 
			{
				Intent myIntent = new Intent( MyP2PWeb.this, ActivityScanWebCams.class );
				MyP2PWeb.this.startActivityForResult( myIntent, Constants.ACTIVITY_SCAN_WEB_CAMS );
			}
			else if( Constants.MENU_SEARCH_SHARED_FILES == oMenuItem.m_iSubMenuId ) 
			{
			}
		} 
		else if( Constants.MENU_SERVERS == oMenuItem.m_iMenuId ) 
		{
			if( Constants.MENU_SERVERS_STARTSTOP_WEBCAM == oMenuItem.m_iSubMenuId ) 
			{
				m_MyApp.startActivity( Constants.ePluginTypeCamServer, m_MyApp.getUserAccount(), false );
			}
			else if (Constants.MENU_SERVERS_EDIT_MY_PROFILE == oMenuItem.m_iSubMenuId) 
			{
				Intent myIntent = new Intent( MyP2PWeb.this, ActivityEditProfile.class );
				MyP2PWeb.this.startActivityForResult( myIntent, Constants.ACTIVITY_EDIT_PROFILE );
			} 
			else if (Constants.MENU_SERVERS_VIEW_MY_PROFILE == oMenuItem.m_iSubMenuId) 
			{
				m_MyApp.updateUserAccountFromEngine();
				Intent launchBrowser = new Intent(Intent.ACTION_VIEW, m_MyApp.getUserAccount().getProfileUri( m_MyApp.getUserAccount().getMyExternalIp() ));
				startActivity( launchBrowser );
			} 
			else if (Constants.MENU_SERVERS_EDIT_MY_STORYBOARD == oMenuItem.m_iSubMenuId) 
			{
				Intent myIntent = new Intent( MyP2PWeb.this, ActivityEditStoryboard.class );
				MyP2PWeb.this.startActivityForResult( myIntent, Constants.ACTIVITY_EDIT_STORYBOARD );
			} 
			else if (Constants.MENU_SERVERS_VIEW_MY_STORYBOARD == oMenuItem.m_iSubMenuId) 
			{
				m_MyApp.updateUserAccountFromEngine();
				Intent launchBrowser = new Intent(Intent.ACTION_VIEW, m_MyApp.getUserAccount().getStoryboardUri( m_MyApp.getUserAccount().getMyExternalIp() ) );
				startActivity( launchBrowser );
			} 
			
			//else if( Constants.MENU_SERVERS_SETUP_FILE_SHARE == oMenuItem.m_iSubMenuId ) 
			//{
				//m_MyApp.startActivity( Constants.ePluginTypeCamServer, m_MyApp.getUserAccount(), false );
			//}
			/*
			else if( Constants.MENU_SERVERS_SETUP_FILE_SHARE == oMenuItem.m_iSubMenuId ) 
			{
				//m_MyApp.startActivity( Constants.ePluginTypeCamServer, m_MyApp.getUserAccount(), false );
			}*/		
		} 
		else if( Constants.MENU_FILES == oMenuItem.m_iMenuId ) 
		{
			if( Constants.MENU_FILES_PERSONAL_RECORDER == oMenuItem.m_iSubMenuId ) 
			{
				Intent myIntent = new Intent( MyP2PWeb.this, ActivityPersonalRecorder.class );
				MyP2PWeb.this.startActivityForResult( myIntent, Constants.ACTIVITY_PERSONAL_RECORDER );
			}
			else if( Constants.MENU_FILES_BROWSE_FILES == oMenuItem.m_iSubMenuId ) 
			{
				Intent myIntent = new Intent( MyP2PWeb.this, ActivityBrowseFiles.class );
				MyP2PWeb.this.startActivityForResult( myIntent, Constants.ACTIVITY_BROWSE_DEVICE_FILES );
			}
			else if( Constants.MENU_FILES_VIEW_MY_LIBRARY_FILES == oMenuItem.m_iSubMenuId ) 
			{
				Intent myIntent = new Intent( MyP2PWeb.this, ActivityViewLibraryFiles.class );
				MyP2PWeb.this.startActivityForResult( myIntent, Constants.ACTIVITY_VIEW_LIBRARY_FILES );
			}
			else if( Constants.MENU_FILES_VIEW_SHARED_FILES == oMenuItem.m_iSubMenuId ) 
			{
				Intent myIntent = new Intent( MyP2PWeb.this, ActivityViewSharedFiles.class );
				MyP2PWeb.this.startActivityForResult( myIntent, Constants.ACTIVITY_VIEW_SHARED_FILES );
			}
			else if( Constants.MENU_FILES_VIEW_DOWNLOADS == oMenuItem.m_iSubMenuId ) 
			{
				Intent myIntent = new Intent( MyP2PWeb.this, ActivityDownloads.class );
				MyP2PWeb.this.startActivityForResult( myIntent, Constants.ACTIVITY_DOWNLOADS );
			}
			else if( Constants.MENU_FILES_VIEW_UPLOADS == oMenuItem.m_iSubMenuId ) 
			{
				Intent myIntent = new Intent( MyP2PWeb.this, ActivityUploads.class );
				MyP2PWeb.this.startActivityForResult( myIntent, Constants.ACTIVITY_UPLOADS );
			}
		} 
		else if (Constants.MENU_OPTIONS == oMenuItem.m_iMenuId) 
		{
			if( Constants.MENU_OPTIONS_ABOUT == oMenuItem.m_iSubMenuId ) 
			{
				Intent myIntent = new Intent( MyP2PWeb.this, ActivityAbout.class );
				MyP2PWeb.this.startActivityForResult( myIntent, Constants.ACTIVITY_ABOUT );
			} 
			else if( Constants.MENU_OPTIONS_EDIT_ACCOUNT == oMenuItem.m_iSubMenuId ) 
			{
				Intent myIntent = new Intent( MyP2PWeb.this, ActivityEditAccount.class );
				MyP2PWeb.this.startActivityForResult( myIntent, Constants.ACTIVITY_EDIT_ACCOUNT );
			} 
			else if (Constants.MENU_OPTIONS_EDIT_PERMISSIONS == oMenuItem.m_iSubMenuId) 
			{
				m_MyApp.ShowPopupMenu(Constants.MENU_PLUGIN_PERMISSIONS);
			} 
			else if (Constants.MENU_OPTIONS_NETWORK_SETTINGS == oMenuItem.m_iSubMenuId) 
			{
				Intent myIntent = new Intent( MyP2PWeb.this, ActivityNetworkSettings.class );
				MyP2PWeb.this.startActivityForResult( myIntent, Constants.ACTIVITY_NETWORK_SETTINGS );
			}
			else if (Constants.MENU_OPTIONS_CAMERA_TEST == oMenuItem.m_iSubMenuId) 
			{
				Intent myIntent = new Intent( MyP2PWeb.this, ActivityCamTest.class );
				MyP2PWeb.this.startActivityForResult( myIntent, Constants.ACTIVITY_CAM_TEST );
			}
			else if (Constants.MENU_OPTIONS_DEBUG_SETTINGS == oMenuItem.m_iSubMenuId) 
			{
				//Intent myIntent = new Intent( MyP2PWeb.this, ActivityPickImage.class );
				//MyP2PWeb.this.startActivityForResult( myIntent, Constants.ACTIVITY_PICK_IMAGE );
				NativeTxTo.fromGuiTestCmd( 6, 0, "" );
			}
		} 
		else if (Constants.MENU_PLUGIN_PERMISSIONS == oMenuItem.m_iMenuId) 
		{
			if (Constants.MENU_PLUGIN_PROFILE_PERMISSION == oMenuItem.m_iSubMenuId) 
			{
				Intent myIntent = new Intent(MyP2PWeb.this, ActivityEditPermission.class);
				myIntent.putExtra("PluginType", Constants.ePluginTypeWebServer);
				MyP2PWeb.this.startActivityForResult(myIntent, Constants.ACTIVITY_EDIT_PERMISSION);
			} 
			else if (Constants.MENU_PLUGIN_STORYBOARD_PERMISSION == oMenuItem.m_iSubMenuId) 
			{
				Intent myIntent = new Intent(MyP2PWeb.this, ActivityEditPermission.class);
				myIntent.putExtra("PluginType", Constants.ePluginTypeStoryBoard);
				MyP2PWeb.this.startActivityForResult(myIntent, Constants.ACTIVITY_EDIT_PERMISSION);
			} 
			else if (Constants.MENU_PLUGIN_CHAT_PERMISSION == oMenuItem.m_iSubMenuId) 
			{
				Intent myIntent = new Intent(MyP2PWeb.this, ActivityEditPermission.class);
				myIntent.putExtra("PluginType", Constants.ePluginTypeMultiSession);
				MyP2PWeb.this.startActivityForResult(myIntent, Constants.ACTIVITY_EDIT_PERMISSION);
			} 
			else if (Constants.MENU_PLUGIN_VOICE_PERMISSION == oMenuItem.m_iSubMenuId) 
			{
				Intent myIntent = new Intent(MyP2PWeb.this, ActivityEditPermission.class);
				myIntent.putExtra("PluginType", Constants.ePluginTypeVoicePhone);
				MyP2PWeb.this.startActivityForResult(myIntent, Constants.ACTIVITY_EDIT_PERMISSION);
			} 
			else if (Constants.MENU_PLUGIN_VIDPHONE_PERMISSION == oMenuItem.m_iSubMenuId) 
			{
				Intent myIntent = new Intent(MyP2PWeb.this, ActivityEditPermission.class);
				myIntent.putExtra("PluginType", Constants.ePluginTypeVideoPhone);
				MyP2PWeb.this.startActivityForResult(myIntent, Constants.ACTIVITY_EDIT_PERMISSION);
			} 
			else if (Constants.MENU_PLUGIN_TRUTH_OR_DARE_PERMISSION == oMenuItem.m_iSubMenuId) 
			{
				Intent myIntent = new Intent(MyP2PWeb.this, ActivityEditPermission.class);
				myIntent.putExtra("PluginType", Constants.ePluginTypeTruthOrDare);
				MyP2PWeb.this.startActivityForResult(myIntent, Constants.ACTIVITY_EDIT_PERMISSION);
			} 
			else if (Constants.MENU_PLUGIN_WEBCAM_PERMISSION == oMenuItem.m_iSubMenuId) 
			{
				Intent myIntent = new Intent(MyP2PWeb.this, ActivityEditPermission.class);
				myIntent.putExtra("PluginType", Constants.ePluginTypeCamServer);
				MyP2PWeb.this.startActivityForResult(myIntent, Constants.ACTIVITY_EDIT_PERMISSION);
			} 
			else if (Constants.MENU_PLUGIN_FILE_SHARE_PERMISSION == oMenuItem.m_iSubMenuId) 
			{
				Intent myIntent = new Intent(MyP2PWeb.this, ActivityEditPermission.class);
				myIntent.putExtra("PluginType", Constants.ePluginTypeFileServer);
				MyP2PWeb.this.startActivityForResult(myIntent, Constants.ACTIVITY_EDIT_PERMISSION);
			} 
			else if (Constants.MENU_PLUGIN_FILE_XFER_PERMISSION == oMenuItem.m_iSubMenuId) 
			{
				Intent myIntent = new Intent(MyP2PWeb.this, ActivityEditPermission.class);
				myIntent.putExtra("PluginType", Constants.ePluginTypeFileOffer);
				MyP2PWeb.this.startActivityForResult(myIntent, Constants.ACTIVITY_EDIT_PERMISSION);
			} 
			else if (Constants.MENU_PLUGIN_PROXY_PERMISSION == oMenuItem.m_iSubMenuId) 
			{
				Intent myIntent = new Intent(MyP2PWeb.this, ActivityEditPermissionRelay.class);
				myIntent.putExtra("PluginType", Constants.ePluginTypeRelay);
				MyP2PWeb.this.startActivityForResult(myIntent, Constants.ACTIVITY_EDIT_PERMISSION);
			}
		} 
		else if (Constants.MENU_FRIEND == oMenuItem.m_iMenuId) 
		{
			VxNetIdent oSelectedFriend = m_MyApp.getCurrentFriend();
			if( null != oSelectedFriend ) 
			{
				if (Constants.MENU_FRIEND_VIEW_PROFILE == oMenuItem.m_iSubMenuId)
				{
					m_MyApp.updateUserAccountFromEngine();
					Intent launchBrowser = new Intent(Intent.ACTION_VIEW, oSelectedFriend.getProfileUri( m_MyApp.getUserAccount().getMyExternalIp() ) );
					startActivity(launchBrowser);				
				} 
				else if (Constants.MENU_FRIEND_VIEW_STORYBOARD == oMenuItem.m_iSubMenuId)
				{
					m_MyApp.updateUserAccountFromEngine();
					Intent launchBrowser = new Intent(Intent.ACTION_VIEW, oSelectedFriend.getStoryboardUri( m_MyApp.getUserAccount().getMyExternalIp() ) );
					startActivity(launchBrowser);				
				} 	
				else if (Constants.MENU_FRIEND_OFFER_MULTISESSION == oMenuItem.m_iSubMenuId) 
				{
					if (oSelectedFriend.isAccessAllowed(Constants.ePluginTypeMultiSession)) 
					{
						Intent myIntent = new Intent(MyP2PWeb.this, AppletPeerMultiSession.class);
						m_MyApp.getOffersMgr().lockOffersMgr();
						GuiOfferSession oSession = m_MyApp.getOffersMgr().findAvailableAndActiveOffer( Constants.ePluginTypeMultiSession, oSelectedFriend );
						m_MyApp.getOffersMgr().unlockOffersMgr();
						
						if( null != oSession )
						{
							m_MyApp.setCurrentGuiOfferSession( oSession );
							myIntent.putExtra("IS_OFFER", true);	
						}
						
						myIntent.putExtra("PluginType",Constants.ePluginTypeMultiSession);
						MyP2PWeb.this.startActivityForResult(myIntent, Constants.ACTIVITY_TO_FRIEND_MULTISESSION);
						if( null != oSession )
						{
							m_MyApp.getOffersMgr().offerAcknowlegedByUser( oSession );
						}
					} 
					else 
					{
						showReasonAccessNotAllowed(oSelectedFriend, Constants.ePluginTypeMultiSession);
					}
				} 
				else if (Constants.MENU_FRIEND_OFFER_VOIP == oMenuItem.m_iSubMenuId) 
				{
					if( oSelectedFriend.isAccessAllowed( Constants.ePluginTypeVoicePhone ) ) 
					{
						Intent myIntent = new Intent(MyP2PWeb.this, AppletPeerVoicePhone.class);
						myIntent.putExtra("PluginType", Constants.ePluginTypeVoicePhone);
						m_MyApp.getOffersMgr().lockOffersMgr();
						GuiOfferSession oSession = m_MyApp.getOffersMgr().findAvailableAndActiveOffer( Constants.ePluginTypeVoicePhone, oSelectedFriend );
						m_MyApp.getOffersMgr().unlockOffersMgr();
						if( null != oSession )
						{
							m_MyApp.setCurrentGuiOfferSession( oSession );
							myIntent.putExtra("IS_OFFER", true);	
						}

						MyP2PWeb.this.startActivityForResult(myIntent, Constants.ACTIVITY_TO_FRIEND_VOICE_PHONE);
						if( null != oSession )
						{
							m_MyApp.getOffersMgr().offerAcknowlegedByUser( oSession );
						}
					} 
					else 
					{
						showReasonAccessNotAllowed(oSelectedFriend, Constants.ePluginTypeVoicePhone);
					}
				} 
				else if (Constants.MENU_FRIEND_OFFER_VIDPHONE == oMenuItem.m_iSubMenuId) 
				{
					if (oSelectedFriend
							.isAccessAllowed(Constants.ePluginTypeVideoPhone)) 
					{
						Intent myIntent = new Intent(MyP2PWeb.this, AppletPeerVideoPhone.class);
						myIntent.putExtra("PluginType",Constants.ePluginTypeVideoPhone);
						m_MyApp.getOffersMgr().lockOffersMgr();
						GuiOfferSession oSession = m_MyApp.getOffersMgr().findAvailableAndActiveOffer( Constants.ePluginTypeVideoPhone, oSelectedFriend );
						m_MyApp.getOffersMgr().unlockOffersMgr();
						if( null != oSession )
						{
							m_MyApp.setCurrentGuiOfferSession( oSession );
							myIntent.putExtra("IS_OFFER", true);	
						}

						MyP2PWeb.this.startActivityForResult(myIntent, Constants.ACTIVITY_TO_FRIEND_VIDEO_PHONE);
						if( null != oSession )
						{
							m_MyApp.getOffersMgr().offerAcknowlegedByUser( oSession );
						}
					} 
					else 
					{
						showReasonAccessNotAllowed(oSelectedFriend, Constants.ePluginTypeVideoPhone);
					}
				} 
				else if (Constants.MENU_FRIEND_OFFER_TRUTHORDARE == oMenuItem.m_iSubMenuId) 
				{
					if (oSelectedFriend.isAccessAllowed(Constants.ePluginTypeTruthOrDare)) 
					{
						Intent myIntent = new Intent(MyP2PWeb.this,AppletPeerTodGame.class);
						myIntent.putExtra("PluginType", Constants.ePluginTypeTruthOrDare);
						m_MyApp.getOffersMgr().lockOffersMgr();
						GuiOfferSession oSession = m_MyApp.getOffersMgr().findAvailableAndActiveOffer( Constants.ePluginTypeTruthOrDare, oSelectedFriend );
						m_MyApp.getOffersMgr().unlockOffersMgr();
						if( null != oSession )
						{
							m_MyApp.setCurrentGuiOfferSession( oSession );
							myIntent.putExtra("IS_OFFER", true);	
						}

						MyP2PWeb.this.startActivityForResult(myIntent, Constants.ACTIVITY_TO_FRIEND_TOD_GAME);
						if( null != oSession )
						{
							m_MyApp.getOffersMgr().offerAcknowlegedByUser( oSession );
						}
					} 
					else 
					{
						showReasonAccessNotAllowed(oSelectedFriend, Constants.ePluginTypeTruthOrDare);
					}
				} 
				else if (Constants.MENU_FRIEND_VIEW_SHARED_WEBCAM == oMenuItem.m_iSubMenuId) 
				{
					if( oSelectedFriend.isAccessAllowed( Constants.ePluginTypeCamServer ) ) 
					{
						Intent myIntent = new Intent( MyP2PWeb.this, ActivityCamClient.class );
						MyP2PWeb.this.startActivityForResult( myIntent, Constants.ACTIVITY_CAM_SERVER );
					}
					else 
					{
						showReasonAccessNotAllowed( oSelectedFriend, Constants.ePluginTypeCamServer );
					}
				} 
				else if (Constants.MENU_FRIEND_VIEW_SHARED_FILES == oMenuItem.m_iSubMenuId) 
				{
					if( oSelectedFriend.isAccessAllowed( Constants.ePluginTypeFileServer ) ) 
					{
						Intent myIntent = new Intent( MyP2PWeb.this, AppletPeerViewSharedFiles.class );
						MyP2PWeb.this.startActivityForResult( myIntent, Constants.ACTIVITY_VIEW_SHARED_FILES );
					}
					else 
					{
						showReasonAccessNotAllowed( oSelectedFriend, Constants.ePluginTypeFileServer );
					}
				} 
				else if (Constants.MENU_FRIEND_SEND_FILE == oMenuItem.m_iSubMenuId) 
				{
					if( oSelectedFriend.isAccessAllowed( Constants.ePluginTypeFileOffer ) ) 
					{
						Intent myIntent = new Intent( MyP2PWeb.this, ActivitySelectFileToSend.class );
						MyP2PWeb.this.startActivityForResult( myIntent, Constants.ACTIVITY_SELECT_FILE_TO_SEND );
					}
					else 
					{
						showReasonAccessNotAllowed( oSelectedFriend, Constants.ePluginTypeFileOffer );
					}
				} 
				else if (Constants.MENU_FRIEND_CHANGE_FRIENDSHIP == oMenuItem.m_iSubMenuId) 
				{
					Intent myIntent = new Intent( MyP2PWeb.this, AppletPeerChangeFriendship.class );
					MyP2PWeb.this.startActivityForResult( myIntent, Constants.ACTIVITY_TO_FRIEND_CHANGE_FRIENDSHIP );
				} 
				else if (Constants.MENU_FRIEND_PROXY == oMenuItem.m_iSubMenuId) 
				{
					Intent myIntent = new Intent( MyP2PWeb.this, AppletPeerRequestRelay.class );
					MyP2PWeb.this.startActivityForResult( myIntent, Constants.ACTIVITY_TO_FRIEND_REQUEST_PROXY );
				} 
				/*
				else if (Constants.MENU_FRIEND_EDIT_CONTACT_INFO == oMenuItem.m_iSubMenuId) 
				{
					Intent myIntent = new Intent( MyP2PWeb.this, ActivityEditContactInfo.class );
					MyP2PWeb.this.startActivityForResult( myIntent, Constants.ACTIVITY_EDIT_CONTACT_INFO );
				} 
				else if (Constants.MENU_FRIEND_CALL_USING_PHONE_NUM == oMenuItem.m_iSubMenuId) 
				{
					Intent myIntent = new Intent( MyP2PWeb.this, AppletPeerCallPhoneNumber.class );
					MyP2PWeb.this.startActivityForResult( myIntent, Constants.ACTIVITY_CALL_PHONE_NUM );
				} 
				else if (Constants.MENU_FRIEND_SEND_TEXT_USING_SMS == oMenuItem.m_iSubMenuId) 
				{
					Intent myIntent = new Intent( MyP2PWeb.this, AppletPeerSendSmsTextMsg.class );
					MyP2PWeb.this.startActivityForResult( myIntent, Constants.ACTIVITY_SMS_TEXT_MESSAGE );
				} 				
				else 
				{
				}*/
			}
		}
	}
	
	//========================================================================
	public void toGuiMyRelayConnected( boolean bRelayConnected )
	{
		if( bRelayConnected )
		{
			setStatusMessage( "Relay Connected" );
			if( ( Constants.eFriendViewAllProxies == m_iWhoToViewInList ) ||
				( Constants.eFriendViewMyProxies == m_iWhoToViewInList ) )
			{
				m_iWhoToViewInList = Constants.eFriendViewEverybody;
				if( m_MyP2PWebState.getIsNativeReadyForCalls() )
				{
					// reset which contacts to view
					m_iWhoToViewInList = m_MyApp.getMySettings().getWhichContactsToView();
					Log.i( LOG_TAG, "MyP2PWeb.toGuiMyRelayConnected calling RefreshContacts" );
					//RefreshContacts(); //BRJ I think this is causing issue being called too soon
				}
			}
		}
		else
		{
			setStatusMessage( "Relay Disconnected" );			
		}		
	}

	//========================================================================
	@Override
	public void doGuiNetworkStateChange( int networkState )
	{
		switch( networkState )
		{
		case Constants.eNetworkStateTypeUnknown:
		case Constants.eNetworkStateTypeLost:
		case Constants.eNetworkStateTypeNoInternetConnection:
			m_NetworkStateBut.setImageResource( R.drawable.button_network_disconnected );
			break;

		case Constants.eNetworkStateTypeAvail:
		case Constants.eNetworkStateTypeTestConnection:
		case Constants.eNetworkStateTypeRelaySearch:
		case Constants.eNetworkStateTypeAnnounce:
		case Constants.eNetworkStateTypeGetRelayList:
			m_NetworkStateBut.setImageResource( R.drawable.button_network_discover );
			break;

		case Constants.eNetworkStateTypeOnlineDirect:
			m_NetworkStateBut.setImageResource( R.drawable.button_network_direct_connect );
			m_oSearchBut.setEnabled( true );
			break;

		case Constants.eNetworkStateTypeOnlineThroughRelay:
			m_NetworkStateBut.setImageResource( R.drawable.button_network_relay );
			m_oSearchBut.setEnabled( true );
			break;

		case Constants.eNetworkStateTypeFailedResolveAnchor:
			m_NetworkStateBut.setImageResource( R.drawable.button_network_anchor );
			break;
		}		
	}

	//========================================================================
	@Override
	public void doGuiSetIdentHasTextOffers( VxNetIdent hisIdent, boolean haveTextOffer )
	{
		if( null != m_oContactsAdapter )
		{
			m_oContactsAdapter.doGuiSetIdentHasTextOffers( hisIdent, haveTextOffer );
		}		
	}

	//========================================================================
	@Override
	public void toGuiNewAssetMessage( int eAssetActionMsgIn, VxGUID onlineIdIn, int assetParamIn )
	{
		final int eAssetActionMsg = eAssetActionMsgIn;
		final VxGUID onlineId = onlineIdIn;
		final int assetParam = assetParamIn;
		try 
		{
			runOnUiThread(new Runnable() 
			{	
				@Override
				public void run() 
				{
				    final String orgName = Thread.currentThread().getName();
				    Thread.currentThread().setName(orgName + " MyP2PWeb:toGuiNewAssetMessage");		

					if( null != m_oContactsAdapter )
					{
						m_oContactsAdapter.doGuiNewAssetMessage( eAssetActionMsg, onlineId, assetParam );
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
	public void doGuiAssetViewingMessage( int eAssetActionMsg, VxGUID onlineId, int assetParam )
	{
		if( null != m_oContactsAdapter )
		{
			m_oContactsAdapter.doGuiAssetViewingMessage( eAssetActionMsg, onlineId, assetParam );
		}				
	}
}

