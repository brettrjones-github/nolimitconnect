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

#define MIN_SEARCH_TEXT_LEN 3

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

    eConnectReasonChatRoomAnnounce,
    eConnectReasonChatRoomJoin,
    eConnectReasonChatRoomSearch,

    eConnectReasonGroupAnnounce,
    eConnectReasonGroupJoin,
    eConnectReasonGroupSearch,

    eConnectReasonRandomConnectAnnounce,
    eConnectReasonRandomConnectJoin,
    eConnectReasonRandomConnectSearch,

    eConnectReasonRelayAnnounce,
    eConnectReasonRelayJoin,
    eConnectReasonRelaySearch,

    eConnectReasonAnnouncePing,
    eConnectReasonStayConnected,
    eConnectReasonPlugin,
    eConnectReasonOtherSearch,

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

namespace FirewallSettings
{
    enum EFirewallTestType
    {
        eFirewallTestUrlConnectionTest = 0,
        eFirewallTestAssumeNoFirewall = 1,
        eFirewallTestAssumeFirewalled = 2,

        eMaxFirewallTestType
    };
};

enum EGenderType
{
    eGenderTypeUnspecified = 0,
    eGenderTypeMale,
    eGenderTypeFemale,

    eMaxGenderType
};

enum EHostAnnounceStatus
{
    eHostAnnounceUnknown = 0,
    eHostAnnounceInvalidUrl = 1,
    eHostAnnounceQueryIdInProgress = 2,
    eHostAnnounceQueryIdSuccess = 3,
    eHostAnnounceQueryIdFailed = 4,
    eHostAnnounceConnecting = 5,
    eHostAnnounceConnectSuccess = 6,
    eHostAnnounceConnectFailed = 7,
    eHostAnnounceSendingJoinRequest = 8,
    eHostAnnounceSendJoinRequestFailed = 9,
    eHostAnnounceSuccess = 10,
    eHostAnnounceFail = 11,
    eHostAnnounceFailPermission = 12,
    eHostAnnounceFailConnectDropped = 13,
    eHostAnnounceInvalidParam = 14,
    eHostAnnouncePluginDisabled = 15,

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
    eHostJoinConnectSuccess = 6,
    eHostJoinConnectFailed = 7,
    eHostJoinSendingJoinRequest = 8,
    eHostJoinSendJoinRequestFailed = 9,
    eHostJoinSuccess = 10,
    eHostJoinFail = 11,
    eHostJoinFailPermission = 12,
    eHostJoinFailConnectDropped = 13,
    eHostJoinInvalidParam = 14,
    eHostJoinPluginDisabled = 15,

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
    eHostSearchConnectSuccess = 6,
    eHostSearchConnectFailed = 7,
    eHostSearchSendingSearchRequest = 8,
    eHostSearchSendSearchRequestFailed = 9,
    eHostSearchSuccess = 10,
    eHostSearchFail = 11,
    eHostSearchFailPermission = 12,
    eHostSearchFailConnectDropped = 13,
    eHostSearchInvalidParam = 14,
    eHostSearchPluginDisabled = 15,
    eHostSearchNoMatches = 16,
    eHostSearchCompleted = 17,

    eMaxHostSearchStatus
};

enum EHostServiceType
{
    eHostServiceInvalid = 0,
    eHostServiceNetworkHost,        // Tier 1 network host service
    eHostServiceGroupListing,       // Tier 2 group listing service
    eHostServiceGroup,              // Tier 3 host a group service ( group users are considered Tier 4 )
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
    eHostTypeChatRoom = 1,
    eHostTypeConnectTest = 2,
    eHostTypeGroup = 3,
    eHostTypeNetwork = 4,
    eHostTypeRandomConnect = 5,

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

enum EModuleState
{
    eModuleStateUnknown = 0,
    eModuleStateInitialized,
    eModuleStateDeinitialized,
    eModuleStateInitError,

    eMaxModuleState // must be last
};

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
    eNetworkStateTypeFailedResolveHostNetwork	    = 10,
    eNetworkStateTypeFailedResolveHostGroupList = 11,
    eNetworkStateTypeFailedResolveHostGroup = 12,

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
enum EPluginType
{
    //! NOTE: don't handle packets for ePluginTypeInvalid or ePluginTypeWebServer
    ePluginTypeInvalid			= 0,	//!< unknown or disabled

    ePluginTypeAdmin			= 1,	//!< Administration ( intended for updates but not currently used )
    ePluginTypeAboutMePage      = 2,	//!< about me page plugin ( using web page server )
    ePluginTypeAvatarImage      = 3,	//!< handle users avatar image
    ePluginTypeCamServer        = 4,	//!< Web cam broadcast plugin
    ePluginTypeFileServer       = 5,	//!< Shared files server
    ePluginTypeFileXfer         = 6,	//!< Offer/accept send a file person to person
    ePluginTypeMessenger        = 7,	//!< Text, voice and video message texting with voice phone, video chat and truth or dare game available in session
    ePluginTypeStoryboard       = 8,	//!< User editable story board web page server
    ePluginTypeTruthOrDare      = 9,	//!< Video Chat Truth Or Dare game  
    ePluginTypeVideoPhone       = 10,	//!< Video Chat with motion detect and stream recording
    ePluginTypeVoicePhone       = 11,	//!< VOIP audio only phone call
    ePluginTypeChatRoomClient   = 12,	//!< chat room user client plugin
    ePluginTypeChatRoomHost     = 13,	//!< chat room hosting plugin
    ePluginTypeConnectTestClient    = 14,	//!< Connection Test Client
    ePluginTypeConnectTestHost      = 15,	//!< Connection Test Service
    ePluginTypeGroupClient          = 16,	//!< group client
    ePluginTypeGroupHost            = 17,   //!< group hosting
    ePluginTypeRandomConnectClient  = 18,	//!< Random connect to another person client
    ePluginTypeRandomConnectHost    = 19,	//!< Random connect to another person hosting
    ePluginTypeNetworkClient        = 20,	//!< network client
    ePluginTypeNetworkHost          = 21,	//!< master network hosting
    ePluginTypeNetworkSearchList    = 22,	//!< group and chat room list for network search

    ePluginTypeRelay                = 23,	//!< Relay services plugin

    // NOTE: plugin types 24 - 47 not implemented .. reserved for future use
    eMaxImplementedPluginType	= 24, 
    // plugins 0-47 are part of PktAnnounce
    // plugins after 47 are system plugins and do not go out in announcement pkt
    eMaxUserPluginType = 48, // this marks end of announced permission
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
    eLangNorwegian,     //	(no) Norwegian
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
    eNetCmdHostReq					    = 5,		
    eNetCmdHostReply					= 6,		
    eNetCmdAboutMePage					= 7,		
    eNetCmdStoryboardPage				= 8,	
    eNetCmdQueryHostOnlineIdReq         = 9,
    eNetCmdQueryHostOnlineIdReply      = 10,

    eMaxNetCmdType
};

enum ENetCmdError
{
    eNetCmdErrorUnknown                 = 0,
    eNetCmdErrorNone                    = 1,
    eNetCmdErrorServiceDisabled         = 2,
    eNetCmdErrorPermissionLevel         = 3,  

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
const char * DescribeModuleState( EModuleState moduleState );
//! Network State as text
const char * DescribeNetworkState( ENetworkStateType networkStateType );
//! Net Available Status as text
const char * DescribeNetAvailStatus( ENetAvailStatus netAvailStatus );
//! Net Command type as text
const char * DescribeNetCmdType( ENetCmdType netCmdType );
//! Net Command Error as text
const char * DescribeNetCmdError( ENetCmdError netCmdError );
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
//! Describe scan type
const char * DescribeSearchType( ESearchType scearchType );

// for use in database mainly 
// If you add a plugin type be sure to update getPluginName
const char * getPluginName( EPluginType pluginType );
