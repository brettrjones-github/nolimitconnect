#pragma once
#include <AppDependLibrariesConfig.h>
#ifdef TARGET_OS_WINDOWS
# include "config_libvpx_win32.h"
#else
# include "config_libvpx_linux.h"
#endif // TARGET_OS_WINDOWS

// define to build apps with main
//#define BUILD_VPC_ENCODER_APP		1
//#define BUILD_VPC_DECODER_APP		1
