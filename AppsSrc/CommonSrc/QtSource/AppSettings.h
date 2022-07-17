#pragma once
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

#include "AppDefs.h"
#include "AppProfile.h"
#include "HomeWindow.h"

#include <CoreLib/VxSettings.h>

class AppSettings : public VxSettings, public AppProfile
{
public:
	AppSettings();
	virtual ~AppSettings();

	void						setAppShortName( QString appShortName )		{  m_AppShortName = appShortName.toUtf8().constData(); }
	const char *				getAppShortName( void )						{ return m_AppShortName.c_str(); }

	RCODE						appSettingStartup( const char * dbSettingsFile, EDefaultAppMode appDefaultMode );
	void						appSettingShutdown( void );

	void						setIsMessengerFullScreen( bool isFullScreen );
	bool						getIsMessengerFullScreen( void );

	void						setFeatureEnable( EAppFeatures appFeature, bool enable );
	bool						getFeatureEnable( EAppFeatures appFeature );

	void						setLastSelectedTheme( EThemeType selectedTheme );
	EThemeType					getLastSelectedTheme( void );

	void						setHomePageLayout( EHomeLayout homeLayout );
	EHomeLayout					getHomePageLayout( void );

	void						setMutePhoneRing( bool bMutePhoneRing );
	bool						getMutePhoneRing( void );

	void						setMuteNotifySound( bool bMuteNotifySound );
	bool						getMuteNotifySound( void );

	void						setIsConfirmDeleteDisabled( bool confirmIsDisabled );
	bool						getIsConfirmDeleteDisabled( void );

	void						setLastBrowseShareDir( std::string& browseDir );
	void						getLastBrowseShareDir( std::string& browseDir );

	void						setLastBrowseDir( EFileFilterType eFileFilterType, std::string& browseDir );
	void						getLastBrowseDir( EFileFilterType eFileFilterType, std::string& browseDir );

	void						setLastGalleryDir( std::string& galleryDir );
	void						getLastGalleryDir( std::string& galleryDir );

    void						setLastHostSearchText( ESearchType searchType, std::string& searchText );
    void						getLastHostSearchText( ESearchType searchType, std::string& searchText );
    void						setLastHostSearchAgeType( ESearchType searchType, EAgeType ageType );
    void						getLastHostSearchAgeType( ESearchType searchType, EAgeType& ageType );
    void						setLastHostSearchGender( ESearchType searchType, EGenderType genderType );
    void						getLastHostSearchGender( ESearchType searchType, EGenderType& genderType );
    void						setLastHostSearchLanguage( ESearchType searchType, ELanguageType languageType );
    void						getLastHostSearchLanguage( ESearchType searchType, ELanguageType& languageType );
    void						setLastHostSearchContentRating( ESearchType searchType, EContentRating contentRating );
    void						getLastHostSearchContentRating( ESearchType searchType, EContentRating& contentRating );

	void						setCamSourceId( uint32_t camId );
	uint32_t					getCamSourceId( void );

	void						setCamShowPreview( bool showPreview );
	bool						getCamShowPreview( void );

	void						setCamRotation( uint32_t camId, uint32_t camRotation );
	uint32_t					getCamRotation( uint32_t camId );

	void						setVidFeedRotation( uint32_t feedRotation );
	uint32_t					getVidFeedRotation( void );

    void						setLastAppletLaunched( EApplet applet );
    EApplet						getLastAppletLaunched( void );

    void						setLastUsedTestUrl( std::string& testUrl );
    void						getLastUsedTestUrl( std::string& testUrl );

    void                        setVerboseLog( bool verbose );
    bool                        getVerboseLog( void );

    void                        setLogLevels( uint32_t logLevelFlags );
    uint32_t                    getLogLevels( void );

    void                        setLogModules( uint32_t logModuleFlags );
    uint32_t                    getLogModules( void );

	void						setFavoriteHostGroupUrl( std::string& hostUrl );
	void						getFavoriteHostGroupUrl( std::string& hostUrl );

	void						setSoundInDeviceIndex( int32_t deviceIndex );
	int32_t						getSoundInDeviceIndex( void );

	void						setSoundOutDeviceIndex( int32_t deviceIndex );
	int32_t						getSoundOutDeviceIndex( void );

protected:
	bool						appModeSettingsAreInitialize( void );
	void						setupAppModeSettings( EDefaultAppMode appDefaultMode );
    std::string                 getAppendedType( const char * key, ESearchType searchType );

	std::string					m_AppShortName;
	// caching of feature settings
	bool						m_AppFeatureIsCached[ eMaxAppFeatures ];
	bool						m_AppFeatureValue[eMaxAppFeatures];
};

