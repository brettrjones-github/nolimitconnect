#include <CommonHdr.h>

#include "IGoTv.h"
#include "AppsSrc/CommonSrc/QtSource/AppCommon.h"

//============================================================================
void IGoTv::toGuiLog( int logLevel, const char * msg )
{
    getAppCommon().toGuiLog( logLevel, msg );
}

//============================================================================
void IGoTv::toGuiAppErr( EAppErr eAppErr, const char * errMsg )
{
    getAppCommon().toGuiAppErr( eAppErr, errMsg );
}

//============================================================================
void IGoTv::toGuiStatusMessage( const char * statusMsg )
{
    getAppCommon().toGuiStatusMessage( statusMsg );
}

//============================================================================
void IGoTv::toGuiPluginMsg( EPluginType pluginType, VxGUID& onlineId, EPluginMsgType msgType, const char* paramMsg, ... )
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
void IGoTv::toGuiPluginMsg( EPluginType pluginType, VxGUID& onlineId, EPluginMsgType msgType, std::string& paramMsg )
{
    getAppCommon().toGuiPluginMsg( pluginType, onlineId, msgType, paramMsg );
}

//============================================================================
void IGoTv::toGuiPluginCommError( EPluginType pluginType, VxGUID& onlineId, EPluginMsgType msgType, ECommErr commErr )
{
    getAppCommon().toGuiPluginCommError( pluginType, onlineId, msgType, commErr );
}

//============================================================================
void IGoTv::toGuiMicrophonePeak( EAppModule appModule, int peekVal0to32768 )
{
    getAppCommon().toGuiMicrophonePeak( appModule, peekVal0to32768 );
}

//============================================================================
void IGoTv::toGuiWantMicrophoneRecording( EAppModule appModule, bool wantMicInput )
{
    getAppCommon().toGuiWantMicrophoneRecording( appModule, wantMicInput );
}

//============================================================================
void IGoTv::toGuiWantSpeakerOutput( EAppModule appModule, bool wantSpeakerOutput )
{
    getAppCommon().toGuiWantSpeakerOutput( appModule, wantSpeakerOutput );
}

//============================================================================
int IGoTv::toGuiPlayAudio( EAppModule appModule, int16_t * pu16PcmData, int pcmDataLenInBytes, bool isSilence )
{
    return getAppCommon().toGuiPlayAudio( appModule, pu16PcmData, pcmDataLenInBytes, isSilence );
}

//============================================================================
int IGoTv::toGuiPlayAudio( EAppModule appModule, float * pu16PcmData, int pcmDataLenInBytes )
{
    return getAppCommon().toGuiPlayAudio( appModule, pu16PcmData, pcmDataLenInBytes );
}

//============================================================================
double IGoTv::toGuiGetAudioDelaySeconds( EAppModule appModule )
{
    return getAppCommon().toGuiGetAudioDelaySeconds( appModule );
}

//============================================================================
double IGoTv::toGuiGetAudioCacheTotalSeconds( EAppModule appModule )
{
    return getAppCommon().toGuiGetAudioCacheTotalSeconds( appModule  );
}

//============================================================================
int IGoTv::toGuiGetAudioCacheFreeSpace( EAppModule appModule )
{
    return getAppCommon().toGuiGetAudioCacheFreeSpace( appModule );
}

//============================================================================
void IGoTv::toGuiWantVideoCapture( bool wantVidCapture )
{
    getAppCommon().toGuiWantVideoCapture( wantVidCapture );
}

//============================================================================
void IGoTv::toGuiPlayVideoFrame( VxGUID& onlineId, uint8_t * pu8Jpg, uint32_t u32JpgDataLen, int motion0to100000 )
{
    if( false == getPtoP().isAppPaused() )
    {
        getAppCommon().toGuiPlayVideoFrame( onlineId, pu8Jpg, u32JpgDataLen, motion0to100000 );
    }
}
//============================================================================
//! show jpeg for video playback
int IGoTv::toGuiPlayVideoFrame( VxGUID& onlineId, uint8_t * picBuf, uint32_t picBufLen, int picWidth, int picHeight )
{
    if( false == getPtoP().isAppPaused() )
    {
        return getAppCommon().toGuiPlayVideoFrame( onlineId, picBuf, picBufLen, picWidth, picHeight );
    }

    return 0;
}

//============================================================================
void IGoTv::toGuiNetAvailableStatus( ENetAvailStatus eNetAvailStatus )
{
    getAppCommon().toGuiNetAvailableStatus( eNetAvailStatus );
}

//============================================================================
void IGoTv::toGuiNetworkState( ENetworkStateType eNetworkState, const char* stateMsg )
{
    getAppCommon().toGuiNetworkState( eNetworkState, stateMsg );
}

//============================================================================
void IGoTv::toGuiMyRelayStatus( EMyRelayStatus eRelayStatus, const char * msg )
{
    getAppCommon().toGuiMyRelayStatus( eRelayStatus, msg );
}

//============================================================================
void IGoTv::toGuiHostAnnounceStatus( EHostType hostType, VxGUID& sessionId, EHostAnnounceStatus annStatus, const char * msg )
{
    getAppCommon().toGuiHostAnnounceStatus( hostType, sessionId, annStatus, msg );
}

//============================================================================
void IGoTv::toGuiHostJoinStatus( EHostType hostType, VxGUID& sessionId, EHostJoinStatus joinStatus, const char * msg )
{
    getAppCommon().toGuiHostJoinStatus( hostType, sessionId, joinStatus, msg );
}

//============================================================================
void IGoTv::toGuiUserOnlineStatus( VxNetIdent* netIdent, bool isOnline )
{
    getAppCommon().toGuiUserOnlineStatus( netIdent, isOnline );
}

//============================================================================
void IGoTv::toGuiHostSearchStatus( EHostType hostType, VxGUID& sessionId, EHostSearchStatus searchStatus, ECommErr commErr, const char * msg )
{
    getAppCommon().toGuiHostSearchStatus( hostType, sessionId, searchStatus, commErr, msg );
}

//============================================================================
void IGoTv::toGuiHostSearchResult( EHostType hostType, VxGUID& sessionId, HostedInfo& hostedInfo )
{
    getAppCommon().toGuiHostSearchResult( hostType, sessionId, hostedInfo );
}

//============================================================================
void IGoTv::toGuiHostSearchComplete( EHostType hostType, VxGUID& sessionId )
{
    getAppCommon().toGuiHostSearchComplete( hostType, sessionId );
}

//============================================================================
void IGoTv::toGuiGroupieSearchStatus( EHostType hostType, VxGUID& sessionId, EHostSearchStatus searchStatus, ECommErr commErr, const char* msg )
{
    getAppCommon().toGuiGroupieSearchStatus( hostType, sessionId, searchStatus, commErr, msg );
}

//============================================================================
void IGoTv::toGuiGroupieSearchResult( EHostType hostType, VxGUID& sessionId, GroupieInfo& groupieInfo )
{
    getAppCommon().toGuiGroupieSearchResult( hostType, sessionId, groupieInfo );
}

//============================================================================
void IGoTv::toGuiGroupieSearchComplete( EHostType hostType, VxGUID& sessionId )
{
    getAppCommon().toGuiGroupieSearchComplete( hostType, sessionId );
}

//============================================================================
void IGoTv::toGuiIsPortOpenStatus( EIsPortOpenStatus eIsPortOpenStatus, const char * msg )
{
    getAppCommon().toGuiIsPortOpenStatus( eIsPortOpenStatus, msg );
}

//============================================================================
void IGoTv::toGuiRunTestStatus( const char *testName, ERunTestStatus eRunTestStatus, const char * msg )
{
    getAppCommon().toGuiRunTestStatus( testName, eRunTestStatus, msg );
}

//============================================================================
void IGoTv::toGuiRandomConnectStatus( ERandomConnectStatus eRandomConnectStatus, const char * msg )
{
    getAppCommon().toGuiRandomConnectStatus( eRandomConnectStatus, msg );
}

//============================================================================
void IGoTv::toGuiIndentListUpdate( EUserViewType listType, VxGUID& onlineId, uint64_t timestamp )
{
    getAppCommon().toGuiIndentListUpdate( listType, onlineId, timestamp );
}

//============================================================================
void IGoTv::toGuiIndentListRemove( EUserViewType listType, VxGUID& onlineId )
{
    getAppCommon().toGuiIndentListRemove( listType, onlineId );
}

//============================================================================
void IGoTv::toGuiContactAdded( VxNetIdent * netIdent )
{
    getAppCommon().toGuiContactAdded( netIdent );
}

//============================================================================
void IGoTv::toGuiContactRemoved( VxGUID& onlineId )
{
    getAppCommon().toGuiContactRemoved( onlineId );
}

//============================================================================
void IGoTv::toGuiContactOnline( VxNetIdent * netIdent )
{
    getAppCommon().toGuiContactOnline( netIdent );
}

//============================================================================
void IGoTv::toGuiContactOffline( VxNetIdent * netIdent )
{
    getAppCommon().toGuiContactOffline( netIdent );
}

//============================================================================
void IGoTv::toGuiContactNameChange( VxNetIdent * netIdent )
{
    getAppCommon().toGuiContactNameChange( netIdent );
}

//============================================================================
void IGoTv::toGuiContactDescChange( VxNetIdent * netIdent )
{
    getAppCommon().toGuiContactDescChange( netIdent );
}

//============================================================================
void IGoTv::toGuiContactMyFriendshipChange( VxNetIdent * netIdent )
{
    getAppCommon().toGuiContactMyFriendshipChange( netIdent );
}

//============================================================================
void IGoTv::toGuiContactHisFriendshipChange( VxNetIdent * netIdent )
{
    getAppCommon().toGuiContactHisFriendshipChange( netIdent );
}

//============================================================================
void IGoTv::toGuiContactSearchFlagsChange( VxNetIdent * netIdent )
{
    getAppCommon().toGuiContactSearchFlagsChange( netIdent );
}

//============================================================================
void IGoTv::toGuiContactConnectionChange( VxNetIdent * netIdent )
{
    getAppCommon().toGuiContactConnectionChange( netIdent );
}

//============================================================================
void IGoTv::toGuiContactLastSessionTimeChange( VxNetIdent * netIdent )
{
    getAppCommon().toGuiContactLastSessionTimeChange( netIdent );
}

//============================================================================
void IGoTv::toGuiContactAnythingChange( VxNetIdent * netIdent )
{
    getAppCommon().toGuiContactAnythingChange( netIdent );
}

//============================================================================
void IGoTv::toGuiPluginPermissionChange( VxNetIdent * netIdent )
{
    getAppCommon().toGuiPluginPermissionChange( netIdent );
}

//============================================================================
void IGoTv::toGuiUpdateMyIdent( VxNetIdent * netIdent )
{
    getAppCommon().toGuiUpdateMyIdent( netIdent );
}

//============================================================================
void IGoTv::toGuiSaveMyIdent( VxNetIdent* netIdent )
{
    getAppCommon().toGuiSaveMyIdent( netIdent );
}

//============================================================================
void IGoTv::toGuiRxedPluginOffer(   VxNetIdent *	netIdent,
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
void IGoTv::toGuiRxedOfferReply(    VxNetIdent *	    netIdent,
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
void IGoTv::toGuiPluginSessionEnded(    VxNetIdent *	netIdent,
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
void IGoTv::toGuiPluginStatus( EPluginType		ePluginType,
                               int				statusType,
                               int				statusValue )
{
    getAppCommon().toGuiPluginStatus( ePluginType,
                                       statusType,
                                       statusValue );
}

//============================================================================
void IGoTv::toGuiInstMsg( VxNetIdent *	    netIdent,
                          EPluginType		ePluginType,
                          const char *	    pMsg )
{
    getAppCommon().toGuiInstMsg(  netIdent,
                                  ePluginType,
                                  pMsg );
}

//============================================================================
void IGoTv::toGuiFileList(  const char *	fileName,
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
void IGoTv::toGuiFileListReply( VxNetIdent *	netIdent,
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
void IGoTv::toGuiStartUpload(   VxNetIdent *	netIdent,
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
void IGoTv::toGuiStartDownload(     VxNetIdent *	netIdent,
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
void IGoTv::toGuiFileXferState( EPluginType pluginType, VxGUID&	lclSessionId, EXferState eXferState, int param1, int param2 )
{
    getAppCommon().toGuiFileXferState( pluginType, lclSessionId, eXferState, param1, param2 );
}

//============================================================================
void IGoTv::toGuiFileDownloadComplete( EPluginType pluginType, VxGUID& lclSessionId, std::string& fileName, EXferError xferError )
{
    getAppCommon().toGuiFileDownloadComplete( pluginType, lclSessionId, fileName, xferError );
}

//============================================================================
void IGoTv::toGuiFileUploadComplete( EPluginType pluginType, VxGUID& lclSessionId, std::string& fileName, EXferError xferError )
{
    getAppCommon().toGuiFileUploadComplete( pluginType, lclSessionId, fileName, xferError );
}

//============================================================================
void IGoTv::toGuiScanSearchComplete( EScanType eScanType )
{
    getAppCommon().toGuiScanSearchComplete( eScanType );
}

//============================================================================
void IGoTv::toGuiScanResultSuccess( EScanType eScanType, VxNetIdent *	netIdent )
{
    getAppCommon().toGuiScanResultSuccess( eScanType, netIdent );
}

//============================================================================
void IGoTv::toGuiSearchResultError( EScanType eScanType, VxNetIdent * netIdent, int errCode )
{
    getAppCommon().toGuiSearchResultError( eScanType, netIdent, errCode );
}

//============================================================================
void IGoTv::toGuiSearchResultProfilePic(    VxNetIdent *	netIdent,
                                            uint8_t *		pu8JpgData,
                                            uint32_t		u32JpgDataLen )
{
    getAppCommon().toGuiSearchResultProfilePic( netIdent, pu8JpgData, u32JpgDataLen );
}

//============================================================================
void IGoTv::toGuiSearchResultFileSearch(    VxNetIdent *	netIdent,
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
void IGoTv::toGuiModuleState( EAppModule moduleNum, EModuleState moduleState )
{
	getAppCommon().toGuiModuleState( moduleNum, moduleState );
}

//============================================================================
void IGoTv::toGuiSetGameValueVar( EPluginType	ePluginType,
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
void IGoTv::toGuiSetGameActionVar( EPluginType	ePluginType,
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
void IGoTv::toGuiAssetAdded( AssetBaseInfo * assetInfo )
{
    LogModule( eLogAssets, LOG_DEBUG, "IGoTv::toGuiAssetAdded" );
    getAppCommon().toGuiAssetAdded( assetInfo );
}

//============================================================================
void IGoTv::toGuiAssetAction( EAssetAction assetAction, VxGUID& assetId, int pos0to100000 )
{
    getAppCommon().toGuiAssetAction( assetAction,
                          assetId,
                          pos0to100000 );
}

//============================================================================
void IGoTv::toGuiMultiSessionAction( EMSessionAction mSessionAction, VxGUID& onlineId, int pos0to100000 )
{
    getAppCommon().toGuiMultiSessionAction( mSessionAction,
                                 onlineId,
                                 pos0to100000 );
}

//============================================================================
void IGoTv::toGuiAssetSessionHistory( AssetBaseInfo * assetInfo )
{
    getAppCommon().toGuiAssetSessionHistory( assetInfo );
}

//============================================================================
void IGoTv::toGuiBlobAdded( BlobInfo * hostListInfo )
{
    getAppCommon().toGuiBlobAdded( hostListInfo );
}

//============================================================================
void IGoTv::toGuiBlobAction( EAssetAction assetAction, VxGUID& assetId, int pos0to100000 )
{
    getAppCommon().toGuiBlobAction( assetAction,
                                        assetId,
                                        pos0to100000 );
}
//============================================================================
void IGoTv::toGuiBlobSessionHistory( BlobInfo * hostListInfo )
{
    getAppCommon().toGuiBlobSessionHistory( hostListInfo );
}

////============================================================================
//void IGoTv::toGuiThumbAdded( ThumbInfo * thumbInfo )
//{
//    getAppCommon().toGuiThumbAdded( thumbInfo );
//}
//
////============================================================================
//void IGoTv::toGuiThumbUpdated( ThumbInfo * thumbInfo )
//{
//    getAppCommon().toGuiThumbUpdated( thumbInfo );
//}
//
////============================================================================
//void IGoTv::toGuiThumbRemoved( VxGUID& thumbId )
//{
//    getAppCommon().toGuiThumbRemoved( thumbId );
//}
