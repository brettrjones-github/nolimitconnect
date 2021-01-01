#pragma once
//============================================================================
// Copyright (C) 2009 Brett R. Jones 
// Issued to MIT style license by Brett R. Jones in 2017
//
// You may use, copy, modify, merge, publish, distribute, sub-license, and/or sell this software 
// provided this Copyright is not modified or removed and is included all copies or substantial portions of the Software
//
// This code is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
//
// bjones.engineer@gmail.com
// http://www.nolimitconnect.com
//============================================================================

#include "IDefs.h"

#include <NetLib/NetSettings.h>

#include <PktLib/VxCommon.h>

#include <CoreLib/VxKeyDefs.h>
#include <CoreLib/VxFileTypeMasks.h>
#include <CoreLib/AssetDefs.h>
#include <CoreLib/MediaCallbackInterface.h>

class InetAddress;
class FileShareSettings;
class AssetInfo;

//! \public Plugin server state
enum EPluginServerState
{
	ePluginServerStateDisabled,		//< server is disabled
	ePluginServerStateStarted,		//< server is running
	ePluginServerStateStopped,		//< server is enabled but not running

	eMaxPluginServerState
};

//! \public Video recording state
enum EVideoRecordState
{
	eVideoRecordStateDisabled,
	eVideoRecordStateStopRecording,
	eVideoRecordStateStartRecording,
	eVideoRecordStateStartRecordingInPausedState,
	eVideoRecordStatePauseRecording,
	eVideoRecordStateResumeRecording,
	eVideoRecordStateCancelRecording,
	eVideoRecordStateError,

	eMaxVideoRecordState
};

//! \public Audio recording state
enum ESndRecordState
{
	eSndRecordStateDisabled,
	eSndRecordStateStopRecording,
	eSndRecordStateStartRecording,
	eSndRecordStateStartRecordingInPausedState,
	eSndRecordStatePauseRecording,
	eSndRecordStateResumeRecording,
	eSndRecordStateCancelRecording,
	eSndRecordStateError,

	eMaxSndRecordState
};

//! \public Media request for callback when processed MediaProcessor types
enum EMediaInputType
{
	eMediaInputNone,
	eMediaInputAudioPkts,
	eMediaInputAudioOpus,
	eMediaInputAudioPcm,

	eMediaInputVideoPkts,
	eMediaInputVideoJpgSmall,
	eMediaInputVideoJpgBig,

	eMediaInputMixer,

	eMaxMediaInputType
};

/// IFromGui is an abstract interface for from GUI to P2PEngine calls
class IFromGui
{
public:

	/// set is commercial not currently used ( Is set true or false by compiler define APP_MYP2PWEB )
	virtual void				fromGuiSetIsAppCommercial( bool isCommercial ) = 0;
	/// returns true if APP_MYP2PWEB is not defined
	/// if APP_MYP2PWEB is not defined then is commercial Test App build with source code for sale else is My P2P Web production build
	/// Test App has 2 differences from production build of My P2P Web 
	///  1.) Encryption is different so is not compatible with My P2P Web network. 
	///  2.) Default url for connection test and anchor services is p2pthing.com instead of gotvptop.com 
	virtual bool				fromGuiGetIsAppCommercial( void ) = 0;
	/// return engine version in binary
	virtual uint16_t			fromGuiGetAppVersionBinary( void ) = 0;
	/// return engine version as a 1.2.3 version number
	virtual const char *		fromGuiGetAppVersionString( void ) = 0;
	/// returns My P2P Web or Test App if commercial build
	virtual const char *		fromGuiGetAppName( void ) = 0;
	/// returns MyP2PWeb or TestApp if commercial build
	virtual const char *		fromGuiGetAppNameNoSpaces( void ) = 0;

	/// First call to engine should send path to assets ( game and app resources ) and path to root of where to write application data
	virtual void				fromGuiAppStartup( const char * assetsDir, const char * rootDataDir ) = 0;
	/// not currently used except for debug.. tells engine application is still running
	virtual void				fromGuiKickWatchdog( void ) = 0;

	/// Second call to engine should send path where to write login name specific application data 
	virtual void				fromGuiSetUserSpecificDir( const char * userSpecificDir ) = 0;
	/// Third call to engine should send path where to put downloads from other users
	virtual void				fromGuiSetUserXferDir( const char * userDownloadDir ) = 0;
	/// Call to engine when application is about to exit
	virtual void				fromGuiAppShutdown( void ) = 0;
	/// Returns disk space available in incomplete downloads directory
	virtual uint64_t			fromGuiGetDiskFreeSpace( void ) = 0; 

	/// Periodic call to engine for game and application updates
	virtual void				fromGuiAppIdle( void  ) = 0;  

	/// Call to engine when application is paused
	virtual void				fromGuiAppPause( void  ) = 0;
	/// Call to engine when application is resumed
	virtual void				fromGuiAppResume( void  ) = 0; 

	/// Axis orientation of device changed 
	virtual bool				fromGuiOrientationEvent( float f32RotX, float f32RotY, float f32RotZ ) = 0;
	/// Mouse or touch event
	virtual bool				fromGuiMouseEvent( EMouseButtonType eMouseButType, EMouseEventType eMouseEventType, int iMouseXPos, int iMouseYPos  ) = 0;
	/// Mouse wheel event
	virtual bool				fromGuiMouseWheel( float f32MouseWheelDist ) = 0;
	/// Key was pressed event
	virtual bool				fromGuiKeyEvent( EKeyEventType eKeyEventType, EKeyCode eKey, int iFlags = 0 ) = 0;

	/// Not currently used ( for opengl games )
	virtual void				fromGuiNativeGlInit( void ) = 0;
	/// Not currently used ( for opengl games )
	virtual void				fromGuiNativeGlResize( int w, int h ) = 0;
	/// Not currently used ( for opengl games )
	virtual int					fromGuiNativeGlRender( void ) = 0;
	/// Not currently used ( for opengl games )
	virtual void				fromGuiNativeGlPauseRender( void ) = 0;
	/// Not currently used ( for opengl games )
	virtual void				fromGuiNativeGlResumeRender( void ) = 0;
	/// Not currently used ( for opengl games )
	virtual void				fromGuiNativeGlDestroy( void ) = 0;

	///// Microphone sound capture ( 8000hz PCM 16 bit data, 80ms of sound ) 
	//virtual void				fromGuiMicrophoneData( int16_t* pu16PcmData, uint16_t pcmDataLenBytes ) = 0;
	///// Microphone sound capture with info for echo cancel ( 8000hz PCM 16 bit data, 80ms of sound ) 
	//virtual void				fromGuiMicrophoneDataWithInfo( int16_t * pcmData, int pcmDataLenBytes, int totalDelayTimeMs, int clockDrift ) = 0;
	///// Mute/Unmute microphone
	//virtual void				fromGuiMuteMicrophone( bool muteMic ) = 0; 
	///// Returns true if microphone is muted
	//virtual bool				fromGuiIsMicrophoneMuted( void ) = 0; 
	///// Mute/Unmute speaker
	//virtual void				fromGuiMuteSpeaker(	bool muteSpeaker ) = 0; 
	///// Returns true if speaker is muted
	//virtual bool				fromGuiIsSpeakerMuted( void ) = 0; 
	///// Enable/Disable echo cancellation
	//virtual void				fromGuiEchoCancelEnable( bool enableEchoCancel ) = 0; 
	///// Returns true if echo cancellation is enabled
	//virtual bool				fromGuiIsEchoCancelEnabled( void ) = 0; 

	///// Called when need more sound for speaker output
	//virtual void				fromGuiAudioOutSpaceAvail( int freeSpaceLen ) = 0;

	/// Record audio control
	virtual bool				fromGuiSndRecord( ESndRecordState eRecState, VxGUID& feedId, const char * fileName ) = 0;
	/// Record video control
	virtual bool				fromGuiVideoRecord( EVideoRecordState eRecState, VxGUID& feedId, const char * fileName  ) = 0;

    //! play video or audio file
	virtual bool				fromGuiPlayLocalMedia( const char *  fileName, uint64_t fileLen, uint8_t fileType, int pos ) = 0;

	/// Called when raw camera capture video data is available ( u32FourCc is the video data format )
	virtual void				fromGuiVideoData( uint32_t u32FourCc, uint8_t * pu8VidDataIn, int iWidth, int iHeight, uint32_t u32VidDataLen, int iRotation ) = 0;
	/// Called when cut scene movie has finished playing
	virtual bool				fromGuiMovieDone( void  ) = 0;

	/// Add/Remove callback from MediaProcessor when given media type is processed and available
	virtual void				fromGuiWantMediaInput( EMediaInputType mediaType, MediaCallbackInterface * callback, void * userData, bool wantInput ) = 0;
	/// Add/Remove callback from MediaProcessor when given media type is processed and available from specific user
	virtual void				fromGuiWantMediaInput( VxGUID& onlineId, EMediaInputType mediaType, bool wantInput ) = 0;

	/// Called with identity of user that logged on
	virtual void				fromGuiUserLoggedOn( VxNetIdent * netIdent ) = 0;
	/// Called when user changes his/her online name
	virtual void				fromGuiOnlineNameChanged( const char * newOnlineName ) = 0;
	/// Called when user changes his/her mood message
	virtual void				fromGuiMoodMessageChanged( const char * newMoodMessage ) = 0;
    /// Called when user changes his/her personal info
    virtual void				fromGuiIdentPersonalInfoChanged( int age, int gender, int language, int preferredContent ) = 0;

	/// Called when user changes his/her About Me web page picture.. sets flag that user has About Me Picture for scanning
	virtual void				fromGuiSetUserHasProfilePicture( bool haveAboutMePicture ) = 0;
	/// Called when user changes permission level of plugin
	virtual void				fromGuiUpdateMyIdent( VxNetIdent * netIdent, bool permissionAndStatsOnly = false ) = 0;
	/// Can be called to get users identity from engine.. usually for current node url or network info
	virtual void				fromGuiQueryMyIdent( VxNetIdent * poRetIdent ) = 0;
	/// Can tell engine to set has offers flag to be restored if application is restarted
	virtual void				fromGuiSetIdentHasTextOffers( VxGUID& onlineId, bool hasTextOffers ) = 0;
	/// Change permission level granted to a contact 
	virtual bool				fromGuiChangeMyFriendshipToHim(	VxGUID&			onlineId, 
																EFriendState	eMyFriendshipToHim,
																EFriendState	eHisFriendshipToMe ) = 0;
	/// Set network settings
	virtual void				fromGuiSetNetSettings( NetSettings& netSettings ) = 0;
	/// Get network settings
	virtual void				fromGuiGetNetSettings( NetSettings& netSettings ) = 0;
	/// Prompt network state machine to reconnect with new settings
	virtual void				fromGuiNetworkSettingsChanged( void ) = 0;
	/// Set number of users that can use device as relay
	virtual void				fromGuiSetRelaySettings( int userRelayMaxCnt, int systemRelayMaxCnt ) = 0;

	/// Call when internet connection is available
	virtual void				fromGuiNetworkAvailable( const char * lclIp = NULL, bool isCellularNetwork = false ) = 0;
	/// Call when internet connection is disconnected
	virtual void				fromGuiNetworkLost( void ) = 0;
    /// state of a network layer
    virtual ENetLayerState	    fromGuiGetNetLayerState( ENetLayerType netLayer = eNetLayerTypeInternet ) = 0;

	/// Run anchor settings connection tests
	virtual void				fromGuiVerifyNetHostSettings( void ) = 0;
	/// Run test to see if TCP port is open and what the external IP Address is
	virtual void				fromGuiRunIsPortOpenTest( uint16_t port ) = 0;
    /// Run test to query host guid
    virtual void				fromGuiRunQueryHostIdTest( void ) = 0;
    /// Run test on the given url
    virtual void				fromGuiRunUrlAction( const char * myUrl, const char * ptopUrl, ENetCmdType testType ) = 0;

	/// Set file share settings
	virtual void				fromGuiSetFileShareSettings( FileShareSettings& fileShareSettings ) = 0;
	/// Get file share settings
	virtual void				fromGuiGetFileShareSettings( FileShareSettings& fileShareSettings ) = 0;
	/// Update About Me Web page
	virtual void				fromGuiUpdateWebPageProfile(	const char *	pProfileDir,	// directory containing user profile
																const char *	strGreeting,	// greeting text
																const char *	aboutMe,		// about me text
																const char *	url1,			// favorite url 1
																const char *	url2,			// favorite url 2
                                                                const char *	url3,           // favorite url 3
                                                                const char *	donation ) = 0;	// donation		
	/// Set permission level required to access a plugin
	virtual void				fromGuiSetPluginPermission( EPluginType ePluginType, EFriendState eFriendState ) = 0;
	/// Get permission level required to access a plugin
	virtual int					fromGuiGetPluginPermission( EPluginType ePluginType ) = 0;
	/// Get server state of specific plugin ( disabled or in session or not in session )
	virtual EPluginServerState	fromGuiGetPluginServerState( EPluginType ePluginType ) = 0;

	/// Start plugin session or server
	virtual void				fromGuiStartPluginSession( EPluginType ePluginType, VxGUID& onlineId, int pvUserData = 0, VxGUID lclSessionId = VxGUID::nullVxGUID() ) = 0;
	/// Stop plugin session or server
	virtual void				fromGuiStopPluginSession( EPluginType ePluginType, VxGUID& onlineId, int pvUserData = 0, VxGUID lclSessionId = VxGUID::nullVxGUID()  ) = 0;
	/// Return true if plugin is in session
	virtual bool				fromGuiIsPluginInSession( EPluginType ePluginType, VxNetIdent * netIdent = NULL, int pvUserData = 0, VxGUID lclSessionId = VxGUID::nullVxGUID() ) = 0;
	/// Send offer of file or session to a contact
	virtual bool				fromGuiMakePluginOffer(	EPluginType		ePluginType, 
														VxGUID&			onlineId,
														int				pvUserData,
														const char *	pOfferMsg, 
														const char *	pFileName = NULL,
														uint8_t *		fileHashId = 0,
														VxGUID			lclSessionId = VxGUID::nullVxGUID() ) = 0;
	/// Contact sent session offer reply
	virtual bool				fromGuiToPluginOfferReply(	EPluginType		ePluginType,
															VxGUID&			onlineId,
															int				pvUserData,
															int				iOfferResponse,
															VxGUID			lclSessionId ) = 0;
	/// Plugin control such as cancel download etc.
	virtual int					fromGuiPluginControl(	EPluginType		ePluginType, 
														VxGUID&			onlineId, 
														const char *	pControl, 
														const char *	pAction,
														uint32_t		u32ActionData,
														VxGUID&			lclSessionId = VxGUID::nullVxGUID(),
														uint8_t *		fileHashId = 0 ) = 0;
	/// Send Text Message to contact
	virtual bool				fromGuiInstMsg(		EPluginType		ePluginType, 
													VxGUID&			onlineId, 
													const char *	pMsg ) = 0; 

	/// If true then network direct connect test failed and communication requires relay service from another node
	virtual void				fromGuiRequireRelay( bool bRequireRelay ) = 0;
	/// Set contact as preferred provider of relay service ( relay search will attempt relay service from this contact first ) 
	virtual void				fromGuiUseRelay( VxGUID& onlineId, bool bUseAsRelay = true ) = 0;

	//! Send to GUI all contacts for given view type selection
	virtual void				fromGuiSendContactList( EFriendViewType eFriendView, int MaxContactsToSend ) = 0;
	//! Send new announcement to anchor and refresh contact list
	virtual void				fromGuiRefreshContactList( int MaxContactsToSend ) = 0;

	///============================================================================
	/// Scan network for user shared resources
	///============================================================================

	/// Start scan of contacts on network for given resource ( About Me picture, Shared Web Cam, etc. )
	virtual void				fromGuiStartScan( EScanType eScanType, uint8_t searchFlags, uint8_t fileTypeFlags, const char * pSearchPattern = "" ) = 0;
	/// Force scan to move to next search result now instead of waiting for timer countdown
	virtual void				fromGuiNextScan( EScanType eScanType ) = 0;
	/// Stop scanning network
	virtual void				fromGuiStopScan( EScanType eScanType ) = 0;

	//============================================================================
	// misc
	//============================================================================

	/// Get current IPv4 IP Address
	virtual InetAddress			fromGuiGetMyIPv4Address( void ) = 0;
	/// Get current IPv6 IP Address
	virtual InetAddress			fromGuiGetMyIPv6Address( void ) = 0;

	/// Notify engine that user has modified his/her Story Board Web Page
	virtual void				fromGuiUserModifiedStoryboard( void ) = 0;

	/// Cancel a download
	virtual void				fromGuiCancelDownload( VxGUID& fileInstance ) = 0;
	/// Cancel a upload
	virtual void				fromGuiCancelUpload( VxGUID& fileInstance ) = 0;

	/// Set game variable ( used for Truth Or Dare video chat game )
	virtual bool				fromGuiSetGameValueVar( EPluginType	ePluginType, 
														VxGUID&		onlineId, 
														int32_t			varId, 
														int32_t			varValue ) = 0;
	/// Set game action ( used for Truth Or Dare video chat game )
	virtual bool				fromGuiSetGameActionVar(	EPluginType	ePluginType, 
															VxGUID&		onlineId, 
															int32_t			actionId, 
															int32_t			actionValue ) = 0;

	/// Send directory listing to GUI
	virtual bool				fromGuiBrowseFiles(	const char * dir, bool lookupShareStatus, uint8_t fileFilterMask = VXFILE_TYPE_ALLNOTEXE | VXFILE_TYPE_DIRECTORY ) = 0; 
	/// Send list of shared files to GUI
	virtual bool				fromGuiGetSharedFiles( uint8_t fileTypeFilter ) = 0;
	/// Share/Unshare a file
	virtual bool				fromGuiSetFileIsShared( const char * fileName, bool isShared, uint8_t * fileHashId = 0 ) = 0;
	/// Return true if file is shared
	virtual bool				fromGuiGetIsFileShared( const char * fileName ) = 0;

	/// Returns -1 if unknown else percent downloaded
	virtual int					fromGuiGetFileDownloadState( uint8_t * fileHashId ) = 0;
	/// Add/Remove file from library
	virtual bool				fromGuiAddFileToLibrary( const char * fileName, bool addFile, uint8_t * fileHashId = 0 ) = 0;
	/// Return true if file is in library
	virtual bool				fromGuiGetIsFileInLibrary( const char * fileName ) = 0;
	/// Send to GUI file that are in library of the given file type mask
	virtual void				fromGuiGetFileLibraryList( uint8_t fileTypeFilter ) = 0;
	/// Return true if video file was created by My P2P Web
	virtual bool				fromGuiIsMyP2PWebVideoFile( const char * fileName ) = 0;
	/// Return true if audio file was created by My P2P Web
	virtual bool				fromGuiIsMyP2PWebAudioFile( const char * fileName ) = 0;
	
	/// Delete file.. if shred file is true then write random data to file before deleting it
	virtual int					fromGuiDeleteFile( const char * fileName, bool shredFile ) = 0;

	/// Send Text Chat session assets of contact to GUI
	virtual void				fromGuiQuerySessionHistory( VxGUID& historyId ) = 0;
	/// Text Chat asset action such as remove
	virtual bool				fromGuiAssetAction( EAssetAction assetAction, AssetInfo& assetInfo, int pos0to100000 = 0 ) = 0;
	/// Text Chat asset action such as remove
	virtual bool				fromGuiAssetAction( EAssetAction assetAction, VxGUID& assetId, int pos0to100000 = 0 ) = 0;
	/// Send Text Chat asset to contact
	virtual bool				fromGuiSendAsset( AssetInfo& assetInfo ) = 0;
	/// Text Chat session action such as start video chat session
	virtual bool				fromGuiMultiSessionAction( EMSessionAction mSessionAction, VxGUID& onlineId, int pos0to100000, VxGUID lclSessionId = VxGUID::nullVxGUID() ) = 0;
	
	/// Get random TCP port that is not currently in use
	virtual uint16_t			fromGuiGetRandomTcpPort( void ) = 0;

    /// Get url for this node
    virtual void                fromGuiGetNodeUrl( std::string& nodeUrl ) = 0;
    /// Get internet status
    virtual EInternetStatus     fromGuiGetInternetStatus( void ) = 0;
    /// Get network status
    virtual ENetAvailStatus     fromGuiGetNetAvailStatus( void ) = 0;
#ifdef TARGET_OS_ANDROID
	/// Not used.. in android multicast for discovery of contact on same LAN is done in Jave code
	virtual int					fromGuiMulitcastPkt( unsigned char * data, int len ) = 0;
#endif // TARGET_OS_ANDROID

	enum ETestParam1
	{
		eTestParam1Unknown					= 0,
		eTestParam1FullNetTest1				= 1,
		eTestParam1Ping						= 2,
		eTestParam1WhatsMyIp				= 3,
		eTestParam1IsMyPortOpen				= 4,
		eTestParam1AnnounceNow				= 5,
		eTestParamSoundDelayTest			= 6,

		eMaxTestParam1
	};

	/// For testing and debug only 
	virtual bool				fromGuiTestCmd(	IFromGui::ETestParam1		eTestParam1, 
												int							testParam2 = 0, 
												const char *				testParam3 = NULL ) = 0;
	/// Debug only set which logging to filter
	virtual void				fromGuiDebugSettings( uint32_t u32LogFlags, const char *	pLogFileName = NULL ) = 0;
	/// Debug only send capture of log messages to GUI
	virtual void				fromGuiSendLog( uint32_t u32LogFlags ) = 0;
};
