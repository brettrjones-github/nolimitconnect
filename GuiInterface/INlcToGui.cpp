#include <CommonHdr.h>

#include "INlc.h"
#include "AppsSrc/CommonSrc/QtSource/AppCommon.h"

//============================================================================
void INlc::toGuiLog( int logLevel, const char * msg )
{
    getAppCommon().toGuiLog( logLevel, msg );
}

//============================================================================
void INlc::toGuiAppErr( EAppErr eAppErr, const char * errMsg )
{
    getAppCommon().toGuiAppErr( eAppErr, errMsg );
}

//============================================================================
void INlc::toGuiStatusMessage( const char * statusMsg )
{
    getAppCommon().toGuiStatusMessage( statusMsg );
}

//============================================================================
void INlc::toGuiPluginMsg( EPluginType pluginType, VxGUID& onlineId, EPluginMsgType msgType, const char* paramMsg, ... )
{
    std::string paramValue{ "" };
    if( paramMsg )
    {
        char szBuffer[2048];
        szBuffer[0] = 0;
        va_list arg_ptr;
        va_start( arg_ptr, paramMsg );
#ifdef TARGET_OS_WINDOWS
        vsnprintf( szBuffer, 2048, paramMsg, ( char* )arg_ptr );
#else
        vsnprintf( szBuffer, 2048, paramMsg, arg_ptr );
#endif //  TARGET_OS_WINDOWS
        szBuffer[2047] = 0;
        va_end( arg_ptr );
        if( 0 != szBuffer[0] )
        {
            paramValue = szBuffer;
        }
    }

    getAppCommon().toGuiPluginMsg( pluginType, onlineId, msgType, paramValue );
}

//============================================================================
void INlc::toGuiPluginMsg( EPluginType pluginType, VxGUID& onlineId, EPluginMsgType msgType, std::string& paramMsg )
{
    getAppCommon().toGuiPluginMsg( pluginType, onlineId, msgType, paramMsg );
}

//============================================================================
void INlc::toGuiPluginCommError( EPluginType pluginType, VxGUID& onlineId, EPluginMsgType msgType, ECommErr commErr )
{
    getAppCommon().toGuiPluginCommError( pluginType, onlineId, msgType, commErr );
}

//============================================================================
void INlc::toGuiWantMicrophoneRecording( EAppModule appModule, bool wantMicInput )
{
    getAppCommon().toGuiWantMicrophoneRecording( appModule, wantMicInput );
}

//============================================================================
void INlc::toGuiWantSpeakerOutput( EAppModule appModule, bool wantSpeakerOutput )
{
    getAppCommon().toGuiWantSpeakerOutput( appModule, wantSpeakerOutput );
}

//============================================================================
int INlc::toGuiPlayAudioFrame( EAppModule appModule, int16_t * pu16PcmData, int pcmDataLenInBytes, bool isSilence )
{
    return getAppCommon().toGuiPlayAudioFrame( appModule, pu16PcmData, pcmDataLenInBytes, isSilence );
}

#if ENABLE_KODI
//============================================================================
int INlc::toGuiPlayAudioFrame( EAppModule appModule, float * pu16PcmData, int pcmDataLenInBytes )
{
    return getAppCommon().toGuiPlayAudioFrame( appModule, pu16PcmData, pcmDataLenInBytes );
}
#endif // ENABLE_KODI

//============================================================================
void INlc::toGuiWantVideoCapture( EAppModule appModule, bool wantVidCapture )
{
    getAppCommon().toGuiWantVideoCapture( appModule, wantVidCapture );
}

//============================================================================
void INlc::toGuiPlayVideoFrame( VxGUID& onlineId, uint8_t * pu8Jpg, uint32_t u32JpgDataLen, int motion0to100000 )
{
    if( false == getPtoP().isAppPaused() )
    {
        getAppCommon().toGuiPlayVideoFrame( onlineId, pu8Jpg, u32JpgDataLen, motion0to100000 );
    }
}
//============================================================================
//! show jpeg for video playback
int INlc::toGuiPlayVideoFrame( VxGUID& onlineId, uint8_t * picBuf, uint32_t picBufLen, int picWidth, int picHeight )
{
    if( false == getPtoP().isAppPaused() )
    {
        return getAppCommon().toGuiPlayVideoFrame( onlineId, picBuf, picBufLen, picWidth, picHeight );
    }

    return 0;
}

//============================================================================
void INlc::toGuiNetAvailableStatus( ENetAvailStatus eNetAvailStatus )
{
    getAppCommon().toGuiNetAvailableStatus( eNetAvailStatus );
}

//============================================================================
void INlc::toGuiNetworkState( ENetworkStateType eNetworkState, const char* stateMsg )
{
    getAppCommon().toGuiNetworkState( eNetworkState, stateMsg );
}

//============================================================================
void INlc::toGuiHostAnnounceStatus( EHostType hostType, VxGUID& sessionId, EHostAnnounceStatus annStatus, const char * msg )
{
    getAppCommon().toGuiHostAnnounceStatus( hostType, sessionId, annStatus, msg );
}

//============================================================================
void INlc::toGuiHostJoinStatus( EHostType hostType, VxGUID& sessionId, EHostJoinStatus joinStatus, const char * msg )
{
    getAppCommon().toGuiHostJoinStatus( hostType, sessionId, joinStatus, msg );
}

//============================================================================
void INlc::toGuiUserOnlineStatus( VxNetIdent* netIdent, bool isOnline )
{
    getAppCommon().toGuiUserOnlineStatus( netIdent, isOnline );
}

//============================================================================
void INlc::toGuiHostSearchStatus( EHostType hostType, VxGUID& sessionId, EHostSearchStatus searchStatus, ECommErr commErr, const char * msg )
{
    getAppCommon().toGuiHostSearchStatus( hostType, sessionId, searchStatus, commErr, msg );
}

//============================================================================
void INlc::toGuiHostSearchResult( EHostType hostType, VxGUID& sessionId, HostedInfo& hostedInfo )
{
    getAppCommon().toGuiHostSearchResult( hostType, sessionId, hostedInfo );
}

//============================================================================
void INlc::toGuiHostSearchComplete( EHostType hostType, VxGUID& sessionId )
{
    getAppCommon().toGuiHostSearchComplete( hostType, sessionId );
}

//============================================================================
void INlc::toGuiGroupieSearchStatus( EHostType hostType, VxGUID& sessionId, EHostSearchStatus searchStatus, ECommErr commErr, const char* msg )
{
    getAppCommon().toGuiGroupieSearchStatus( hostType, sessionId, searchStatus, commErr, msg );
}

//============================================================================
void INlc::toGuiGroupieSearchResult( EHostType hostType, VxGUID& sessionId, GroupieInfo& groupieInfo )
{
    getAppCommon().toGuiGroupieSearchResult( hostType, sessionId, groupieInfo );
}

//============================================================================
void INlc::toGuiGroupieSearchComplete( EHostType hostType, VxGUID& sessionId )
{
    getAppCommon().toGuiGroupieSearchComplete( hostType, sessionId );
}

//============================================================================
void INlc::toGuiIsPortOpenStatus( EIsPortOpenStatus eIsPortOpenStatus, const char * msg )
{
    getAppCommon().toGuiIsPortOpenStatus( eIsPortOpenStatus, msg );
}

//============================================================================
void INlc::toGuiRunTestStatus( const char *testName, ERunTestStatus eRunTestStatus, const char * msg )
{
    getAppCommon().toGuiRunTestStatus( testName, eRunTestStatus, msg );
}

//============================================================================
void INlc::toGuiRandomConnectStatus( ERandomConnectStatus eRandomConnectStatus, const char * msg )
{
    getAppCommon().toGuiRandomConnectStatus( eRandomConnectStatus, msg );
}

//============================================================================
void INlc::toGuiIndentListUpdate( EUserViewType listType, VxGUID& onlineId, uint64_t timestamp )
{
    getAppCommon().toGuiIndentListUpdate( listType, onlineId, timestamp );
}

//============================================================================
void INlc::toGuiIndentListRemove( EUserViewType listType, VxGUID& onlineId )
{
    getAppCommon().toGuiIndentListRemove( listType, onlineId );
}

//============================================================================
void INlc::toGuiContactAdded( VxNetIdent * netIdent )
{
    getAppCommon().toGuiContactAdded( netIdent );
}

//============================================================================
void INlc::toGuiContactRemoved( VxGUID& onlineId )
{
    getAppCommon().toGuiContactRemoved( onlineId );
}

//============================================================================
void INlc::toGuiContactOnline( VxNetIdent * netIdent )
{
    getAppCommon().toGuiContactOnline( netIdent );
}

//============================================================================
void INlc::toGuiContactOffline( VxNetIdent * netIdent )
{
    getAppCommon().toGuiContactOffline( netIdent );
}

//============================================================================
void INlc::toGuiContactNameChange( VxNetIdent * netIdent )
{
    getAppCommon().toGuiContactNameChange( netIdent );
}

//============================================================================
void INlc::toGuiContactDescChange( VxNetIdent * netIdent )
{
    getAppCommon().toGuiContactDescChange( netIdent );
}

//============================================================================
void INlc::toGuiContactMyFriendshipChange( VxNetIdent * netIdent )
{
    getAppCommon().toGuiContactMyFriendshipChange( netIdent );
}

//============================================================================
void INlc::toGuiContactHisFriendshipChange( VxNetIdent * netIdent )
{
    getAppCommon().toGuiContactHisFriendshipChange( netIdent );
}

//============================================================================
void INlc::toGuiContactSearchFlagsChange( VxNetIdent * netIdent )
{
    getAppCommon().toGuiContactSearchFlagsChange( netIdent );
}

//============================================================================
void INlc::toGuiContactConnectionChange( VxNetIdent * netIdent )
{
    getAppCommon().toGuiContactConnectionChange( netIdent );
}

//============================================================================
void INlc::toGuiContactLastSessionTimeChange( VxNetIdent * netIdent )
{
    getAppCommon().toGuiContactLastSessionTimeChange( netIdent );
}

//============================================================================
void INlc::toGuiContactAnythingChange( VxNetIdent * netIdent )
{
    getAppCommon().toGuiContactAnythingChange( netIdent );
}

//============================================================================
void INlc::toGuiPluginPermissionChange( VxNetIdent * netIdent )
{
    getAppCommon().toGuiPluginPermissionChange( netIdent );
}

//============================================================================
void INlc::toGuiUpdateMyIdent( VxNetIdent * netIdent )
{
    getAppCommon().toGuiUpdateMyIdent( netIdent );
}

//============================================================================
void INlc::toGuiSaveMyIdent( VxNetIdent* netIdent )
{
    getAppCommon().toGuiSaveMyIdent( netIdent );
}

//============================================================================
void INlc::toGuiRxedPluginOffer(   VxNetIdent *	netIdent,
                                    EPluginType		ePluginType,
                                    const char *	pMsg,
                                    int			    pvUserData,
                                    const char *	pFileName,
                                    uint8_t *		fileHashData,
                                    VxGUID&			lclSessionId,
                                    VxGUID&			rmtSessionId )
{
    getAppCommon().toGuiRxedPluginOffer( netIdent,
                              ePluginType,
                              pMsg,
                              pvUserData,
                              pFileName,
                              fileHashData,
                              lclSessionId,
                              rmtSessionId );
}

//============================================================================
void INlc::toGuiRxedOfferReply(    VxNetIdent *	    netIdent,
                                     EPluginType		ePluginType,
                                     int				pvUserData,
                                     EOfferResponse	    eOfferResponse,
                                     const char *	    pFileName,
                                     uint8_t *			fileHashData,
                                     VxGUID&			lclSessionId,
                                     VxGUID&			rmtSessionId )
{
    getAppCommon().toGuiRxedOfferReply( netIdent,
                                         ePluginType,
                                         pvUserData,
                                         eOfferResponse,
                                         pFileName,
                                         fileHashData,
                                         lclSessionId,
                                         rmtSessionId );
}

//============================================================================
void INlc::toGuiPluginSessionEnded(    VxNetIdent *	netIdent,
                                        EPluginType		ePluginType,
                                        int				pvUserData,
                                        EOfferResponse	eOfferResponse,
                                        VxGUID&			lclSessionId )
{
    getAppCommon().toGuiPluginSessionEnded( netIdent,
                                 ePluginType,
                                 pvUserData,
                                 eOfferResponse,
                                 lclSessionId );
}

//============================================================================
void INlc::toGuiPluginStatus( EPluginType		ePluginType,
                               int				statusType,
                               int				statusValue )
{
    getAppCommon().toGuiPluginStatus( ePluginType,
                                       statusType,
                                       statusValue );
}

//============================================================================
void INlc::toGuiInstMsg( VxNetIdent *	    netIdent,
                          EPluginType		ePluginType,
                          const char *	    pMsg )
{
    getAppCommon().toGuiInstMsg(  netIdent,
                                  ePluginType,
                                  pMsg );
}

//============================================================================
void INlc::toGuiFileList(  const char *	fileName,
                            uint64_t		fileLen,
                            uint8_t			fileType,
                            bool			isShared,
                            bool			isInLibrary,
                            VxGUID          assetId,
                            uint8_t *		fileHashId )
{
    getAppCommon().toGuiFileList( fileName,
                                   fileLen,
                                   fileType,
                                   isShared,
                                   isInLibrary,
                                   assetId,
                                   fileHashId );
}

//============================================================================
void INlc::toGuiFileListReply( VxNetIdent *	netIdent,
                                EPluginType		ePluginType,
                                uint8_t			u8FileType,
                                uint64_t		u64FileLen,
                                const char *	pFileName,
                                VxGUID          assetId,
                                uint8_t *		fileHashData )
{
    getAppCommon().toGuiFileListReply( netIdent,
                                        ePluginType,
                                        u8FileType,
                                        u64FileLen,
                                        pFileName,
                                        assetId,
                                        fileHashData );
}

//============================================================================
void INlc::toGuiStartUpload(   VxNetIdent *	netIdent,
                                EPluginType		ePluginType,
                                VxGUID&			fileInstanceId,
                                uint8_t			u8FileType,
                                uint64_t		u64FileLen,
                                std::string&	pFileName,
                                VxGUID&         assetId,
                                VxSha1Hash&		fileHashData )
{
    getAppCommon().toGuiStartUpload( netIdent,
                                      ePluginType,
                                      fileInstanceId,
                                      u8FileType,
                                      u64FileLen,
                                      pFileName,
                                      assetId,
                                      fileHashData );
}

//============================================================================
void INlc::toGuiStartDownload(     VxNetIdent *	netIdent,
                                    EPluginType		ePluginType,
                                    VxGUID&			fileInstanceId,
                                    uint8_t			u8FileType,
                                    uint64_t		u64FileLen,
                                    std::string&    pFileName,
                                    VxGUID&          assetId,
                                    VxSha1Hash&     fileHashData )
{
    getAppCommon().toGuiStartDownload( netIdent,
                                        ePluginType,
                                        fileInstanceId,
                                        u8FileType,
                                        u64FileLen,
                                        pFileName,
                                        assetId,
                                        fileHashData );
}

//============================================================================
void INlc::toGuiFileXferState( EPluginType pluginType, VxGUID&	lclSessionId, EXferState eXferState, int param1, int param2 )
{
    getAppCommon().toGuiFileXferState( pluginType, lclSessionId, eXferState, param1, param2 );
}

//============================================================================
void INlc::toGuiFileDownloadComplete( EPluginType pluginType, VxGUID& lclSessionId, std::string& fileName, EXferError xferError )
{
    getAppCommon().toGuiFileDownloadComplete( pluginType, lclSessionId, fileName, xferError );
}

//============================================================================
void INlc::toGuiFileUploadComplete( EPluginType pluginType, VxGUID& lclSessionId, std::string& fileName, EXferError xferError )
{
    getAppCommon().toGuiFileUploadComplete( pluginType, lclSessionId, fileName, xferError );
}

//============================================================================
void INlc::toGuiScanSearchComplete( EScanType eScanType )
{
    getAppCommon().toGuiScanSearchComplete( eScanType );
}

//============================================================================
void INlc::toGuiScanResultSuccess( EScanType eScanType, VxNetIdent *	netIdent )
{
    getAppCommon().toGuiScanResultSuccess( eScanType, netIdent );
}

//============================================================================
void INlc::toGuiSearchResultError( EScanType eScanType, VxNetIdent * netIdent, int errCode )
{
    getAppCommon().toGuiSearchResultError( eScanType, netIdent, errCode );
}

//============================================================================
void INlc::toGuiSearchResultProfilePic(    VxNetIdent *	netIdent,
                                            uint8_t *		pu8JpgData,
                                            uint32_t		u32JpgDataLen )
{
    getAppCommon().toGuiSearchResultProfilePic( netIdent, pu8JpgData, u32JpgDataLen );
}

//============================================================================
void INlc::toGuiSearchResultFileSearch(    VxNetIdent *	netIdent,
                                            VxGUID&			fileInstanceId,
                                            uint8_t			u8FileType,
                                            uint64_t		u64FileLen,
                                            const char *	pFileName,
                                            VxGUID          assetId)
{
    getAppCommon().toGuiSearchResultFileSearch( netIdent,
                                                fileInstanceId,
                                                u8FileType,
                                                u64FileLen,
                                                pFileName,
                                                assetId );
}

//============================================================================
void INlc::toGuiModuleState( EAppModule moduleNum, EModuleState moduleState )
{
	getAppCommon().toGuiModuleState( moduleNum, moduleState );
}

//============================================================================
void INlc::toGuiSetGameValueVar( EPluginType	ePluginType,
                                      VxGUID&	oOnlineId,
                                      int32_t	s32VarId,
                                      int32_t	s32VarValue )
{
    getAppCommon().toGuiSetGameValueVar(  ePluginType,
                                                 oOnlineId,
                                                 s32VarId,
                                                 s32VarValue );
}

//============================================================================
void INlc::toGuiSetGameActionVar( EPluginType	ePluginType,
                                       VxGUID&	oOnlineId,
                                       int32_t	s32VarId,
                                       int32_t	s32VarValue )
{
    getAppCommon().toGuiSetGameActionVar(  ePluginType,
                                                  oOnlineId,
                                                  s32VarId,
                                                  s32VarValue );
}

//============================================================================
void INlc::toGuiAssetAdded( AssetBaseInfo * assetInfo )
{
    LogModule( eLogAssets, LOG_VERBOSE, "INlc::toGuiAssetAdded" );
    getAppCommon().toGuiAssetAdded( assetInfo );
}

//============================================================================
void INlc::toGuiAssetUpdated( AssetBaseInfo* assetInfo )
{
    LogModule( eLogAssets, LOG_VERBOSE, "INlc::toGuiAssetUpdated" );
    getAppCommon().toGuiAssetUpdated( assetInfo );
}

//============================================================================
void INlc::toGuiAssetRemoved( AssetBaseInfo* assetInfo )
{
    LogModule( eLogAssets, LOG_VERBOSE, "INlc::toGuiAssetRemoved" );
    getAppCommon().toGuiAssetRemoved( assetInfo );
}

//============================================================================
void INlc::toGuiAssetXferState( VxGUID& assetUniqueId, EAssetSendState assetSendState, int param )
{
    getAppCommon().toGuiAssetXferState( assetUniqueId, assetSendState, param );
}

//============================================================================
void INlc::toGuiAssetAction( EAssetAction assetAction, VxGUID& assetId, int pos0to100000 )
{
    getAppCommon().toGuiAssetAction( assetAction, assetId, pos0to100000 );
}

//============================================================================
void INlc::toGuiMultiSessionAction( EMSessionAction mSessionAction, VxGUID& onlineId, int pos0to100000 )
{
    getAppCommon().toGuiMultiSessionAction( mSessionAction,
                                 onlineId,
                                 pos0to100000 );
}

//============================================================================
void INlc::toGuiAssetSessionHistory( AssetBaseInfo * assetInfo )
{
    getAppCommon().toGuiAssetSessionHistory( assetInfo );
}

//============================================================================
void INlc::toGuiBlobAdded( BlobInfo * hostListInfo )
{
    getAppCommon().toGuiBlobAdded( hostListInfo );
}

//============================================================================
void INlc::toGuiBlobAction( EAssetAction assetAction, VxGUID& assetId, int pos0to100000 )
{
    getAppCommon().toGuiBlobAction( assetAction,
                                        assetId,
                                        pos0to100000 );
}

//============================================================================
void INlc::toGuiBlobSessionHistory( BlobInfo * hostListInfo )
{
    getAppCommon().toGuiBlobSessionHistory( hostListInfo );
}

//============================================================================
void INlc::toGuiPushToTalkStatus( VxGUID& onlineId, EPushToTalkStatus pushToTalkStatus )
{
    getAppCommon().toGuiPushToTalkStatus( onlineId, pushToTalkStatus );
}

//============================================================================
void INlc::toGuiNetworkIsTested( bool requiresRelay, std::string& ipAddr, uint16_t ipPort )
{
    getAppCommon().toGuiNetworkIsTested( requiresRelay, ipAddr, ipPort );
}
