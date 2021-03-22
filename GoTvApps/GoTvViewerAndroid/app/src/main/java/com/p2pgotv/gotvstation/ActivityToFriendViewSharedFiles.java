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

import java.util.ArrayList;

import android.app.Activity;
import android.app.AlertDialog;
import android.content.DialogInterface;
import android.content.Intent;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.view.WindowManager;
import android.widget.LinearLayout;
import android.widget.ListView;
import android.widget.Toast;

public class AppletPeerViewSharedFiles  extends ActivityListBase implements ToGuiFileXferInterface
{
	public static final String LOG_TAG = "AppletPeerViewSharedFiles:";
	   
	//=== vars ===//
	private ReuseFriendDisplayLogic m_FriendDisplayLogic 	= new ReuseFriendDisplayLogic();
	private VxNetIdent			m_HisIdent					= null;
	private VxGUID				m_LclSessionId				= new VxGUID();
	private boolean				m_OfferSent					= false;
	
	ArrayList<GuiFileXferSession> 	m_FileXferList 			= new  ArrayList<GuiFileXferSession>();
	private FileXferAdapter		m_FileXferAdapter			= null;
	private ListView			m_FileListView				= null;
	private int					m_ePluginType 				= Constants.ePluginTypeFileServer;
	
    //========================================================================
    @Override
    public void onCreate(Bundle savedInstanceState) 
    {
 		m_MyApp 							= (MyApp) this.getApplication();
        m_HisIdent 							= m_MyApp.getCurrentFriend();
		m_ePluginType 						= Constants.ePluginTypeFileServer;
		
        setContentView( R.layout.activity_to_friend_view_shared_files );
        super.onCreate(savedInstanceState);
        setPageTitle( R.string.activity_to_friend_view_shared_files_title );
        // keep keyboard from popping up
        this.getWindow().setSoftInputMode(WindowManager.LayoutParams.SOFT_INPUT_STATE_ALWAYS_HIDDEN); 
        m_LclSessionId.initializeWithNewGUID();
        m_FileXferAdapter = new FileXferAdapter( this, R.layout.file_xfer_item, m_FileXferList );
    	setListAdapter( m_FileXferAdapter );
    	m_FileListView = getListView();
  
        m_HisIdent = m_MyApp.getCurrentFriend();
        setActivityReadyState( this );
	 	if( null != m_HisIdent )
	 	{
	 		m_FriendDisplayLogic.fillFriendDisplayGui( this, m_HisIdent ); 		
			if (false == NativeTxTo.fromGuiMakePluginOffer(	Constants.ePluginTypeFileServer, 
											        		m_HisIdent.m_u64IdHiPart,
											        		m_HisIdent.m_u64IdLoPart, 
											        		0, 
											        		"View Files", 
											        		"",
											        		m_LclSessionId.m_u64IdHiPart,
											        		m_LclSessionId.m_u64IdLoPart ) )
			{
				showOfflineMsg();
			}
			else
			{
				m_OfferSent = true;
			}
	 	}
	 	
	 	if( m_OfferSent )
	 	{
	 		setStatusMessage( "Waiting for File Offer Response" );	 		
	 	} 
	 	else
	 	{
	 		setStatusMessage( "User Is Offline" );
	 	}
    }

	//========================================================================
    @Override
    protected void onPause() 
    {
        setActivityReadyState( null );
     	super.onPause();    	
    }

	//========================================================================
    @Override
    protected void onResume() 
    {
        setActivityReadyState( this );
    	super.onResume();    	
    }

	//========================================================================
    @Override
    protected void onDestroy() 
    {
        setActivityReadyState( null );
     	super.onDestroy();    	
    }
    
 	//========================================================================
    protected void setActivityReadyState( Activity curActivity )
    {
		try 
		{
			if( null == curActivity )
			{
				NativeRxFrom.wantToGuiFileXferClient( this, false );
			}
			else
			{
				NativeRxFrom.wantToGuiFileXferClient( this, true );
			}
		} 
		catch (InterruptedException e) 
		{
			e.printStackTrace();
		}
    }
	
	//========================================================================
	void showOfflineMsg()
	{
        Toast.makeText(AppletPeerViewSharedFiles.this, m_HisIdent.m_as8OnlineName + " Is Offline.", Toast.LENGTH_SHORT).show();		
	}

	//========================================================================
	@Override
	public void toGuiFileList(VxFileInfo fileInfo) 
	{
	}

	//========================================================================
	@Override
	public void toGuiFileListReply(GuiFileXferSession xferSession) 
	{
		if( xferSession.getFullFileName().isEmpty() )
		{
			// end of file list
			setStatusMessage( "File List Complete" );
			return;
		}
		
		if( Constants.eXferStateUnknown == xferSession.getXferState() )
		{
			xferSession.setXferState(  Constants.eXferStateDownloadNotStarted, 0, 0 );
		}
		
		m_FileXferAdapter.lockFileXferAdapter();
		m_FileXferList.add( xferSession );
		m_FileXferAdapter.unlockFileXferAdapter();
		forceListUpdate();
	}
	
	//============================================================================
	private void forceListUpdate()
	{
		try 
		{
			runOnUiThread(new Runnable() 
			{	
				@Override
				public void run() 
				{
					m_FileXferAdapter.notifyDataSetChanged();
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
	public void toGuiStartUpload(GuiFileXferSession xferSession) 
	{
	}

	//========================================================================
	@Override
	public void toGuiStartDownload(GuiFileXferSession xferSession) 
	{
		m_FileXferAdapter.toGuiFileXferState( xferSession.getLclSessionId(), Constants.eXferStateBeginDownload, 0, 0 );
		forceListUpdate();
	}

	//========================================================================
	@Override
	public void toGuiFileXferState(VxGUID lclSession, int eXferState, int param1, int param2) 
	{
		m_FileXferAdapter.toGuiFileXferState( lclSession, eXferState, param1, param2 );
		forceListUpdate();
	}

	//========================================================================
	@Override
	public void toGuiFileDownloadComplete(VxGUID lclSession, String newFileName, int xferError) 
	{
		m_FileXferAdapter.toGuiFileDownloadComplete( lclSession, newFileName, xferError );
		forceListUpdate();
	}

	//========================================================================
	@Override
	public void toGuiFileUploadComplete(VxGUID lclSession, int xferError) 
	{
	}
	   
	//========================================================================
	public VxFileInfo getFileInfoForItemView( View v )
	{
		// Assumes lock has been done
	   	for( int i = 0; i < m_FileListView.getCount(); i++ ) 
    	{
    		View itemView = m_FileListView.getChildAt(i);
    		if( itemView == v )
    		{
    			return m_FileXferAdapter.getFileByPosition( i );
    		}
        }

		return null;
	}
	
	//========================================================================
	public void onFileXferFileIconButClick( View v )
	{
		m_MyApp.playButtonClick() ;
		int position = m_FileListView.getPositionForView( (View) v.getParent() );
		if( position >= 0 )
		{
			m_FileXferAdapter.lockFileXferAdapter();
			GuiFileXferSession fileInfo = m_FileXferAdapter.getFileByPosition( position );
			m_FileXferAdapter.unlockFileXferAdapter();
			if( null != fileInfo )
			{
				if( fileInfo.isDirectory() )
				{
				}
				else
				{
					if( fileInfo.getIsCompleted() )
					{
						playFile( this, fileInfo );
					}
					else
					{
						Toast.makeText( this, R.string.play_file_requires_completed_download, Toast.LENGTH_SHORT ).show();
					}
				}
			}
		}
	}
	
	//========================================================================
	public void onFileXferAcceptButClick( View v )
	{
    	Log.i(LOG_TAG, "onFileXferAcceptButClick" );
		m_MyApp.playButtonClick() ;
		int position = m_FileListView.getPositionForView( (View) v.getParent() );
		if( position >= 0 )
		{
			m_FileXferAdapter.lockFileXferAdapter();
			GuiFileXferSession xferSession = m_FileXferAdapter.getFileByPosition( position );
			if( null != xferSession )
			{
				if( xferSession.isDirectory() )
				{
				}
				else
				{
					switch( xferSession.getXferState() )
					{
					case Constants.eXferStateUploadNotStarted:
						beginUpload( xferSession, position );
						break;
						
					case Constants.eXferStateDownloadNotStarted:
						beginDownload( xferSession, position );
						break;

					case Constants.eXferStateCompletedUpload:
					case Constants.eXferStateCompletedDownload:
						removeDownload( xferSession, position );
						break;
					case Constants.eXferStateWaitingOfferResponse:
					case Constants.eXferStateInUploadQue:
					case Constants.eXferStateInDownloadQue:
					case Constants.eXferStateBeginUpload:
					case Constants.eXferStateBeginDownload:
					case Constants.eXferStateInUploadXfer:
					case Constants.eXferStateInDownloadXfer:
					case Constants.eXferStateUserCanceledUpload:
					case Constants.eXferStateUserCanceledDownload:
					case Constants.eXferStateUploadOfferRejected:
					case Constants.eXferStateUploadError:
					case Constants.eXferStateDownloadError:
						cancelDownload( xferSession, position );					
						break;
						
					case Constants.eXferStateUnknown:						
					default:
						break;
					}				
				}
			}
			
			m_FileXferAdapter.unlockFileXferAdapter();
		}
	}
	
	//========================================================================
	public void beginUpload( GuiFileXferSession xferSession, int position )
	{
		beginDownload( xferSession, position );
	}
	
	//========================================================================
	public void beginDownload( GuiFileXferSession xferSession, int position )
	{
		if(	-1 != NativeTxTo.fromGuiGetFileDownloadState( xferSession.getFileHashId().getHashData() ) )
		{
		    Toast.makeText(AppletPeerViewSharedFiles.this, "File is already downloading", Toast.LENGTH_SHORT).show();		
			return;
		}
		else
		{
			if( false == xferSession.getLclSessionId().isOnlineIdValid() )
			{
				xferSession.getLclSessionId().initializeWithNewGUID();
			}

			int xferError = NativeTxTo.fromGuiPluginControl( 	Constants.ePluginTypeFileServer, 
																m_HisIdent.getMyOnlineId().m_u64IdHiPart, 
																m_HisIdent.getMyOnlineId().m_u64IdLoPart, 
																"DownloadFile", 
																xferSession.getFullFileName(),
																0,
																xferSession.getLclSessionId().m_u64IdHiPart,
																xferSession.getLclSessionId().m_u64IdLoPart,
																xferSession.getFileHashId().getHashData() );
			if( Constants.eXferErrorNone == xferError )
			{
				// make copy.. downloads will delete
				//GuiFileXferSession * poNewInfo = new GuiFileXferSession();
				//*poNewInfo = *m_SelectedFileInfo;
				//m_MyApp.getActivityDownloads()->slotUpdateDownload( poNewInfo );
			}
			else 
			{
				String xferErr = GuiHelpers.describeEXferError( xferError );
			    Toast.makeText(AppletPeerViewSharedFiles.this, xferErr, Toast.LENGTH_LONG).show();		
				return;
			}
		}	
	}
	
	//========================================================================
	public void removeDownload( GuiFileXferSession xferSession, int position )
	{
		m_FileXferList.remove( position );
		forceListUpdate();
	}
	
	//========================================================================
	public void cancelDownload( GuiFileXferSession xferSession, int position )
	{
		xferSession.setXferState( Constants.eXferStateUserCanceledDownload, 0, 0 );
		NativeTxTo.fromGuiCancelDownload( xferSession.getLclSessionId().m_u64IdHiPart, xferSession.getLclSessionId().m_u64IdLoPart );
		removeDownload( xferSession, position );		
	}
	
	//========================================================================
	public VxFileInfo getActionButtonFileInfo( View v )
	{
		LinearLayout buttonCenterLayout = (LinearLayout) v.getParent();
		if( null != buttonCenterLayout )
		{
			LinearLayout actionButtonsLayout = (LinearLayout)buttonCenterLayout.getParent();
			if( null != actionButtonsLayout )
			{
				LinearLayout vertLayout = (LinearLayout)actionButtonsLayout.getParent();
				if( null != actionButtonsLayout )
				{
					int position = m_FileListView.getPositionForView( (View) vertLayout.getParent() );
					if( position >= 0 )
					{
						m_FileXferAdapter.lockFileXferAdapter();
						GuiFileXferSession fileInfo = m_FileXferAdapter.getFileByPosition( position );
						m_FileXferAdapter.unlockFileXferAdapter();
						return fileInfo;
					}
				}
			}	
		}
		
		return null;
	}

	//========================================================================
	public void onFileXferLibraryButClick( View v )
	{
    	Log.i(LOG_TAG, "onFileXferLibraryButClick" );
		m_MyApp.playButtonClick() ;
		VxFileInfo fileInfo = getActionButtonFileInfo( v );
		if( null != fileInfo )
		{
			if( Constants.VXFILE_TYPE_DIRECTORY == fileInfo.getFileType() )
			{
				//browseToDirectory( fileInfo );
			}
			else
			{
				// is file
				// toggle is in library state
				boolean isInLib = !fileInfo.getIsInLibrary();
				fileInfo.setIsInLibrary( isInLib );
				NativeTxTo.fromGuiAddFileToLibrary(	fileInfo.getFullFileName(), 
													fileInfo.getIsInLibrary(),
													fileInfo.getFileHashId().getHashData() );
				m_FileXferAdapter.notifyDataSetChanged();
			}
		}
	}
	
	//========================================================================
	public void onFileXferShareButClick( View v )
	{
		Log.i(LOG_TAG, "onFileXferLibraryButClick" );
		m_MyApp.playButtonClick() ;
		VxFileInfo fileInfo = getActionButtonFileInfo( v );
		if( null != fileInfo )
		{
			if( Constants.VXFILE_TYPE_DIRECTORY == fileInfo.getFileType() )
			{
				//browseToDirectory( fileInfo );
			}
			else
			{
				// is file
				// toggle is in library state
				boolean isShared = !fileInfo.getIsShared();
				fileInfo.setIsShared( isShared );
				NativeTxTo.fromGuiSetFileIsShared( fileInfo.getFullFileName(), fileInfo.getIsShared(), fileInfo.getFileHashData() );
				m_FileXferAdapter.notifyDataSetChanged();
			}
		}	
	}
	
	//========================================================================
	public void onFileXferPlayButClick( View v )
	{
		Log.i(LOG_TAG, "onFileXferPlayButClick" );
		m_MyApp.playButtonClick() ;
		VxFileInfo fileInfo = getActionButtonFileInfo( v );
		if( null != fileInfo )
		{
			if( Constants.VXFILE_TYPE_DIRECTORY == fileInfo.getFileType() )
			{
				//browseToDirectory( fileInfo );
			}
			else
			{
				playFile( this, fileInfo );
			}	
		}
	}
	
	//========================================================================
	public void onFileXferShredButClick( View v )
	{
		Log.i(LOG_TAG, "onFileXferShredButClick" );
		m_MyApp.playButtonClick() ;
		final VxFileInfo fileInfo = getActionButtonFileInfo( v );
		final int position = m_FileListView.getPositionForView( (View) v.getParent() );
		final Activity thisActivity = this;
		if( position >= 0 )
		{
			if( null != fileInfo )
			{
				if( Constants.VXFILE_TYPE_DIRECTORY == fileInfo.getFileType() )
				{
					//browseToDirectory( fileInfo );
				}
				else
				{
					new AlertDialog.Builder( this )	 
				 	.setIcon(R.drawable.button_shredder_normal)	 
				 	.setTitle("Delete File Confirm")	 
				 	.setMessage("Press SHRED to write random data to file then delete file.\n Press REMOVE to just remove file from list")	 
				 	.setPositiveButton("SHRED", new DialogInterface.OnClickListener()
					 {
					 	@Override
					 	public void onClick(DialogInterface dialog, int which) 
					 	{
					 		m_MyApp.playSound( ESndDef.eSndDefPaperShredder );
							Log.d( LOG_TAG, "onShredMediaButClick Shredding file " + fileInfo.getFullFileName() );
							NativeTxTo.fromGuiSetFileIsShared( fileInfo.getFullFileName(), false, fileInfo.getFileHashData() );
							NativeTxTo.fromGuiAddFileToLibrary( fileInfo.getFullFileName(), false, fileInfo.getFileHashData() );
							m_FileXferList.remove( position );
							forceListUpdate();
							NativeTxTo.fromGuiDeleteFile( fileInfo.getFullFileName(), true );
					 	}
					 })
					 .setNegativeButton("Cancel", new DialogInterface.OnClickListener()
					 {
						 @Override
						 public void onClick(DialogInterface dialog, int which) 
						 {
							 m_MyApp.playButtonClick();
							 Toast.makeText( thisActivity, "Operation Canceled", Toast.LENGTH_SHORT).show();
						 }
					 })
					 .setNeutralButton("REMOVE", new DialogInterface.OnClickListener()
					 {
						 @Override
						 public void onClick(DialogInterface dialog, int which) 
						 {
							 Log.d( LOG_TAG, "onShredMediaButClick Removing file " + fileInfo.getFullFileName() );
							 m_MyApp.playSound( ESndDef.eSndDefNeckSnap );
							 m_FileXferList.remove( position );
							 forceListUpdate();
						 }
					 })
					 .show();
				}	
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
		case Constants.ACTIVITY_CONFIRM_SHRED_FILE:
			Log.d( LOG_TAG, "onActivityResult ACTIVITY_CONFIRM_SHRED_FILE" );
			if( resultCode == Activity.RESULT_OK ) 
			{
				/*
				VxFileInfo fileInfo = m_MyApp.getCurrentFileInfo();
				if( null != fileInfo )
				{
					m_MyApp.playSound( ESndDef.eSndDefPaperShredder );
					String fullFileName = fileInfo.getFullFileName();
					m_FileXferAdapter.lockFileXferAdapter();
					m_FileXferAdapter.removeFile( fileInfo );
					m_FileXferAdapter.unlockFileXferAdapter();
					m_FileXferAdapter.notifyDataSetChanged();
					NativeTxTo.fromGuiDeleteFile( fullFileName, true );
				}	*/			
			}
			
			break;
		}
		
		setCamCaptureActivity( this );
	}
}
