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
// http://www.gotvptop.com
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
	case eAppletHomePage:				return eMyIconHome;
    case eAppletCreateAccount:			return eMyIconUserIdentity;
    case eAppletKodi:				    return eMyIconKodi;
    case eAppletUserIdentity:			return eMyIconUserIdentity;
	case eAppletMessenger:				return eMyIconMessenger;
	case eAppletPlayerVideo:			return eMyIconVideoPhoneNormal;
//	case eAppletPlayerMusic:			return eMyIconMusic;
//	case eAppletPlayerPhoto:			return eMyIconGallery;
//	case eAppletStreamViewerVideo:		return eMyIconVideoStreamViewer;
//	case eAppletStreamViewerCam:		return eMyIconCamStreamViewer;
//	case eAppletStreamViewerMusic:		return eMyIconMusicStreamPlayer;
	//case eAppletRemoteControl:			return eMyIconRemoteControl;
	//case eAppletPlugins:				return eMyIconPlugins;
    case eAppletPersonalRecorder:		return eMyIconNotepadNormal;
    case eAppletLibrary:				return eMyIconLibraryNormal;

	case eAppletTheme:					return eMyIconTheme;
    //case eAppletNetworkKey:				return eMyIconNetworkKey;
    case eAppletNetworkSettings:	    return eMyIconNetworkSettings;
	case eAppletSettings:				return eMyIconSettings;

    case eAppletGroupUser:              return eMyIconGroupUser;
    case eAppletNetHostingPage:         return eMyIconNetHosting;
    case eAppletGroupHost:              return eMyIconGroupHost;
    case eAppletGroupAnchor:            return eMyIconGroupListingHost;
    case eAppletGoTvPtoPNetworkHost:    return eMyIconNetHosting;

    case eAppletShareServicesPage:      return eMyIconShareServices;
    case eAppletStoryboard:             return eMyIconStoryBoardNormal;
    case eAppletSharedWebCam:           return eMyIconCamStreamProvider;
    case eAppletSharedContent:			return eMyIconSharedContent;
    case eAppletAboutGoTvPtoP:			return eMyIconApp;

	case eMaxBasicApplets:				return eMyIconUnknown; // this should never happen

	case ePluginAppletCamProvider:		return eMyIconCamStreamProvider;
	case ePluginAppletGoTvStation:		return eMyIconGoTvStation;
	case ePluginAppletGoTvNetworkHost:	return eMyIconGoTvNetworkHost;

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
	case eAppletHomePage:				return QObject::tr( "Home" );
    case eAppletAboutGoTvPtoP:		    return QObject::tr( "About GoTv Person to Person" );
    case eAppletCreateAccount:			return QObject::tr( "Create Account" );
    case eAppletKodi:				    return QObject::tr( "Kodi (TM)" );
    case eAppletUserIdentity:			return QObject::tr( "User Identity" );
	case eAppletMessenger:				return QObject::tr( "Messenger" );
	case eAppletPlayerVideo:			return QObject::tr( "Video Player" );
	//case eAppletPlayerMusic:			return QObject::tr( "Music Player" );
	//case eAppletPlayerPhoto:			return QObject::tr( "Image Gallery" );
	//case eAppletStreamViewerVideo:		return QObject::tr( "Video Streams" );
//	case eAppletStreamViewerCam:		return QObject::tr( "Search Cam Streams" );
	//case eAppletStreamViewerMusic:		return QObject::tr( "Music Streams" );
	//case eAppletRemoteControl:			return QObject::tr( "Remote Control" );
    case eAppletPersonalRecorder:		return QObject::tr( "Personal Notes And Records" );
    case eAppletLibrary:				return QObject::tr( "My Library Of Media" );

	case eAppletTheme:					return QObject::tr( "Theme" );
    //case eAppletNetworkKey:				return QObject::tr( "Network Key" );
    case eAppletNetworkSettings:		return QObject::tr( "Network Settings" );

	case eAppletSettings:				return QObject::tr( "Settings" );

    case eAppletGroupUser:              return QObject::tr( "Join Group" );
    case eAppletNetHostingPage:         return QObject::tr( "Group And Network Hosting" );
    case eAppletGroupHost:              return QObject::tr( "Host A Group" );
    case eAppletGroupAnchor:            return QObject::tr( "Host List Of Groups" );
    case eAppletGoTvPtoPNetworkHost:    return QObject::tr( "Host A GoTvPtoP Network" );

    case eAppletShareServicesPage:      return QObject::tr( "Share with others sevices" );
    case eAppletStoryboard:             return QObject::tr( "Share story (Blog)" );
    case eAppletSharedWebCam:           return QObject::tr( "Share your web cam" );
    case eAppletSharedContent:			return QObject::tr( "Share Media Files" );

	case ePluginAppletCamProvider:		return QObject::tr( "Provide Cam Stream" );
	case ePluginAppletGoTvStation:		return QObject::tr( "Manage Station" );
	case ePluginAppletGoTvNetworkHost:	return QObject::tr( "Manage Network Host" );

	case eMaxBasicApplets:
	case eAppletUnknown:
	default:
		return QObject::tr( "Unknown" );
	}
}
