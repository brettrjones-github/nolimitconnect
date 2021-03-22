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

import android.app.Activity;
import android.content.Intent;
import android.graphics.drawable.Drawable;

import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.view.View.OnTouchListener;
import android.view.MotionEvent;
import android.widget.ImageView;
import android.widget.TextView;


public class AppletPeerGuiBase extends ActivityBase implements ToGuiActivityInterface 
{
	//=== constants ===//
	private static final String LOG_TAG = "AppletPeerGuiBase:";
	
	//=== vars ===//
    public VxNetIdent 			m_HisIdent 				= null;
	public boolean 				m_IsOffer 				= false;
	protected boolean 			m_IsServerPlugin 		= false;
	
    // friend
    public ImageView           	m_HisIdentIcon 			= null;    
    public TextView            	m_HisIdentName 			= null;
    public TextView            	m_HisIdentMoodMsg 		= null;
    public TextView            	m_HisIdentTruths 		= null;
    public TextView            	m_HisIdentIconSmall 	= null;

    // plugin
	int							m_ePluginType 			= Constants.ePluginTypeInvalid;	
    public ImageView           	m_oPluginIcon 			= null;  	
	public TextView				m_oPluginText 			= null; 	
	
	// camera
	public ImageViewWithSizer	m_VidCamView 			= null;
	private ImageViewWithSizer	m_VidCamThumbnail 		= null;
	private VidCamCtrlLogic		m_VidCamLogic			= null;
	public boolean 				m_bIsCamThumbnailVisible = false;
   
  	protected OfferSessionLogic	m_OfferSessionLogic		= null;

	//========================================================================
    @Override
    public void onCreate(Bundle savedInstanceState) 
    {
        super.onCreate(savedInstanceState);
        if( m_IsServerPlugin )
        {
    		m_HisIdent = m_MyApp.getUserAccount();
        	m_OfferSessionLogic = new OfferSessionLogic( this, this, m_MyApp, m_ePluginType, m_HisIdent, null );
        	m_OfferSessionLogic.setIsServerSession( true );
        }
        else
        {
	        Bundle extras = getIntent().getExtras(); 
	        if( null != extras ) 
	        { 
	        	m_IsOffer = extras.getBoolean("IS_OFFER"); 
	        } 
	        
	        if( m_IsOffer )
	        {
	        	m_OfferSessionLogic = new OfferSessionLogic( this, this, m_MyApp, m_ePluginType, m_HisIdent, m_MyApp.m_oGuiOfferSession );
	        }
	        else
	        {
	        	m_HisIdent = m_MyApp.getCurrentFriend();
	        	m_OfferSessionLogic = new OfferSessionLogic( this, this, m_MyApp, m_ePluginType, m_HisIdent, null );
	        }
        }
              
        setupBaseUI();
        
        setActivityReadyState( this );
        if( ( Constants.ePluginTypeCamServer == m_ePluginType )
        		 || ( Constants.ePluginTypeVoicePhone == m_ePluginType )
        		 || ( Constants.ePluginTypeVideoPhone == m_ePluginType )
        		 || ( Constants.ePluginTypeTruthOrDare == m_ePluginType ) )
        {
        	 showMicrophoneButton( true );
        }        
    }
    
	//========================================================================
	public boolean sendOfferOrResponse() 
	{
        if( false == m_OfferSessionLogic.sendOfferOrResponse() )
        {
            setActivityReadyState( null );
            finish();
            return false;
        }
        else
        {
            m_OfferSessionLogic.startPluginSessionIfIsSessionOffer();
            return true;
        }
    };
    
	//======================================================================== 
	@Override
	public void onOncePer500ms() 
	{
		if( null != m_VidCamLogic )
		{
			m_VidCamLogic.onOncePer500ms();
		}
		
		super.onOncePer500ms();
    };
    
  	//========================================================================
    public int 						getPluginType()												{ return m_ePluginType; }
    public boolean 					isSessionOffer()											{ return m_IsOffer; }
    public void 					setVidCamView(ImageViewWithSizer camView) 					{ m_VidCamView = camView; }
    public ImageViewWithSizer 		getVidCamView() 											{ return m_VidCamView; }
    public void 					setVidCamThumbnail( ImageViewWithSizer camViewThmbnail ) 	{ m_VidCamThumbnail = camViewThmbnail; }
    public ImageViewWithSizer 		getVidCamThumbnail() 										{ return m_VidCamThumbnail; }
     
 	 //========================================================================
     public void enableAudioControls( boolean enable )
     {
    	 showMicrophoneButton( enable );
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
       
  	 //========================================================================
     protected void setActivityReadyState( Activity curActivity )
     {
 		try 
 		{
 			if( null == curActivity )
 			{
 				NativeRxFrom.wantToGuiActivityClient( this, false );
 			}
 			else
 			{
				NativeRxFrom.wantToGuiActivityClient( this, true );
 			}
 		} 
 		catch( InterruptedException e ) 
 		{
 			e.printStackTrace();
 		}
    }
     
	//========================================================================
    public VxGUID getLclSessionId()
    {
        return m_OfferSessionLogic.getLclSessionId();    	
    }

	//========================================================================
    @Override
    protected void onPause() 
    {
        setActivityReadyState( null );
     	if( false == m_IsServerPlugin )
    	{
	 		if( m_bIsCamThumbnailVisible )
	 		{
	 			VxGUID myId = m_MyApp.getMyOnlineId();
	 			NativeTxTo.fromGuiWantMediaInput( myId.m_u64IdHiPart, myId.m_u64IdLoPart, Constants.eMediaInputVideoJpgSmall, false);
	 		}
    	}
     	
    	super.onPause();    	
    }

	//========================================================================
    @Override
    protected void onResume() 
    {
        setActivityReadyState( this );
    	if( false == m_IsServerPlugin )
    	{
 	 		if( m_bIsCamThumbnailVisible )
	 		{
	 			VxGUID myId = m_MyApp.getMyOnlineId();
	 			NativeTxTo.fromGuiWantMediaInput( myId.m_u64IdHiPart, myId.m_u64IdLoPart, Constants.eMediaInputVideoJpgSmall, true);
	 		}
    	}
     	
    	super.onResume();
    	if( null != m_VidCamLogic )
    	{
    		m_VidCamLogic.activityHasResumed();
    	}
    }

	//========================================================================
    @Override
    protected void onStop() 
    {
    	stopOnTimeEvents();
        setActivityReadyState( null );
        m_OfferSessionLogic.onStop();
    	super.onStop();    	
    }
    
	//========================================================================
    @Override
    public void onDestroy() 
    {
    	stopOnTimeEvents();
        setActivityReadyState( null );
    	if( null != m_VidCamLogic )
    	{
    		m_VidCamLogic.activityOnDestroy();
    	}
    	
		super.onDestroy();
    }
  
	//========================================================================
    @Override
    public void onBackPressed() 
    {
    	m_MyApp.playButtonClick();
		Intent oThisIntent = getIntent();
		setResult( RESULT_CANCELED, oThisIntent );
		super.onBackPressed();
    }
     
	//========================================================================
	private void setupBaseUI()
	{  
        GuiHelpers.setupTitle( getBaseContext(), m_ePluginType, m_TitleText, m_OfferSessionLogic.getIsMyself() );
        m_oPluginText = (TextView)this.findViewById(R.id.plugin_text);
        if( null != m_oPluginText)
        {
	    	int ePluginAccess = m_HisIdent.getPluginAccessState( m_ePluginType );
	    	String strAction = m_HisIdent.describePluginAction( m_ePluginType, ePluginAccess );
	    	m_oPluginText.setText(strAction);
	    	
	        m_oPluginIcon = (ImageView)this.findViewById(R.id.plugin_icon);
	        m_oPluginIcon.setBackgroundResource( MyIcons.getPluginIcon( m_ePluginType, ePluginAccess));
        	m_oPluginText.setOnTouchListener(new OnTouchListener() 
        	{ 
        		//@Override
        	    public boolean onTouch(View v, MotionEvent event) 
        		{ 
        	    	   switch (event.getAction()) 
         	    	   {
         	    	    case MotionEvent.ACTION_DOWN:
         	    	        //some code....
         	    	        break;
         	    	    case MotionEvent.ACTION_UP:
         	    	        v.performClick();
         	    	       onPluginClick();
         	    	        break;
         	    	    default:
         	    	        break;
         	    	    }
         	    	   
         	    	    return true;
        		} 
        	}); 
        	
        	m_oPluginIcon.setOnTouchListener(new OnTouchListener() 
        	{ 
        		//@Override
        	    public boolean onTouch(View v, MotionEvent event) 
        		{ 
     	    	   switch (event.getAction()) 
     	    	   {
     	    	    case MotionEvent.ACTION_DOWN:
     	    	        //some code....
     	    	        break;
     	    	    case MotionEvent.ACTION_UP:
     	    	        v.performClick();
     	    	       onPluginClick();
     	    	        break;
     	    	    default:
     	    	        break;
     	    	    }
     	    	   
     	    	    return true;
         	    } 
        	}); 
	    }
        
        setupFriend();
        setupCamView();
 	}
	
	//========================================================================
	private void setupCamView()
    {
        m_VidCamView = (ImageViewWithSizer)this.findViewById(R.id.reuse_cam_view);
        m_VidCamThumbnail = (ImageViewWithSizer)this.findViewById(R.id.reuse_cam_thumbnail);
        if( null != m_VidCamView )
        {
        	m_VidCamLogic = new VidCamCtrlLogic( m_MyApp, this, m_HisIdent, m_VidCamView, m_VidCamThumbnail );
        	if( Constants.ePluginTypeCamServer == m_ePluginType )
        	{
        		m_VidCamLogic.disablePreview(  true );
        	}
        	
        	m_VidCamLogic.initializeVidCamLogic();
        }
    }
 	   
	//========================================================================
	private void setupFriend()
	{    	
        m_HisIdentIcon = (ImageView)this.findViewById( R.id.friend_icon );
        if( null != m_HisIdentIcon )
        {
        	m_HisIdentIcon.setOnTouchListener(new OnTouchListener() 
        	{ 
        		//@Override
        	    public boolean onTouch(View v, MotionEvent event) 
        		{ 
     	    	   switch (event.getAction()) 
     	    	   {
     	    	    case MotionEvent.ACTION_DOWN:
     	    	        //some code....
     	    	        break;
     	    	    case MotionEvent.ACTION_UP:
     	    	        v.performClick();
     	    	       onFriendClick();
     	    	        break;
     	    	    default:
     	    	        break;
     	    	    }
     	    	   
     	    	    return true;
        	    } 
        	}); 
        }
        
        m_HisIdentIconSmall = (TextView)this.findViewById( R.id.friend_icon_small );
        if( null != m_HisIdentIconSmall )
        {
        	m_HisIdentIconSmall.setOnTouchListener(new OnTouchListener() 
        	{ 
        		//@Override
        	    public boolean onTouch(View v, MotionEvent event) 
        		{ 
     	    	   switch (event.getAction()) 
     	    	   {
     	    	    case MotionEvent.ACTION_DOWN:
     	    	        //some code....
     	    	        break;
     	    	    case MotionEvent.ACTION_UP:
     	    	        v.performClick();
     	    	       onFriendClick();
     	    	        break;
     	    	    default:
     	    	        break;
     	    	    }
     	    	   
     	    	    return true;
        	    } 
        	}); 
        }
        
        m_HisIdentName = (TextView)this.findViewById( R.id.friend_name );
        if( null != m_HisIdentName )
        {
        	m_HisIdentName.setOnTouchListener(new OnTouchListener() 
        	{ 
        		//@Override
        	    public boolean onTouch(View v, MotionEvent event) 
        		{ 
      	    	   switch (event.getAction()) 
      	    	   {
      	    	    case MotionEvent.ACTION_DOWN:
      	    	        //some code....
      	    	        break;
      	    	    case MotionEvent.ACTION_UP:
      	    	        v.performClick();
      	    	        onFriendClick();
      	    	        break;
      	    	    default:
      	    	        break;
      	    	    }
      	    	   
      	    	    return true;
        		} 
        	}); 
        }
        
        m_HisIdentMoodMsg = (TextView)this.findViewById( R.id.friend_mood_msg );
        if( null != m_HisIdentMoodMsg )
        {
        	m_HisIdentMoodMsg.setOnTouchListener(new OnTouchListener() 
        	{ 
        		//@Override
        	    public boolean onTouch(View v, MotionEvent event) 
        		{ 
      	    	   switch (event.getAction()) 
      	    	   {
      	    	    case MotionEvent.ACTION_DOWN:
      	    	        //some code....
      	    	        break;
      	    	    case MotionEvent.ACTION_UP:
      	    	        v.performClick();
      	    	       onFriendClick();
      	    	        break;
      	    	    default:
      	    	        break;
      	    	    }
      	    	   
      	    	    return true;
        	    } 
        	}); 
        }
        
        m_HisIdentTruths = (TextView)this.findViewById( R.id.friend_truths );
        if( null != m_HisIdentTruths )
        {
        	m_HisIdentTruths.setOnTouchListener(new OnTouchListener() 
        	{ 
        		//@Override
        	    public boolean onTouch(View v, MotionEvent event) 
        		{ 
      	    	   switch (event.getAction()) 
      	    	   {
      	    	    case MotionEvent.ACTION_DOWN:
      	    	        //some code....
      	    	        break;
      	    	    case MotionEvent.ACTION_UP:
      	    	        v.performClick();
      	    	       onFriendClick();
      	    	        break;
      	    	    default:
      	    	        break;
      	    	    }
      	    	   
      	    	    return true;
        	    } 
        	}); 
        }
       
        fillFriend();
	}
	
	//========================================================================
	private void  fillFriend()
	{
		if( null != m_HisIdent )
		{
			int iFriendshipType = m_HisIdent.getMyFriendshipToHim();
			if( null != m_HisIdentIcon )
			{
				m_HisIdentIcon.setImageResource( Constants.getFriendIconResource( iFriendshipType ));  
			}
			
			String strFriendship = m_HisIdent.describeMyFriendshipToHim();
			
			if( null != m_HisIdentIconSmall )
			{
	        	Drawable img = getBaseContext().getResources().getDrawable(  Constants.getFriendIconResource( iFriendshipType ) ); 
	        	img.setBounds( 0, 0, 48, 48 ); 
	        	m_HisIdentIconSmall.setCompoundDrawables( img, null, null, null ); 
	        	m_HisIdentIconSmall.setText( strFriendship + m_HisIdent.m_as8OnlineName ); 
			}
			
			if( null != m_HisIdentName )
			{
				m_HisIdentName.setText( m_HisIdent.m_as8OnlineName + " - " + strFriendship ); 
			}
			
			if( null != m_HisIdentMoodMsg )
			{
				m_HisIdentMoodMsg.setText( m_HisIdent.getOnlineDescription() );	
			}
			
			if( null != m_HisIdentTruths )
			{
				m_HisIdentTruths.setText( m_HisIdent.getTruthsAndDaresDescription() );	
			}		
		}
	}

	//========================================================================
	@Override
	public void onActivityResult(int requestCode, int resultCode, Intent data) 
	{		
		super.onActivityResult(requestCode, resultCode, data);
		Log.d(LOG_TAG, "onActivityResult result code " + resultCode);
		switch (requestCode) 
		{
		case Constants.ACTIVITY_CAMERA_SETTINGS:
			Log.d( LOG_TAG, "onActivityResult ACTIVITY_CAMERA_SETTINGS" );
			//m_iImageRotation = m_MyApp.getMySettings().getCamImageOrientation();
			//setPreviewVisibility();
			break;
		}
		
		setCamCaptureActivity( this );
	}
	
	//========================================================================
	public VxNetIdent getHisIdent() 
	{
		return m_HisIdent;
	}	
	
	//========================================================================
	public void webCamSourceOffline() 
	{
	}	
		
	//========================================================================       
    private void onFriendClick()
    {
    	viewFriendProfile();
    }
    
	//========================================================================       
    public void viewFriendProfile() 
    {
    	if( null != m_HisIdent )
    	{
    		Intent launchBrowser = new Intent( Intent.ACTION_VIEW, m_HisIdent.getProfileUri( m_MyApp.getUserAccount().getMyExternalIp() ) );
    		startActivity(launchBrowser); 		
    	}
	}   
	
	//========================================================================       
    private void onPluginClick()
    {
    	showSetPluginPermissions();
    }
	
	//========================================================================       
    // for derived classes to override and get is in session state
    public void onInSession( boolean isInSession )
    {
    }
   
	//========================================================================       
    public void showSetPluginPermissions() 
    {
		Intent myIntent = new Intent(this, ActivityEditPermission.class );
		myIntent.putExtra("PluginType", m_ePluginType );
		startActivityForResult( myIntent, Constants.ACTIVITY_EDIT_PERMISSION );       	
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

	//======================================================================== 
	@Override
	public void toGuiContactOnline( VxNetIdent hisIdent, boolean newContact ) 
	{
		m_OfferSessionLogic.toGuiContactOnline( hisIdent, newContact );
	}

	//======================================================================== 
	@Override
	public void toGuiContactOffline( VxNetIdent hisIdent ) 
	{
		m_OfferSessionLogic.toGuiContactOffline( hisIdent );
	}
    
 	//========================================================================
    public void toGuiPlayVideoFrame( VxGUID guidId, byte[] pu8Jpg, int jpgDataLen, int motion0to100000 )
    {
    	if( null != m_VidCamLogic )
    	{
    		m_VidCamLogic.toGuiPlayVideoFrame( guidId, pu8Jpg, jpgDataLen, motion0to100000 );
    	}
    	else
    	{
	    	if( ( null != m_VidCamView ) && guidId.equals( m_VidCamView.getVideoFeedId() ) )
	    	{
		    	m_VidCamView.toGuiPlayVideoFrame( guidId, pu8Jpg, jpgDataLen, motion0to100000 );
	    	}
	    	
	    	if( null != m_VidCamThumbnail )
	    	{
		    	m_VidCamThumbnail.toGuiPlayVideoFrame( guidId, pu8Jpg, jpgDataLen, motion0to100000 );		
	     	}   
    	}
    }

	//======================================================================== 
	@Override
	public void toGuiInstMsg(VxNetIdent friendIdent, int ePluginType, String strMsg ) 
	{
	}

	//======================================================================== 
	@Override
	public void toGuiMultiSessionAction( int eMSessionAction, VxGUID onlineId, int actionParam ) 
	{
	}

	//======================================================================== 
	@Override
	public void toGuiSetGameActionVar(	int iPlugin, VxGUID sessionId, int s32ActionId, int s32VarValue) 
	{
	}

	//======================================================================== 
	@Override
	public void toGuiSetGameValueVar(	int iPlugin, VxGUID sessionId, int s32VarId, int s32VarValue) 
	{
	}
 }
