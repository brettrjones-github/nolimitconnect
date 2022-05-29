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
// http://www.nolimitconnect.org
//============================================================================

//! \public Enumerated plugins
enum EPluginType
{
	//! NOTE: don't handle packets for ePluginTypeInvalid or ePluginTypeWebServer
	ePluginTypeInvalid			= 0,	//!< unknown or disabled

	ePluginTypeAdmin			= 1,	//!< Administration ( intended for updates but not currently used )
    ePluginTypeAboutMePage      = 2,	//!< about me page plugin ( using web page server )
    ePluginTypeAvatarImage      = 3,	//!< handle users avatar image
    ePluginTypeCamServer        = 4,	//!< Web cam broadcast plugin
    ePluginTypeChatRoom         = 5,	//!< chat room host plugin
    ePluginTypeFileServer       = 6,	//!< Shared files server
    ePluginTypeFileXfer         = 7,	//!< Offer/accept send a file person to person
    ePluginTypeMessenger        = 8,	//!< Text, voice and video message texting with voice phone, video chat and truth or dare game available in session
    ePluginTypeStoryboard       = 9,	//!< User editable story board web page server
    ePluginTypeTruthOrDare      = 10,	//!< Video Chat Truth Or Dare game  
    ePluginTypeVideoPhone       = 11,	//!< Video Chat with motion detect and stream recording
    ePluginTypeVoicePhone       = 12,	//!< VOIP audio only phone call
    ePluginTypeHostGroup        = 13,	//!< group host
    ePluginTypeHostNetwork      = 14,	//!< master network host
    ePluginTypeServiceConnectTest = 15,	//!< Connection Test Service

    ePluginTypeClientConnectTest = 16,	//!< Connection Test Client
    ePluginTypeHostGroupListing = 17,	//!< group list host
    ePluginTypeRandomConnect    = 18,	//!< Random connect to another person ( Using Phone Shake )
    ePluginTypeRandomConnectRelay = 19,	//!< Random connect to another person relay service
    ePluginTypeRelay            = 20,	//!< Relay services plugin

	// NOTE: plugin types 20 - 47 not implemented .. reserved for future use
	eMaxImplementedPluginType	= 21, 
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

// for use in database mainly 
// If you add a plugin type be sure to update getPluginName
const char * getPluginName( EPluginType pluginType );

//! \public Enumeration of plugin accessibility
enum EPluginAccessState
{
	ePluginAccessOk				= 0,		//!< Plugin access allowed
	ePluginAccessLocked			= 1,		//!< Insufficient Friend permission level
	ePluginAccessDisabled		= 2,		//!< Plugin disabled or no files shared or no web cam broadcast
	ePluginAccessIgnored		= 3,		//!< Access denied because you are being ignored
	ePluginAccessInactive		= 4,		//!< Enabled and have access but no shared files or web cam
	ePluginAccessBusy			= 5,		//!< Sufficient permission but cannot accept any more sessions
	ePluginAccessRequiresDirectConnect = 6,	//!< Plugin access requires contact have direct connection
	ePluginAccessRequiresOnline		= 7		//!< Plugin access requires contact to be online
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

//! \public In Text Chat Session Actions
enum EMSessionAction
{
	eMSessionActionNone,
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
	eRandomConnectStatusEmptyList						= 3,
	eRandomConnectStatusSendRequestFail				= 4,
	eRandomConnectStatusInvalidResponse				= 5,
	eRandomConnectStatusDecryptError					= 6,
	eRandomConnectStatusSearchComplete					= 7,

	eMaxRandomConnectStatusType
};

enum EGenderType
{
    eGenderTypeUnspecified,
    eGenderTypeMale,
    eGenderTypeFemale,

    eMaxGenderType
};

enum ELanguageType
{
    eLanguageUnspecified,
    eLanguageEnglishUS,
    eLanguageEnglishBritian,

    eMaxLanguageType
};

enum EContentRating
{
    eContentRatingUnspecified,
    eContentRatingFamily,
    eContentRatingAdult,
    eContentRatingXXX,
    eContentRatingDarkWeb,
    eContentRatingPersonal,

    eMaxContentRating
};

enum EContentCatagory
{
    eContentCatagoryUnspecified,
    eContentCatagoryVideo,
    eContentCatagoryAudio,
    eContentCatagoryImage,
    eContentCatagoryText,
    eContentCatagoryPersonal,
    eContentCatagoryOther,

    eMaxContentCatagory
};

enum ESubCatagory
{
    eSubCatagoryUnspecified,

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

//! \public Network protocol layer
enum ENetLayerType
{
    eNetLayerTypeUndefined,		        //< not specified/initialized
    eNetLayerTypeInternet,		        //< can communicate with internet
    eNetLayerTypePtoP,		            //< tcp and extern ip found
    eNetLayerTypeNetHost,		        //< network host available
    eNetLayerTypeNetGroup,		        //< network host available

    eMaxNetLayerType		            //< max must be last
};

enum ENetLayerState
{
    eNetLayerStateUndefined,		    //< not specified/initialized
    eNetLayerStateWrongType,		    //< invalid or not relevant net layer type
    eNetLayerStateTesting,		        //< testing for available 
    eNetLayerStateFailed,		        //< failed or some issue
    eNetLayerStateAvailable,		    //< available and ready for use

    eMaxNetLayerState		            //< max must be last
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

//! Internet Status as text
const char * DescribeInternetStatus( EInternetStatus internetStatus );

enum ENetAvailStatus
{
    eNetAvailNoInternet = 0,
    eNetAvailHostAvail,
    eNetAvailP2PAvail,
    eNetAvailOnlineButNoRelay,
    eNetAvailFullOnlineWithRelay,
    eNetAvailFullOnlineDirectConnect,
    eNetAvailGroupHost,

    eMaxNetAvailStatus
};

//! Net Available Status as text
const char * DescribeNetAvailStatus( ENetAvailStatus netAvailStatus );
