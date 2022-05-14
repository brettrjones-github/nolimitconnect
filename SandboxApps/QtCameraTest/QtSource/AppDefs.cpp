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
    //case eAppletApplicationInfo:	    return eMyIconDebug;
    case eAppletApplicationInfo:	    return eMyIconUnknown;
    case eAppletCreateAccount:			return eMyIconUserIdentity;
    case eAppletGroupUser:              return eMyIconGroupUser;
    case eAppletHomePage:				return eMyIconHome;
    case eAppletKodi:				    return eMyIconKodi;
    case eAppletLibrary:				return eMyIconLibraryNormal;
    case eAppletMessenger:				return eMyIconMessenger;
    case eAppletNetHostingPage:         return eMyIconServiceHostNetwork;
    case eAppletNetworkSettings:	    return eMyIconNetworkSettings;
    case eAppletPlayerVideo:			return eMyIconVideoPhoneNormal;
    case eAppletPersonalRecorder:		return eMyIconNotepadNormal;
    case eAppletSettings:				return eMyIconSettings;
    case eAppletClientStoryboard:       return eMyIconStoryBoardNormal;
   
    case eAppletSearchPage:	            return eMyIconSearch;
    case eAppletSearchPersons:	        return eMyIconSearchPerson;
    case eAppletSearchMood:	            return eMyIconSearchHeart;
    case eAppletScanAboutMe:	        return eMyIconSearchImage;
    case eAppletScanStoryboard:	        return eMyIconSearchStoryboard;
    case eAppletScanSharedFiles:	    return eMyIconSearchFolder;
    case eAppletScanWebCam:	            return eMyIconSearchWebCam;

    case eAppletClientAvatarImage:          return eMyIconAvatarImage;
    case eAppletClientChatRoom:             return eMyIconClientChatRoom;
    case eAppletClientRandomConnect:	    return eMyIconSearchRandomConnect;
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

    case eAppletServiceSettings:            return eMyIconUnknown;

    case eAppletSettingsAboutMe:            return eMyIconSettingsAboutMe;
    case eAppletSettingsAvatarImage:        return eMyIconSettingsAvatarImage;
    case eAppletSettingsWebCamServer:       return eMyIconSettingsShareWebCam;
    case eAppletSettingsChatRoom:           return eMyIconSettingsChatRoom;
    case eAppletSettingsConnectTest:        return eMyIconSettingsConnectionTest;
    case eAppletSettingsShareFiles:         return eMyIconSettingsShareFiles;
    case eAppletSettingsFileXfer:           return eMyIconSettingsFileXfer;
    case eAppletSettingsHostGroup:          return eMyIconSettingsHostGroup;

    case eAppletSettingsHostNetwork:        return eMyIconSettingsHostNetwork;
    case eAppletSettingsMessenger:          return eMyIconSettingsMessenger;
    case eAppletSettingsRandomConnect:      return eMyIconSettingsRandomConnect;
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
    case eAppletChooseThumbnail:		    return QObject::tr( "Choose Thumbnail" );
    case eAppletCreateAccount:			    return QObject::tr( "Create Account" );
    case eAppletGalleryImage:			    return QObject::tr( "Image Gallery" );
    case eAppletGalleryThumb:			    return QObject::tr( "Thumbnail Gallery" );
    case eAppletSnapshot:				    return QObject::tr( "Take Snapshot" );
    case eAppletKodi:				        return QObject::tr( "Kodi (TM)" );
    case eAppletUserIdentity:			    return QObject::tr( "User Identity" );
	case eAppletMessenger:				    return QObject::tr( "Messenger" );
	case eAppletPlayerVideo:			    return QObject::tr( "Video Player" );
    case eAppletPersonalRecorder:		    return QObject::tr( "Personal Notes And Records" );
    case eAppletLibrary:				    return QObject::tr( "My Library Of Media" );

	case eAppletTheme:					    return QObject::tr( "Theme" );
    case eAppletNetworkSettings:		    return QObject::tr( "Network Settings" );

    case eAppletSettings:				    return QObject::tr( "Settings" );

    case eAppletLogSettings:		        return QObject::tr( "Log Settings" );
    case eAppletLogView:	                return QObject::tr( "View Log" );

    case eAppletSearchPage:				    return QObject::tr( "Search" );
    case eAppletSearchPersons:			    return QObject::tr( "Search Group For Person Name" );
    case eAppletSearchMood:			        return QObject::tr( "Search Group For Mood Message" );
    case eAppletScanAboutMe:			    return QObject::tr( "Search Group For About Me Pictures" );
    case eAppletScanStoryboard:			    return QObject::tr( "Search Group For Story Boards" );
    case eAppletScanSharedFiles:		    return QObject::tr( "Search Group For Shared Files" );
    case eAppletScanWebCam:				    return QObject::tr( "Search Group For Shared Web Cams" );

    case eAppletClientChatRoom:		        return QObject::tr( "Chat with others in Chat room" );
    case eAppletClientRandomConnect:		return QObject::tr( "Connect With Random Person" );

    case eAppletGroupUser:                  return QObject::tr( "Join Group" );
    case eAppletNetHostingPage:             return QObject::tr( "Group And Network Hosting" );
    case eAppletPermissionList:             return QObject::tr( "Services Permission List" );

    case eAppletClientAvatarImage:          return QObject::tr( "Get Persons Avatar Image For Contact List" );
    case eAppletClientStoryboard:           return QObject::tr( "Share story (Blog)" );

    case eAppletShareServicesPage:          return QObject::tr( "Share with others sevices" );
    case eAppletServiceAboutMe:             return QObject::tr( "Share About Me Page" );
    case eAppletServiceAvatarImage:         return QObject::tr( "Share Your Avatar Image For Contact List" );
    case eAppletServiceChatRoom:            return QObject::tr( "Host A Chat Room" );
    case eAppletServiceConnectionTest:      return QObject::tr( "Service Connection Test" );
    case eAppletServiceHostGroup:           return QObject::tr( "Host A Group" );
    case eAppletServiceHostNetwork:         return QObject::tr( "Host A NoLimitConnect Network" );
    case eAppletServiceShareFiles:			return QObject::tr( "Share Media Files" );
    case eAppletServiceShareWebCam:         return QObject::tr( "Share your web cam" );
    case eAppletServiceStoryboard:          return QObject::tr( "Share Story Page (Blog)" );
    case eAppletServiceRandomConnect:       return QObject::tr( "Service Random Person Connect" );

    case eAppletServiceSettings:            return QObject::tr( "Service Settings" );

    case eAppletSettingsAboutMe:            return QObject::tr( "About Me Page Settings" );
    case eAppletSettingsAvatarImage:        return QObject::tr( "Avatar Image Settings" );
    case eAppletSettingsChatRoom:           return QObject::tr( "Chat Room Settings" );
    case eAppletSettingsWebCamServer:       return QObject::tr( "Shared Web Cam Settings" );
    case eAppletSettingsConnectTest:        return QObject::tr( "Connection Test Service Settings" );
    case eAppletSettingsShareFiles:         return QObject::tr( "Shared Files Settings" );
    case eAppletSettingsFileXfer:           return QObject::tr( "Person To Person File Xfer Settings" );
    case eAppletSettingsHostGroup:          return QObject::tr( "Host A Group Settings" );
    case eAppletSettingsHostNetwork:        return QObject::tr( "Host A Network Settings" );
    case eAppletSettingsMessenger:          return QObject::tr( "Messenger Settings" );
    case eAppletSettingsRandomConnect:      return QObject::tr( "Random Connect Service Settings" );
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
