//============================================================================
// Copyright (C) 2014 Brett R. Jones
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

#include <QWidget> // must be declared first or linux Qt 6.2.4 will error in qmetatype.h 2167:23: array subscript value ‘53’ is outside the bounds

#include "AppSettings.h"

#include <ptop_src/ptop_engine_src/P2PEngine/EngineSettingsDefaultValues.h>

#include <NetLib/VxGetRandomPort.h>
#include <CoreLib/VxParse.h>

#include <QString>

namespace
{
	#define APP_SETTINGS_DBVERSION		1
	#define SELECTED_THEME_TYPE			"SELECTED_THEME_TYPE"
	#define HOME_LAYOUT_TYPE			"HOME_LAYOUT_TYPE"
}

//============================================================================
AppSettings::AppSettings()
: VxSettings( "AppSettingsDb" )
{
	// mark all values as not cached
	memset( m_AppFeatureIsCached, 0, sizeof( m_AppFeatureIsCached ) );
}

//============================================================================
AppSettings::~AppSettings()
{
}

//============================================================================
RCODE AppSettings::appSettingStartup( const char* dbSettingsFile, EDefaultAppMode appDefaultMode )
{
	RCODE rc = dbStartup( APP_SETTINGS_DBVERSION, dbSettingsFile );
	if( ( 0 == rc )
		&& ( false == appModeSettingsAreInitialize() ) )
	{
		setupAppModeSettings( appDefaultMode );
	}

	return rc;
}

//============================================================================
void AppSettings::appSettingShutdown( void )
{
	dbShutdown();
}

//============================================================================
void AppSettings::setIsMessengerFullScreen( bool isFullScreen )
{
	setIniValue( getAppShortName(), "MessangerFullScreen", isFullScreen );
}

//============================================================================
bool AppSettings::getIsMessengerFullScreen( void )
{
	bool fullScreen;
	getIniValue( getAppShortName(), "MessangerFullScreen", fullScreen, false );
	return fullScreen;
}

//============================================================================
void AppSettings::setFeatureEnable( EAppFeatures appFeature, bool enable )
{
	setIniValue( getAppShortName(), DescribeAppFeature( appFeature ), enable );
	m_AppFeatureValue[appFeature] = enable;
	m_AppFeatureIsCached[appFeature] = true;
}

//============================================================================
bool AppSettings::getFeatureEnable( EAppFeatures appFeature )
{
	// for now app features are used for disable
	// in the future if features are enabled then should let this code work
	if( eAppFeatureAboutMePage == appFeature )
	{
		return true;
	}

	if( eAppFeatureStoryboard == appFeature )
	{
		return true;
	}

	if( eAppFeatureTheme == appFeature )
	{
		return true;
	}

	if( eAppFeatureLanguageChoice == appFeature )
	{
		return true;
	}

	if( eAppFeaturePreferredContent == appFeature )
	{
		return true;
	}

	if( eAppFeatureChatRoom == appFeature )
	{
		return true;
	}

	if( eAppFeatureRandomConnect == appFeature )
	{
		return true;
	}

	return false;

	if( m_AppFeatureIsCached[appFeature] )
	{
		// pull from cache
		return m_AppFeatureValue[ appFeature ];
	}
	
	// get feature from db
	bool featureValue = false;
	getIniValue( getAppShortName(), DescribeAppFeature( appFeature ), featureValue, 0 );
	m_AppFeatureValue[appFeature] = featureValue;
	m_AppFeatureIsCached[appFeature] = true;
	return m_AppFeatureValue[appFeature];
}

//============================================================================
void AppSettings::setLastSelectedTheme( EThemeType selectedTheme )
{
	uint32_t themeType = (uint32_t)selectedTheme;
	setIniValue( getAppShortName(), SELECTED_THEME_TYPE, themeType );
}

//============================================================================
EThemeType AppSettings::getLastSelectedTheme( void )
{
	uint32_t themeType = 1;
	getIniValue( getAppShortName(), SELECTED_THEME_TYPE, themeType, (uint32_t)eThemeTypeDark );
	return (EThemeType)themeType;
}

//============================================================================
void AppSettings::setHomePageLayout( EHomeLayout homeLayout )
{
	uint32_t homeLayoutType = (uint32_t)homeLayout;
	setIniValue( getAppShortName(), HOME_LAYOUT_TYPE, homeLayoutType );
}

//============================================================================
EHomeLayout AppSettings::getHomePageLayout( void )
{
	uint32_t homeLayoutType = 1;
	getIniValue( getAppShortName(), HOME_LAYOUT_TYPE, homeLayoutType, 1 );
	return (EHomeLayout)homeLayoutType;
}

//============================================================================
void AppSettings::setLastBrowseDir( EFileFilterType eFileFilterType, std::string& browseDir )
{
	if( 0 != browseDir.length() )
	{
		QString keyStr = QString("BrowseDir%1").arg( (int)eFileFilterType );
		setIniValue( getAppShortName(), keyStr.toUtf8().constData(), browseDir );
	}
}

//============================================================================
void AppSettings::getLastBrowseDir( EFileFilterType eFileFilterType, std::string& browseDir )
{
	QString keyStr = QString("BrowseDir%1").arg( (int)eFileFilterType );
	getIniValue( getAppShortName(), keyStr.toUtf8().constData(), browseDir, "" );
}

//============================================================================
void AppSettings::setLastGalleryDir( std::string& galleryDir )
{
	setIniValue( getAppShortName(), "GalleryDir", galleryDir );
}

//============================================================================
void AppSettings::getLastGalleryDir( std::string& galleryDir )
{
	getIniValue( getAppShortName(), "GalleryDir", galleryDir, "" );
}

//============================================================================
void AppSettings::setLastBrowseShareDir( std::string& browseDir )
{
	setIniValue( getAppShortName(), "ShareBrowseDir", browseDir );
}

//============================================================================
void AppSettings::getLastBrowseShareDir( std::string& browseDir )
{
	getIniValue( getAppShortName(), "ShareBrowseDir", browseDir, "" );
}

//============================================================================
void AppSettings::getLastHostSearchText( ESearchType searchType, std::string& searchText )
{
    getIniValue( getAppShortName(), getAppendedType("LastHostSearchText", searchType).c_str(), searchText, "" );
}

//============================================================================
void AppSettings::setLastHostSearchText( ESearchType searchType, std::string& searchText )
{
    setIniValue( getAppShortName(), getAppendedType("LastHostSearchText", searchType).c_str(), searchText );
}

//============================================================================
void AppSettings::setLastHostSearchAgeType( ESearchType searchType, EAgeType ageType )
{
    uint32_t val = (uint32_t)ageType;
    setIniValue( getAppShortName(), getAppendedType("LastHostSearchAge", searchType).c_str(), val );
}

//============================================================================
void AppSettings::getLastHostSearchAgeType( ESearchType searchType, EAgeType& ageType )
{
    uint32_t val = 0;
    getIniValue( getAppShortName(), getAppendedType("LastHostSearchAge", searchType).c_str(), val, 0 );
    ageType = (EAgeType)val;
}

//============================================================================
void AppSettings::setLastHostSearchGender( ESearchType searchType, EGenderType genderType )
{
    uint32_t val = (uint32_t)genderType;
    setIniValue( getAppShortName(), getAppendedType("LastHostSearchGender", searchType).c_str(), val );
}

//============================================================================
void AppSettings::getLastHostSearchGender( ESearchType searchType, EGenderType& genderType )
{
    uint32_t val = 0;
    getIniValue( getAppShortName(), getAppendedType("LastHostSearchGender", searchType).c_str(), val, 0 );
    genderType = (EGenderType)val;
}

//============================================================================
void AppSettings::setLastHostSearchLanguage( ESearchType searchType, ELanguageType languageType )
{
    uint32_t val = (uint32_t)languageType;
    setIniValue( getAppShortName(), getAppendedType("LastHostSearchLanguage", searchType).c_str(), val );
}

//============================================================================
void AppSettings::getLastHostSearchLanguage( ESearchType searchType, ELanguageType& languageType )
{
    uint32_t val = 0;
    getIniValue( getAppShortName(), getAppendedType("LastHostSearchLanguage", searchType).c_str(), val, 0 );
    languageType = (ELanguageType)val;
}

//============================================================================
void AppSettings::setLastHostSearchContentRating( ESearchType searchType, EContentRating contentRating )
{
    uint32_t val = (uint32_t)contentRating;
    setIniValue( getAppShortName(), getAppendedType("LastHostSearchRating", searchType).c_str(), val );
}

//============================================================================
void AppSettings::getLastHostSearchContentRating( ESearchType searchType, EContentRating& contentRating )
{
    uint32_t val = 0;
    getIniValue( getAppShortName(), getAppendedType("LastHostSearchRating", searchType).c_str(), val, 0 );
    contentRating = (EContentRating)val;
}

//============================================================================
void AppSettings::setCamRotation( uint32_t camId, uint32_t camRotation )
{
	std::string camKey;
	StdStringFormat( camKey, "CamRotation%d", camId );
	setIniValue( getAppShortName(), camKey.c_str(), camRotation );
}

//============================================================================
uint32_t AppSettings::getCamRotation( uint32_t camId )
{
	uint32_t camRotation = 0;
	std::string camKey;
	StdStringFormat( camKey, "CamRotation%d", camId );
	getIniValue( getAppShortName(), camKey.c_str(), camRotation, 0 );
	return camRotation;
}

//============================================================================
void AppSettings::setCamSourceId( uint32_t camId )
{
	setIniValue( getAppShortName(), "CamSourceId", camId );
}

//============================================================================
uint32_t AppSettings::getCamSourceId( void )
{
	uint32_t camSourceId = 0;
	getIniValue( getAppShortName(), "CamSourceId", camSourceId, 0 );
	return camSourceId;
}

//============================================================================
void AppSettings::setCamShowPreview( bool showPreview )
{
	uint32_t showPreviewVal = showPreview ? 1 : 0;
	setIniValue( getAppShortName(), "CamShowPreview", showPreviewVal );
}

//============================================================================
bool AppSettings::getCamShowPreview( void )
{
	uint32_t showPreview = 0;
	getIniValue( getAppShortName(), "CamShowPreview", showPreview, 0 );
	return showPreview ? true : false;
}

//============================================================================
void AppSettings::setVidFeedRotation( uint32_t feedRotation )
{
	setIniValue( getAppShortName(), "VidFeedRotation", feedRotation );
}

//============================================================================
uint32_t AppSettings::getVidFeedRotation( void )
{
	uint32_t feedRotation = 0;
	getIniValue( getAppShortName(), "VidFeedRotation", feedRotation, 0 );
	return feedRotation;
}

//============================================================================
void AppSettings::setLastAppletLaunched( EApplet applet )
{
     uint32_t appletVal = (EApplet)applet;
    setIniValue( getAppShortName(), "LastAppletLaunched", appletVal );
}

//============================================================================
EApplet AppSettings::getLastAppletLaunched( void )
{
    uint32_t appletVal = 0;
    getIniValue( getAppShortName(), "LastAppletLaunched", appletVal, 0 );
    return (EApplet)appletVal;
}

//============================================================================
void AppSettings::setVerboseLog( bool verbose )
{
    uint32_t appletVal = verbose;
    setIniValue( getAppShortName(), "VerboseLog", appletVal );
}

//============================================================================
bool AppSettings::getVerboseLog( void )
{
    uint32_t appletVal = 0;
    getIniValue( getAppShortName(), "VerboseLog", appletVal, 0 );
    return (bool)appletVal;
}

//============================================================================
void AppSettings::setLogLevels( uint32_t logLevelFlags )
{
#if defined(DEBUG)
    setIniValue( getAppShortName(), "LogLevelsD", logLevelFlags );
#else
    setIniValue( getAppShortName(), "LogLevels", logLevelFlags );
#endif // defined(DEBUG)
}

//============================================================================
uint32_t AppSettings::getLogLevels( void )
{
    uint32_t logLevelFlags = 0;
#if defined(DEBUG)
    logLevelFlags = LOG_FATAL | LOG_SEVERE | LOG_ASSERT | LOG_ERROR | LOG_WARN | LOG_DEBUG | LOG_INFO | LOG_STATUS;	// LOG_VERBOSE
    getIniValue( getAppShortName(), "LogLevelsD", logLevelFlags, logLevelFlags );
#else
    logLevelFlags = LOG_FATAL | LOG_SEVERE | LOG_ASSERT;
    getIniValue( getAppShortName(), "LogLevels", logLevelFlags, logLevelFlags );
#endif // defined(DEBUG)
    return logLevelFlags;
}

//============================================================================
void AppSettings::setLogModules( uint32_t logLevelFlags )
{
#if defined(DEBUG)
    setIniValue( getAppShortName(), "LogModulesD", logLevelFlags );
#else
    setIniValue( getAppShortName(), "LogModules", logLevelFlags );
#endif // defined(DEBUG)
}

//============================================================================
uint32_t AppSettings::getLogModules( void )
{
    uint32_t logModuleFlags = 0;
#if defined(DEBUG)
	uint32_t logModuleFlagsDefault = 0;
    getIniValue( getAppShortName(), "LogModulesD", logModuleFlags, logModuleFlagsDefault );
#else
    getIniValue( getAppShortName(), "LogModules", logModuleFlags, logModuleFlags );
#endif // defined(DEBUG)
    return logModuleFlags;
}

//============================================================================
bool AppSettings::getMutePhoneRing( void )
{
	bool bMute;
	getIniValue( getAppShortName(), "MutePhoneRing", bMute, false );

	return bMute;
}

//============================================================================
void AppSettings::setMutePhoneRing( bool bMutePhoneRing )
{
	setIniValue( getAppShortName(), "MutePhoneRing", bMutePhoneRing );
}

//============================================================================
bool AppSettings::getMuteNotifySound( void )
{
	bool bMute;
	getIniValue( getAppShortName(), "MuteNotifySound", bMute, false );

	return bMute;
}

//============================================================================
void AppSettings::setMuteNotifySound( bool bNotifySound )
{
	uint32_t u32Value = bNotifySound;
	setIniValue( getAppShortName(), "MuteNotifySound", u32Value );
}

//============================================================================
void AppSettings::setIsConfirmDeleteDisabled( bool confirmIsDisabled )
{
	uint32_t u32Value = confirmIsDisabled;
	setIniValue( getAppShortName(), "ConfirmDeleteDisable", u32Value );
}

//============================================================================
bool AppSettings::getIsConfirmDeleteDisabled( void )
{
	bool confirmIsDisabled;
	getIniValue( getAppShortName(), "ConfirmDeleteDisable", confirmIsDisabled, false );
	return confirmIsDisabled;
}

//============================================================================
bool AppSettings::appModeSettingsAreInitialize( void )
{
	bool confirmIsInitialized = false;
	getIniValue( getAppShortName(), "AppDefaultInited", confirmIsInitialized, false );
	return confirmIsInitialized;
}

//============================================================================
std::string AppSettings::getAppendedType( const char* key, ESearchType searchType )
{
    std::string result;
    StdStringFormat( result, "%s%d", key, (int)searchType );
    return result;
}

//============================================================================
void AppSettings::setupAppModeSettings( EDefaultAppMode appDefaultMode )
{
	// set all features to false
	for( int i = 0; i < eMaxAppFeatures; i++ )
	{
		setFeatureEnable( (EAppFeatures)i, false );
	}

#if ENABLE_KODI
	setFeatureEnable( eAppFeatureKodi, true );
#endif // ENABLE_KODI
	//// enable app mode specific features
	//switch( appDefaultMode )
	//{
 //   case eAppModeNlcViewer:
	//	// setFeatureEnable( eAppFeatureViewer, true );
	//	break;
 //   case eAppModeNlcProvider:
	//	// setFeatureEnable( eAppFeatureProvider, true );
	//	break;
 //   case eAppModeNlcStation:
	//	// setFeatureEnable( eAppFeatureStation, true );
	//	break;
 //   case eAppModeNlcNetworkHost:
	//	// setFeatureEnable( eAppFeatureNetworkHost, true );
	//	break;
 //   default:
 //       break;
	//}

	// mark database as intialized with app mode settings
	uint32_t u32Value = 1;
	setIniValue( getAppShortName(), "AppDefaultInited", u32Value );
	m_AppSettingsInitialized = true;
}

//============================================================================
void AppSettings::setLastUsedTestUrl( std::string& testUrl )
{
    setIniValue( getAppShortName(), "LastTestUrl", testUrl );
}

//============================================================================
void AppSettings::getLastUsedTestUrl( std::string& testUrl )
{
    getIniValue( getAppShortName(), "LastTestUrl", testUrl, "" );
}

//============================================================================
void AppSettings::setFavoriteHostGroupUrl( std::string& hostUrl )
{
	setIniValue( getAppShortName(), "FavGroupHostUrl", hostUrl );
}

//============================================================================
void AppSettings::getFavoriteHostGroupUrl( std::string& hostUrl )
{
	getIniValue( getAppShortName(), "FavGroupHostUrl", hostUrl, "" );
}

//============================================================================
void AppSettings::setSoundInDeviceIndex( int32_t deviceIndex )
{
	setIniValue( getAppShortName(), "SoundInDeviceIndex", deviceIndex );
}

//============================================================================
int32_t AppSettings::getSoundInDeviceIndex( void )
{
	int32_t deviceIndex = 0;
	getIniValue( getAppShortName(), "SoundInDeviceIndex", deviceIndex );
	return deviceIndex;
}

//============================================================================
void AppSettings::setSoundOutDeviceIndex( int32_t deviceIndex )
{
	setIniValue( getAppShortName(), "SoundOutDeviceIndex", deviceIndex );
}

//============================================================================
int32_t AppSettings::getSoundOutDeviceIndex( void )
{
	int32_t deviceIndex = 0;
	getIniValue( getAppShortName(), "SoundOutDeviceIndex", deviceIndex, 0 );
	return deviceIndex;
}

//============================================================================
void AppSettings::setRunOnStartupCamServer( bool runOnStartup )
{
	setIniValue( getAppShortName(), "RunOnStartupCamServer", runOnStartup );
}

//============================================================================
bool AppSettings::getRunOnStartupCamServer( void )
{
	bool runOnStartup = false;
	getIniValue( getAppShortName(), "RunOnStartupCamServer", runOnStartup, false );
	return runOnStartup;
}

//============================================================================
void AppSettings::setRunOnStartupFileShareServer( bool runOnStartup )
{
	setIniValue( getAppShortName(), "RunOnStartupFileShareServer", runOnStartup );
}

//============================================================================
bool AppSettings::getRunOnStartupFileShareServer( void )
{
	bool runOnStartup = false;
	getIniValue( getAppShortName(), "RunOnStartupFileShareServer", runOnStartup, false );
	return runOnStartup;
}

//============================================================================
void AppSettings::setAllowLoopBackOfMyself( bool allowLoopback )
{
	setIniValue( getAppShortName(), "AllowLoopbackMyself", allowLoopback );
}

//============================================================================
bool AppSettings::getAllowLoopBackOfMyself( void )
{
	bool allowLoopback = false;
	getIniValue( getAppShortName(), "AllowLoopbackMyself", allowLoopback, false );
	return allowLoopback;
}

//============================================================================
void AppSettings::setEchoDelayParam( int delayMs )
{
	uint32_t delayVal = (uint32_t)delayMs;
	setIniValue( getAppShortName(), "EchoDelayParam", delayVal );
}

//============================================================================
int AppSettings::getEchoDelayParam( void )
{
	uint32_t defaultParamVal = 100;
#if defined(TARGET_OS_ANDROID)
	defaultParamVal = 200;
#elif defined(TARGET_OS_WINDOWS) 
	defaultParamVal = 100;
#elif defined(TARGET_OS_LINUX) 
	defaultParamVal = 100;
#endif // defined(TARGET_OS_ANDROID)

	uint32_t paramVal;
	getIniValue( getAppShortName(), "EchoDelayParam", paramVal, defaultParamVal );
	return (int)paramVal;
}

//============================================================================
void AppSettings::setEchoCancelEnable( bool enable )
{
	setIniValue( getAppShortName(), "EchoCancelEnable", enable );
}

//============================================================================
bool AppSettings::getEchoCancelEnable( void )
{
	//bool echoCancelEnable = true;
	//getIniValue( getAppShortName(), "EchoCancelEnable", echoCancelEnable, true );
	//return echoCancelEnable;
	return true; // currently alwasy senabled
}
