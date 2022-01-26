#include <CommonHdr.h>
#include "IGoTv.h"
#include "ptop_src/ptop_engine_src/P2PEngine/P2PEngine.h"
#include <ptop_src/ptop_engine_src/BigListLib/BigListInfo.h>

#include <CoreLib/VxGlobals.h>

//============================================================================
//=== from gui ===//
//============================================================================
void IGoTv::fromGuiSetupContext( void )
{
    //LogMsg( LOG_DEBUG, "IGoTv::fromGuiSetupContext\n");
    //if( 0 == m_AudioMgr )
    //{
    //    //__android_log_write( ANDROID_LOG_INFO, ENGINE_LOG_TAG, "fromGuiSetupContext create Audio Manager" );
    //    m_AudioMgr = new webrtc::AudioManager( *this );
    //    //__android_log_write( ANDROID_LOG_INFO, ENGINE_LOG_TAG, "fromGuiSetupContext initAndroidAudioMgr" );
    //    m_AudioMgr->initAndroidAudioMgr();
    //    //__android_log_write( ANDROID_LOG_INFO, ENGINE_LOG_TAG, "fromGuiSetupContext create Audio Manager done" );
    //}
}

//============================================================================
void IGoTv::fromGuiSetIsAppCommercial( bool isCommercial )
{
    VxSetIsApplicationCommercial( isCommercial );
}

//============================================================================
bool IGoTv::fromGuiGetIsAppCommercial( void )
{
    return VxGetIsApplicationCommercial();
}

//============================================================================
uint16_t IGoTv::fromGuiGetAppVersionBinary( void )
{
    return VxGetAppVersion();
}

//============================================================================
const char * IGoTv::fromGuiGetAppVersionString( void )
{
    return VxGetAppVersionString();
}

//============================================================================
const char * IGoTv::fromGuiGetAppName( void )
{
    return VxGetApplicationTitle();
}

//============================================================================
const char * IGoTv::fromGuiGetAppNameNoSpaces( void )
{
    return VxGetApplicationNameNoSpaces();
}

//============================================================================
void IGoTv::fromGuiAppStartup( const char * assetDir, const char * rootDataDir )
{
    //LogMsg( LOG_DEBUG, "IGoTv::fromGuiAppStartup.. calling fromGuiSetupContext\n");
    //if( 0 == m_AudioMgr )
    //{
    //    fromGuiSetupContext();
    //}

    getPtoP().fromGuiAppStartup( assetDir, rootDataDir );

    //if( 0 != m_AudioMgr )
    //{
    //    m_AudioMgr->toGuiWantMicrophoneRecording( true );
    //}

    //if( 0 != m_AudioMgr )
    //{
    //    m_AudioMgr->toGuiWantSpeakerOutput( true );
    //}
}

//============================================================================
void IGoTv::fromGuiKickWatchdog( void )
{
    getPtoP().fromGuiKickWatchdog();
}

//============================================================================
void IGoTv::fromGuiSetUserXferDir( const char * userDir )
{
    //LogMsg( LOG_DEBUG, "IGoTv::fromGuiSetUserXferDir %s\n", userDir );
    getPtoP().fromGuiSetUserXferDir( userDir );
}

//============================================================================
void IGoTv::fromGuiSetUserSpecificDir( const char * userDir )
{
    getPtoP().fromGuiSetUserSpecificDir( userDir );
    // needs called after user specific data folder is set
    getOsInterface().initDirectories();
}

//============================================================================
uint64_t IGoTv::fromGuiGetDiskFreeSpace( void )
{
    return getPtoP().fromGuiGetDiskFreeSpace();
}

//============================================================================
uint64_t IGoTv::fromGuiClearCache( ECacheType cacheType )
{
    return getPtoP().fromGuiClearCache( cacheType );
}

//============================================================================
void IGoTv::fromGuiAppShutdown( void )
{
    LogMsg( LOG_INFO, "fromGuiAppShutdown" );
    VxSetAppIsShuttingDown( true );

    getPtoP().fromGuiAppShutdown();
}

//============================================================================
void IGoTv::fromGuiAppIdle( void )
{
    getPtoP().fromGuiAppIdle();
}

//============================================================================
void IGoTv::fromGuiAppPauseOrResume( bool isPaused )
{
    if( isPaused )
    {
        getPtoP().fromGuiAppPause();
    }
    else
    {
        getPtoP().fromGuiAppResume();
    }
}

//============================================================================
void IGoTv::fromGuiUserLoggedOn( VxNetIdent * netIdent )
{
    getPtoP().fromGuiUserLoggedOn( netIdent );
}

//============================================================================
void IGoTv::fromGuiOnlineNameChanged( const char * newOnlineName )
{
    getPtoP().fromGuiOnlineNameChanged( newOnlineName );
}

//============================================================================
void IGoTv::fromGuiMoodMessageChanged( const char * newMoodMessage )
{
    getPtoP().fromGuiMoodMessageChanged( newMoodMessage );
}

//============================================================================
void IGoTv::fromGuiIdentPersonalInfoChanged( int age, int gender, int language, int preferredContent )
{
    getPtoP().fromGuiIdentPersonalInfoChanged( age, gender, language, preferredContent );
}

//============================================================================
void IGoTv::fromGuiSetUserHasProfilePicture( bool haveProfilePick )
{
    getPtoP().fromGuiSetUserHasProfilePicture( haveProfilePick );
}

//============================================================================
bool IGoTv::fromGuiOrientationEvent( float f32RotX, float f32RotY, float f32RotZ )
{
    return getPtoP().fromGuiOrientationEvent( f32RotX, f32RotY, f32RotZ );
}

//============================================================================
bool IGoTv::fromGuiMouseEvent( EMouseButtonType eMouseButType, EMouseEventType eMouseEventType, int iMouseXPos, int iMouseYPos )
{
    return getPtoP().fromGuiMouseEvent( eMouseButType, eMouseEventType, iMouseXPos, iMouseYPos );
}

//============================================================================
bool IGoTv::fromGuiMouseWheel( float f32MouseWheelDist )
{
    return getPtoP().fromGuiMouseWheel( f32MouseWheelDist );
}

//============================================================================
bool IGoTv::fromGuiKeyEvent( EKeyEventType eKeyEventType, EKeyCode eKey, int iFlags )
{
    return getPtoP().fromGuiKeyEvent( eKeyEventType, eKey, iFlags );
}

//============================================================================
void IGoTv::fromGuiNativeGlInit( void )
{
    getPtoP().fromGuiNativeGlInit();
}

//============================================================================
void IGoTv::fromGuiNativeGlResize( int width, int height )
{
    getPtoP().fromGuiNativeGlResize( width, height );
}

//============================================================================
int IGoTv::fromGuiNativeGlRender( void )
{
    return getPtoP().fromGuiNativeGlRender();
}

//============================================================================
void IGoTv::fromGuiNativeGlPauseRender( void )
{
    getPtoP().fromGuiNativeGlPauseRender();
}

//============================================================================
void IGoTv::fromGuiNativeGlResumeRender( void )
{
    getPtoP().fromGuiNativeGlResumeRender();
}

//============================================================================
void IGoTv::fromGuiNativeGlDestroy( void )
{
    getPtoP().fromGuiNativeGlDestroy();
}

//============================================================================
void IGoTv::fromGuiNeedMorePlayData( int16_t * retAudioSamples, int deviceReqDataLen )
{
//    getPtoP().fromGuiNeedMorePlayData( retAudioSamples, deviceReqDataLen );
}

//============================================================================
void IGoTv::fromGuiMicrophoneDataWithInfo( int16_t * pcmData, int pcmDataLenBytes, bool isSilence, int totalDelayTimeMs, int clockDrift )
{
    getPtoP().fromGuiMicrophoneDataWithInfo( pcmData, pcmDataLenBytes, isSilence, totalDelayTimeMs, clockDrift );
}

//============================================================================
void IGoTv::fromGuiMuteMicrophone( bool mute )
{
    getPtoP().fromGuiMuteMicrophone( mute );
}

//============================================================================
void IGoTv::fromGuiMuteSpeaker( bool mute )
{
    getPtoP().fromGuiMuteSpeaker( mute );
}

//============================================================================
void IGoTv::fromGuiEchoCancelEnable( bool enableEchoCancel )
{
    getPtoP().fromGuiEchoCancelEnable( enableEchoCancel );
}

//============================================================================
bool IGoTv::fromGuiIsMicrophoneMuted( void )
{
    return getPtoP().fromGuiIsMicrophoneMuted();
}

//============================================================================
bool IGoTv::fromGuiIsSpeakerMuted( void )
{
    return getPtoP().fromGuiIsSpeakerMuted();
}

//============================================================================
bool IGoTv::fromGuiIsEchoCancelEnabled( void )
{
    return getPtoP().fromGuiIsEchoCancelEnabled();
}

//============================================================================
void IGoTv::fromGuiWantMediaInput( VxGUID& id, EMediaInputType eMediaType, bool bWantInput )
{
    getPtoP().fromGuiWantMediaInput( id, eMediaType, bWantInput );
}

//============================================================================
void IGoTv::fromGuiVideoData( uint32_t u32FourCc, uint8_t * pu8VidDataIn, int iWidth, int iHeight, uint32_t u32VidDataLen, int iRotation )
{
    getPtoP().fromGuiVideoData( u32FourCc, pu8VidDataIn, iWidth, iHeight, u32VidDataLen, iRotation );
}

//============================================================================
bool IGoTv::fromGuiMovieDone( void )
{
    return getPtoP().fromGuiMovieDone();
}

//============================================================================
void IGoTv::fromGuiApplyNetHostSettings( NetHostSetting& netSettings )
{
    getPtoP().fromGuiApplyNetHostSettings( netSettings );
}

//============================================================================
void IGoTv::fromGuiGetNetSettings( NetSettings& netSettings )
{
    getPtoP().fromGuiGetNetSettings( netSettings );
}

//============================================================================
void IGoTv::fromGuiSetNetSettings( NetSettings& netSettings )
{
    getPtoP().fromGuiSetNetSettings( netSettings );
}

//============================================================================
void IGoTv::fromGuiNetworkSettingsChanged( void )
{
    getPtoP().fromGuiNetworkSettingsChanged();
}

//============================================================================
void IGoTv::fromGuiNetworkAvailable( const char * lclIp, bool isCellularNetwork )
{
    getPtoP().fromGuiNetworkAvailable( lclIp, isCellularNetwork );
}

//============================================================================
void IGoTv::fromGuiNetworkLost( void )
{
    getPtoP().fromGuiNetworkLost();
}

//============================================================================
ENetLayerState IGoTv::fromGuiGetNetLayerState( ENetLayerType netLayer )
{
    return getPtoP().fromGuiGetNetLayerState( netLayer );
}

//============================================================================
void IGoTv::fromGuiRunIsPortOpenTest( uint16_t port )
{
    getPtoP().fromGuiRunIsPortOpenTest( port );
}

//============================================================================
void IGoTv::fromGuiAnnounceHost( EHostType hostType, VxGUID& sessionId, std::string& hostUrl )
{
    getPtoP().fromGuiAnnounceHost( hostType, sessionId, hostUrl );
}

//============================================================================
void IGoTv::fromGuiJoinHost( EHostType hostType, VxGUID& sessionId, std::string& hostUrl )
{
    getPtoP().fromGuiJoinHost( hostType, sessionId, hostUrl );
}

//============================================================================
void IGoTv::fromGuiUnJoinHost( EHostType hostType, VxGUID& sessionId, std::string& hostUrl )
{
    getPtoP().fromGuiUnJoinHost( hostType, sessionId, hostUrl );
}

//============================================================================
void IGoTv::fromGuiJoinLastJoinedHost( EHostType hostType, VxGUID& sessionId )
{
    getPtoP().fromGuiJoinLastJoinedHost( hostType, sessionId );
}

//============================================================================
void IGoTv::fromGuiSearchHost( EHostType hostType, SearchParams& searchParams, bool enable )
{
    getPtoP().fromGuiSearchHost( hostType, searchParams, enable );
}

//============================================================================
void IGoTv::fromGuiSendAnnouncedList( EHostType hostType, VxGUID& sessionId )
{
    getPtoP().fromGuiSendAnnouncedList( hostType, sessionId );
}

//============================================================================
void IGoTv::fromGuiRunUrlAction( VxGUID& sessionId, const char * myUrl, const char * ptopUrl, ENetCmdType testType )
{
    getPtoP().fromGuiRunUrlAction( sessionId, myUrl, ptopUrl, testType );
}

//============================================================================
void IGoTv::fromGuiGetFileShareSettings( FileShareSettings& fileShareSettings )
{
    getPtoP().fromGuiGetFileShareSettings( fileShareSettings );
}

//============================================================================
void IGoTv::fromGuiSetFileShareSettings( FileShareSettings& fileShareSettings )
{
    getPtoP().fromGuiSetFileShareSettings( fileShareSettings );
}

//============================================================================
void IGoTv::fromGuiUpdateWebPageProfile( const char *	pProfileDir,	// directory containing user profile
                                             const char *	strGreeting,	// greeting text
                                             const char *	aboutMe,		// about me text
                                             const char *	url1,			// favorite url 1
                                             const char *	url2,			// favorite url 2
                                             const char *	url3,
                                             const char *	donation )
{
    getPtoP().fromGuiUpdateWebPageProfile( pProfileDir,	// directory containing user profile
                                          strGreeting,	// greeting text
                                          aboutMe,		// about me text
                                          url1,			// favorite url 1
                                          url2,			// favorite url 2
                                          url3,
                                          donation );
}

//============================================================================
void IGoTv::fromGuiSetPluginPermission( EPluginType ePluginType, int eFriendState )
{
    getPtoP().fromGuiSetPluginPermission( ePluginType, ( EFriendState )eFriendState );
}

//============================================================================
int IGoTv::fromGuiGetPluginPermission( EPluginType ePluginType )
{
    return ( int )getPtoP().fromGuiGetPluginPermission( ePluginType );
}

//============================================================================
int IGoTv::fromGuiGetPluginServerState( EPluginType ePluginType )
{
    return getPtoP().fromGuiGetPluginServerState( ePluginType );
}

//============================================================================
void IGoTv::fromGuiStartPluginSession( EPluginType ePluginType, VxGUID oOnlineId, int pvUserData, VxGUID lclSessionId )
{
    getPtoP().fromGuiStartPluginSession( ePluginType, oOnlineId, pvUserData, lclSessionId );
}

//============================================================================
void IGoTv::fromGuiStopPluginSession( EPluginType ePluginType, VxGUID oOnlineId, int pvUserData, VxGUID lclSessionId )
{
    getPtoP().fromGuiStopPluginSession( ePluginType, oOnlineId, pvUserData, lclSessionId );
}

//============================================================================
bool IGoTv::fromGuiIsPluginInSession( EPluginType ePluginType, VxGUID oOnlineId, int pvUserData, VxGUID lclSessionId )
{
    BigListInfo * bigListInfo = getPtoP().getBigListMgr().findBigListInfo( oOnlineId );
    if( bigListInfo )
    {
        return getPtoP().fromGuiIsPluginInSession( ePluginType, bigListInfo->getVxNetIdent(), pvUserData, lclSessionId );
    }

    return false;
}

//============================================================================
bool IGoTv::fromGuiMakePluginOffer( EPluginType		ePluginType,
                                        VxGUID&			oOnlineId,
                                        int				pvUserData,
                                        const char *	pOfferMsg,
                                        const char *	pFileName,
                                        VxGUID&			sessionId )
{
    return getPtoP().fromGuiMakePluginOffer( ePluginType,
                                            oOnlineId,
                                            pvUserData,
                                            pOfferMsg,
                                            pFileName,
                                            0,
                                            sessionId );
}

//============================================================================
bool IGoTv::fromGuiToPluginOfferReply( EPluginType		ePluginType,
                                           VxGUID&			oOnlineId,
                                           int  			pvUserData,
                                           int				iOfferResponse,
                                           VxGUID&			lclSessionId )
{
    return getPtoP().fromGuiToPluginOfferReply( ePluginType,
                                               oOnlineId,
                                               pvUserData,
                                               iOfferResponse,
                                               lclSessionId );
}

//============================================================================
int IGoTv::fromGuiPluginControl( EPluginType		ePluginType,
                                     VxGUID&			oOnlineId,
                                     const char *	pControl,
                                     const char *	pAction,
                                     uint32_t				u32ActionData,
                                     VxGUID&			lclSessionId,
                                     uint8_t *			fileHashData )
{
    return getPtoP().fromGuiPluginControl( ePluginType,
                                          oOnlineId,
                                          pControl,
                                          pAction,
                                          u32ActionData,
                                          lclSessionId,
                                          fileHashData );
}

//============================================================================
bool IGoTv::fromGuiInstMsg( EPluginType		ePluginType,
                                VxGUID&			oOnlineId,
                                const char *	pMsg )
{
    return getPtoP().fromGuiInstMsg( ePluginType, oOnlineId, pMsg );
}

//============================================================================
bool IGoTv::fromGuiPushToTalk( VxGUID& onlineId, bool enableTalk )
{
    return getPtoP().fromGuiPushToTalk( onlineId, enableTalk );
}

//============================================================================
bool IGoTv::fromGuiChangeMyFriendshipToHim( VxGUID&			oOnlineId,
                                                EFriendState	eMyFriendshipToHim,
                                                EFriendState	eHisFriendshipToMe )
{
    return getPtoP().fromGuiChangeMyFriendshipToHim( oOnlineId, eMyFriendshipToHim, eHisFriendshipToMe );
}

//============================================================================
void IGoTv::fromGuiSendContactList( EFriendViewType eFriendView, int maxContactsToSend )
{
    getPtoP().fromGuiSendContactList( eFriendView, maxContactsToSend );
}

//============================================================================
void IGoTv::fromGuiRefreshContactList( int maxContactsToSend )
{
    getPtoP().fromGuiRefreshContactList( maxContactsToSend );
}

//============================================================================
void IGoTv::fromGuiRequireRelay( bool bRequireRelay )
{
    getPtoP().fromGuiRequireRelay( bRequireRelay );
}

//============================================================================
void IGoTv::fromGuiUseRelay( VxGUID& oOnlineId, bool bUseAsRelay )
{
    getPtoP().fromGuiUseRelay( oOnlineId, bUseAsRelay );
}

//============================================================================
void IGoTv::fromGuiRelayPermissionCount( int userPermittedCount, int anonymousCount )
{
    getPtoP().fromGuiRelayPermissionCount( userPermittedCount, anonymousCount );
}

//============================================================================
void IGoTv::fromGuiStartScan( EScanType eScanType, uint8_t searchFlags, uint8_t fileTypeFlags, const char * pSearchPattern )
{
    getPtoP().fromGuiStartScan( eScanType, searchFlags, fileTypeFlags, pSearchPattern );
}

//============================================================================
void IGoTv::fromGuiNextScan( EScanType eScanType )
{
    getPtoP().fromGuiNextScan( eScanType );
}

//============================================================================
void IGoTv::fromGuiStopScan( EScanType eScanType )
{
    getPtoP().fromGuiStopScan( eScanType );
}

//============================================================================
InetAddress IGoTv::fromGuiGetMyIPv4Address( void )
{
    return getPtoP().fromGuiGetMyIPv4Address();
}

//============================================================================
InetAddress IGoTv::fromGuiGetMyIPv6Address( void )
{
    return getPtoP().fromGuiGetMyIPv6Address();
}

//============================================================================
void IGoTv::fromGuiUpdateMyIdent( VxNetIdent * netIdent, bool permissionAndStatsOnly )
{
    getPtoP().fromGuiUpdateMyIdent( netIdent, permissionAndStatsOnly );
}

//============================================================================
void IGoTv::fromGuiQueryMyIdent( VxNetIdent * poRetIdent )
{
    getPtoP().fromGuiQueryMyIdent( poRetIdent );
}

//============================================================================
void IGoTv::fromGuiSetIdentHasTextOffers( VxGUID& onlineId, bool hasTextOffers )
{
    getPtoP().fromGuiSetIdentHasTextOffers( onlineId, hasTextOffers );
}

//============================================================================
void IGoTv::fromGuiUserModifiedStoryboard( void )
{
    getPtoP().fromGuiUserModifiedStoryboard();
}

//============================================================================
void IGoTv::fromGuiCancelDownload( VxGUID& fileInstanceId )
{
    getPtoP().fromGuiCancelDownload( fileInstanceId );
}

//============================================================================
void IGoTv::fromGuiCancelUpload( VxGUID& fileInstanceId )
{
    getPtoP().fromGuiCancelUpload( fileInstanceId );
}

//============================================================================
bool IGoTv::fromGuiSetGameValueVar( EPluginType	ePluginType,
                                        VxGUID&	oOnlineId,
                                        int32_t			s32VarId,
                                        int32_t			s32VarValue )
{
    return getPtoP().fromGuiSetGameValueVar( ePluginType,
                                            oOnlineId,
                                            s32VarId,
                                            s32VarValue );
}

//============================================================================
bool IGoTv::fromGuiSetGameActionVar( EPluginType	ePluginType,
                                         VxGUID&	oOnlineId,
                                         int32_t			s32VarId,
                                         int32_t			s32VarValue )
{
    return getPtoP().fromGuiSetGameActionVar( ePluginType,
                                             oOnlineId,
                                             s32VarId,
                                             s32VarValue );
}

//============================================================================
bool IGoTv::fromGuiTestCmd( IFromGui::ETestParam1		eTestParam1,
                                int							testParam2,
                                const char *				testParam3 )
{
    return getPtoP().fromGuiTestCmd( eTestParam1, testParam2, testParam3 );
}

//============================================================================
bool IGoTv::fromGuiNearbyBroadcastEnable( bool enable )
{
    return getPtoP().fromGuiNearbyBroadcastEnable( enable );
}

//============================================================================
void IGoTv::fromGuiDebugSettings( uint32_t u32LogFlags, const char *	pLogFileName )
{
    getPtoP().fromGuiDebugSettings( u32LogFlags, pLogFileName );
}

//============================================================================
void IGoTv::fromGuiSendLog( uint32_t u32LogFlags )
{
    getPtoP().fromGuiSendLog( u32LogFlags );
}

//============================================================================
bool IGoTv::fromGuiBrowseFiles( const char * dir, bool lookupShareStatus, uint8_t fileFilterMask )
{
    return getPtoP().fromGuiBrowseFiles( dir, lookupShareStatus, fileFilterMask );
}

//============================================================================
bool IGoTv::fromGuiGetSharedFiles( uint8_t fileTypeFilter )
{
    return getPtoP().fromGuiGetSharedFiles( fileTypeFilter );
}

//============================================================================
bool IGoTv::fromGuiSetFileIsShared( const char * fileName, bool isShared )
{
    return getPtoP().fromGuiSetFileIsShared( fileName, isShared );
}

//============================================================================
bool IGoTv::fromGuiGetIsFileShared( const char * fileName )
{
    return getPtoP().fromGuiGetIsFileShared( fileName );
}

//============================================================================
int IGoTv::fromGuiGetFileDownloadState( uint8_t * fileHashId )
{
    return getPtoP().fromGuiGetFileDownloadState( fileHashId );
}

//============================================================================
bool IGoTv::fromGuiAddFileToLibrary( const char * fileName, bool addFile, uint8_t * fileHashId )
{
    return getPtoP().fromGuiAddFileToLibrary( fileName, addFile, fileHashId );
}

//============================================================================
void IGoTv::fromGuiGetFileLibraryList( uint8_t fileTypeFilter )
{
    getPtoP().fromGuiGetFileLibraryList( fileTypeFilter );
}

//============================================================================
bool IGoTv::fromGuiGetIsFileInLibrary( const char * fileName )
{
    return getPtoP().fromGuiGetIsFileInLibrary( fileName );
}

//============================================================================
bool IGoTv::fromGuiIsMyP2PWebVideoFile( const char * fileName )
{
    return getPtoP().fromGuiIsMyP2PWebVideoFile( fileName );
}

//============================================================================
bool IGoTv::fromGuiIsMyP2PWebAudioFile( const char * fileName )
{
    return getPtoP().fromGuiIsMyP2PWebAudioFile( fileName );
}

//============================================================================
int IGoTv::fromGuiDeleteFile( const char * fileName, bool shredFile )
{
    return getPtoP().fromGuiDeleteFile( fileName, shredFile );
}

//============================================================================
bool IGoTv::fromGuiAssetAction( EAssetAction assetAction, AssetBaseInfo& assetInfo, int pos0to100000 )
{
    return getPtoP().fromGuiAssetAction( assetAction, assetInfo, pos0to100000 );
}

//============================================================================
bool IGoTv::fromGuiAssetAction( EAssetAction assetAction, VxGUID& assetId, int pos0to100000 )
{
    return getPtoP().fromGuiAssetAction( assetAction, assetId, pos0to100000 );
}

//============================================================================
bool IGoTv::fromGuiSendAsset( AssetBaseInfo& assetInfo ) 
{
    return getPtoP().fromGuiSendAsset( assetInfo );
}

//============================================================================
bool IGoTv::fromGuiVideoRecord( EVideoRecordState eRecState, VxGUID& feedId, const char * fileName )
{
    return getPtoP().fromGuiVideoRecord( eRecState, feedId, fileName );
}

//============================================================================
bool IGoTv::fromGuiSndRecord( ESndRecordState eRecState, VxGUID& feedId, const char * fileName )
{
    return getPtoP().fromGuiSndRecord( eRecState, feedId, fileName );
}

//============================================================================
void IGoTv::fromGuiQuerySessionHistory( VxGUID& historyId )
{
    getPtoP().fromGuiQuerySessionHistory( historyId );
}

//============================================================================
bool IGoTv::fromGuiMultiSessionAction( EMSessionAction mSessionAction, VxGUID& onlineId, int pos0to100000 )
{
    return getPtoP().fromGuiMultiSessionAction( mSessionAction, onlineId, pos0to100000 );
}

//============================================================================
int IGoTv::fromGuiGetJoinedListCount( EPluginType pluginType )
{
    return getPtoP().fromGuiGetJoinedListCount( pluginType );
}

//============================================================================
void IGoTv::fromGuiListAction( EListAction listAction )
{
    getPtoP().fromGuiListAction( listAction );
}

//============================================================================
std::string IGoTv::fromGuiQueryDefaultUrl( EHostType hostType )
{
    return getPtoP().fromGuiQueryDefaultUrl( hostType );
}

//============================================================================
bool IGoTv::fromGuiSetDefaultUrl( EHostType hostType, std::string& hostUrl )
{
    return getPtoP().fromGuiSetDefaultUrl( hostType, hostUrl );
}

//============================================================================
bool IGoTv::fromGuiQueryIdentity( std::string& url, VxNetIdent& retNetIdent, bool requestIdentityIfUnknown )
{
    return getPtoP().fromGuiQueryIdentity( url, retNetIdent, requestIdentityIfUnknown );
}

//============================================================================
bool IGoTv::fromGuiQueryHosts( std::string& netHostUrl, EHostType hostType, std::vector<HostedInfo>& hostedInfoList, VxGUID& hostIdIfNullThenAll )
{
    return getPtoP().fromGuiQueryHosts( netHostUrl, hostType, hostedInfoList, hostIdIfNullThenAll );
}

//============================================================================
bool IGoTv::fromGuiQueryMyHostedInfo( EHostType hostType, std::vector<HostedInfo>& hostedInfoList )
{
    return getPtoP().fromGuiQueryMyHostedInfo( hostType, hostedInfoList );
}

//============================================================================
bool IGoTv::fromGuiQueryHostListFromNetworkHost( VxPtopUrl& netHostUrl, EHostType hostType, VxGUID& hostIdIfNullThenAll )
{
    return getPtoP().fromGuiQueryHostListFromNetworkHost( netHostUrl, hostType, hostIdIfNullThenAll );
}

//============================================================================
bool IGoTv::fromGuiQueryGroupiesFromHosted( VxPtopUrl& hostedUrl, EHostType hostType, VxGUID& onlineIdIfNullThenAll )
{
    return getPtoP().fromGuiQueryGroupiesFromHosted( hostedUrl, hostType, onlineIdIfNullThenAll );
}