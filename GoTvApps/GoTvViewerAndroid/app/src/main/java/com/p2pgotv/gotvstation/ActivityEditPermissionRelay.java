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

import android.content.*;
import android.util.Log;
import android.view.*;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.EditText;
import android.widget.ImageView;
import android.widget.TextView;
import android.widget.Toast;
import android.os.Bundle;
import android.widget.RadioGroup;

public class ActivityEditPermissionRelay extends ActivityBase implements RadioGroup.OnCheckedChangeListener 
{
	//=== constants ===//
	private static final String LOG_TAG = "ActivityEditPermissionRelay";
	
	//=== vars ===//
	public Button 						m_CloseButton 			= null;
	public TextView            			m_TitleText 			= null;
	
    private int 						m_iPermissionChoice 	= -1;
    private int							m_ePluginType = Constants.ePluginTypeInvalid;

 	// widgets
    protected static RadioGroup 		m_oRadioGroup;
    ImageView							m_oPluginIcon 			= null;
	TextView							m_oPluginLabel 			= null;
 	Button 								m_oOkBut 				= null;
 	Button 								m_oCancelBut 			= null;
 	EditText							m_EditUserRelayCnt 		= null;
 	EditText							m_EditSystemRelayCnt 	= null;
 	int									m_UserRelayCnt			= 0;
 	int									m_SystemRelayCnt		= 2;
	
    //========================================================================
    @Override
    public void onCreate(Bundle savedInstanceState) 
    {
        setContentView(R.layout.activity_edit_permission_relay);
        super.onCreate(savedInstanceState);
        setPageTitle( R.string.activity_edit_relay_permissions );
        
        Bundle extras = getIntent().getExtras(); 
        if( null != extras ) 
        { 
        	m_ePluginType = extras.getInt("PluginType"); 
        } 
        
		m_UserRelayCnt = m_MyApp.getMySettings().getUserRelayPermissionCount();
		if( m_UserRelayCnt < 0 )
		{
			m_UserRelayCnt = 0;
		}
		if( m_UserRelayCnt > 1000 )
		{
			m_UserRelayCnt = 1000;
		}

		m_SystemRelayCnt  = m_MyApp.getMySettings().getSystemRelayPermissionCount();
		if( m_SystemRelayCnt < 2 )
		{
			m_SystemRelayCnt = 2;
		}
		if( m_SystemRelayCnt > 1000 )
		{
			m_SystemRelayCnt = 1000;
		}
     	
        m_EditUserRelayCnt 	= (EditText)this.findViewById( R.id.relay_user_cnt_edit );
        if( null != m_EditUserRelayCnt )
        {
        	m_EditUserRelayCnt.setText( String.valueOf( m_UserRelayCnt ) );
        }
        
        m_EditSystemRelayCnt 	= (EditText)this.findViewById( R.id.relay_anon_cnt_edit );;
        if( null != m_EditSystemRelayCnt )
        {
        	m_EditSystemRelayCnt.setText( "2");
        }

       
        if( Constants.ePluginTypeInvalid != m_ePluginType )
        {	      
	        m_TitleText = (TextView)this.findViewById( R.id.label_title );
	        GuiHelpers.setupTitle(getBaseContext(), m_ePluginType, m_TitleText, false );
	        if( null != m_TitleText )
	        {
	        	m_TitleText.setText(R.string.activity_edit_relay_permissions );
	        }
	        
	        m_CloseButton = (Button)this.findViewById(R.id.button_close);
	        if( null != m_CloseButton )
	        {
		        m_CloseButton.setOnClickListener(new OnClickListener() 
		        {
				 	  public void onClick(View v) 
				 	  {
						  m_MyApp.playButtonClick();
						  OnCancelButClick();
				 	  }
				 });	
	        }
	              
	        m_oRadioGroup = (RadioGroup)this.findViewById(R.id.permission_radio_group);     
	        m_oRadioGroup.setOnCheckedChangeListener(this);
	   
	    	this.m_oOkBut = (Button)this.findViewById(R.id.but_permission_ok);
		 	this.m_oOkBut.setOnClickListener(new OnClickListener() 
		 	{
		 		public void onClick(View v) 
		 		{
				    m_MyApp.playButtonClick();
				    OnOkButClick(v);
		 		}
		 	});
		 	
	    	this.m_oCancelBut = (Button)this.findViewById(R.id.but_permission_cancel);
		 	this.m_oCancelBut.setOnClickListener(new OnClickListener() 
		 	{
		 		public void onClick(View v)
		 		{
				    m_MyApp.playButtonClick();
				    OnCancelButClick();
		 		}
		 	});
		 	
	       	m_iPermissionChoice = m_MyApp.getUserAccount().getPluginPermission(m_ePluginType);
	        updatePermissionButtons();
        }
        else
        {
        	Toast.makeText(this, " Invalid Plugin.", Toast.LENGTH_SHORT).show();	
        	finish();
        }
    }

    //========================================================================
    @Override
    public void onBackPressed() 
    {
    	OnCancelButClick();
    }
    
    //========================================================================
    public void OnCancelButClick() 
    { 
    	// get intent for this activity
		Intent oThisIntent = getIntent();
		// return result of edit activity back to MainActivity
		setResult(RESULT_CANCELED, oThisIntent);
		finish();
    }
    
    //========================================================================
    public void OnOkButClick(View v) 
    { 
    	updatePluginPermissions();
  		// get intent for this activity
		Intent oThisIntent = getIntent();
		setResult(RESULT_OK, oThisIntent);
		finish();
    }
    
    //============================================================================
    //! update ident and save to database then send permission change to engine
    void updatePluginPermissions()
    {
    	m_MyApp.getUserAccount().setPluginPermission( m_ePluginType, m_iPermissionChoice );
    	if( false == m_MyApp.saveUserAccountToDatabase( m_MyApp.getUserAccount() ) )
    	{
    		Log.e(LOG_TAG, "updatePluginPermissions: ERROR updating database\n");
    	}
    	
    	NativeTxTo.fromGuiSetPluginPermission( m_ePluginType, m_iPermissionChoice );
    	
    	if( ( null != m_EditUserRelayCnt ) 
    		&& ( null != m_EditSystemRelayCnt ) )
    	{
    		String strUserCnt = m_EditUserRelayCnt.getText().toString();
    		m_UserRelayCnt = Integer.parseInt( strUserCnt );;
			if( m_UserRelayCnt < 0 )
			{
				m_UserRelayCnt = 0;
			}
			if( m_UserRelayCnt > 1000 )
			{
				m_UserRelayCnt = 1000;
			}
			
			String strSystemCnt = m_EditSystemRelayCnt.getText().toString();
    		m_SystemRelayCnt = Integer.parseInt( strSystemCnt );;
			if( m_SystemRelayCnt < 2 )
			{
				m_SystemRelayCnt = 2;
			}
			if( m_SystemRelayCnt > 1000 )
			{
				m_SystemRelayCnt = 1000;
			}

			m_MyApp.getMySettings().setSystemRelayPermissionCount( m_SystemRelayCnt );
			
			NativeTxTo.fromGuiSetRelaySettings( m_UserRelayCnt, m_SystemRelayCnt );
    	}
    }
  
    //========================================================================
	//@Override
	public void onCheckedChanged(RadioGroup arg0, int checkedId) 
	{
		if( R.id.permission_administrator == checkedId )
		{
			m_iPermissionChoice = Constants.eFriendStateAdministrator;
		} 
		else if( R.id.permission_friend == checkedId )
		{
			m_iPermissionChoice = Constants.eFriendStateFriend;
		}
		else if(  R.id.permission_guest == checkedId )
		{
			m_iPermissionChoice = Constants.eFriendStateGuest;
		}
		else if( R.id.permission_anonymous == checkedId )
		{
			m_iPermissionChoice = Constants.eFriendStateAnonymous;
		}
		else if( R.id.permission_disable == checkedId )
		{
			m_iPermissionChoice = Constants.eFriendStateIgnore;
		}
	}
	
    //========================================================================
	public void updatePermissionButtons()
	{
		switch( m_iPermissionChoice )
		{
		case Constants.eFriendStateAdministrator:
			m_oRadioGroup.check(R.id.permission_administrator);
			break;
		case Constants.eFriendStateFriend:
			m_oRadioGroup.check(R.id.permission_friend);
			break;

		case Constants.eFriendStateGuest:
			m_oRadioGroup.check(R.id.permission_guest);
			break;
			
		case Constants.eFriendStateIgnore:
			m_oRadioGroup.check(R.id.permission_disable);
			break;
			
		case Constants.eFriendStateAnonymous:
		default:
			m_oRadioGroup.check(R.id.permission_anonymous);
		}	
	}
}

