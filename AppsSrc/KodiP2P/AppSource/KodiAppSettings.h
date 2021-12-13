#pragma once

#include <CommonSrc/QtSource/AppSettings.h>

class KodiAppSettings : public AppSettings
{
public:
	KodiAppSettings();
	virtual ~KodiAppSettings();

	bool						playerSettingsStartup( const char * dbSettingsFile );
};

