#pragma once
#include <NlcDependLibrariesConfig.h>
#ifdef TARGET_OS_WINDOWS
# include <libvpx/vp8_rtcd.h>
#else
# include <libvpx/config/linux/generic/vp8_rtcd.h>
#endif // TARGET_OS_WINDOWS
