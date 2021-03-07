//============================================================================
// Copyright (C) 2017 Brett R. Jones
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
#include <app_precompiled_hdr.h>
#include "AppDefs.h"
#include <QObject>

//============================================================================
// returns feature name ( never translated )
const char * DescribeAppFeature( EAppFeatures appFeature )
{
	// do NOT translate
	switch( appFeature )
	{
	case eAppFeatureViewer:
		return "FeatureViewer";
	case eAppFeatureProvider:
		return "FeatureProvider";
	case eAppFeatureStation:
		return "FeatureStation";
	case eAppFeatureNetworkHost:
		return "FeatureNetworkHost";

	case eAppFeatureUnknown:
	default:
		return "FeatureUnknown";
	}
}

//============================================================================
EMyIcons GetAppletIcon( EApplet applet )
{
	switch( applet )
	{
    case eActivityAppSetup:             return eMyIconApp;
    case eAppletAboutNoLimitConnect:	return eMyIconApp;
    case eAppletApplicationInfo:	    return eMyIconUnknown;
    case eAppletBrowseFiles:	        return eMyIconFileOpen;
    case eAppletChatRoomJoinSearch:	    return eMyIconSearchJoinChatRoom;
    case eAppletChatRoomJoin:	        return eMyIconClientChatRoom;
    case eAppletClientAvatarImage:      return eMyIconAvatarImage;
    case eAppletClientChatRoom:         return eMyIconClientChatRoom;
    case eAppletClientRandomConnect:	return eMyIconSearchRandomConnect;
    case eAppletClientStoryboard:       return eMyIconStoryBoardNormal;
    case eAppletCreateAccount:			return eMyIconUserIdentity;
    case eAppletGroupJoin:              return eMyIconGroupUser;
    case eAppletHomePage:				return eMyIconHome;
    case eAppletKodi:				    return eMyIconKodi;
    case eAppletLibrary:				return eMyIconLibraryNormal;
    case eAppletLog:	                return eMyIconDebug;
    case eAppletMessenger:				return eMyIconMessenger;
    case eAppletNetHostingPage:         return eMyIconServiceHostNetwork;
    case eAppletNetworkSettings:	    return eMyIconNetworkSettings;
    case eAppletPlayerVideo:			return eMyIconVideoPhoneNormal;
    case eAppletPersonalRecorder:		return eMyIconNotepadNormal;
    case eAppletSettingsPage:		    return eMyIconSettings;
   
    case eAppletSearchPage:	                return eMyIconSearch;
    case eAppletGroupJoinSearch:	        return eMyIconSearchJoinGroup;
    case eAppletRandomConnectJoinSearch:    return eMyIconSearchJoinRandomConnect;
    case eAppletSearchPersons:	            return eMyIconSearchPerson;
    case eAppletSearchMood:	                return eMyIconSearchHeart;
    case eAppletScanAboutMe:	            return eMyIconSearchImage;
    case eAppletScanStoryboard:	            return eMyIconSearchStoryboard;
    case eAppletScanSharedFiles:	        return eMyIconSearchFolder;
    case eAppletScanWebCam:	                return eMyIconSearchWebCam;

    case eAppletTestAndDebug:	            return eMyIconDebug;
    case eAppletTheme:					    return eMyIconTheme;

	case ePluginAppletCamProvider:		    return eMyIconCamStreamProvider;
	case ePluginAppletGoTvStation:		    return eMyIconGoTvStation;
	case ePluginAppletGoTvNetworkHost:	    return eMyIconServiceHostNetwork;
    case eAppletUserIdentity:			    return eMyIconUserIdentity;

    case eAppletShareServicesPage:          return eMyIconShareServices;
    case eAppletServiceAboutMe:             return eMyIconServiceShareAboutMe;
    case eAppletServiceAvatarImage:         return eMyIconServiceAvatarImage;
    case eAppletServiceShareFiles:          return eMyIconServiceShareFiles;
    case eAppletServiceShareWebCam:         return eMyIconServiceShareWebCam;
    case eAppletServiceStoryboard:          return eMyIconServiceShareStoryboard;

    case eAppletServiceHostGroup:           return eMyIconServiceHostGroup;
    case eAppletPermissionList:             return eMyIconKeyEnabled;
    case eAppletServiceHostNetwork:         return eMyIconServiceHostNetwork;

    case eAppletServiceChatRoom:            return eMyIconServiceChatRoom;
    case eAppletServiceConnectionTest:      return eMyIconServiceConnectionTest;
    case eAppletServiceRandomConnect:       return eMyIconServiceRandomConnect;
    case eAppletServiceRandomConnectRelay:  return eMyIconServiceRandomConnectRelay;
    case eAppletServiceRelay:               return eMyIconServiceRelay;

    case eAppletServiceSettings:            return eMyIconUnknown;

    case eAppletSettingsAboutMe:            return eMyIconSettingsAboutMe;
    case eAppletSettingsAvatarImage:        return eMyIconSettingsAvatarImage;
    case eAppletSettingsWebCamServer:       return eMyIconSettingsShareWebCam;
    case eAppletSettingsChatRoom:           return eMyIconSettingsChatRoom;
    case eAppletSettingsConnectTest:        return eMyIconSettingsConnectionTest;
    case eAppletSettingsShareFiles:         return eMyIconSettingsShareFiles;
    case eAppletSettingsFileXfer:           return eMyIconSettingsFileXfer;
    case eAppletSettingsHostChatRoom:       return eMyIconSettingsChatRoom;
    case eAppletSettingsHostGroup:          return eMyIconSettingsHostGroup;
    // case eAppletSettingsHostGroupListing:            return eMyIconSettingsHostGroupListing;

    case eAppletSettingsHostNetwork:        return eMyIconSettingsHostNetwork;
    case eAppletSettingsHostRandomConnect:  return eMyIconSettingsRandomConnect;
    case eAppletSettingsMessenger:          return eMyIconSettingsMessenger;
    case eAppletSettingsRandomConnect:      return eMyIconSettingsRandomConnect;
    case eAppletSettingsRandomConnectRelay: return eMyIconSettingsRandomConnectRelay;
    // case eAppletSettingsRelay:            return eMyIconSettingsRelay;
    case eAppletSettingsStoryboard:         return eMyIconSettingsShareStoryboard;
    case eAppletSettingsTruthOrDare:        return eMyIconSettingsTruthOrDare;
    case eAppletSettingsVideoPhone:         return eMyIconSettingsVideoPhone;
    case eAppletSettingsVoicePhone:         return eMyIconSettingsVoicePhone;


    case eAppletLogSettings:                return eMyIconDebug;
    case eAppletLogView:                    return eMyIconDebug;

    // case eAppletPlayerMusic:			return eMyIconMusic;
    // case eAppletPlayerPhoto:			return eMyIconGallery;
    // case eAppletStreamViewerVideo:		return eMyIconVideoStreamViewer;
    // case eAppletStreamViewerCam:		return eMyIconCamStreamViewer;
    // case eAppletStreamViewerMusic:		return eMyIconMusicStreamPlayer;
    // case eAppletRemoteControl:			return eMyIconRemoteControl;
    // case eAppletPlugins:				return eMyIconPlugins;

	case eAppletUnknown:
	default:
		return eMyIconUnknown;
	}
}

//============================================================================
QString DescribeApplet( EApplet applet )
{
	switch( applet )
	{
    case eActivityAppSetup:				    return QObject::tr( "Application Initialize" );
    case eAppletEditAvatarImage:		    return QObject::tr( "Edit My Avatar Image" );
    case eAppletEditStoryboard:		        return QObject::tr( "Edit My Story Board (Blog)" );
    case eAppletHomePage:				    return QObject::tr( "Home" );
    case eAppletAboutNoLimitConnect:        return QObject::tr( "About No Limit Connect" );
    case eAppletApplicationInfo:            return QObject::tr( "Application Information" );
    case eAppletBrowseFiles:			    return QObject::tr( "Browse Files" );
    case eAppletChatRoomJoin:               return QObject::tr( "Join Chat Room" );
    case eAppletChatRoomJoinSearch:			return QObject::tr( "Search For Chat Room To Join" );
    case eAppletChooseThumbnail:		    return QObject::tr( "Choose Thumbnail" );
    case eAppletClientChatRoom:		        return QObject::tr( "Chat with others in Chat room" );
    case eAppletClientRandomConnect:		return QObject::tr( "Connect With Random Person" );
    case eAppletClientAvatarImage:          return QObject::tr( "Get Persons Avatar Image For Contact List" );
    case eAppletClientStoryboard:           return QObject::tr( "Share story (Blog)" );
    case eAppletCreateAccount:			    return QObject::tr( "Create Account" );
    case eAppletGalleryImage:			    return QObject::tr( "Image Gallery" );
    case eAppletGalleryThumb:			    return QObject::tr( "Thumbnail Gallery" );
    case eAppletGroupJoin:                  return QObject::tr( "Join Group" );
    case eAppletGroupJoinSearch:			return QObject::tr( "Search For Group To Join" );
    case eAppletRandomConnectJoinSearch:	return QObject::tr( "Search For Random Connect Host To Join" );
    case eAppletSnapshot:				    return QObject::tr( "Take Snapshot" );
    case eAppletKodi:				        return QObject::tr( "Kodi (TM)" );
    case eAppletLibrary:				    return QObject::tr( "My Library Of Media" );
    case eAppletLog:                        return QObject::tr( "Application Log" );
    case eAppletLogSettings:		        return QObject::tr( "Log Settings" );
    case eAppletLogView:	                return QObject::tr( "View Log" );
	case eAppletMessenger:				    return QObject::tr( "Messenger" );
    case eAppletNetworkSettings:		    return QObject::tr( "Network Settings" );
	case eAppletPlayerVideo:			    return QObject::tr( "Video Player" );
    case eAppletPersonalRecorder:		    return QObject::tr( "Personal Notes And Records" );

    case eAppletTestAndDebug:			    return QObject::tr( "Test And Debug" );
	case eAppletTheme:					    return QObject::tr( "Theme" );
    case eAppletUserIdentity:			    return QObject::tr( "User Identity" );

    case eAppletSettingsPage:				return QObject::tr( "Settings" );

    case eAppletSearchPage:				    return QObject::tr( "Search" );
    case eAppletSearchPersons:			    return QObject::tr( "Search Group For Person Name" );
    case eAppletSearchMood:			        return QObject::tr( "Search Group For Mood Message" );
    case eAppletScanAboutMe:			    return QObject::tr( "Search Group For About Me Pictures" );
    case eAppletScanStoryboard:			    return QObject::tr( "Search Group For Story Boards" );
    case eAppletScanSharedFiles:		    return QObject::tr( "Search Group For Shared Files" );
    case eAppletScanWebCam:				    return QObject::tr( "Search Group For Shared Web Cams" );

    case eAppletNetHostingPage:             return QObject::tr( "Hosting Services" );
    case eAppletPermissionList:             return QObject::tr( "Share Permission List" );

    case eAppletShareServicesPage:          return QObject::tr( "Share with others sevices" );
    case eAppletServiceAboutMe:             return QObject::tr( "Share About Me Page" );
    case eAppletServiceAvatarImage:         return QObject::tr( "Share Your Avatar Image For Contact List" );
    case eAppletServiceChatRoom:            return QObject::tr( "View My Hosted Chat Room" );
    case eAppletServiceConnectionTest:      return QObject::tr( "Service Connection Test" );
    case eAppletServiceHostGroup:           return QObject::tr( "Host A Group" );
    case eAppletServiceHostNetwork:         return QObject::tr( "Host A NoLimitConnect Network" );
    case eAppletServiceShareFiles:			return QObject::tr( "Share Media Files" );
    case eAppletServiceShareWebCam:         return QObject::tr( "Share your web cam" );
    case eAppletServiceStoryboard:          return QObject::tr( "Share Story Page (Blog)" );
    case eAppletServiceRandomConnect:       return QObject::tr( "Service Random Person Connect" );
    case eAppletServiceRandomConnectRelay:  return QObject::tr( "Service Random Person Relay" );
    case eAppletServiceRelay:               return QObject::tr( "Service Relay" );

    case eAppletServiceSettings:            return QObject::tr( "Service Settings" );

    case eAppletSettingsAboutMe:            return QObject::tr( "About Me Page Settings" );
    case eAppletSettingsAvatarImage:        return QObject::tr( "Avatar Image Settings" );
    case eAppletSettingsChatRoom:           return QObject::tr( "Chat Room Settings" );
    case eAppletSettingsWebCamServer:       return QObject::tr( "Shared Web Cam Settings" );
    case eAppletSettingsConnectTest:        return QObject::tr( "Connection Test Service Settings" );
    case eAppletSettingsShareFiles:         return QObject::tr( "Shared Files Settings" );
    case eAppletSettingsFileXfer:           return QObject::tr( "Person To Person File Xfer Settings" );
    case eAppletSettingsHostChatRoom:       return QObject::tr( "Host A Chat Room Settings" );
    case eAppletSettingsHostGroup:          return QObject::tr( "Host A Group Settings" );
    case eAppletSettingsHostNetwork:        return QObject::tr( "Host A Network Settings" );
    case eAppletSettingsHostRandomConnect:  return QObject::tr( "Host Random Connect" );
    case eAppletSettingsMessenger:          return QObject::tr( "Messenger Settings" );
    case eAppletSettingsRandomConnect:      return QObject::tr( "Random Connect Service Settings" );
    case eAppletSettingsRandomConnectRelay: return QObject::tr( "Random Connect Relay Service Settings" );
    // case eAppletSettingsRelay:              return QObject::tr( "Relay Service Settings" );
    case eAppletSettingsStoryboard:         return QObject::tr( "Shared Story (Blog) Settings" );
    case eAppletSettingsTruthOrDare:        return QObject::tr( "Truth Or Dare Video Chat Game Settings" );
    case eAppletSettingsVideoPhone:         return QObject::tr( "Video Chat Settings" );
    case eAppletSettingsVoicePhone:         return QObject::tr( "Voice Only Phone Call Settings" );

	case ePluginAppletCamProvider:		    return QObject::tr( "Provide Cam Stream" );
	case ePluginAppletGoTvStation:		    return QObject::tr( "Manage Station" );
	case ePluginAppletGoTvNetworkHost:	    return QObject::tr( "Manage Station Network Host" );


        //case eAppletPlayerMusic:			return QObject::tr( "Music Player" );
//case eAppletPlayerPhoto:			return QObject::tr( "Image Gallery" );
//case eAppletStreamViewerVideo:		return QObject::tr( "Video Streams" );
//	case eAppletStreamViewerCam:		return QObject::tr( "Search Cam Streams" );
    //case eAppletStreamViewerMusic:		return QObject::tr( "Music Streams" );
    //case eAppletRemoteControl:			return QObject::tr( "Remote Control" );
     //case eAppletNetworkKey:				return QObject::tr( "Network Key" );

	case eMaxBasicApplets:
	case eAppletUnknown:
	default:
		return QObject::tr( "Unknown" );
	}
}
