#pragma once
#include <AppDependLibrariesConfig.h>

#ifdef TARGET_OS_WINDOWS
# include <libvpx/vpx_dsp_rtcd.h>
#else
# include <libvpx/config/linux/generic/vpx_dsp_rtcd.h>
#endif // TARGET_OS_WINDOWS
