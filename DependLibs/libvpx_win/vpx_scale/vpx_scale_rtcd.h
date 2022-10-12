#pragma once
#include <NlcDependLibrariesConfig.h>

#ifdef TARGET_OS_WINDOWS
# include <libvpx/vpx_scale_rtcd.h>
#else
# include <libvpx/config/linux/generic/vpx_scale_rtcd.h>
#endif // TARGET_OS_WINDOWS
