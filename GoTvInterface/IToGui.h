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
// brett.jones@engineer.com
// http://www.gotvptop.net
//============================================================================

#include "IDefs.h"
#include "GoTvInterface/IGoTvDefs.h"

#include <CoreLib/AppErr.h>
#include <CoreLib/AssetDefs.h>
#include <PktLib/VxCommon.h>

class VxNetIdent;
class AssetInfo;
class VxGUID;

//! \public Enumeration of Relay Search Status
enum EMyRelayStatus
{
	eMyRelayStatusDisconnected				= 0,
	eMyRelayStatusConnected					= 1,
	eMyRelayStatusSearching					= 2,
	eMyRelayStatusRequestService			= 3,
	eMyRelayStatusNoRelaysListed			= 4,
	eMyRelayStatusRelayListExhasted			= 5,
	eMyRelayStatusAssumeFirewalled			= 6,
	eMyRelayStatusAssumeCanDirectConnect	= 7,

	eMaxMyRelayStatus
};

//! Describe Relay Search Status as text
const char * DescribeRelayStatus( EMyRelayStatus eRelayStatus );

//! \public Enumeration of Network State Machine states/actions
enum ENetworkStateType
{
	eNetworkStateTypeUnknown				= 0,
	eNetworkStateTypeLost					= 1,
	eNetworkStateTypeAvail					= 2,
	eNetworkStateTypeTestConnection			= 3,
	eNetworkStateTypeRelaySearch			= 4,
	eNetworkStateTypeAnnounce				= 5,
	eNetworkStateTypeOnlineDirect			= 6,
	eNetworkStateTypeOnlineThroughRelay		= 7,
	eNetworkStateTypeGetRelayList			= 8,
	eNetworkStateTypeNoInternetConnection	= 9,
	eNetworkStateTypeFailedResolveAnchor	= 10,

	eMaxNetworkStateType
};

//! Network State as text
const char * DescribeNetworkState( ENetworkStateType networkStateType );

//! \public Anchor connection test state
enum EAnchorTestStatus
{
	eAnchorTestStatusUnknown						= 0,
	eAnchorTestStatusLogMsg							= 1,

	eAnchorTestStatusAnchorOk						= 2,
	eAnchorTestStatusAnchorConnectFail				= 3,
	eAnchorTestStatusAnchorConnectionDropped		= 4,
	eAnchorTestStatusAnchorTestComplete				= 5,

	eAnchorTestStatusNetServiceOk					= 6,
	eAnchorTestStatusNetServiceConnectFail			= 7,
	eAnchorTestStatusNetServiceConnectionDropped	= 8,
	eAnchorTestStatusNetServiceTestComplete			= 9,

	eMaxAnchorTestStatusType
};

//! Anchor connection test state as text
const char * DescribeAnchorStatus( EAnchorTestStatus eAnchorStatus );

//! \public Can Direct Connect test state
enum EIsPortOpenStatus
{
	eIsPortOpenStatusUnknown						= 0,
	eIsPortOpenStatusLogMsg							= 1,

	eIsPortOpenStatusOpen							= 2,
	eIsPortOpenStatusClosed							= 3,
	eIsPortOpenStatusConnectFail					= 4,
	eIsPortOpenStatusConnectionDropped				= 5,
	eIsPortOpenStatusInvalidResponse				= 6,
	eIsPortOpenStatusTestComplete					= 7,

	eMaxIsPortOpenStatusType
};

//! Describe Direct Connect test state as text
const char * DescribePortOpenStatus( EIsPortOpenStatus ePortOpenStatus );


//! Describe connect by shaking phone ( or press simulate phone shake ) status as text
const char * DescribePhoneShakeStatus( EPhoneShakeStatus ePortOpenStatus );

//! IToGui is an abstract interface for calls to GUI from native C++/C code
class IToGui
{
public:
    virtual void				playGoTvMedia( AssetInfo * assetInfo ) {};

	/// Send log message to GUI
	virtual void				toGuiLog( int logFlags, const char * pMsg ) = 0;
	/// Send error occurred message to GUI
	virtual void				toGuiAppErr( EAppErr eAppErr, const char* errMsg = "" ) = 0;
	/// Send status bar message to GUI
	virtual void				toGuiStatusMessage( const char* errMsg ) = 0;

	/// Start/Stop microphone sound capture
	virtual void				toGuiWantMicrophoneRecording( bool wantMicInput ) = 0;
	/// Start/Stop output to speaker
	virtual void				toGuiWantSpeakerOutput( bool wantSpeakerOutput ) = 0;
	/// Send audio from MediaProcessor mixer to GUI for playback
	virtual void				toGuiPlayAudio( int16_t * pu16PcmData, int pcmDataLenInBytes ) = 0;
    virtual int  				toGuiPlayAudio( EAppModule appModule, int16_t * pu16PcmData, int pcmDataLenInBytes ) = 0;
    virtual double  			toGuiGetAudioDelaySeconds() = 0;
    virtual double				toGuiGetAudioCacheTotalSeconds() = 0;
    virtual int				    toGuiGetAudioCacheFreeSpace( EAppModule appModule ) = 0;

	/// Start/Stop camera capture
	virtual void				toGuiWantVideoCapture( bool wantVidCapture ) = 0;
	/// Send video feed frame to GUI for playback.. includes amount of motion detected
	virtual void				toGuiPlayVideoFrame( VxGUID& onlineId, uint8_t * pu8Jpg, uint32_t u32JpgDataLen, int motion0to100000 ) = 0;
    /// Send video feed frame to GUI for playback.. 
    virtual int				    toGuiPlayVideoFrame( VxGUID& onlineId, uint8_t * picBuf, uint32_t picBufLen, int picWidth, int picHeight ) = 0;

	/// Send Network state to GUI for display
	virtual void				toGuiNetworkState( ENetworkStateType eNetworkState, const char* stateMsg = "" ) = 0;
	/// Send anchor status to GUI for display
	virtual void				toGuiAnchorStatus( EAnchorTestStatus eAnchorStatus, const char * msg = "" ) = 0;
	/// Send is port open test state/status to GUI
	virtual void				toGuiIsPortOpenStatus( EIsPortOpenStatus eIsPortOpenStatus, const char * msg = "" ) = 0;
	/// Send connect by phone shake status to GUI
	virtual void				toGuiPhoneShakeStatus( EPhoneShakeStatus ePhoneShakeStatus, const char * msg = "" ) = 0;
	/// Send relay status to GUI for display
	virtual void				toGuiMyRelayStatus( EMyRelayStatus eRelayStatus, const char * msg = "" ) = 0;

	/// Update contact to online state GUI
	virtual void				toGuiContactOnline( VxNetIdent * netIdent, bool newContact = false ) = 0;
	/// Update contact to offline state GUI
	virtual void				toGuiContactOffline( VxNetIdent * netIdent ) = 0;
	/// Not currently used. Notify GUI if contact is on same LAN network or is nearby
	virtual void				toGuiContactNearby( VxNetIdent * netIdent ) = 0;
	/// Not currently used. Notify GUI if contact is no longer nearby
	virtual void				toGuiContactNotNearby( VxNetIdent * netIdent ) = 0;

	/// Notify GUI that contact changed his/her online name
	virtual void				toGuiContactNameChange( VxNetIdent * netIdent ) = 0;
	/// Notify GUI that contact changed his/her mood message
	virtual void				toGuiContactDescChange( VxNetIdent * netIdent ) = 0;
	/// Notify GUI that permission level given by you to contact has changed
	virtual void				toGuiContactMyFriendshipChange( VxNetIdent * netIdent ) = 0;
	/// Notify GUI that contact changed his/her permission level given to you
	virtual void				toGuiContactHisFriendshipChange( VxNetIdent * netIdent ) = 0;
	/// Notify GUI that contacts search flags changed
	virtual void				toGuiContactSearchFlagsChange( VxNetIdent * netIdent ) = 0;
	/// Notify GUI that contact is connected via different connection
	virtual void				toGuiContactConnectionChange( VxNetIdent * netIdent ) = 0;
	/// Notify GUI that contact changed one or more of his/her plugins permission level required to access his/her plugin
	virtual void				toGuiPluginPermissionChange( VxNetIdent * netIdent ) = 0;
	/// Notify GUI that contact info of any type changed
	virtual void				toGuiContactAnythingChange( VxNetIdent * netIdent ) = 0;

	/// Notify GUI that the last time contact was in session with you changed
	virtual void				toGuiContactLastSessionTimeChange( VxNetIdent * netIdent ) = 0;

	//! Called from engine when need to update my online identity
	virtual void				toGuiUpdateMyIdent( VxNetIdent * netIdent ) = 0;

	//! Contact has sent a offer
	virtual void				toGuiRxedPluginOffer(  VxNetIdent *		netIdent, 
														EPluginType		ePluginType, 
														const char *	pMsg,
													    int				pvUserData,
														const char *	pFileName = NULL,
														uint8_t *		fileHashData = 0,
														VxGUID&			lclSessionId = VxGUID::nullVxGUID(),
														VxGUID&			rmtSessionId = VxGUID::nullVxGUID() ) = 0;
	//! Contact has responded to offer
	virtual void				toGuiRxedOfferReply(	VxNetIdent *	netIdent, 
														EPluginType		ePluginType,
														int				pvUserData,
														EOfferResponse	eOfferResponse, 
														const char *	pFileName = NULL,
														uint8_t *		fileHashData = 0,
														VxGUID&			lclSessionId = VxGUID::nullVxGUID(),
														VxGUID&			rmtSessionId = VxGUID::nullVxGUID() ) = 0;
	//! Plugin session has stopped
	virtual void				toGuiPluginSessionEnded(	VxNetIdent *	netIdent, 
															EPluginType		ePluginType,
															int				pvUserData,
															EOfferResponse	eOfferResponse,
															VxGUID&			lclSessionId = VxGUID::nullVxGUID() ) = 0;
	//! Plugin session status or variable value
	virtual void				toGuiPluginStatus(	EPluginType		ePluginType,
													int				statusType,
													int				statusValue ) = 0;
	//! Received text message from contact
	virtual void				toGuiInstMsg(	VxNetIdent *	netIdent,
												EPluginType		ePluginType,
												const char *	pMsg ) = 0;
	//! Send list of contacts shared files to GUI
	virtual void				toGuiFileListReply(	VxNetIdent *	netIdent, 
													EPluginType		ePluginType, 
													uint8_t			u8FileType, 
													uint64_t		u64FileLen, 
													const char *	pFileName,
													uint8_t *		fileHashData ) = 0;
	//! Send list of files to GUI.. used to send directory listing or shared files or files that are in library
	virtual void				toGuiFileList(	const char *	fileName, 
												uint64_t		fileLen, 
												uint8_t			fileType, 
												bool			isShared,
												bool			isInLibrary,
												uint8_t *		fileHashId = 0 ) = 0;
	//! Upload a file started
	virtual void				toGuiStartUpload(	VxNetIdent *	netIdent, 
													EPluginType		ePluginType, 
													VxGUID&			lclSessionId, 
													uint8_t			u8FileType, 
													uint64_t		u64FileLen, 
													const char *	pFileName,
													uint8_t *		fileHashData ) = 0;
	/// Upload a file completed
	virtual void				toGuiFileUploadComplete( VxGUID& lclSessionId, EXferError xferError ) = 0;
	/// Download a file started
	virtual void				toGuiStartDownload(	VxNetIdent *	netIdent, 
													EPluginType		ePluginType, 
													VxGUID&			lclSessionId, 
													uint8_t			u8FileType, 
													uint64_t		u64FileLen, 
													const char *	pFileName,
													uint8_t *		fileHashData ) = 0;
	/// Download a file completed
	virtual void				toGuiFileDownloadComplete( VxGUID& lclSessionId, const char * newFileName, EXferError xferError ) = 0;
	/// File transfer progress and/or state
	virtual void				toGuiFileXferState( VxGUID& lclSessionId, EXferState eXferState, int param1, int param2 ) = 0;

	/// Video file or audio file or emote icon or text message asset info in result of request to get Text Chat session assets
	virtual void				toGuiSessionHistory( AssetInfo * assetInfo ) = 0;
	/// Video file or audio file or emote icon or text message has been added to Text Chat session
	virtual void				toGuiAssetAdded( AssetInfo * assetInfo ) = 0;
	/// Asset state has changed ( like transfer has failed )
	virtual void				toGuiAssetAction( EAssetAction assetAction, VxGUID& assetId, int pos0to100000 ) = 0;
	/// Text Chat session action like video chat session requested
	virtual void				toGuiMultiSessionAction( EMSessionAction mSessionAction, VxGUID& onlineId, int pos0to100000 ) = 0;
	/// Game variable has changed ( Used by Truth Or Dare video chat game )
	virtual bool				toGuiSetGameValueVar(	EPluginType	    ePluginType, 
														VxGUID&		    oOnlineId, 
														int32_t			s32VarId, 
														int32_t			s32VarValue ) = 0;
	/// Game action has occurred ( Used by Truth Or Dare video chat game )
	virtual bool				toGuiSetGameActionVar(	EPluginType	    ePluginType, 
														VxGUID&		    oOnlineId, 
														int32_t			s32VarId, 
														int32_t			s32VarValue ) = 0;

	///============================================================================
	/// Scan network for user shared resources
	///============================================================================

	/// Search found an contact matching and accessible from this node resource
	virtual void				toGuiSearchResultSuccess( EScanType eScanType, VxNetIdent * netIdent ) = 0;
	/// Search had error scanning this user
	virtual void				toGuiSearchResultError( EScanType eScanType, VxNetIdent * netIdent, int errCode ) = 0;
	/// Search had completed or has no more contact nodes to search
	virtual void				toGuiScanSearchComplete( EScanType eScanType ) = 0;	
	/// Search has found and received a About Me picture from contact
	virtual void				toGuiSearchResultProfilePic(	VxNetIdent *	netIdent, 
																uint8_t *		pu8JpgData, 
																uint32_t		u32JpgDataLen ) = 0;
	/// Search has found a matching file
	virtual void				toGuiSearchResultFileSearch(	VxNetIdent *	netIdent, 		
																VxGUID&			lclSessionId, 
																uint8_t			u8FileType, 
																uint64_t		u64FileLen, 
																const char *	pFileName ) = 0;
	/// a module has changed state
	virtual void				toGuiModuleState( int moduleNum, int moduleState ) = 0;
};
