#pragma once

#include <AppDependLibrariesConfig.h>

#if defined(TARGET_OS_WINDOWS)
# define WEBRTC_WIN
#else
# define WEBRTC_POSIX
#endif // defined(TARGET_OS_WINDOWS)

#define WEBRTC_APM_DEBUG_DUMP 0

