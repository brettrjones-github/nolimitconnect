#pragma once

#include <CommonSrc/QtSource/AppSettings.h>

class GoTvPlayerSettings : public AppSettings
{
public:
	GoTvPlayerSettings();
	virtual ~GoTvPlayerSettings();

	bool						playerSettingsStartup( const char * dbSettingsFile );
};

