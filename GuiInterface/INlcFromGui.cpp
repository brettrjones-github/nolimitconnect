#include <CommonHdr.h>
#include "INlc.h"
#include "ptop_src/ptop_engine_src/P2PEngine/P2PEngine.h"
#include <ptop_src/ptop_engine_src/BigListLib/BigListInfo.h>

#include <CoreLib/VxGlobals.h>

//============================================================================
//=== from gui ===//
//============================================================================
void INlc::fromGuiSetupContext( void )
{
    //LogMsg( LOG_DEBUG, "INlc::fromGuiSetupContext\n");
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
void INlc::fromGuiSetIsAppCommercial( bool isCommercial )
{
    VxSetIsApplicationCommercial( isCommercial );
}

//============================================================================
bool INlc::fromGuiGetIsAppCommercial( void )
{
    return VxGetIsApplicationCommercial();
}

//============================================================================
uint16_t INlc::fromGuiGetAppVersionBinary( void )
{
    return VxGetAppVersion();
}

//============================================================================
const char * INlc::fromGuiGetAppVersionString( void )
{
    return VxGetAppVersionString();
}

//============================================================================
const char * INlc::fromGuiGetAppName( void )
{
    return VxGetApplicationTitle();
}

//============================================================================
const char * INlc::fromGuiGetAppNameNoSpaces( void )
{
    return VxGetApplicationNameNoSpaces();
}

//============================================================================
void INlc::fromGuiAppStartup( const char * assetDir, const char * rootDataDir )
{
    //LogMsg( LOG_DEBUG, "INlc::fromGuiAppStartup.. calling fromGuiSetupContext\n");
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
void INlc::fromGuiKickWatchdog( void )
{
    getPtoP().fromGuiKickWatchdog();
}

//============================================================================
void INlc::fromGuiSetUserXferDir( const char * userDir )
{
    //LogMsg( LOG_DEBUG, "INlc::fromGuiSetUserXferDir %s\n", userDir );
    getPtoP().fromGuiSetUserXferDir( userDir );
}

//============================================================================
void INlc::fromGuiSetUserSpecificDir( const char * userDir )
{
    getPtoP().fromGuiSetUserSpecificDir( userDir );
    // needs called after user specific data folder is set
    getOsInterface().initDirectories();
}

//============================================================================
uint64_t INlc::fromGuiGetDiskFreeSpace( void )
{
    return getPtoP().fromGuiGetDiskFreeSpace();
}

//============================================================================
uint64_t INlc::fromGuiClearCache( ECacheType cacheType )
{
    return getPtoP().fromGuiClearCache( cacheType );
}

//============================================================================
void INlc::fromGuiAppShutdown( void )
{
    LogMsg( LOG_INFO, "fromGuiAppShutdown" );
    VxSetAppIsShuttingDown( true );

    getPtoP().fromGuiAppShutdown();
}

//============================================================================
void INlc::fromGuiAppPauseOrResume( bool isPaused )
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
void INlc::fromGuiUserLoggedOn( VxNetIdent * netIdent )
{
    getPtoP().fromGuiUserLoggedOn( netIdent );
}

//============================================================================
void INlc::fromGuiOnlineNameChanged( const char * newOnlineName )
{
    getPtoP().fromGuiOnlineNameChanged( newOnlineName );
}

//============================================================================
void INlc::fromGuiMoodMessageChanged( const char * newMoodMessage )
{
    getPtoP().fromGuiMoodMessageChanged( newMoodMessage );
}

//============================================================================
void INlc::fromGuiIdentPersonalInfoChanged( int age, int gender, int language, int preferredContent )
{
    getPtoP().fromGuiIdentPersonalInfoChanged( age, gender, language, preferredContent );
}

//============================================================================
void INlc::fromGuiSetUserHasProfilePicture( bool haveProfilePick )
{
    getPtoP().fromGuiSetUserHasProfilePicture( haveProfilePick );
}

//============================================================================
bool INlc::fromGuiOrientationEvent( float f32RotX, float f32RotY, float f32RotZ )
{
    return getPtoP().fromGuiOrientationEvent( f32RotX, f32RotY, f32RotZ );
}

//============================================================================
bool INlc::fromGuiMouseEvent( EMouseButtonType eMouseButType, EMouseEventType eMouseEventType, int iMouseXPos, int iMouseYPos )
{
    return getPtoP().fromGuiMouseEvent( eMouseButType, eMouseEventType, iMouseXPos, iMouseYPos );
}

//============================================================================
bool INlc::fromGuiMouseWheel( float f32MouseWheelDist )
{
    return getPtoP().fromGuiMouseWheel( f32MouseWheelDist );
}

//============================================================================
bool INlc::fromGuiKeyEvent( EKeyEventType eKeyEventType, EKeyCode eKey, int iFlags )
{
    return getPtoP().fromGuiKeyEvent( eKeyEventType, eKey, iFlags );
}

//============================================================================
void INlc::fromGuiNativeGlInit( void )
{
    getPtoP().fromGuiNativeGlInit();
}

//============================================================================
void INlc::fromGuiNativeGlResize( int width, int height )
{
    getPtoP().fromGuiNativeGlResize( width, height );
}

//============================================================================
int INlc::fromGuiNativeGlRender( void )
{
    return getPtoP().fromGuiNativeGlRender();
}

//============================================================================
void INlc::fromGuiNativeGlPauseRender( void )
{
    getPtoP().fromGuiNativeGlPauseRender();
}

//============================================================================
void INlc::fromGuiNativeGlResumeRender( void )
{
    getPtoP().fromGuiNativeGlResumeRender();
}

//============================================================================
void INlc::fromGuiNativeGlDestroy( void )
{
    getPtoP().fromGuiNativeGlDestroy();
}

//============================================================================
void INlc::fromGuiNeedMorePlayData( int16_t * retAudioSamples, int deviceReqDataLen )
{
//    getPtoP().fromGuiNeedMorePlayData( retAudioSamples, deviceReqDataLen );
}

//============================================================================
void INlc::fromGuiMuteMicrophone( bool mute )
{
    getPtoP().fromGuiMuteMicrophone( mute );
}

//============================================================================
void INlc::fromGuiMuteSpeaker( bool mute )
{
    getPtoP().fromGuiMuteSpeaker( mute );
}

//============================================================================
bool INlc::fromGuiIsMicrophoneMuted( void )
{
    return getPtoP().fromGuiIsMicrophoneMuted();
}

//============================================================================
bool INlc::fromGuiIsSpeakerMuted( void )
{
    return getPtoP().fromGuiIsSpeakerMuted();
}

//============================================================================
void INlc::fromGuiWantMediaInput( VxGUID& id, EMediaInputType eMediaType, EAppModule appModule, bool wantInput )
{
    getPtoP().fromGuiWantMediaInput( id, eMediaType, appModule, wantInput );
}

//============================================================================
void INlc::fromGuiVideoData( uint32_t u32FourCc, uint8_t * pu8VidDataIn, int iWidth, int iHeight, uint32_t u32VidDataLen, int iRotation )
{
    getPtoP().fromGuiVideoData( u32FourCc, pu8VidDataIn, iWidth, iHeight, u32VidDataLen, iRotation );
}

//============================================================================
bool INlc::fromGuiMovieDone( void )
{
    return getPtoP().fromGuiMovieDone();
}

//============================================================================
void INlc::fromGuiApplyNetHostSettings( NetHostSetting& netSettings )
{
    getPtoP().fromGuiApplyNetHostSettings( netSettings );
}

//============================================================================
void INlc::fromGuiGetNetSettings( NetSettings& netSettings )
{
    getPtoP().fromGuiGetNetSettings( netSettings );
}

//============================================================================
void INlc::fromGuiSetNetSettings( NetSettings& netSettings )
{
    getPtoP().fromGuiSetNetSettings( netSettings );
}

//============================================================================
void INlc::fromGuiNetworkSettingsChanged( void )
{
    getPtoP().fromGuiNetworkSettingsChanged();
}

//============================================================================
void INlc::fromGuiNetworkAvailable( const char * lclIp, bool isCellularNetwork )
{
    getPtoP().fromGuiNetworkAvailable( lclIp, isCellularNetwork );
}

//============================================================================
void INlc::fromGuiNetworkLost( void )
{
    getPtoP().fromGuiNetworkLost();
}

//============================================================================
ENetLayerState INlc::fromGuiGetNetLayerState( ENetLayerType netLayer )
{
    return getPtoP().fromGuiGetNetLayerState( netLayer );
}

//============================================================================
void INlc::fromGuiRunIsPortOpenTest( uint16_t port )
{
    getPtoP().fromGuiRunIsPortOpenTest( port );
}

//============================================================================
void INlc::fromGuiAnnounceHost( EHostType hostType, VxGUID& sessionId, std::string& hostUrl )
{
    getPtoP().fromGuiAnnounceHost( hostType, sessionId, hostUrl );
}

//============================================================================
void INlc::fromGuiJoinHost( EHostType hostType, VxGUID& sessionId, std::string& hostUrl )
{
    getPtoP().fromGuiJoinHost( hostType, sessionId, hostUrl );
}

//============================================================================
void INlc::fromGuiLeaveHost( EHostType hostType, VxGUID& sessionId, std::string& hostUrl )
{
    getPtoP().fromGuiLeaveHost( hostType, sessionId, hostUrl );
}

//============================================================================
void INlc::fromGuiUnJoinHost( EHostType hostType, VxGUID& sessionId, std::string& hostUrl )
{
    getPtoP().fromGuiUnJoinHost( hostType, sessionId, hostUrl );
}

//============================================================================
void INlc::fromGuiJoinLastJoinedHost( EHostType hostType, VxGUID& sessionId )
{
    getPtoP().fromGuiJoinLastJoinedHost( hostType, sessionId );
}

//============================================================================
void INlc::fromGuiSearchHost( EHostType hostType, SearchParams& searchParams, bool enable )
{
    getPtoP().fromGuiSearchHost( hostType, searchParams, enable );
}

//============================================================================
void INlc::fromGuiSendAnnouncedList( EHostType hostType, VxGUID& sessionId )
{
    getPtoP().fromGuiSendAnnouncedList( hostType, sessionId );
}

//============================================================================
void INlc::fromGuiRunUrlAction( VxGUID& sessionId, const char * myUrl, const char * ptopUrl, ENetCmdType testType )
{
    getPtoP().fromGuiRunUrlAction( sessionId, myUrl, ptopUrl, testType );
}

//============================================================================
void INlc::fromGuiGetFileShareSettings( FileShareSettings& fileShareSettings )
{
    getPtoP().fromGuiGetFileShareSettings( fileShareSettings );
}

//============================================================================
void INlc::fromGuiSetFileShareSettings( FileShareSettings& fileShareSettings )
{
    getPtoP().fromGuiSetFileShareSettings( fileShareSettings );
}

//============================================================================
void INlc::fromGuiUpdateWebPageProfile( const char *	pProfileDir,	// directory containing user profile
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
void INlc::fromGuiSetPluginPermission( EPluginType ePluginType, int eFriendState )
{
    getPtoP().fromGuiSetPluginPermission( ePluginType, ( EFriendState )eFriendState );
}

//============================================================================
int INlc::fromGuiGetPluginPermission( EPluginType ePluginType )
{
    return ( int )getPtoP().fromGuiGetPluginPermission( ePluginType );
}

//============================================================================
int INlc::fromGuiGetPluginServerState( EPluginType ePluginType )
{
    return getPtoP().fromGuiGetPluginServerState( ePluginType );
}

//============================================================================
void INlc::fromGuiStartPluginSession( EPluginType ePluginType, VxGUID oOnlineId, int pvUserData, VxGUID lclSessionId )
{
    getPtoP().fromGuiStartPluginSession( ePluginType, oOnlineId, pvUserData, lclSessionId );
}

//============================================================================
void INlc::fromGuiStopPluginSession( EPluginType ePluginType, VxGUID oOnlineId, int pvUserData, VxGUID lclSessionId )
{
    getPtoP().fromGuiStopPluginSession( ePluginType, oOnlineId, pvUserData, lclSessionId );
}

//============================================================================
bool INlc::fromGuiIsPluginInSession( EPluginType ePluginType, VxGUID oOnlineId, int pvUserData, VxGUID lclSessionId )
{
    VxNetIdent* netIdent = getPtoP().getBigListMgr().findNetIdent( oOnlineId );
    if( netIdent )
    {
        return getPtoP().fromGuiIsPluginInSession( ePluginType, netIdent, pvUserData, lclSessionId );
    }

    return false;
}

//============================================================================
bool INlc::fromGuiMakePluginOffer( EPluginType		ePluginType,
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
bool INlc::fromGuiToPluginOfferReply( EPluginType		ePluginType,
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
int INlc::fromGuiPluginControl( EPluginType		ePluginType,
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
bool INlc::fromGuiInstMsg( EPluginType		ePluginType,
                                VxGUID&			oOnlineId,
                                const char *	pMsg )
{
    return getPtoP().fromGuiInstMsg( ePluginType, oOnlineId, pMsg );
}

//============================================================================
bool INlc::fromGuiPushToTalk( VxGUID& onlineId, bool enableTalk )
{
    return getPtoP().fromGuiPushToTalk( onlineId, enableTalk );
}

//============================================================================
bool INlc::fromGuiChangeMyFriendshipToHim( VxGUID&			oOnlineId,
                                                EFriendState	eMyFriendshipToHim,
                                                EFriendState	eHisFriendshipToMe )
{
    return getPtoP().fromGuiChangeMyFriendshipToHim( oOnlineId, eMyFriendshipToHim, eHisFriendshipToMe );
}

//============================================================================
void INlc::fromGuiSendContactList( EFriendViewType eFriendView, int maxContactsToSend )
{
    getPtoP().fromGuiSendContactList( eFriendView, maxContactsToSend );
}

//============================================================================
void INlc::fromGuiRefreshContactList( int maxContactsToSend )
{
    getPtoP().fromGuiRefreshContactList( maxContactsToSend );
}

//============================================================================
void INlc::fromGuiRequireRelay( bool bRequireRelay )
{
    getPtoP().fromGuiRequireRelay( bRequireRelay );
}

//============================================================================
void INlc::fromGuiUseRelay( VxGUID& oOnlineId, bool bUseAsRelay )
{
    getPtoP().fromGuiUseRelay( oOnlineId, bUseAsRelay );
}

//============================================================================
void INlc::fromGuiRelayPermissionCount( int userPermittedCount, int anonymousCount )
{
    getPtoP().fromGuiRelayPermissionCount( userPermittedCount, anonymousCount );
}

//============================================================================
void INlc::fromGuiStartScan( EScanType eScanType, uint8_t searchFlags, uint8_t fileTypeFlags, const char * pSearchPattern )
{
    getPtoP().fromGuiStartScan( eScanType, searchFlags, fileTypeFlags, pSearchPattern );
}

//============================================================================
void INlc::fromGuiNextScan( EScanType eScanType )
{
    getPtoP().fromGuiNextScan( eScanType );
}

//============================================================================
void INlc::fromGuiStopScan( EScanType eScanType )
{
    getPtoP().fromGuiStopScan( eScanType );
}

//============================================================================
InetAddress INlc::fromGuiGetMyIPv4Address( void )
{
    return getPtoP().fromGuiGetMyIPv4Address();
}

//============================================================================
InetAddress INlc::fromGuiGetMyIPv6Address( void )
{
    return getPtoP().fromGuiGetMyIPv6Address();
}

//============================================================================
void INlc::fromGuiUpdateMyIdent( VxNetIdent * netIdent, bool permissionAndStatsOnly )
{
    getPtoP().fromGuiUpdateMyIdent( netIdent, permissionAndStatsOnly );
}

//============================================================================
void INlc::fromGuiQueryMyIdent( VxNetIdent * poRetIdent )
{
    getPtoP().fromGuiQueryMyIdent( poRetIdent );
}

//============================================================================
void INlc::fromGuiSetIdentHasTextOffers( VxGUID& onlineId, bool hasTextOffers )
{
    getPtoP().fromGuiSetIdentHasTextOffers( onlineId, hasTextOffers );
}

//============================================================================
void INlc::fromGuiUserModifiedStoryboard( void )
{
    getPtoP().fromGuiUserModifiedStoryboard();
}

//============================================================================
void INlc::fromGuiCancelDownload( VxGUID& fileInstanceId )
{
    getPtoP().fromGuiCancelDownload( fileInstanceId );
}

//============================================================================
void INlc::fromGuiCancelUpload( VxGUID& fileInstanceId )
{
    getPtoP().fromGuiCancelUpload( fileInstanceId );
}

//============================================================================
bool INlc::fromGuiSetGameValueVar( EPluginType	ePluginType,
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
bool INlc::fromGuiSetGameActionVar( EPluginType	ePluginType,
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
bool INlc::fromGuiTestCmd( IFromGui::ETestParam1		eTestParam1,
                                int							testParam2,
                                const char *				testParam3 )
{
    return getPtoP().fromGuiTestCmd( eTestParam1, testParam2, testParam3 );
}

//============================================================================
bool INlc::fromGuiNearbyBroadcastEnable( bool enable )
{
    return getPtoP().fromGuiNearbyBroadcastEnable( enable );
}

//============================================================================
void INlc::fromGuiDebugSettings( uint32_t u32LogFlags, const char *	pLogFileName )
{
    getPtoP().fromGuiDebugSettings( u32LogFlags, pLogFileName );
}

//============================================================================
void INlc::fromGuiSendLog( uint32_t u32LogFlags )
{
    getPtoP().fromGuiSendLog( u32LogFlags );
}

//============================================================================
bool INlc::fromGuiBrowseFiles( const char * dir, bool lookupShareStatus, uint8_t fileFilterMask )
{
    return getPtoP().fromGuiBrowseFiles( dir, lookupShareStatus, fileFilterMask );
}

//============================================================================
bool INlc::fromGuiGetSharedFiles( uint8_t fileTypeFilter )
{
    return getPtoP().fromGuiGetSharedFiles( fileTypeFilter );
}

//============================================================================
bool INlc::fromGuiSetFileIsShared( const char * fileName, bool isShared )
{
    return getPtoP().fromGuiSetFileIsShared( fileName, isShared );
}

//============================================================================
bool INlc::fromGuiGetIsFileShared( const char * fileName )
{
    return getPtoP().fromGuiGetIsFileShared( fileName );
}

//============================================================================
int INlc::fromGuiGetFileDownloadState( uint8_t * fileHashId )
{
    return getPtoP().fromGuiGetFileDownloadState( fileHashId );
}

//============================================================================
bool INlc::fromGuiAddFileToLibrary( const char * fileName, bool addFile, uint8_t * fileHashId )
{
    return getPtoP().fromGuiAddFileToLibrary( fileName, addFile, fileHashId );
}

//============================================================================
void INlc::fromGuiGetFileLibraryList( uint8_t fileTypeFilter )
{
    getPtoP().fromGuiGetFileLibraryList( fileTypeFilter );
}

//============================================================================
bool INlc::fromGuiGetIsFileInLibrary( const char * fileName )
{
    return getPtoP().fromGuiGetIsFileInLibrary( fileName );
}

//============================================================================
bool INlc::fromGuiIsMyP2PWebVideoFile( const char * fileName )
{
    return getPtoP().fromGuiIsMyP2PWebVideoFile( fileName );
}

//============================================================================
bool INlc::fromGuiIsMyP2PWebAudioFile( const char * fileName )
{
    return getPtoP().fromGuiIsMyP2PWebAudioFile( fileName );
}

//============================================================================
int INlc::fromGuiDeleteFile( const char * fileName, bool shredFile )
{
    return getPtoP().fromGuiDeleteFile( fileName, shredFile );
}

//============================================================================
bool INlc::fromGuiAssetAction( EAssetAction assetAction, AssetBaseInfo& assetInfo, int pos0to100000 )
{
    return getPtoP().fromGuiAssetAction( assetAction, assetInfo, pos0to100000 );
}

//============================================================================
bool INlc::fromGuiAssetAction( EAssetAction assetAction, VxGUID& assetId, int pos0to100000 )
{
    return getPtoP().fromGuiAssetAction( assetAction, assetId, pos0to100000 );
}

//============================================================================
bool INlc::fromGuiSendAsset( AssetBaseInfo& assetInfo ) 
{
    return getPtoP().fromGuiSendAsset( assetInfo );
}

//============================================================================
bool INlc::fromGuiVideoRecord( EVideoRecordState eRecState, VxGUID& feedId, const char * fileName )
{
    return getPtoP().fromGuiVideoRecord( eRecState, feedId, fileName );
}

//============================================================================
bool INlc::fromGuiSndRecord( ESndRecordState eRecState, VxGUID& feedId, const char * fileName )
{
    return getPtoP().fromGuiSndRecord( eRecState, feedId, fileName );
}

//============================================================================
void INlc::fromGuiQuerySessionHistory( VxGUID& historyId )
{
    getPtoP().fromGuiQuerySessionHistory( historyId );
}

//============================================================================
bool INlc::fromGuiMultiSessionAction( EMSessionAction mSessionAction, VxGUID& onlineId, int pos0to100000 )
{
    return getPtoP().fromGuiMultiSessionAction( mSessionAction, onlineId, pos0to100000 );
}

//============================================================================
int INlc::fromGuiGetJoinedListCount( EPluginType pluginType )
{
    return getPtoP().fromGuiGetJoinedListCount( pluginType );
}

//============================================================================
void INlc::fromGuiListAction( EListAction listAction )
{
    getPtoP().fromGuiListAction( listAction );
}

//============================================================================
std::string INlc::fromGuiQueryDefaultUrl( EHostType hostType )
{
    return getPtoP().fromGuiQueryDefaultUrl( hostType );
}

//============================================================================
bool INlc::fromGuiSetDefaultUrl( EHostType hostType, std::string& hostUrl )
{
    return getPtoP().fromGuiSetDefaultUrl( hostType, hostUrl );
}

//============================================================================
bool INlc::fromGuiQueryIdentity( std::string& url, VxNetIdent& retNetIdent, bool requestIdentityIfUnknown )
{
    return getPtoP().fromGuiQueryIdentity( url, retNetIdent, requestIdentityIfUnknown );
}

//============================================================================
bool INlc::fromGuiQueryIdentity( GroupieInfo& groupieInfo, VxNetIdent& retNetIdent, bool requestIdentityIfUnknown )
{
    return getPtoP().fromGuiQueryIdentity( groupieInfo, retNetIdent, requestIdentityIfUnknown );
}

//============================================================================
bool INlc::fromGuiQueryHosts( std::string& netHostUrl, EHostType hostType, std::vector<HostedInfo>& hostedInfoList, VxGUID& hostIdIfNullThenAll )
{
    return getPtoP().fromGuiQueryHosts( netHostUrl, hostType, hostedInfoList, hostIdIfNullThenAll );
}

//============================================================================
bool INlc::fromGuiQueryMyHostedInfo( EHostType hostType, std::vector<HostedInfo>& hostedInfoList )
{
    return getPtoP().fromGuiQueryMyHostedInfo( hostType, hostedInfoList );
}

//============================================================================
bool INlc::fromGuiQueryHostListFromNetworkHost( VxPtopUrl& netHostUrl, EHostType hostType, VxGUID& hostIdIfNullThenAll )
{
    return getPtoP().fromGuiQueryHostListFromNetworkHost( netHostUrl, hostType, hostIdIfNullThenAll );
}

//============================================================================
bool INlc::fromGuiQueryGroupiesFromHosted( VxPtopUrl& hostedUrl, EHostType hostType, VxGUID& onlineIdIfNullThenAll )
{
    return getPtoP().fromGuiQueryGroupiesFromHosted( hostedUrl, hostType, onlineIdIfNullThenAll );
}

//============================================================================
bool INlc::fromGuiDownloadWebPage( EWebPageType webPageType, VxGUID& onlineId )
{
    return getPtoP().fromGuiDownloadWebPage( webPageType, onlineId );
}

//============================================================================
bool INlc::fromGuiCancelWebPage( EWebPageType webPageType, VxGUID& onlineId )
{
    return getPtoP().fromGuiCancelWebPage( webPageType, onlineId );
}
