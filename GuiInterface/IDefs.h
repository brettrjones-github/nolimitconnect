#pragma once
//============================================================================
// Copyright (C) 2013 Brett R. Jones 
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

#include <NetLib/VxXferDefs.h>

#define SECOND_MS               1000
#define SECONDS_2_MS            (SECOND_MS * 2)
#define SECONDS_3_MS            (SECOND_MS * 3)
#define SECONDS_5_MS            (SECOND_MS * 5)
#define SECONDS_10_MS           (SECOND_MS * 10)
#define SECONDS_30_MS           (SECOND_MS * 30)
#define MINUTE_MS               (SECOND_MS * 60)
#define MINUTES_15_MS           (MINUTE_MS * 15)
#define MINUTES_30_MS           (MINUTE_MS * 30)
#define HOUR_MS                 (MINUTE_MS * 60)
#define HOURS_24_MS             (HOUR_MS * 24)
#define HOURS_48_MS             (HOUR_MS * 48)
#define WEEK_MS                 (HOURS_24_MS * 7)
#define MONTH_MS                (WEEK_MS * 30)
#define YEAR_MS                 (MONTH_MS * 12)

#define MIN_SEARCH_TEXT_LEN     3

// NOTE: if you add or remove a enum then must update corresponding DescribeXXX( enumXXX )

enum EAppModule
{
    eAppModuleInvalid = 0,
    eAppModuleAll,
    eAppModuleKodi,
    eAppModulePtoP,
    eAppModuleTest,

    eMaxAppModule // must be last
};

enum EAgeType
{
    eAgeTypeUnspecified = 0,
    eAgeTypeUnder21,
    eAgeType21OrOlder,

    eMaxAgeType
};

enum ECacheType
{
    eCacheTypeNone = 0,
    eCacheTypeThumbnail,

    eMaxCacheType
};

enum ECommErr
{
    eCommErrNone                 = 0,
    eCommErrInvalidPkt,
    eCommErrUserOffline,
    eCommErrSearchTextToShort, 
    eCommErrSearchNoMatch, 
    eCommErrInvalidHostType, 
    eCommErrPluginNotEnabled,
    eCommErrPluginPermission,

    eMaxCommErr
};

enum EConnectReason
{
    eConnectReasonUnknown = 0,

    eConnectReasonGroupAnnounce,
    eConnectReasonGroupJoin,
    eConnectReasonGroupSearch,

    eConnectReasonChatRoomAnnounce,
    eConnectReasonChatRoomJoin,
    eConnectReasonChatRoomSearch,

    eConnectReasonRandomConnectAnnounce,
    eConnectReasonRandomConnectJoin,
    eConnectReasonRandomConnectSearch,

    eConnectReasonAnnouncePing,
    eConnectReasonStayConnected,
    eConnectReasonPlugin,
    eConnectReasonOtherSearch,

    eConnectReasonNearbyLan,
    eConnectReasonSameExternalIp,
    eConnectReasonReverseConnectRequested,
    eConnectReasonPktTcpPunch,
    eConnectReasonRelayService,

    eMaxConnectReason
};

enum EConnectStatus
{
    eConnectStatusUnknown = 0,

    eConnectStatusReady,
    eConnectStatusBadParam,
    eConnectStatusBadAddress,
    eConnectStatusPermissionDenied,
    eConnectStatusConnecting,
    eConnectStatusConnectFailed,
    eConnectStatusSendPktAnnFailed,
    eConnectStatusHandshaking,
    eConnectStatusHandshakeTimeout,
    eConnectStatusConnectSuccess,
    eConnectStatusDropped,
    eConnectStatusRxAnnTimeout,

    eMaxConnectStatus
};

enum EContentRating
{
    eContentRatingUnspecified = 0,
    eContentRatingFamily,
    eContentRatingAdult,
    eContentRatingXXX,
    eContentRatingDarkWeb,
    eContentRatingPersonal,

    eMaxContentRating
};

enum EContentCatagory
{
    eContentCatagoryUnspecified = 0,
    eContentCatagoryVideo,
    eContentCatagoryAudio,
    eContentCatagoryImage,
    eContentCatagoryText,
    eContentCatagoryPersonal,
    eContentCatagoryOther,

    eMaxContentCatagory
};

enum EUserViewType
{
    eUserViewTypeNone,
    eUserViewTypeFriends,
    eUserViewTypeGroup,
    eUserViewTypeChatRoom,
    eUserViewTypeRandomConnect,
    eUserViewTypeEverybody,
    eUserViewTypeIgnored,
    eUserViewTypeNearby,
    eUserViewTypeOnline,
    eUserViewTypeDirectConnect,

    eMaxUserViewTypeNone,
};

//! \public Enumeration of friendship type. NOTE: also used as plugin permission type
enum EFriendState
{
    eFriendStateIgnore		= 0,	//!< Unknown or disabled or ignore
    eFriendStateAnonymous	= 1,	//!< Anonymous contact
    eFriendStateGuest		= 2,	//!< Guest contact
    eFriendStateFriend		= 3,	//!< Friend contact
    eFriendStateAdmin		= 4		//!< Administrator contact
};

//! \public Enumeration of which contacts to show in contact list
enum EFriendViewType
{
    eFriendViewEverybody		= 0,
    eFriendViewAdministrators   = 1,
    eFriendViewFriendsAndGuests = 2,
    eFriendViewAnonymous		= 3,
    eFriendViewIgnored			= 4,
    eFriendViewMyProxies		= 5,
    eFriendViewAllProxies		= 6,
    eFriendViewRefresh			= 7,

    eMaxFriendViewType // must be last
};

enum EFirewallTestType
{
    eFirewallTestUrlConnectionTest = 0,
    eFirewallTestAssumeNoFirewall = 1,
    eFirewallTestAssumeFirewalled = 2,

    eMaxFirewallTestType
};

enum EGenderType
{
    eGenderTypeUnspecified = 0,
    eGenderTypeMale,
    eGenderTypeFemale,

    eMaxGenderType
};

enum EHackerLevel
{
    eHackerLevelUnknown,
    eHackerLevelSuspicious,
    eHackerLevelMedium,
    eHackerLevelSevere,

    eMaxHackerLevel
};

enum EHackerReason
{
    eHackerReasonUnknown,
    eHackerReasonPeerName,
    eHackerReasonHostByName,
    eHackerReasonNoHostIpAddr,
    eHackerReasonHostIpOptions,
    eHackerReasonNetCmdLength,
    eHackerReasonNetCmdListInvalid,
    eHackerReasonNetSrvUrlInvalid,
    eHackerReasonNetSrvPluginInvalid,
    eHackerReasonNetSrvQueryIdPermission,
    eHackerReasonHttpAttack,
    eHackerReasonPktOnlineIdMeFromMyIp,
    eHackerReasonPktOnlineIdMeFromAnotherIp,
    eHackerReasonPktAnnNotFirstPacket,
    eHackerReasonPktHdrInvalid,


    eMaxHackerReason
};

enum EHostAnnounceStatus
{
    eHostAnnounceUnknown = 0,
    eHostAnnounceInvalidUrl = 1,
    eHostAnnounceQueryIdInProgress = 2,
    eHostAnnounceQueryIdSuccess = 3,
    eHostAnnounceQueryIdFailed = 4,
    eHostAnnounceConnecting = 5,
    eHostAnnounceHandshaking = 6,
    eHostAnnounceHandshakeTimeout = 7,
    eHostAnnounceConnectSuccess = 8,
    eHostAnnounceConnectFailed = 9,
    eHostAnnounceSendingJoinRequest = 10,
    eHostAnnounceSendJoinRequestFailed = 11,
    eHostAnnounceSuccess = 12,
    eHostAnnounceFail = 13,
    eHostAnnounceFailPermission = 14,
    eHostAnnounceFailConnectDropped = 15,
    eHostAnnounceInvalidParam = 16,
    eHostAnnouncePluginDisabled = 17,
    eHostAnnounceDone = 18,

    eMaxHostAnnounceStatus
};

enum EHostJoinStatus
{
    eHostJoinUnknown = 0,
    eHostJoinInvalidUrl = 1,
    eHostJoinQueryIdInProgress = 2,
    eHostJoinQueryIdSuccess = 3,
    eHostJoinQueryIdFailed = 4,
    eHostJoinConnecting = 5,
    eHostJoinHandshaking = 6,
    eHostJoinHandshakeTimeout = 7,
    eHostJoinConnectSuccess = 8,
    eHostJoinConnectFailed = 9,
    eHostJoinSendingJoinRequest = 10,
    eHostJoinSendJoinRequestFailed = 11,
    eHostJoinSuccess = 12,
    eHostJoinFail = 13,
    eHostJoinFailPermission = 14,
    eHostJoinFailConnectDropped = 15,
    eHostJoinInvalidParam = 16,
    eHostJoinPluginDisabled = 17,
    eHostJoinDone = 18,

    eMaxHostJoinStatus
};

enum EHostSearchStatus
{
    eHostSearchUnknown = 0,
    eHostSearchInvalidUrl = 1,
    eHostSearchQueryIdInProgress = 2,
    eHostSearchQueryIdSuccess = 3,
    eHostSearchQueryIdFailed = 4,
    eHostSearchConnecting = 5,
    eHostSearchHandshaking = 6,
    eHostSearchHandshakeTimeout = 7,
    eHostSearchConnectSuccess = 8,
    eHostSearchConnectFailed = 9,
    eHostSearchSendingSearchRequest = 10,
    eHostSearchSendSearchRequestFailed = 11,
    eHostSearchSuccess = 12,
    eHostSearchFail = 13,
    eHostSearchFailPermission = 14,
    eHostSearchFailConnectDropped = 15,
    eHostSearchInvalidParam = 16,
    eHostSearchPluginDisabled = 17,
    eHostSearchNoMatches = 18,
    eHostSearchCompleted = 19,
    eHostSearchDone = 20,

    eMaxHostSearchStatus
};

enum EHostServiceType
{
    eHostServiceInvalid = 0,
    eHostServiceNetworkHost,        // Tier 1 network host service
    eHostServiceGroupListing,       // Tier 2 group listing service
    eHostServiceGroup,              // Tier 3 host a group service ( group users are considered Tier 4 )
    eHostServiceChatRoom,           // Tier 3 host a chat room service ( chat room users are considered Tier 4 )
    eHostServiceRelay,              // relay service normally provided by group host ( Tier 3 )
    eHostServiceRandomConnect,      // random connect service normally provided by network host ( Tier 1 )
    eHostServiceRandomConnectRelay, // random connection relay service normally provided by network host ( Tier 1 )
    eHostServiceConnectTest,        // test connection for relay requirement and IP Address  ( normally provided by all Tiers )

    eMaxHostServiceType
};

//! \public Host connection test state
enum EHostTestStatus
{
    eHostTestStatusUnknown = 0,
    eHostTestStatusLogMsg = 1,

    eHostTestStatusHostOk = 2,
    eHostTestStatusHostConnectFail = 3,
    eHostTestStatusHostConnectionDropped = 4,
    eHostTestStatusHostTestComplete = 5,

    eHostTestStatusNetServiceOk = 6,
    eHostTestStatusNetServiceConnectFail = 7,
    eHostTestStatusNetServiceConnectionDropped = 8,
    eHostTestStatusNetServiceTestComplete = 9,

    eMaxHostTestStatusType
};

enum EHostType
{
    eHostTypeUnknown = 0,
    eHostTypeConnectTest = 1,
    eHostTypeNetwork = 2,
    eHostTypePeerUser = 3,
    // these hosts provide relay services and are announced to network hosts
    eHostTypeGroup = 4,
    eHostTypeChatRoom = 5,
    eHostTypeRandomConnect = 6,

    eMaxHostType
};

enum EInternetStatus
{
    eInternetNoInternet = 0,
    eInternetInternetAvailable,
    eInternetTestHostUnavailable,
    eInternetTestHostAvailable,
    eInternetAssumeDirectConnect,
    eInternetCanDirectConnect,
    eInternetRequiresRelay,

    eMaxInternetStatus
};

enum EJoinState
{
    eJoinStateNone     = 0,
    eJoinStateSending,
    eJoinStateSendFail,
    eJoinStateSendAcked,
    eJoinStateJoinRequested,
    eJoinStateJoinAccepted,
    eJoinStateJoinDenied,

    eMaxJoinState	
};

enum EListAction
{
    eListActionAnnounced,
    eListActionJoinedHost,
    eListActionJoinedClient,

    eMaxListAction
};

enum EMembershipState
{
    eMembershipStateNone = 0,
    eMembershipStateCanBeRequested,
    eMembershipStateJoined,
    eMembershipStateJoinDenied,

    eMaxMembershipState
};

enum EModuleState
{
    eModuleStateUnknown = 0,
    eModuleStateInitialized,
    eModuleStateDeinitialized,
    eModuleStateInitError,

    eMaxModuleState // must be last
};

enum ENetActionType
{
    eNetActionUnknown = 0,
    eNetActionIdle = 1,
    eNetActionIsPortOpen = 2,

    eMaxNetAction
};

//! \public Enumeration of Network State Machine states/actions
enum ENetworkStateType
{
    eNetworkStateTypeUnknown				= 0,
    eNetworkStateTypeLost					= 1,
    eNetworkStateTypeAvail					= 2,
    eNetworkStateTypeTestConnection			= 3,
    eNetworkStateTypeOnlineDirect			= 4,
    eNetworkStateTypeWaitForRelay           = 5,
    eNetworkStateTypeOnlineThroughRelay		= 6,
    eNetworkStateTypeNoInternetConnection	= 7,
    eNetworkStateTypeFailedResolveHostNetwork	= 8,
    eNetworkStateTypeFailedResolveHostGroupList = 9,
    eNetworkStateTypeFailedResolveHostGroup     = 10,

    eMaxNetworkStateType
};

//! \public Enumeration of session offer response
enum EOfferResponse
{
    eOfferResponseNotSet		= 0,	//!< Unknown or not set offer response from contact
    eOfferResponseAccept		= 1,	//!< Contact accepted offer
    eOfferResponseReject		= 2,	//!< Contact rejected offer
    eOfferResponseBusy			= 3,	//!< Contact cannot accept session because already in session
    eOfferResponseCancelSession	= 4,	//!< Contact exited session
    eOfferResponseEndSession	= 5,	//!< Session end because of any reason
    eOfferResponseUserOffline	= 6		//!< Session end because contact is not online
};

//! public Enumeration of offer state
enum EOfferState
{
    eOfferStateNone                 = 0,	// no offer state
    eOfferStateSending              = 1,
    eOfferStateSent                 = 2,
    eOfferStateSendFailed           = 3,
    eOfferStateRxedByUser           = 4,    // recieved by destination user
    eOfferStateBusy                 = 5,    // destination user is already in session
    eOfferStateAccepted             = 6,
    eOfferStateRejected             = 7,
    eOfferStateCanceled             = 8,
    eOfferStateUserOffline          = 9,
    eOfferStateInSession            = 10,   // in session or in xfer progress
    eOfferStateSessionComplete      = 11,   // session or xfer completed
    eOfferStateSessionFailed        = 12,   // session or xfer failed

    eMaxOfferState
};

//! public Enumeration of offer
enum EOfferType
{
    eOfferTypeUnknown		        = 0,	//!< Unknown or no offer
    eOfferTypeJoinGroup		        = 1,
    eOfferTypeJoinChatRoom		    = 2,
    eOfferTypeJoinRandomConnect		= 3,
    eOfferTypePhotoFile		        = 4,
    eOfferTypeAudioFile		        = 5,
    eOfferTypeVideoFile		        = 6,
    eOfferTypeDocumentFile		    = 7,
    eOfferTypeArchiveFile		    = 8,    // archive or cd/dvd image
    eOfferTypeExecutableFile		= 9,    // exe files are not normally allowed but here for error notification
    eOfferTypeOtherFile		        = 10,   // unrecognized file type
    eOfferTypeDirectory		        = 11,   // directory of files
    eOfferTypeFriendship		    = 12, 
    eOfferTypeMessenger		        = 13,   // instant message session 
    eOfferTypeTruthOrDare           = 14,	// Video Chat Truth Or Dare game  
    eOfferTypeVideoPhone            = 15,	// Video Chat with motion detect and stream recording
    eOfferTypeVoicePhone            = 16,	// VOIP audio only phone call

    eOfferTypeChatText				= 17,
    eOfferTypeChatFace				= 18,
    eOfferTypeChatStockAvatar		= 19,
    eOfferTypeChatCustomAvatar		= 20,
    eOfferTypeThumbnail             = 21,
    eOfferTypeCamRecord             = 22,

    eMaxOfferType
};

enum EOfferLocation
{
    eOfferLocUnknown				= 0x00,
    eOfferLocLibrary				= 0x01,
    eOfferLocShared					= 0x02,
    eOfferLocPersonalRec			= 0x04,
    eOfferLocThumbDirectory         = 0x08,
    eOfferLocCamRecord              = 0x10
};

enum EOfferAction
{
    eOfferActionUnknown				= 0,
    eOfferActionDeleteFile			= 1,
    eOfferActionShreadFile			= 2,
    eOfferActionAddToOfferMgr		= 3,
    eOfferActionRemoveFromOfferMgr	= 4,	
    eOfferActionUpdateOffer			= 5,
    eOfferActionAddOfferAndSend		= 6,
    eOfferActionOfferResend			= 7,

    eOfferActionAddToShare			= 8,
    eOfferActionRemoveFromShare		= 9,
    eOfferActionAddToLibrary		= 10,
    eOfferActionRemoveFromLibrary	= 11,
    eOfferActionAddToHistory		= 12,
    eOfferActionRemoveFromHistory	= 13,

    eOfferActionRecordBegin			= 14,
    eOfferActionRecordPause			= 15,
    eOfferActionRecordResume		= 16,
    eOfferActionRecordProgress		= 17,
    eOfferActionRecordEnd			= 18,
    eOfferActionRecordCancel		= 19,

    eOfferActionPlayBegin			= 20,
    eOfferActionPlayOneFrame		= 21,
    eOfferActionPlayPause			= 22,
    eOfferActionPlayResume			= 23,
    eOfferActionPlayProgress		= 24,
    eOfferActionPlayEnd				= 25,
    eOfferActionPlayCancel			= 26,

    eOfferActionTxBegin				= 27,
    eOfferActionTxProgress			= 28,
    eOfferActionTxSuccess			= 29,
    eOfferActionTxError				= 30,
    eOfferActionTxCancel			= 31,
    eOfferActionTxPermission		= 32,

    eOfferActionRxBegin				= 33,
    eOfferActionRxProgress			= 34,
    eOfferActionRxSuccess			= 35,
    eOfferActionRxError				= 36,
    eOfferActionRxCancel			= 37,
    eOfferActionRxPermission		= 38,

    eOfferActionRxNotifyNewMsg		= 39,
    eOfferActionRxViewingMsg		= 40,
};

enum EOfferMgrType
{
    eOfferMgrTypeNone,
    eOfferMgrTypeOfferBase,
    eOfferMgrTypeOfferClient,
    eOfferMgrTypeOfferHost,

    eMaxOfferMgrType
};

enum EOfferSendState
{
    eOfferSendStateNone     = 0,
    eOfferSendStateTxProgress,
    eOfferSendStateRxProgress,
    eOfferSendStateTxSuccess,
    eOfferSendStateTxFail,
    eOfferSendStateRxSuccess,
    eOfferSendStateRxFail,
    eOfferSendStateTxPermissionErr,
    eOfferSendStateRxPermissionErr,

    eMaxOfferSendState	
};

enum EOnlineState
{
    eOnlineStateUnknown         = 0,
    eOnlineStateConnecting      = 1,
    eOnlineStateConnectFailed   = 2,
    eOnlineStateOnline          = 3,
    eOnlineStateOffline         = 4,

    eMaxOnlineState	
};

//! \public Enumeration of plugin accessibility
enum EPluginAccess
{
    ePluginAccessNotSet			= 0,		//!< Plugin access not initialized
	ePluginAccessOk				= 1,		//!< Plugin access allowed
	ePluginAccessLocked			= 2,		//!< Insufficient Friend permission level
	ePluginAccessDisabled		= 3,		//!< Plugin disabled or no files shared or no web cam broadcast
	ePluginAccessIgnored		= 4,		//!< Access denied because you are being ignored
	ePluginAccessInactive		= 5,		//!< Enabled and have access but no shared files or web cam
	ePluginAccessBusy			= 6,		//!< Sufficient permission but cannot accept any more sessions
	ePluginAccessRequiresDirectConnect = 7,	//!< Plugin access requires contact have direct connection
	ePluginAccessRequiresOnline		= 8,	//!< Plugin access requires contact to be online

    eMaxPluginAccessState
};

//! \public Enumerated plugins
enum EPluginType // NOTE: update PluginTypeEnumStrings and GuiParams::describePluginType if you change this
{
    //! NOTE: don't handle packets for ePluginTypeInvalid or ePluginTypeWebServer
    ePluginTypeInvalid			    = 0,	//!< unknown or disabled

    ePluginTypeAdmin			    = 1,	//!< Administration ( intended for updates but not currently used )
    ePluginTypeAboutMePage          = 2,	//!< about me page plugin ( using web page server )
    ePluginTypeCamServer            = 3,	//!< Web cam broadcast plugin
    ePluginTypeFileServer           = 4,	//!< Shared files server
    ePluginTypeFileXfer             = 5,	//!< Offer/accept send a file person to person
    ePluginTypeMessenger            = 6,	//!< Text, voice and video message texting with voice phone, video chat and truth or dare game available in session
    ePluginTypeStoryboard           = 7,	//!< User editable story board web page server
    ePluginTypeTruthOrDare          = 8,	//!< Video Chat Truth Or Dare game  
    ePluginTypeVideoPhone           = 9,	//!< Video Chat with motion detect and stream recording
    ePluginTypeVoicePhone           = 10,	//!< VOIP audio only phone call
    ePluginTypePushToTalk           = 11,	//!< VOIP audio push to talk

    ePluginTypeClientGroup          = 12,	//!< group client
    ePluginTypeClientChatRoom       = 13,	//!< chat room user client plugin
    ePluginTypeClientRandomConnect  = 14,	//!< Random connect to another person client
    ePluginTypeClientPeerUser       = 15,	//!< mainly for avatar image
    ePluginTypeClientNetwork        = 16,	//!< network client
    ePluginTypeClientConnectTest    = 17,	//!< Connection Test Client

    ePluginTypeHostGroup            = 18,   //!< group hosting
    ePluginTypeHostChatRoom         = 19,	//!< chat room hosting plugin
    ePluginTypeHostRandomConnect    = 20,	//!< Random connect to another person hosting
    ePluginTypeHostPeerUser         = 21,	//!< mainly for avatar image
    ePluginTypeHostNetwork          = 22,	//!< master network hosting
    ePluginTypeHostConnectTest      = 23,	//!< Connection Test Service

    ePluginTypeNetworkSearchList    = 24,	//!< group and chat room list for network search

    ePluginTypeRelay                = 25,	//!< Relay services plugin

    // NOTE: plugin types 25 - 47 not implemented .. reserved for future use
    eMaxImplementedPluginType	    = 26, 
    // plugins 0-47 are part of PktAnnounce
    // plugins after 47 are system plugins and do not go out in announcement pkt
    eMaxUserPluginType              = 48, // this marks end of announced permission
    ePluginTypeCameraService,
    ePluginTypeMJPEGReader,
    ePluginTypeMJPEGWriter,
    ePluginTypePersonalRecorder,
    ePluginTypeNetServices,
    ePluginTypeSearch,
    ePluginTypeSndReader,
    ePluginTypeSndWriter,
    ePluginTypeWebServer,	//!< Web server plugin ( for About Me and Story Board web pages )

    eMaxPluginType	
};

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

//! \public In Text Chat Session Actions
enum EMSessionAction
{
	eMSessionActionNone = 0,
	eMSessionActionChatSessionReq,
	eMSessionActionChatSessionAccept,
	eMSessionActionChatSessionReject,
	eMSessionActionOffer,
	eMSessionActionAccept,
	eMSessionActionReject,
	eMSessionActionHangup,

	eMaxMSessionAction
};

//! \public Session Types
enum EMSessionType
{
	eMSessionTypePhone 						= 0,
	eMSessionTypeVidChat 					= 1,
	eMSessionTypeTruthOrDare 				= 2,

	eMaxMSessionType
};

enum ELanguageType
{
    eLangUnspecified = 0,
    eLangEnglish,       //  (en) English
    eLangBulgarian,	    //  (bg) Bulgarian
    eLangChinese,	    //  (ch) Chinese
    eLangCroatian,      //  (hr) Croatian
    eLangCzech,         //  (cs) Czech
    eLangDanish,        //  (da) Danish
    eLangDutch,         //  (nl) Dutch
    eLangEstonian,      //  (et) Estonian
    eLangFinnish,       //  (fi) Finnish
    eLangFrench,        //	(fr) French
    eLangGerman,        //	(de) German
    eLangGreek,         //  (el) Greek
    eLangHindi,         //  (hi) Hindi
    eLangHungarian,     //	(hu) Hungarian
    eLangItalian,       //	(it) Italian
    eLangJapanese,      //	(jp) Japanese
    eLangLatvian,       //	(lv) Latvian
    eLangLithuanian,    //	(lt) Lithuanian
    eLangPolish,        //	(pl) Polish
    eLangPortuguese,    //	(pt) Portuguese
    eLangRomanian,      //	(ro) Romanian
    eLangRussian,       //	(ru) Russian
    eLangSerbian,       //  (sr) Serbian
    eLangSlovak,        //	(sl) Slovak
    eLangSpanish,       //	(es) Spanish
    eLangSwedish,       //	(sv) Swedish
    eLangThai,          //  (th) Thiawanese
    eLangTurkish,       //  (tr) Turkish
    eLangUkrainian,     //	(uk) Ukrainian

    eMaxLanguageType
};

//! \public Network protocol layer
enum ENetLayerType
{
    eNetLayerTypeUndefined = 0,		    //< not specified/initialized
    eNetLayerTypeInternet,		        //< can communicate with internet
    eNetLayerTypePtoP,		            //< tcp and extern ip found
    eNetLayerTypeNetHost,		        //< network host available
    eNetLayerTypeNetGroup,		        //< network host available

    eMaxNetLayerType		            //< max must be last
};

enum ENetLayerState
{
    eNetLayerStateUndefined = 0,		//< not specified/initialized
    eNetLayerStateWrongType,		    //< invalid or not relevant net layer type
    eNetLayerStateTesting,		        //< testing for available 
    eNetLayerStateFailed,		        //< failed or some issue
    eNetLayerStateAvailable,		    //< available and ready for use

    eMaxNetLayerState		            //< max must be last
};

enum ENetAvailStatus
{
    eNetAvailNoInternet = 0,
    eNetAvailHostAvail,
    eNetAvailP2PAvail,
    eNetAvailOnlineButNoRelay,
    eNetAvailFullOnlineWithRelay,
    eNetAvailFullOnlineDirectConnect,
    eNetAvailRelayGroupHost,
    eNetAvailDirectGroupHost,

    eMaxNetAvailStatus
};

enum ENetCmdType
{
    eNetCmdUnknown						= 0,
    eNetCmdPing							= 1,		
    eNetCmdPong							= 2,		
    eNetCmdIsMyPortOpenReq				= 3,		
    eNetCmdIsMyPortOpenReply			= 4,			
    eNetCmdQueryHostOnlineIdReq         = 5,
    eNetCmdQueryHostOnlineIdReply       = 6,

    eMaxNetCmdType
};

enum ENetCmdError
{
    eNetCmdErrorUnknown                 = 0,
    eNetCmdErrorNone                    = 1,
    eNetCmdErrorServiceDisabled         = 2,
    eNetCmdErrorPermissionLevel         = 3,  
    eNetCmdErrorFailedResolveIpAddr     = 4,
    eNetCmdErrorInvalidContent          = 5,
    eNetCmdErrorPortIsClosed            = 6,
    eNetCmdErrorConnectFailed           = 7,
    eNetCmdErrorTxFailed                = 8,
    eNetCmdErrorRxFailed                = 9,
    eNetCmdErrorBadParameter            = 10,
    eNetCmdErrorResponseTimedOut        = 11,

    eMaxNetCmdError
};

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

//! \public Enumeration of relay status
enum ERelayStatus
{
    eRelayStatusUnknown			= -1,	//!< Unknown
    eRelayStatusOk				= 0,	//!< Relay enabled and access accepted 
    eRelayStatusPermissionErr	= 1,	//!< Insufficient permission
    eRelayStatusBusy			= 2,	//!< Relay cannot accept any more session

    eMaxRelayStatus
};


//! \public Connect by shaking phone ( or press simulate phone shake ) status
enum ERandomConnectStatus
{
    eRandomConnectStatusUnknown						= 0,
    eRandomConnectStatusContactHostFail				= 1,
    eRandomConnectStatusFoundFriend					= 2,
    eRandomConnectStatusEmptyList					= 3,
    eRandomConnectStatusSendRequestFail				= 4,
    eRandomConnectStatusInvalidResponse				= 5,
    eRandomConnectStatusDecryptError				= 6,
    eRandomConnectStatusSearchComplete				= 7,

    eMaxRandomConnectStatusType
};

//! \public run a test like query host id state
enum ERunTestStatus
{
    eRunTestStatusUnknown = 0,
    eRunTestStatusLogMsg = 1,

    eRunTestStatusTestSuccess = 2,
    eRunTestStatusTestFail = 3,
    eRunTestStatusTestBadParam = 4,
    eRunTestStatusAlreadyQueued = 5,
    eRunTestStatusConnectFail = 6,
    eRunTestStatusConnectionDropped = 7,
    eRunTestStatusInvalidResponse = 8,
    eRunTestStatusMyPortIsOpen = 9,
    eRunTestStatusMyPortIsClosed = 10,
    eRunTestStatusTestComplete = 11,
    eRunTestStatusTestCompleteFail = 12,
    eRunTestStatusTestCompleteSuccess = 13,

    eMaxRunTestStatusType
};

//! \public Scan/search type enumeration
enum EScanType
{
    eScanTypeNone,				    //!< Unknown or not set search type
    eScanTypeChatRoomJoinSearch,	//!< Search for Chat Room to Join
    eScanTypeGroupJoinSearch,	    //!< Search for Group to Join
    eScanTypeRandomConnect,		    //!< Get contacts who have done phone shake connect in last 20 seconds
    eScanTypePeopleSearch,		    //!< Search for contact of given name 
    eScanTypeMoodMsgSearch,		    //!< Search for contacts with given text in mood message
    eScanTypeProfilePic,		    //!< Search for contacts with not default About Me Web Page picture
    eScanTypeCamServer,			    //!< Search for contacts with shared web cam
    eScanTypeFileSearch,		    //!< Search for contacts with shared files containing given search text
    eScanTypeStoryBoard,		    //!< Search for contacts with modified Story Board Web Page

    eMaxScanType
};

enum ESearchType
{
    eSearchNone,
    eSearchChatRoomHost,	        //!< Search for Chat Room to Join
    eSearchGroupHost,	            //!< Search for Group to Join
    eSearchRandomConnectHost,		//!< Search for Random Connect Server to Join

    eMaxSearchType
};

enum ESktType
{
    eSktTypeNone				= 0,
    eSktTypeTcpConnect			= 1,
    eSktTypeTcpAccept			= 2,
    eSktTypeUdp					= 3,
    eSktTypeUdpBroadcast		= 4,
    eSktTypeLoopback    		= 5,

    eMaxSktType			// always last
};

enum ESktCloseReason
{
    eSktCloseReasonUnknown				= 0,
    eSktCloseNotNeeded                  = 1,
    eSktCloseHackLevelUnknown           = 2,
    eSktCloseHackLevelSuspicious        = 3,
    eSktCloseHackLeveMedium             = 4,
    eSktCloseHackLevetSevere            = 5,
    eSktCloseAcceptFailed               = 6,
    eSktCloseConnectionLost             = 7,
    eSktCloseConnectTimeout             = 8,
    eSktCloseSktDestroy                 = 9,
    eSktCloseAll                        = 10,
    eSktCloseDisconnectAfterSend        = 11,
    eSktCloseSendComplete               = 12,
    eSktCloseSktWithError               = 13,
    eSktCloseCryptoNullData             = 14,
    eSktCloseCryptoInvalidLength        = 15,
    eSktCloseCryptoInvalidKey,
    eSktClosePktLengthInvalid,
    eSktCloseUdpCreate,
    eSktCloseUpnpStart,
    eSktCloseUpnpDone,
    eSktCloseMulticastListenDone,
    eSktCloseWebBrowser,
    eSktCloseImAliveTimeout,
    eSktClosePingTimeout,
    eSktCloseSktHandleInvalid,
    eSktCloseNetCmdLengthInvalid,
    eSktCloseNetCmdListInvalid,
    eSktCloseNetSrvUrlInvalid,
    eSktCloseNetSrvPluginInvalid,
    eSktCloseNetSrvQueryIdPermission,
    eSktCloseNetSrvQueryIdSent,
    eSktCloseHttpUrlInvalid,
    eSktCloseHttpPluginInvalid,
    eSktCloseHttpHandleError,
    eSktCloseHttpNotAcceptSkt,
    eSktCloseHttpCloseNoError,
    eSktCloseHttpCloseWithError,
    eSktCloseHttpNoWebSrv,
    eSktCloseHttpSktDestroy,
    eSktCloseConnectReasonsEmpty,
    eSktCloseFindBigInfoFail,
    eSktCloseFindConnectedInfoFail,
    eSktClosePktAnnSendFail,
    eSktCloseToRelayPktAnnSendFail,
    eSktCloseThroughRelayPktAnnSendFail,
    eSktClosePktOnlineIdMeFromMyIp,
    eSktClosePktOnlineIdMeFromAnotherIp,
    eSktClosePktAnnNotFirstPacket,
    eSktClosePktAnnInvalid,
    eSktClosePktHdrInvalid,
    eSktClosePktPingReqSendFail,
    eSktCloseP2PNotReadyForAcceptSkt,
    eSktCloseUserIgnored,
    eSktCloseRelaySessionNotFound,


    eMaxSktCloseReason			// always last
};

enum ESubCatagory
{
    eSubCatagoryUnspecified = 0,

    eSubCatagoryVideoOther,
    eSubCatagoryVideoPersonal,
    eSubCatagoryVideoXXX,
    eSubCatagoryVideoMovie,
    eSubCatagoryVideoMovieClip,
    eSubCatagoryVideoTvShow,
    eSubCatagoryVideo3d,
    eSubCatagoryVideoCam,

    eSubCatagoryAudioOther,
    eSubCatagoryAudioPersonal,
    eSubCatagoryAudioXXX,
    eSubCatagoryAudioMusic,
    eSubCatagoryAudioBook,
    eSubCatagoryAudioSoundClip,

    eSubCatagoryImageOther,
    eSubCatagoryImagePersonal,
    eSubCatagoryImageXXX,
    eSubCatagoryImagePictures,
    eSubCatagoryImageCovers,

    eSubCatagoryOtherPersonal,
    eSubCatagoryOtherXXX,
    eSubCatagoryOtherEBook,
    eSubCatagoryOtherComic,

    eMaxSubCatagory
};

//============================================================================
// There is a qt translated version of these functions in GuiParams for the user interface
//============================================================================

const char * DescribeAppModule( EAppModule appModule );
const char * DescribeAge( EAgeType ageType );
const char * DescribeCommError( ECommErr commErr );
const char * DescribeConnectReason( EConnectReason connectReason );
const char * DescribeConnectStatus( EConnectStatus connectStatus );
//! describe friend state
const char * DescribeFriendState( EFriendState eFriendState );
const char * DescribeHackerLevel( EHackerLevel hackLevel );
const char * DescribeHackerReason( EHackerReason hackReason );
//! Host announce status as text
const char * DescribeHostAnnounceStatus( EHostAnnounceStatus hostStatus );
//! Host join status as text
const char * DescribeHostJoinStatus( EHostJoinStatus hostStatus );
//! Host search status as text
const char * DescribeHostSearchStatus( EHostSearchStatus hostStatus );
//! Host connection test state as text
// const char * DescribeHostTestStatus( EHostTestStatus eHostStatus );
//! Host type as text
const char * DescribeHostType( EHostType hostType );
//! Internet Status as text
const char * DescribeInternetStatus( EInternetStatus internetStatus );
const char * DescribeJoinState( EJoinState joinState );
const char * DescribeModuleState( EModuleState moduleState );
const char * DescribeListAction( EListAction listAction );
const char * DescribeNetAction( ENetActionType netAction );
//! Network State as text
const char * DescribeNetworkState( ENetworkStateType networkStateType );
//! Net Available Status as text
const char * DescribeNetAvailStatus( ENetAvailStatus netAvailStatus );
//! Net Command type as text
const char * DescribeNetCmdType( ENetCmdType netCmdType );
//! Net Command Error as text
const char * DescribeNetCmdError( ENetCmdError netCmdError );
//! Offer state as text
const char * DescribeOfferState( EOfferState offerState );
//! Offer type as text
const char * DescribeOfferType( EOfferType offerType );
//! Plugin Access as text
const char * DescribePluginAccess( EPluginAccess pluginAccess );
//! Describe plugin.. the first DescribePluginType is translated.. this one is not
const char * DescribePluginType( EPluginType ePluginType );
//! Describe Direct Connect test state as text
const char * DescribePortOpenStatus( EIsPortOpenStatus ePortOpenStatus );
//! Describe connect by shaking phone ( or press simulate phone shake ) status as text
const char * DescribeRandomConnectStatus( ERandomConnectStatus ePortOpenStatus );
//! Describe Relay Search Status as text
const char * DescribeRelayStatus( EMyRelayStatus eRelayStatus );
//! Describe run network test state as text
const char * DescribeRunTestStatus( ERunTestStatus eTestStatus );
//! Describe scan type
const char * DescribeScanType( EScanType scanType );
//! Describe search type
const char * DescribeSearchType( ESearchType searchType );
//! Describe socket close reason
const char * DescribeSktCloseReason( ESktCloseReason closeReason );
//! Describe skt type
const char * DescribeSktType( ESktType sktType );

// for use in database mainly 
// If you add a plugin type be sure to update getPluginName
const char * getPluginName( EPluginType pluginType );

//! convert Host Type to connect reason
EConnectReason HostTypeToConnectAnnounceReason( EHostType hostType );
//! convert Host Type to connect reason
EConnectReason HostTypeToConnectJoinReason( EHostType hostType );
//! convert Host Type to connect reason
EConnectReason HostTypeToConnectSearchReason( EHostType hostType );

//! convert Host Type to service plugin  type
EPluginType HostTypeToHostPlugin( EHostType hostType );
//! convert Host Type to client plugin type
EPluginType HostTypeToClientPlugin( EHostType hostType );
//! convert Plugin Type to host type
EHostType PluginTypeToHostType( EPluginType pluginType );
// if host plugin return its client plugin else return pluginType param
EPluginType HostPluginToClientPluginType( EPluginType pluginType );
// if client plugin return its host plugin else return pluginType param
EPluginType ClientPluginToHostPluginType( EPluginType pluginType );
//! return true if is a client plugin
bool IsClientPluginType( EPluginType pluginType );
//! return true if is a host plugin
bool IsHostPluginType( EPluginType pluginType );
