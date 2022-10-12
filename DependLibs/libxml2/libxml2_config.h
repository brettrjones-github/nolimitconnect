#ifndef LIBXML2_CONFIG_H
#define LIBXML2_CONFIG_H

#include <NlcDependLibrariesConfig.h>

/* Version number of package */
#define VERSION "2.7.6"
/* Name of package */
#define PACKAGE "libxml2"


#ifdef TARGET_OS_WINDOWS
# include "libxml2_config_windows.h"
#elif TARGET_OS_ANDROID
# include "libxml2_config_linux.h"
#elif TARGET_OS_LINUX
# include "libxml2_config_linux.h"
#else
echo error libxml2 no os defined
#endif // TARGET_OS_WINDOWS

#endif // LIBXML2_CONFIG_H
