package com.p2pgotv.gotvstation;

import java.io.IOException;
import java.util.ArrayList;

import android.app.Application;
import android.content.Context;
import android.content.Intent;
import android.content.SharedPreferences;
import android.content.SharedPreferences.Editor;
import android.content.res.AssetManager;
import android.content.res.Resources;
import android.preference.PreferenceManager;
import android.util.Log;
import android.view.View;

public class MyApp extends  Application
{
	public static final String 			LOG_TAG 						= "MyApp";

	// === vars ===//
	private boolean						m_AppIsPaused 					= false;
	private int							m_ActivityPausedCounter			= 0;
	private boolean						m_PauseEventNeedsUpdate 		= false;
	private int							m_PauseUpdateCountdown			= 5;
	
	private boolean 					m_bIsShuttingDown 				= false;
	
	Resources 							m_oRes 							= null;

	private VxFileUtil 					m_VxFileUtil					= null;
	private VxImageUtil 				m_oVxImageUtil					= null;
	private VxMiscUtil 					m_oVxMiscUtil					= null;
	private VxSktUtil 					m_oVxSktUtil					= null;

	private String 						m_strRootStorageCardDir; // may be internal or external
	private String 						m_strAppRootStorageDir;
	
	private String 						m_strAppAssetsDir;
	private String 						m_strAppUsersDir;
	private String 						m_strAppAccountsDir;
	
	private String 						m_strAppUserXferDir;
	private String 						m_strAppUserSpecificDir;
	private String 						m_strAppUserProfileDir;
	
	private MgrSound 					m_oMgrSnd 						= null;
	
	//private MgrVideo 					m_oMgrVideo 					= null;
	//private MgrVibrator 				m_oMgrVibrator 					= null;
	//private MgrLocation 				m_oMgrLocation 					= null;
	
	private MgrNetwork 					m_oMgrNetwork 					= null;
	private CamCaptureMgr 				m_oMgrCamCapture 				= null;
	private OffersMgr					m_OffersMgr						= null;

	private NativeRxFrom 				m_oNativeRxFrom;
	private LogHandler 					m_LogHandler;

	public int 							m_iActivePopupMenu 				= -1;
	public String 						m_strPopupMenuTitle 			= "unknown";
	public VxNetIdent 					m_oCurrentFriend 				= null;
	public GuiOfferSession 				m_oGuiOfferSession 				= null;
	public VxFileInfo					m_CurrentFileInfo 				= null;

	private DataHelper 					m_oDataHelper 					= null;
	private MySettings 					m_oSettingsDb 					= null;
	private UserAccount 				m_oUserAccount 					= null;
	private int 						m_iListenPort;

	private boolean 					m_bAppIdleTimerStarted 			= false;
	private boolean 					m_KeepIdleThreadRunining 		= false;
	public long 						m_s64SysTimeMs 					= 0; // system time in milliseconds updated 20 ms
									
	public long 						m_s64SysTimeSec 				= 0;
	private Thread 						m_oIdleThread;

	Context								m_ApplicationContext			= null;
	MyP2PWeb 							m_oMyP2PWeb 					= null;
	MyP2PWebState 						m_MyP2PWebState 				= null;
	ActivityTestNetwork 				m_oActTestNetwork 				= null;
	public ActivityTestAnchorSettings 	m_oActTestAnchorSettings 		= null;
	public ActivityTestIsMyPortOpen 	m_oActTestIsMyPortOpen 			= null;

	public ActivityPersonalRecorder 	m_ActivityPersonalRecorder 		= null;
	ActivityPersonalRecorderState		m_ActivityPersonalRecorderState = null;
	AppletPeerMultiSessionState	m_ActivityMultiSessionState 	= null;

	public boolean 						m_bRelayRequired 				= true;
	ArrayList<GuiFileXferSession> 		m_UploadsList 					= new  ArrayList<GuiFileXferSession>();
	public FileXferAdapter 				m_UploadsAdapter 				= null;
	ArrayList<GuiFileXferSession> 		m_DownloadsList 				= new  ArrayList<GuiFileXferSession>();
	public FileXferAdapter 				m_DownloadsAdapter 				= null;
	private boolean						m_IsMyResourcesInitialize		= false;
	private VxGUID						m_NullGuidId					= new VxGUID();
	public boolean 						m_IsMainCreateCalledOnce		= false;
	public boolean 						m_bNetworkMgrStarted 			= false;
	public boolean 						m_IsCpuMonitorEnabled 			= false;
	public boolean 						m_IsMyAppInitialized			= false;
	private int 						m_eNetworkState					= 0;
	
	static int m_TempButtonClickCnt = 0;

	//========================================================================
	@Override
	public void onCreate() 
	{
		super.onCreate();
		Log.d(Constants.LIFETIME, "&&& MyApp onCreate start" );
		// BRJ somehow cam capture mgr startupCamCaptureMgr is sometimes called before initialized so move create to here
		if( null == m_oMgrSnd )
		{
			this.m_oMgrSnd					= new MgrSound(this);
		}
		
		if( null == m_oMgrCamCapture )
		{	
			this.m_oMgrCamCapture			= new CamCaptureMgr(this);
		}
		
		//FontManager.preLoadFonts(this);
		System.loadLibrary( "MyP2PWebLib" );

		// create all objects
		this.m_LogHandler 				= new LogHandler();
		this.m_VxFileUtil 				= new VxFileUtil();
		this.m_oVxImageUtil 			= new VxImageUtil();
		this.m_oVxMiscUtil 				= new VxMiscUtil();
		this.m_oVxSktUtil 				= new VxSktUtil();
		
		this.m_oDataHelper 				= new DataHelper(this);
		this.m_oSettingsDb 				= new MySettings(this);
		this.m_oUserAccount 			= new UserAccount();
		
		// managers
		m_OffersMgr				= new OffersMgr(this);
		
		// leave here.. may want in future
		//this.m_oMgrVideo				= new MgrVideo(this);
		//this.m_oMgrVibrator			= new MgrVibrator(this);
		//this.m_oMgrLocation			= new MgrLocation(this);

		// native interface
		m_oNativeRxFrom			= new NativeRxFrom(this);

		m_UploadsAdapter			= new FileXferAdapter( this, R.layout.file_xfer_item, m_UploadsList );
		m_DownloadsAdapter			= new FileXferAdapter( this, R.layout.file_xfer_item, m_DownloadsList );
		m_IsMyAppInitialized = true;
		Log.d(Constants.LIFETIME, "&&& MyApp onCreate done" );
	}

	//========================================================================
	@Override
	public void onTerminate() 
	{
		// NOTE: this is never called except by emulator
		Log.d(Constants.LIFETIME, "&&& MyApp onTerminate" );
		m_bIsShuttingDown = true;
		m_IsMyAppInitialized = false; 
		super.onTerminate();
	}
	
	//========================================================================
	//public void 				setIsAppPaused( boolean isPaused ) 				{ m_AppIsPaused = isPaused; }	
	public boolean 				getIsAppPaused() 								{ return m_AppIsPaused; }	
	public boolean 				getIsCpuMonitorEnabled() 						{ return m_IsCpuMonitorEnabled; }	
	
	public void 				setIsAppShuttingDown( boolean isShuttingDown ) 	
	{ 
		m_bIsShuttingDown = isShuttingDown; 
		//if( m_bIsShuttingDown )
		//{
		//	NativeRxFrom.toGuiShutdownHardware(); // BRJ causes crash
		//}
	}
	
	public boolean 				getIsAppShuttingDown() 							{ return m_bIsShuttingDown; }	
	
	public void 				setCurrentFriend( VxNetIdent oFriend ) 			{ m_oCurrentFriend = oFriend; }
	public VxNetIdent 			getCurrentFriend() 								{ return m_oCurrentFriend; }
	public void 				setCurrentGuiOfferSession( GuiOfferSession session ) 	{ m_oGuiOfferSession = session; }
	public GuiOfferSession 		getCurrentGuiOfferSession() 					{ return m_oGuiOfferSession; }
	public void 				setCurrentFileInfo( VxFileInfo fileInfo ) 		{ m_CurrentFileInfo = fileInfo; }
	public VxFileInfo 			getCurrentFileInfo() 							{ return m_CurrentFileInfo; }
	
	public String 				getNoSpaceAppName() 							{ return NativeTxTo.fromGuiGetAppNameNoSpaces(); }
	public MyP2PWebState 		getMyP2PWebState()								{ return m_MyP2PWebState; }
	public LogHandler 			getLogHandler() 								{ return m_LogHandler; }

	public MyP2PWeb 			getMainActivity() 								{ return this.m_oMyP2PWeb; }
	public Resources 			getMyResources()  								{ return this.m_oRes; }
	public VxFileUtil 			getVxFileUtil()  								{ return this.m_VxFileUtil; }
	public VxSktUtil 			getVxSktUtil() 									{ return this.m_oVxSktUtil; }
	public VxImageUtil 			getVxImageUtil() 								{ return this.m_oVxImageUtil; }
	public VxMiscUtil 			getVxMiscUtil() 								{ return this.m_oVxMiscUtil; }
	public DataHelper 			getDataHelper() 								{ return this.m_oDataHelper; }
	public void 				setUserAccount( UserAccount oAccount ) 			{ this.m_oUserAccount = oAccount; }
	public UserAccount 			getUserAccount() 								{ return this.m_oUserAccount; }
	public boolean				saveUserAccountToDatabase( UserAccount oAccount){ return getDataHelper().updateAccount(oAccount ); }
	public FileXferAdapter 		getUploadsAdapter() 							{ return this.m_UploadsAdapter; }
	public FileXferAdapter 		getDownloadsAdapter() 							{ return this.m_DownloadsAdapter; }
	public int 					getListenPort() 								{ return this.m_iListenPort; }
	public MySettings 			getMySettings() 								{ return this.m_oSettingsDb; }
	
	public MgrSound 			getMgrSound()  									{ return this.m_oMgrSnd; }
	//public MgrVideo 			getMgrVideo()  									{ return this.m_oMgrVideo; }
	//public MgrVibrator 			getMgrVibrator() 								{ return this.m_oMgrVibrator; }
	//public MgrLocation 			getMgrLocation() 								{ return this.m_oMgrLocation; }
	public MgrNetwork 			getMgrNetwork() 								{ return this.m_oMgrNetwork; }
	public CamCaptureMgr 		getCamCaptureMgr() 								{ return this.m_oMgrCamCapture; }
	public OffersMgr 			getOffersMgr() 									{ return this.m_OffersMgr; }
	
	public NativeRxFrom 		getNativeRxFrom()  								{ return this.m_oNativeRxFrom; }
	public int 					getActiveMenu() 								{ return this.m_iActivePopupMenu; }
	
	public String 				getAppRootStorageDir() 							{ return this.m_strAppRootStorageDir; }
	public String 				getUserXferDir() 								{ return this.m_strAppUserXferDir; }
	public String 				getUserSpecificDir() 							{ return this.m_strAppUserSpecificDir; }
	public String 				getUserProfileDir()  							{ return this.m_strAppUserProfileDir; }	
	public String 				getAppAssetsDir() 								{ return this.m_strAppAssetsDir; }
	public String 				getAppUsersDir() 								{ return this.m_strAppUsersDir; }
	public String 				getAppAccountsDir()  							{ return this.m_strAppAccountsDir; }	
	public void 				setNetworkState( int eNetworkState )			{ m_eNetworkState = eNetworkState; }
	public int 					getNetworkState()								{ return m_eNetworkState; }
	
	// directory structure on disk
	// /storage/MyP2PWeb/ 					root application data storage
	//					/data/				ShredFilesDb.db3 and application generated files
	//                  /temp/				temporary files directory
	//                  /assets/gui/ 		gui assets
	//                         /shaders/ 	opengl shaders
	//                         /profile/ 	profile default files
	// user specific directories
	//                  /accounts/userName/settings/ 	databases
	//                  /accounts/userName/profile/ 	profile and story board user web pages
	// user xfer directories         
	//                  /users/userName/downloads/
	//                                 /uploads/
	//                                 /incomplete/
	//                                 /me/ 			personal recordings
	//                                 /contacts/ 		contact assets

	//========================================================================
	public String getPersonalRecordDir() 							
	{ 
		String personalDir = m_strAppUserXferDir;
		personalDir += "me/";
		VxFileUtil.createDirectory( personalDir );
		return personalDir;
	}

	//========================================================================
	public String getTemporaryDir() 							
	{ 
		String tempDir = m_strAppRootStorageDir;
		tempDir += "temp/";
		VxFileUtil.createDirectory( tempDir );
		return tempDir;
	}

	//========================================================================
	public VxGUID getMyOnlineId() 
	{
		if( null == m_oUserAccount )
		{
			Log.d( LOG_TAG, "getMyOnlineId NULL  m_oUserAccount" );
			return m_NullGuidId;
		}
		
		return m_oUserAccount.getMyOnlineId();
	}
	
	//========================================================================
	public VxNetIdent getMyIdentity() 
	{
		if( null == m_oUserAccount )
		{
			Log.d( LOG_TAG, "getMyIdentity NULL  m_oUserAccount" );
			return null;
		}
		
		return m_oUserAccount;
	}
	
	//========================================================================
	public boolean isWebCamServerRunning()
	{
		return NativeTxTo.fromGuiIsPluginInSession( Constants.ePluginTypeCamServer, 
				m_oUserAccount.m_u64IdHiPart, 
				m_oUserAccount.m_u64IdLoPart, 
				0, 0, 0 );
	}

	//========================================================================
	public void toGuiUpdateMyIdent( VxNetIdent oIdent )
	{
		LogHandler.logMsg( LogHandler.LOG_DEBUG, "toGuiUpdateMyIdent"); 
		m_oUserAccount.m_as8OnlineIPv4 			= oIdent.m_as8LocalIPv4;
		m_oUserAccount.m_OnlinePort 			= oIdent.m_OnlinePort;
		m_oUserAccount.m_u8OnlineStatusFlags 	= oIdent.m_u8OnlineStatusFlags;
		m_oUserAccount.m_u8SearchFlags 			= oIdent.m_u8SearchFlags;
		m_oUserAccount.m_u8FileTypeFlags 		= oIdent.m_u8FileTypeFlags;

		m_oUserAccount.m_as8RelayIPv4  			= oIdent.m_as8RelayIPv4;
		m_oUserAccount.m_RelayPort 				= oIdent.m_RelayPort;
		m_oUserAccount.m_u64RelayIdHiPart 		= oIdent.m_u64RelayIdHiPart;
		m_oUserAccount.m_u64RelayIdLoPart 		= oIdent.m_u64RelayIdLoPart;

		m_oUserAccount.m_u8RelayFlags 			= oIdent.m_u8RelayFlags;
		
		saveUserAccountToDatabase( m_oUserAccount );
	}
	
	//========================================================================
	public ActivityPersonalRecorderState getPersonalRecorderState()
	{
		if( null == m_ActivityPersonalRecorderState )
		{
			m_ActivityPersonalRecorderState = new ActivityPersonalRecorderState( this );
		}
		
		return m_ActivityPersonalRecorderState;
	}
	
	//========================================================================
	public AppletPeerMultiSessionState getMultiSessionState()
	{
		if( null == m_ActivityMultiSessionState )
		{
			m_ActivityMultiSessionState = new AppletPeerMultiSessionState( this );
		}
		
		return m_ActivityMultiSessionState;
	}

	//========================================================================
	public int myAppInitialize( Context context, Resources oRes ) 
	{
		Log.d( Constants.LIFETIME, "&&&&&&&&& APPLICATION myAppInitialize" );
		int iResult = 0;		
		if( ( false == m_IsMyResourcesInitialize ) || ( null == m_oRes ) )
		{
			m_oRes = oRes;
			m_VxFileUtil.Startup(oRes);			
			if( m_VxFileUtil.isExtenalStorageWritable() )
			{
				m_strRootStorageCardDir = m_VxFileUtil.getSdCardDir();		
			}
			else
			{
				m_strRootStorageCardDir = context.getFilesDir().getParentFile().getPath();	
			}
			
			// paths to external directories
			m_strAppRootStorageDir 	= m_strRootStorageCardDir + "/" + getNoSpaceAppName() + "/";
			
			m_strAppAssetsDir 		= m_strAppRootStorageDir + "assets/";
			m_strAppUsersDir 		= m_strAppRootStorageDir + "users/";
			m_strAppAccountsDir 	= m_strAppRootStorageDir + "accounts/";
			
			m_strAppUserSpecificDir	= m_strAppAccountsDir;
			m_strAppUserXferDir		= m_strAppUsersDir;
	
			VxFileUtil.createDirectory( m_strAppRootStorageDir );
			VxFileUtil.createDirectory( m_strAppAssetsDir );		
			VxFileUtil.createDirectory( m_strAppUsersDir );
			VxFileUtil.createDirectory( m_strAppAccountsDir );
		}

		return iResult;
	}
	
	//========================================================================
	private void doOnApplicationPaused( boolean isPausedState )
	{
		m_AppIsPaused = isPausedState;
		Log.d( Constants.LIFETIME, "######&&& doOnApplicationPaused " + isPausedState );
		//NativeTxTo.fromGuiAppPauseOrResume( m_AppIsPaused ); // BRJ causes deadlock in some instances
	}
	
	//========================================================================
	public String removeRootStorageDir( String fileOrDir )
	{
		String resultString = fileOrDir;
		int rootLen = m_strRootStorageCardDir.length();
		if( ( rootLen < fileOrDir.length() ) && ( rootLen > 0 ) )
		{
			String compDir = fileOrDir.substring( 0, rootLen );
			if( compDir.matches( m_strRootStorageCardDir ) )
			{
				resultString = fileOrDir.substring( rootLen );
			}			
		}
		
		return resultString;
	}
	
	//========================================================================
	public void applicationMainCreate( Context applicationContext ) 
	{
		Log.d(Constants.LIFETIME, "&&&&& APPLICATION applicationStartup");

		if( false == m_IsMainCreateCalledOnce )
		{
			m_IsMainCreateCalledOnce = true;
			m_ApplicationContext = applicationContext;
			m_oMgrNetwork				= new MgrNetwork(this);
			m_bIsShuttingDown = false;
			//Log.d(Constants.LIFETIME, "&&&&&&&&& APPLICATION applicationStartup m_oMgrCamCapture.startup" );
			if( null == m_oMgrCamCapture )
			{
				m_oMgrCamCapture = new CamCaptureMgr( this );
			}
			
			m_oMgrCamCapture.startupCamCaptureMgr();
			//Log.d(Constants.LIFETIME, "&&&&&&&&& APPLICATION applicationStartup m_oMgrSnd.startup" );
			if( null == m_oMgrSnd )
			{
				m_oMgrSnd = new MgrSound( this );
			}
			
			m_oMgrSnd.startup( m_ApplicationContext );			
			//Log.d(Constants.LIFETIME, "&&&&&&&&& APPLICATION applicationStartup done" );
		}	
	}

	//========================================================================
	public void applicationMainDestroy() 
	{
		Log.d(Constants.LIFETIME, "&&&&&&&&& APPLICATION applicationShutdown start");
		//if( m_IsMainCreateCalledOnce )
		{
			playSound( ESndDef.eSndDefAppShutdown );
			stopAppIdleTimer();
			m_bIsShuttingDown = true;
			if( null != m_oMgrNetwork )
			{
				m_oMgrNetwork.shutdownMgrNetwork();
				m_oMgrNetwork				= null;
			}
				
			m_bNetworkMgrStarted 		= false;

			NativeTxTo.fromGuiAppShutdown();
			m_oMyP2PWeb = null;
			if( null != m_oMgrCamCapture )
			{
				m_oMgrCamCapture.shutdownCamCaptureMgr();
				m_oMgrCamCapture = null;
			}
			
			if( null != m_oMgrSnd )
			{
				m_oMgrSnd.shutdown();	
				m_oMgrSnd = null;
			}
			
			m_IsMainCreateCalledOnce = false;
		}
		
		Log.d(Constants.LIFETIME, "&&&&&&&&& APPLICATION applicationShutdown done");
	}
	
	//========================================================================
	void setActivityPause( boolean isPaused ) 			
	{ 
		// a resume can happen on startup without pause so don't actual keep count
		if( isPaused )
		{
			m_PauseUpdateCountdown			= 4;
			m_ActivityPausedCounter 		= 1;
			//m_ActivityPausedCounter++;		
		}
		else
		{
			m_PauseUpdateCountdown			= 2;
			m_ActivityPausedCounter 		= 0;
			//m_ActivityPausedCounter--;	
		}
		
		Log.d( Constants.LIFETIME, "### setActivityPause counter " + m_ActivityPausedCounter );
		m_PauseEventNeedsUpdate 		= true;
	}	

	//========================================================================
	void  startActivity( int ePluginType, VxNetIdent  friendIdent, boolean isOffer )
	{
		if( false == getIsAppShuttingDown() )
		{
			switch( ePluginType )
			{
			case Constants.ePluginTypeCamServer:
				setCurrentFriend( friendIdent );
				if( isFriendMyself( friendIdent ) )
				{
					Intent myIntent = new Intent( getMainActivity(), ActivityCamServer.class );
					myIntent.putExtra("IS_OFFER", false);
					getMainActivity().startActivityForResult( myIntent, Constants.ACTIVITY_CAM_SERVER );
				}
				else
				{
					Intent myIntent = new Intent( getMainActivity(), ActivityCamClient.class );
					myIntent.putExtra("IS_OFFER", isOffer);
					getMainActivity().startActivityForResult( myIntent, Constants.ACTIVITY_CAM_CLIENT );				
				}
				break;

			case Constants.ePluginTypeVoicePhone:
				setCurrentFriend( friendIdent );
				Intent myVoicePhoneIntent = new Intent( getMainActivity(), AppletPeerVoicePhone.class );
				myVoicePhoneIntent.putExtra("IS_OFFER", isOffer);
				getMainActivity().startActivityForResult( myVoicePhoneIntent, Constants.ACTIVITY_TO_FRIEND_VOICE_PHONE );
				break;

			case Constants.ePluginTypeVideoPhone:
				setCurrentFriend( friendIdent );
				Intent myVideoPhoneIntent = new Intent( getMainActivity(), AppletPeerVideoPhone.class );
				myVideoPhoneIntent.putExtra("IS_OFFER", isOffer);
				getMainActivity().startActivityForResult( myVideoPhoneIntent, Constants.ACTIVITY_TO_FRIEND_VIDEO_PHONE );
				break;
				
			case Constants.ePluginTypeTruthOrDare:
				setCurrentFriend( friendIdent );
				Intent myTodGameIntent = new Intent( getMainActivity(), AppletPeerTodGame.class );
				myTodGameIntent.putExtra("IS_OFFER", isOffer);
				getMainActivity().startActivityForResult( myTodGameIntent, Constants.ACTIVITY_TO_FRIEND_TOD_GAME );
				break;			
				
			case Constants.ePluginTypeMultiSession:
				setCurrentFriend( friendIdent );
				Intent myMultiSessionIntent = new Intent( getMainActivity(), AppletPeerMultiSession.class );
				myMultiSessionIntent.putExtra("IS_OFFER", isOffer);
				getMainActivity().startActivityForResult( myMultiSessionIntent, Constants.ACTIVITY_TO_FRIEND_MULTISESSION );
				break;
				
			default:
				Log.e(LOG_TAG, "MyApp::startActivity UNKNOWN plugin Type" );
				break;				
			}
		}
	}
	
	//========================================================================
	// NOTE: only call toGuiUserMessage from java.. not thread safe
	public void toGuiUserMessage( String strMsg )
	{
		NativeRxFrom.toGuiUserMessage( strMsg );
	}
	
	//========================================================================
	// NOTE: only call toGuiUserMessage from java.. not thread safe
	public void toGuiUserMessage( int stringResId )
	{
		NativeRxFrom.toGuiUserMessage( stringResId );
	}
	
	//========================================================================
	public boolean flushDns()
	{
        Log.e(LOG_TAG, "ZZZZZZ flushDns start " );
        Process proc = null;
        try 
        {
        	//proc = Runtime.getRuntime().exec(new String[] { "su", "-c", "runYourCommand"});
        	
        	// some phones don't have su so this doesn't work
            proc = Runtime.getRuntime().exec("su");
            proc.getOutputStream().write(("ndc resolver flushdefaultif").getBytes());
            boolean result = ( proc.waitFor() == 0 );
            Log.e(LOG_TAG, "ZZZZZ flushDns result " + result );         
        } 
        catch (IOException e) 
        {
            Log.e(LOG_TAG, "ZZZZZ flushDns exception " );
            e.printStackTrace();
        }
        catch (InterruptedException e) 
        {
            Log.e(LOG_TAG, "ZZZZZ flushDns InterruptedException " );
            e.printStackTrace();
        }

        return false;
	}
	
	//========================================================================
	public void sendNetworkSettingsToNative() 
	{
		NetSettings netSettings = new NetSettings();
		netSettings.setAnchorWebsiteUrl( m_oSettingsDb.getAnchorWebsiteUrl() );
		netSettings.setNetworkName( m_oSettingsDb.getNetworkName() );
		netSettings.setIsDeviceAnAnchor( m_oSettingsDb.getIsDeviceAnAnchor() );
		netSettings.setNetServiceWebsiteUrl( m_oSettingsDb.getConnectionTestWebsiteUrl() );
		netSettings.setMyTcpInPort( m_oSettingsDb.getMyTcpInPort() );
		netSettings.setMyMulticastPort( m_oSettingsDb.getMyMulticastPort() );
		netSettings.setMulticastEnable( m_oSettingsDb.getMulticastEnable() );
		netSettings.setUseUpnpPortForward( m_oSettingsDb.getUseUpnpPortForward() );
		netSettings.setUserRelayPermissionCount( getMySettings().getUserRelayPermissionCount() );
		netSettings.setSystemRelayPermissionCount( getMySettings().getSystemRelayPermissionCount() );

		NativeTxTo.fromGuiSetNetSettings( netSettings );
	}
	
	//========================================================================
	public void setupDefaultProfile() throws IOException
	{
		if( false == m_IsMyResourcesInitialize )
		{
			copyNativeAssetsToAppStorageDir();			
			m_IsMyResourcesInitialize = true;
			Log.d( LOG_TAG, "setupDefaultProfile sending asset dir " + getAppAssetsDir() + " root dir " + getAppRootStorageDir() );
			NativeTxTo.fromGuiAppStartup( getAppAssetsDir(), getAppRootStorageDir() );
			Log.d( LOG_TAG, "setupDefaultProfile fromGuiAppStartup done" );
		}	
	}

	//========================================================================
	public void copyNativeAssetsToAppStorageDir() throws IOException
	{ 
		if( false == m_IsMyResourcesInitialize )
		{
			// copy files from assets folder in apk to folder in sdcard or internal memory so can be used by c++
			String cardAssetsDir = m_strAppAssetsDir;
			Log.d(LOG_TAG, "cardAssetsDir " + cardAssetsDir );

			VxFileUtil.createDirectory( cardAssetsDir );		
			
			AssetManager assetManager = getAssets();
			String[] nativeSubFoldersInAssetsFolder = null;
			try 
			{
				nativeSubFoldersInAssetsFolder = assetManager.list("native");
			} 
			catch (IOException e) 
			{
				Log.e(LOG_TAG, e.getMessage());
			}		
			
			int assetSubFolderCnt = nativeSubFoldersInAssetsFolder.length;
			for (int assetDirIdx = 0; assetDirIdx < assetSubFolderCnt; assetDirIdx++) 
			{
				String strSubDir = nativeSubFoldersInAssetsFolder[assetDirIdx];
				// make asset sub directories
				String cardAssetSubDir = cardAssetsDir + strSubDir; 
				VxFileUtil.createDirectory( cardAssetSubDir );
				//Log.e(LOG_TAG, "Creating dir " + cardAssetSubDir );
						
				String[] files = null;
				try 
				{
					files = assetManager.list("native/" + strSubDir);
				} 
				catch (IOException e) 
				{
					Log.e(LOG_TAG, e.getMessage());
				}
				
		        int iFileCnt = files.length;	
				Log.d(LOG_TAG, "Files in asset dir " + cardAssetSubDir + " " + iFileCnt );
				for( int iFileIdx = 0; iFileIdx < iFileCnt; iFileIdx++ ) 
				{
		        	String strJustFileName = files[iFileIdx];
		        	String strSrcFile = "native/" + strSubDir + "/" + strJustFileName;
		        	String strDestFile = cardAssetSubDir + "/" + strJustFileName;
					Log.d(LOG_TAG, "VxCopyAsssetToSdcardIfDoesntExist src " + strSrcFile + " dest " + strDestFile );
		        	m_VxFileUtil.VxCopyAsssetToSdcardIfDoesntExist(		strSrcFile, 
																	    strDestFile );
				}
			}				
		}
	}
	
	//========================================================================
	public void sendEmail( MyP2PWeb activity, String emailBodyText )
	{
		Intent result = new Intent(Intent.ACTION_SEND);
		result.setType("plain/text");
		result.putExtra(Intent.EXTRA_EMAIL, new String[]{"brett1900@usa.com"});
		result.putExtra(Intent.EXTRA_SUBJECT, "MyP2PWeb ");
		result.putExtra(Intent.EXTRA_TEXT, emailBodyText );

		activity.startActivity( result.createChooser(result, "Send Email") );		
	}

	//========================================================================
	public String stringResourceToText( int resourceId )
	{
		return getResources().getString( resourceId );
	}
	
	//========================================================================
	public boolean isFriendMyself(VxNetIdent oFriend) 
	{
		if ((oFriend.m_u64IdHiPart == m_oUserAccount.m_u64IdHiPart)
				&& (oFriend.m_u64IdLoPart == m_oUserAccount.m_u64IdLoPart)) 
		{
			return true;
		}
		
		return false;
	}

	//========================================================================
	public boolean isMyOnlineId( VxGUID oOnlineId ) 
	{
		if( ( oOnlineId.m_u64IdHiPart == m_oUserAccount.m_u64IdHiPart )
			&& ( oOnlineId.m_u64IdLoPart == m_oUserAccount.m_u64IdLoPart ) ) 
		{
			return true;
		}
		
		return false;
	}

    //========================================================================
	public void toGuiIsPortOpenStatus(String formatedMsg) 
	{
		if( null != m_oActTestIsMyPortOpen )
		{
			m_oActTestIsMyPortOpen.toGuiIsPortOpenStatus( formatedMsg );
		}
	}

    //========================================================================
	public void toGuiAnchorStatus(String formatedMsg) 
	{
		if( null != m_oActTestAnchorSettings )
		{
			m_oActTestAnchorSettings.toGuiAnchorStatus( formatedMsg );
		}
	}	

    //========================================================================
    private void savePreferenceString(String key, String value) 
    {
    	SharedPreferences sharedPreferences = PreferenceManager.getDefaultSharedPreferences(this);
    			Editor editor = sharedPreferences.edit();
    		        editor.putString(key, value);
    		        editor.commit();
    }
    
    //========================================================================
    private String loadPreferenceString(String key) 
    {
    	SharedPreferences sharedPreferences = PreferenceManager.getDefaultSharedPreferences(this);
    	return sharedPreferences.getString(key, "");
    }
 	
	//========================================================================
	public void setUserSpecificDirectory( String userSpecificDir ) 
	{
		m_strAppUserSpecificDir = userSpecificDir;
		VxFileUtil.createDirectory( m_strAppUserSpecificDir );
		m_strAppUserProfileDir = m_strAppUserSpecificDir + "profile/";
		VxFileUtil.createDirectory( m_strAppUserProfileDir );	
		// copy default profile page if none exist
		AssetManager assetManager = getAssets();
		String[] nativeSubFoldersInAssetsFolder = null;
		try 
		{
			nativeSubFoldersInAssetsFolder = assetManager.list("native");
		} 
		catch (IOException e) 
		{
			Log.e(LOG_TAG, e.getMessage());
		}		
		
		int assetSubFolderCnt = nativeSubFoldersInAssetsFolder.length;
		for (int assetDirIdx = 0; assetDirIdx < assetSubFolderCnt; assetDirIdx++) 
		{
			String strSubDir = nativeSubFoldersInAssetsFolder[assetDirIdx];
			if( !strSubDir.matches("profile"))
			{
				continue;
			}
			// make asset sub directories
			String cardAssetSubDir = m_strAppUserProfileDir;
			//Log.e( LOG_TAG, "Creating dir " + cardAssetSubDir );
					
			String[] files = null;
			try 
			{
				files = assetManager.list("native/" + strSubDir);
			} 
			catch (IOException e) 
			{
				Log.e( LOG_TAG, e.getMessage() );
			}
			
	        int iFileCnt = files.length;	
			Log.d( LOG_TAG, "Files in asset dir " + cardAssetSubDir + " " + iFileCnt );
			for( int iFileIdx = 0; iFileIdx < iFileCnt; iFileIdx++ ) 
			{
	        	String strJustFileName = files[iFileIdx];
	        	String strSrcFile = "native/" + strSubDir + "/" + strJustFileName;
	        	String strDestFile = cardAssetSubDir + "/" + strJustFileName;
	        	
        		// VxCopyAsssetToSdcardIfDoesntExist checks size of file for match size and will copy if doesn't match. 
	        	// We don't want to overwrite user changed profile page so just check if exists first
	        	if( false == VxFileUtil.fileExists( strDestFile  ) )
	        	{
					//Log.d(LOG_TAG, "VxCopyAsssetToSdcardIfDoesntExist src " + strSrcFile + " dest " + strDestFile );
	        		m_VxFileUtil.VxCopyAsssetToSdcardIfDoesntExist( strSrcFile, strDestFile );
	        	}
			}
		}				
	}
	
	//========================================================================
	public void setUserXferDirectory( String userXferDir ) 
	{
		m_strAppUserXferDir = userXferDir;
		VxFileUtil.createDirectory( m_strAppUserXferDir );
	}

	//========================================================================
	public boolean updateUserAccountFromDatabase() 
	{
		boolean bIsValid = false;
		String strLoginName = m_oDataHelper.getLastLogin();
		if( 0 != strLoginName.length() ) 
		{
			if( true == m_oDataHelper.getAccountByName( strLoginName, m_oUserAccount ) ) 
			{
				if( m_oUserAccount.isValid() ) 
				{
					m_oUserAccount.setOnlinePort( getMySettings().getMyTcpInPort() );
					m_oUserAccount.setOnlineIPv4( getMgrNetwork().getDefaultIpAddress() );
					m_oUserAccount.setRequiresRelay( m_bRelayRequired );
					bIsValid = true;
					Log.i(LOG_TAG, "updateUserAccount hi " + m_oUserAccount.m_u64IdHiPart + " lo " + m_oUserAccount.m_u64IdLoPart );
				}
			} 
			else 
			{
				Log.i(LOG_TAG, "login has last name but couldnt be retrieved from dB " );
			}
		}
		
		return bIsValid;
	}

	//========================================================================
	public void updateUserAccountFromEngine()
	{
		NativeTxTo.fromGuiQueryMyIdent( m_oUserAccount );
	}

	//========================================================================
	public void playSound( ESndDef sndDef ) 
	{
		if( null != m_oMgrSnd )
		{
			m_oMgrSnd.playSound( sndDef );
		}
	}

	//========================================================================
	public void playSoundFromThread( ESndDef sndDef ) 
	{
		if( null != m_oMgrSnd )
		{
			m_oMgrSnd.playSoundFromThread( sndDef );
		}
	}

	//========================================================================
	public void playButtonClick() 
	{
		m_TempButtonClickCnt++;
		Log.e( LOG_TAG, "playButtonClick " + String.valueOf( m_TempButtonClickCnt ) );
		playSound( ESndDef.eSndDefButtonClick );
	}

	//========================================================================
	public void startAppIdleTimer() 
	{
		Log.d(LOG_TAG, "StartAppIdleTimer");
		if (false == m_bAppIdleTimerStarted) 
		{
			m_bAppIdleTimerStarted 		= true; // run onIdle at 20 fps
			m_KeepIdleThreadRunining 	= true;
			Log.d(LOG_TAG, "StartAppIdleTimer: create runnable thread");
			m_oIdleThread = new Thread(new Runnable() 
			{
				long s32CurTime;
				long s32ElapsedTime;
				long s64SysTimeSec;

				// @Override
				public void run() 
				{
				    final String orgName = Thread.currentThread().getName();
				    Thread.currentThread().setName(orgName + " AppIdleTimer");		

					try 
					{
						while( m_KeepIdleThreadRunining 
								&& ( false == getIsAppShuttingDown() ) )
						{
							s32CurTime = System.currentTimeMillis();
							s32ElapsedTime = s32CurTime - m_s64SysTimeMs;
							m_s64SysTimeMs = s32CurTime; // we want 20 frames per second
							
							if( getIsAppPaused() )
							{
								// if app is asleep then native will still do things but more slowly
								if (s32ElapsedTime < 400) 
								{
									Thread.sleep(400 - s32ElapsedTime);
								}									
							}
							else
							{
								if (s32ElapsedTime < 300) 
								{
									Thread.sleep(300 - s32ElapsedTime);
								}								
							}

							NativeTxTo.fromGuiAppIdle();
							s64SysTimeSec = m_s64SysTimeMs / 1000;
							if (s64SysTimeSec != m_s64SysTimeSec) 
							{
								m_s64SysTimeSec = s64SysTimeSec;
								doOncePerSecond();
							}
						}
						
						Log.d( LOG_TAG, "Idle Thread Exit" );
						m_bAppIdleTimerStarted = false;
					} 
					catch (InterruptedException e) 
					{
						e.printStackTrace();
					}
				}			
			});
			
			if( false == getIsAppShuttingDown() )
			{
				m_oIdleThread.start();
			}
		}
	}

	//========================================================================
	void stopAppIdleTimer() 
	{
		m_KeepIdleThreadRunining = false;
		/*
		try
		{
			Log.i( LOG_TAG, "MyApp:stopAppIdleTimer start" );
			//if( m_oIdleThread.isAlive() )
			//{
			//	m_oIdleThread.join();
			//}
			
			Log.i( LOG_TAG, "MyApp:stopAppIdleTimer done" );
		}
		catch( InterruptedException e )
		{
			e.printStackTrace();
		}
		*/
	}
	
	//========================================================================
	public void doOncePerSecond() 
	{
		m_OffersMgr.onOncePerSecond();
		if( m_bNetworkMgrStarted )
		{
			getMgrNetwork().onOncePerSecond();
		}
		
		if( m_PauseEventNeedsUpdate ) 
		{
			m_PauseUpdateCountdown--;
			if( 0 >= m_PauseUpdateCountdown )
			{
				m_PauseEventNeedsUpdate = false;
				boolean isPausedState = ( 0 >= m_ActivityPausedCounter ) ? false : true;
				//Log.d( Constants.LIFETIME, "### Paused Counter " + m_ActivityPausedCounter + " Paused State " + isPausedState );
				if( isPausedState != m_AppIsPaused )
				{
					doOnApplicationPaused( isPausedState );
				}				
			}
		}
	}

	//========================================================================
	// ! show popup menu
	public void ShowPopupMenu(int iMenuId) 
	{
		try 
		{
			m_iActivePopupMenu = iMenuId;
			Intent myIntent = new Intent(m_oMyP2PWeb, PopupMenu.class);
			m_oMyP2PWeb.startActivityForResult(myIntent, iMenuId);
		} 
		catch (Exception e) 
		{
			Log.e(LOG_TAG, "ShowPopupMenu error: " + e.getMessage(), e);
		}
	}

	//========================================================================
	// ! user selected item from popup menu
	public void onMenuSelected(PopupMenuItem oMenuItem) 
	{
		m_oMyP2PWeb.onMenuSelected(oMenuItem);
	}
      
	//========================================================================
	public void onFileXferFileIconButClick( View v )
	{
		NativeRxFrom.onFileXferFileIconButClick(v);
	}
    
	//========================================================================
	public void onFileXferAcceptButClick( View v )
	{
		NativeRxFrom.onFileXferAcceptButClick(v);
	}
  
	//========================================================================
	public void onFileXferPlayButClick( View v )
	{
		NativeRxFrom.onFileXferPlayButClick(v);
	}
    
	//========================================================================
	public void onFileXferLibraryButClick( View v )
	{
		NativeRxFrom.onFileXferLibraryButClick(v);
	}
    
	//========================================================================
	public void onFileXferShareButClick( View v )
	{
		NativeRxFrom.onFileXferShareButClick(v);
	}
    
	//========================================================================
	public void onFileXferShredButClick( View v )
	{
		NativeRxFrom.onFileXferShredButClick(v);
	}

}

