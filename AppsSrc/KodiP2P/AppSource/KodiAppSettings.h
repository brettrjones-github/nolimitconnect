#pragma once

#include <GoTvCommon/QtSource/AppSettings.h>

class KodiAppSettings : public AppSettings
{
public:
	KodiAppSettings();
	virtual ~KodiAppSettings();

	bool						playerSettingsStartup( const char * dbSettingsFile );
};

