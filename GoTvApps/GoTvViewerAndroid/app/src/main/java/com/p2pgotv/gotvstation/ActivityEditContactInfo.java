package com.p2pgotv.gotvstation;


import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.EditText;
import android.widget.ImageButton;
import android.widget.Toast;

import android.content.Intent;
import android.os.Bundle;

public class ActivityEditContactInfo extends AppletPeerGuiBase 
{
	//=== constants ===//
	public static final String LOG_TAG = "ActivityEditContactInfo:";
	
	//=== vars ===//
    Button						m_ImportFromContactsButton 			= null;
    EditText					m_ContactNameEdit					= null;
    EditText 					m_MobilePhoneNumEdit				= null;
    ImageButton 				m_PhotoImageButton 					= null;
	
    //======================================================================== 
    /** Called when the activity is first created. */
    @Override
    public void onCreate(Bundle savedInstanceState) 
    {
    	m_ePluginType =  Constants.ePluginTypeAdmin;
        setContentView(R.layout.activity_edit_contact_info);
        super.onCreate(savedInstanceState);
        setPageTitle( R.string.edit_contact_info );
        
        m_ImportFromContactsButton	= (Button) findViewById( R.id.import_from_phone_contacts_button );
	 	this.m_ImportFromContactsButton.setOnClickListener(new OnClickListener() 
	 	{
	 		public void onClick(View v)
	 		{
 			    m_MyApp.playButtonClick();
	 			onImportButClick();
	 		}
	 	});
	 	
        m_ContactNameEdit			= (EditText) findViewById( R.id.contact_name_edit );
        m_MobilePhoneNumEdit 		= (EditText) findViewById(R.id.mobile_phone_num_edit);
        m_PhotoImageButton 			= (ImageButton) findViewById( R.id.phone_book_photo_image );
	 	this.m_PhotoImageButton.setOnClickListener(new OnClickListener() 
	 	{
	 		public void onClick(View v)
	 		{
 			    m_MyApp.playButtonClick();
	 			onPhotoImageButClick();
	 		}
	 	});        
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
		Intent oThisIntent = getIntent();
		// return result of edit activity back to MainActivity
		setResult(RESULT_CANCELED, oThisIntent);
		finish();
    }
       
    //========================================================================
    public void OnOkButClick(View v) 
    { 
		Intent oThisIntent = getIntent();
		// return result of edit activity back to MainActivity
		setResult(RESULT_OK, oThisIntent);
		finish();
    }
    
    //========================================================================
    public void onImportButClick() 
    { 

    }
    
    //========================================================================
    public void onPhotoImageButClick() 
    { 

    }   
    
};


