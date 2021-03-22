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

import android.content.Intent;
import android.util.*;
import android.view.*;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.RadioButton;
import android.widget.ImageView;
import android.widget.RadioGroup;
import android.widget.TextView;

import android.os.Bundle;

public class AppletPeerChangeFriendship extends ActivityBase implements RadioGroup.OnCheckedChangeListener 
{
    public static final String LOG_TAG = "ChangeFriendship:";
    //=== activity vars ===//
    
    private VxNetIdent 			m_HisIdent = null;
    
    // text
    private TextView            m_HisIdentLabel = null;
    private TextView            m_HisIdentText = null;
    private ImageView           m_HisIdentIcon = null;
    
    private TextView            m_oMeLabel = null;  
    private TextView            m_oMeText = null;  
    private ImageView           m_oMeIcon = null;
    private int           		m_eMyFriendshipToHim = 0;

 	// widgets
    protected static RadioGroup m_oRadioGroup;

    // buttons
    private Button              m_oOkBut 				= null;
    private Button              m_oCancelBut 			= null;
     
    private RadioButton 		m_oRadioButtonAdmin 	= null; 
    private RadioButton 		m_oRadioButtonFriend 	= null; 
    private RadioButton 		m_oRadioButtonGuest 	= null; 
    private RadioButton 		m_oRadioButtonAnonymous = null; 
    private RadioButton 		m_oRadioButtonIgnore 	= null; 
    
    //========================================================================
    /** Called when the activity is first created. */
    @Override
    public void onCreate(Bundle savedInstanceState) 
    {
        setContentView(R.layout.change_friendship);
        super.onCreate(savedInstanceState);
        setPageTitle( R.string.activity_change_friendship_name );
        
        m_HisIdent = m_MyApp.getCurrentFriend();
        
        // launch Login activity
        Log.i(LOG_TAG, "onCreateCalled");
        
        m_oRadioGroup = (RadioGroup)this.findViewById(R.id.permission_radio_group);     
        m_oRadioGroup.setOnCheckedChangeListener(this);
        m_oRadioGroup.setPadding(0, 0, 0, 20);
        
        this.m_HisIdentLabel = (TextView)this.findViewById(R.id.friend_friendship);
        m_HisIdentLabel.setText( m_HisIdent.m_as8OnlineName + getString( R.string.friendship_to_me ) ); 
        
        this.m_HisIdentText = (TextView)this.findViewById(R.id.friend_text);
        this.m_HisIdentText.setText( m_HisIdent.describeHisFriendshipToMe() );
        
        this.m_HisIdentIcon = (ImageView)this.findViewById(R.id.imageViewFriend);
        updateFriendIcon( m_HisIdentIcon, m_HisIdent.getHisFriendshipToMe() );
        
        this.m_oMeLabel = (TextView)this.findViewById(R.id.me_friendship);   
        m_oMeLabel.setText( "My Friendship To " + m_HisIdent.m_as8OnlineName );
        m_oMeLabel.setPadding(0, 20, 0, 0);
        
        this.m_oMeText = (TextView)this.findViewById(R.id.me_text); 
        this.m_oMeIcon = (ImageView)this.findViewById(R.id.imageViewMe);
        m_oMeIcon.setPadding(0, 0, 0, 20);
        
        this.m_oRadioButtonAdmin 		= (RadioButton)this.findViewById(R.id.admin_permission);
        this.m_oRadioButtonFriend 		= (RadioButton)this.findViewById(R.id.friend_permission);
        this.m_oRadioButtonGuest 		= (RadioButton)this.findViewById(R.id.guest_permission);
        this.m_oRadioButtonAnonymous 	= (RadioButton)this.findViewById(R.id.anonymous_permission);
        this.m_oRadioButtonIgnore 		= (RadioButton)this.findViewById(R.id.ignore_permission);
        
        m_eMyFriendshipToHim = m_HisIdent.getMyFriendshipToHim();
        updateRadioButtonFromMyFriendshipToHim();
        
        updateMyFriendshipToHim();
        
        this.m_oOkBut = (Button)this.findViewById(R.id.reuse_accept_but);
        this.m_oOkBut.setOnClickListener(new OnClickListener() 
        {
        	public void onClick(View v) 
        	{
  			   	m_MyApp.playButtonClick();
        		OnOkButClick(v);
        	}
        });     

        this.m_oCancelBut = (Button)this.findViewById(R.id.reuse_cancel_but);
        this.m_oCancelBut.setOnClickListener(new OnClickListener() 
        {
        	public void onClick(View v) 
        	{
  			   	m_MyApp.playButtonClick();
  			   	OnCancelButClick(v);
        	}
        }); 
        
    }
    
    //========================================================================
    void updateRadioButtonFromMyFriendshipToHim()
    {
    	m_oRadioButtonAdmin.setChecked(false);
    	m_oRadioButtonFriend.setChecked(false);
    	m_oRadioButtonGuest.setChecked(false);
    	m_oRadioButtonAnonymous.setChecked(false);
    	m_oRadioButtonIgnore.setChecked(false);
    	
        m_eMyFriendshipToHim = m_HisIdent.getMyFriendshipToHim();
		switch( m_eMyFriendshipToHim )
		{
		case  Constants.eFriendStateAdministrator:
	    	m_oRadioButtonAdmin.setChecked(true);
	      	Log.i(LOG_TAG, "checked admin " );
			break;
		case  Constants.eFriendStateFriend:
	    	m_oRadioButtonFriend.setChecked(true);
	      	Log.i(LOG_TAG, "checked friend " );
			break;
		case Constants.eFriendStateGuest:
	    	m_oRadioButtonGuest.setChecked(true);
	      	Log.i(LOG_TAG, "checked guest " );
	      	break;
		case Constants.eFriendStateAnonymous:
	    	m_oRadioButtonAnonymous.setChecked(true);
	      	Log.i(LOG_TAG, "checked anonymous " );
	      	break;
		case Constants.eFriendStateIgnore:
	    	m_oRadioButtonIgnore.setChecked(true);
	      	Log.i(LOG_TAG, "checked ignored " );
	      	break;			
		}    	 
    }
    
    //========================================================================
    public void updateMyFriendshipToHim()
    {
    	updateFriendIcon( m_oMeIcon, m_eMyFriendshipToHim );
        this.m_oMeText.setText(  Constants.describeFriendState( m_eMyFriendshipToHim ) );
    }
    
    //========================================================================
    public void updateFriendIcon( ImageView oFriendIcon, int iFriendshipType )
    {
    	Log.i(LOG_TAG, "updateFriendIcon " + iFriendshipType );
    	oFriendIcon.setImageResource( Constants.getFriendIconResource( iFriendshipType ));   
    }
    
    //========================================================================
	//@Override
	public void onCheckedChanged(RadioGroup arg0, int checkedId) 
	{
	   	Log.i(LOG_TAG, "onCheckedChanged " + checkedId );

		if( R.id.admin_permission == checkedId )
		{
			m_eMyFriendshipToHim = Constants.eFriendStateAdministrator;
		}
		else if( R.id.friend_permission == checkedId )
		{
			m_eMyFriendshipToHim = Constants.eFriendStateFriend;
		}
		else if( R.id.guest_permission == checkedId )
		{
			m_eMyFriendshipToHim = Constants.eFriendStateGuest;
		}
		else if( R.id.anonymous_permission == checkedId )
		{		
			m_eMyFriendshipToHim = Constants.eFriendStateAnonymous;
		}
		else if( R.id.ignore_permission == checkedId )
		{		
			m_eMyFriendshipToHim = Constants.eFriendStateIgnore;			
		}
		
		updateMyFriendshipToHim();
		
    	Log.i(LOG_TAG, "current friend permission " + m_eMyFriendshipToHim );
    	m_HisIdent.setMyFriendshipToHimFlags( (byte)m_eMyFriendshipToHim);	
    	byte eMyFriendshipToHim = m_HisIdent.getMyFriendshipToHimFlags();
    	Log.i(LOG_TAG, "result friend permission " + eMyFriendshipToHim );
	}
    	  
    //======================================================================== 
    public void OnOkButClick(View v) 
    {
    	m_HisIdent.setMyFriendshipToHimFlags( (byte)m_eMyFriendshipToHim);
    	Log.i(LOG_TAG, "friend permission " + m_eMyFriendshipToHim + " " + m_HisIdent.m_as8OnlineName +  m_HisIdent.describeMyId() );
    	NativeTxTo.fromGuiChangeMyFriendshipToHim( m_HisIdent.m_u64IdHiPart, m_HisIdent.m_u64IdLoPart, m_eMyFriendshipToHim, m_HisIdent.getHisFriendshipToMe() );
    	Log.i(LOG_TAG, "friend permission " + m_eMyFriendshipToHim + " " + m_HisIdent.m_as8OnlineName +  m_HisIdent.describeMyId() );
		// get intent for this activity
		Intent oThisIntent = getIntent();
		// return result of activity back to MainActivity
		setResult(RESULT_OK, oThisIntent);
		finish();
    }   
    
    //========================================================================  
	public void OnCancelButClick(View v) 
	{
  		// get intent for this activity
		Intent oThisIntent = getIntent();
		// return result of activity back to MainActivity
		setResult(RESULT_CANCELED, oThisIntent);
		finish();
	}    
    
 }
