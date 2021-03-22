//============================================================================
// Copyright (C) 2013 Brett R. Jones
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

import java.util.ArrayList;

import android.content.Intent;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.util.Log;
import android.view.View;
import android.widget.ListView;

public class ActivityFriendListBase extends ActivityListBase implements ToGuiOfferInterface
{
	//=== constants ===//
	private static final String LOG_TAG = "ActivityFriendListBase:";

	//=== vars ===//
	private ArrayList<VxNetIdent> 	m_HisIdentList 			= null;
	public ContactsAdapter 			m_oContactsAdapter 		= null;
	private Handler 				m_oMessageHandler 		= null;
	public int 						m_iWhoToViewInList 		= 0;
	private ListView 				m_ContactListView 		= null;

	//========================================================================
	@Override
	public void onCreate(Bundle savedInstanceState) 
	{
 		super.onCreate(savedInstanceState);
        m_ContactListView 		= getListView();
		this.m_HisIdentList 	= new ArrayList<VxNetIdent>();		
		this.m_oContactsAdapter = new ContactsAdapter( this, R.layout.contact_item, m_HisIdentList );
		setListAdapter( this.m_oContactsAdapter );
 
		m_oMessageHandler = new Handler() 
		{
		    @Override
		    public void handleMessage(Message msg) 
		    {
		    	switch(msg.what)
		    	{
		    	case Constants.MSG_TYPE_UPDATE_FRIEND:
		    		//Log.i(LOG_TAG, "Rxed MSG_UPDATE_FRIEND");
		    		m_oContactsAdapter.doUpdateFriend( (VxNetIdent) msg.obj, false );
		    		m_oContactsAdapter.notifyDataSetChanged();
		    		break;
		    		
		    	case Constants.MSG_TYPE_UPDATE_CONTACT_SESSION_TIME:
		    		//Log.i(LOG_TAG, "Rxed MSG_UPDATE_FRIEND");
		    		m_oContactsAdapter.doUpdateFriend( (VxNetIdent) msg.obj, true );
		    		m_oContactsAdapter.notifyDataSetChanged();
		    		break;
		    		
		    	case Constants.MSG_TYPE_REFRESH_CONTACTS:
		    		//Log.i(LOG_TAG, "Rxed MSG_REFRESH_CONTACTS");
					m_oContactsAdapter.clearList();
					//Log.i(LOG_TAG, "RefreshContacts: notifyDataSetChanged ");
					m_oContactsAdapter.notifyDataSetChanged();
					Log.i(LOG_TAG, "RefreshContacts: fromGuiSendContactList " + m_iWhoToViewInList );
					NativeTxTo.fromGuiSendContactList( m_iWhoToViewInList );		    		
		    		break;

		    	case Constants.MSG_TYPE_CLEAR_CONTACTS:
		    		//Log.i(LOG_TAG, "Rxed MSG_TYPE_CLEAR_CONTACTS");
					m_oContactsAdapter.clearList();
					//Log.i(LOG_TAG, "RefreshContacts: notifyDataSetChanged ");
					m_oContactsAdapter.notifyDataSetChanged();
		    		break;

		    	}
		    	
		    	super.handleMessage(msg);
		    }
		};
	}
	
	//========================================================================
	@Override
	public void onDestroy() 
	{
    	if( null != m_MyApp.getCamCaptureMgr() )
    	{
    		m_oContactsAdapter.clearList();
    		m_oMessageHandler = null;
    	}
    	
 		super.onDestroy();
	}

	//========================================================================
	public void RefreshContacts() 
	{
		if( null != m_oMessageHandler )
		{
			Message msg; 
			msg = Message.obtain(); 
			msg.what = Constants.MSG_TYPE_REFRESH_CONTACTS;   
			m_oMessageHandler.sendMessage(msg); 
		}
		else
		{
			Log.e( LOG_TAG, "UpdateFriend with null message handler" );
		}
	}	  
    
	//========================================================================
	public void UpdateFriend( VxNetIdent oIdentIn, boolean sessionTimeChange ) 
	{
		if( null != m_oMessageHandler )
		{
			Message msg; 
			msg 		= Message.obtain(); 
			msg.what 	= sessionTimeChange ? Constants.MSG_TYPE_UPDATE_FRIEND : Constants.MSG_TYPE_UPDATE_CONTACT_SESSION_TIME;   
			msg.obj 	= oIdentIn;
			m_oMessageHandler.sendMessage(msg); 
		}
		else
		{
			Log.e( LOG_TAG, "UpdateFriend with null message handler" );
		}
	}
    
	//========================================================================
	public void ClearContactList() 
	{
		Message msg; 
		msg = Message.obtain();
		msg.what = Constants.MSG_TYPE_CLEAR_CONTACTS;   
		msg.obj = null;
		m_oMessageHandler.sendMessage(msg); 
	}

	//========================================================================
	@Override
	public void onListItemClick(ListView parent, View v, int position, long id) 
	{
		VxNetIdent friend = m_oContactsAdapter.getFriendByPosition(position);
		if( null != friend ) 
		{
			m_MyApp.playButtonClick() ;
			Log.i( LOG_TAG, "onListItemClick " + friend.m_as8OnlineName + friend.describeMyId() );
			m_MyApp.setCurrentFriend(friend);
			if( friend.isAccessAllowed( Constants.ePluginTypeMultiSession ) ) 
			{
				Intent myIntent = new Intent( this, AppletPeerMultiSession.class );
				m_MyApp.getOffersMgr().lockOffersMgr();
				GuiOfferSession oSession = m_MyApp.getOffersMgr().findAvailableAndActiveOffer( Constants.ePluginTypeMultiSession, friend );
				m_MyApp.getOffersMgr().unlockOffersMgr();
				if( null != oSession )
				{
					m_MyApp.setCurrentGuiOfferSession( oSession );
					myIntent.putExtra("IS_OFFER", true);	
				}
				
				myIntent.putExtra( "PluginType", Constants.ePluginTypeMultiSession );
				this.startActivityForResult(myIntent, Constants.ACTIVITY_TO_FRIEND_MULTISESSION);
				if( null != oSession )
				{
					m_MyApp.getOffersMgr().offerAcknowlegedByUser( oSession );
				}
			} 
			else 
			{
				showReasonAccessNotAllowed( friend, Constants.ePluginTypeMultiSession );
			}
		}
	}

	//========================================================================
	public void showReasonAccessNotAllowed( VxNetIdent netIdent, int ePluginType ) 
	{
		int accessState = netIdent.getMyAccessPermissionFromHim( ePluginType );
		String accessDesc = Constants.describePluginType( ePluginType );
		accessDesc += " with ";
		accessDesc += netIdent.getOnlineName();
		accessDesc +=  " ";

		if( Constants.ePluginAccessOk != accessState )
		{
			accessDesc +=  Constants.describePluginAccess( accessState );
			m_MyApp.toGuiUserMessage( accessDesc );
			return;
		}

		if( false == netIdent.isOnline() )
		{
			accessDesc +=   " requires user be online ";
			m_MyApp.toGuiUserMessage( accessDesc );
			return;
		}
	}
	
	//========================================================================
    public void onFriendMenuButClick( View v )
    {
    	Log.i( LOG_TAG, "onFriendMenuButClick" );
    	final int position = getPositionForItemView((View) v.getParent()); 
    	if( position >= 0 )
    	{
    		VxNetIdent friend = m_oContactsAdapter.getFriendByPosition( position );
    		if( null != friend ) 
    		{
    			m_MyApp.playButtonClick() ;
    			Log.i( LOG_TAG, "onFriendMenuButClick " + friend.m_as8OnlineName + friend.describeMyId() );
    			m_MyApp.setCurrentFriend( friend );
    			m_MyApp.ShowPopupMenu( Constants.MENU_FRIEND );
    		}
 		}
    } 
    
 	//========================================================================
     public int getPositionForItemView( View v )
     {
     	Log.i(LOG_TAG, "getPositionForItemView" );
     	for( int i = 0; i < m_ContactListView.getCount(); i++ ) 
     	{
     		View itemView = m_ContactListView.getChildAt(i);
     		if( itemView == v )
     		{
     			return i;
     		}
        }
     	
     	return -1;
     }

  	//========================================================================
	@Override
	public void doGuiUpdatePluginOffer( GuiOfferSession offerSession )
	{
		if( Constants.ePluginTypeMultiSession == offerSession.getPluginType() )
		{
			boolean haveSessionOffer = ( EOfferState.eOfferStateRxedOffer == offerSession.getOfferState() );
			m_oContactsAdapter.doGuiSetIdentHasTextOffers( offerSession.m_HisIdent, haveSessionOffer );	
		}
	}

 	//========================================================================
	@Override
	public void doGuiOfferRemoved( GuiOfferSession offerSession )
	{
		if( Constants.ePluginTypeMultiSession == offerSession.getPluginType() )
		{
			m_oContactsAdapter.doGuiSetIdentHasTextOffers( offerSession.m_HisIdent, false );	
		}
	}

 	//========================================================================
	@Override
	public void doGuiAllOffersRemoved()
	{
		m_oContactsAdapter.setAllToNoTextSessionOffers();	
	}
     
};
