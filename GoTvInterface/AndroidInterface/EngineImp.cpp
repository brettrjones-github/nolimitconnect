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

#include "NativeCommon.h"
#include "EngineImp.h"

#include "JavaHelpers.h"

#include "NativeToJava.h"
#include <CoreLib/VxGlobals.h>

#define ENGINE_LOG_TAG "NativeEngine:"

namespace
{
	EngineImp *					g_EngineImp = NULL; 
}

//============================================================================
EngineImp&	GetEngineImp( void )
{
	if( NULL == g_EngineImp )
	{
		__android_log_write( ANDROID_LOG_INFO, ENGINE_LOG_TAG, "GetEngineImp create start" );
		g_EngineImp = new EngineImp();
		__android_log_write( ANDROID_LOG_INFO, ENGINE_LOG_TAG, "GetEngineImp create done" );
	}

	return *g_EngineImp;
}

//============================================================================
EngineImp::EngineImp()
: m_VxPeerMgr()
, m_Engine( *this, m_VxPeerMgr )
//, m_MyFrameReadIdx( 0 )
//, m_MyFrameDataAvail( 0 )
, m_BuildInfo()
, m_AudioMgr( 0 )
{
	__android_log_write( ANDROID_LOG_INFO, ENGINE_LOG_TAG, "EngineImp::EngineImp" );
}

//============================================================================
EngineImp::~EngineImp()
{
	__android_log_write( ANDROID_LOG_INFO, ENGINE_LOG_TAG, "EngineImp::~EngineImp" );
}

//============================================================================

//============================================================================
void EngineImp::fromGuiMicrophoneData( int16_t * pcmData, int pcmDataLenInBytes )
{
	//__android_log_write( ANDROID_LOG_INFO, ENGINE_LOG_TAG, "fromGuiMicrophoneData" );
	m_Engine.fromGuiMicrophoneDataWithInfo( pcmData, pcmDataLenInBytes, 0, 0 );
}

//============================================================================
void EngineImp::fromGuiNeedMorePlayData(	int16_t * retAudioSamples,
											int deviceReqDataLen )
{
	//__android_log_write( ANDROID_LOG_INFO, ENGINE_LOG_TAG, "fromGuiNeedMorePlayData" );
	memset( retAudioSamples, 0, deviceReqDataLen );
}

//============================================================================
void EngineImp::toGuiLog( int logLevel, const char * msg )
{
	//__android_log_write( ANDROID_LOG_INFO, ENGINE_LOG_TAG, "toGuiLog" );
	JavaToGuiLog( logLevel, msg );
}

//============================================================================
void EngineImp::toGuiAppErr( EAppErr eAppErr, const char * errMsg )
{
	__android_log_write( ANDROID_LOG_INFO, ENGINE_LOG_TAG, "toGuiAppErr" );
	JavaToGuiAppErr( (int)eAppErr, errMsg );
}

//============================================================================
void EngineImp::toGuiStatusMessage( const char * statusMsg )
{
	//__android_log_write( ANDROID_LOG_INFO, ENGINE_LOG_TAG, "toGuiStatusMessage" );
	JavaToGuiStatusMessage( statusMsg );
}

//============================================================================
void EngineImp::toGuiWantMicrophoneRecording( bool wantMicInput )
{
	__android_log_write( ANDROID_LOG_INFO, ENGINE_LOG_TAG, "toGuiWantMicrophoneRecording" );
	JavaToGuiWantMicrophoneRecording( wantMicInput );
}

//============================================================================
void EngineImp::toGuiWantSpeakerOutput( bool wantSpeakerOutput )
{
	//__android_log_write( ANDROID_LOG_INFO, ENGINE_LOG_TAG, "toGuiWantSpeakerOutput" );
	JavaToGuiWantSpeakerOutput( wantSpeakerOutput );
}

//============================================================================
void EngineImp::toGuiPlayAudio( int16_t * pu16PcmData, int pcmDataLenInBytes )
{
	//JavaToGuiPlayAudio( pu16PcmData, u16PcmDataLen );
	if( 0 != m_AudioMgr )
	{
		m_AudioMgr->toGuiPlayAudio( pu16PcmData, pcmDataLenInBytes );
	}
}

//============================================================================
void EngineImp::toGuiWantVideoCapture( bool wantVidCapture )
{
	JavaToGuiWantVideoCapture( wantVidCapture );
}

//============================================================================
void EngineImp::toGuiPlayVideoFrame( VxGUID& onlineId, uint8_t * pu8Jpg, uint32_t u32JpgDataLen, int motion0to100000 )
{
	if( false == m_Engine.isAppPaused() )
	{
		JavaToGuiPlayVideoFrame( onlineId, pu8Jpg, u32JpgDataLen, motion0to100000 );
	}
}

//============================================================================
void EngineImp::toGuiNetworkState( ENetworkStateType eNetworkState, const char* stateMsg )
{
	//__android_log_write( ANDROID_LOG_INFO, ENGINE_LOG_TAG, "toGuiNetworkState" );
	JavaToGuiNetworkState( eNetworkState, stateMsg );
}

//============================================================================
void EngineImp::toGuiMyRelayStatus( EMyRelayStatus eRelayStatus, const char * msg )
{
	//__android_log_write( ANDROID_LOG_INFO, ENGINE_LOG_TAG, "toGuiMyRelayStatus" );
	JavaToGuiMyRelayStatus( eRelayStatus, msg );
}

//============================================================================
void EngineImp::toGuiHostStatus( EHostTestStatus eHostStatus, const char * msg )
{
	JavaToGuiHostStatus( eHostStatus, msg );
}

//============================================================================
void EngineImp::toGuiIsPortOpenStatus( EIsPortOpenStatus eIsPortOpenStatus, const char * msg )
{
	JavaToGuiIsPortOpenStatus( eIsPortOpenStatus, msg );
}

//============================================================================
void EngineImp::toGuiIsPortOpenStatus( const char *testName, ERunTestStatus eIsPortOpenStatus, const char * msg )
{
    JavaToGuiIsPortOpenStatus( testName, eIsPortOpenStatus, msg );
}

//============================================================================
void EngineImp::toGuiRandomConnectStatus( ERandomConnectStatus eRandomConnectStatus, const char * msg )
{
	JavaToGuiRandomConnectStatus( eRandomConnectStatus, msg );
}

//============================================================================
void EngineImp::toGuiContactOnline( VxNetIdent * netIdent, bool newContact )
{
	//__android_log_write( ANDROID_LOG_INFO, ENGINE_LOG_TAG, "toGuiContactOnline" );
	JavaToGuiContactOnline( netIdent, newContact );
}

//============================================================================
void EngineImp::toGuiContactOffline( VxNetIdent * netIdent )
{
	JavaToGuiContactOffline( netIdent );
}

//============================================================================
void EngineImp::toGuiContactNearby( VxNetIdent * netIdent )
{
	JavaToGuiContactNearby( netIdent );
}

//============================================================================
void EngineImp::toGuiContactNotNearby( VxNetIdent * netIdent )
{
	JavaToGuiContactNotNearby( netIdent );
}

//============================================================================
void EngineImp::toGuiContactNameChange( VxNetIdent * netIdent )
{
	JavaToGuiContactNameChange( netIdent );
}

//============================================================================
void EngineImp::toGuiContactDescChange( VxNetIdent * netIdent )
{
	JavaToGuiContactDescChange( netIdent );
}

//============================================================================
void EngineImp::toGuiContactMyFriendshipChange( VxNetIdent * netIdent )
{
	JavaToGuiContactMyFriendshipChange( netIdent );
}

//============================================================================
void EngineImp::toGuiContactHisFriendshipChange( VxNetIdent * netIdent )
{
	JavaToGuiContactHisFriendshipChange( netIdent );
}

//============================================================================
void EngineImp::toGuiContactSearchFlagsChange( VxNetIdent * netIdent )
{
	JavaToGuiContactSearchFlagsChange( netIdent );
}

//============================================================================
void EngineImp::toGuiContactConnectionChange( VxNetIdent * netIdent )
{
	JavaToGuiContactConnectionChange( netIdent );
}

//============================================================================
void EngineImp::toGuiContactLastSessionTimeChange( VxNetIdent * netIdent )
{
	JavaToGuiContactLastSessionTimeChange( netIdent );
}

//============================================================================
void EngineImp::toGuiContactAnythingChange( VxNetIdent * netIdent )
{
	JavaToGuiContactAnythingChange( netIdent );
}

//============================================================================
void EngineImp::toGuiPluginPermissionChange( VxNetIdent * netIdent )
{
	JavaToGuiPluginPermissionChange( netIdent );
}

//============================================================================
void EngineImp::toGuiUpdateMyIdent( VxNetIdent * netIdent )
{
	JavaToGuiUpdateMyIdent( netIdent );
}

//============================================================================
void EngineImp::toGuiRxedPluginOffer(	VxNetIdent *	netIdent, 
										EPluginType		ePluginType, 
										const char *	pMsg,
										 int			pvUserData,
										const char *	pFileName,
										uint8_t *			fileHashData,
										VxGUID&			lclSessionId,
										VxGUID&			rmtSessionId )
{
	JavaToGuiRxedPluginOffer(	netIdent, 
								ePluginType, 
								pMsg, 
								pvUserData, 
								pFileName,
								fileHashData,
								lclSessionId,
								rmtSessionId  );
}

//============================================================================
void EngineImp::toGuiRxedOfferReply(	VxNetIdent *	netIdent, 
										EPluginType		ePluginType,
										int				pvUserData,
										EOfferResponse	eOfferResponse, 
										const char *	pFileName,
										uint8_t *			fileHashData,
										VxGUID&			lclSessionId,
										VxGUID&			rmtSessionId )
{
	JavaToGuiRxedOfferReply(	netIdent, 
								ePluginType, 
								pvUserData, 
								eOfferResponse, 
								pFileName,
								fileHashData,
								lclSessionId,
								rmtSessionId );
}

//============================================================================
void EngineImp::toGuiPluginSessionEnded(	VxNetIdent *	netIdent, 
											EPluginType		ePluginType,
											int				pvUserData,
											EOfferResponse	eOfferResponse,
											VxGUID&			lclSessionId )
{
	JavaToGuiPluginSessionEnded(	netIdent, 
									ePluginType, 
									pvUserData, 
									eOfferResponse,
									lclSessionId );
}

//============================================================================
void EngineImp::toGuiPluginStatus(	EPluginType		ePluginType,
									int				statusType,
									int				statusValue )
{
	JavaToGuiPluginStatus(	ePluginType,
							statusType,
							statusValue );
}

//============================================================================
void EngineImp::toGuiInstMsg(	VxNetIdent *	netIdent, 
								EPluginType		ePluginType, 
								const char *	pMsg )
{
	JavaToGuiInstMsg(	netIdent, 
						ePluginType, 
						pMsg );
}

//============================================================================
void EngineImp::toGuiFileList(		const char *	fileName,
									uint64_t				fileLen,
									uint8_t				fileType,
									bool			isShared,
									bool			isInLibrary,
									uint8_t *			fileHashId )
{
	JavaToGuiFileList(		fileName,
							fileLen,
							fileType,
							isShared,
							isInLibrary,
							fileHashId );
}

//============================================================================
void EngineImp::toGuiFileListReply(		VxNetIdent *	netIdent, 
										EPluginType		ePluginType, 
										uint8_t				u8FileType, 
										uint64_t				u64FileLen, 
										const char *	pFileName,
										uint8_t *			fileHashData )
{
	JavaToGuiFileListReply(		netIdent, 
								ePluginType, 
								u8FileType, 
								u64FileLen, 
								pFileName,
								fileHashData );
}

//============================================================================
void EngineImp::toGuiStartUpload(	VxNetIdent *	netIdent, 
									EPluginType		ePluginType, 
									VxGUID&			fileInstanceId, 
									uint8_t				u8FileType, 
									uint64_t				u64FileLen, 
									const char *	pFileName,
									uint8_t *			fileHashData )
{
	JavaToGuiStartUpload(	netIdent, 
							ePluginType, 
							fileInstanceId, 
							u8FileType, 
							u64FileLen, 
							pFileName,
							fileHashData );
}

//============================================================================
void EngineImp::toGuiStartDownload(		VxNetIdent *	netIdent, 
										EPluginType		ePluginType, 
										VxGUID&			fileInstanceId, 
										uint8_t				u8FileType, 
										uint64_t				u64FileLen, 
										const char *	pFileName,
										uint8_t *			fileHashData )
{
	JavaToGuiStartDownload(		netIdent, 
								ePluginType, 
								fileInstanceId, 
								u8FileType, 
								u64FileLen, 
								pFileName,
								fileHashData );
}

//============================================================================
void EngineImp::toGuiFileXferState( VxGUID&	lclSessionId, EXferState eXferState, int param1, int param2 )
{
	JavaToGuiFileXferState(	lclSessionId, eXferState, param1, param2 );
}

//============================================================================
void EngineImp::toGuiFileDownloadComplete( VxGUID& lclSessionId, const char * newFileName, EXferError xferError )
{
	JavaToGuiFileDownloadComplete( lclSessionId, newFileName, xferError );
}

//============================================================================
void EngineImp::toGuiFileUploadComplete( VxGUID& lclSessionId, EXferError xferError )
{
	JavaToGuiFileUploadComplete( lclSessionId, xferError );
}

//============================================================================
void EngineImp::toGuiScanSearchComplete( EScanType eScanType )
{
	JavaToGuiScanSearchComplete( eScanType );
}

//============================================================================
void EngineImp::toGuiSearchResultSuccess( EScanType eScanType, VxNetIdent *	netIdent )
{
	JavaToGuiSearchResultSuccess( eScanType, netIdent );
}

//============================================================================
void EngineImp::toGuiSearchResultError( EScanType eScanType, VxNetIdent * netIdent, int errCode )
{
	JavaToGuiSearchResultError( eScanType, netIdent, errCode );
}

//============================================================================
void EngineImp::toGuiSearchResultProfilePic(	VxNetIdent *	netIdent, 
												uint8_t *			pu8JpgData, 
												uint32_t				u32JpgDataLen )
{
	JavaToGuiSearchResultProfilePic( netIdent, pu8JpgData, u32JpgDataLen );
}

//============================================================================
void EngineImp::toGuiSearchResultFileSearch(	VxNetIdent *	netIdent, 		
												VxGUID&			fileInstanceId,
												uint8_t				u8FileType,
												uint64_t				u64FileLen,
												const char *	pFileName )
{
	JavaToGuiSearchResultFileSearch(	netIdent, 		
										fileInstanceId, 
										u8FileType, 
										u64FileLen, 
										pFileName );
}

//============================================================================
bool EngineImp::toGuiSetGameValueVar(	EPluginType	ePluginType, 
										VxGUID&	oOnlineId, 
										int32_t			s32VarId, 
										int32_t			s32VarValue )
{
	return JavaToGuiSetGameValueVar(	ePluginType, 
										oOnlineId, 
										s32VarId, 
										s32VarValue );
}

//============================================================================
bool EngineImp::toGuiSetGameActionVar(	EPluginType	ePluginType, 
										VxGUID&	oOnlineId, 
										int32_t			s32VarId, 
										int32_t			s32VarValue )
{
	return JavaToGuiSetGameActionVar(	ePluginType, 
										oOnlineId, 
										s32VarId, 
										s32VarValue );
}

//============================================================================
void EngineImp::toGuiAssetAdded( AssetInfo * assetInfo )
{
    if( LOG_FLAG_ASSETS & VxGetModuleLogFlags() )
	    LogMsg( LOG_DEBUG, "EngineImp::toGuiAssetAdded\n" );
	JavaToGuiAssetAdded( assetInfo );
}

//============================================================================
void EngineImp::toGuiAssetSessionHistory( AssetInfo * assetInfo )
{
	JavaToGuiSessionHistory( assetInfo );
}

//============================================================================
void EngineImp::toGuiAssetAction( EAssetAction assetAction, VxGUID& assetId, int pos0to100000 )
{
	JavaToGuiAssetAction(	assetAction, 
							assetId, 
							pos0to100000 );
}

//============================================================================
void EngineImp::toGuiMultiSessionAction( EMSessionAction mSessionAction, VxGUID& onlineId, int pos0to100000 )
{
	JavaToGuiMultiSessionAction(	mSessionAction, 
									onlineId, 
									pos0to100000 );
}

//============================================================================
//=== from gui ===//
//============================================================================
void EngineImp::fromGuiSetupContext( void )
{
	//LogMsg( LOG_DEBUG, "EngineImp::fromGuiSetupContext\n");
	if( 0 == m_AudioMgr )
	{
		//__android_log_write( ANDROID_LOG_INFO, ENGINE_LOG_TAG, "fromGuiSetupContext create Audio Manager" );
		m_AudioMgr = new webrtc::AudioManager( *this );
		//__android_log_write( ANDROID_LOG_INFO, ENGINE_LOG_TAG, "fromGuiSetupContext initAndroidAudioMgr" );
		m_AudioMgr->initAndroidAudioMgr();
		//__android_log_write( ANDROID_LOG_INFO, ENGINE_LOG_TAG, "fromGuiSetupContext create Audio Manager done" );
	}
}

//============================================================================
void EngineImp::fromGuiSetIsAppCommercial( bool isCommercial )
{
	VxSetIsApplicationCommercial( isCommercial );
}
 
//============================================================================
bool EngineImp::fromGuiGetIsAppCommercial( void )
{
	return VxGetIsApplicationCommercial();
}


//============================================================================
uint16_t EngineImp::fromGuiGetAppVersionBinary( void )
{
	return VxGetAppVersion();
}

//============================================================================
const char * EngineImp::fromGuiGetAppVersionString( void )
{
	return VxGetAppVersionString();
}

//============================================================================
const char * EngineImp::fromGuiGetAppName( void )
{
	return VxGetApplicationName();
}

//============================================================================
const char * EngineImp::fromGuiGetAppNameNoSpaces( void )
{
	return VxGetApplicationNameNoSpaces();
}

//============================================================================
void EngineImp::fromGuiAppStartup( const char * assetDir, const char * rootDataDir )
{
	//LogMsg( LOG_DEBUG, "EngineImp::fromGuiAppStartup.. calling fromGuiSetupContext\n");
	if( 0 == m_AudioMgr )
	{
		fromGuiSetupContext();
	}

	m_Engine.fromGuiAppStartup( assetDir, rootDataDir );

	if( 0 != m_AudioMgr )
	{
		m_AudioMgr->toGuiWantMicrophoneRecording( true );
	}

	if( 0 != m_AudioMgr )
	{
		m_AudioMgr->toGuiWantSpeakerOutput( true );
	}
}

//============================================================================
void EngineImp::fromGuiKickWatchdog( void  )
{
	m_Engine.fromGuiKickWatchdog();
}

//============================================================================
void EngineImp::fromGuiSetUserXferDir( const char * userDir )
{
	//LogMsg( LOG_DEBUG, "EngineImp::fromGuiSetUserXferDir %s\n", userDir );
	m_Engine.fromGuiSetUserXferDir( userDir );
}

//============================================================================
void EngineImp::fromGuiSetUserSpecificDir( const char * userDir )
{
	//LogMsg( LOG_DEBUG, "EngineImp::fromGuiSetUserSpecificDir %s\n", userDir);
	m_Engine.fromGuiSetUserSpecificDir( userDir );
}

//============================================================================
uint64_t EngineImp::fromGuiGetDiskFreeSpace( void  ) 
{
	return m_Engine.fromGuiGetDiskFreeSpace();
}

//============================================================================
void EngineImp::fromGuiAppShutdown( void  )
{
	__android_log_write( ANDROID_LOG_INFO, ENGINE_LOG_TAG, "---fromGuiAppShutdown start.. doing audio shutdown\n" );
	VxSetAppIsShuttingDown( true );
	if( 0 != m_AudioMgr )
	{
		m_AudioMgr->Close();
	}

	__android_log_write( ANDROID_LOG_INFO, ENGINE_LOG_TAG, "---fromGuiAppShutdown doing engine shutdown\n" );
	m_Engine.fromGuiAppShutdown();
	__android_log_write( ANDROID_LOG_INFO, ENGINE_LOG_TAG, "---fromGuiAppShutdown done\n" ); 
}

//============================================================================
void EngineImp::fromGuiAppIdle( void  )
{
	m_Engine.fromGuiAppIdle();
}

//============================================================================
void EngineImp::fromGuiAppPauseOrResume( bool isPaused )
{
	if( isPaused )
	{
		m_Engine.fromGuiAppPause();
	}
	else
	{
		m_Engine.fromGuiAppResume();
	}
}

//============================================================================
void EngineImp::fromGuiUserLoggedOn( VxNetIdent * netIdent )
{
	m_Engine.fromGuiUserLoggedOn( netIdent );
}

//============================================================================
void EngineImp::fromGuiOnlineNameChanged( const char * newOnlineName )
{
	m_Engine.fromGuiOnlineNameChanged( newOnlineName );
}

//============================================================================
void EngineImp::fromGuiMoodMessageChanged( const char * newMoodMessage )
{
	m_Engine.fromGuiMoodMessageChanged( newMoodMessage );
}

//============================================================================
void EngineImp::fromGuiSetUserHasProfilePicture( bool haveProfilePick )
{
	m_Engine.fromGuiSetUserHasProfilePicture( haveProfilePick );
}

//============================================================================
bool EngineImp::fromGuiOrientationEvent( float f32RotX, float f32RotY, float f32RotZ  )
{
	return m_Engine.fromGuiOrientationEvent( f32RotX, f32RotY, f32RotZ );
}

//============================================================================
bool EngineImp::fromGuiMouseEvent( EMouseButtonType eMouseButType, EMouseEventType eMouseEventType, int iMouseXPos, int iMouseYPos  )
{
	return m_Engine.fromGuiMouseEvent( eMouseButType, eMouseEventType, iMouseXPos, iMouseYPos );
}

//============================================================================
bool EngineImp::fromGuiMouseWheel( float f32MouseWheelDist )
{
	return m_Engine.fromGuiMouseWheel( f32MouseWheelDist );
}

//============================================================================
bool EngineImp::fromGuiKeyEvent( EKeyEventType eKeyEventType, EKeyCode eKey, int iFlags )
{
	return m_Engine.fromGuiKeyEvent( eKeyEventType, eKey, iFlags );
}

//============================================================================
void EngineImp::fromGuiNativeGlInit( void )
{
	m_Engine.fromGuiNativeGlInit();
}

//============================================================================
void EngineImp::fromGuiNativeGlResize( int width, int height )
{
	m_Engine.fromGuiNativeGlResize( width, height );
}

//============================================================================
int EngineImp::fromGuiNativeGlRender( void )
{
	return m_Engine.fromGuiNativeGlRender();
}

//============================================================================
void EngineImp::fromGuiNativeGlPauseRender( void )
{
	m_Engine.fromGuiNativeGlPauseRender();
}

//============================================================================
void EngineImp::fromGuiNativeGlResumeRender( void )
{
	m_Engine.fromGuiNativeGlResumeRender();
}

//============================================================================
void EngineImp::fromGuiNativeGlDestroy( void )
{
	m_Engine.fromGuiNativeGlDestroy();
}

//============================================================================
void EngineImp::fromGuiMuteMicrophone( bool mute )
{
	m_Engine.fromGuiMuteMicrophone( mute );
}

//============================================================================
void EngineImp::fromGuiMuteSpeaker( bool mute )
{
	m_Engine.fromGuiMuteSpeaker( mute );
}

//============================================================================
void EngineImp::fromGuiEchoCancelEnable( bool enableEchoCancel )
{
	m_Engine.fromGuiEchoCancelEnable( enableEchoCancel );
}

//============================================================================
bool EngineImp::fromGuiIsMicrophoneMuted( void )
{
	return m_Engine.fromGuiIsMicrophoneMuted();
}

//============================================================================
bool EngineImp::fromGuiIsSpeakerMuted( void )
{
	return m_Engine.fromGuiIsSpeakerMuted();
}

//============================================================================
bool EngineImp::fromGuiIsEchoCancelEnabled( void )
{
	return m_Engine.fromGuiIsEchoCancelEnabled();
}

//============================================================================
void EngineImp::fromGuiWantMediaInput( VxGUID& id, EMediaInputType eMediaType, bool bWantInput )
{
	m_Engine.fromGuiWantMediaInput( id, eMediaType, bWantInput);
}

//============================================================================
void EngineImp::fromGuiVideoData( uint32_t u32FourCc, uint8_t * pu8VidDataIn, int iWidth, int iHeight, uint32_t u32VidDataLen, int iRotation )
{
	m_Engine.fromGuiVideoData( u32FourCc, pu8VidDataIn, iWidth, iHeight, u32VidDataLen, iRotation );
}

//============================================================================
bool EngineImp::fromGuiMovieDone( void )
{
	return m_Engine.fromGuiMovieDone();
}

//============================================================================
void EngineImp::fromGuiGetNetSettings( NetSettings& netSettings )
{
	m_Engine.fromGuiGetNetSettings( netSettings );
}

//============================================================================
void EngineImp::fromGuiSetNetSettings( NetSettings& netSettings )
{
	m_Engine.fromGuiSetNetSettings( netSettings );
}

//============================================================================
void EngineImp::fromGuiNetworkSettingsChanged( void )
{
	m_Engine.fromGuiNetworkSettingsChanged();
}

//============================================================================
void EngineImp::fromGuiVerifyHostSettings( void )
{
	m_Engine.fromGuiVerifyHostSettings();
}

//============================================================================
void EngineImp::fromGuiNetworkAvailable( const char * lclIp, bool isCellularNetwork )
{
	m_Engine.fromGuiNetworkAvailable( lclIp, isCellularNetwork );
}

//============================================================================
void EngineImp::fromGuiNetworkLost( void )
{
	m_Engine.fromGuiNetworkLost();
}

//============================================================================
void EngineImp::fromGuiRunIsPortOpenTest( uint16_t port )
{
	m_Engine.fromGuiRunIsPortOpenTest( port );
}

//============================================================================
void EngineImp::fromGuiGetFileShareSettings( FileShareSettings& fileShareSettings )
{
	m_Engine.fromGuiGetFileShareSettings( fileShareSettings );
}

//============================================================================
void EngineImp::fromGuiSetFileShareSettings( FileShareSettings& fileShareSettings )
{
	m_Engine.fromGuiSetFileShareSettings( fileShareSettings );
}

//============================================================================
void EngineImp::fromGuiUpdateWebPageProfile(	const char *	pProfileDir,	// directory containing user profile
														const char *	strGreeting,	// greeting text
														const char *	aboutMe,		// about me text
														const char *	url1,			// favorite url 1
														const char *	url2,			// favorite url 2
														const char *	url3 )
{
	m_Engine.fromGuiUpdateWebPageProfile(	pProfileDir,	// directory containing user profile
											strGreeting,	// greeting text
											aboutMe,		// about me text
											url1,			// favorite url 1
											url2,			// favorite url 2
											url3 );
}

//============================================================================
void EngineImp::fromGuiSetPluginPermission( EPluginType ePluginType, int eFriendState )
{
	m_Engine.fromGuiSetPluginPermission( ePluginType, (EFriendState)eFriendState );
}

//============================================================================
int EngineImp::fromGuiGetPluginPermission( EPluginType ePluginType )
{
	return (int)m_Engine.fromGuiGetPluginPermission( ePluginType );
}

//============================================================================
int EngineImp::fromGuiGetPluginServerState( EPluginType ePluginType )
{
	return m_Engine.fromGuiGetPluginServerState( ePluginType );
}

//============================================================================
void EngineImp::fromGuiStartPluginSession( EPluginType ePluginType, VxGUID& oOnlineId, int pvUserData, VxGUID lclSessionId )
{
	m_Engine.fromGuiStartPluginSession( ePluginType, oOnlineId, pvUserData, lclSessionId );
}

//============================================================================
void EngineImp::fromGuiStopPluginSession( EPluginType ePluginType, VxGUID& oOnlineId, int pvUserData, VxGUID lclSessionId  )
{
	m_Engine.fromGuiStopPluginSession( ePluginType, oOnlineId, pvUserData, lclSessionId );
}

//============================================================================
bool EngineImp::fromGuiIsPluginInSession( EPluginType ePluginType, VxGUID& oOnlineId, int pvUserData, VxGUID lclSessionId )
{
	BigListInfo * bigListInfo = m_Engine.getBigListMgr().findBigListInfo( oOnlineId ); 
	return m_Engine.fromGuiIsPluginInSession( ePluginType, (VxNetIdent *)bigListInfo, pvUserData, lclSessionId );
}

//============================================================================
bool EngineImp::fromGuiMakePluginOffer(	EPluginType		ePluginType, 
									    VxGUID&			oOnlineId,
										int				pvUserData,
									    const char *	pOfferMsg, 
									    const char *	pFileName,
										VxGUID&			sessionId  )
{
	return m_Engine.fromGuiMakePluginOffer(	ePluginType, 
											oOnlineId, 
											pvUserData,
											pOfferMsg, 
											pFileName,
											0,
											sessionId  );
}

//============================================================================
bool EngineImp::fromGuiToPluginOfferReply(	EPluginType		ePluginType,
											VxGUID&			oOnlineId,
											  int  			pvUserData,
											int				iOfferResponse,
											VxGUID&			lclSessionId  )
{
	return m_Engine.fromGuiToPluginOfferReply(	ePluginType,
												oOnlineId, 
												pvUserData,
												iOfferResponse,
												lclSessionId  );
}

//============================================================================
int EngineImp::fromGuiPluginControl(	EPluginType		ePluginType, 
										VxGUID&			oOnlineId, 
										const char *	pControl, 
										const char *	pAction,
										uint32_t				u32ActionData,
										VxGUID&			lclSessionId,
										uint8_t *			fileHashData )
{
	return m_Engine.fromGuiPluginControl(	ePluginType, 
											oOnlineId, 
											pControl, 
											pAction,
											u32ActionData,
											lclSessionId,
											fileHashData );
}

//============================================================================
bool EngineImp::fromGuiInstMsg(		EPluginType		ePluginType, 
									VxGUID&			oOnlineId, 
									const char *	pMsg )
{
	return m_Engine.fromGuiInstMsg( ePluginType, oOnlineId, pMsg );
}

//============================================================================
bool EngineImp::fromGuiChangeMyFriendshipToHim(	VxGUID&			oOnlineId, 
												EFriendState	eMyFriendshipToHim,
												EFriendState	eHisFriendshipToMe )
{
	return m_Engine.fromGuiChangeMyFriendshipToHim( oOnlineId, eMyFriendshipToHim, eHisFriendshipToMe );
}

//============================================================================
void EngineImp::fromGuiSendContactList( EFriendViewType eFriendView, int maxContactsToSend )
{
	m_Engine.fromGuiSendContactList( eFriendView, maxContactsToSend );
}

//============================================================================
void EngineImp::fromGuiRefreshContactList( int maxContactsToSend )
{
	m_Engine.fromGuiRefreshContactList( maxContactsToSend );
}

//============================================================================
void EngineImp::fromGuiRequireRelay( bool bRequireRelay )
{
	m_Engine.fromGuiRequireRelay( bRequireRelay );
}

//============================================================================
void EngineImp::fromGuiUseRelay( VxGUID& oOnlineId, bool bUseAsRelay )
{
	m_Engine.fromGuiUseRelay( oOnlineId, bUseAsRelay );
}

//============================================================================
void EngineImp::fromGuiRelayPermissionCount( int userPermittedCount, int anonymousCount )
{
	m_Engine.fromGuiRelayPermissionCount( userPermittedCount, anonymousCount );
}

//============================================================================
void EngineImp::fromGuiStartScan( EScanType eScanType, uint8_t searchFlags, uint8_t fileTypeFlags, const char * pSearchPattern )
{
	m_Engine.fromGuiStartScan( eScanType, searchFlags, fileTypeFlags, pSearchPattern );
}

//============================================================================
void EngineImp::fromGuiNextScan( EScanType eScanType )
{
	m_Engine.fromGuiNextScan( eScanType );
}

//============================================================================
void EngineImp::fromGuiStopScan( EScanType eScanType )
{
	m_Engine.fromGuiStopScan( eScanType );
}

//============================================================================
InetAddress EngineImp::fromGuiGetMyIPv4Address( void )
{
	return m_Engine.fromGuiGetMyIPv4Address();
}

//============================================================================
InetAddress EngineImp::fromGuiGetMyIPv6Address( void )
{
	return m_Engine.fromGuiGetMyIPv6Address();
}

//============================================================================
void EngineImp::fromGuiUpdateMyIdent( VxNetIdent * netIdent, bool permissionAndStatsOnly )
{
	m_Engine.fromGuiUpdateMyIdent( netIdent, permissionAndStatsOnly );
}

//============================================================================
void EngineImp::fromGuiQueryMyIdent( VxNetIdent * poRetIdent )
{
	m_Engine.fromGuiQueryMyIdent( poRetIdent );
}

//============================================================================
void EngineImp::fromGuiSetIdentHasTextOffers( VxGUID& onlineId, bool hasTextOffers )
{
	m_Engine.fromGuiSetIdentHasTextOffers( onlineId, hasTextOffers );
}

//============================================================================
void EngineImp::fromGuiUserModifiedStoryboard( void )
{
	m_Engine.fromGuiUserModifiedStoryboard();
}

//============================================================================
void EngineImp::fromGuiCancelDownload( VxGUID& fileInstanceId )
{
	m_Engine.fromGuiCancelDownload( fileInstanceId );
}

//============================================================================
void EngineImp::fromGuiCancelUpload( VxGUID& fileInstanceId )
{
	m_Engine.fromGuiCancelUpload( fileInstanceId );
}

//============================================================================
bool EngineImp::fromGuiSetGameValueVar( EPluginType	ePluginType, 
									   VxGUID&	oOnlineId, 
									   int32_t			s32VarId, 
									   int32_t			s32VarValue )
{
	return m_Engine.fromGuiSetGameValueVar( ePluginType, 
											oOnlineId, 
											s32VarId, 
											s32VarValue );
}

//============================================================================
bool EngineImp::fromGuiSetGameActionVar(	EPluginType	ePluginType, 
											VxGUID&	oOnlineId, 
											int32_t			s32VarId, 
											int32_t			s32VarValue )
{
	return m_Engine.fromGuiSetGameActionVar(	ePluginType, 
												oOnlineId, 
												s32VarId, 
												s32VarValue );
}

//============================================================================
bool EngineImp::fromGuiTestCmd(	IFromGui::ETestParam1		eTestParam1, 
								int							testParam2, 
								const char *				testParam3 )
{
	return m_Engine.fromGuiTestCmd( eTestParam1, testParam2, testParam3 );
}

//============================================================================
void EngineImp::fromGuiMulitcastPkt( uint8_t * pktData, int dataLen )
{
	m_Engine.fromGuiMulitcastPkt( pktData, dataLen );
}

//============================================================================
void EngineImp::fromGuiDebugSettings( uint32_t u32LogFlags, const char *	pLogFileName )
{
	m_Engine.fromGuiDebugSettings( u32LogFlags, pLogFileName );
}

//============================================================================
void EngineImp::fromGuiSendLog( uint32_t u32LogFlags )
{
	m_Engine.fromGuiSendLog( u32LogFlags );
}

//============================================================================
bool EngineImp::fromGuiBrowseFiles( const char * dir, bool lookupShareStatus, uint8_t fileFilterMask )
{
	return m_Engine.fromGuiBrowseFiles( dir, lookupShareStatus, fileFilterMask );
}

//============================================================================
bool EngineImp::fromGuiGetSharedFiles( uint8_t fileTypeFilter )
{
	return m_Engine.fromGuiGetSharedFiles( fileTypeFilter );
}

//============================================================================
bool EngineImp::fromGuiSetFileIsShared(	const char * fileName, bool isShared )
{
	return m_Engine.fromGuiSetFileIsShared( fileName, isShared );
}

//============================================================================
bool EngineImp::fromGuiGetIsFileShared(	const char * fileName )
{
	return m_Engine.fromGuiGetIsFileShared( fileName );
}

//============================================================================
int EngineImp::fromGuiGetFileDownloadState( uint8_t * fileHashId )
{
	return m_Engine.fromGuiGetFileDownloadState( fileHashId );	
}

//============================================================================
bool EngineImp::fromGuiAddFileToLibrary( const char * fileName, bool addFile, uint8_t * fileHashId )
{
	return m_Engine.fromGuiAddFileToLibrary( fileName, addFile, fileHashId );	
}

//============================================================================
void EngineImp::fromGuiGetFileLibraryList( uint8_t fileTypeFilter )
{
	m_Engine.fromGuiGetFileLibraryList( fileTypeFilter );
}

//============================================================================
bool EngineImp::fromGuiGetIsFileInLibrary( const char * fileName )
{
	return m_Engine.fromGuiGetIsFileInLibrary( fileName );	
}

//============================================================================
bool EngineImp::fromGuiIsMyP2PWebVideoFile( const char * fileName )
{
	return m_Engine.fromGuiIsMyP2PWebVideoFile( fileName );	
}

//============================================================================
bool EngineImp::fromGuiIsMyP2PWebAudioFile( const char * fileName )
{
	return m_Engine.fromGuiIsMyP2PWebAudioFile( fileName );
}
	
//============================================================================
int EngineImp::fromGuiDeleteFile( const char * fileName, bool shredFile )
{
	return m_Engine.fromGuiDeleteFile( fileName, shredFile );
}

//============================================================================
bool EngineImp::fromGuiAssetAction( EAssetAction assetAction, AssetInfo& assetInfo, int pos0to100000 )
{
	return m_Engine.fromGuiAssetAction( assetAction, assetInfo, pos0to100000 );
}

//============================================================================
bool EngineImp::fromGuiAssetAction( EAssetAction assetAction, VxGUID& assetId, int pos0to100000 )
{
	return m_Engine.fromGuiAssetAction( assetAction, assetId, pos0to100000 );
}

//============================================================================
bool EngineImp::fromGuiSendAsset( AssetInfo& assetInfo )
{
	return m_Engine.fromGuiSendAsset( assetInfo );
}

//============================================================================
bool EngineImp::fromGuiVideoRecord( EVideoRecordState eRecState, VxGUID& feedId, const char * fileName )
{
	return m_Engine.fromGuiVideoRecord( eRecState, feedId, fileName );
}

//============================================================================
bool EngineImp::fromGuiSndRecord( ESndRecordState eRecState, VxGUID& feedId, const char * fileName )
{
	return m_Engine.fromGuiSndRecord( eRecState, feedId, fileName );
}

//============================================================================
void EngineImp::fromGuiQuerySessionHistory( VxGUID& historyId )
{
	m_Engine.fromGuiQuerySessionHistory( historyId );	
}

//============================================================================
bool EngineImp::fromGuiMultiSessionAction( EMSessionAction mSessionAction, VxGUID& onlineId, int pos0to100000 )
{
	return m_Engine.fromGuiMultiSessionAction( mSessionAction, onlineId, pos0to100000 );
}

#endif //TARGET_OS_ANDROID

