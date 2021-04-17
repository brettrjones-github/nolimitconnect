#pragma once
//============================================================================
// Copyright (C) 2010 Brett R. Jones
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

// android specific code
#ifdef TARGET_OS_ANDROID

#include "JavaBaseClass.h"
#include <GuiInterface/IToGui.h>

class NativeToJavaClass : public JavaBaseClass
{
public:
	NativeToJavaClass(  JavaEnvCache& envCache, jclass javaRefClass );

	//=== vars ===//
	jmethodID 					m_methToGuiLog;
	jmethodID 					m_methToGuiAppErr;
	jmethodID 					m_methToGuiStatusMessage;

	jmethodID 					m_methToGuiWantMicrophoneRecording;
	jmethodID 					m_methToGuiWantSpeakerOutput;
	jmethodID 					m_methToGuiWantVideoCapture;
	jmethodID 					m_methToGuiPlayVideoFrame;

	jmethodID 					m_methToGuiNetworkState;
	jmethodID 					m_methToGuiMyRelayStatus;
	jmethodID 					m_methToGuiHostStatus;
	jmethodID 					m_methToGuiIsPortOpenStatus;
	jmethodID 					m_methToGuiRandomConnectStatus;

	jmethodID 					m_methToGuiContactOnline;
	jmethodID 					m_methToGuiContactOffline;
	jmethodID 					m_methToGuiContactNearby;
	jmethodID 					m_methToGuiContactNotNearby;

	jmethodID 					m_methToGuiContactNameChange;
	jmethodID 					m_methToGuiContactDescChange;
	jmethodID 					m_methToGuiContactMyFriendshipChange;
	jmethodID 					m_methToGuiContactHisFriendshipChange;
	jmethodID 					m_methToGuiContactSearchFlagsChange;
	jmethodID 					m_methToGuiContactConnectionChange;
	jmethodID 					m_methToGuiContactAnythingChange;
	jmethodID 					m_methToGuiContactLastSessionTimeChange;

	jmethodID 					m_methToGuiPluginPermissionChange;

	jmethodID 					m_methToGuiUpdateMyIdent;

	jmethodID 					m_methToGuiRxedPluginOffer;
	jmethodID 					m_methToGuiRxedOfferReply;
	jmethodID 					m_methToGuiSessionEnded;
	jmethodID 					m_methToGuiPluginStatus;

	jmethodID 					m_methToGuiInstMsg;

	jmethodID 					m_methToGuiFileList;
	jmethodID 					m_methToGuiFileListReply;

	jmethodID 					m_methToGuiStartUpload;
	jmethodID 					m_methToGuiStartDownload;
	jmethodID 					m_methToGuiFileXferState;	
	jmethodID 					m_methToGuiFileDownloadComplete;	
	jmethodID 					m_methToGuiFileUploadComplete;

	jmethodID 					m_methToGuiScanSearchComplete;
	jmethodID 					m_methToGuiSearchResultSuccess;
	jmethodID 					m_methToGuiSearchResultError;

	jmethodID 					m_methSearchResultProfilePic;
	jmethodID 					m_methSearchResultFileSearch;

	jmethodID 					m_methToGuiSetGameValueVar;
	jmethodID 					m_methToGuiSetGameActionVar;

	jmethodID 					m_methToGuiGenerateGuidHiPart;
	jmethodID 					m_methToGuiGenerateGuidLoPart;
	
	jmethodID 					m_methToGuiAssetAdded;
	jmethodID 					m_methToGuiSessionHistory;
	jmethodID 					m_methToGuiAssetAction;
	jmethodID 					m_methToGuiMultiSessionAction;
};

//extern std::map<unsigned int, NativeToJavaClass *> g_oNativeToJavaMap; 

NativeToJavaClass * GetNativeToJavaClass( void );
void		NativeToJavaClassThreadExit( unsigned int uiThreadId, const char * );
void		NativeToJavaClassShutdown( void );

//=== app methods ===//

void		JavaToGuiLog( int logLevel, const char * pMsg );
void		JavaToGuiAppErr( int eAppErr, const char * pMsg );
void		JavaToGuiStatusMessage( const char * pMsg );

//void		JavaToGuiStartAppIdle( void );
//void		JavaToGuiStopAppIdle( void );

//=== audio methods ===//
void		JavaToGuiWantMicrophoneRecording( bool wantMicInput );
void		JavaToGuiWantSpeakerOutput( bool wantSpeakerOutput );

void		JavaToGuiWantVideoCapture( bool wantVidCapture );
void		JavaToGuiPlayVideoFrame( VxGUID& onlineId, uint8_t * pu8Jpg, uint32_t u32JpgDataLen, int motion0to100000 );

void		JavaToGuiNetworkState( ENetworkStateType eNetworkState, const char* stateMsg = "" );
void		JavaToGuiMyRelayStatus( EMyRelayStatus eRelayStatus, const char * msg = "" );
void		JavaToGuiHostStatus( EHostTestStatus eHostStatus, const char * msg = "" );
void		JavaToGuiIsPortOpenStatus( EIsPortOpenStatus eIsPortOpenStatus, const char * msg = "" );
void		JavaToGuiRandomConnectStatus( ERandomConnectStatus eRandomConnectStatus, const char * msg = "" );

void		JavaToGuiContactOnline( VxNetIdent * netIdent, bool newContact );
void		JavaToGuiContactOffline( VxNetIdent * netIdent );
void		JavaToGuiContactNearby( VxNetIdent * netIdent );
void		JavaToGuiContactNotNearby( VxNetIdent * netIdent );

//! called when contact changes
void		JavaToGuiContactNameChange( VxNetIdent * netIdent );
void		JavaToGuiContactDescChange( VxNetIdent * netIdent );
void		JavaToGuiContactMyFriendshipChange( VxNetIdent * netIdent );
void		JavaToGuiContactHisFriendshipChange( VxNetIdent * netIdent );
void		JavaToGuiContactSearchFlagsChange( VxNetIdent * netIdent );
void		JavaToGuiContactConnectionChange( VxNetIdent * netIdent );
void		JavaToGuiContactAnythingChange( VxNetIdent * netIdent );
void 		JavaToGuiContactLastSessionTimeChange( VxNetIdent * netIdent );

void		JavaToGuiPluginPermissionChange( VxNetIdent * netIdent );

//! called from engine when need to update identity
void		JavaToGuiUpdateMyIdent( VxNetIdent * netIdent );

//! add offer to notify list
void		JavaToGuiRxedPluginOffer(   VxNetIdent *	netIdent, 
										EPluginType		ePluginType, 
										const char *	pMsg,
										int				pvUserData,
										const char *	pFileName,
										uint8_t *			fileHashData,
										VxGUID&			lclSessionId,
										VxGUID&			rmtSessionId );
//! response to offer
void		JavaToGuiRxedOfferReply(	VxNetIdent *	netIdent, 
										EPluginType		ePluginType,
										int				pvUserData,
										EOfferResponse	eOfferResponse, 
										const char *	pFileName,
										uint8_t *			fileHashData,
										VxGUID&			lclSessionId,
										VxGUID&			rmtSessionId );

void		JavaToGuiPluginSessionEnded(	VxNetIdent *	netIdent, 
											EPluginType		ePluginType,
											int				pvUserData,
											EOfferResponse	eOfferResponse,
											VxGUID&			lclSessionId );

void 		JavaToGuiPluginStatus(	EPluginType		ePluginType,
									int				statusType,
									int				statusValue );

void		JavaToGuiInstMsg(	VxNetIdent *	netIdent, 
								EPluginType		ePluginType, 
								const char *	pMsg );

void 		JavaToGuiFileList(	const char *	fileName,
								uint64_t				fileLen,
								uint8_t				fileType,
								bool			isShared,
								bool			isInLibrary,
								uint8_t *			fileHashData );

void		JavaToGuiFileListReply(		VxNetIdent *	netIdent, 
										EPluginType		ePluginType, 
										uint8_t				u8FileType, 
										uint64_t				u64FileLen, 
										const char *	pFileName,
										uint8_t *			fileHashData );

void		JavaToGuiStartUpload(	VxNetIdent *	netIdent, 
									EPluginType		ePluginType, 
									VxGUID&			fileInstanceId, 
									uint8_t				u8FileType, 
									uint64_t				u64FileLen, 
									const char *	pFileName,
									uint8_t *			fileHashData );

void		JavaToGuiStartDownload(	VxNetIdent *	netIdent, 
									EPluginType		ePluginType, 
									VxGUID&			fileInstanceId, 
									uint8_t				u8FileType, 
									uint64_t				u64FileLen, 
									const char *	pFileName,
									uint8_t *			fileHashData );

void		JavaToGuiFileXferState( VxGUID&	lclSessionId, EXferState eXferState, int param1, int param2 );
void		JavaToGuiFileDownloadComplete( VxGUID& lclSessionId, const char * newFileName, EXferError xferError );
void		JavaToGuiFileUploadComplete( VxGUID& lclSessionId, EXferError xferError );

//=== scan ===//
void		JavaToGuiScanSearchComplete( EScanType eScanType );	
void		JavaToGuiSearchResultSuccess( EScanType eScanType, VxNetIdent *	netIdent );	
void		JavaToGuiSearchResultError( EScanType eScanType, VxNetIdent *	netIdent, int errCode );	

void		JavaToGuiSearchResultProfilePic(	VxNetIdent *	netIdent, 
												uint8_t *			pu8JpgData, 
												uint32_t				u32JpgDataLen );

void		JavaToGuiSearchResultFileSearch(	VxNetIdent *	netIdent, 		
												VxGUID&			fileInstanceId, 
												uint8_t				u8FileType, 
												uint64_t				u64FileLen, 
												const char *	pFileName );

bool		JavaToGuiSetGameValueVar(	EPluginType		ePluginType, 
										VxGUID&			oOnlineId, 
										int32_t				s32VarId, 
										int32_t				s32VarValue );

bool		JavaToGuiSetGameActionVar(	EPluginType		ePluginType, 
										VxGUID&		oOnlineId, 
										int32_t				s32VarId, 
										int32_t				s32VarValue );

void		JavaToGuiGenerateGuid( VxGUID& retGuid );

void		JavaToGuiAssetAdded( AssetInfo * assetInfo );
void		JavaToGuiSessionHistory( AssetInfo * assetInfo );
void		JavaToGuiAssetAction(	EAssetAction assetAction, 
									VxGUID& assetId, 
									int pos0to100000 );
void		JavaToGuiMultiSessionAction(	EMSessionAction mSessionAction, 
											VxGUID& onlineId, 
											int pos0to100000 );
#endif //TARGET_OS_ANDROID
