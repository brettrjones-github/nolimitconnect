#pragma once
//============================================================================
// Copyright (C) 2014 Brett R. Jones
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

#ifdef TARGET_OS_ANDROID
#include "build_info.h"
#include "AndroidAudioMgr.h"

#include <GuiInterface/IToGui.h>
#include <GuiInterface/IFromGui.h>
#include <GoTvCore/GoTvP2P/P2PEngine/P2PEngine.h>

#include <NetLib/VxPeerMgr.h>

//const uint32_t					MY_FRAME_DATA_LEN			= 1280;
//const uint32_t					MY_FRAME_SAMPLES			= 640;

class  EngineImp : public IToGui
{
public:
	EngineImp();
	virtual ~EngineImp();

	P2PEngine&					getEngine( void )					{ return m_Engine; }
	webrtc::BuildInfo&			getBuildInfo( void )				{ return m_BuildInfo; }

	//============================================================================
	//=== to gui ===//
	//============================================================================
	virtual void				toGuiLog( int logFlags, const char * pMsg );
	virtual void				toGuiAppErr( EAppErr eAppErr, const char* errMsg = "" );
	virtual void				toGuiStatusMessage( const char* statusMsg );

	virtual void				toGuiWantMicrophoneRecording( bool wantMicInput );
	virtual void				toGuiWantSpeakerOutput( bool wantSpeakerOutput );
	virtual void				toGuiPlayAudio( int16_t * pu16PcmData, int pcmDataLenInBytes );

	virtual void				toGuiWantVideoCapture( bool wantVidCapture );
	virtual void				toGuiPlayVideoFrame( VxGUID& onlineId, uint8_t * pu8Jpg, uint32_t u32JpgDataLen, int motion0to100000 );

	virtual void				toGuiNetworkState( ENetworkStateType eNetworkState, const char* stateMsg = "" );
	virtual void				toGuiMyRelayStatus( EMyRelayStatus eRelayStatus, const char * msg = "" );
	virtual void				toGuiHostStatus( EHostTestStatus eHostStatus, const char * msg = "" );
	virtual void				toGuiIsPortOpenStatus( EIsPortOpenStatus eIsPortOpenStatus, const char * msg = "" );
    virtual void				toGuiRunTestStatus( const char *testName, ERunTestStatus eRunTestStatus, const char * msg = "" ) override;
	virtual void				toGuiRandomConnectStatus( ERandomConnectStatus eRandomConnectStatus, const char * msg = "" );

	virtual void				toGuiContactOnline( VxNetIdent * netIdent, bool newContact = false );
	virtual void				toGuiContactOffline( VxNetIdent * netIdent );
	virtual void				toGuiContactNearby( VxNetIdent * netIdent );
	virtual void				toGuiContactNotNearby( VxNetIdent * netIdent );

	//! called when contact changes
	virtual void				toGuiContactNameChange( VxNetIdent * netIdent );
	virtual void				toGuiContactDescChange( VxNetIdent * netIdent );
	virtual void				toGuiContactMyFriendshipChange( VxNetIdent * netIdent );
	virtual void				toGuiContactHisFriendshipChange( VxNetIdent * netIdent );
	virtual void				toGuiContactSearchFlagsChange( VxNetIdent * netIdent );
	virtual void				toGuiContactConnectionChange( VxNetIdent * netIdent );
	virtual void				toGuiContactAnythingChange( VxNetIdent * netIdent );
	virtual void				toGuiContactLastSessionTimeChange( VxNetIdent * netIdent );

	virtual void				toGuiPluginPermissionChange( VxNetIdent * netIdent );

	//! called from engine when need to update identity
	virtual void				toGuiUpdateMyIdent( VxNetIdent * netIdent );
	virtual void				fromGuiSetIdentHasTextOffers( VxGUID& onlineId, bool hasTextOffers ); 

	//! add offer to notify list
	virtual void				toGuiRxedPluginOffer(  VxNetIdent *		netIdent, 
														EPluginType		ePluginType, 
														const char *	pMsg,
													   	int				pvUserData,
														const char *	pFileName = NULL,
														uint8_t *			fileHashData = 0,
														VxGUID&			lclSessionId = VxGUID::nullVxGUID(),
														VxGUID&			rmtSessionId = VxGUID::nullVxGUID() );
	//! response to offer
	virtual void				toGuiRxedOfferReply(	VxNetIdent *	netIdent, 
														EPluginType		ePluginType,
														int				pvUserData,
														EOfferResponse	eOfferResponse, 
														const char *	pFileName = NULL,
														uint8_t *			fileHashData = 0,
														VxGUID&			lclSessionId = VxGUID::nullVxGUID(),
														VxGUID&			rmtSessionId = VxGUID::nullVxGUID() );

	virtual void				toGuiPluginSessionEnded(	VxNetIdent *	netIdent, 
															EPluginType		ePluginType,
															int				pvUserData,
															EOfferResponse	eOfferResponse,
															VxGUID&			lclSessionId = VxGUID::nullVxGUID() );

	virtual void				toGuiPluginStatus(	EPluginType		ePluginType,
													int				statusType,
													int				statusValue );

	virtual void				toGuiInstMsg(	VxNetIdent *	netIdent, 
												EPluginType		ePluginType, 
												const char *	pMsg );

	virtual void				toGuiFileListReply(	VxNetIdent *	netIdent, 
													EPluginType		ePluginType, 
													uint8_t				u8FileType, 
													uint64_t				u64FileLen, 
													const char *	pFileName,
													uint8_t *			fileHashData );

	virtual void				toGuiStartUpload(	VxNetIdent *	netIdent, 
													EPluginType		ePluginType, 
													VxGUID&			fileInstanceId, 
													uint8_t				u8FileType, 
													uint64_t				u64FileLen, 
													const char *	pFileName,
													uint8_t *			fileHashData );

	virtual void				toGuiStartDownload(	VxNetIdent *	netIdent, 
													EPluginType		ePluginType, 
													VxGUID&			fileInstanceId, 
													uint8_t				u8FileType, 
													uint64_t				u64FileLen, 
													const char *	pFileName,
													uint8_t *			fileHashData );
	
	virtual void				toGuiFileDownloadComplete( VxGUID& lclSessionId, const char * newFileName, EXferError xferError );
	virtual void				toGuiFileUploadComplete( VxGUID& lclSessionId, EXferError xferError );

	virtual void				toGuiFileXferState( VxGUID& lclSessionId, EXferState eXferState, int param1, int param2 );

	//=== scan ===//
	virtual void				toGuiScanSearchComplete( EScanType eScanType );	
	virtual void				toGuiSearchResultSuccess( EScanType eScanType, VxNetIdent *	netIdent );	
	virtual void				toGuiSearchResultError( EScanType eScanType, VxNetIdent *	netIdent, int errCode );	

	virtual void				toGuiSearchResultProfilePic(	VxNetIdent *	netIdent, 
																uint8_t *			pu8JpgData, 
																uint32_t				u32JpgDataLen );

	virtual void				toGuiSearchResultFileSearch(	VxNetIdent *	netIdent, 		
																VxGUID&			fileInstanceId, 
																uint8_t				u8FileType, 
																uint64_t				u64FileLen, 
																const char *	pFileName );

	virtual bool				toGuiSetGameValueVar(	EPluginType	ePluginType, 
														VxGUID&	oOnlineId, 
														int32_t			s32VarId, 
														int32_t			s32VarValue );

	virtual bool				toGuiSetGameActionVar(	EPluginType	ePluginType, 
														VxGUID&	oOnlineId, 
														int32_t			s32VarId, 
														int32_t			s32VarValue );
	
	virtual void				toGuiFileList(	const char *	fileName, 
												uint64_t				fileLen, 
												uint8_t				fileType, 
												bool			isShared,
												bool			isInLibrary,
												uint8_t *			fileHashId = 0 );

	virtual void				toGuiAssetAdded( AssetInfo * assetInfo );
	virtual void				toGuiAssetSessionHistory( AssetInfo * assetInfo );
	virtual void				toGuiAssetAction( EAssetAction assetAction, VxGUID& assetId, int pos0to100000 );
	virtual void				toGuiMultiSessionAction( EMSessionAction mSessionAction, VxGUID& onlineId, int pos0to100000 );

	//============================================================================
	//=== from gui ===//
	//============================================================================
	virtual void				fromGuiSetupContext( void );

	virtual void				fromGuiSetIsAppCommercial( bool isCommercial );
	virtual bool				fromGuiGetIsAppCommercial( void );
	virtual uint16_t					fromGuiGetAppVersionBinary( void );
	const char *				fromGuiGetAppVersionString( void );
	virtual const char *		fromGuiGetAppName( void );
	virtual const char *		fromGuiGetAppNameNoSpaces( void );

	virtual void				fromGuiAppStartup( const char * assetDir, const char * rootDataDir );
	virtual void				fromGuiKickWatchdog( void );

	virtual void				fromGuiSetUserXferDir( const char * userDir );
	virtual void				fromGuiSetUserSpecificDir( const char * userDir );
	virtual uint64_t					fromGuiGetDiskFreeSpace( void  ); 
	virtual void				fromGuiAppShutdown( void );

	virtual void				fromGuiAppIdle( void );
	virtual void				fromGuiAppPauseOrResume( bool isPaused );

	virtual bool				fromGuiOrientationEvent( float f32RotX, float f32RotY, float f32RotZ  );
	virtual bool				fromGuiMouseEvent( EMouseButtonType eMouseButType, EMouseEventType eMouseEventType, int iMouseXPos, int iMouseYPos  );
	virtual bool				fromGuiMouseWheel( float f32MouseWheelDist );
	virtual bool				fromGuiKeyEvent( EKeyEventType eKeyEventType, EKeyCode eKey, int iFlags = 0 );

	virtual void				fromGuiNativeGlInit( void );
	virtual void				fromGuiNativeGlResize( int width, int height );
	virtual int					fromGuiNativeGlRender( void );
	virtual void				fromGuiNativeGlPauseRender( void );
	virtual void				fromGuiNativeGlResumeRender( void );
	virtual void				fromGuiNativeGlDestroy( void );

	virtual void				fromGuiNeedMorePlayData(	int16_t * retAudioSamples,
															int deviceReqDataLen );
	virtual void				fromGuiMicrophoneData( int16_t * pcmData, int pcmDataLenInBytes );
	virtual void				fromGuiMuteMicrophone( bool mute ); 
	virtual void				fromGuiMuteSpeaker( bool mute ); 
	virtual void				fromGuiEchoCancelEnable( bool enableEchoCancel );
	virtual bool				fromGuiIsMicrophoneMuted( void );	
	virtual bool				fromGuiIsSpeakerMuted( void );		
	virtual bool				fromGuiIsEchoCancelEnabled( void );	

	virtual void				fromGuiWantMediaInput( VxGUID& id, EMediaInputType eMediaType, bool bWantInput );
	virtual void				fromGuiVideoData( uint32_t u32FourCc, uint8_t * pu8VidDataIn, int iWidth, int iHeight, uint32_t u32VidDataLen, int iRotation );
	virtual bool				fromGuiMovieDone( void );

	virtual void				fromGuiUserLoggedOn( VxNetIdent * netIdent );
	virtual void				fromGuiOnlineNameChanged( const char * newOnlineName );
	virtual void				fromGuiMoodMessageChanged( const char * newMoodMessage );
	virtual void				fromGuiSetUserHasProfilePicture( bool haveProfilePick );
	virtual void				fromGuiUpdateMyIdent( VxNetIdent * netIdent, bool permissionAndStatsOnly );
	virtual void				fromGuiQueryMyIdent( VxNetIdent * poRetIdent );

	virtual void				fromGuiGetNetSettings( NetSettings& netSettings );
	virtual void				fromGuiSetNetSettings( NetSettings& netSettings );
	virtual void				fromGuiNetworkSettingsChanged( void );

	virtual void				fromGuiNetworkAvailable( const char * lclIp = NULL, bool isCellularNetwork = false );
	virtual void				fromGuiNetworkLost( void );
    virtual ENetLayerState	    fromGuiGetNetLayerState( ENetLayerType netLayer = eNetLayerTypeInternet );

	virtual void				fromGuiVerifyHostSettings( void );
	virtual void				fromGuiRunIsPortOpenTest( uint16_t port );

	virtual void				fromGuiGetFileShareSettings( FileShareSettings& fileShareSettings );
	virtual void				fromGuiSetFileShareSettings( FileShareSettings& fileShareSettings );

	virtual void				fromGuiUpdateWebPageProfile(	const char *	pProfileDir,	// directory containing user profile
																const char *	strGreeting,	// greeting text
																const char *	aboutMe,		// about me text
																const char *	url1,			// favorite url 1
																const char *	url2,			// favorite url 2
																const char *	url3 );			// favorite url 3

	virtual void 				fromGuiSetPluginPermission( EPluginType ePluginType, int eFriendState );
	virtual int					fromGuiGetPluginPermission( EPluginType ePluginType );
	virtual int					fromGuiGetPluginServerState( EPluginType ePluginType );

	virtual void				fromGuiStartPluginSession( EPluginType ePluginType, VxGUID& oOnlineId, int pvUserData = 0, VxGUID lclSessionId = VxGUID::nullVxGUID() );
	virtual void				fromGuiStopPluginSession( EPluginType ePluginType, VxGUID& oOnlineId, int pvUserData = 0, VxGUID lclSessionId = VxGUID::nullVxGUID()  );
	virtual bool				fromGuiIsPluginInSession( EPluginType ePluginType, VxGUID& oOnlineId, int pvUserData = 0, VxGUID lclSessionId = VxGUID::nullVxGUID() );

	virtual bool				fromGuiMakePluginOffer(	EPluginType		ePluginType, 
														VxGUID&			oOnlineId,
														int				pvUserData,
														const char *	pOfferMsg, 
														const char *	pFileName,
														VxGUID&			lclSessionId );

	virtual bool				fromGuiToPluginOfferReply(	EPluginType		ePluginType,
															VxGUID&			oOnlineId,
															int				pvUserData,
															int				iOfferResponse,
															VxGUID&			lclSessionId );

	virtual int					fromGuiPluginControl(	EPluginType		ePluginType, 
														VxGUID&			oOnlineId, 
														const char *	pControl, 
														const char *	pAction,
														uint32_t				u32ActionData,
														VxGUID&			lclSessionId,
														uint8_t *			fileHashData );

	virtual bool				fromGuiInstMsg(		EPluginType		ePluginType, 
													VxGUID&			oOnlineId, 
													const char *	pMsg ); 

	virtual bool				fromGuiChangeMyFriendshipToHim(	VxGUID&			oOnlineId, 
																EFriendState	eMyFriendshipToHim,
																EFriendState	eHisFriendshipToMe );
	virtual void				fromGuiSendContactList( EFriendViewType eFriendView, int maxContactsToSend );
	virtual void				fromGuiRefreshContactList( int maxContactsToSend );

	virtual void				fromGuiRequireRelay( bool bRequireRelay );
	virtual void				fromGuiUseRelay( VxGUID& oOnlineId, bool bUseAsRelay = true );
	virtual void				fromGuiRelayPermissionCount( int userPermittedCount, int anonymousCount ); 

	virtual void				fromGuiStartScan( EScanType eScanType, uint8_t searchFlags, uint8_t fileTypeFlags, const char * pSearchPattern = "" );
	virtual void				fromGuiNextScan( EScanType eScanType );
	virtual void				fromGuiStopScan( EScanType eScanType );

	virtual InetAddress			fromGuiGetMyIPv4Address( void );
	virtual InetAddress			fromGuiGetMyIPv6Address( void );

	virtual void				fromGuiUserModifiedStoryboard( void );

	virtual void				fromGuiCancelDownload( VxGUID&			fileInstanceId );
	virtual void				fromGuiCancelUpload( VxGUID&			fileInstanceId );

	virtual bool				fromGuiSetGameValueVar( EPluginType	ePluginType, 
														VxGUID&		oOnlineId, 
														int32_t			s32VarId, 
														int32_t			s32VarValue );

	virtual bool				fromGuiSetGameActionVar(	EPluginType	ePluginType, 
															VxGUID&		oOnlineId, 
															int32_t			s32VarId, 
															int32_t			s32VarValue );

	virtual bool				fromGuiTestCmd(	IFromGui::ETestParam1		eTestParam1, 
												int							testParam2 = 0, 
												const char *				testParam3 = NULL );
	virtual void				fromGuiMulitcastPkt( uint8_t * pktData, int dataLen );
	virtual void				fromGuiDebugSettings( uint32_t u32LogFlags, const char *	pLogFileName = NULL );
	virtual void				fromGuiSendLog( uint32_t u32LogFlags );

	virtual bool				fromGuiBrowseFiles(	const char * dir, bool lookupShareStatus, uint8_t fileFilterMask = VXFILE_TYPE_ALLNOTEXE | VXFILE_TYPE_DIRECTORY ); 
	virtual bool				fromGuiGetSharedFiles( uint8_t fileTypeFilter );
	virtual bool				fromGuiSetFileIsShared( const char * fileName, bool isShared );
	virtual bool				fromGuiGetIsFileShared( const char * fileName );
	
	virtual int					fromGuiGetFileDownloadState( uint8_t * fileHashId );
	virtual bool				fromGuiAddFileToLibrary( const char * fileName, bool addFile, uint8_t * fileHashId = 0 );
	virtual void				fromGuiGetFileLibraryList( uint8_t fileTypeFilter );
	virtual bool				fromGuiGetIsFileInLibrary( const char * fileName );
	virtual bool				fromGuiIsMyP2PWebVideoFile( const char * fileName );	
	virtual bool				fromGuiIsMyP2PWebAudioFile( const char * fileName );	

	virtual int					fromGuiDeleteFile( const char * fileName, bool shredFile );

	virtual bool				fromGuiAssetAction( EAssetAction assetAction, AssetInfo& assetInfo, int pos0to100000 = 0 );
	virtual bool				fromGuiAssetAction( EAssetAction assetAction, VxGUID& assetId, int pos0to100000 = 0 );
	virtual bool				fromGuiSendAsset( AssetInfo& assetInfo );

	virtual bool				fromGuiVideoRecord( EVideoRecordState eRecState, VxGUID& feedId, const char * fileName );
	virtual bool				fromGuiSndRecord( ESndRecordState eRecState, VxGUID& feedId, const char * fileName );

	virtual void				fromGuiQuerySessionHistory( VxGUID& historyId );
	virtual bool				fromGuiMultiSessionAction( EMSessionAction mSessionAction, VxGUID& onlineId, int pos0to100000 );
	
private:
	//=== vars ===//
	VxPeerMgr					m_VxPeerMgr;
	P2PEngine					m_Engine;
	webrtc::BuildInfo			m_BuildInfo;
	webrtc::AudioManager *		m_AudioMgr;
};

EngineImp&	GetEngineImp( void );

#endif //TARGET_OS_ANDROID
