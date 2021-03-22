//============================================================================
// Copyright (C) 2016 Brett R. Jones
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
import android.app.AlertDialog;
import android.content.DialogInterface;
import android.content.Intent;
import android.util.Log;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.LinearLayout;
import android.widget.TextView;


public class OfferBarWidget implements ToGuiOfferInterface
{
	private static final String LOG_TAG = "OfferBarWidget:";

	private MyApp				m_MyApp 				= null;
	private OffersMgr			m_OffersMgr				= null;
	//private VxGUID				m_OfferSessionId		= null;
	//private int					m_ePluginType			= Constants.ePluginTypeInvalid;
	
	private Activity 			m_Activity				= null;
	private LinearLayout 		m_OfferLayout			= null;
	private TextView 			m_TotalOffersTextView 	= null;
	private Button 				m_OfferButton			= null;
	private Button 				m_AcceptButton			= null;
	private Button 				m_RejectButton			= null; 
	private TextView 			m_TopTextView			= null; 
	private TextView 			m_MissedCallsLabel		= null; 
	private TextView 			m_MissedCallsTextView	= null; 
	private TextView 			m_BottomTextView		= null;
	
	private int 				m_OfferCount			= 0; 
	private GuiOfferSession 	m_OfferState 			= null;
	private VxNetIdent			m_HisIdent				= null;
	private boolean 			m_OfferAcceptable		= false; 
	private boolean 			m_OfferBarVisible		= false; 
	
	//========================================================================
	OfferBarWidget( 	MyApp myApp, Activity activity, 
						LinearLayout offerLayout, TextView totalOffersTextView, 
						Button offerIcon, Button acceptButton, Button rejectButton, 
						TextView topTextView, 
						TextView missedCallsLabel, 
						TextView missedCallsTextView, 
						TextView bottomTextView )
	{
		m_MyApp 				= myApp;
		m_OffersMgr 			= myApp.getOffersMgr();
		m_Activity				= activity;
		
		m_OfferLayout			= offerLayout;
		m_TotalOffersTextView 	= totalOffersTextView;
		m_OfferButton			= offerIcon;
		m_AcceptButton			= acceptButton;
		m_RejectButton			= rejectButton; 
		m_TopTextView			= topTextView; 
		m_MissedCallsLabel		= missedCallsLabel; 
		m_MissedCallsTextView	= missedCallsTextView; 
		m_BottomTextView		= bottomTextView; 
		if( null != m_OfferButton )
		{
			m_OfferButton.setOnClickListener(new OnClickListener() 
	    	{
	            public void onClick(View v) 
	            {
		        	m_MyApp.playButtonClick();
		        	onAcceptOfferButtonClicked();
	            }
	        });     		
		}
		
		if( null != m_AcceptButton )
		{
			m_AcceptButton.setOnClickListener(new OnClickListener() 
	    	{
	            public void onClick(View v) 
	            {
		        	m_MyApp.playButtonClick();
		        	onAcceptOfferButtonClicked();
	            }
	        });     		
		}
		
		if( null != m_RejectButton )
		{
			m_RejectButton.setOnClickListener(new OnClickListener() 
	    	{
	            public void onClick(View v) 
	            {
		        	m_MyApp.playButtonClick();
		        	onRejectOfferButtonClicked();
	            }
	        });     		
		}
		
		m_OffersMgr.wantToGuiOfferCallbacks( this, true );
		updateOfferBar();
	}
	
	//========================================================================
	private void onAcceptOfferButtonClicked()
	{
    	if( ( 0 == m_OfferCount )
    		|| ( null == m_OfferState ) )
    	{
    		return;
    	}
    	
		GuiOfferSession offerSession = m_OfferState;
		if( null == offerSession )
		{
			return;
		}
    	
    	if( m_OfferAcceptable )
    	{
    		Log.i( LOG_TAG, "onAcceptOfferButtonClicked " + m_HisIdent.m_as8OnlineName + m_HisIdent.describeMyId() );
    		m_MyApp.setCurrentFriend( m_HisIdent );
    		m_MyApp.setCurrentGuiOfferSession( offerSession );
    		Intent myIntent = null;
    		switch( offerSession.m_ePluginType )
    		{       			
    		case Constants.ePluginTypeMultiSession:
    			myIntent = new Intent( m_Activity, AppletPeerMultiSession.class);
    			myIntent.putExtra("IS_OFFER", true);
    			m_Activity.startActivityForResult( myIntent, Constants.ACTIVITY_TO_FRIEND_MULTISESSION );
    			break;  		
    		
    		case Constants.ePluginTypeVoicePhone:
    			myIntent = new Intent(m_Activity, AppletPeerVoicePhone.class);
    			myIntent.putExtra("IS_OFFER", true);
    			m_Activity.startActivityForResult( myIntent, Constants.ACTIVITY_TO_FRIEND_VOICE_PHONE );
    			break;  	
        		
    		case Constants.ePluginTypeVideoPhone:
    			myIntent = new Intent(m_Activity, AppletPeerVideoPhone.class);
    			myIntent.putExtra("IS_OFFER", true);
    			m_Activity.startActivityForResult( myIntent, Constants.ACTIVITY_TO_FRIEND_VIDEO_PHONE );
    			break;  	
        		
    		case Constants.ePluginTypeTruthOrDare:
    			myIntent = new Intent(m_Activity, AppletPeerTodGame.class);
    			myIntent.putExtra("IS_OFFER", true);
    			m_Activity.startActivityForResult( myIntent, Constants.ACTIVITY_TO_FRIEND_TOD_GAME );
    			break;  	
        	

    		case Constants.ePluginTypeFileOffer:
  	        	VxGUID lclSessionId = offerSession.getLclSessionId();
  	        	if( null == lclSessionId )
  	        	{
  	        		lclSessionId = offerSession.getRmtSessionId();
  	        	}
  	        	
  	        	if( null == lclSessionId )
  	        	{
 	        		lclSessionId = new VxGUID();
  	        		lclSessionId.initializeWithNewGUID();
  	        		offerSession.setLclSessionId( lclSessionId );
  	        	}
  	        	else if( false == lclSessionId.isOnlineIdValid() )
  	        	{
  	        		lclSessionId.initializeWithNewGUID();
  	        		offerSession.setLclSessionId( lclSessionId );
  	        	}
  	        	
  	        	if( false == NativeTxTo.fromGuiToPluginOfferReply( offerSession.getPluginType(), m_HisIdent.m_u64IdHiPart, m_HisIdent.m_u64IdLoPart,
	            				0, // user data
	            				Constants.eOfferResponseAccept,
	            				lclSessionId.m_u64IdHiPart,
	            				lclSessionId.m_u64IdLoPart ) )
  	        	{
  	        		errMsgBox( false, m_HisIdent.getOnlineName() + " Is Offline " );
  	        	}
  	        	
	        	break; 
    		}
    	}		
    	
		m_MyApp.getOffersMgr().offerAcknowlegedByUser( offerSession );
	}
	
	//========================================================================
	private void onRejectOfferButtonClicked()
	{
    	if( m_OfferAcceptable )
    	{
    		VxGUID lclSessionId = m_OfferState.getLclSessionId();
    		NativeTxTo.fromGuiToPluginOfferReply( 	m_OfferState.getPluginType(), 
								    				m_HisIdent.m_u64IdHiPart, m_HisIdent.m_u64IdLoPart,
								    				0, // user data
								    				Constants.eOfferResponseReject,
								    				lclSessionId.m_u64IdHiPart,
								    				lclSessionId.m_u64IdLoPart ); 		
    	}
    	
		m_MyApp.getOffersMgr().offerAcknowlegedByUser( m_OfferState );
	}

	//========================================================================
	private void updateOfferBar()
	{
		if( null == m_OfferLayout )
		{
			return;
		}
		
		m_OfferCount = m_OffersMgr.getTotalOfferCount();
		if( 0 == m_OfferCount )
		{
			m_OfferState = null;
			setOfferBarVisibility( false );
		}
		else
		{
			setOfferBarVisibility( true );			
			m_OfferState = m_OffersMgr.getTopGuiOfferSession();
			if( null != m_OfferState )
			{
				if( null != m_TotalOffersTextView )
				{
					String offerCnt = "" + m_OfferCount;
					m_TotalOffersTextView.setText( offerCnt );
				}
				
				m_HisIdent 			= m_OfferState.getHisIdent();
				GuiOfferSession offerSession = m_OfferState;
				if( null != offerSession )
				{				
					m_OfferAcceptable 	= true;
					if( offerSession.getIsSessionEnded() )
					{
						m_OfferAcceptable = false;
					}
					
					boolean isOnline 	= m_HisIdent.isOnline();
					if( isOnline )
					{
						m_OfferLayout.setBackgroundResource( R.color.offer_bar_bkg_color );
					}
					else
					{
						m_OfferLayout.setBackgroundResource( R.color.offline_bkg_color );
						m_OfferAcceptable = false;
					}
					
					if( true == m_OfferAcceptable )
					{
						EOfferState offerState = m_OfferState.getOfferState();
						if(  ( EOfferState.eOfferStateRxedOffer == offerState )
							|| ( EOfferState.eOfferStateSentAccept == offerState ) )
						{
							// ok
						}
						else
						{
							m_OfferAcceptable = false;
						}					
					}
					
					if( null != m_AcceptButton )
					{
			       		if( m_OfferAcceptable ) 
			       		{
			       			m_AcceptButton.setBackgroundResource( R.drawable.button_accept_normal );
			       			m_AcceptButton.setEnabled( true );
				       	}
			       		else
			       		{
			       			m_AcceptButton.setBackgroundResource( R.drawable.button_accept_disabled );
			       			m_AcceptButton.setEnabled( false );			
			       		}						
					}
				
		            if( null != m_TopTextView ) 
		            {
		            	m_TopTextView.setText( m_HisIdent.getOnlineName() + " - " + m_HisIdent.describeMyFriendshipToHim() );                            
		            }
		            
		            if( null != m_BottomTextView ) 
		            {
		            	String offerMsg = offerSession.getOfferMsg();
		            	if( 0 != offerMsg.length() )
		            	{
		            		m_BottomTextView.setText( offerMsg );
		            	}
		            }
		            
		            if( null != m_MissedCallsLabel && null != m_MissedCallsTextView ) 
		            {
		            	if( 0 != offerSession.m_MissedCallCount )
		            	{
		            		m_MissedCallsLabel.setVisibility( View.VISIBLE );
		            		m_MissedCallsTextView.setVisibility( View.VISIBLE );
		            		String missedCnt = String.valueOf( offerSession.m_MissedCallCount );
		            		m_MissedCallsTextView.setText( missedCnt );             	            		
		            	}
		            	else
		            	{
		            		m_MissedCallsLabel.setVisibility( View.GONE );
		            		m_MissedCallsTextView.setVisibility( View.GONE );           		
		            	}
		            }  
		            		             
		            if( null != m_OfferButton )
		            {
			            int iIconResource = MyIcons.getPluginIcon( offerSession.m_ePluginType, Constants.ePluginAccessOk ); 
		            	m_OfferButton.setBackgroundResource(iIconResource);
		            }
		       		
		       		if( null != m_RejectButton )
		       		{
			       		m_RejectButton.setBackgroundResource(R.drawable.button_cancel_normal);		       			
		       		}
				}
			}
		}	
	}
	
	//========================================================================
	private void setOfferBarVisibility( boolean visible )
	{
		if( m_OfferBarVisible != visible )
		{
			m_OfferBarVisible = visible;
			if( null != m_OfferLayout )
			{
				m_OfferLayout.setVisibility( m_OfferBarVisible ? View.VISIBLE : View.GONE );
			}			
		}
	}
	
	//========================================================================
	@Override
	public void doGuiUpdatePluginOffer( GuiOfferSession offerState )
	{
		updateOfferBar();
	}
	
	//========================================================================
	@Override
	public void doGuiOfferRemoved( GuiOfferSession offerState )
	{
		updateOfferBar();
	}
	
	//========================================================================
	@Override
	public void doGuiAllOffersRemoved()
	{
		updateOfferBar();
	}

	//========================================================================
	public void shudownOfferBarWidget()
	{
		m_OffersMgr.wantToGuiOfferCallbacks( this, false );
		m_OfferCount			= 0; 
		m_OfferAcceptable		= false; 
	}

	
	//============================================================================
	//! show message box to user
	void errMsgBox( boolean bExitWhenClicked, String strMsg )
	{
		final boolean exitWhenClicked = bExitWhenClicked;
		AlertDialog.Builder builder = new AlertDialog.Builder(m_Activity); 
		builder.setMessage(strMsg) 
		       .setPositiveButton("Ok", new DialogInterface.OnClickListener() 
		       { 
		           public void onClick(DialogInterface dialog, int id) 
		           { 
	     			   m_MyApp.playButtonClick();
		               // Handle Ok 
		        	   if( exitWhenClicked )
		        	   {
		        		   //onBackButtonPressed();
		        	   }
		           } 
		       }) 
		       /*
		       .setNegativeButton("Cancel", new DialogInterface.OnClickListener() 
		       { 
		           public void onClick(DialogInterface dialog, int id) 
		           { 
		               // Handle Cancel 
     			   		m_MyApp.playButtonClick();
		           } 
		       }) 
		       */
		       .create().show(); 	
	}
};
