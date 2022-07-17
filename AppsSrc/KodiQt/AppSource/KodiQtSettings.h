#pragma once

#include <CommonSrc/QtSource/AppSettings.h>

class NlcPlayerSettings : public AppSettings
{
public:
	NlcPlayerSettings();
	virtual ~NlcPlayerSettings();

	bool						playerSettingsStartup( const char * dbSettingsFile );
};

